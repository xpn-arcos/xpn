#!/bin/bash
#set -x

usage () {
	echo "Usage:"
	echo "	$1 <module_name>"
}

# Usage: insert_text_before_delimiter "This is a nice text to insert" "# THIS IS A FANCY DELIMITER #" "file.c"
# Usage: insert_text_before_delimiter "$TEXT" "$DELIMITER" "$FILE"
insert_text_before_delimiter () {
	TEXT=$1
	DELIMITER=$2
	FILE=$3
	
	DELIM_LN=`grep -n -x -m 1 "$DELIMITER" $FILE | cut -f 1 -d :`
	if [ -z $DELIM_LN ]
	then
		echo "Error: Delimiter not found: $DELIMITER"
		exit
	fi
	TOTAL_FILE_LINES=`cat $FILE | wc -l`
	LINES_BEFORE=`expr $DELIM_LN - 1`
	LINES_AFTER=`expr $TOTAL_FILE_LINES - $LINES_BEFORE`
	
	head -n $LINES_BEFORE $FILE > tmp.$$
	echo -e $TEXT >> tmp.$$
	tail -n $LINES_AFTER $FILE >> tmp.$$
	mv tmp.$$ $FILE
}

create_include_structure () {
	echo "Creating include/nfi_${MODULE} structure"
	
	mkdir ${INCLUDE_DIR}/nfi_${MODULE}
	
	echo "Copying include files"
	
	cp ${INCLUDE_DIR}/nfi_$TEMPLATE_MODULE/nfi_${TEMPLATE_MODULE}.h ${INCLUDE_DIR}/nfi_$MODULE/nfi_${MODULE}.h
	cp ${INCLUDE_DIR}/nfi_$TEMPLATE_MODULE/nfi_${TEMPLATE_MODULE}_err.h ${INCLUDE_DIR}/nfi_$MODULE/nfi_${MODULE}_err.h
	cp ${INCLUDE_DIR}/nfi_$TEMPLATE_MODULE/${TEMPLATE_MODULE}_lib.h ${INCLUDE_DIR}/nfi_$MODULE/${MODULE}_lib.h
	
	OLD_DIR=$PWD
	cd ${INCLUDE_DIR}/nfi_$MODULE/
	
	echo "Modifing nfi_${MODULE}.h"
	
	FILE=nfi_${MODULE}.h
	
	cat $FILE | sed s/$TEMPLATE_MODULE_UPPERCASE/$MODULE_UPPERCASE/g > tmp.$$
	mv tmp.$$ $FILE
	
	cat $FILE | sed s/nfi_$TEMPLATE_MODULE/nfi_$MODULE/g > tmp.$$
	mv tmp.$$ $FILE
	
	echo "Modifing nfi_${MODULE}_err.h"
	
	FILE=nfi_${MODULE}_err.h
	
	cat $FILE | sed s/$TEMPLATE_MODULE_UPPERCASE/$MODULE_UPPERCASE/g > tmp.$$
	mv tmp.$$ $FILE
	
	cat $FILE | sed s/${TEMPLATE_MODULE}_err/${MODULE}_err/g > tmp.$$
	mv tmp.$$ $FILE
	
	echo "Modifing ${MODULE}_lib.h"
	
	FILE=${MODULE}_lib.h
	
	cat $FILE | sed s/$TEMPLATE_MODULE_UPPERCASE/$MODULE_UPPERCASE/g > tmp.$$
	mv tmp.$$ $FILE
	
	cat $FILE | sed s/nfi_$TEMPLATE_MODULE/nfi_$MODULE/g > tmp.$$
	mv tmp.$$ $FILE
	
	cd $OLD_DIR
}

