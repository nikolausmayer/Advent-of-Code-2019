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

while read line; do
  workline=$line;
  HEAD=1;

  ## 1202 program alarm
  workline=`REASSEMBLE $workline 1 12`;
  workline=`REASSEMBLE $workline 2 02`;

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

  echo "Result: `GET $workline 1`";
done < "$1";

