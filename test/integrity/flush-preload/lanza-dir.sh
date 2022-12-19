#!/bin/sh
set -x
export XPN_CONF=/home/lmsan/expand/test/xpn/xpn.conf.example.xml
export MYSERVER_FILE=/home/lmsan/expand/test/xpn/xpn.dns
./d2xpn.exe test.tmp /PNFS3/tmp/kkk1
./xpn2d.exe /PNFS3/tmp/kkk1 test1.tmp
