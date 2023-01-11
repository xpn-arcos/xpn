#!/bin/bash
set -x


# Try 1
#env LD_PRELOAD=../../../src/bypass/xpn_bypass.so:$LD_PRELOAD \
#    XPN_CONF=./xpn.conf \
#    python3 ./main.py


# Try 2
gcc -fPIC -shared -o xpn_py.so xpn_py.c

env XPN_CONF=./xpn.conf \
    python3 ./main.py

rm -fr xpn_py.so
rm -fr demo.txt

