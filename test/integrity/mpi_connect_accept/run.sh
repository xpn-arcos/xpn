#!/bin/sh
set -x

/opt/mpich/bin/mpirun -np 1 ./server > tag.txt &

sleep 1
/opt/mpich/bin/mpirun -np 1 ./client $(cat tag.txt)

sleep 1
/opt/mpich/bin/mpirun -np 1 ./client $(cat tag.txt)

