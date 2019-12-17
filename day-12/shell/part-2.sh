#!/bin/bash

m1x=`head -n1 "$1" | tail -n1 | sed 's/[<>xyz= ]//g' | cut -d',' -f1`;
m1y=`head -n1 "$1" | tail -n1 | sed 's/[<>xyz= ]//g' | cut -d',' -f2`;
m1z=`head -n1 "$1" | tail -n1 | sed 's/[<>xyz= ]//g' | cut -d',' -f3`;
m2x=`head -n2 "$1" | tail -n1 | sed 's/[<>xyz= ]//g' | cut -d',' -f1`;
m2y=`head -n2 "$1" | tail -n1 | sed 's/[<>xyz= ]//g' | cut -d',' -f2`;
m2z=`head -n2 "$1" | tail -n1 | sed 's/[<>xyz= ]//g' | cut -d',' -f3`;
m3x=`head -n3 "$1" | tail -n1 | sed 's/[<>xyz= ]//g' | cut -d',' -f1`;
m3y=`head -n3 "$1" | tail -n1 | sed 's/[<>xyz= ]//g' | cut -d',' -f2`;
m3z=`head -n3 "$1" | tail -n1 | sed 's/[<>xyz= ]//g' | cut -d',' -f3`;
m4x=`head -n4 "$1" | tail -n1 | sed 's/[<>xyz= ]//g' | cut -d',' -f1`;
m4y=`head -n4 "$1" | tail -n1 | sed 's/[<>xyz= ]//g' | cut -d',' -f2`;
m4z=`head -n4 "$1" | tail -n1 | sed 's/[<>xyz= ]//g' | cut -d',' -f3`;

v1x=0; v1y=0; v1z=0;
v2x=0; v2y=0; v2z=0;
v3x=0; v3y=0; v3z=0;
v4x=0; v4y=0; v4z=0;

echo "" > sig1.txt;
echo "" > sig2.txt;
echo "" > sig3.txt;

p1=0;
p2=0;
p3=0;

