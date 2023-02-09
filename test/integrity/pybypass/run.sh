#!/bin/bash
#set -x


# Option 1: preload from XPN
echo "### PRELOAD ####################################"

echo "action:"
echo "env LD_PRELOAD=../../../src/bypass/xpn_bypass.so:\$LD_PRELOAD  XPN_CONF=./xpn.conf  python3 ./main_preload.py"

echo "output:"
      env LD_PRELOAD=../../../src/bypass/xpn_bypass.so:$LD_PRELOAD   XPN_CONF=./xpn.conf  python3 ./main_preload.py


# Option 2: preload in python with decorator
echo "### DECORATOR ##################################"

echo "action:"
gcc -fPIC -shared -o py_xpn.so py_xpn.c
echo "python3 ./main_decorator.py"

echo "output:"
      python3 ./main_decorator.py


# Clean
echo "################################################"
rm -fr py_xpn.so
rm -fr demo.txt

