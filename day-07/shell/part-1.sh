#!/bin/bash

set -eu;

INTCODE=intcode-silent.sh;

mkfifo mypipe;
mkfifo mypipe2;


maxscore=0;

A=0;
while test $A -le 4; do
  B=0;
  while test $B -le 4; do
    C=0;
    while test $C -le 4; do
      D=0;
      while test $D -le 4; do
        E=0;
        while test $E -le 4; do
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
            bash $INTCODE "$1" mypipe mypipe2 &
            sub=$!;
            echo $A > mypipe;
            sleep 0.1;
            echo 0  > mypipe;
            sleep 0.1;
            result=`cat < mypipe2`;
            wait $sub;

            bash $INTCODE "$1" mypipe mypipe2 &
            sub=$!;
            sleep 0.1;
            echo $B > mypipe;
            sleep 0.1;
            echo $result > mypipe;
            result=`cat < mypipe2`;
            wait $sub;

            bash $INTCODE "$1" mypipe mypipe2 &
            sub=$!;
            sleep 0.1;
            echo $C > mypipe;
            sleep 0.1;
            echo $result > mypipe;
            result=`cat < mypipe2`;
            wait $sub;

            bash $INTCODE "$1" mypipe mypipe2 &
            sub=$!;
            sleep 0.1;
            echo $D > mypipe;
            sleep 0.1;
            echo $result > mypipe;
            result=`cat < mypipe2`;
            wait $sub;

            bash $INTCODE "$1" mypipe mypipe2 &
            sub=$!;
            sleep 0.1;
            echo $E > mypipe;
            sleep 0.1;
            echo $result > mypipe;
            result=`cat < mypipe2`;
            wait $sub;

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



rm mypipe mypipe2;


