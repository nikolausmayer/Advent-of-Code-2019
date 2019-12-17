#!/bin/bash

## Read puzzle input
data=`cat "$1"`;

echo "$data";
echo "";

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

y=1;
while test $y -le $H; do
  x=1;
  while test $x -le $W; do

    if test `GET $x $y $data` = "#"; then
      echo "";
      echo "$x $y ASTEROID START";
      score=0;
      canvas=$data;

      dyo=0;
      while test $dyo -lt $H; do
        dxo=0;
        while test $dxo -lt $W; do

          dyf=-1;
          while test $dyf -le 1; do
          dxf=-1;
          while test $dxf -le 1; do
          dy=`expr $dyo \* $dyf`;
          dx=`expr $dxo \* $dxf`;

          multiplier=1;
          occluded=0;
          while `:`; do
            if test $dx -eq 0 -a $dy -eq 0; then
              break;
            fi

            xx=`expr $x + \( $dx \* $multiplier \)`;
            yy=`expr $y + \( $dy \* $multiplier \)`;
            if test $xx -le 0 -o $xx -gt $W -o \
                    $yy -le 0 -o $yy -gt $H; then
              break;
            fi

            if test `GET $xx $yy $canvas` = "#"; then
              if test $occluded -eq 0; then
                echo "d$dx d$dy -- found NEW asteroid at $xx,$yy";
                score=`expr $score + 1`;
              else
                echo "d$dx d$dy -- found asteroid (occluded) at $xx,$yy";
              fi
              canvas=`SET $xx $yy "o" "$canvas"`;
              occluded=1;
              echo $canvas;
            elif test `GET $xx $yy $canvas` = "o"; then
              echo "d$dx d$dy -- found asteroid at $xx,$yy";
              occluded=1;
            fi

            multiplier=`expr $multiplier + 1`;
          done

          dxf=`expr $dxf + 2`;
          done
          dyf=`expr $dyf + 2`;
          done

          dxo=`expr $dxo + 1`;
        done
        dyo=`expr $dyo + 1`;
      done

      if test $score -gt $maximum_score; then
        maximum_score=$score;
        echo "New maximum score $maximum_score at $x,$y";
      else
        echo "  (score $score at $x,$y)";
      fi

    fi

    x=`expr $x + 1`;
  done
  y=`expr $y + 1`;
done

