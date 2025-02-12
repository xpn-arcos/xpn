#!/bin/bash
#set -x

#
#  Copyright 2020-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos
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
    echo " $0  -m <mpicc path>  -i <Install path> -s <Source path>"
    echo " Where:"
    echo " * <mpicc   path> = full path where the mpicc is installed."
    echo " * <Install path> = full path where XPN is going to be installed."
    echo " * <Source  path> = full path to the source code XPN."
    echo ""
}


## get arguments
while getopts "m:i:s:" opt; do
    case "${opt}" in
          m) MPICC_PATH=${OPTARG}
             ;;
          i) INSTALL_PATH=${OPTARG}
             ;;
          s) SRC_PATH=${OPTARG}
             ;;
          *) echo " Error:"
             echo " * Unknown option: ${opt}"
             usage
             exit
             ;;
    esac
done

## check arguments
if [ "$MPICC_PATH" == "" ]; then
   echo " Error:"
   echo " * Empty MPICC_PATH"
   usage
   exit
fi
if [ "$INSTALL_PATH" == "" ]; then
   echo " Error:"
   echo " * Empty INSTALL_PATH"
   usage
   exit
fi
if [ "$SRC_PATH" == "" ]; then
   echo " Error:"
   echo " * Empty SRC_PATH"
   usage
   exit
fi
if [ ! -d "$SRC_PATH" ]; then
   echo " Skip XPN:"
   echo " * Directory not found: $SRC_PATH"
   exit
fi


## XPN
echo " * XPN: preparing directories..."
  rm -fr "${INSTALL_PATH}/xpn"
mkdir -p "${INSTALL_PATH}/xpn/lib64"
ln    -s "${INSTALL_PATH}/xpn/lib64"   "${INSTALL_PATH}/xpn/lib"

echo " * XPN: compiling and installing..."
pushd .
cd "$SRC_PATH"
ACLOCAL_FLAGS="-I /usr/share/aclocal/" autoreconf -v -i -s -W all
LD_LIBRARY_PATH="$LD_LIBRARY_PATH:$HOME/bin/mosquitto/lib64" CFLAGS="-I${INSTALL_PATH}/mosquitto/include" CPPFLAGS="-I${INSTALL_PATH}/mosquitto/include" LDFLAGS="-L${INSTALL_PATH}/mosquitto/lib64" ./configure --prefix="${INSTALL_PATH}/xpn" --enable-mosquitto
CFLAGS="-I${INSTALL_PATH}/mosquitto/include" CPPFLAGS="-I${INSTALL_PATH}/mosquitto/include" LDFLAGS="-L${INSTALL_PATH}/mosquitto/lib64" make clean
CFLAGS="-I${INSTALL_PATH}/mosquitto/include" CPPFLAGS="-I${INSTALL_PATH}/mosquitto/include" LDFLAGS="-L${INSTALL_PATH}/mosquitto/lib64" make -j 16
#doxygen doc/doxygen-XPN.cfg
make install
popd
