#!/bin/bash
set -x


# Option 1: preload from XPN
echo "### PRELOAD ####################################"
env LD_PRELOAD="../../../src/bypass/xpn_bypass.so:../../../../mxml/libmxml.so.1:$LD_PRELOAD"  XPN_CONF="./xpn.conf"  open-write-close /tmp/expand/P1/demo.txt  1
env LD_PRELOAD="../../../src/bypass/xpn_bypass.so:../../../../mxml/libmxml.so.1:$LD_PRELOAD"  XPN_CONF="./xpn.conf"  open-write-close /tmp/expand/P1/demo.txt  2
env LD_PRELOAD="../../../src/bypass/xpn_bypass.so:../../../../mxml/libmxml.so.1:$LD_PRELOAD"  XPN_CONF="./xpn.conf"  open-write-close /tmp/expand/P1/demo.txt  4
env LD_PRELOAD="../../../src/bypass/xpn_bypass.so:../../../../mxml/libmxml.so.1:$LD_PRELOAD"  XPN_CONF="./xpn.conf"  open-write-close /tmp/expand/P1/demo.txt  8
env LD_PRELOAD="../../../src/bypass/xpn_bypass.so:../../../../mxml/libmxml.so.1:$LD_PRELOAD"  XPN_CONF="./xpn.conf"  open-write-close /tmp/expand/P1/demo.txt  16
echo "################################################"

# Clean
rm -fr /tmp/demo.txt

