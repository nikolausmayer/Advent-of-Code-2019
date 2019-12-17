#!/bin/bash

if test ! -f painted.txt; then
  bash part-1.sh "$1";
fi

minx=`cut -d',' -f1 painted.txt | sort -g | head -n1`;
maxx=`cut -d',' -f1 painted.txt | sort -g | tail -n1`;
miny=`cut -d',' -f2 painted.txt | cut -d' ' -f1 | sort -g | head -n1`;
maxy=`cut -d',' -f2 painted.txt | cut -d' ' -f1 | sort -g | tail -n1`;

y=$miny;
while test $y -le $maxy; do
  x=$minx;
  line="";
  while test $x -le $maxx; do
    if test `grep -c "^$x,$y " painted.txt` -eq 0; then
      pixel=0;
    else
      pixel=`grep "^$x,$y " painted.txt | cut -d' ' -f2`;
    fi

    if test $pixel -eq 0; then
      line="${line}_";
    else
      line="${line}#";
    fi

    x=`expr $x + 1`;
  done
  echo $line;
  y=`expr $y + 1`;
done

