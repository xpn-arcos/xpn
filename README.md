# XPN 1.0r2
*Expand Ad-Hoc Parallel File System*

 * ℹ️ *Homepage*: https://xpn-arcos.github.io/arcos-xpn.github.io/ 
 * 📚 *Source*:    https://github.com/xpn-arcos/xpn
 * ↔️ *Licence*:  [GNU GENERAL PUBLIC LICENSE Version 3](https://github.com/dcamarmas/xpn/blob/master/COPYING)</br>
 * 😃 *Authors*:  Felix Garcia Carballeira, Luis Miguel Sanchez Garcia, Borja Bergua Guerra, Alejandro Calderon Mateos, Diego Camarmas Alonso, David Garcia Fernandez



## 1. Install dependencies

XPN needs the typical C development tools and the [minixml](http://www.minixml.org).

If you are administrator of your local machine then you need to execute:
```
sudo apt-get install -y autoconf automake gcc g++ make \
                        flex libtool doxygen \
                        libmpich-dev libmxml-dev
```


## 2. Build XPN 

As an example of build scenario:
* MPICH is installed at '/opt/software/install-mpich'
* Installation directory will be '/opt/xpn'

Then, to build Expand you need to execute:
```
./autogen.sh
mkdir -p /opt/xpn 
./configure --prefix=/opt/xpn \
            --enable-nfs3 \
            --enable-tcpserver \
            --enable-mpiserver=/opt/software/install-mpich/bin
make clean
make -j
make install
```

Alternatively, you can use:
```
git clone https://github.com/michaelrsweet/mxml.git
git clone https://github.com/dcamarmas/xpn.git
./xpn/build-me-compact.sh picasso
```


## 3. Execution

### 3.1 mpiServer ###
  The typical executions has 3 main steps:
  - First, execute hydra nameserver: `hydra_nameserver &`
  - Next, launch the mpiServer:      `mpiexec -np <# of processes> -nameserver ${HYDRA_HOSTNAME} <mpiServer.exe>`
  - Finally, launch the XPN client:  `mpiexec -np <# of processes> -nameserver ${HYDRA_HOSTNAME} -genv LD_PRELOAD=<bypass path> ./<program path>`

For example:
   * To start an 8 process mpiServer and 2 process client:
```
HYDRA_HOSTNAME=$(hostname)
hydra_nameserver &
sleep 1
mpirun -np 8 -nameserver ${HYDRA_HOSTNAME} -machinefile machines ./src_servers/mpiServer/mpiServer.exe
sleep 1
mpirun -np 2 -nameserver ${HYDRA_HOSTNAME} -machinefile machines ./test/IOP-ION/IONMPI /PNFS/
```

