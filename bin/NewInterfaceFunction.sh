#!/bin/sh
#set -x

# 
#  Params
# 

if [ $# -lt 4 ]; then
        echo ""
        echo "  NewInterfaceFunction"
        echo " ----------------------"
        echo ""
	echo " Usage: $0 '<return type>' '<function name>' '<function parameters>' '<macro parameters>'"
        echo ""
        echo " Example:"
        echo "  \$ cd Expand-2.0/"
        echo "  \$ $0 'int' 'getVersion' 'int fd' 'mfd'"
        echo ""
	exit
fi


#
#
#
FNAME_UPPERCASE=`echo "$2" |  awk '{ print toupper($1) }'`
NIFASPOINTER=$1"\\t (*fsi_"$2")\\t( "$3" ) ;"
NIFASFUNCTION=$1"\\t  "$2"\\t\\t( "$3" ) ;"
NIFASFUNCTION2=$1" "$2" ( "$3" ) "
NIFASMACRO1="#define xpni_fsit_FSI_$FNAME_UPPERCASE($4) \\\\\n              ((xpni_fsit_get($4)->fsi_$2)($4))"
NIFASFUNCTION5=$1"\\t  xpni_fsit_"$2"\\t\\t( "$3" ) ;"


#
#
#
echo -n " Please, wait a moment "
echo -n " [."

cat > bin/NewInterfaceFunction.sed << EOF1

s/\/\*#NewInterfaceFunction\.AsPointer#\*\//$NIFASPOINTER         \/\*#NewInterfaceFunction\.AsPointer#\*\//g

s/\/\*#NewInterfaceFunction\.AsFunction#\*\//$NIFASFUNCTION5      \/\*#NewInterfaceFunction\.AsFunction#\*\//g

s/\/\*#NewInterfaceFunction\.AsMacro1#\*\//$NIFASMACRO1      \/\*#NewInterfaceFunction\.AsMacro1#\*\//g

EOF1

echo -n "."
sed -f bin/NewInterfaceFunction.sed include/xpni/common/xpni_fsit.h > /tmp/NewIntFac.$$
cp -a include/xpni/common/xpni_fsit.h include/xpni/common/xpni_fsit.h.`date +%F-%s`
mv -f /tmp/NewIntFac.$$ include/xpni/common/xpni_fsit.h
rm -fr bin/NewInterfaceFunction.sed



#
#
#
echo -n "."

PREFIX=`ls -1d src/xpni/xpni_*|sed 's/src\/xpni\/xpni_//g'|sed 's/\///g'`
for P in $PREFIX; do

	echo -n "."

	MBFILE="src/xpni/xpni_"$P"/.MakeBones"
	MBFILE2="src/.MakeBones"
	SRCFILE="src/xpni/xpni_"$P"/xpni_"$P"_"$2".c"
	SRCFILE2="xpni_"$P"_"$2".c"
	OBJFILE="xpni/xpni_"$P"/xpni_"$P"_"$2".o"
        NIFASFUNCTION3=$1" xpni_"$P"_"$2" ( "$3" ) "
        NIFASNAME3=" xpni_"$P"_"$2
        NIFASNAME4=" xpni_lowfsi_"$2

cat > $SRCFILE << EOF2



   /* ... Include / Inclusion ........................................... */

      #include "xpni/xpni_$P/xpni_$P.h"


   /* ... Functions / Funciones ......................................... */

      $NIFASFUNCTION3
      {
	       
        /* debugging */
        #if defined(XPNI_DEBUG)
            printf("[%s:%d] $NIFASNAME3(...); \n",
                   __FILE__,__LINE__);
        #endif

        return xpni_lowfsi_$NIFASNAME4(xpni_fit_get_XPN_DATA_FD(fd));

      }


   /* ................................................................... */

EOF2

	cp -a $MBFILE   $MBFILE.`date +%F-%s`
	echo $SRCFILE2 >> $MBFILE
	cp -a $MBFILE2  $MBFILE2.`date +%F-%s`
	echo $OBJFILE  >> $MBFILE2

	NIFASFUNCTION3=$1"\\t  xpni_"$P"_"$2"\\t\\t( "$3" ) ;"

cat > bin/NewInterfaceFunction.sed << EOF3

s/\/\*#NewInterfaceFunction\.AsFunction3#\*\//$NIFASFUNCTION3      \/\*#NewInterfaceFunction\.AsFunction3#\*\//g

EOF3

        FSI_HEADER="include/xpni/xpni_$P/xpni_$P.h"
	sed -f bin/NewInterfaceFunction.sed $FSI_HEADER > /tmp/NewIntFac.$$
	cp -a  $FSI_HEADER $FSI_HEADER.`date +%F-%s`
	mv -f  /tmp/NewIntFac.$$ $FSI_HEADER
	rm -fr bin/NewInterfaceFunction.sed

done


#
#
#
echo -n ".]"
echo ""
echo " Done."

