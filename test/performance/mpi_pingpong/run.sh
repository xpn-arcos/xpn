#!/bin/sh
set -x

# Run ping-pong
/opt/mpich/bin/mpirun -n 2 ./ping

