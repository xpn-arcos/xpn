#!/bin/bash

# Prerequisites
#
# Intel(R) Fortran Compiler for Intel(R) EM64T-based applications, Version 8.1 Build 20060606 Package ID: l_fce_pc_8.1.037

EXPAND="$HOME/expand/trunk/expand"
MPICH="$EXPAND/test/io_bench/third-party/mpich-1.2.4-XPN-patch"
HDF5="$EXPAND/test/io_bench/third-party/hdf5-1.4.5-post8"


cd third-party/src


# Mpich 1.2.4

rm -rf mpich-1.2.4 $MPICH
tar zxvf mpich-1.2.4.tar.gz
cd mpich-1.2.4
patch -p1 < $EXPAND/romio-expand/XPN-v2.10-linux-user-patch-MPICH-1.2.4
env RSHCOMMAND="rsh" CFLAGS="-I$EXPAND/include/ -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64" LDFLAGS="-L/home/software/intel/intel_fce_80/lib" LD_LIBRARY_PATH="/home/software/intel/intel_fce_80/lib" FC="/home/software/intel/intel_fce_80/bin/ifort" FFLAGS="-w" F90="/home/software/intel/intel_fce_80/bin/ifort" F90FLAGS="-w" F90LINKER="/home/software/intel/intel_fce_80/bin/ifort" ./configure --enable-long-long --with-device=ch_p4 --with-arch=LINUX --enable-f77 --enable-f90modules --with-romio=-file_system=xpn -lib="-L$EXPAND/lib -lxpn -lpthread -L$HOME/mxml/usr/lib -lmxml -L/home/software/globus-5.2.2/lib64 -lglobus_xio -lglobus_ftp_client -lglobus_common -lssl -lcrypto -L/home/software/globus-5.2.2/lib64 -lglobus_ftp_client -lglobus_common -lssl -lcrypto -L$EXPAND/src/nfi/nfi_http/fusedav/src -lfusedav -lpthread -L$EXPAND/src/nfi/nfi_http/fusedav/libneon/src/.libs -lneon -lexpat -lgssapi_krb5 -lssl" --prefix=$MPICH 2>&1 | tee build-log.txt
make 2>&1 | tee -a build-log.txt
make install 2>&1 | tee -a build-log.txt
cd ..


# HDF5 1.4.5-post8

rm -rf hdf5-1.4.5-post8 $HDF5
tar zxvf hdf5-1.4.5-post8.tar.gz
cd hdf5-1.4.5-post8
cp src/H5detect.c src/H5detect.c.bak
sed '727{s/assert/\/*assert/;s/;/;*\//}' src/H5detect.c.bak > src/H5detect.c
env CFLAGS="-D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -DH5_HAVE_PARALLEL" CC=$MPICH/bin/mpicc ./configure --enable-parallel --disable-shared --prefix=$HDF5 2>&1 | tee build-log.txt
make 2>&1 | tee -a build-log.txt
make install 2>&1 | tee -a build-log.txt
cd ..


cd ../..
