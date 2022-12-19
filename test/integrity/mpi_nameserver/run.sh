#!/bin/bash
set -x

# Run hydra nameserver
hydra_nameserver &
sleep 3

# Run server
/opt/mpich/bin/mpirun -n 1 -nameserver localhost ./server &
sleep 2

# Run client
/opt/mpich/bin/mpirun -n 1 -nameserver localhost ./client &

# Stop hydra nameserver
pkill hydra_nameserver
