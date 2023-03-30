#!/bin/bash


#
# alternative - 1
#
export XPN_CONF=./xpn.conf
export XPN_DNS=/tmp/tcp_server.dns
export XPN_LOCALITY=0
export XPN_SESSION=1
export XPN_MQTT=1

rm -fr /export/data/d4
rm -fr /export/data2/d4

sleep 3

./mkdir2

