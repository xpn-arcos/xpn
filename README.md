# XPN 1.4
*Expand Ad-Hoc Parallel File System*

 * ℹ️ *Homepage*: https://xpn-arcos.github.io/arcos-xpn.github.io/ 
 * 📚 *Source*:    https://github.com/xpn-arcos/xpn
 * ↔️ *Licence*:  [GNU GENERAL PUBLIC LICENSE Version 3](https://github.com/dcamarmas/xpn/blob/master/COPYING)</br>
 * 😃 *Authors*:  Felix Garcia Carballeira, Luis Miguel Sanchez Garcia, Borja Bergua Guerra, Alejandro Calderon Mateos, Diego Camarmas Alonso, David Garcia Fernandez



## 1. Install dependencies

XPN needs the typical C development tools and a MPI implementation installed.

### 1.1 Install dependencies on a Linux machine
If you are administrator of your local machine then you need to execute:
```
sudo apt-get install -y autoconf automake gcc g++ make \
                        flex libtool doxygen \
                        libmpich-dev
```

### 1.2 Load dependencies
If you are an user of a cluster with already installed software then you might try the following:
```bash
module available
module purge
module load gcc/11.2.0
module load openmpi/4.1.2
module list
```


## 2. Download source code

You need to download the source code of [XPN](https://xpn-arcos.github.io/arcos-xpn.github.io/) and [minixml](http://www.minixml.org).

You can download both by executing:
```
mkdir $HOME/src
cd    $HOME/src
git clone https://github.com/michaelrsweet/mxml.git
git clone https://github.com/xpn-arcos/xpn.git
```

You must do both 'git clone' requests in the same directory (e.g.: $HOME/src).


## 3. Build XPN 

As an example scenario we will consider the following one:
* MPI distribution is installed at '/opt/software/install-mpich'
* Installation directory will be '/opt/xpn'

To build Expand you need to execute:
```
cd $HOME/src
./xpn/build-me.sh -m /opt/software/install-mpich/bin -i /opt/xpn
```

### 3.1 Build XPN on some predefined platforms
For some predefined platforms 'build-me-compact.sh' offers a default configuration by simply specifying the platform as a parameter.
Please for help try to execute:
```
./xpn/build-me-compact.sh
```


## 3. Execution

### 3.1 mpiServer ###
  The typical executions has 3 main steps:
  - First, launch the mpiServer:   `mpiexec -np <# of processes> <mpiServer.exe>`
  - Then,  launch the XPN client:  `mpiexec -np <# of processes> -genv LD_PRELOAD=<bypass path> ./<program path>`

For example:
   * To start an 8-processes mpiServer and 2-processes client:
```
echo localhost > machines
mpirun -np 8 -machinefile machines ./src_servers/mpiServer/mpiServer.exe
sleep 1
mpirun -np 2 -machinefile machines ./test/IOP-ION/IONMPI /PNFS/
```

