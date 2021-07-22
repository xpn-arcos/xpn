#!/bin/sh
set -x


#
#
#
HOSTS="pato11 pato21 pato31 pato41 pato51 pato61 pato71 pato81"
DIRS="pato11 pato21 pato31 pato41 pato51 pato61 pato71 pato81"
#DIRS="pato11 pato21 pato31 pato41 pato51 pato61 pato71 pato81 pato12 pato22 pato32 pato42 pato52 pato62 pato72 pato82 pato13 pato23 pato33 pato43 pato53 pato63 pato73 pato83"


#
#
#
for H in $HOSTS; do 
for D in $DIRS; do 

     ssh $H  rm -fr /export/temp/$D/PNFS/*

done
done

