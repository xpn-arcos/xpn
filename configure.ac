# Process this file with autoconf to produce a configure script.
#
#  Expand's configure.ac
#  ---------------------
#
#   Process this file with autoconf to produce
#   a configure script.
#

#
# begin
#
AC_INIT([Expand],[1.9.0],[xpn@arcos.inf.uc3m.es],[expand])
AC_CONFIG_AUX_DIR(./config)
AC_CONFIG_HEADER(include/config.h)
AC_PROG_INSTALL
AC_PROG_RANLIB
# AC_PROG_LIBTOOL

#
# Canonicalize the configuration name.
#
AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE(subdir-objects)
AM_CONDITIONAL([am__fastdepCC], [test x$am__fastdepCC = xam__fastdepCC])
AM_CONDITIONAL([AMDEP], [test x$amdep = xamdep])
AM_PROG_CC_C_O

#
# Checks for programs.
#
AC_PROG_MAKE_SET


#
# Checks for libraries.
#
# Replace `main' with a function in -lpthread:
AC_CHECK_LIB(pthread, main)


#
# Checks for header files.
#
AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS(fcntl.h string.h strings.h unistd.h sys/ioctl.h sys/time.h netinet/tcp.h netinet/in.h pthread.h windows.h sys/param.h dirent.h rpc/rpc.h rpc/clnt.h rpc/types.h)

#
# Checks for typedefs, structures, and compiler characteristics.
#
AC_C_CONST
AC_HEADER_TIME
AC_CHECK_TYPE(ptrdiff_t,long)
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SIGNAL
AC_CHECK_SIZEOF(unsigned short,2)
AC_CHECK_SIZEOF(unsigned,4)
AC_CHECK_SIZEOF(unsigned long,8)
AC_CHECK_SIZEOF(char *,4)
AC_CHECK_SIZEOF(ptrdiff_t,4)
AC_CHECK_SIZEOF(size_t,4)


#
# Checks for library functions.
#
AC_FUNC_VPRINTF
AC_CHECK_FUNCS(gethostname gettimeofday select socket strdup strerror)
AC_CHECK_FUNCS(memcmp memcpy memmove memset)


if test "$target_os" = "Linux" -o "$target_os" = "linux" -o "$target_os" = "linux-gnu"
then
  OS="__LINUX__"
  #CC="g++"
  #MAKE="make -s"
  MAKE="make"
  #CFLAGS="-O2 -Wall -Wextra"
  #AC_MSG_RESULT("$CFLAGS");
   #CFLAGS="-g3 -ggdb3 -Wall -Wextra -static"
   CFLAGS="-fPIC -O0 -g -g3 -ggdb -ggdb3 -Wall -Wextra" #ADDED
  #AC_MSG_RESULT("$CFLAGS");
  #CDEFS="-D_REENTRANT -DPOSIX_THREADS -DLINUX -DDEBUG -DXPNI_LOWFSI_POSIX -DXPNI_LOWFSI_XPN"
   CDEFS="-D_REENTRANT -DPOSIX_THREADS -DHAVE_CONFIG_H -DLINUX -DXPNI_LOWFSI_XPN -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64 -DENABLE_MXML"
  LIBLINK="-lpthread"
  LIBS="$LIBS -L\$(HOME)/mxml/usr/lib -lmxml -ldl"
  AC_SUBST(OS)
  AC_SUBST(CC)
  AC_SUBST(MAKE)
  AC_SUBST(CFLAGS)
  AC_SUBST(CDEFS)
  AC_SUBST(LIBLINK)
  AC_MSG_RESULT("$target_os");
