#!/bin/bash
set -x

mkdir -p /tmp/work/data
src/xpn_server/xpn_server -d /tmp/work/data -p 3456 -m 0 -t 1

