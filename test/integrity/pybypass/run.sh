#!/bin/bash
set -x

export XPN_CONF=./xpn.conf

# Try 1
echo " ### PRELOAD ####################################"
#env LD_PRELOAD=../../../src/bypass/xpn_bypass.so:$LD_PRELOAD \
LD_PRELOAD=../../../src/bypass/xpn_bypass.so python3 ./main_preload.py


# Try 2
echo " ### DECORATOR ##################################"
gcc -fPIC -shared -o py_xpn.so py_xpn.c

python3 ./main_decorator.py


# Clean
echo " ################################################"
rm -fr py_xpn.so
rm -fr demo.txt