create_src_structure () {
	echo "Creating src/nfi/nfi_$MODULE structure"
	
	mkdir ${SRC_DIR}/nfi_$MODULE
	
	echo "Copying source files"
	
	cp ${SRC_DIR}/nfi_$TEMPLATE_MODULE/nfi_$TEMPLATE_MODULE.c ${SRC_DIR}/nfi_$MODULE/nfi_$MODULE.c
	cp ${SRC_DIR}/nfi_$TEMPLATE_MODULE/nfi_${TEMPLATE_MODULE}_err.c ${SRC_DIR}/nfi_$MODULE/nfi_${MODULE}_err.c
	
	OLD_DIR=$PWD
	cd ${SRC_DIR}/nfi_$MODULE
	
	echo "Modifing nfi_$MODULE.c"
	
	FILE=nfi_$MODULE.c
	
	cat $FILE | sed s/nfi_$TEMPLATE_MODULE/nfi_$MODULE/g > tmp.$$
	mv tmp.$$ $FILE
	
	cat $FILE | sed s/${TEMPLATE_MODULE}_lib.h/${MODULE}_lib.h/g > tmp.$$
	mv tmp.$$ $FILE
	
	cat $FILE | sed s/${TEMPLATE_MODULE}_err/${MODULE}_err/g > tmp.$$
	mv tmp.$$ $FILE
	
	cat $FILE | sed s/${TEMPLATE_MODULE}hilos/${MODULE}hilos/g > tmp.$$
	mv tmp.$$ $FILE
	
	cat $FILE | sed s/$TEMPLATE_MODULE_UPPERCASE/$MODULE_UPPERCASE/g > tmp.$$
	mv tmp.$$ $FILE
	
	echo "Modifing nfi_${MODULE}_err.c"
	
	FILE=nfi_${MODULE}_err.c
	
	cat $FILE | sed s/nfi_$TEMPLATE_MODULE/nfi_$MODULE/g > tmp.$$
	mv tmp.$$ $FILE
	
	cat $FILE | sed s/${TEMPLATE_MODULE}_lib.h/${MODULE}_lib.h/g > tmp.$$
	mv tmp.$$ $FILE
	
	cat $FILE | sed s/${TEMPLATE_MODULE}_err/${MODULE}_err/g > tmp.$$
	mv tmp.$$ $FILE
	
	cat $FILE | sed s/$TEMPLATE_MODULE_UPPERCASE/$MODULE_UPPERCASE/g > tmp.$$
	mv tmp.$$ $FILE
	
	cd $OLD_DIR
}

create_configure_ac_structure () {
	echo "Modifing configure.ac structure"
	
	TEXT="### BEGIN OF $MODULE_UPPERCASE BLOCK. Do not remove this line. ###\n\
#\n\
# Defines the necessary variables if $MODULE is enabled.\n\
#\n\
AC_ARG_ENABLE(\t[${MODULE}],\n\
\t\t[AS_HELP_STRING([--enable-${MODULE}@<:@=/path/to/${MODULE}/@:>@ (Don't use '~')],[Enable $MODULE module.])],\n\
\t\t[\n\
\t\t\t[CDEFS=\"\$CDEFS -DENABLE_${MODULE_UPPERCASE}\"]\n\
\t\t\t[INCLUDEFLAGS=\"\$INCLUDEFLAGS -I\\\$(top_srcdir)/include/nfi/nfi_${MODULE}\"]\n\
\t\t\t#[LIBS=\"\$LIBS -L\$enable_${MODULE}/lib/ -l${MODULE}\"]\n\
\t\t\t#[M_HEADERS=\"\$M_HEADERS -I\$enable_${MODULE}/include/\"]\n\
\t\t\t[NFI_${MODULE_UPPERCASE}=\"nfi_${MODULE}\"]\n\
\t\t\t[NFI_${MODULE_UPPERCASE}_OBJECTS=\"\\\$(NFI_${MODULE_UPPERCASE}_OBJECTS)\"]\n\
\t\t]\n\
)\n\
AC_SUBST(NFI_${MODULE_UPPERCASE})\n\
AC_SUBST(NFI_${MODULE_UPPERCASE}_OBJECTS)\n\
AM_CONDITIONAL([ENABLE_${MODULE_UPPERCASE}], [test \"\$NFI_${MODULE_UPPERCASE}\" = \"nfi_${MODULE}\"])\n\
### END OF $MODULE_UPPERCASE BLOCK. Do not remove this line. ###"
	DELIMITER="### END OF AC_ARG_ENABLE BLOCK. Do not remove this line. ###"
	FILE="configure.ac"
	
	insert_text_before_delimiter "$TEXT" "$DELIMITER" "$FILE"
}