elif test "$target_os" = "CYGWIN32" -o "$target_os" = "cygwin32" 
then
  OS="__WIN32__"
  #CC="g++"
  MAKE="make -s"
  #CFLAGS="-g -Wall -Wextra"
   CFLAGS="-O2 -Wall -Wextra"
  #CDEFS="-DHAVE_CONFIG_H -D_REENTRANT -DPOSIX_THREADS -DLINUX -DDEBUG -DXPNI_LOWFSI_POSIX -DXPNI_LOWFSI_XPN"
   CDEFS="-DHAVE_CONFIG_H -D_REENTRANT -DPOSIX_THREADS -ULINUX -DWIN32 -DXPNI_LOWFSI_XPN"
  LIBLINK="-lpthread"
  AC_SUBST(OS)
  AC_SUBST(CC)
  AC_SUBST(MAKE)
  AC_SUBST(CFLAGS)
  AC_SUBST(CDEFS)
  AC_SUBST(LIBLINK)
  AC_MSG_RESULT("Cygwin32");
elif test "$target_os" = "AIX" -o "$target_os" = "aix"
then
  OS="__AIX__"
  #CC="g++"
  MAKE="make"
  #CFLAGS="-g -Wall -Wextra"
   CFLAGS="-O2 -Wall -Wextra"
  #CDEFS="-D_REENTRANT"
   CDEFS="-D_REENTRANT"
  LIBLINK="-lpthread "
  AC_SUBST(OS)
  AC_SUBST(CC)
  AC_SUBST(MAKE)
  AC_SUBST(CFLAGS)
  AC_SUBST(CDEFS)
  AC_SUBST(LIBLINK)
  AC_MSG_RESULT("AIX");
elif test "$target_os" = "IRIX6.5" -o "$target_os" = "irix6.5"
then
  OS="__IRIX__"
  #CC="g++"
  MAKE="gmake"
  #CFLAGS="-g -Wall -Wextra"
   CFLAGS="-O2 -Wall -Wextra"
  #CDEFS="-D_REENTRANT -DPOSIX_THREADS"
   CDEFS="-D_REENTRANT -DPOSIX_THREADS"
  LIBLINK="-lpthread "
  AC_SUBST(OS)
  AC_SUBST(CC)
  AC_SUBST(MAKE)
  AC_SUBST(CFLAGS)
  AC_SUBST(CDEFS)
  AC_SUBST(LIBLINK)
  AC_MSG_RESULT("IRIX6.5");
else
  OS="__LINUX__"
  #CC="g++"
  MAKE="make"
  CFLAGS="-O2 -Wall -Wextra"
  #CFLAGS="-O2 -Wall -Wextra"
   CFLAGS="-g  -Wall -Wextra"
  #CDEFS="-D_REENTRANT -DPOSIX_THREADS -DLINUX -DDEBUG -DDEBUG_NFS -DXPNI_LOWFSI_POSIX -DXPNI_LOWFSI_XPN"
   CDEFS="-D_REENTRANT -DPOSIX_THREADS -DHAVE_CONFIG_H -DLINUX -DXPNI_LOWFSI_XPN"
  LIBLINK="-lpthread "
  AC_SUBST(OS)
  AC_SUBST(CC)
  AC_SUBST(MAKE)
  AC_SUBST(CFLAGS)
  AC_SUBST(CDEFS)
  AC_SUBST(LIBLINK)
  AC_MSG_RESULT("no defined \(using Linux configuration\)");
fi

INCLUDEFLAGS="\
-I\$(HOME)/mxml/usr/include \
-I\$(top_srcdir)/include \
-I\$(top_srcdir)/include/base \
-I\$(top_srcdir)/include/xpn \
-I\$(top_srcdir)/include/xpn/xpn_simple \
-I\$(top_srcdir)/include/nfi \
-I\$(top_srcdir)/include/nfi/nfi_local \
-I\$(top_srcdir)/include/xpni/xpni_ft \
-I\$(top_srcdir)/include/xpni/xpni_log"

### BEGIN OF AC_ARG_ENABLE BLOCK. Do not remove this line. ###

