#!/bin/bash

if test -d ./factory; then
  rm -r ./factory;
fi

all_recipes="$1";

CHECK () {
  if test ! -f "factory/$1"; then
    echo -n "0" > "factory/$1";
  fi
}

NUM_OF () {
  CHECK $1;

  cat "factory/$1";
}

ENOUGH_OF () {
  CHECK $2;

  if test $1 -le `NUM_OF $2`; then
    echo -n "Y";
  else
    echo -n "N";
  fi
}

ADD () {
  CHECK $2;

  num=`NUM_OF $2`;
  expr $num + $1 > factory/$2;
}

SUBTRACT () {
  CHECK $2;

  num=`NUM_OF $2`;
  expr $num - $1 > factory/$2;
}



mkdir factory;
echo "1 FUEL" > Q;


while test `cat Q | wc -l` -gt 0; do
  echo "";

  next=`tail -n1 Q`;
  next_num=`echo -n $next | cut -d' ' -f1`;
  next_name=`echo -n $next | cut -d' ' -f2`;
  echo "next: $next_num of $next_name";

  ## ORE is special
  if test $next_name = "ORE"; then
    ADD $next_num tally;
    ADD $next_num ORE;
    echo "ORE requirement tally: `NUM_OF tally`";
    head -n-1 Q > tmp; mv tmp Q;
    continue;
  fi

  ## Do we already have enough of X?
  if test `ENOUGH_OF $next_num $next_name` = "Y"; then
    echo "  (OK, `NUM_OF $next_name` >= $next_num in stock)";
    #SUBTRACT $next_num $next_name;
    head -n-1 Q > tmp; mv tmp Q;
    continue;
  else
    echo "  (NOK, `NUM_OF $next_name` < $next_num $next_name)";
  fi

  recipe=`grep -e ".*=>.* $next_name" "$all_recipes" | sed 's/, /,/g'`;
  educts=`echo -n $recipe | cut -d'=' -f1`;
  echo "  recipe: $recipe";

  all_requirements_met="Y";

  ## Can we produce X with what we have in stock?
  num_reqs=`echo -n $educts | sed 's/[^,]//g' | wc -c`;
  num_reqs=`expr $num_reqs + 1`;
  req_idx=1;
  while test $req_idx -le $num_reqs; do
    req_num=`echo -n $educts | cut -d',' -f$req_idx | cut -d' ' -f1`;
    req_name=`echo -n $educts | cut -d',' -f$req_idx | cut -d' ' -f2`;
    if test ! `ENOUGH_OF $req_num $req_name` = "Y"; then
      echo "    (NOK, `NUM_OF $req_name` < $req_num $req_name)";
      all_requirements_met="N";
      break;
    else
      echo "    (OK, `NUM_OF $req_name` >= $req_num $req_name)";
    fi
    req_idx=`expr $req_idx + 1`;
  done

  if test $all_requirements_met = "Y"; then
    ## Time to produce
    req_idx=1;
    while test $req_idx -le $num_reqs; do
      req_num=`echo -n $educts | cut -d',' -f$req_idx | cut -d' ' -f1`;
      req_name=`echo -n $educts | cut -d',' -f$req_idx | cut -d' ' -f2`;
      echo "  (consumed $req_num $req_name)";
      SUBTRACT $req_num $req_name;
      req_idx=`expr $req_idx + 1`;
    done
    products=`echo -n $recipe | cut -d'>' -f2`;
    product_num=`echo -n $products | cut -d' ' -f1`;
    product_name=`echo -n $products | cut -d' ' -f2`;
    ADD $product_num $product_name;
    echo "  (OK, produced $product_num $product_name)";

    head -n-1 Q > tmp; mv tmp Q;
  else
    req_idx=1;
    while test $req_idx -le $num_reqs; do
      req_num=`echo -n $educts | cut -d',' -f$req_idx | cut -d' ' -f1`;
      req_name=`echo -n $educts | cut -d',' -f$req_idx | cut -d' ' -f2`;
      echo "$req_num $req_name" >> Q;
      echo "  (requested $req_num $req_name)";
      req_idx=`expr $req_idx + 1`;
    done
  fi
done

echo "";
echo "Total ORE required: `NUM_OF tally`";

