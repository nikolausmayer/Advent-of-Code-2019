#!/bin/bash

set -eu;

INTCODE=intcode-silent.sh;

mkfifo mypipe-5to1;
mkfifo mypipe-1to2;
mkfifo mypipe-2to3;
mkfifo mypipe-3to4;
mkfifo mypipe-4to5;


maxscore=0;

A=5;
while test $A -le 9; do
  B=5;
  while test $B -le 9; do
    C=5;
    while test $C -le 9; do
      D=5;
      while test $D -le 9; do
        E=5;
        while test $E -le 9; do
          #echo $A$B$C$D$E;

          ## Check that all indices are unique
          unique=`echo $A,$B,$C,$D,$E  \
                  | tr ',' '\n'        \
                  | sort               \
                  | tr '\n' ','        \
                  | sed 's/,//g'       \
                  | sed -E 's/(.)\1//g'\
                  | wc -c`;
          if test $unique -eq 5; then
            #echo "==" $A$B$C$D$E;
            ## Run amplifier cascade
            bash $INTCODE "$1" mypipe-5to1 mypipe-1to2 &
            sub1=$!;
            echo $A > mypipe-5to1;

            bash $INTCODE "$1" mypipe-1to2 mypipe-2to3 &
            sub2=$!;
            sleep 0.1;
            echo $B > mypipe-1to2;

            bash $INTCODE "$1" mypipe-2to3 mypipe-3to4 &
            sub3=$!;
            sleep 0.1;
            echo $C > mypipe-2to3;

            bash $INTCODE "$1" mypipe-3to4 mypipe-4to5 &
            sub4=$!;
            sleep 0.1;
            echo $D > mypipe-3to4;

            bash $INTCODE "$1" mypipe-4to5 mypipe-5to1 &
            sub5=$!;
            sleep 0.1;
            echo $E > mypipe-4to5;

            ## Initial signal
            echo 0  > mypipe-5to1;

            wait $sub1;
            #echo "A EXIT";
            wait $sub2;
            #echo "B EXIT";
            wait $sub3;
            #echo "C EXIT";
            wait $sub4;
            #echo "D EXIT";
            result=`cat < mypipe-5to1`;
            wait $sub5;

            if test $result -gt $maxscore; then
              echo "New maximum: $A$B$C$D$E yields $result";
              maxscore=$result;
            fi
          fi

          E=`expr $E + 1`;
        done
        D=`expr $D + 1`;
      done
      C=`expr $C + 1`;
    done
    B=`expr $B + 1`;
  done
  A=`expr $A + 1`;
done

echo "Maximum score: $maxscore";



rm ./mypipe*;


