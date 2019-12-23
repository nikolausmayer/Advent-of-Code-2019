#!/bin/bash

minx=`cut -d',' -f1 canvas.txt | sort -g | head -n1`;
maxx=`cut -d',' -f1 canvas.txt | sort -g | tail -n1`;
miny=`cut -d',' -f2 canvas.txt | sort -g | head -n1`;
maxy=`cut -d',' -f2 canvas.txt | sort -g | tail -n1`;
minx=`expr $minx - 2`;
maxx=`expr $maxx + 2`;
miny=`expr $miny - 2`;
maxy=`expr $maxy + 2`;
droidx=`tail -n1 actions.txt | cut -d',' -f1`;
droidy=`tail -n1 actions.txt | cut -d',' -f2`;
droiddir=`tail -n1 actions.txt | cut -d',' -f3`;
case $droiddir in
  1) droiddir="↑";;
  2) droiddir="↓";;
  3) droiddir="←";;
  4) droiddir="→";;
esac

# minx=`expr $droidx - 5`;
# maxx=`expr $droidx + 5`;
# miny=`expr $droidy - 5`;
# maxy=`expr $droidy + 5`;

# X=$minx;
# line="";
# while test $X -le $maxx; do
#   line="$line`printf "%2d" $X`";
#   X=`expr $X + 1`;
# done
# echo "   │$line";
# X=$minx;
# line="";
# while test $X -le $maxx; do
#   line="${line}──";
#   X=`expr $X + 1`;
# done
# echo "───┼$line";

Y=$miny;
while test $Y -le $maxy; do
  line="";
  X=$minx;
  while test $X -le $maxx; do
    if test $X -eq $droidx -a $Y -eq $droidy; then
      line="${line} ${droiddir}";
    elif test `grep -c "^$X,$Y," canvas.txt` -eq 0; then
      if test -f tile_plans/_${X}_${Y}_; then
        #planlength=`cat tile_plans/_${X}_${Y}_ | wc -l`;
        #line="${line}`printf "%2d" $planlength`";
        line="${line} P";
      else
        line="${line} ?";
      fi
    else
      tile=`grep "^$X,$Y," canvas.txt | tail -n1 | cut -d',' -f3`;
      line="${line} ${tile}";
    fi
    X=`expr $X + 1`;
  done
  # echo "`printf %3d $Y`│$line";
  echo $line;
  Y=`expr $Y + 1`;
done

