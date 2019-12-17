#!/bin/bash


NEXT_PRIME () {
  prime_candidate=$1;

  is_prime="N";
  while test $is_prime = "N"; do
    prime_candidate=`expr $prime_candidate + 1`;
    prime_tester=2;
    is_prime="Y";
    while test `expr $prime_tester \* $prime_tester` -lt $prime_candidate; do
      if test `expr $prime_candidate % $prime_tester` -eq 0; then
        is_prime="N";
        break;
      fi
      prime_tester=`expr $prime_tester + 1`;
      if test $prime_tester -gt 3; then
        prime_tester=`expr $prime_tester + 1`;
      fi
    done
  done

  echo $prime_candidate;
}


## Compute least common multiple of the 3 period lengths
p1=$1;
p2=$2;
p3=$3;

if test -d lcm; then
  rm -r ./lcm;
fi
mkdir lcm;

mkdir lcm/pf1;
check=1;
rest=$p1;
prime_candidate=2;
while test `expr $prime_candidate \* $prime_candidate` -lt $p1; do
  if test $rest -eq 1; then
    break;
  fi
  while test `expr $rest % $prime_candidate` -eq 0; do
    rest=`expr $rest / $prime_candidate`;
    check=`expr $check \* $prime_candidate`;
    echo 1 >> lcm/pf1/$prime_candidate;
  done
  prime_candidate=`NEXT_PRIME $prime_candidate`;
done
if test $rest -gt 1; then
  echo 1 >> lcm/pf1/$rest;
fi

mkdir lcm/pf2;
check=1;
rest=$p2;
prime_candidate=2;
while test `expr $prime_candidate \* $prime_candidate` -lt $p2; do
  if test $rest -eq 1; then
    break;
  fi
  while test `expr $rest % $prime_candidate` -eq 0; do
    rest=`expr $rest / $prime_candidate`;
    check=`expr $check \* $prime_candidate`;
    echo 1 >> lcm/pf2/$prime_candidate;
  done
  prime_candidate=`NEXT_PRIME $prime_candidate`;
done
if test $rest -gt 1; then
  echo 1 >> lcm/pf2/$rest;
fi

mkdir lcm/pf3;
check=1;
rest=$p3;
prime_candidate=2;
while test `expr $prime_candidate \* $prime_candidate` -lt $p3; do
  if test $rest -eq 1; then
    break;
  fi
  while test `expr $rest % $prime_candidate` -eq 0; do
    rest=`expr $rest / $prime_candidate`;
    check=`expr $check \* $prime_candidate`;
    echo 1 >> lcm/pf3/$prime_candidate;
  done
  prime_candidate=`NEXT_PRIME $prime_candidate`;
done
if test $rest -gt 1; then
  echo 1 >> lcm/pf3/$rest;
fi


## Find largest prime factor exponents
find lcm -type f \
| cut -d'/' -f3 \
| sort -g \
| uniq \
| while read pf; do
    power=`find lcm -type f -name $pf -exec wc -l {} \; \
           | sort -g \
           | tail -n1`;
    echo $pf $power;
  done \
| awk 'BEGIN{res=1} {res*=$1**$2} END{print res}';

