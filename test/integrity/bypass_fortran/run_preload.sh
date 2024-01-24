#!/bin/bash
#set -x


echo "action:"
echo "gfortran -g -o main_preload main_preload.f"90
echo "env LD_PRELOAD=../../../src/bypass/xpn_bypass.so:\$LD_PRELOAD  XPN_CONF=./xpn.conf  ./main_preload"

echo "output:"
      gfortran -g -o main_preload main_preload.f90
      env LD_PRELOAD="../../../src/bypass/xpn_bypass.so:../../../../mxml/libmxml.so.1:$LD_PRELOAD"  XPN_CONF=./xpn.conf  ./main_preload
      cat /tmp/expand/P1/demo.txt

# Clean
rm -fr main_preload
rm -fr /tmp/demo.txt
rm -fr /tmp/expand/P1/demo.txt

