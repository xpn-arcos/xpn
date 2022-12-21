#!/bin/bash
# shellcheck disable=all
#set -x

# 
#  Copyright 2020-2023 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos
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
PKG_NAMES="autoconf automake gcc g++ make flex libtool doxygen"
for P in $PKG_NAMES; do
    apt-mark showinstall | grep -q "^$P$" || sudo apt-get install -y $P
done

# 2) working path...
MPICC_PATH=/home/dcamarma/mpich-install/bin/mpicc
INSTALL_PATH=/home/dcamarma/bin/
BASE_PATH=$(dirname $0)

# 3) preconfigure build-me...
$BASE_PATH/../../build-me.sh         -m $MPICC_PATH -i $INSTALL_PATH
$BASE_PATH/../build-me-benchmarks.sh -m $MPICC_PATH -i $INSTALL_PATH

