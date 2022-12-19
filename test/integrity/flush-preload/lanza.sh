#!/bin/sh
set -x
export XPN_CONF=/home/lmsan/expand/test/xpn/xpn.conf.example.xml
export XPN_PROFILE=/home/lmsan/expand/test/xpn/xpn.profile.example.xml
export IOPROXY_FILE=/home/lmsan/expand/test/xpn/xpn.dns
export MYSERVER_FILE=/home/lmsan/expand/test/xpn/xpn.dns
./d2xpn.exe test.tmp /PNFS4/storage/kkk1
./xpn2d.exe /PNFS4/stroage/kkk1 test1.tmp
