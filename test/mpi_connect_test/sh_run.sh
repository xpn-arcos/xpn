#!/bin/bash
set -x

# Run hydra nameserver
hydra_nameserver &
sleep 1

# Run server
mpirun.mpich -n 2 -nameserver localhost ./server.exe &
sleep 1

# Run client
mpirun.mpich -n 2 -nameserver localhost ./client.exe &
