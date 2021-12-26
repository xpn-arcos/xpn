#!/bin/sh
set -x

#
#
#
PATOS="pato11 pato21 pato31 pato41 pato51 pato61 pato71 pato81"

#
#
#
#if [ $# -gt 3 ]; then
#	echo "Usage: crun.sh <command>"
#	exit;
#fi

for II in $PATOS; do

	/usr/bin/ssh $II /usr/bin/killall  $PWD/dataServer
	#/usr/bin/ssh $II $PWD/dataServer &
done
