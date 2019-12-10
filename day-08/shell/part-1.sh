#!/bin/bash

W=25;
H=6;

raw_image=`cat $1`;

image_size=`echo -n $raw_image | wc -c`;
layer_size=`expr $W \* $H`;
layers=`expr $image_size \/ $layer_size`;

fewest_zeroes=99999;

layer=0;
while test $layer -lt $layers; do
  byte_end=`expr \( $layer + 1 \) \* $layer_size`;
  layer_data=`echo -n $raw_image | head -c $byte_end | tail -c $layer_size`;

  zeroes=`echo -n $layer_data | sed 's/[^0]//g' | wc -c`;
  ones=`echo -n $layer_data | sed 's/[^1]//g' | wc -c`;
  twos=`echo -n $layer_data | sed 's/[^2]//g' | wc -c`;

  if test $zeroes -lt $fewest_zeroes; then
    fewest_zeroes=$zeroes;
    echo "New minimum zeroes layer, score is `expr $ones \* $twos`";
  fi

  layer=`expr $layer + 1`;
done

