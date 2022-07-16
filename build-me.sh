#!/bin/bash
#set -x

# 
#  Copyright 2020-2022 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
#  
#  This file is part of Expand.
#  
#  Expand is free software: you can redistribute it and/or modify
#  it under the terms of the GNU Lesser General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#  
#  Expand is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU Lesser General Public License for more details.
#  
#  You should have received a copy of the GNU Lesser General Public License
#  along with Expand.  If not, see <http://www.gnu.org/licenses/>.
#  

# arguments
if [ "$#" != 1 ]; then
    echo "Usage: $0 [mn | picasso | linux]"
    exit
fi

# initial configuration...
BASE_PATH=$(dirname $0)
MXML_SRC_PATH=$BASE_PATH/../mxml
 XPN_SRC_PATH=$BASE_PATH/../xpn

case $1 in
   "mn")
     MPICH_PATH=/gpfs/apps/MN4/INTEL/2017.4/compilers_and_libraries_2017.4.196/linux/mpi/intel64/
     INSTALL_PATH=$HOME/bin/
     ;;
   "picasso")
     MPICH_PATH=/mnt/home/soft/mpich/programs/x86_64/mpich-3.3.1/
     INSTALL_PATH=$HOME/bin/
     ;;
   *)
     MPICH_PATH=/opt/software/install-mpich/
     INSTALL_PATH=/opt/xpn
     ;;
esac


# Start
echo "Begin."

# pre-requisites
echo "1) check packages..."
case $1 in
   "mn")
     module load "impi/2017.4"
     ;;
   "picasso")
     module load mpich/3.3.1_gcc9

       rm -fr $INSTALL_PATH/base
     mkdir -p $INSTALL_PATH/base/lib
     rm -fr                          $INSTALL_PATH/base/lib/libmpfr.so.4
     ln -s  /usr/lib64/libmpfr.so.6  $INSTALL_PATH/base/lib/libmpfr.so.4
     export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$INSTALL_PATH/base/lib/
     ;;
   *)
     PKG_NAMES="autoconf automake gcc g++ make flex libtool doxygen libmpich-dev libmxml-dev"
     for P in $PKG_NAMES; do
         apt-mark showinstall | grep -q "^$P$" || sudo apt-get install -y $P
     done
     ;;
esac

# directories
echo "2) preparing install directories..."
  rm -fr $INSTALL_PATH/xpn
mkdir -p $INSTALL_PATH/xpn/lib64
ln    -s $INSTALL_PATH/xpn/lib64   $INSTALL_PATH/xpn/lib

  rm -fr $INSTALL_PATH/mxml
mkdir -p $INSTALL_PATH/mxml/lib64
ln    -s $INSTALL_PATH/mxml/lib64  $INSTALL_PATH/mxml/lib

# 1) MXML
if [ -d $MXML_SRC_PATH ]; then
   echo "3) preparing mxml..."
   pushd .
   cd $MXML_SRC_PATH
   ./configure --prefix=$INSTALL_PATH/mxml
   make clean
   make -j 8
   make install
   popd
fi

## 2) XPN
if [ -d $XPN_SRC_PATH ]; then
   echo "4) preparing xpn..."
   pushd .
   cd $XPN_SRC_PATH
   ACLOCAL_FLAGS="-I /usr/share/aclocal/" autoreconf -v -i -s -W all
   ./configure --prefix=$INSTALL_PATH/xpn --enable-nfs3 --enable-tcpserver --enable-mpiserver="$MPICH_PATH/bin"
   make clean
   make -j 8
   #doxygen doc/doxygen-XPN.cfg
   make install
   popd
fi

# Stop
echo "End."

