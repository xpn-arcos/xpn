
 mpiServer
 ---------

## "#define" used:
   * _LARGEFILE64_: add support for files large than 4GB
   * DEBUG	: print debug messages

## Execution:

   * start MPICH hydra:
```
hydra_nameserver &
sleep 1
```

   * start mpiServer:
```
mpirun -np <number of process> -nameserver <hydra host name> ./mpiServer.exe
sleep 1
```

