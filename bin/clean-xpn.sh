#!/bin/sh
#set -x


#
#
#
if [ $# -lt 1 ]; then
	echo "Usage: ping-xpn <conf file>"
	exit
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

	echo "--- "$L"/"$N_LINE" ---------------------------------------------------------"
	echo -n $IP"..."
	ssh $IP rm -fr $DIR/*
	echo " done."

	L=`expr $L + 1`

done

echo "------------------------------------------------------------"

