#!/bin/bash

mkfifo pipe_to_brain;
mkfifo pipe_from_brain;


echo "Starting robot brain...";
bash intcode.sh "$1" pipe_to_brain pipe_from_brain >/dev/null &
sub=$!;
sleep 0.1;


while `:`; do
  tilex=`cat pipe_from_brain`;
  tiley=`cat pipe_from_brain`;
  tileid=`cat pipe_from_brain`;

  echo "$tilex,$tiley $tileid" >> canvas.txt;
done


CLEANUP () {
  wait $sub;
  rm pipe_to_brain pipe_from_brain;
}
trap CLEANUP SIGINT;
CLEANUP;

