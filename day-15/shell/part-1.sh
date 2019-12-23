#!/bin/bash


TILE () {
  if test `grep -c "^$1,$2," canvas.txt` -eq 0; then
    echo "?";
  else
    grep "^$1,$2," canvas.txt | tail -n1 | cut -d',' -f3;
  fi
}

PLANCHECK () {
  if test ! -f "tile_plans/_${1}_${2}_"; then
    echo "  unknown tile at $1,$2";
    echo $3 >> "tile_plans/_${1}_${2}_";
  else
    echo "  new instant plan for $1,$2";
    echo $3 > "tile_plans/_${1}_${2}_";
  fi
}

PLANDELETE () {
  if test $1 -eq 0 -a $2 -eq 0; then
    return;
  fi

  if test -f "tile_plans/_${1}_${2}_"; then
    rm "tile_plans/_${1}_${2}_";
  fi
}

ADDSTEP () {
  #find tile_plans -type f -exec echo $1 >> {} \;
  newstep=$1;
  ls tile_plans | while read f; do
    laststep=`tail -n1 tile_plans/$f`;
    if test $laststep -eq $newstep; then
      head -n-1 tile_plans/$f > tmp;
      mv tmp tile_plans/$f;
    else
      case $newstep in
        1) reverse=2;;
        2) reverse=1;;
        3) reverse=4;;
        4) reverse=3;;
      esac
      echo $reverse >> tile_plans/$f;
    fi
  done
}

mkdir tile_plans;

mkfifo pipe_to_brain;
mkfifo pipe_from_brain;


echo "Starting repair droid...";
bash intcode.sh "$1" pipe_to_brain pipe_from_brain >/dev/null &
sub=$!;
sleep 0.1;

X=0;
Y=0;
echo "0,0,_" > canvas.txt;

## Initial plans for adjacent tiles
PLANCHECK  0 -1  1;
PLANCHECK  0  1  2;
PLANCHECK -1  0  3;
PLANCHECK  1  0  4;
## Special plan to track distance to initial tile
touch tile_plans/_0_0_;

echo "0,0,1" > actions.txt;
cmd=1;
echo "starting by moving N from 0,0";
echo $cmd > pipe_to_brain;

while `:`; do
  state=`cat pipe_from_brain`;

  ## update robot position and map
  case $state in
    0)  ## wall
      tX=$X;
      tY=$Y;
      case $cmd in
        1)
          tY=`expr $tY - 1`;
          ;;
        2)
          tY=`expr $tY + 1`;
          ;;
        3)
          tX=`expr $tX - 1`;
          ;;
        4)
          tX=`expr $tX + 1`;
          ;;
      esac
      echo "wall at $tX,$tY: staying at $X,$Y";
      echo "$tX,$tY,#" >> canvas.txt;
      PLANDELETE $tX $tY;
      ;;
    1)  ## empty
      case $cmd in
        1)
          Y=`expr $Y - 1`;
          ADDSTEP 1;
          ;;
        2)
          Y=`expr $Y + 1`;
          ADDSTEP 2;
          ;;
        3)
          X=`expr $X - 1`;
          ADDSTEP 3;
          ;;
        4)
          X=`expr $X + 1`;
          ADDSTEP 4;
          ;;
      esac
      echo "moved to $X,$Y";
      echo "$X,$Y,_" >> canvas.txt;
      PLANDELETE $X $Y;
      ;;
    2)  ## goal
      case $cmd in
        1)
          Y=`expr $Y - 1`;
          ADDSTEP 1;
          ;;
        2)
          Y=`expr $Y + 1`;
          ADDSTEP 2;
          ;;
        3)
          X=`expr $X - 1`;
          ADDSTEP 3;
          ;;
        4)
          X=`expr $X + 1`;
          ADDSTEP 4;
          ;;
      esac
      echo "ARRIVED AT OXYGEN SYSTEM: $X,$Y";
      echo "$X,$Y,X" >> canvas.txt;
      answer= `cat tile_plans/_0_0_ | wc -l`;
      PLANDELETE $X $Y;
      ;;
  esac


  ## plan next action
  ##
  ## always check for unknown adjacent tiles, and explore them.
  ##
  ## if no unexplored tile is adjacent, plan a path to another
  ## unexplored tile.

  tX=$X; tY=`expr $Y - 1`;
  if test `TILE $tX $tY` = "?"; then 
    PLANCHECK $tX $tY 1;
  fi
  tX=$X; tY=`expr $Y + 1`;
  if test `TILE $tX $tY` = "?"; then 
    PLANCHECK $tX $tY 2;
  fi
  tX=`expr $X - 1`; tY=$Y;
  if test `TILE $tX $tY` = "?"; then 
    PLANCHECK $tX $tY 3;
  fi
  tX=`expr $X + 1`; tY=$Y;
  if test `TILE $tX $tY` = "?"; then 
    PLANCHECK $tX $tY 4;
  fi

  ## Check whether only the special plan _0_0_ is left
  if test `ls tile_plans | wc -l` -eq 1; then
    echo "Map is fully explored.";
    break;
  fi

  ## plan towards the nearest tile
  shortest_plan_tile=`wc -l tile_plans/* \
                      | grep -v "_0_0_" \
                      | sed 's/^ *//' \
                      | sort -g \
                      | head -n1 \
                      | cut -d' ' -f2`;
  cmd=`tail -n1 $shortest_plan_tile`;
  case $cmd in
    1) dir="N";;
    2) dir="S";;
    3) dir="W";;
    4) dir="E";;
  esac
  echo "moving $dir towards $shortest_plan_tile";


  echo "$X,$Y,$cmd" >> actions.txt;

  ## n-s-w-e : 1-2-3-4

  echo $cmd > pipe_to_brain;
done


## Account for missing last map update
answer=`expr $answer + 1`;
echo "Final distance from start to oxygen system: $answer";

CLEANUP () {
  kill $sub;
  rm pipe_to_brain pipe_from_brain;
  #rm -r ./tile_plans;
}
trap CLEANUP SIGINT;
trap CLEANUP EXIT;

