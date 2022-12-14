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

function usage {
    echo ""
    echo " Usage:"
    echo " $0 <platform>"
    echo " Where:"
    echo " * platform = mn4 | cte-arm | picasso | tucan | lab21 | generic"
    echo ""
}

function install_if_not_installed {
     PKG_NAMES="autoconf automake gcc g++ make flex libtool doxygen"
     for P in $PKG_NAMES; do
         apt-mark showinstall | grep -q "^$P$" || sudo apt-get install -y $P
     done
}


# Start
echo ""
echo " build-me-compact"
echo " ----------------"
echo ""
echo " Begin."

# 1) arguments
if [ "$#" != 1 ]; then
    usage
    exit
fi

# 2) initial configuration...
case $1 in
   "mn4")
     # working path...
     MPICC_PATH=/gpfs/apps/MN4/INTEL/2017.4/compilers_and_libraries_2017.4.196/linux/mpi/intel64/bin/mpicc
     INSTALL_PATH=$HOME/mn4/bin/

     # load modules...
     module load "impi/2017.4"
     ;;

   "cte-arm")
     # working path...
     MPICC_PATH=/opt/FJSVxtclanga/tcsds-1.2.26b/bin/mpifcc
     INSTALL_PATH=$HOME/cte-arm/bin/

     # load modules...
     module load fuji
     #module load gcc openmpi

     # patch for cross-compiling
     export    CC=/opt/FJSVxtclanga/tcsds-1.2.26b/bin/mpifcc
     export MPICC=/opt/FJSVxtclanga/tcsds-1.2.26b/bin/mpifcc
     ;;

   "picasso")
     # working path...
     MPICC_PATH=/mnt/home/soft/mpich/programs/x86_64/mpich-3.3.1/bin/mpicc
     INSTALL_PATH=$HOME/bin/

     # load modules...
     module load mpich/3.3.1_gcc9

     # patch for libmpfr.so.4
       rm -fr $INSTALL_PATH/base
     mkdir -p $INSTALL_PATH/base/lib
     rm -fr                          $INSTALL_PATH/base/lib/libmpfr.so.4
     ln -s  /usr/lib64/libmpfr.so.6  $INSTALL_PATH/base/lib/libmpfr.so.4
     ln -s  /usr/lib64/libslurm.so   $INSTALL_PATH/base/lib/libslurm.so.32
     export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$INSTALL_PATH/base/lib/
     ;;

   "lab21")
     # working path...
     MPICC_PATH=/opt/software/install-mpich/bin/mpicc
     INSTALL_PATH=/opt/

     # install software (if needed)...
     install_if_not_installed
     ;;

    "tucan")
     # working path...
     MPICC_PATH=/home/dcamarma/mpich-install/bin/mpicc
     INSTALL_PATH=/home/dcamarma/bin/

     # install software (if needed)...
     install_if_not_installed
     ;;

    "generic")
     # working path...
     MPICC_PATH=/usr/bin/mpicc
     INSTALL_PATH=$HOME/bin/

     # install software (if needed)...
     install_if_not_installed
     ;;

   *)
     echo " ERROR: unknown platform '"$1"' :-("
     usage
     exit
     ;;
esac

# 3) preconfigure build-me...
BASE_PATH=$(dirname $0)
$BASE_PATH/../build-me.sh         -m $MPICC_PATH -i $INSTALL_PATH
$BASE_PATH/build-me-benchmarks.sh -m $MPICC_PATH -i $INSTALL_PATH

# Stop
echo " End."

