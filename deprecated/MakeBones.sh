#!/bin/bash
#set -x


#
# Strings 
#

function MB_MultiLine()
{
  while [ -n "$1" ]; do

     L1=`echo -n "$1" | awk '{ print $1 }'`

     shift 1

     if [ -n "$1" ]; then
         echo $L1" \\"
         echo -n "		"
     else
         echo $L1
     fi

     #echo -n $'\r'

  done
}

function MB_TabToLevel()
{
   LEFT_MARGIN="   "
  INNER_MARGIN="     "

  echo -n "$LEFT_MARGIN"

  SUBDIR=`echo $1|sed 's/[\/]/ /g'`
  LEVELS=`echo $1|sed 's/[^\/]//g'| wc -c`

  n5=1
  while [ $n5 -le $LEVELS ]; do

     TABS=`echo $SUBDIR|cut -f"$n5" -d" "|sed 's/./ /g'`
     echo -n "$TABS"
     echo -n "$INNER_MARGIN"
     n5=`expr $n5 + 1`

  done
}


#
# MakeBone_Definitions
#

function MB_Definitions_Header() 
{
MAKEIN=$1
FILENAME=$2

cat >> $MAKEIN << EOF_MARK


#--------------------------
# Variables
#--------------------------

MAKE         = @MAKE@
EOF_MARK
}

function MB_Definitions_Dir() 
{
MAKEIN=$1
FILENAME=$2

if [ -n "$DIR" ]; then
cat >> $MAKEIN  << EOF_MARK
DIR          = $DIR
EOF_MARK
fi
}

function MB_Definitions_Object() 
{
MAKEIN=$1
FILENAME=$2

N_NOTHING=`echo ""|wc -c`
N_DOTDOT1=`echo $FILENAME|sed 's/\.\///g'|sed 's/[^\/]//g'|sed 's/[ ]*//g'|wc -c`
N_DOTDOT2=`expr $N_DOTDOT1 - $N_NOTHING`

RPATH=""
N2=0
while [ $N2 -lt $N_DOTDOT2 ]; do
    RPATH=$RPATH"../"
    N2=`expr $N2 + 1`
done

XPN_DIR="$RPATH"
M_LIBS=" -L"$XPN_DIR"lib @M_LIBS@"
M_HEADERS=" -I"$XPN_DIR"include @M_HEADERS@"

if [ -n "$OBJECT" ]; then
cat >> $MAKEIN  << EOF_MARK
CC           = @CC@
SOURCE_PATH  = ./
EXPAND_DIR   = $XPN_DIR
OBJECTS      = `MB_MultiLine $OBJECT`
MYFLAGS      = @CFLAGS@ @CDEFS@ -DBUILD_LITE -DHAVE_STRUCT_IOVEC
MYHEADERS    = $M_HEADERS
MYLIBS       = $M_LIBS
EOF_MARK
fi
}

function MB_Definitions_Lib() 
{
MAKEIN=$1
FILENAME=$2

# header for lib
LIBNAMES1=""
LIBOBJS1=""

n3=0
while [ -n "${LIB_NAME[$n3]}" ]; do

    LIBNAME1=${LIB_NAME[$n3]}
    LIBNAMES1=$LIBNAMES1" "$LIBNAME1
    LIBOBJ1=${LIB_OBJ[$n3]}
    LIBOBJS1=$LIBOBJS1" "$LIBOBJ1

    n3=`expr $n3 + 1`

done

if [ -n "${LIB_NAME[0]}" ]; then
cat >> $MAKEIN  << EOF_MARK
LIBRARIES_NAMES = $LIBNAMES1
LIBRARIES_OBJS  = `MB_MultiLine $LIBOBJS1`
EOF_MARK
fi
}

function MB_Definitions_Exe() 
{
MAKEIN=$1
FILENAME=$2

# header for exe
EXENAMES1=""
EXEOBJS1=""

n3=0
while [ -n "${EXE_NAME[$n3]}" ]; do

    EXENAME1=${EXE_NAME[$n3]}
    EXENAMES1=$EXENAMES1" "$EXENAME1
    EXEOBJ1=${EXE_OBJ[$n3]}
    EXEOBJS1=$EXEOBJS1" "$EXEOBJ1

    n3=`expr $n3 + 1`

done

if [ -n "${EXE_NAME[0]}" ]; then
cat >> $MAKEIN  << EOF_MARK
EXE_NAMES    = $EXENAMES1
EXE_OBJS     = `MB_MultiLine $EXEOBJS1`
EOF_MARK
fi
}

