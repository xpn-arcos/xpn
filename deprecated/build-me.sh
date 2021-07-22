#!/bin/bash
#set -x

rm config.cache

#FILES_WITH_COMPILER_LINES=     "config/config.status \
#				config.status \
#				configure.in \
#				src/base/Makefile \
#				src/nfi/common/Makefile \
#				src/nfi/nfi_local/Makefile \
#				src/nfi/nfi_nfs/Makefile \
#				src/xpn/policy/Makefile \
#				src/xpn/xpncore/Makefile \
#				src/xpn2/Makefile \
#				src/xpni/common/Makefile \
#				src/xpni/xpni_ft/Makefile \
#				src/xpni/xpni_log/Makefile \
#				src/xpni/xpni_null/Makefile \
#				test/xpn/Makefile \
#				test/xpn2/Makefile \
#				utils/Makefile"

#FILES_WITH_COMPILER_LINES=     "config/config.status 
#				config.status 
#				configure.in 
#				src/base/Makefile 
#				src/nfi/common/Makefile 
#				src/nfi/nfi_local/Makefile 
#				src/nfi/nfi_nfs/Makefile 
#				src/xpn/policy/Makefile 
#				src/xpn/xpncore/Makefile 
#				src/xpn2/Makefile 
#				src/xpni/common/Makefile 
#				src/xpni/xpni_ft/Makefile 
#				src/xpni/xpni_log/Makefile 
#				src/xpni/xpni_null/Makefile 
#				test/xpn/Makefile 
#				test/xpn2/Makefile 
#				utils/Makefile"


#FILES_WITH_COMPILER_LINES="config/config.status config.status configure.in src/base/Makefile src/nfi/common/Makefile src/nfi/nfi_local/Makefile src/nfi/nfi_nfs/Makefile src/xpn/policy/Makefile src/xpn/xpncore/Makefile src/xpn2/Makefile src/xpni/common/Makefile src/xpni/xpni_ft/Makefile src/xpni/xpni_log/Makefile src/xpni/xpni_null/Makefile test/xpn/Makefile test/xpn2/Makefile utils/Makefile"

FILES_WITH_COMPILER_LINES="config/config.status config.status configure.in src/base/Makefile src/nfi/common/Makefile src/nfi/nfi_local/Makefile src/nfi/nfi_nfs/Makefile utils/Makefile"
#FILES_WITH_LIBRARY_LINES=      "src/.MakeBones \
#				src/Makefile \
#				src/Makefile.in"

#FILES_WITH_LIBRARY_LINES=      "src/.MakeBones 
#				src/Makefile 
#				src/Makefile.in"

FILES_WITH_LIB_LINES="src/.MakeBones src/Makefile src/Makefile.in"

FILES_WITH_L_LINES="test/xpn/.MakeBones test/xpn2/.MakeBones"

#LIBRARYS=      "base \
#		nfi \
#		xpn"

#LIBRARYS=      "base 
#		nfi 
#		xpn"

LIBRARYS="base nfi xpn"

function g++_to_gcc()
{
	for file in $FILES_WITH_COMPILER_LINES
	do
		cat $file | sed 's/g++/gcc/g' > tmp.$$
		mv tmp.$$ $file
	done
}

function gcc_to_g++()
{
	for file in $FILES_WITH_COMPILER_LINES
	do
		cat $file | sed 's/gcc/g++/g' > tmp.$$
		mv tmp.$$ $file
	done
}

function lib_g++_to_gcc()
{
	#for file in $LIBRARYS
	#do
	#	FILE_GCC="$file.a"
	#	FILE_GPP="$file_g++.a"
	#        #cat $1 | sed 's/$file_g++.a/$file.a/g' > tmp.$$
	#        cat $1 | sed 's/$FILE_GPP/$FILE_GCC/g' > tmp.$$
	#        mv tmp.$$ $1
	#done
        for file in $FILES_WITH_LIB_LINES
        do
		cat $file | sed 's/libbase_g++.a/libbase_gcc.a/g' > tmp.$$
		mv tmp.$$ $file
		cat $file | sed 's/libnfi_g++.a/libnfi_gcc.a/g' > tmp.$$
		mv tmp.$$ $file
		cat $file | sed 's/libxpn_g++.a/libxpn_gcc.a/g' > tmp.$$
		mv tmp.$$ $file
	done
}


