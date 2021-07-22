#!/bin/sh
#set -x

usage () {
	echo "Usage:"
	echo "	$1 <module_name>"
}

delete_block () {
	BLOCK_NAME=$1
	FILE=$2
	BEGIN_COMMENT_MARK=$3
	if [ $# -ge 4 ]
	then
		END_COMMENT_MARK=$4
	else
		END_COMMENT_MARK=$BEGIN_COMMENT_MARK
	fi
	
	BEGIN_DELIMITER="$BEGIN_COMMENT_MARK BEGIN OF $BLOCK_NAME BLOCK. Do not remove this line. $END_COMMENT_MARK"
	END_DELIMITER="$BEGIN_COMMENT_MARK END OF $BLOCK_NAME BLOCK. Do not remove this line. $END_COMMENT_MARK"
	
	BEGIN_BLOCK=`grep -n -x -m 1 "$BEGIN_DELIMITER" $FILE | cut -f 1 -d :`
	if [ -z $BEGIN_BLOCK ]
	then
		echo "Error: Delimiter not found: $BEGIN_DELIMITER"
		exit
	fi
	END_BLOCK=`grep -n -x -m 1 "$END_DELIMITER" $FILE | cut -f 1 -d :`
	if [ -z $END_BLOCK ]
	then
		echo "Error: Delimiter not found: $END_DELIMITER"
		exit
	fi
	if [ $BEGIN_BLOCK -ge $END_BLOCK ]
	then
		echo "Error: BEGIN_BLOCK=$BEGIN_BLOCK >= END_BLOCK=$END_BLOCK"
		exit
	fi
	TOTAL_FILE_LINES=`cat $FILE | wc -l`
	LINES_BEFORE=`expr $BEGIN_BLOCK - 1`
	LINES_AFTER=`expr $TOTAL_FILE_LINES - $END_BLOCK`
	
	head -n $LINES_BEFORE $FILE > tmp.$$
	tail -n $LINES_AFTER $FILE >> tmp.$$
	mv tmp.$$ $FILE
}

delete_include_structure () {
	echo "Deleting ${INCLUDE_DIR}/nfi_$MODULE structure"
	
	rm -rf ${INCLUDE_DIR}/nfi_$MODULE
}

delete_src_structure () {
	echo "Deleting ${SRC_DIR}/nfi_$MODULE structure"
	
	rm -rf ${SRC_DIR}/nfi_$MODULE
}

delete_configure_ac_structure () {
	BLOCK_NAME="$MODULE_UPPERCASE"
	FILE="configure.ac"
	COMMENT_MARK="###"
	
	delete_block "$BLOCK_NAME" "$FILE" "$COMMENT_MARK"
}

delete_makefile_am_structures () {
	# NFI_MODULE_HEADER BLOCK
	BLOCK_NAME="NFI_${MODULE_UPPERCASE}_HEADER"
	FILE="lib/Makefile.am"
	COMMENT_MARK="###"
	
	delete_block "$BLOCK_NAME" "$FILE" "$COMMENT_MARK"
	
	# NFI_HEADER BLOCK
	BLOCK_NAME="ENABLE_${MODULE_UPPERCASE}_HEADER"
	FILE="lib/Makefile.am"
	COMMENT_MARK="###"
	
	delete_block "$BLOCK_NAME" "$FILE" "$COMMENT_MARK"
	
	# NFI_MODULE_OBJECTS BLOCK
	BLOCK_NAME="NFI_${MODULE_UPPERCASE}_OBJECTS"
	FILE="lib/Makefile.am"
	COMMENT_MARK="###"
	
	delete_block "$BLOCK_NAME" "$FILE" "$COMMENT_MARK"
	
	# NFI_OBJECTS BLOCK
	BLOCK_NAME="ENABLE_${MODULE_UPPERCASE}_OBJECTS"
	FILE="lib/Makefile.am"
	COMMENT_MARK="###"
	
	delete_block "$BLOCK_NAME" "$FILE" "$COMMENT_MARK"
}

delete_nfi_lib_h_structure () {
	BLOCK_NAME="ENABLE_${MODULE_UPPERCASE}"
	FILE="include/nfi/nfi_lib.h"
	BEGIN_COMMENT_MARK="//"
	END_COMMENT_MARK="//" # FIXME: If it ends with */, then it doesn't work
	
	delete_block "$BLOCK_NAME" "$FILE" "$BEGIN_COMMENT_MARK" "$END_COMMENT_MARK"
}

delete_xpn_policy_init_c_structure () {
	BLOCK_NAME="ENABLE_${MODULE_UPPERCASE}"
	FILE="src/xpn/xpn_simple/policy/xpn_policy_init.c"
	BEGIN_COMMENT_MARK="//"
	END_COMMENT_MARK="//" # FIXME: If it ends with */, then it doesn't work
	
	delete_block "$BLOCK_NAME" "$FILE" "$BEGIN_COMMENT_MARK" "$END_COMMENT_MARK"
}



# Main

if [ $# != 1 ]
then
	echo "ERROR: this script must receive one argument (in lower case) that will be the name of the module to delete."
	usage $0
	exit 1
fi

MODULE=$1
MODULE_UPPERCASE=`echo "$MODULE" | tr "[a-z,-]" "[A-Z,_]"`

INCLUDE_DIR=include/nfi
SRC_DIR=src/nfi
	
if [ ! -d ${SRC_DIR}/nfi_$MODULE ]
then
	echo "ERROR: the module $MODULE does not exist!"
	exit 2
fi

delete_include_structure

delete_src_structure

delete_configure_ac_structure

delete_makefile_am_structures

delete_nfi_lib_h_structure

delete_xpn_policy_init_c_structure

echo ""
echo "Ready!"
echo ""
