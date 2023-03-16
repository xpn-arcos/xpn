#!/bin/bash
set -x

mpirun -np 2 \
	-machinefile machinefile \
	/work/xpn/src/tcp_server/xpn_tcp_server \
	-ns /export/tcp_server.dns \
	-p 3456
