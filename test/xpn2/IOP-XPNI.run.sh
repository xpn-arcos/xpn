#!/bin/sh
set -x

XPN_TEST="IOP-XPNI.exe"
XPN_PART="/PNFS"
#COMM_RANK=0
#COMM_SIZE=1
N_CLIENTS="1"
FILE_SIZE=512

#mpirun -nolocal -np $N_CLIENTS -machinefile ./machines $XPN_TEST $XPN_PART $COMM_RANK $COMM_SIZE $FILE_SIZE
mpirun  -nolocal -np $N_CLIENTS -machinefile ./machines $XPN_TEST $XPN_PART $FILE_SIZE
