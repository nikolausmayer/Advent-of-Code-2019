#!/bin/bash


cat "$1" \
| sed 's/[←↑↓→]/_/' \
| sed 's/X/O/' \
> mapcopy;

W=`head -n1 mapcopy | sed 's/[^ ]//g' | wc -c`;
H=`cat mapcopy | wc -l`;

## Find start point
y=1; 
while test $y -le $H; do
  if test `head -n$y mapcopy | tail -n1 | grep -c "O"` -eq 1; then
    break;
  fi
  y=`expr $y + 1`;
done

x=1; 
while test $x -le $W; do
  if test `head -n$y mapcopy | tail -n1 | cut -d' ' -f$x` = "O"; then
    break;
  fi
  x=`expr $x + 1`;
done

echo "start at $x $y";

echo $x $y > heads;
echo $x,$y,O > tiles;

y=1; 
while test $y -le $H; do
  x=1; 
  while test $x -le $W; do
    if test `head -n$y mapcopy | tail -n1 | cut -d' ' -f$x` = "_"; then
      echo $x,$y,_ >> tiles;
    fi
    x=`expr $x + 1`;
  done
  y=`expr $y + 1`;
done


GET () {
  if test `grep -c "^$1,$2," tiles` -eq 0; then
    echo "?";
  else
    grep "^$1,$2," tiles | tail -n1 | cut -d',' -f3;
  fi
}


time=0;
while test `wc -l heads | cut -d' ' -f1` -gt 0; do
  time=`expr $time + 1`;

  echo -n "" > candidates;
  cat heads | while read hx hy; do
    deadend=1;

    echo "head: $hx $hy";
    ## North
    tx=$hx;
    ty=`expr $hy - 1`;
    if test `GET $tx $ty` = "_"; then
      echo "  $tx $ty N";
      echo $tx $ty >> candidates;
      deadend=0;
    fi
    ## East
    tx=`expr $hx + 1`;
    ty=$hy;
    if test `GET $tx $ty` = "_"; then
      echo "  $tx $ty E";
      echo $tx $ty >> candidates;
      deadend=0;
    fi
    ## South
    tx=$hx;
    ty=`expr $hy + 1`;
    if test `GET $tx $ty` = "_"; then
      echo "  $tx $ty S";
      echo $tx $ty >> candidates;
      deadend=0;
    fi
    ## West
    tx=`expr $hx - 1`;
    ty=$hy;
    if test `GET $tx $ty` = "_"; then
      echo "  $tx $ty W";
      echo $tx $ty >> candidates;
      deadend=0;
    fi

    if test $deadend -eq 1; then
      echo "  this head is a dead end";
    fi
  done

  echo "$time min: `wc -l candidates | cut -d' ' -f1` new tiles";

  sort candidates | uniq > heads;

  cat heads | while read hx hy; do
    echo $hx,$hy,O >> tiles;
  done

  #if test $time -gt 10; then 
  #  exit;
  #fi
done

echo "Total time: `expr $time - 1`";

