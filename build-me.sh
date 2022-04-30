#!/bin/bash
set -x

# install pre-requisites
sudo apt-get install -y autoconf automake gcc g++ make flex libtool
sudo apt-get install -y libmpich-dev libmxml-dev

# create XPN install directory
sudo mkdir -p /opt/xpn 

# configure
./autogen.sh
./configure --prefix=/opt/xpn --enable-nfs3 --enable-tcpserver --enable-mpiserver="/opt/software/install-mpich/bin"

# compile
make clean
make -j

# install
make install

