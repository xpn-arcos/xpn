# XPN 1.2
*Expand Ad-Hoc Parallel File System*

 * ℹ️ *Homepage*: https://xpn-arcos.github.io/arcos-xpn.github.io/
 * 📚 *Source*:    https://github.com/xpn-arcos/xpn
 * ↔️ *Licence*:  [GNU GENERAL PUBLIC LICENSE Version 3](https://github.com/dcamarmas/xpn/blob/master/COPYING)</br>
 * 😃 *Authors*:  Felix Garcia Carballeira, Luis Miguel Sanchez Garcia, Borja Bergua Guerra, Alejandro Calderon Mateos, Diego Camarmas Alonso, David Garcia Fernandez



## 1. Example of Execution with mpi_server

   * To start an 2 process mpi_server and 1 process client:
```
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${HOME}/bin/base/lib:${HOME}/bin/mxml/lib:${HOME}/bin/xpn/lib
echo localhost > machines
sleep 1
HYDRA_HOSTNAME=$(hostname)
hydra_nameserver &
sleep 1
mpirun -np 2 -nameserver ${HYDRA_HOSTNAME} -machinefile machines ../../src_servers/mpi_server/mpi_server.exe
sleep 1
mpirun -np 1 -nameserver ${HYDRA_HOSTNAME} -machinefile machines ./IONMPI /PNFS/
```

   * Note: please use mpich-1.2.4 without xpn patch
