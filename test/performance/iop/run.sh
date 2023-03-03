#!/bin/bash
#set -x


NP="1"
LBUF="2097152 1048576 524288 262144 131072 65536 32768"

for J in $NP; do
	for S in $LBUF; do
		mpirun -np "$J" -machinefile ./machines ./iop  /P1  "$S"
	done
done

