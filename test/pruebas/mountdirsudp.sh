#!/bin/sh
set -x

PATOS_SERV="pato13 pato23 pato33 pato43"
PATOS_CLI="pato53 pato63 pato73 pato83"

for PATO in $PATOS_CLI; do	
	for PATO2 in $PATOS_SERV; do
		if [ "$PATO2" != "$PATO" ]; then
		ssh $PATO mount -t nfs $PATO2:/export/temp/$PATO2/ /export/temp/$PATO2/
		fi
	done
	
done

