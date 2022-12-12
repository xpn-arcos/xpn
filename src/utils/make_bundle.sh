#!/bin/sh
#set -x

if [ $# -ge 1 ]
then
	FILES=$@
else
	FILES=`/bin/ls -1 *.c | sed s/.c$//g`
fi

echo "Making bundle of $FILES"

for f in $FILES
do
	if [ ! -f $f ]
	then
		echo "Warning: Binary '$f' does not exist!"
	fi
done

LIBS="`ldd $FILES | grep '=>' | cut -d '>' -f 2- | cut -d '(' -f 1 | sort | uniq`"
if echo $LIBS | grep globus
then
	LIBS="$LIBS /usr/lib/libglobus_thread_pthread.so"
fi

mkdir xpn_utils_bundle
cp $FILES xpn_utils_bundle/
cp $LIBS xpn_utils_bundle/
tar zcvf xpn_utils_bundle.tar.gz xpn_utils_bundle/
rm -rf xpn_utils_bundle/
