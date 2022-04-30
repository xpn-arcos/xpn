# Expand 1.0r1
Expand: XPN Parallel File System

*Licence*: GNU GENERAL PUBLIC LICENSE Version 3</br>
*Authors*: Felix Garcia Carballeira, Luis Miguel Sanchez Garcia, Borja Bergua Guerra, Alejandro Calderon Mateos, Diego Camarmas Alonso, David Garcia Fernandez


## 1. Dependencies

C develop tools and minixml (http://www.minixml.org):

```
sudo apt-get install -y autoconf automake gcc g++ make flex libtool
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
  mpiexec -np <# of processes> -nameserver ${HYDRA_HOSTNAME} -genv LD_PRELOAD=<bypass path> ./<program path>
```


## 4. Source structure

Expand 1.0r1 sources has four main levels:
* xpni:
  eXPaNd Interface provides many enhanced functionality like Fault-Tolerance support, locking, etc.
* xpn:
  eXPaNd implements a Parallel File System using NFI interfaces.
* nfi:
  Network File Interface provides access to several storage protocols (local and network protocols) like NFS, FTP, etc.
* base:
  Base interface provides independent access to operating system resources. 
  Portability is provided in this layer.

