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
    echo ""
    echo " build-me-compact"
    echo " ----------------"
    echo ""
    echo " Usage:"
    echo " $0 <platform>"
    echo " Where:"
    echo " * platform = mn | picasso | tucan"
    echo ""
    exit
fi

# initial configuration...
case $1 in
   "mn")
     MPICH_PATH=/gpfs/apps/MN4/INTEL/2017.4/compilers_and_libraries_2017.4.196/linux/mpi/intel64/
     INSTALL_PATH=$HOME/bin/

     module load "impi/2017.4"
     ;;
   "picasso")
     MPICH_PATH=/mnt/home/soft/mpich/programs/x86_64/mpich-3.3.1/
     INSTALL_PATH=$HOME/bin/

     module load mpich/3.3.1_gcc9
     ;;
   "tucan")
     MPICH_PATH=/opt/software/install-mpich/
     INSTALL_PATH=/opt/xpn

     PKG_NAMES="autoconf automake gcc g++ make flex libtool doxygen libmpich-dev libmxml-dev"
     for P in $PKG_NAMES; do
         apt-mark showinstall | grep -q "^$P$" || sudo apt-get install -y $P
     done
     ;;
   *)
     echo "Unknown platform '"$1"'"
     exit
     ;;
esac

# preconfigure build-me...
./build-me.sh -m $MPICH_PATH -i $INSTALL_PATH