function MB_Definitions() 
{
MAKEIN=$1
FILENAME=$2

MB_Definitions_Header $MAKEIN $FILENAME
MB_Definitions_Dir    $MAKEIN $FILENAME
MB_Definitions_Object $MAKEIN $FILENAME
MB_Definitions_Lib    $MAKEIN $FILENAME
MB_Definitions_Exe    $MAKEIN $FILENAME
}


#
# MakeBone_CompilerRules
#

function MB_CompilerRules_All_Header ()
{
BASEMAKEIN=$1

ALL_DEPENDENCES=""
if [ -n "$DIR" ]; then
    ALL_DEPENDENCES=$ALL_DEPENDENCES" all_dir "
fi
if [ -n "$OBJECT" ]; then
    ALL_DEPENDENCES=$ALL_DEPENDENCES" all_object "
fi
if [ -n "${LIB_NAME[0]}" ]; then
    ALL_DEPENDENCES=$ALL_DEPENDENCES" all_lib "
fi
if [ -n "${EXE_NAME[0]}" ]; then
    ALL_DEPENDENCES=$ALL_DEPENDENCES" all_exe "
fi

cat >> $BASEMAKEIN"/Makefile.in"  << EOF_MARK

#--------------------------
# Compilation rules
#--------------------------

EOF_MARK

cat >> $BASEMAKEIN"/Makefile.in"  << EOF_MARK

all: $ALL_DEPENDENCES

EOF_MARK
}

function MB_CompilerRules_All_Dir ()
{
BASEMAKEIN=$1

if [ -n "$DIR" ]; then
cat >> $BASEMAKEIN"/Makefile.in"  << EOF_MARK
all_dir:
	@set -e;  for i in \$(DIR); do echo -n "`MB_TabToLevel $BASEMAKEIN`" ; echo \$\$i" ["; \$(MAKE) -C \$\$i -f Makefile; echo -n "`MB_TabToLevel $BASEMAKEIN`" ; echo "]"; done

EOF_MARK
fi
}

function MB_CompilerRules_All_Object ()
{
BASEMAKEIN=$1

if [ -n "$OBJECT" ]; then
cat >> $BASEMAKEIN"/Makefile.in"  << EOF_MARK
all_object:  \$(OBJECTS)

%.o: %.c
	@echo -n "`MB_TabToLevel $BASEMAKEIN`"
	@echo -n "\$< [.." 
	@\$(CC) \$(MYFLAGS) \$(MYHEADERS) -c \$< -o \$@ 
	@echo "..]" 

EOF_MARK
fi
}

function MB_CompilerRules_All_Lib ()
{
BASEMAKEIN=$1

n3=0
while [ -n "${LIB_NAME[$n3]}" ]; do

  LIBOBJS1=${LIB_OBJ[$n3]}
  LIBNAME1=${LIB_NAME[$n3]}

  LIBLABEL0=`basename $LIBNAME1`
  IS_STATIC=`echo $LIBLABEL0|grep "\.a"|wc -l`
  IS_DYNAMIC=`echo $LIBLABEL0|grep "\.so"|wc -l`

  if [ $IS_STATIC == 1 ]; then

       LIBLABEL1=../lib/`basename $LIBNAME1`
       #LIBLABEL1=`basename $LIBNAME1|sed 's/\.a/_a/g'`

cat >> $BASEMAKEIN"/Makefile.in"  << EOF_MARK
$LIBLABEL1: `MB_MultiLine $LIBOBJS1`
	@echo -n "`MB_TabToLevel $BASEMAKEIN`"
	@echo -n "\$@ [.." 
	@ar rcv $LIBNAME1 `MB_MultiLine $LIBOBJS1` > /dev/null
	@echo "..]" 

EOF_MARK

  elif [ $IS_DYNAMIC == 1 ]; then

       LIBLABEL1=../lib/`basename $LIBNAME1`
       #LIBLABEL1=../lib/`basename $LIBNAME1|sed 's/\.so/_so/g'`

cat >> $BASEMAKEIN"/Makefile.in"  << EOF_MARK
$LIBLABEL1: `MB_MultiLine $LIBOBJS1`
	@echo -n "`MB_TabToLevel $BASEMAKEIN`"
	@echo -n "\$@ [.." 
	@\$(CC) -shared -fPIC -ldl -o $LIBNAME1 `MB_MultiLine $LIBOBJS1` 
	@echo "..]" 

EOF_MARK

  else

       echo ""
       echo -n "ERROR at "$FILE2":"$n2" -> "
       echo -n "LIB entry ("$LIBLABEL0") is not static (.a) nor dynamic (.so) "
       echo ""
       exit

  fi

  LIB_LABELS=$LIB_LABELS" "$LIBLABEL1

  n3=`expr $n3 + 1`

done

if [ -n "${LIB_NAME[0]}" ]; then
cat >> $BASEMAKEIN"/Makefile.in"  << EOF_MARK
all_lib: $LIB_LABELS

EOF_MARK
fi
}

