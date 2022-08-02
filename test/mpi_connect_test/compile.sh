#!/bin/bash
set -x

mpicc -Wall -g ./client.c -o client.exe
mpicc -Wall -g ./server.c -o server.exe

