#!/bin/bash
#set -x

# arguments
if [ "$#" != 2 ]; then
    echo "Usage: $0 [mn | picasso | linux]"
    exit
fi

# initial configuration...
case $1 in
   "mn")
     MPICH_PATH=/gpfs/apps/MN4/INTEL/2017.4/compilers_and_libraries_2017.4.196/linux/mpi/intel64/
     INSTALL_PATH=/home/uc3m15/uc3m15672/bin/
     ;;
   "picasso")
     MPICH_PATH=/mnt/home/soft/mpich/programs/x86_64/mpich-3.3.1/
     INSTALL_PATH=/mnt/home/users/uc3m15_res/uc3m15672/bin/
     ;;
   *)
     MPICH_PATH=/opt/software/install-mpich/
     INSTALL_PATH=/opt/xpn
     ;;
esac


# Start
echo "Begin."

# pre-requisites
echo "1) check packages..."
case $1 in
   "mn")
     module load "impi/2017.4"
     ;;
   "picasso")
     module load mpich/3.3.1_gcc9
     ;;
   *)
     PKG_NAMES="autoconf automake gcc g++ make flex libtool doxygen libmpich-dev libmxml-dev"
     for P in $PKG_NAMES; do
         apt-mark showinstall | grep -q "^$P$" || sudo apt-get install -y $P
     done
     ;;
esac

# directories
echo "2) preparing install directories..."
  rm -fr $INSTALL_PATH/xpn
mkdir -p $INSTALL_PATH/xpn/lib64
ln    -s $INSTALL_PATH/xpn/lib64   $INSTALL_PATH/xpn/lib

  rm -fr $INSTALL_PATH/mxml
mkdir -p $INSTALL_PATH/mxml/lib64
ln    -s $INSTALL_PATH/mxml/lib64  $INSTALL_PATH/mxml/lib

# 1) MXML
echo "3) preparing mxml..."
cd ../mxml
./configure --prefix=$INSTALL_PATH/mxml
make clean
make -j 8
make install

## 2) XPN
echo "4) preparing xpn..."
cd ../xpn
ACLOCAL_FLAGS="-I /usr/share/aclocal/" autoreconf -v -i -s -W all
./configure --prefix=$INSTALL_PATH/xpn --enable-nfs3 --enable-tcpserver --enable-mpiserver="$MPICH_PATH/bin"
make clean
make -j 8
#doxygen doc/doxygen-XPN.cfg
make install

# Stop
echo "End."

