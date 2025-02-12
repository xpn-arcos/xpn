#!/bin/sh
set -x

export XPN_CONF="${HOME}/expand/test/xpn/xpn.conf.example.xml"
export XPN_PROFILE="${HOME}/expand/test/xpn/xpn.profile.example.xml"
export IOPROXY_FILE="${HOME}/expand/test/xpn/xpn.dns"
export MYSERVER_FILE="${HOME}/expand/test/xpn/xpn.dns"

 ./cp-local2xpn.exe /tmp/test.tmp /PNFS4/storage/kkk1
#./cp-xpn2local.exe               /PNFS4/stroage/kkk1 test1.tmp

