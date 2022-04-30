
 mpiServer
 ---------

## Compile:
   * To compile mpiServer:
```
make clean; make
```

## Execution:

   * To start MPICH hydra nameserver:
```
HYDRA_HOSTNAME=$(hostname)
hydra_nameserver &
sleep 1
```

   * To start one mpiServer with 8 processes:
```
mpirun -np 8 -nameserver ${HYDRA_HOSTNAME} ./mpiServer.exe
```

