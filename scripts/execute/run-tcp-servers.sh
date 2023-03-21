#!/bin/bash
set -x

echo "Individual tcp_server"

/work/xpn/src/tcp_server/xpn_tcp_server \
        -ns /export/tcp_server.dns \
        -p 3456

exit

echo "Several tcp_server with MPI"

mpirun -np 2 \
	-machinefile machinefile \
	/work/xpn/src/tcp_server/xpn_tcp_server \
	-ns /export/tcp_server.dns \
	-p 3456

exit

echo "Using docker u20.sh"

./u20.sh start 4
./u20.sh network | grep -v "Show" > machinefile
./u20.sh mpirun 2 "/work/xpn/src/tcp_server/xpn_tcp_server -ns /export/tcp_server.dns -p 3456" &
./u20.sh mpirun 2 "/work/xpn/test/integrity/xpn/run.sh"
./u20.sh stop