function MB_CompilerRules_All_Exe ()
{
BASEMAKEIN=$1
EXE_LABELS=""

n7=0
while [ -n "${EXE_NAME[$n7]}" ]; do

  EXELIBS1=${EXE_LIB[$n7]}
  EXEOBJS1=${EXE_OBJ[$n7]}
  EXENAME1=${EXE_NAME[$n7]}

  EXELABEL0=`basename $EXENAME1`
  EXELABEL1=`basename $EXENAME1`
  #EXELABEL1=`basename $EXENAME1|sed 's/\.exe/_exe/g'`

cat >> $BASEMAKEIN"/Makefile.in"  << EOF_MARK
$EXELABEL1: `MB_MultiLine $EXEOBJS1 \$\(EXPAND_DIR\)/lib/libxpn.a`
	@echo -n "`MB_TabToLevel $BASEMAKEIN`"
	@echo -n "\$@ [.." 
	@\$(CC) -o $EXENAME1  \\
             `MB_MultiLine $EXEOBJS1` \\
             `MB_MultiLine $EXELIBS1` \$(MYLIBS) > /dev/null
	@echo "..]" 

EOF_MARK

  EXE_LABELS=$EXE_LABELS" "$EXELABEL1

  n7=`expr $n7 + 1`

done

if [ -n "${EXE_NAME[0]}" ]; then
cat >> $BASEMAKEIN"/Makefile.in"  << EOF_MARK
all_exe: $EXE_LABELS

EOF_MARK
fi
}

function MB_CompilerRules_All ()
{
BASEMAKEIN=$1

MB_CompilerRules_All_Header $BASEMAKEIN
MB_CompilerRules_All_Dir    $BASEMAKEIN
MB_CompilerRules_All_Object $BASEMAKEIN
MB_CompilerRules_All_Lib    $BASEMAKEIN
MB_CompilerRules_All_Exe    $BASEMAKEIN
}


function MB_CompilerRules_Clean_Header ()
{
BASEMAKEIN=$1
cat >> $BASEMAKEIN"/Makefile.in"  << EOF_MARK

clean:
EOF_MARK
}

function MB_CompilerRules_Clean_Dir ()
{
BASEMAKEIN=$1

if [ -n "$DIR" ]; then
cat >> $BASEMAKEIN"/Makefile.in"  << EOF_MARK
	@set -e;  for i in \$(DIR); do echo -n "`MB_TabToLevel $BASEMAKEIN`" ; echo \$\$i" ["; \$(MAKE) -C \$\$i -f Makefile clean; echo -n "`MB_TabToLevel $BASEMAKEIN`" ; echo "]"; done

EOF_MARK
fi
}

function MB_CompilerRules_Clean_Object ()
{
BASEMAKEIN=$1

if [ -n "$OBJECT" ]; then
cat >> $BASEMAKEIN"/Makefile.in"  << EOF_MARK
	rm -f \$(OBJECTS)
EOF_MARK
fi
}

function MB_CompilerRules_Clean_Lib ()
{
BASEMAKEIN=$1

if [ -n "${LIB_NAME[0]}" ]; then
cat >> $BASEMAKEIN"/Makefile.in"  << EOF_MARK
	rm -f \$(LIBRARIES_NAMES)
	rm -f \$(LIBRARIES_OBJS)
EOF_MARK
fi
}

function MB_CompilerRules_Clean_Exe ()
{
BASEMAKEIN=$1

if [ -n "${EXE_NAME[0]}" ]; then
cat >> $BASEMAKEIN"/Makefile.in"  << EOF_MARK
	rm -f \$(EXE_NAMES)
	rm -f \$(EXE_OBJS)

EOF_MARK
fi
}

