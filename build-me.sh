#!/bin/bash
#set -x

# Start
echo "Begin."

# install pre-requisites
PKG_NAMES="autoconf automake gcc g++ make flex libtool doxygen libmpich-dev libmxml-dev"
echo "1) check packages "$PKG_NAMES" are installed..."
for P in $PKG_NAMES; do
    apt-mark showinstall | grep -q "^$P$" || sudo apt-get install -y $P
done

# autoreconf
echo "2) autoreconf..."
ACLOCAL_FLAGS="-I /usr/share/aclocal/" autoreconf -v -i -s -W all

# configure
echo "3) configure..."
[ ! -d "/home/dcamarma/xpn-install" ] && sudo mkdir -p /home/dcamarma/xpn-install 
./configure --prefix=/home/dcamarma/xpn-install --enable-nfs3 --enable-tcpserver --enable-mpiserver="/home/dcamarma/mpich-install/bin"

# compile
echo "4) make..."
make clean
make -j

# install
echo "5) make install..."
#doxygen doc/doxygen-XPN.cfg
make install

# Stop
echo "End."

