#!/bin/bash

XPN_CONF=./xpn.conf  XPN_DNS=/tmp/tcp_server.dns  XPN_LOCALITY=0  ./mkdir2
XPN_CONF=./xpn.conf  XPN_DNS=/tmp/tcp_server.dns  XPN_LOCALITY=0  ./rmdir
XPN_CONF=./xpn.conf  XPN_DNS=/tmp/tcp_server.dns  XPN_LOCALITY=0  ./rmdir2
XPN_CONF=./xpn.conf  XPN_DNS=/tmp/tcp_server.dns  XPN_LOCALITY=0  ./open-write-close 
XPN_CONF=./xpn.conf  XPN_DNS=/tmp/tcp_server.dns  XPN_LOCALITY=0  ./rename
XPN_CONF=./xpn.conf  XPN_DNS=/tmp/tcp_server.dns  XPN_LOCALITY=0  ./unlink 
XPN_CONF=./xpn.conf  XPN_DNS=/tmp/tcp_server.dns  XPN_LOCALITY=0  ./open-write-close 
XPN_CONF=./xpn.conf  XPN_DNS=/tmp/tcp_server.dns  XPN_LOCALITY=0  ./open-read-close 
XPN_CONF=./xpn.conf  XPN_DNS=/tmp/tcp_server.dns  XPN_LOCALITY=0  ./unlink 
XPN_CONF=./xpn.conf  XPN_DNS=/tmp/tcp_server.dns  XPN_LOCALITY=0  ./open-unlink
XPN_CONF=./xpn.conf  XPN_DNS=/tmp/tcp_server.dns  XPN_LOCALITY=0  ./create-close-unlink
