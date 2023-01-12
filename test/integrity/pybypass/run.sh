#!/bin/bash
set -x

export XPN_CONF=./xpn.conf

# Try 1
echo " ### PRELOAD ####################################"
#env LD_PRELOAD=../../../src/bypass/xpn_bypass.so:$LD_PRELOAD \
LD_PRELOAD=../../../src/bypass/xpn_bypass.so python3 ./main_preload.py


# Try 2
echo " ### DECORATOR ##################################"
gcc -fPIC -shared -o xpn_py.so xpn_py.c

python3 ./main_decorator.py


# Clean
echo " ################################################"
rm -fr xpn_py.so
rm -fr demo.txt

