#!/bin/bash
set -x

g++ hdf5_test.cpp -I/usr/include/hdf5/serial -L/usr/lib/x86_64-linux-gnu/hdf5/serial -lhdf5_cpp -lhdf5 -o hdf5_test


