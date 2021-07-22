#!/bin/sh
#set -x


N_LINE=`wc -l xpn.conf | awk '{ print $1}'`
N_URL=`expr $N_LINE - 1` 

DO_PING=0
if [ $# -gt 0 ]; then
	DO_PING=1
fi

L=2
while [ $L -le $N_LINE ]; do

	LINE=`head -$L xpn.conf | tail -1`
	#echo $LINE

	PROTO=`echo $LINE | cut -f1 -d':'`
	IP=`echo $LINE | cut -f3 -d'/'`
	DIR=`echo $LINE | sed 's/.*:\/\/[^\/]*//g'`

	#echo $PROTO
	#echo $IP
	#echo $DIR

	echo $IP
	if [ $DO_PING -eq 1 ]; then
	        echo "------------------------------------------------------------"
		ping $IP -c 1 -n
	fi
	echo "------------------------------------------------------------"
	ssh $IP ls -las $DIR
	echo "------------------------------------------------------------"

	L=`expr $L + 1`

done

