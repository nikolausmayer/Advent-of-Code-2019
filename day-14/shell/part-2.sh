#!/bin/bash

echo "
Makefile.tmp/ORE:
	echo 1 > Makefile.tmp/ORE
" > Makefile;

if test -d Makefile.tmp; then
  rm -r ./Makefile.tmp;
fi
mkdir Makefile.tmp;

while read recipe; do
  product=`echo -n $recipe | cut -d'>' -f2`;
  product_num=`echo -n $product | cut -d' ' -f1`;
  product_name=`echo -n $product | cut -d' ' -f2`;

  educts=`echo -n $recipe | cut -d'=' -f1 | sed 's/, /,/g'`;

  target_string="Makefile.tmp/$product_name: "

  num_reqs=`echo -n $educts | sed 's/[^,]//g' | wc -c`;
  num_reqs=`expr $num_reqs + 1`;
  req_idx=1;
  while test $req_idx -le $num_reqs; do
    req_name=`echo -n $educts | cut -d',' -f$req_idx | cut -d' ' -f2`;
    target_string="$target_string Makefile.tmp/$req_name";

    req_idx=`expr $req_idx + 1`;
  done
  echo "$target_string" >> Makefile;

  
  produce_string="	echo \"scale=9; (0";

  req_idx=1;
  while test $req_idx -le $num_reqs; do
    req_num=`echo -n $educts | cut -d',' -f$req_idx | cut -d' ' -f1`;
    req_name=`echo -n $educts | cut -d',' -f$req_idx | cut -d' ' -f2`;

    produce_string="$produce_string + $req_num*"'`'"cat Makefile.tmp/$req_name"'`'" ";

    req_idx=`expr $req_idx + 1`;
  done

  produce_string="$produce_string )/$product_num\" | bc -l > Makefile.tmp/$product_name";

  echo "$produce_string" >> Makefile;
  echo "" >> Makefile;

done < "$1";


make Makefile.tmp/FUEL > /dev/null;
unit_cost=`cat Makefile.tmp/FUEL`;
echo "Answer:";
echo "1000000000000/$unit_cost" | bc;

