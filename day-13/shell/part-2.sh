#!/bin/bash

mkfifo pipe_to_brain;
mkfifo pipe_from_brain;


echo -n "" > canvas.txt;


echo "Starting arcade...";
bash intcode.sh "$1" pipe_to_brain pipe_from_brain >/dev/null &
sub=$!;
sleep 0.1;


while `:`; do
  while `:`; do
    tilex=`cat pipe_from_brain`;
    tiley=`cat pipe_from_brain`;
    tileid=`cat pipe_from_brain`;

    if test $tilex -eq -2; then
      echo "GOT BREAK";
      break;
    fi

    echo "$tilex,$tiley $tileid" >> canvas.txt;

    ## TODO refresh canvas when input is read
    ## TODO find ball position on canvas
    ## TODO find paddle position on canvas
    ## TODO move joystick to keep paddle at X position of ball
    ##      -1 0 1
    ##      <- o ->
    ## TODO loop until no more "block" tiles
    ## TODO read scores
  done

  paddle_x=`grep " 3$" canvas.txt | tail -n1 | cut -d',' -f1`;
  ball_x=`grep " 4$" canvas.txt | tail -n1 | cut -d',' -f1`;
  if   test $ball_x -lt $paddle_x; then
    echo "paddle at $paddle_x, ball at $ball_x (LEFT)";
    echo "-1" > pipe_to_brain;
  elif test $ball_x -eq $paddle_x; then
    echo "paddle at $paddle_x, ball at $ball_x (STAY)";
    echo "0" > pipe_to_brain;
  else
    echo "paddle at $paddle_x, ball at $ball_x (RIGHT)";
    echo "1" > pipe_to_brain;
  fi

  score=`grep "^-1,0" canvas.txt | tail -n1 | cut -d' ' -f2`;
  echo "current score: $score";
done


CLEANUP () {
  wait $sub;
  rm pipe_to_brain pipe_from_brain;
}
trap CLEANUP SIGINT;
CLEANUP;

