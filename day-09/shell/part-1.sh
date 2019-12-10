#!/bin/bash

set -eu


workline=`cat $1`",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0";
HEAD=1;
last_output="";
relative_base=0;


#EXTEND_WORKLINE () {
#  extend_to=$1;
#  we_have=`echo $workline | sed 's/[^,]//g' | wc -c`;
#  extend_by=`expr $extend_to - $we_have`;
#  i=0;
#  extend_string="";
#  while test $i -lt $extend_by; do
#    extend_string=$extend_string,0;
#    i=`expr $i + 1`;
#  done;
#  workline=$workline$extend_string;
#}

GET () {
  res=`echo $workline | cut -d"," -f$1`;
  ## TODO extend workline
  if test `echo $res | wc -c` -lt 2; then
    #EXTEND_WORKLINE $1;
    #res=0;
    echo "NOT ENOUGH" >&2;
    exit `false`;
  fi
  echo $res;
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


GET_INPUT_PARAMETER () {
  mode=$1;
  A=$2;

  case $mode in
    0)
      A=`expr $A + 1`;
      A=`GET $A`;
      #echo position mode;
      ;;
    1)
      #echo immediate mode;
      ;;
    2)
      A=`expr $A + $relative_base + 1`;
      A=`GET $A`;
      #echo relative mode;
      ;;
  esac

  echo $A;
}

GET_INPUT_N () {
  param_modes=$1;
  n=$2;
  m=`expr $n + 1`;

  A=`expr $HEAD + $n`;
  A=`GET $A`;
  mode=`echo $param_modes | tail -c$m | head -c1`;
  A=`GET_INPUT_PARAMETER $mode $A`;

  echo $A;
}

GET_OUTPUT_N () {
  param_modes=$1;
  n=$2;
  m=`expr $n + 1`;

  C=`expr $HEAD + $n`;
  C=`GET $C`;

  mode=`echo $param_modes | tail -c$m | head -c1` ;
  if test $mode -eq 2; then
    C=`expr $C + $relative_base`;
  fi

  echo $C;
}


while :; do
  #sleep 1;
  echo "";
  echo $HEAD;
  echo $relative_base;
  #echo $workline
  opcode=`echo $workline | cut -d"," -f$HEAD | tail -c3`;
  param_modes=`echo $workline | cut -d"," -f$HEAD | head -c-3`;
  echo $param_modes $opcode;
  opcode=`expr $opcode + 0`;
  param_modes=`printf "%03d" $param_modes`;
  echo $param_modes $opcode;
  
  case $opcode in
    1) ## "C:=A+B"
      A=`GET_INPUT_N $param_modes 1`;
      B=`GET_INPUT_N $param_modes 2`;
      C=`GET_OUTPUT_N $param_modes 3`;

      opresult=`expr $A + $B` || `:`;
      echo $C := $A + $B = $opresult

      workline=`REASSEMBLE $workline $C $opresult`;
      HEAD=`expr $HEAD + 4`;
      ;;
    2) ## "C:=A*B"
      A=`GET_INPUT_N $param_modes 1`;
      B=`GET_INPUT_N $param_modes 2`;
      C=`GET_OUTPUT_N $param_modes 3`;
      echo $param_modes$opcode,$A,$B,$C

      opresult=`expr $A \* $B` || `:`;
      echo $C := $A \* $B = $opresult

      workline=`REASSEMBLE $workline $C $opresult`;
      HEAD=`expr $HEAD + 4`;
      ;;
    3) ## input
      A=`GET_OUTPUT_N $param_modes 1`;
      echo $param_modes$opcode,$A

      read -p "USER INPUT PLEASE: " INPUT;

      echo $A := $INPUT;
      workline=`REASSEMBLE $workline $A $INPUT`;
      HEAD=`expr $HEAD + 2`;
      ;;
    4) ## output
      OUTPUT=`GET_INPUT_N $param_modes 1`;
      echo $param_modes$opcode,$OUTPUT

      echo "Output: $OUTPUT";
      HEAD=`expr $HEAD + 2`;
      last_output=$OUTPUT;
      ;;
    5) ## "jump-if-true"
      A=`GET_INPUT_N $param_modes 1`;
      B=`GET_INPUT_N $param_modes 2`;
      echo $param_modes$opcode,$A,$B

      echo "HEAD := $B + 1";
      if test $A -ne 0; then
        HEAD=`expr $B + 1`;
      else
        HEAD=`expr $HEAD + 3`;
      fi
      ;;
    6) ## "jump-if-false"
      A=`GET_INPUT_N $param_modes 1`;
      B=`GET_INPUT_N $param_modes 2`;
      echo $param_modes$opcode,$A,$B

      echo "HEAD := $B + 1";
      if test $A -eq 0; then
        HEAD=`expr $B + 1`;
      else
        HEAD=`expr $HEAD + 3`;
      fi
      ;;
    7) ## "less than"
      A=`GET_INPUT_N $param_modes 1`;
      B=`GET_INPUT_N $param_modes 2`;
      C=`GET_OUTPUT_N $param_modes 3`;
      echo $param_modes$opcode,$A,$B,$C

      if test $A -lt $B; then
        echo "$C := ($A < $B)";
        workline=`REASSEMBLE $workline $C 1`;
      else
        echo "$C := ($A !< $B)";
        workline=`REASSEMBLE $workline $C 0`;
      fi

      HEAD=`expr $HEAD + 4`;
      ;;
    8) ## "equals"
      A=`GET_INPUT_N $param_modes 1`;
      B=`GET_INPUT_N $param_modes 2`;
      C=`GET_OUTPUT_N $param_modes 3`;
      echo $param_modes$opcode,$A,$B,$C

      if test $A -eq $B; then
        echo "$C := ($A == $B)";
        workline=`REASSEMBLE $workline $C 1`;
      else
        echo "$C := ($A != $B)";
        workline=`REASSEMBLE $workline $C 0`;
      fi

      HEAD=`expr $HEAD + 4`;
      ;;
    9) ## "adjust relative base"
      relbase=`GET_INPUT_N $param_modes 1`;
      echo $param_modes$opcode,$relbase

      echo "New relative base: $relative_base + $relbase";
      relative_base=`expr $relative_base + $relbase`;
      HEAD=`expr $HEAD + 2`;
      ;;
    99) 
      echo "HALT";
      break;
      ;;
    [?]) 
      echo "WRONG OPCODE: $opcode";
      exit `false`;
      ;;
  esac
done

echo "";
echo "Result: `GET 1`";
echo "Last output: $last_output";

