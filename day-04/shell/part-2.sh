#!/bin/bash

input=`cat "$1"`;
start=`echo $input | cut -d"-" -f1`;
end=`echo $input | cut -d"-" -f2`;

i=$start;
valid=0;

d1=1;
while test $d1 -le 5; do
  d2=$d1;
  while test $d2 -le 9; do
    d3=$d2;
    while test $d3 -le 9; do
      d4=$d3;
      while test $d4 -le 9; do
        d5=$d4;
        while test $d5 -le 9; do
          d6=$d5;
          while test $d6 -le 9; do

            num=$d1$d2$d3$d4$d5$d6;
            num2=`echo $num \
                  | sed -E 's/([[:digit:]])\1{5}/------/g' \
                  | sed -E 's/([[:digit:]])\1{4}/-----/g' \
                  | sed -E 's/([[:digit:]])\1{3}/----/g' \
                  | sed -E 's/([[:digit:]])\1{2}/---/g' \
                  | sed -E 's/([[:digit:]])\1{1}/X/g'`;

            if test $num -ge $start -a $num -le $end; then
              if test `echo $num2 | grep X`; then
                echo $num;
                valid=`expr $valid + 1`;
              fi
            fi

            d6=`expr $d6 + 1`;
          done
          d5=`expr $d5 + 1`;
        done
        d4=`expr $d4 + 1`;
      done
      d3=`expr $d3 + 1`;
    done
    d2=`expr $d2 + 1`;
  done
  d1=`expr $d1 + 1`;
done

echo "Valid: " $valid;

