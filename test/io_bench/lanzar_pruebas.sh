#!/bin/bash
set -x
$HOME/mpich-1.2.4/bin/mpirun -np 1 -machinefile machines -nolocal flash_benchmark_io &> salida &
ps aux | grep flash_benchmark_io
