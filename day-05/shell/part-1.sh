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


workline=`cat $1`;
HEAD=1;

## 1202 program alarm
#workline=`REASSEMBLE $workline 1 12`;
#workline=`REASSEMBLE $workline 2 02`;

while :; do
  echo "";
  #echo $workline
  opcode=`echo $workline | cut -d"," -f$HEAD | tail -c3`;
  param_modes=`echo $workline | cut -d"," -f$HEAD | head -c-3`;
  echo $param_modes $opcode;
  opcode=`expr $opcode + 0`;
  param_modes=`printf "%03d" $param_modes`;
  echo $param_modes $opcode;
  
  case $opcode in
    1)
      A=`expr $HEAD + 1`;
      B=`expr $HEAD + 2`;
      C=`expr $HEAD + 3`;
      A=`GET $workline $A`;
      B=`GET $workline $B`;
      C=`GET $workline $C`;
      echo $param_modes$opcode,$A,$B,$C

      ## C (output) is always in position mode
      if test `echo $param_modes | tail -c2` -eq 0; then
        A=`expr $A + 1`;
        A=`GET $workline $A`;
        echo first position mode
      else 
        echo first immediate mode
      fi
      if test `echo $param_modes | tail -c3 | head -c1` -eq 0; then
        B=`expr $B + 1`;
        B=`GET $workline $B`;
        echo second position mode
      else
        echo second immediate mode
      fi

      opresult=`expr $A + $B` || `:`;
      echo $C := $A + $B = $opresult

      workline=`REASSEMBLE $workline $C $opresult`;
      HEAD=`expr $HEAD + 4`;
      ;;
    2)
      A=`expr $HEAD + 1`;
      B=`expr $HEAD + 2`;
      C=`expr $HEAD + 3`;
      A=`GET $workline $A`;
      B=`GET $workline $B`;
      C=`GET $workline $C`;
      echo $param_modes$opcode,$A,$B,$C

      ## C (output) is always in position mode
      if test `echo $param_modes | tail -c2` -eq 0; then
        A=`expr $A + 1`;
        A=`GET $workline $A`;
        echo first position mode
      else 
        echo first immediate mode
      fi
      if test `echo $param_modes | tail -c3 | head -c1` -eq 0; then
        B=`expr $B + 1`;
        B=`GET $workline $B`;
        echo second position mode
      else
        echo second immediate mode
      fi

      opresult=`expr $A \* $B` || `:`;
      echo $C := $A \* $B = $opresult

      workline=`REASSEMBLE $workline $C $opresult`;
      HEAD=`expr $HEAD + 4`;
      ;;
    3)
      A=`expr $HEAD + 1`;
      A=`GET $workline $A`;
      echo $param_modes$opcode,$A

      read -p "USER INPUT PLEASE: " INPUT;

      echo $A := $INPUT;
      workline=`REASSEMBLE $workline $A $INPUT`;
      HEAD=`expr $HEAD + 2`;
      ;;
    4)
      A=`expr $HEAD + 1`;
      OUTPUT=`GET $workline $A`;
      echo $param_modes$opcode,$OUTPUT

      if test `echo $param_modes | tail -c2` -eq 0; then
        OUTPUT=`expr $OUTPUT + 1`;
        OUTPUT=`GET $workline $OUTPUT`;
        echo first position mode
      else 
        echo first immediate mode
      fi

      echo "Output: $OUTPUT";
      HEAD=`expr $HEAD + 2`;
      ;;
    99) 
      echo "HALT";
      break;
      ;;
    [?]) echo "WRONG OPCODE: $opcode";;
  esac
done

echo "";
echo "Result: `GET $workline 1`";

