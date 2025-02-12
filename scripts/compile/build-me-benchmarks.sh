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
    echo " $0  -m <mpicc path>  -i <Install path>"
    echo " Where:"
    echo " * <mpicc   path> = full path where the mpicc is installed."
    echo " * <Install path> = full path where IOR and IO500 is going to be installed."
    echo ""
}

# Start
echo ""
echo " build-me (benchmarks)"
echo " --------"
echo ""
echo " Begin."


# 1) Arguments...

## base path
BASE_PATH=$(dirname $0)

## get arguments
while getopts "m:i:" opt; do
    case "${opt}" in
          m) MPICC_PATH=${OPTARG}
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


# Benchmarks
$BASE_PATH/software/ior.sh    -m $MPICC_PATH -i $INSTALL_PATH -s $BASE_PATH/../../../ior
$BASE_PATH/software/io500.sh  -m $MPICC_PATH -i $INSTALL_PATH -s $BASE_PATH/../../../io500

echo " End."
