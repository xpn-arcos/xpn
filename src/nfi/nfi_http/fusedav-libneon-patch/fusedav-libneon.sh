#!/bin/sh
set -x

# Composed of the following files:
# 
# fusedav-c20061002001751.patch
# fusedav-c20061002001751.tar.gz
# fusedav-libneon.sh
# fusedav-libneon-xpn.patch
# libneon-r1801.patch
# libneon-r1801.tar.gz
# partial-get-put.txt
# 
# List of added/modified files by the patches:
# 
# fusedav:
# 
# configure.ac # for Expand
# Makefile.am # for Expand
# src/Makefile.am
# src/filecache.c
# src/filecache.h
# src/remoteaccess.c
# src/remoteaccess.h
# fusedav.c
# 
# 
# libneon:
# 
# configure.gnu # for Expand
# Makefile.in # for Expand
# src/ne_basic.c
# src/ne_basic.h
# src/ne_request.c
# src/ne_request.h


# It works with the following 'fusedav' commit:
# 
# commit c21f1d7baabba32a55eb163bdb0300477fc78bdf
# Author: Lennart Poettering <lennart@poettering.net>
# Date:   Mon Oct 2 00:17:51 2006 +0000
# 
# git clone git://git.0pointer.de/fusedav.git
# cd fusedav; git checkout -b c20061002001751 c21f1d7baabba32a55eb163bdb0300477fc78bdf; cd ..

tar zxvf fusedav-c20061002001751.tar.gz; mv fusedav-c20061002001751 fusedav;

cd fusedav
rm -rf `find . -name "*.git"`
patch -p1 < ../fusedav-c20061002001751.patch


# It works with 'libneon' revision 1801.
# 
# svn co -r1801 http://svn.webdav.org/repos/projects/neon/trunk libneon/

tar zxvf ../libneon-r1801.tar.gz; mv libneon-r1801 libneon

cd libneon
rm -rf `find . -name "*.svn"`
patch -p1 < ../../libneon-r1801.patch
cd ..

patch -p1 < ../fusedav-libneon-xpn.patch
./bootstrap.sh

cd libneon
./autogen.sh
cd ..


./configure NEON_CFLAGS="-D_LARGEFILE64_SOURCE -DNE_LFS -I../libneon/src" NEON_LIBS="-L../libneon/src/.libs -lneon" LIBS="-lexpat -lgssapi_krb5 -lssl"
make

