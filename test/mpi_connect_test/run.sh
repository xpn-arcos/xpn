#!/bin/bash
set -x

# Run hydra nameserver
hydra_nameserver &
sleep 3

# Run server
mpirun.mpich -n 1 -nameserver localhost ./server.exe &
sleep 2

# Run client
mpirun.mpich -n 1 -nameserver localhost ./client.exe &
