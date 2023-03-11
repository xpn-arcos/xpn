#!/bin/sh
set -x

export XPN_CONF=$HOME/expand/test/xpn/xpn.conf.example.xml
export MYSERVER_FILE=$HOME/expand/test/xpn/xpn.dns

./d2xpn.exe  test.tmp         /PNFS3/tmp/kkk1
./xpn2d.exe  /PNFS3/tmp/kkk1  test1.tmp

