#!/bin/bash

sum=0;

while read line; do
  printf "Module mass: %d\n" "$line";
  required_fuel=`expr $line / 3`;
  required_fuel=`expr $required_fuel - 2`;

  fuel_for_fuel=$required_fuel;
  while test $fuel_for_fuel -gt 0; do
    fuel_for_fuel=`expr $fuel_for_fuel / 3`;
    fuel_for_fuel=`expr $fuel_for_fuel - 2`;
    if test $fuel_for_fuel -gt 0; then
      printf "    Adding fuel for fuel: %d\n" "$fuel_for_fuel";
      required_fuel=`expr $required_fuel + $fuel_for_fuel`;
    fi
  done
  printf "  Module fuel required: %d\n" "$required_fuel";

  sum=`expr $sum + $required_fuel`;
done < "$1";

printf "Total Fuel Required: %d\n" "$sum";

