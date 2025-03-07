#!/bin/bash


#
# alternative - 1
#
export XPN_CONF=./xpn.conf
export XPN_LOCALITY=0

./mkdir2
./rmdir
./rmdir2
./open-write-close 
./rename
./unlink 
./open-write-close 
./open-read-close 
./unlink 
./open-unlink
./creat-close-unlink

#
# alternative - 2
#

#XPN_CONF=./xpn.conf    XPN_LOCALITY=0  ./mkdir2
#XPN_CONF=./xpn.conf    XPN_LOCALITY=0  ./rmdir
#XPN_CONF=./xpn.conf    XPN_LOCALITY=0  ./rmdir2
#XPN_CONF=./xpn.conf    XPN_LOCALITY=0  ./open-write-close 
#XPN_CONF=./xpn.conf    XPN_LOCALITY=0  ./rename
#XPN_CONF=./xpn.conf    XPN_LOCALITY=0  ./unlink 
#XPN_CONF=./xpn.conf    XPN_LOCALITY=0  ./open-write-close 
#XPN_CONF=./xpn.conf    XPN_LOCALITY=0  ./open-read-close 
#XPN_CONF=./xpn.conf    XPN_LOCALITY=0  ./unlink 
#XPN_CONF=./xpn.conf    XPN_LOCALITY=0  ./open-unlink
#XPN_CONF=./xpn.conf    XPN_LOCALITY=0  ./create-close-unlink

