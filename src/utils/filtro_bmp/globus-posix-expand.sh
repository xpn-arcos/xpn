#!/bin/bash

rm -f tiempos.txt

NUM_ITER=10

#POSIX 4KB
#echo -n "Posix4KB=(0" >> tiempos.txt
#i=1
#while [ $i -le $NUM_ITER ]
#do
#	rm -f t.txt
#	/usr/bin/time -f "%e" -o t.txt globusrun-ws -submit -F gridimadrid.ciemat.es -S -f posix/filtro_bmp_posix_4KB.xml
#	T=`cat t.txt`
#	echo -n " + $T" >> tiempos.txt
#	let "i+=1"
#done
#echo ")/$NUM_ITER" >> tiempos.txt

#POSIX 64KB
#echo -n "Posix64KB=(0" >> tiempos.txt
#i=1
#while [ $i -le $NUM_ITER ]
#do
#	rm -f t.txt
#	/usr/bin/time -f "%e" -o t.txt globusrun-ws -submit -F gridimadrid.ciemat.es -S -f posix/filtro_bmp_posix_64KB.xml
#	T=`cat t.txt`
#	echo -n " + $T" >> tiempos.txt
#	let "i+=1"
#done
#echo ")/$NUM_ITER" >> tiempos.txt

#EXPAND 4KB
echo -n "Expand4KB=(0" >> tiempos.txt
i=1
while [ $i -le $NUM_ITER ]
do
	rm -f t.txt
	/usr/bin/time -f "%e" -o t.txt globusrun-ws -submit -F gridimadrid.ciemat.es -J -S -f expand/filtro_bmp_expand_4KB.xml
	T=$(cat t.txt)
	echo -n " + $T" >> tiempos.txt
	let "i+=1"
done
echo ")/$NUM_ITER" >> tiempos.txt

#EXPAND 64KB
echo -n "Expand64KB=(0" >> tiempos.txt
i=1
while [ $i -le $NUM_ITER ]
do
	rm -f t.txt
	/usr/bin/time -f "%e" -o t.txt globusrun-ws -submit -F gridimadrid.ciemat.es -J -S -f expand/filtro_bmp_expand_64KB.xml
	T=$(cat t.txt)
	echo -n " + $T" >> tiempos.txt
	let "i+=1"
done
echo ")/$NUM_ITER" >> tiempos.txt

echo ""
cat tiempos.txt
