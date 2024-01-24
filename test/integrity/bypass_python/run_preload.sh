#!/bin/bash
#set -x


# Option 1: preload from XPN
echo "### PRELOAD ####################################"

echo "action:"
echo "env LD_PRELOAD=../../../src/bypass/xpn_bypass.so:\$LD_PRELOAD  XPN_CONF=./xpn.conf  python3 ./main_preload.py /tmp/expand/P1/demo.txt"

echo "output:"
      env LD_PRELOAD="../../../src/bypass/xpn_bypass.so:../../../../mxml/libmxml.so.1:$LD_PRELOAD"  XPN_CONF=./xpn.conf  python3 ./main_preload.py /tmp/expand/P1/demo.txt


# Clean
echo "################################################"
rm -fr /tmp/demo.txt

