#!/bin/sh
set -x

# 
#  Params
# 

if [ $# -lt 1 ]; then
        echo ""
        echo "  NewInterface"
        echo " --------------"
        echo ""
	echo " Usage: $0 <prefix for this new interface>"
        echo ""
        echo " Example:"
        echo "  \$ cd Expand-2.0/src/xpni"
        echo "  \$ $0 blue"
        echo "  \$ ls -las blue_xpn"
        echo "  Makefile"
        echo "  Makefile.in"
        echo "  xpn_blue_close.c"
        echo "  xpn_blue_destroy.c"
        echo "  xpn_blue_init.c"
        echo "  xpn_blue_open.c"
        echo "  xpn_blue_read.c"
        echo "  xpn_blue_write.c"
        echo "  xpn_blue_register.c"
        echo "  ..."
        echo ""
	exit
fi

INAME=$1
IDIR="xpni_"$INAME
NDIR="xpni_null"

 
#
# Copy and modify
#

mkdir -p $IDIR
echo " Please, wait a moment..."

FILES=`ls -1 $NDIR`
FILES=$FILES" .MakeBones"
for F in $FILES; do

	NF=`echo $F|sed 's/null/'$INAME'/g'`
	sed 's/null/'$INAME'/g' $NDIR/$F > $IDIR/$NF

done
echo " Done."