create_makefile_am_structures () {
	echo "Modifing lib/Makefile.am structure"
	
	# NFI_MODULE_HEADER BLOCK
	TEXT="### BEGIN OF NFI_${MODULE_UPPERCASE}_HEADER BLOCK. Do not remove this line. ###\n\
NFI_${MODULE_UPPERCASE}_HEADER=\t@top_srcdir@/include/nfi/nfi_${MODULE}/nfi_${MODULE}_err.h \\\\\n\
\t\t\t@top_srcdir@/include/nfi/nfi_${MODULE}/nfi_${MODULE}.h \\\\\n\
\t\t\t@top_srcdir@/include/nfi/nfi_${MODULE}/${MODULE}_lib.h\n\
### END OF NFI_${MODULE_UPPERCASE}_HEADER BLOCK. Do not remove this line. ###"
	DELIMITER="### END OF NFI_MODULE_HEADER BLOCK. Do not remove this line. ###"
	FILE="lib/Makefile.am"
	
	insert_text_before_delimiter "$TEXT" "$DELIMITER" "$FILE"
	
	# NFI_HEADER BLOCK
	TEXT="### BEGIN OF ENABLE_${MODULE_UPPERCASE}_HEADER BLOCK. Do not remove this line. ###\n\
if ENABLE_${MODULE_UPPERCASE}\n\
NFI_HEADER+=\$(NFI_${MODULE_UPPERCASE}_HEADER)\n\
endif\n\
### END OF ENABLE_${MODULE_UPPERCASE}_HEADER BLOCK. Do not remove this line. ###"
	DELIMITER="### END OF NFI_HEADER BLOCK. Do not remove this line. ###"
	FILE="lib/Makefile.am"
	
	insert_text_before_delimiter "$TEXT" "$DELIMITER" "$FILE"
	
	# NFI_MODULE_OBJECTS BLOCK
	TEXT="### BEGIN OF NFI_${MODULE_UPPERCASE}_OBJECTS BLOCK. Do not remove this line. ###\n\
NFI_${MODULE_UPPERCASE}_OBJECTS=\t@top_srcdir@/src/nfi/nfi_${MODULE}/nfi_${MODULE}.c \\\\\n\
\t\t\t@top_srcdir@/src/nfi/nfi_${MODULE}/nfi_${MODULE}_err.c\n\
### END OF NFI_${MODULE_UPPERCASE}_OBJECTS BLOCK. Do not remove this line. ###"
	DELIMITER="### END OF NFI_MODULE_OBJECTS BLOCK. Do not remove this line. ###"
	FILE="lib/Makefile.am"
	
	insert_text_before_delimiter "$TEXT" "$DELIMITER" "$FILE"
	
	# NFI_OBJECTS BLOCK
	TEXT="### BEGIN OF ENABLE_${MODULE_UPPERCASE}_OBJECTS BLOCK. Do not remove this line. ###\n\
if ENABLE_${MODULE_UPPERCASE}\n\
NFI_OBJECTS+=\$(NFI_${MODULE_UPPERCASE}_OBJECTS)\n\
endif\n\
### END OF ENABLE_${MODULE_UPPERCASE}_OBJECTS BLOCK. Do not remove this line. ###"
	DELIMITER="### END OF NFI_OBJECTS BLOCK. Do not remove this line. ###"
	FILE="lib/Makefile.am"
	
	insert_text_before_delimiter "$TEXT" "$DELIMITER" "$FILE"
}

