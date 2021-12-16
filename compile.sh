#!/bin/bash
set -x

# install pre-requisites
sudo apt-get install -y libmxml-dev flex libtool

# create XPN install directory
sudo mkdir -p /opt/xpn 

# configure
./autogen.sh
./configure --prefix=/opt/xpn --enable-nfs3 --enable-tcpserver --enable-mpiserver

# compile
make clean
make -j 8

# install
make install

