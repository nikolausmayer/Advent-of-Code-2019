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

iter=0;
while test $iter -lt 1000; do

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


  echo "pos=<x=$m1x, y=$m1y, z=$m1z>, vel=<x=$v1x, y=$v1y, z=$v1z>";
  echo "pos=<x=$m2x, y=$m2y, z=$m2z>, vel=<x=$v2x, y=$v2y, z=$v2z>";
  echo "pos=<x=$m3x, y=$m3y, z=$m3z>, vel=<x=$v3x, y=$v3y, z=$v3z>";
  echo "pos=<x=$m4x, y=$m4y, z=$m4z>, vel=<x=$v4x, y=$v4y, z=$v4z>";
  echo "";

  m1p=0; m1k=0;
  if test $m1x -lt 0; then m1p=`expr $m1p - $m1x`; else m1p=`expr $m1p + $m1x`; fi
  if test $m1y -lt 0; then m1p=`expr $m1p - $m1y`; else m1p=`expr $m1p + $m1y`; fi
  if test $m1z -lt 0; then m1p=`expr $m1p - $m1z`; else m1p=`expr $m1p + $m1z`; fi
  if test $v1x -lt 0; then m1k=`expr $m1k - $v1x`; else m1k=`expr $m1k + $v1x`; fi
  if test $v1y -lt 0; then m1k=`expr $m1k - $v1y`; else m1k=`expr $m1k + $v1y`; fi
  if test $v1z -lt 0; then m1k=`expr $m1k - $v1z`; else m1k=`expr $m1k + $v1z`; fi
  m2p=0; m2k=0;
  if test $m2x -lt 0; then m2p=`expr $m2p - $m2x`; else m2p=`expr $m2p + $m2x`; fi
  if test $m2y -lt 0; then m2p=`expr $m2p - $m2y`; else m2p=`expr $m2p + $m2y`; fi
  if test $m2z -lt 0; then m2p=`expr $m2p - $m2z`; else m2p=`expr $m2p + $m2z`; fi
  if test $v2x -lt 0; then m2k=`expr $m2k - $v2x`; else m2k=`expr $m2k + $v2x`; fi
  if test $v2y -lt 0; then m2k=`expr $m2k - $v2y`; else m2k=`expr $m2k + $v2y`; fi
  if test $v2z -lt 0; then m2k=`expr $m2k - $v2z`; else m2k=`expr $m2k + $v2z`; fi
  m3p=0; m3k=0;
  if test $m3x -lt 0; then m3p=`expr $m3p - $m3x`; else m3p=`expr $m3p + $m3x`; fi
  if test $m3y -lt 0; then m3p=`expr $m3p - $m3y`; else m3p=`expr $m3p + $m3y`; fi
  if test $m3z -lt 0; then m3p=`expr $m3p - $m3z`; else m3p=`expr $m3p + $m3z`; fi
  if test $v3x -lt 0; then m3k=`expr $m3k - $v3x`; else m3k=`expr $m3k + $v3x`; fi
  if test $v3y -lt 0; then m3k=`expr $m3k - $v3y`; else m3k=`expr $m3k + $v3y`; fi
  if test $v3z -lt 0; then m3k=`expr $m3k - $v3z`; else m3k=`expr $m3k + $v3z`; fi
  m4p=0; m4k=0;
  if test $m4x -lt 0; then m4p=`expr $m4p - $m4x`; else m4p=`expr $m4p + $m4x`; fi
  if test $m4y -lt 0; then m4p=`expr $m4p - $m4y`; else m4p=`expr $m4p + $m4y`; fi
  if test $m4z -lt 0; then m4p=`expr $m4p - $m4z`; else m4p=`expr $m4p + $m4z`; fi
  if test $v4x -lt 0; then m4k=`expr $m4k - $v4x`; else m4k=`expr $m4k + $v4x`; fi
  if test $v4y -lt 0; then m4k=`expr $m4k - $v4y`; else m4k=`expr $m4k + $v4y`; fi
  if test $v4z -lt 0; then m4k=`expr $m4k - $v4z`; else m4k=`expr $m4k + $v4z`; fi
  system_energy=`expr \( $m1p \* $m1k \) + \
                      \( $m2p \* $m2k \) + \
                      \( $m3p \* $m3k \) + \
                      \( $m4p \* $m4k \)`;
  echo "System energy = $system_energy";


  iter=`expr $iter + 1`;
done