### BEGIN OF NFS BLOCK. Do not remove this line. ###
#
# Defines the necessary variables if nfs is enabled.
#
AC_ARG_ENABLE(	[nfs],
		[AS_HELP_STRING([--enable-nfs],[Enable NFS v2 module.])],
		[
			[CDEFS="$CDEFS -DENABLE_NFS"]
			[INCLUDEFLAGS="$INCLUDEFLAGS -I\$(top_srcdir)/include/nfi/nfi_nfs"]
			[NFI_NFS="nfi_nfs"]
			[NFI_NFS_OBJECTS="\$(NFI_NFS_OBJECTS)"]
		]
)
AC_SUBST(NFI_NFS)
AC_SUBST(NFI_NFS_OBJECTS)
AM_CONDITIONAL([ENABLE_NFS], [test "$NFI_NFS" = "nfi_nfs"])
### END OF NFS BLOCK. Do not remove this line. ###

### BEGIN OF NFS3 BLOCK. Do not remove this line. ###
#
# Defines the necessary variables if nfs3 is enabled.
#
AC_ARG_ENABLE(	[nfs3],
		[AS_HELP_STRING([--enable-nfs3],[Enable NFS v3 module.])],
		[
			[CDEFS="$CDEFS -DENABLE_NFS3"]
			[INCLUDEFLAGS="$INCLUDEFLAGS -I\$(top_srcdir)/include/nfi/nfi_nfs3"]
			[NFI_NFS3="nfi_nfs3"]
			[NFI_NFS3_OBJECTS="\$(NFI_NFS3_OBJECTS)"]
		]
)
AC_SUBST(NFI_NFS3)
AC_SUBST(NFI_NFS3_OBJECTS)
AM_CONDITIONAL([ENABLE_NFS3], [test "$NFI_NFS3" = "nfi_nfs3"])
### END OF NFS3 BLOCK. Do not remove this line. ###

### BEGIN OF FTP BLOCK. Do not remove this line. ###
#
# Defines the necessary variables if ftp is enabled.
#
AC_ARG_ENABLE(	[ftp],
		[AS_HELP_STRING([--enable-ftp],[Enable FTP module.])],
		[
			[CDEFS="$CDEFS -DENABLE_FTP"]
			[INCLUDEFLAGS="$INCLUDEFLAGS -I\$(top_srcdir)/include/nfi/nfi_ftp"]
			[NFI_FTP="nfi_ftp"]
			[NFI_FTP_OBJECTS="\$(NFI_FTP_OBJECTS)"]
		]
)
AC_SUBST(NFI_FTP)
AC_SUBST(NFI_FTP_OBJECTS)
AM_CONDITIONAL([ENABLE_FTP], [test "$NFI_FTP" = "nfi_ftp"])
### END OF FTP BLOCK. Do not remove this line. ###

