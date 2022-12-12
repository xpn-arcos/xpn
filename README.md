# XPN 2.0
*Expand Ad-Hoc Parallel File System*

[![License: GPL3](https://img.shields.io/badge/License-GPL3-blue.svg)](https://opensource.org/licenses/GPL-3.0)
![version](https://img.shields.io/badge/version-2.0-blue)

* *Homepage*: https://xpn-arcos.github.io/xpn-arcos.github.io/
* *Source*:   https://github.com/xpn-arcos/xpn
* *Licence*:  [GNU GENERAL PUBLIC LICENSE Version 3](https://github.com/dcamarmas/xpn/blob/master/COPYING)</br>
* *Authors*:  Felix Garcia Carballeira, Luis Miguel Sanchez Garcia, Borja Bergua Guerra, Alejandro Calderon Mateos, Diego Camarmas Alonso, David Garcia Fernandez



## 1. Installing dependencies

XPN needs the typical C development tools and a MPI implementation installed:
  * If you are administrator of your local machine then you need to execute:
    ```
    sudo apt-get install -y autoconf automake gcc g++ make flex libtool doxygen libmpich-dev
    ```
  * If you are an user of a cluster with already installed software then you might try the following:
    ```bash
    module available
    module load <your compiler module>
    module load <your mpi module>
    module list
    ```


## 2. Download the source code of XPN

You need to download the source code of [XPN](https://xpn-arcos.github.io/arcos-xpn.github.io/) and [minixml](http://www.minixml.org), you can download both by executing:
```
mkdir $HOME/src
cd    $HOME/src
git clone https://github.com/michaelrsweet/mxml.git
git clone https://github.com/xpn-arcos/xpn.git
```

You must do both 'git clone' requests in the same directory (e.g.: $HOME/src).


## 3. Building XPN 

To build Expand you need to execute:
```
cd $HOME/src
./xpn/build-me.sh -m <MPICC_PATH> -i <INSTALL_PATH>
```
Where:
  * MPICC_PATH is the full path to your mpicc compiler (e.g.: /usr/bin/mpicc) 
  * INSTALL_PATH is the full path of the directory where XPN is going to be installed (e.g.: $HOME/bin_xpn)


## 4. Executing XPN

First, you need to get familiar with 4 special files:
  * ```<hostfile>``` for MPI, it is a text file with the list of host names (one per line).
  * ```<nameserver file>``` for XPN, it will be (at runtime) a text file with the list of host names where XPN servers are executing.
  * ```<XPN configuration file>``` for XPN, it is a xml file with the configuration for the partition at the XPN servers where files are stored.
  * ```<server file>``` is a text file with the list of the servers to be stopped (one host name per line).

Then, you need to get familiar with 3 special environment variables for XPN:
  * ```XPN_DNS```  for the full path to the nameserver file.
  * ```XPN_CONF``` for the full path to the XPN configuration file.
  * ```XPN_IS_MPISERVER``` is used to at the Expand client to inform that the Expand servers are based on MPI. 

### 4.1 Ad-Hoc Expand (based on MPI)
The typical executions has 3 main steps:
- First, launch the Expand MPI server (xpn_mpi_server):  

    ```
    mpiexec -np <number of processes> \
            -hostfile <full path to the hostfile> \
            -genv LD_LIBRARY_PATH <INSTALL_PATH>/mxml/lib:$LD_LIBRARY_PATH \
            <INSTALL_PATH>/bin/xpn_mpi_server -ns <nameserver file> -tp &
    ```

    To use a thread pool to serve the requests add the -tp flag.

- Then,  launch the program that will use Expand (XPN client):
    
    ```
    mpiexec -np <number of processes> \
            -hostfile <full path to the hostfile> \
            -genv LD_LIBRARY_PATH <INSTALL_PATH>/mxml/lib:$LD_LIBRARY_PATH \
            -genv XPN_DNS <nameserver file> \
            -genv XPN_CONF <XPN configuration file> \
            -genv LD_PRELOAD LD_PRELOAD=<INSTALL_PATH>/xpn/lib/xpn_bypass.so \
            -genv XPN_IS_MPISERVER 1 \
            <program path>
    ```

- At the end of your working session, you need to stop the MPI server (xpn_mpi_server):  

    ```
    mpiexec -np 1 \
            -genv LD_LIBRARY_PATH <INSTALL_PATH>/mxml/lib:$LD_LIBRARY_PATH \
            -genv XPN_DNS <nameserver file> \
            <INSTALL_PATH>/bin/xpn_stop_mpi_server -f <server file>
    ```


## 5. XPN Examples

As an build example scenario we will consider the following one:
* MPI distribution is installed at '/opt/software/install-mpich'
* Installation directory will be '/opt/xpn'

To build Expand in this case you need to execute:
```
cd $HOME/src
./xpn/build-me.sh -m /opt/software/install-mpich/bin/mpicc -i /opt/xpn
```

