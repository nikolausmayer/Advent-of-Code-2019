#!/bin/bash

set -eu;

## Check if the required data is here
if test ! -d COM; then
  echo "No COM found, will use part-1.sh to generate...";
  bash ../part-1.sh "$1";
  echo "done.";
fi

## Full orbit paths to $COM
YOU=`find COM -name YOU`;
SAN=`find COM -name SAN`;

## Eliminate common prefix of $YOU and $SANTA (stepwise)
while `:`; do
  if test `echo $YOU | cut -d"/" -f1` = `echo $SAN | cut -d"/" -f1`; then
    YOU=`echo $YOU | cut -d"/" -f2-`;
    SAN=`echo $SAN | cut -d"/" -f2-`;
  else
    break;
  fi
done

## The remaining number or orbits is the number of required transfers
echo $YOU $SAN | sed 's/[^/]//g' | wc -c | awk '{print $1 - 1}';


