#!/bin/bash


#
# alternative - 1
#
export XPN_CONF=./xpn.conf
export XPN_DNS=/work/export/tcp_server.dns
export XPN_LOCALITY=0

rm -fr /export/data/d4
rm -fr /export/data2/d4

sleep 3

./mkdir2

