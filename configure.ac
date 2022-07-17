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
AC_INIT([Expand],[1.0],[fgcarbal@inf.uc3m.es],[expand])
AC_CONFIG_AUX_DIR(./config)
AC_CONFIG_HEADER(include/config.h)
AC_PROG_INSTALL
AC_PROG_RANLIB

#
# Canonicalize the configuration name.
#
AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE([1.0 foreign subdir-objects])
AM_CONDITIONAL([am__fastdepCC], [test x$am__fastdepCC = xam__fastdepCC])
AM_CONDITIONAL([AMDEP], [test x$amdep = xamdep])
AM_PROG_CC_C_O
AM_PROG_AR

#
# Checks for programs.
#
AC_PROG_MAKE_SET
AC_PROG_CC([mpicc icc gcc])


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


if test "$target_os" = "Linux" -o "$target_os" = "linux" -o "$target_os" = "linux-gnu" -o "$target_os" = "linux gnu"
then
  OS="__LINUX__"
  CC="mpicc"
  MAKE="make"
  CFLAGS="-fPIC -O2 -g -g3 -ggdb -Wall -Wextra -std=c11"
  CDEFS="-D_REENTRANT -DPOSIX_THREADS -DHAVE_CONFIG_H -DLINUX -DXPNI_LOWFSI_XPN -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64 -DENABLE_MXML"
  LIBLINK="-lpthread"
  LIBS+="-L\$(HOME)/bin/mxml/lib -lmxml -ldl"
  AC_SUBST(OS)
  AC_SUBST(CC)
  AC_SUBST(MAKE)
  AC_SUBST(CFLAGS)
  AC_SUBST(CDEFS)
  AC_SUBST(LIBLINK)
  AC_MSG_RESULT("$target_os");
elif test "$target_os" = "AIX" -o "$target_os" = "aix"
then
  OS="__AIX__"
  #CC="g++"
  MAKE="make"
  CFLAGS="-O2 -Wall -Wextra"
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
  CFLAGS="-O2 -Wall -Wextra"
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
 #CDEFS="-D_REENTRANT -DPOSIX_THREADS -DLINUX -DDEBUG -DDEBUG_NFS -DXPNI_LOWFSI_POSIX -DXPNI_LOWFSI_XPN"
  CDEFS="-D_REENTRANT -DPOSIX_THREADS -DHAVE_CONFIG_H -DLINUX -DXPNI_LOWFSI_XPN"
  LIBLINK="-lpthread "
  AC_SUBST(OS)
  AC_SUBST(CC)
  AC_SUBST(MAKE)
  AC_SUBST(CFLAGS)
  AC_SUBST(CDEFS)
  AC_SUBST(LIBLINK)
  AC_MSG_RESULT("no defined \(using Linux configuration as default configuration\)");
fi

INCLUDEFLAGS="\
-I\$(HOME)/bin/mxml/include \
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
AC_ARG_ENABLE(  [nfs],
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
AC_ARG_ENABLE(  [nfs3],
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


### BEGIN OF TCPSERVER BLOCK. Do not remove this line. ###
#
# Defines the necessary variables if gridftp is enabled.
#
AC_ARG_ENABLE(  [tcpserver],
                [AS_HELP_STRING([--enable-tcpserver],[Enable tcpServer module.])],
                [
                  [CDEFS="$CDEFS -DENABLE_TCPSERVER"]
                  [INCLUDEFLAGS="$INCLUDEFLAGS -I\$(top_srcdir)/include/nfi/nfi_tcpServer"]
                  [NFI_TCPSERVER="nfi_tcpserver"]
                  [NFI_TCPSERVER_OBJECTS="\$(NFI_TCPSERVER_OBJECTS)"]
                ]
)
AC_SUBST(NFI_TCPSERVER)
AC_SUBST(NFI_TCPSERVER_OBJECTS)
AM_CONDITIONAL([ENABLE_TCPSERVER], [test "$NFI_TCPSERVER" = "nfi_tcpserver"])
### END OF TCPSERVER BLOCK. Do not remove this line. ###

### BEGIN OF MPISERVER BLOCK. Do not remove this line. ###
#
# Defines the necessary variables if gridftp is enabled.
#
AC_ARG_ENABLE(  [mpiserver],
                [AS_HELP_STRING([--enable-mpiserver@<:@=/path/to/mpi/@:>@ (Don't use '~')],[Enable mpiServer module.])],
                [
                  [case "${enableval}" in
                    yes) CC="mpicc";;
                    no)  CC="mpicc" ;;
                    *)   CC="$enableval/mpicc" ;;
                  esac]
                  [CDEFS="$CDEFS -DENABLE_MPISERVER"]
                  [INCLUDEFLAGS="$INCLUDEFLAGS -I\$(top_srcdir)/include/nfi/nfi_mpiServer"]
                  [NFI_MPISERVER="nfi_mpiserver"]
                  [NFI_MPISERVER_OBJECTS="\$(NFI_MPISERVER_OBJECTS)"]
                ]
)
AC_SUBST(CC)
AC_SUBST(NFI_MPISERVER)
AC_SUBST(NFI_MPISERVER_OBJECTS)
AM_CONDITIONAL([ENABLE_MPISERVER], [test "$NFI_MPISERVER" = "nfi_mpiserver"])
### END OF MPISERVER BLOCK. Do not remove this line. ###


CPPFLAGS="$CDEFS $INCLUDEFLAGS $M_HEADERS $CPPFLAGS"
LDFLAGS="$LIBLINK $LIBS"
AC_SUBST(CPPFLAGS)
AC_SUBST(LDFLAGS)
AC_SUBST(LIBS)
AC_SUBST(M_HEADERS)


#
# Build makefile and headers...
#
AC_CONFIG_FILES([Makefile lib/Makefile utils/Makefile src_bypass/Makefile src_servers/tcpServer/Makefile src_servers/mpiServer/Makefile])
AC_OUTPUT

