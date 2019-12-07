#!/bin/bash

set -eu;


mkdir -p COM;

stack="COM|";

while test $stack != "|"; do
  #echo "";
  #echo Stack: $stack;

  center=`echo $stack | cut -d"|" -f1`;

  grep "$center)" $1 > next_lines || `:`;

  found_center=`find COM -name $center`;

  if test `wc -l next_lines | cut -d" " -f1` -gt 0; then
    while read line; do
      orbiter=`echo $line | cut -d')' -f2`;
      #echo Line: $line = $found_center $orbiter;
      mkdir "$found_center/$orbiter";
      stack="$stack$orbiter|";
      #echo $stack;
    done < next_lines;
  fi;

  stack=`echo $stack | cut -d'|' -f2-`;
  #echo $stack;

  if test `echo $stack | wc -c` -eq 1; then
    #echo "DONE";
    break;
  fi

done;

find COM | tr '\n' ',' | sed 's/[^/]//g' | wc -c;

rm next_lines;
#rm -r COM;

