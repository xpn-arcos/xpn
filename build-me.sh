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

function usage {
    echo ""
    echo " Usage:"
    echo " $0  -m <MPI path>  -i <Install path>"
    echo " Where:"
    echo " * <MPI     path> = full path where the MPI implementation (MPICH) is installed."
    echo " * <Install path> = full path where MXML and XPN is going to be installed."
    echo ""
}

# Start
echo ""
echo " build-me"
echo " --------"
echo ""
echo " Begin."


# 1) Arguments...

## base path
BASE_PATH=$(dirname $0)

## get arguments
while getopts "m:i:" opt; do
    case "${opt}" in
          m) MPICH_PATH=${OPTARG}
             ;;
          i) INSTALL_PATH=${OPTARG}
             ;;
          *) echo " Error:"
             echo " * Unknown option: ${opt}"
             usage
             exit
             ;;
    esac
done

## check arguments
if [ "$MPICH_PATH" == "" ]; then
   echo " Error:"
   echo " * Empty MPICH_PATH"
   usage
   exit
fi
if [ "$INSTALL_PATH" == "" ]; then
   echo " Error:"
   echo " * Empty INSTALL_PATH"
   usage
   exit
fi


# 2) XPN and dependencies...
echo "XPN + MXML..."

## MXML
MXML_SRC_PATH=$BASE_PATH/../mxml
if [ -d $MXML_SRC_PATH ]; then
   echo " * MXML: preparing directories..."
     rm -fr $INSTALL_PATH/mxml
   mkdir -p $INSTALL_PATH/mxml/lib64
   ln    -s $INSTALL_PATH/mxml/lib64  $INSTALL_PATH/mxml/lib

   echo " * MXML: compiling and installing..."
   pushd .
   cd $MXML_SRC_PATH
   ./configure --prefix=$INSTALL_PATH/mxml
   make clean
   make -j 8
   make install
   popd
fi

## XPN
XPN_SRC_PATH=$BASE_PATH/../xpn
if [ -d $XPN_SRC_PATH ]; then
   echo " * XPN: preparing directories..."
     rm -fr $INSTALL_PATH/xpn
   mkdir -p $INSTALL_PATH/xpn/lib64
   ln    -s $INSTALL_PATH/xpn/lib64   $INSTALL_PATH/xpn/lib

   echo " * XPN: compiling and installing..."
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


# Benchmarks
echo "IOR + IO500..."

## IOR
IOR_SRC_PATH=$BASE_PATH/../ior
if [ -d $IOR_SRC_PATH ]; then
   echo " * IOR: preparing directories..."
     rm -fr $INSTALL_PATH/ior
   mkdir -p $INSTALL_PATH/ior/lib64
   ln    -s $INSTALL_PATH/ior/lib64   $INSTALL_PATH/ior/lib

   echo " * IOR: compiling and installing..."
   pushd .
   cd $IOR_SRC_PATH
   export MPICC=$MPICH_PATH/bin/mpicc
   ./configure --prefix=$INSTALL_PATH/ior
   make clean
   make -j 8
   make install
   popd
fi

## IO500
IO500_SRC_PATH=$BASE_PATH/../io500
if [ -d $IO500_SRC_PATH ]; then
   echo " * IO500: preparing directories..."
     rm -fr $INSTALL_PATH/io500
   mkdir -p $INSTALL_PATH/io500/lib64
   ln    -s $INSTALL_PATH/io500/lib64   $INSTALL_PATH/io500/lib

   echo " * IO500 DISCLAIMER:"
   echo "   ** Please remember IO500 needs to git clone some components the first time."
   echo "   ** If you don't have access to perform git clone then please ./prepare.sh in other machine first and copy the resulting directory."
   echo ""

   echo " * IO500: compiling and installing..."
   pushd .
   cd $IO500_SRC_PATH
   cat ./build/pfind/prepare.sh | sed "s/git clone/#git clone/g" > ./build/pfind/prepare-alt.sh
   chmod a+x ./build/pfind/prepare-alt.sh
   sed -i "s/^VERSION=/#VERSION=/g" Makefile
   sed -i 's/CC = mpicc/CC = ${MPICH_PATH}\/bin\/mpicc/g' Makefile
   cat prepare.sh | sed "s/^INSTALL_DIR/#INSTALL_DIR/g" | sed "s/git_co https/#git_co https/g" | sed "s|./prepare.sh|./prepare-alt.sh|g" > prepare-alt.sh
   chmod a+x prepare-alt.sh
   #env INSTALL_DIR=$INSTALL_PATH/io500 CC=$MPICH_PATH/bin/mpicc MPICC=$MPICH_PATH/bin/mpicc CFLAGS="-std=c11"  ./prepare-alt.sh
   env INSTALL_DIR=$INSTALL_PATH/io500 CC=$MPICH_PATH/bin/mpicc MPICC=$MPICH_PATH/bin/mpicc  ./prepare-alt.sh
   #rm -fr prepare-alt.sh

fi


# Stop
echo " End."

