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


# 1) software (if needed)...
module load mpich/3.3.1_gcc9

# 2) working path...
MPICC_PATH=/mnt/home/soft/mpich/programs/x86_64/mpich-3.3.1/bin/mpicc
INSTALL_PATH=$HOME/bin/
BASE_PATH=$(dirname $0)

# patch for libmpfr.so.4
rm -fr $INSTALL_PATH/base
mkdir -p $INSTALL_PATH/base/lib
rm -fr                          $INSTALL_PATH/base/lib/libmpfr.so.4
ln -s  /usr/lib64/libmpfr.so.6  $INSTALL_PATH/base/lib/libmpfr.so.4
ln -s  /usr/lib64/libslurm.so   $INSTALL_PATH/base/lib/libslurm.so.32
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$INSTALL_PATH/base/lib/

# 3) preconfigure build-me...
$BASE_PATH/../software/mxml.sh                       -i $INSTALL_PATH -s $BASE_PATH/../../../../mxml
$BASE_PATH/../software/xpn.sh         -m $MPICC_PATH -i $INSTALL_PATH -s $BASE_PATH/../../../../xpn
$BASE_PATH/../software/ior.sh         -m $MPICC_PATH -i $INSTALL_PATH -s $BASE_PATH/../../../../ior
$BASE_PATH/../software/lz4.sh         -m $MPICC_PATH -i $INSTALL_PATH -s $BASE_PATH/../../../../io500/build/pfind/lz4/
$BASE_PATH/../software/io500.sh       -m $MPICC_PATH -i $INSTALL_PATH -s $BASE_PATH/../../../../io500
