#!/bin/sh
# shellcheck disable=all
#set -x


#
#
#
if [ $# -lt 1 ]; then
	echo "Usage: ping-xpn <conf file>"
	exit
fi

DO_PING=0
if [ $# -gt 2 ]; then
	DO_PING=1
fi


#
#
#
XPN_CONF=$1
N_LINE=`wc -l $XPN_CONF | awk '{ print $1}'`
N_URL=`expr $N_LINE - 1` 

L=2
while [ $L -le $N_LINE ]; do

	LINE=`head -$L $XPN_CONF | tail -1`
	#echo $LINE

	PROTO=`echo $LINE | cut -f1 -d':'`
	IP=`echo $LINE | cut -f3 -d'/'`
	DIR=`echo $LINE | sed 's/.*:\/\/[^\/]*//g'`

	#echo $PROTO
	#echo $IP
	#echo $DIR

	echo "------------------------------------------------------------"
	echo $IP
	if [ $DO_PING -eq 1 ]; then
	        echo "------------------------------------------------------------"
		ping $IP -c 1 -n
	fi
	echo "------------------------------------------------------------"
	ssh $IP ls -las $DIR

	L=`expr $L + 1`

done

echo "------------------------------------------------------------"