function lib_gcc_to_g++()
{
	#for file in $LIBRARYS
	#do
	#	FILE_GCC="$file.a"
	#	FILE_GPP="$file_g++.a"
	#        #cat $1 | sed 's/$file.a/$file_g++.a/g' > tmp.$$
	#        cat $1 | sed 's/$FILE_GCC/$FILE_GPP/g' > tmp.$$
	#        mv tmp.$$ $1
	#done
        for file in $FILES_WITH_LIB_LINES
        do
		cat $file | sed 's/libbase_gcc.a/libbase_g++.a/g' > tmp.$$
		mv tmp.$$ $file
		cat $file | sed 's/libnfi_gcc.a/libnfi_g++.a/g' > tmp.$$
		mv tmp.$$ $file
		cat $file | sed 's/libxpn_gcc.a/libxpn_g++.a/g' > tmp.$$
		mv tmp.$$ $file
	done
}


function l_g++_to_gcc()
{
	for file in $FILES_WITH_L_LINES
	do
		cat $file | sed 's/-lxpn_g++/-lxpn_gcc/g' > tmp.$$
		mv tmp.$$ $file
	done
}

function l_gcc_to_g++()
{
	for file in $FILES_WITH_L_LINES
	do
		cat $file | sed 's/-lxpn_gcc/-lxpn_g++/g' > tmp.$$
		mv tmp.$$ $file
	done
}




#
# build the gcc version
#
function build-gcc()
{
	echo "Building with gcc..."
	# 1.- g++ to gcc
	g++_to_gcc
	/usr/bin/autoconf

	# 2.- preconfigure
	#cat src/.Makebones | sed 's/libxpn_cpp.a/libxpn.a/g' > tmp.$$
	#mv tmp.$$ src/.Makebones
	lib_g++_to_gcc
	l_g++_to_gcc

	echo -n " Makefile.in for XPN sources..."
	./bin/MakeBones.sh -m src
	echo -n " Makefile.in for Test sources..."
	./bin/MakeBones.sh -m test
	echo -n " Makefile.in for Utils sources..."
	./bin/MakeBones.sh -m utils

	# 3.- configure
	./configure

	# 4. make clean
	/usr/bin/make -k clean
	
	# 5.- postconfigure
	echo -n " Documentation for XPN sources..."
	doxygen doc/doxygen-XPN.cfg

	# 5.1.- create compatibility librarys
	cd lib
	ln -sf libbase_gcc.a libbase.a
	ln -sf libnfi_gcc.a libnfi.a
	ln -sf libxpn_gcc.a libxpn.a
	cd ..

	# 6.- make
	/usr/bin/make
}




#
# build the g++ version
#
function build-g++()
{
	echo "Building with g++..."
	# 1.- g++ to gcc
	gcc_to_g++
	/usr/bin/autoconf

	# 2.- preconfigure
	lib_gcc_to_g++
	l_gcc_to_g++

	echo -n " Makefile.in for XPN sources..."
	./bin/MakeBones.sh -m src
	echo -n " Makefile.in for Test sources..."
	./bin/MakeBones.sh -m test
	echo -n " Makefile.in for Utils sources..."
	./bin/MakeBones.sh -m utils

	# 3.- configure
	./configure

	# 4. make clean
	/usr/bin/make -k clean
	
	# 5.- postconfigure
	echo -n " Documentation for XPN sources..."
	doxygen doc/doxygen-XPN.cfg

	# 5.1.- create compatibility librarys
	cd lib
	ln -sf libbase_g++.a libbase.a
	ln -sf libnfi_g++.a libnfi.a
	ln -sf libxpn_g++.a libxpn.a
	cd ..

	# 6.- make
	/usr/bin/make
}



#
# Main
#



#
# check build tools
#
if [ ! -f /usr/bin/autoconf ]; then
	echo "I can't find autoconf... try to check the full path..."
	exit
fi


if [[ $# -eq 1  && $1 = "gcc" ]]
then
	build-gcc
elif [[ $# -eq 1  && $1 = "g++" ]]
then
	build-g++
else
	echo "Building all..."
	build-gcc
	build-g++
fi

