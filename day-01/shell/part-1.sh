#!/bin/bash

sum=0;

while read line; do
  required_fuel=`expr $line / 3`;
  required_fuel=`expr $required_fuel - 2`;
  sum=`expr $sum + $required_fuel`;
done < "$1";

printf "Total Fuel Requirement: %d\n" "$sum";

