#!/bin/sh
set -x

PATOS="pato13 pato23 pato33 pato43 pato53 pato63 pato73 pato83"

for PATO in $PATOS; do
	
	for PATO2 in $PATOS; do
		if [ $PATO2 != $PATO ]; then
			ssh $PATO umount /export/temp/$PATO2/
		fi
	done
	
done

