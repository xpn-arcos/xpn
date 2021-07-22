#!/bin/sh
set -x

PATOR="pato53 pato63 pato73 pato83"
PATOS="pato13 pato23 pato33 pato43"

for PATO in $PATOR; do
	for PATO2 in $PATOS; do
		ssh $PATO rm /export/temp/$PATO2/* -rf
	done
done