function MB_CompilerRules_Clean ()
{
BASEMAKEIN=$1

MB_CompilerRules_Clean_Header $BASEMAKEIN
MB_CompilerRules_Clean_Dir    $BASEMAKEIN
MB_CompilerRules_Clean_Object $BASEMAKEIN
MB_CompilerRules_Clean_Lib    $BASEMAKEIN
MB_CompilerRules_Clean_Exe    $BASEMAKEIN
}


#
# MakeBone_MakeBone2Makefile.in
#

function MB_CompileMakeBone ()
{
  FILE2=$1

  BASEMAKEIN=`dirname $FILE2`
  MAKEIN=$BASEMAKEIN"/Makefile.in"
  if [ $MAKEIN -nt $FILE2 ]; then
      echo ".]" # PROGRESS
      return 0
  else
      rm -fr $MAKEIN
      touch $MAKEIN
  fi

  OBJECT=""
  DIR=""
  LIBNAME=""
  LIBOBJ=""
  EXENAME=""
  EXEOBJ=""
  EXELIB=""

  n4=0
  n6=0

  unset LIB_NAME
  unset LIB_OBJ
  unset EXE_NAME
  unset EXE_OBJ
  unset EXE_LIB

  nlineas2=`wc -l $FILE2 | awk '{print $1}'`

  n2=1
  while [ $n2 -le $nlineas2 ]; do

    echo -n "." # PROGRESS

    # 1.- get line
    LINE2=`/usr/bin/head -n $n2 $FILE2 | /usr/bin/tail -n 1`

    # 2.- check comment
    IS_COMMENT=`echo $LINE2 | grep "^#.*" | wc -l`
    if [ "$IS_COMMENT" -eq 1 ]; then
         n2=`expr $n2 + 1`
         continue;
    fi

    # 3.- check empty
    IS_EMPTY=`echo $LINE2 | grep "^$" | wc -l`
    if [ "$IS_EMPTY" -eq 1 ]; then
         n2=`expr $n2 + 1`
         continue;
    fi

    # 4.- switch (first word) of (OBJECTS->X,DIRS->X,LIB->X)
    WORD1=`echo $LINE2 | awk '{ print $1 }'`

    if [ $WORD1 == "OBJECTS" ]; then

       STATE="OBJECT"

    elif [ $WORD1 == "DIRS" ]; then

       STATE="DIR"

    elif [ $WORD1 == "LIB" ]; then

       STATE="LIB"

       if [ -n "$LIBNAME" ]; then
            LIB_NAME[$n4]=$LIBNAME
            LIB_OBJ[$n4]=$LIBOBJ
            n4=`expr $n4 + 1`
       fi

       WORD2=`echo $LINE2 | awk '{ print $2 }'`
       LIBNAME=$WORD2
       LIBOBJ=""

       if [ -z $LIBNAME ]; then
           echo ""
           echo -n "ERROR at "$FILE2":"$n2" -> "
           echo -n "LIB entry without library name in "$FILE2
           echo ""
           exit
       fi

    elif [ $WORD1 == "EXE" ]; then

       STATE="EXE"

       if [ -n "$EXENAME" ]; then
            EXE_NAME[$n6]=$EXENAME
            EXE_OBJ[$n6]=$EXEOBJ
            EXE_LIB[$n6]=$EXELIB
            n6=`expr $n6 + 1`
       fi

       WORD2=`echo $LINE2 | awk '{ print $2 }'`
       EXENAME=$WORD2
       EXEOBJ=""
       EXELIB=""

       if [ -z $EXENAME ]; then
           echo ""
           echo -n "ERROR at "$FILE2":"$n2" -> "
           echo -n "EXE entry without executable name in "$FILE2
           echo ""
           exit
       fi

    else

       if [ $STATE == "OBJECT" ]; then
            OBJ=`echo $LINE2|sed 's/\.c/\.o/g'`
            OBJECT=$OBJECT" "$OBJ
       fi
       if [ $STATE == "DIR" ]; then
            D=$LINE2
            DIR=$DIR" "$D
       fi
       if [ $STATE == "LIB" ]; then
            L=$LINE2
            LIBOBJ=$LIBOBJ" "$L

	    IS_SOURCE=`echo $L|grep '\.c'|wc -l`
	    if [ $IS_SOURCE -eq 1 ]; then
		   echo ""
		   echo -n "ERROR at "$FILE2":"$n2" -> "
		   echo -n "Found a source file name in LIB rule."
		   echo ""
		   echo " -> "$L" <- "
		   echo ""
		   echo " VERY IMPORTANT!"
		   echo " ---------------"
		   echo " Change file extension to '.o' or your source will be damage"
		   echo " in the compilation process"
		   echo ""
		   exit;
	    fi

       fi
       if [ $STATE == "EXE" ]; then
            L=$LINE2

            IS_EXELIB=`echo $LINE2 | grep "^-l.*" | wc -l`
            if [ "$IS_EXELIB" -eq 1 ]; then
                 EXELIB=$EXELIB" "$L
            else
                 EXEOBJ=$EXEOBJ" "$L
            fi

       fi

    fi

    # 5.- next line
    n2=`expr $n2 + 1`

  done

  # flush unclosed states
  if [ -n "$LIBNAME" ]; then
            LIB_NAME[$n4]="$LIBNAME"
            LIB_OBJ[$n4]="$LIBOBJ"
            n4=`expr $n4 + 1`
  fi
  if [ -n "$EXENAME" ]; then
            EXE_NAME[$n6]="$EXENAME"
            EXE_OBJ[$n6]="$EXEOBJ"
            EXE_LIB[$n6]="$EXELIB"
            n6=`expr $n6 + 1`
  fi

  echo ".]" # PROGRESS

  echo -n "		-> Definitions		[..." # PROGRESS
  MB_Definitions         $MAKEIN $FILE2
  echo ".]" # PROGRESS

  echo -n "		-> Compiler rules	[..." # PROGRESS
  MB_CompilerRules_All   $BASEMAKEIN $FILE2
  MB_CompilerRules_Clean $BASEMAKEIN $FILE2
  echo ".]" # PROGRESS
}


