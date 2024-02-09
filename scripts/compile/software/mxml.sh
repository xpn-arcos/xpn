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
    echo " $0  -i <Install path> -s <Source path>"
    echo " Where:"
    echo " * <Install path> = full path where MXML is going to be installed."
    echo " * <Source  path> = full path to the source code of MXML."
    echo ""
}


## get arguments
while getopts "i:s:" opt; do
    case "${opt}" in
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
   echo " Skip MXML:"
   echo " * Directory not found: $SRC_PATH"
   exit
fi


## MXML
echo " * MXML: preparing directories..."
  rm -fr "${INSTALL_PATH}/mxml"
mkdir -p "${INSTALL_PATH}/mxml/lib64"
ln    -s "${INSTALL_PATH}/mxml/lib64"  "${INSTALL_PATH}/mxml/lib"

echo " * MXML: compiling and installing..."
pushd .
cd "$SRC_PATH"
./configure --prefix="${INSTALL_PATH}/mxml"
make clean
make -j 8
make install
popd