### BEGIN OF XIO BLOCK. Do not remove this line. ###
#
# Defines the necessary variables if xio is enabled.
#
AC_ARG_ENABLE(	[xio],
                [AS_HELP_STRING([--enable-xio=/path/to/globus/ (Don't use '~')],[Enable Globus XIO module.])],
                [
                        [CDEFS="$CDEFS -DENABLE_XIO"]
			[INCLUDEFLAGS="$INCLUDEFLAGS -I\$(top_srcdir)/include/nfi/nfi_xio"]
                        #[LIBS="$LIBS -L$enable_xio/lib/ -lglobus_xio_gcc32dbg -lglobus_ftp_client_gcc32dbg"]
                        #[M_HEADERS="$M_HEADERS -I$enable_xio/include/gcc32dbg/"]
			#[LIBS="$LIBS -lglobus_xio -lglobus_ftp_client -lglobus_common -lcrypto"]
                        #[M_HEADERS="$M_HEADERS -I/usr/lib/globus/include"]
			[if test "x$enable_xio" = "xyes"; then
				XIO_INCLUDE="-I/usr/include/globus -I/usr/lib/globus/include"
				XIO_LIB=""
			 else
				XIO_INCLUDE="-I$enable_xio/include/globus -I$enable_xio/include/globus/gcc64dbg"
				XIO_LIB="-L$enable_xio/lib64"
			 fi]
			[LIBS="$LIBS $XIO_LIB -lglobus_xio -lglobus_ftp_client -lglobus_common -lssl -lcrypto"]
                        [M_HEADERS="$M_HEADERS $XIO_INCLUDE"]
			[NFI_XIO="nfi_xio"]
			[NFI_XIO_OBJECTS="\$(NFI_XIO_OBJECTS)"]
                ]
)
AC_SUBST(NFI_XIO)
AC_SUBST(NFI_XIO_OBJECTS)
AM_CONDITIONAL([ENABLE_XIO], [test "$NFI_XIO" = "nfi_xio"])
### END OF XIO BLOCK. Do not remove this line. ###

### BEGIN OF GSOAP BLOCK. Do not remove this line. ###
#
# Defines the necessary variables if gsoap is enabled.
#
AC_ARG_ENABLE(	[gsoap],
		[AS_HELP_STRING([--enable-gsoap@<:@=/path/to/gsoap/@:>@ (Don't use '~')],[Enable Gsoap module.])],
		[
			#[if test "x$enable_gsoap" = "xyes"; then
			#	GSOAP_BIN_DIR="/usr/bin"
			#	GSOAP_INCLUDE_DIR="/usr/include"
			#	GSOAP_LIB_DIR="/usr/lib"
			# else
				GSOAP_BIN_DIR="$enable_gsoap/bin"
				GSOAP_INCLUDE_DIR="$enable_gsoap/include"
				GSOAP_IMPORT_DIR="$enable_gsoap/share/gsoap/import"
				GSOAP_LIB_DIR="$enable_gsoap/lib"
			# fi]
			[CDEFS="$CDEFS -DENABLE_GSOAP"]
			[INCLUDEFLAGS="$INCLUDEFLAGS -I\$(top_srcdir)/include/nfi/nfi_gsoap"]
			[LIBS="$LIBS -L$GSOAP_LIB_DIR -L/usr/local/ssl/lib -L\$(top_srcdir)/src/nfi/nfi_gsoap/PosixIOService/ -lIOServiceClient -lpthread -lm -lgsoapssl -lssl -lcrypto -lz -ldl"]
			[M_HEADERS="$M_HEADERS -I$GSOAP_INCLUDE_DIR -I$GSOAP_IMPORT_DIR -I/usr/local/ssl/include/ -I\$(top_srcdir)/src/nfi/nfi_gsoap/PosixIOService/ -I\$(top_srcdir)/src/nfi/nfi_gsoap/PosixIOService/project"]
			[NFI_GSOAP="nfi_gsoap"]
			[NFI_GSOAP_OBJECTS="\$(NFI_GSOAP_OBJECTS)"]
			[POSIXIOSERVICE="src/nfi/nfi_gsoap/PosixIOService"]
		]
)
AC_SUBST(NFI_GSOAP)
AC_SUBST(NFI_GSOAP_OBJECTS)
AC_SUBST(GSOAP_BIN_DIR)
AC_SUBST(GSOAP_INCLUDE_DIR)
AC_SUBST(GSOAP_IMPORT_DIR)
AC_SUBST(GSOAP_LIB_DIR)
AC_SUBST(POSIXIOSERVICE)
AM_CONDITIONAL([ENABLE_GSOAP], [test "$NFI_GSOAP" = "nfi_gsoap"])
### END OF GSOAP BLOCK. Do not remove this line. ###

### BEGIN OF GRIDFTP BLOCK. Do not remove this line. ###
#
# Defines the necessary variables if gridftp is enabled.
#
AC_ARG_ENABLE(	[gridftp],
                [AS_HELP_STRING([--enable-gridftp=/path/to/globus/ (Don't use '~')],[Enable Globus GridFTP module.])],
                [
                        [CDEFS="$CDEFS -DENABLE_GRIDFTP"]
			[INCLUDEFLAGS="$INCLUDEFLAGS -I\$(top_srcdir)/include/nfi/nfi_gridftp"]
                        #[LIBS="$LIBS -L$enable_gridftp/lib/ -lglobus_ftp_client_gcc32dbg"]
                        #[M_HEADERS="$M_HEADERS -I$enable_gridftp/include/gcc32dbg/"]
			#[LIBS="$LIBS -lglobus_ftp_client -lglobus_common -lcrypto"]
       	                #[M_HEADERS="$M_HEADERS -I/usr/include/globus -I/usr/lib/globus/include"]
			[if test "x$enable_gridftp" = "xyes"; then
				GRIDFTP_INCLUDE="-I/usr/include/globus -I/usr/lib/globus/include"
				GRIDFTP_LIB=""
			 else
				GRIDFTP_INCLUDE="-I$enable_gridftp/include -I$enable_gridftp/include/globus/gcc64dbg"
				GRIDFTP_LIB="-L$enable_gridftp/lib64"
			 fi]
			[LIBS="$LIBS $GRIDFTP_LIB -lglobus_ftp_client -lglobus_common -lssl -lcrypto"]
       	                [M_HEADERS="$M_HEADERS $GRIDFTP_INCLUDE"]
			[NFI_GRIDFTP="nfi_gridftp"]
			[NFI_GRIDFTP_OBJECTS="\$(NFI_GRIDFTP_OBJECTS)"]
                ]
)
AC_SUBST(NFI_GRIDFTP)
AC_SUBST(NFI_GRIDFTP_OBJECTS)
AM_CONDITIONAL([ENABLE_GRIDFTP], [test "$NFI_GRIDFTP" = "nfi_gridftp"])
### END OF GRIDFTP BLOCK. Do not remove this line. ###

### BEGIN OF TCPSERVER BLOCK. Do not remove this line. ###
#
# Defines the necessary variables if gridftp is enabled.
#
AC_ARG_ENABLE(	[myserver],
                [AS_HELP_STRING([--enable-myserver],[Enable MyServer module.])],
                [
                        [CDEFS="$CDEFS -DENABLE_TCPSERVER"]
			[INCLUDEFLAGS="$INCLUDEFLAGS -I\$(top_srcdir)/include/nfi/nfi_myServer"]
			[NFI_TCPSERVER="nfi_myserver"]
			[NFI_TCPSERVER_OBJECTS="\$(NFI_TCPSERVER_OBJECTS)"]
                ]
)
AC_SUBST(NFI_TCPSERVER)
AC_SUBST(NFI_TCPSERVER_OBJECTS)
AM_CONDITIONAL([ENABLE_TCPSERVER], [test "$NFI_TCPSERVER" = "nfi_myserver"])
### END OF TCPSERVER BLOCK. Do not remove this line. ###

### BEGIN OF MPISERVER BLOCK. Do not remove this line. ###
#
# Defines the necessary variables if gridftp is enabled.
#
AC_ARG_ENABLE(	[myserver],
                [AS_HELP_STRING([--enable-myserver],[Enable MyServer module.])],
                [
                        [CDEFS="$CDEFS -DENABLE_MPISERVER"]
			[INCLUDEFLAGS="$INCLUDEFLAGS -I\$(top_srcdir)/include/nfi/nfi_myServer"]
			[NFI_MPISERVER="nfi_myserver"]
			[NFI_MPISERVER_OBJECTS="\$(NFI_MPISERVER_OBJECTS)"]
                ]
)
AC_SUBST(NFI_MPISERVER)
AC_SUBST(NFI_MPISERVER_OBJECTS)
AM_CONDITIONAL([ENABLE_MPISERVER], [test "$NFI_MPISERVER" = "nfi_myserver"])
### END OF MPISERVER BLOCK. Do not remove this line. ###


### BEGIN OF IOPROXY_RPC BLOCK. Do not remove this line. ###
#
# Defines the necessary variables if ioproxy-rpc is enabled.
#
AC_ARG_ENABLE(	[ioproxy-rpc],
		[AS_HELP_STRING([--enable-ioproxy-rpc@<:@=/path/to/ioproxy-rpc/@:>@ (Don't use '~')],[Enable ioproxy-rpc module.])],
		[
			[CDEFS="$CDEFS -DENABLE_IOPROXY_RPC"]
			[INCLUDEFLAGS="$INCLUDEFLAGS -I\$(top_srcdir)/include/nfi/nfi_ioproxy-rpc"]
			[NFI_IOPROXY_RPC="nfi_ioproxy-rpc"]
			[NFI_IOPROXY_RPC_OBJECTS="\$(NFI_IOPROXY_RPC_OBJECTS)"]
		]
)
AC_SUBST(NFI_IOPROXY_RPC)
AC_SUBST(NFI_IOPROXY_RPC_OBJECTS)
AM_CONDITIONAL([ENABLE_IOPROXY_RPC], [test "$NFI_IOPROXY_RPC" = "nfi_ioproxy-rpc"])
### END OF IOPROXY_RPC BLOCK. Do not remove this line. ###

### BEGIN OF HTTP BLOCK. Do not remove this line. ###
#
# Defines the necessary variables if http is enabled.
#
AC_ARG_ENABLE(	[http],
		[AS_HELP_STRING([--enable-http],[Enable http module.])],
		[
			[CDEFS="$CDEFS -DENABLE_HTTP"]
			[INCLUDEFLAGS="$INCLUDEFLAGS -I\$(top_srcdir)/include/nfi/nfi_http"]
			[LIBS="$LIBS -L\$(top_srcdir)/src/nfi/nfi_http/fusedav/src -lfusedav -lpthread -L\$(top_srcdir)/src/nfi/nfi_http/fusedav/libneon/src/.libs -lneon -lexpat -lgssapi_krb5 -lssl"]
			[M_HEADERS="$M_HEADERS -I\$(top_srcdir)/src/nfi/nfi_http/fusedav/src -I\$(top_srcdir)/src/nfi/nfi_http/fusedav/libneon/src"]
			[NFI_HTTP="nfi_http"]
			[NFI_HTTP_OBJECTS="\$(NFI_HTTP_OBJECTS)"]
			[FUSEDAV="src/nfi/nfi_http/fusedav"]
		]
)
AC_SUBST(NFI_HTTP)
AC_SUBST(NFI_HTTP_OBJECTS)
AC_SUBST(FUSEDAV)
AM_CONDITIONAL([ENABLE_HTTP], [test "$NFI_HTTP" = "nfi_http"])

if test x$FUSEDAV != x; then
	AC_CONFIG_SUBDIRS([$FUSEDAV])
fi

### END OF HTTP BLOCK. Do not remove this line. ###
### END OF AC_ARG_ENABLE BLOCK. Do not remove this line. ###

AC_ARG_ENABLE(	[fuse],
                [AS_HELP_STRING([--enable-fuse@<:@=/path/to/fuse/@:>@ (Don't use '~')],[Enable FUSE module.])],
		[FUSE="fuse"]
		[case "${enableval}" in
			yes) ;;
			no) FUSE="" ;;
			*) LIBFUSE_A="$enable_fuse/lib/libfuse.a" ;;
		esac]
)
AC_SUBST(FUSE)
AC_SUBST(LIBFUSE_A)

CPPFLAGS="$CDEFS $INCLUDEFLAGS $M_HEADERS $CPPFLAGS"
LDFLAGS="$LIBLINK $LIBS"
AC_SUBST(CPPFLAGS)
AC_SUBST(LDFLAGS)


AC_SUBST(LIBS)
AC_SUBST(M_HEADERS)


# Initialize the test suite.
AC_CONFIG_TESTDIR([test/autotest])
AM_MISSING_PROG([AUTOM4TE], [autom4te])
AC_ARG_WITH(	[ltp],
		[AS_HELP_STRING([--with-ltp],
			[Support for LTP testsuite @<:@default=20080731@:>@])],
		[LTP_VERSION="$with_ltp"],
		[LTP_VERSION=20080731])
AC_SUBST(LTP_VERSION)
AC_SUBST([LTP_PATH], ["\${abs_top_srcdir}/test/autotest/ltp-full-$LTP_VERSION"])
AC_CONFIG_FILES([test/autotest/testsuite.at])

#
# Build makefile and headers...
#
AC_CONFIG_FILES([Makefile lib/Makefile fuse/Makefile utils/Makefile src/nfi/nfi_gsoap/PosixIOService/Makefile external-utils/xpnuploader/Makefile test/autotest/Makefile])
AC_OUTPUT
