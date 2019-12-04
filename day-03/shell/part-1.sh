#!/bin/bash

pos1="_";
pos2="0,0=";

while read line; do
  x=0;
  y=0;

  while read cmd; do
    echo $cmd;
    dir=`echo $cmd | head -c1`;
    count=`echo $cmd | tail -c+2`;
    i=0;
    while test $i -lt $count; do
      case $dir in
        U) dx=0;  dy=1;;
        D) dx=0;  dy=-1;;
        L) dx=-1; dy=0;;
        R) dx=1;  dy=0;;
      esac
      x=`expr $x + $dx`;
      y=`expr $y + $dy`;
      pos2="${pos2}_$x,$y=";
      i=`expr $i + 1`;
    done
  done <<EOF
`echo $line | tr "," "\n"`
EOF

  if test $pos1 = "_"; then
    pos1=$pos2;
    pos2="0,0=";
  fi
done < "$1";

mindist=9999;
while read pos; do
  if test `echo $pos1 | grep -c "_$pos"` -gt 0; then
    x=`echo $pos | cut -d"," -f1`;
    y=`echo $pos | cut -d"," -f2 | cut -d"=" -f1`;
    if test $x -lt 0; then
      x=`echo $x | tail -c+2`;
    fi
    if test $y -lt 0; then
      y=`echo $y | tail -c+2`;
    fi
    dist=`expr $x + $y`;
    if test $dist -lt $mindist; then
      mindist=$dist;
    fi
  fi
done <<EOF
`echo $pos2 | tr "_" "\n"`
EOF

echo "Minimal distance is:" $mindist;


exit `:`;

