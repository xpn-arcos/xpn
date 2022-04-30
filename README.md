# Expand 1.0r1
Expand: XPN Ad-Hoc Parallel File System

*Licence*: GNU GENERAL PUBLIC LICENSE Version 3</br>
*Authors*: Felix Garcia Carballeira, Luis Miguel Sanchez Garcia, Borja Bergua Guerra, Alejandro Calderon Mateos, Diego Camarmas Alonso, David Garcia Fernandez

*Web*:    https://xpn-arcos.github.io/arcos-xpn.github.io/
*Source*: https://github.com/xpn-arcos/xpn


## 1. Dependencies

C develop tools and minixml (http://www.minixml.org):

```
sudo apt-get install -y autoconf automake gcc g++ make
sudo apt-get install -y flex libtool sudo apt install doxygen
sudo apt-get install -y libmpich-dev libmxml-dev
```


## 2. Build

Briefly, the main steps to build Expand are:
```
./autogen.sh
./configure [<options>]
make clean
make -j
doxygen doc/doxygen-XPN.cfg
make install
```

For example (if MPICH is installed at '/opt/software/install-mpich'):
```
./autogen.sh
mkdir -p /opt/xpn 
./configure --prefix=/opt/xpn --enable-nfs3 --enable-tcpserver --enable-mpiserver=/opt/software/install-mpich/bin
make clean
make -j
doxygen doc/doxygen-XPN.cfg
make install
```


## 3. Execution

- mpiServer:
```
  hydra_nameserver &
  mpiexec -np <# of processes> -nameserver ${HYDRA_HOSTNAME} -genv LD_PRELOAD=<bypass path> ./<program path>
```

For example:
   * To start MPICH hydra nameserver:
```
HYDRA_HOSTNAME=$(hostname)
hydra_nameserver &
sleep 1
```
   * To start one mpiServer with 8 processes:
```
mpirun -np 8 -nameserver ${HYDRA_HOSTNAME} -machinefile machines ./src_servers/mpiServer/mpiServer.exe
sleep 1
```
   * To start one client with 2 processes:
```
mpirun -np 2 -nameserver ${HYDRA_HOSTNAME} -machinefile machines ./test/IOP-ION/IONMPI /PNFS/
```

