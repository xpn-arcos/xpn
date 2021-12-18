#!/bin/bash
set -x

#
# initialization
# 
HOSTNAME=$(hostname)
SERVER_NP=2
N_TESTS=2

# https://stackoverflow.com/questions/360201/how-do-i-kill-background-processes-jobs-when-my-shell-script-exits
trap "trap - SIGTERM && kill -- -$$" SIGINT SIGTERM EXIT

# start MPICH hydra
hydra_nameserver &
sleep 1

# run mpiServer...
mpirun -np $SERVER_NP -nameserver ${HOSTNAME} ./mpiServer.exe
sleep 1

# kill background process
sleep 1
kill -- -$$

