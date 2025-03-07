#!/bin/bash
set -x

mkdir -p /tmp/work/data
src/xpn_server/xpn_server -w /tmp/work/data -m 0 -t 1 -s sck