function MB_CompileMakeBones ()
{
  # find .MakeBones...
  BASENAME=$1
  find $BASENAME -name ".MakeBones" -print > $FILE

  # Starting
  nlineas=`wc -l $FILE | awk '{print $1}'`
  echo "" # PROGRESS
  echo "$nlineas files found.                        " # PROGRESS

  # Compile each one...
  n=1
  while [ $n -le $nlineas ]; do

    LINE=`/usr/bin/head -n $n $FILE | /usr/bin/tail -n 1`
    MKNM=`dirname $LINE`"/Makefile.in"
    echo -n "	""($n/$nlineas)  "$MKNM"	[" # PROGRESS

    #/usr/bin/dos2unix $LINE
    MB_CompileMakeBone $LINE
    n=`expr $n + 1`

  done

  # Ending
  echo -n $'\r' # PROGRESS
  echo "Done.                                       " # PROGRESS
  echo "" # PROGRESS
}


#
#  Main
#

function MB_RmTmpFiles ()
{
  rm -fr $FILE
  exit
}

function MB_Main ()
{
  MB_VERSION="0.85"

  if [ $# -eq 1 -a "$1" == "-h" ]; then

        echo ""
        echo ""
	echo "	 MakeBones "
        echo "	-----------"
        echo ""
	echo "	$0 builds Makefile.in file based on .MakeBones files"
        echo "	.MakeBones file format has three cases:"
        echo ""
        echo "	    - Call Makefile from subdirectories"
        echo ""
        echo "	      DIRS"
        echo "	      <directory name 1>"
        echo "	      <directory name 2>"
        echo "	      <...>"
        echo ""
        echo "	    - Compile source files"
        echo ""
        echo "	      OBJECTS"
        echo "	      <source .c name 1>"
        echo "	      <source .c name 2>"
        echo "	      <...>"
        echo ""
        echo "	    - Build a library"
        echo ""
        echo "	      LIB <library name .a or .so>"
        echo "	      <source .o name 1>"
        echo "	      <source .o name 2>"
        echo "	      <...>"
        echo ""
        echo "	    - Build a program"
        echo ""
        echo "	      EXE <program.exe>"
        echo "	      <object .o  name 1>"
        echo "	      <lib    -la name 2>"
        echo "	      <...>"
        echo ""
	echo "	 MakeBones version "$MB_VERSION
        echo ""
        echo ""
	exit

  elif [ $# -eq 2 -a "$1" == "-m" ]; then

	BASENAME=$2
	FILE="/tmp/MakeBones.$$"
	trap MB_RmTmpFiles SIGINT
	MB_CompileMakeBones $BASENAME
	MB_RmTmpFiles

  else

	echo ""
	echo "	Usage: $0 -m <root directory where scanning starts>"
	echo "	Usage: $0 -h"
	echo ""

  fi
}

MB_Main $*

