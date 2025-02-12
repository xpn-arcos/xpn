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
    echo " * <Install path> = full path where Prometheus-client is going to be installed."
    echo " * <Source  path> = full path to the source code of Prometheus-client."
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
   echo " Skip Prometheus-client:"
   echo " * Directory not found: $SRC_PATH"
   exit
fi


## Prometheus-client
echo " * Prometheus-client: preparing directories..."
  rm  -fr "$INSTALL_PATH/promcli"
mkdir -fr "$INSTALL_PATH/promcli/lib"

echo " * Prometheus-client: compiling and installing..."
pushd .
cd "$SRC_PATH"
bash auto clean
bash auto build
cp  -a  $(find . -name "*.so")  "$INSTALL_PATH/promcli/lib"
cp  -a  ./prom/include          "$INSTALL_PATH/promcli/"
popd
