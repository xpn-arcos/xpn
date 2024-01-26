#!/bin/bash
set -x

host_to_kill=$(tail -n 1 "/shared/dns.txt" | awk '{print $2}' | sed 's/.*://')
echo "Terminate server $i: $host_to_kill"
/home/lab/src/xpn/scripts/execute/xpn.sh -w /shared -x /tmp/ -p 1 -k $host_to_kill terminate 