create_nfi_lib_h_structure () {
	echo "Modifing include/nfi/nfi_lib.h structure"
	
	TEXT="// BEGIN OF ENABLE_${MODULE_UPPERCASE} BLOCK. Do not remove this line. //\n\
#ifdef ENABLE_${MODULE_UPPERCASE}\n\
\t#include \"nfi_${MODULE}/${MODULE}_lib.h\"\n\
#endif\n\
// END OF ENABLE_${MODULE_UPPERCASE} BLOCK. Do not remove this line. //"
	DELIMITER="// END OF ENABLE_MODULE BLOCK. Do not remove this line. //" # FIXME: If ends with */, then it doesn't work
	FILE="include/nfi/nfi_lib.h"
	
	insert_text_before_delimiter "$TEXT" "$DELIMITER" "$FILE"
}

create_xpn_policy_init_c_structure () {
	echo "Modifing src/xpn/xpn_simple/policy/xpn_policy_init.c structure"
	
	TEXT="// BEGIN OF ENABLE_${MODULE_UPPERCASE} BLOCK. Do not remove this line. //\n\
#ifdef ENABLE_${MODULE_UPPERCASE}\n\
\telse if (strcmp(prt,\"${MODULE}\") == 0){\n\
\t\tret = nfi_${MODULE}_init(url, serv, NULL);\n\
\t\tif(ret < 0){\n\
\t\t\txpn_err(XPNERR_INITSERV);\n\
\t\t\treturn -1;\n\
\t\t}\n\
\t}\n\
#endif\n\
// END OF ENABLE_${MODULE_UPPERCASE} BLOCK. Do not remove this line. //"
	DELIMITER="// END OF ENABLE_MODULE BLOCK. Do not remove this line. //" # FIXME: If ends with */, then it doesn't work
	FILE="src/xpn/xpn_simple/policy/xpn_policy_init.c"
	
	insert_text_before_delimiter "$TEXT" "$DELIMITER" "$FILE"
}



# Main

if [ $# != 1 ]
then
	echo "ERROR: this script must receive one argument (in lower case) that will be the name of the new module."
	usage $0
	exit 1
fi

TEMPLATE_MODULE=local
TEMPLATE_MODULE_UPPERCASE=`echo "$TEMPLATE_MODULE" | tr "[a-z,-]" "[A-Z,_]"`
MODULE=$1
MODULE_UPPERCASE=`echo "$MODULE" | tr "[a-z,-]" "[A-Z,_]"`

INCLUDE_DIR=include/nfi
SRC_DIR=src/nfi
	
if [ -d ${SRC_DIR}/nfi_$MODULE ]
then
	echo "ERROR: the module $MODULE already exists!"
	exit 2
fi

create_include_structure

create_src_structure

create_nfi_lib_h_structure

create_xpn_policy_init_c_structure

create_configure_ac_structure

create_makefile_am_structures

echo ""
echo "Ready!"
echo ""
echo "List of added files:"
echo "	${INCLUDE_DIR}/nfi_$MODULE/nfi_http.h"
echo "	${INCLUDE_DIR}/nfi_$MODULE/nfi_http_err.h"
echo "	${INCLUDE_DIR}/nfi_$MODULE/http_lib.h"
echo "	${SRC_DIR}/nfi_$MODULE/nfi_http.c"
echo "	${SRC_DIR}/nfi_$MODULE/nfi_http_err.c"
echo ""
echo "List of modified files:"
echo "  include/nfi/nfi_lib.h"
echo "  src/xpn/xpn_simple/policy/xpn_policy_init.c"
echo "  configure.ac"
echo "  lib/Makefile.am"
echo ""
echo ""
echo "Write your include code under:"
echo "	${INCLUDE_DIR}/nfi_$MODULE/"
echo ""
echo "Write your src code under:"
echo "	${SRC_DIR}/nfi_$MODULE/"
echo ""
echo "Modify the AC_ARG_ENABLE directive at your desire in:"
echo "	configure.ac"
echo ""
echo "Modify the NFI_${MODULE_UPPERCASE}_HEADER and NFI_${MODULE_UPPERCASE}_OBJECTS directives at your desire in:"
echo "	lib/Makefile.am"
echo ""
#echo "Add a new protocol (#define $MODULE_UPPERCASE N) to:"
#echo "	${INCLUDE_DIR}/nfi.h"
#echo ""
