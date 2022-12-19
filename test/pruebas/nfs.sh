#!/bin/bash
set -x

MPI="pato11 pato21 pato31 pato41 pato51 pato61 pato71 pato81"

for I in $MPI; do
	rsh "$I" /etc/init.d/nfs-common restart
done
