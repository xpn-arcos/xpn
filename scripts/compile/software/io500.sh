#!/bin/bash
# shellcheck disable=all
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
    echo " * <Install path> = full path where IO500 is going to be installed."
    echo " * <Source  path> = full path to the source code of IO500."
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
   echo " Skip IO500:"
   echo " * Directory not found: $SRC_PATH"
   exit
fi

## IO500
echo " * IO500: preparing directories..."
  rm -fr "$INSTALL_PATH/io500"
mkdir -p "$INSTALL_PATH/io500/lib64"
ln    -s "$INSTALL_PATH/io500/lib64"   "$INSTALL_PATH/io500/lib"

echo " * IO500: compiling and installing..."
pushd .
cd "$SRC_PATH"
export PATH=$(dirname $MPICC_PATH):$PATH
env INSTALL_DIR=$INSTALL_PATH/io500 CC=$MPICC_PATH MPICC=$MPICC_PATH  ./prepare.sh
popd
