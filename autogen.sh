#!/bin/bash

OLD_DIR=$PWD

echo -e "Entering in src/nfi/nfi_http/fusedav/"
cd src/nfi/nfi_http/fusedav/
cp /usr/share/misc/config.sub .
cp /usr/share/misc/config.guess .
NOCONFIGURE=1 ./bootstrap.sh

echo -e "\n\nEntering in src/nfi/nfi_http/fusedav/libneon/"
cd libneon/
./autogen.sh

echo -e "\n\nEntering in $OLD_DIR"
cd $OLD_DIR

#libtoolize
#aclocal
#autoconf
#autoheader
#automake --add-missing

ACLOCAL_FLAGS="-I /usr/share/aclocal/" autoreconf -v -i -s -W all -m
