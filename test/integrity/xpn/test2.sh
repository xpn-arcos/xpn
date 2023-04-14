#!/bin/bash

rm /work/data/test_1
rm /work/data2/test_1

make clean;make -j;clear

# MQTT

export XPN_DNS=/work/tcp_server.dns
export XPN_LOCALITY=0
export XPN_SESSION=0
export XPN_MQTT=0

#
# 128 - 1000 - 512k
#
export XPN_CONF=./xpn.conf

./open-write-close 1000 > t1_tcp_128_1000_512k.txt

sleep 2

stat -c '%s' /work/data/test_1 /work/data2/test_1 | awk 'BEGIN {sum=0}   {sum = sum + $1 }   END { print sum } ' > file_tcp_128_1000_512k.txt

rm /work/data/test_1
rm /work/data2/test_1

#
# 128 - 10000 - 512k
#

./open-write-close 10000 > t1_tcp_128_10000_512k.txt

sleep 2

stat -c '%s' /work/data/test_1 /work/data2/test_1 | awk 'BEGIN {sum=0}   {sum = sum + $1 }   END { print sum } ' > file_tcp_128_10000_512k.txt

rm /work/data/test_1
rm /work/data2/test_1

#
# 128 - 100000 - 512k
#

./open-write-close 100000 > t1_tcp_128_100000_512k.txt

sleep 2

stat -c '%s' /work/data/test_1 /work/data2/test_1 | awk 'BEGIN {sum=0}   {sum = sum + $1 }   END { print sum } ' > file_tcp_128_100000_512k.txt
