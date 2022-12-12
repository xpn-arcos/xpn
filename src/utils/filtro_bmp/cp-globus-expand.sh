#!/bin/bash

rm -f tiempos.txt

NUM_ITER=10

#GLOBUS
echo -n "Globus=" >> tiempos.txt
SUM=0
i=1
while [ $i -le $NUM_ITER ]
do
	rm -f t.txt
	/usr/bin/time -f "%e" -o t.txt globusrun-ws -submit -F gridimadrid.ciemat.es -S -f cp_globus.xml
	T=`cat t.txt`
	SUM="$SUM + $T"
	let "i+=1"
done
echo "($SUM)/$NUM_ITER" >> tiempos.txt

#EXPAND 4KB
echo -n "Expand4KB=" >> tiempos.txt
SUM=0
i=1
while [ $i -le $NUM_ITER ]
do
	rm -f t.txt
	/usr/bin/time -f "%e" -o t.txt globusrun-ws -submit -F gridimadrid.ciemat.es -J -S -f cp_expand_4KB.xml
	T=`cat t.txt`
	SUM="$SUM + $T"
	let "i+=1"
done
echo "($SUM)/$NUM_ITER" >> tiempos.txt

#EXPAND 64KB
echo -n "Expand64KB=" >> tiempos.txt
SUM=0
i=1
while [ $i -le $NUM_ITER ]
do
	rm -f t.txt
	/usr/bin/time -f "%e" -o t.txt globusrun-ws -submit -F gridimadrid.ciemat.es -J -S -f cp_expand_64KB.xml
	T=`cat t.txt`
	SUM="$SUM + $T"
	let "i+=1"
done
echo "($SUM)/$NUM_ITER" >> tiempos.txt

#EXPAND 256KB
echo -n "Expand256KB=" >> tiempos.txt
SUM=0
i=1
while [ $i -le $NUM_ITER ]
do
	rm -f t.txt
	/usr/bin/time -f "%e" -o t.txt globusrun-ws -submit -F gridimadrid.ciemat.es -J -S -f cp_expand_256KB.xml
	T=`cat t.txt`
	SUM="$SUM + $T"
	let "i+=1"
done
echo "($SUM)/$NUM_ITER" >> tiempos.txt

echo ""
cat tiempos.txt
