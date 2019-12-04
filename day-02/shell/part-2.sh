#!/bin/bash

set -eu

GET () {
  echo $1 | cut -d"," -f$2;
}

REASSEMBLE () {
  to=$2;
  from=`expr $2 + 2`;
  B=`echo $1 | cut -d"," -f${from}-`;
  if test $2 -gt 0; then
    A=`echo $1 | cut -d"," -f-${to}`;
    echo $A,$3,$B;
  else
    echo $3,$B;
  fi
}

RUN () {
  workline=`cat $1`;
  HEAD=1;

  ## 1202 program alarm
  workline=`REASSEMBLE $workline 1 $2`;
  workline=`REASSEMBLE $workline 2 $3`;

  while :; do
    #echo $workline;
    opcode=`echo $workline | cut -d"," -f$HEAD`;
    case $opcode in
      1)
        A=`expr $HEAD + 1`;
        B=`expr $HEAD + 2`;
        C=`expr $HEAD + 3`;
        A=`GET $workline $A`;
        B=`GET $workline $B`;
        C=`GET $workline $C`;

        A=`expr $A + 1`;
        B=`expr $B + 1`;
        #C=`expr $C + 1`;
        A=`GET $workline $A`;
        B=`GET $workline $B`;
        #C=`GET $workline $C`;

        opresult=`expr $A + $B`;
        #echo $C := $A + $B = $opresult

        workline=`REASSEMBLE $workline $C $opresult`;
        ;;
      2)
        A=`expr $HEAD + 1`;
        B=`expr $HEAD + 2`;
        C=`expr $HEAD + 3`;
        A=`GET $workline $A`;
        B=`GET $workline $B`;
        C=`GET $workline $C`;

        A=`expr $A + 1`;
        B=`expr $B + 1`;
        #C=`expr $C + 1`;
        A=`GET $workline $A`;
        B=`GET $workline $B`;
        #C=`GET $workline $C`;

        opresult=`expr $A \* $B`;
        #echo $C := $A \* $B = $opresult

        workline=`REASSEMBLE $workline $C $opresult`;
        ;;
      99) break;;
      [?]) echo "WRONG OPCODE: $opcode";;
    esac
    HEAD=`expr $HEAD + 4`;
  done

  GET $workline 1;
}


noun=0;
while test $noun -le 99; do
  verb=0;
  while test $verb -le 99; do
    result=`RUN $1 $noun $verb`;
    echo $verb $noun : $result;

    if test $result -lt `expr 19690720 - 100`; then
      break;
    fi
    if test $result -gt `expr 19690720 + 100`; then
      break;
    fi

    if test $result -eq 19690720; then
      echo `expr \( 100 \* $noun \) + $verb`;
      exit `:`;
    fi
    verb=`expr $verb + 1`;
  done
  noun=`expr $noun + 1`;
done

