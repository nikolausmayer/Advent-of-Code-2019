#!/bin/bash

#set -eu;

mkfifo pipe_to_brain;
mkfifo pipe_from_brain;

X=0;
Y=0;
DIR=0;
TURN_LEFT () {
  DIR=`expr $DIR - 1`;
  if test $DIR -lt 0; then
    DIR=`expr $DIR + 4`;
  elif test $DIR -gt 3; then
    DIR=`expr $DIR - 4`;
  fi
}
TURN_RIGHT () {
  DIR=`expr $DIR + 1`;
  if test $DIR -lt 0; then
    DIR=`expr $DIR + 4`;
  elif test $DIR -gt 3; then
    DIR=`expr $DIR - 4`;
  fi
}
MOVE () {
  if   test $DIR -eq 0; then  ## North
    Y=`expr $Y - 1`;
  elif test $DIR -eq 1; then  ## East
    X=`expr $X + 1`;
  elif test $DIR -eq 2; then  ## South
    Y=`expr $Y + 1`;
  elif test $DIR -eq 3; then  ## West
    X=`expr $X - 1`;
  fi
}



echo "Starting robot brain...";
bash intcode.sh "$1" pipe_to_brain pipe_from_brain >/dev/null &
sub=$!;
sleep 0.1;
echo "" > painted.txt;

while `:`; do
  color_on_field=0;
  if test `grep -c -- "^$X,$Y " painted.txt || true` -gt 0; then
    color_on_field=`grep -- "^$X,$Y " painted.txt \
                    | tail -n1 \
                    | cut -d' ' -f2`;
  fi
  echo "$X,$Y is $color_on_field";
  echo $color_on_field > pipe_to_brain;

  color_to_paint=`cat pipe_from_brain`;
  echo "$X,$Y $color_to_paint" >> painted.txt;

  turning_instruction=`cat pipe_from_brain`;
  echo $color_to_paint $turning_instruction;
  if test $turning_instruction -eq 0; then
    TURN_LEFT;
  else
    TURN_RIGHT;
  fi
  MOVE;
done


CLEANUP () {
  wait $sub;
  rm pipe_to_brain pipe_from_brain;
}
trap CLEANUP SIGINT;
CLEANUP;

