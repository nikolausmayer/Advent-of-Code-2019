#!/bin/bash

W=25;
H=6;

raw_image=`cat $1`;

image_size=`echo -n $raw_image | wc -c`;
layer_size=`expr $W \* $H`;
layers=`expr $image_size \/ $layer_size`;

## "transparent" pixels in the bottom layer can be treated like 0 or 1
saved_layer=`echo -n $raw_image \
             | tail -c $layer_size \
             | sed 's/2/0/g'`;
#echo $saved_layer;

layer=`expr $layers - 2`;
while test $layer -ge 0; do
  byte_end=`expr \( $layer + 1 \) \* $layer_size`;
  layer_data=`echo -n $raw_image  \
              | head -c $byte_end \
              | tail -c $layer_size`;

  doubled_layer=`echo "$layer_data * 2 + $saved_layer" \
                 | bc \
                 | tr '\n' ' ' \
                 | sed 's/[\\ ]//g'`;
  #doubled_layer=`printf "%0${layer_size}d" $doubled_layer`;
  saved_layer=`echo -n $doubled_layer \
               | sed 's/0/0/g' \
               | sed 's/1/0/g' \
               | sed 's/2/1/g' \
               | sed 's/3/1/g' \
               | sed 's/4/0/g' \
               | sed 's/5/1/g'`;

  layer=`expr $layer - 1`;
done

byte=$W;
while test $byte -le $layer_size; do
  echo -n $saved_layer | head -c $byte | tail -c $W | sed 's/0/ /g';
  echo "";
  byte=`expr $byte + $W`;
done

