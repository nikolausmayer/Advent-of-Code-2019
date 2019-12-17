#!/bin/bash

refx=40;
refy=42;

if test -f tmp.txt; then
  rm tmp.txt;
fi

RAW_ATAN_APPROX () {
  if test $1 -eq 0; then
    ## Edge case
    echo 0;
  else
    ## Make absolutes of arguments
    if test $1 -gt 0; then
      ax=$1;
    else
      ax=`expr $1 \* -1`;
    fi

    if test $2 -gt 0; then
      ay=$2;
    else
      ay=`expr $2 \* -1`;
    fi

    ## Compute approximation to ATAN2
    ## "Efficient approximations for the arctanget function" Rajan 2006
    PI_4=0.78539816;
    echo "scale=9; $PI_4 * ($ay/$ax) - ($ay/$ax)*(($ay/$ax)-1) * (0.2447 + 0.0663*($ay/$ax))" | bc;
  fi
}

RAW_ATAN () {
  if test $1 -eq 0; then
    ## Edge case
    echo 0;
  else
    ## Make absolutes of arguments
    if test $1 -gt 0; then
      ax=$1;
    else
      ax=`expr $1 \* -1`;
    fi

    if test $2 -gt 0; then
      ay=$2;
    else
      ay=`expr $2 \* -1`;
    fi

    echo "scale=9; a($ay/$ax)" | bc -l;
  fi
}

ATAN () {
  #alpha=`RAW_ATAN_APPROX $1 $2`;
  alpha=`RAW_ATAN $1 $2`;
  PI_4=0.78539816;

  ##           3pi/4               |          0 | 2pi
  ##             |                 |            |    
  ##             |                 |            |    
  ##             |                 |            |    
  ##             |                 |            |    
  ##  pi --------+-------- 0 | 2pi | 3pi/4 -----+--------  pi/2
  ##             |                 |            |         
  ##             |                 |            |         
  ##             |   DOMAIN        |            | 
  ##             |                 |            |         
  ##            pi/2               |           pi         

  ## Use actual signs of arguments
  ## And rotate so that 0 is "up"
  if   test $1 -eq 0; then
    if   test $2 -eq 0; then
      echo "ERR" >&2;
      exit `:`;
    elif test $2 -gt 0; then
      echo "scale=9; 4*$PI_4" | bc;
    else
      echo "0";
    fi
  elif test $1 -gt 0; then
    if   test $2 -eq 0; then
      echo "scale=9; 2*$PI_4" | bc;
    elif test $2 -gt 0; then
      echo "scale=9; $alpha + 2*$PI_4" | bc;
    else
      echo "scale=9; (2*$PI_4 - $alpha)" | bc;
    fi
  else
    if   test $2 -eq 0; then
      echo "scale=9; 6*$PI_4" | bc;
    elif test $2 -gt 0; then
      echo "scale=9; (2*$PI_4 - $alpha)+2*$PI_4" | bc;
    else
      echo "scale=9; $alpha + 3*2*$PI_4" | bc;
    fi
  fi
}




## Read puzzle input
data=`cat "$1"`;

## Dimensions of map
H=`echo $data | sed 's/[^ ]//g' | wc -c`;
W=`echo -n $data | cut -d' ' -f1 | wc -c`;
W=`expr $W - 1`;

data=`echo $data | sed 's/ //g'`;

## Get an (x,y) entry from map
GET () {
  index=`expr \( \( $2 - 1 \) \* $W \) + $1`;
  echo -n $3 | head -c$index | tail -c1;
  #echo -n $data | cut -d' ' -f$2 | head -c$1 | tail -c1;
}



## Compute, sort, and uniquelify asteroid angles
y=1;
while test $y -le $H; do
  x=1;
  while test $x -le $W; do
    xx=`expr $x - $refx`;
    yy=`expr $y - $refy`;

    if test $xx -ne 0 -o $yy -ne 0; then
      if test `GET $x $y $data` = "#"; then
        echo `ATAN $xx $yy` `expr $xx \* $xx + $yy \* $yy` $xx $yy >> tmp.txt;
      fi
    fi

    x=`expr $x + 1`;
  done
  y=`expr $y + 1`;
done

## Clockwise asteroids
sed -i 's/^\./0./' tmp.txt;
cut -d' ' -f1 tmp.txt \
| sort -g \
| uniq \
| while read f; do 
    grep "^$f " tmp.txt \
    | cut -d' ' -f2 \
    | sort -g \
    | while read g; do
        grep "^$f $g " tmp.txt;
      done
  done > sorted.txt;
cp sorted.txt sorted-cp.txt

## unique asteroid angles (clockwise)
cut -d' ' -f1 tmp.txt \
| sort -g \
| uniq > angles.txt;
sed 's/^\./0./' angles.txt | sort -g > angles-cp.txt;



## Set an (x,y) entry in a map copy
SET () {
  text=$4;
  cut_to=`expr \( \( $2 - 1 \) \* $W \) + $1 - 1`;
  cut_from=`expr \( \( $2 - 1 \) \* $W \) + $1 + 1`;

  if test $cut_to -lt 0; then
    prefix="";
  else
    prefix=`echo $text | head -c$cut_to`;
  fi

  ## TODO "-ge" ?
  if test $cut_from -ge `echo $text | wc -c`; then
    suffix="";
  else
    suffix=`echo $text | tail -c+$cut_from`;
  fi

  #echo $1 $2 >&2;
  #echo $prefix >&2;
  #echo $3 >&2;
  #echo $suffix >&2;
  #echo $text >&2;
  echo "$prefix$3$suffix";
}

maximum_score=0;

## !! Top left should be (0,0) via instructions

y=$refy;
x=$refx;

canvas=$data;

asteroid_index=0;
angle_index=1;
num_angles=`wc -l angles-cp.txt | cut -d' ' -f1`;
while test $asteroid_index -lt 200; do
  if test `wc -l sorted-cp.txt | cut -d' ' -f1` -eq 0; then
    break;
  fi

  next_angle=`head -n$angle_index angles-cp.txt | tail -n1`;
  if test `grep -c "^$next_angle " sorted-cp.txt` -gt 0; then
    next_asteroid=`grep "^$next_angle " sorted-cp.txt | head -n1`;

    grep -v "^$next_asteroid$" sorted-cp.txt > tmp.txt;
    mv tmp.txt sorted-cp.txt;

    asteroid_x=`echo $next_asteroid | cut -d' ' -f3`;
    asteroid_y=`echo $next_asteroid | cut -d' ' -f4`;
    asteroid_x=`expr $asteroid_x + $x`;
    asteroid_y=`expr $asteroid_y + $y`;
    echo $asteroid_index : $next_angle : $asteroid_x $asteroid_y;

    asteroid_index=`expr $asteroid_index + 1`;
  fi

  angle_index=`expr $angle_index % $num_angles + 1`;
done