iter=0;
while test $iter -lt 1000000; do

  if test -f sig1.txt; then
    if test `grep -c "^$m1x,$m2x,$m3x,,$v1x,$v2x,$v3x$" sig1.txt` -eq 1; then
      echo "period X: $iter";
      rm sig1.txt;
      p1=$iter;
    fi
  fi
  if test -f sig2.txt; then
    if test `grep -c "^$m1y,$m2y,$m3y,,$v1y,$v2y,$v3y$" sig2.txt` -eq 1; then
      echo "period Y: $iter";
      rm sig2.txt;
      p2=$iter;
    fi
  fi
  if test -f sig3.txt; then
    if test `grep -c "^$m1z,$m2z,$m3z,,$v1z,$v2z,$v3z$" sig3.txt` -eq 1; then
      echo "period Z: $iter";
      rm sig3.txt;
      p3=$iter;
    fi
  fi

  ## Found all 3 period lengths? Compute least common multiple
  if test $p1 -ne 0 -a \
          $p2 -ne 0 -a \
          $p3 -ne 0; then
    
    exit `:`;
  fi

  if test -f sig1.txt; then
    echo "$m1x,$m2x,$m3x,,$v1x,$v2x,$v3x" >> sig1.txt;
  fi
  if test -f sig2.txt; then
    echo "$m1y,$m2y,$m3y,,$v1y,$v2y,$v3y" >> sig2.txt;
  fi
  if test -f sig3.txt; then
    echo "$m1z,$m2z,$m3z,,$v1z,$v2z,$v3z" >> sig3.txt;
  fi


  if   test $m1x -lt $m2x; then v1x=`expr $v1x + 1`;
  elif test $m1x -gt $m2x; then v1x=`expr $v1x - 1`; fi
  if   test $m1x -lt $m3x; then v1x=`expr $v1x + 1`;
  elif test $m1x -gt $m3x; then v1x=`expr $v1x - 1`; fi
  if   test $m1x -lt $m4x; then v1x=`expr $v1x + 1`;
  elif test $m1x -gt $m4x; then v1x=`expr $v1x - 1`; fi

  if   test $m1y -lt $m2y; then v1y=`expr $v1y + 1`;
  elif test $m1y -gt $m2y; then v1y=`expr $v1y - 1`; fi
  if   test $m1y -lt $m3y; then v1y=`expr $v1y + 1`;
  elif test $m1y -gt $m3y; then v1y=`expr $v1y - 1`; fi
  if   test $m1y -lt $m4y; then v1y=`expr $v1y + 1`;
  elif test $m1y -gt $m4y; then v1y=`expr $v1y - 1`; fi

  if   test $m1z -lt $m2z; then v1z=`expr $v1z + 1`;
  elif test $m1z -gt $m2z; then v1z=`expr $v1z - 1`; fi
  if   test $m1z -lt $m3z; then v1z=`expr $v1z + 1`;
  elif test $m1z -gt $m3z; then v1z=`expr $v1z - 1`; fi
  if   test $m1z -lt $m4z; then v1z=`expr $v1z + 1`;
  elif test $m1z -gt $m4z; then v1z=`expr $v1z - 1`; fi


  if   test $m2x -lt $m1x; then v2x=`expr $v2x + 1`;
  elif test $m2x -gt $m1x; then v2x=`expr $v2x - 1`; fi
  if   test $m2x -lt $m3x; then v2x=`expr $v2x + 1`;
  elif test $m2x -gt $m3x; then v2x=`expr $v2x - 1`; fi
  if   test $m2x -lt $m4x; then v2x=`expr $v2x + 1`;
  elif test $m2x -gt $m4x; then v2x=`expr $v2x - 1`; fi

  if   test $m2y -lt $m1y; then v2y=`expr $v2y + 1`;
  elif test $m2y -gt $m1y; then v2y=`expr $v2y - 1`; fi
  if   test $m2y -lt $m3y; then v2y=`expr $v2y + 1`;
  elif test $m2y -gt $m3y; then v2y=`expr $v2y - 1`; fi
  if   test $m2y -lt $m4y; then v2y=`expr $v2y + 1`;
  elif test $m2y -gt $m4y; then v2y=`expr $v2y - 1`; fi

  if   test $m2z -lt $m1z; then v2z=`expr $v2z + 1`;
  elif test $m2z -gt $m1z; then v2z=`expr $v2z - 1`; fi
  if   test $m2z -lt $m3z; then v2z=`expr $v2z + 1`;
  elif test $m2z -gt $m3z; then v2z=`expr $v2z - 1`; fi
  if   test $m2z -lt $m4z; then v2z=`expr $v2z + 1`;
  elif test $m2z -gt $m4z; then v2z=`expr $v2z - 1`; fi


  if   test $m3x -lt $m1x; then v3x=`expr $v3x + 1`;
  elif test $m3x -gt $m1x; then v3x=`expr $v3x - 1`; fi
  if   test $m3x -lt $m2x; then v3x=`expr $v3x + 1`;
  elif test $m3x -gt $m2x; then v3x=`expr $v3x - 1`; fi
  if   test $m3x -lt $m4x; then v3x=`expr $v3x + 1`;
  elif test $m3x -gt $m4x; then v3x=`expr $v3x - 1`; fi

  if   test $m3y -lt $m1y; then v3y=`expr $v3y + 1`;
  elif test $m3y -gt $m1y; then v3y=`expr $v3y - 1`; fi
  if   test $m3y -lt $m2y; then v3y=`expr $v3y + 1`;
  elif test $m3y -gt $m2y; then v3y=`expr $v3y - 1`; fi
  if   test $m3y -lt $m4y; then v3y=`expr $v3y + 1`;
  elif test $m3y -gt $m4y; then v3y=`expr $v3y - 1`; fi

  if   test $m3z -lt $m1z; then v3z=`expr $v3z + 1`;
  elif test $m3z -gt $m1z; then v3z=`expr $v3z - 1`; fi
  if   test $m3z -lt $m2z; then v3z=`expr $v3z + 1`;
  elif test $m3z -gt $m2z; then v3z=`expr $v3z - 1`; fi
  if   test $m3z -lt $m4z; then v3z=`expr $v3z + 1`;
  elif test $m3z -gt $m4z; then v3z=`expr $v3z - 1`; fi


  if   test $m4x -lt $m1x; then v4x=`expr $v4x + 1`;
  elif test $m4x -gt $m1x; then v4x=`expr $v4x - 1`; fi
  if   test $m4x -lt $m2x; then v4x=`expr $v4x + 1`;
  elif test $m4x -gt $m2x; then v4x=`expr $v4x - 1`; fi
  if   test $m4x -lt $m3x; then v4x=`expr $v4x + 1`;
  elif test $m4x -gt $m3x; then v4x=`expr $v4x - 1`; fi

  if   test $m4y -lt $m1y; then v4y=`expr $v4y + 1`;
  elif test $m4y -gt $m1y; then v4y=`expr $v4y - 1`; fi
  if   test $m4y -lt $m2y; then v4y=`expr $v4y + 1`;
  elif test $m4y -gt $m2y; then v4y=`expr $v4y - 1`; fi
  if   test $m4y -lt $m3y; then v4y=`expr $v4y + 1`;
  elif test $m4y -gt $m3y; then v4y=`expr $v4y - 1`; fi

  if   test $m4z -lt $m1z; then v4z=`expr $v4z + 1`;
  elif test $m4z -gt $m1z; then v4z=`expr $v4z - 1`; fi
  if   test $m4z -lt $m2z; then v4z=`expr $v4z + 1`;
  elif test $m4z -gt $m2z; then v4z=`expr $v4z - 1`; fi
  if   test $m4z -lt $m3z; then v4z=`expr $v4z + 1`;
  elif test $m4z -gt $m3z; then v4z=`expr $v4z - 1`; fi



  m1x=`expr $m1x + $v1x`; 
  m1y=`expr $m1y + $v1y`; 
  m1z=`expr $m1z + $v1z`;

  m2x=`expr $m2x + $v2x`; 
  m2y=`expr $m2y + $v2y`; 
  m2z=`expr $m2z + $v2z`;

  m3x=`expr $m3x + $v3x`; 
  m3y=`expr $m3y + $v3y`; 
  m3z=`expr $m3z + $v3z`;

  m4x=`expr $m4x + $v4x`; 
  m4y=`expr $m4y + $v4y`; 
  m4z=`expr $m4z + $v4z`;


  iter=`expr $iter + 1`;
done

## The answer is the least common multiple of the three periods lengths
bash lcm.sh $p1 $p2 $p3;

