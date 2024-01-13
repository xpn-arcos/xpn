#
#  Expand's configure.ac
#  ---------------------
#
#   Process this file with autoconf to produce a configure script.
#

#
# begin
#
AC_INIT([Expand],[2.0],[fgcarbal@inf.uc3m.es],[expand])
AC_CONFIG_AUX_DIR(./config)
AC_CONFIG_HEADERS([include/config.h])
AC_PROG_INSTALL
AC_PROG_RANLIB



#
# Canonicalize the configuration name.
#
AC_CANONICAL_TARGET
AC_CANONICAL_HOST
AC_CANONICAL_BUILD

AM_INIT_AUTOMAKE([1.0 foreign subdir-objects])
AM_CONDITIONAL([am__fastdepCC], [test x$am__fastdepCC = xam__fastdepCC])
AM_CONDITIONAL([AMDEP], [test x$amdep = xamdep])
AM_PROG_AR



#
# Checks for programs.
#
AC_PROG_MAKE_SET
AC_PROG_CC([gcc mpicc icc mpifcc])
AM_PROG_CC_C_O



#
# Check __open2 support
#
MPICC_INTEL=$(mpicc --version | grep "Intel" | wc -l)
if test "x$MPICC_INTEL" == "x1"; then
   AC_DEFINE([HAVE_ICC], [1], [we have icc as mpicc compiler])
fi



#
# Checks for libraries.
#
AC_CHECK_LIB(pthread,pthread_create)
AC_CHECK_LIB(dl,dlopen)
AC_CHECK_LIB(mxml,mxmlNewXML)
AC_CHECK_LIB(mosquitto,mosquitto_lib_init)



#
# Checks for header files.
#
## AC_CHECK_INCLUDES_DEFAULT
AC_PROG_EGREP

AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS(fcntl.h string.h strings.h unistd.h sys/ioctl.h sys/time.h netinet/tcp.h netinet/in.h pthread.h sys/param.h dirent.h rpc/rpc.h rpc/clnt.h rpc/types.h mpi.h mosquitto.h)



#
# Checks for typedefs, structures, and compiler characteristics.
#
AC_C_CONST
AC_CHECK_HEADERS_ONCE([sys/time.h])

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



#
# Set initial variables depending on $target_cpu and $target_os...
#
IFLAGS="-I../../mxml        -I../../bin/mosquitto/include       "
IFLAGS+="-I../../../mxml    -I../../../bin/mosquitto/include    -I../../../xpn/include    -I../../../xpn/include/xpn_client -I../../../xpn/include/base "
IFLAGS+="-I../../../../mxml -I../../../../bin/mosquitto/include -I../../../../xpn/include -I../../../../xpn/include/xpn_client -I../../../../xpn/include/base "

LFLAGS=" -L../../mxml        -L../../bin/mosquitto/lib        -L../../xpn/lib "
LFLAGS+="-L../../../mxml     -L../../../bin/mosquitto/lib     -L../../../xpn/lib "
LFLAGS+="-L../../../../mxml  -L../../../../bin/mosquitto/lib  -L../../../../xpn/lib "

LIBLINK=$LIBS
LIBLINK+=" -lmxml"
LIBS=$LFLAGS" "$LIBS" -lmxml"

if test "$target_cpu" = "AARCH64" -o "$target_cpu" = "aarch64"
then
  OS="__LINUX__"
  CC="mpifcc"
  MAKE="make"

  CDEFS="-D_REENTRANT -DHAVE_CONFIG_H -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64 -DENABLE_MXML"
  CFLAGS=$IFLAGS" -fPIC -O2 -g -std=c11"

elif test "$target_os" = "Linux" -o "$target_os" = "linux" -o "$target_os" = "linux-gnu" -o "$target_os" = "linux gnu"
then
  OS="__LINUX__"
  CC="mpicc"
  MAKE="make"

  CDEFS="-D_REENTRANT -DHAVE_CONFIG_H -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64 -DENABLE_MXML"
  CFLAGS=$IFLAGS" -fPIC -O2 -g -g3 -ggdb -Wall -Wextra -std=c11"

elif test "$target_os" = "AIX" -o "$target_os" = "aix"
then
  OS="__AIX__"
  CC="mpicc"
  MAKE="make"

  CDEFS="-D_REENTRANT -DHAVE_CONFIG_H -D_LARGE_FILES=1 -DENABLE_MXML"
  CFLAGS=$IFLAGS" -O2 -Wall -Wextra -fPIC -g -std=c11"

elif test "$target_os" = "IRIX6.5" -o "$target_os" = "irix6.5"
then
  OS="__IRIX__"
  CC="mpicc"
  MAKE="gmake"

  CDEFS="-D_REENTRANT -DHAVE_CONFIG_H -DENABLE_MXML"
  CFLAGS=$IFLAGS" -O2 -Wall -Wextra -fPIC -g -std=c11"

else
  OS="__LINUX__"
  CC="mpicc"
  MAKE="make"

  CDEFS="-D_REENTRANT -DHAVE_CONFIG_H -DENABLE_MXML"
  CFLAGS=$IFLAGS" -O2 -Wall -Wextra -fPIC -g -std=c11"

  AC_MSG_RESULT("Using Linux configuration as default configuration...");
fi

AC_SUBST(OS)
AC_SUBST(CC)
AC_SUBST(MAKE)
AC_SUBST(CFLAGS)
AC_SUBST(CDEFS)
AC_SUBST(LIBLINK)
AC_MSG_RESULT("$target_cpu");
AC_MSG_RESULT("$target_os");



#
# Set Include_Flags
#
INCLUDEFLAGS="\
-I\$(top_srcdir)/include \
-I\$(top_srcdir)/include/base \
-I\$(top_srcdir)/include/bypass \
-I\$(top_srcdir)/include/xpn_client/ \
-I\$(top_srcdir)/include/xpn_client/xpn \
-I\$(top_srcdir)/include/xpn_client/xpn/xpn_simple \
-I\$(top_srcdir)/include/xpn_client/nfi \
-I\$(top_srcdir)/include/xpn_client/nfi/nfi_mpi_server \
-I\$(top_srcdir)/include/xpn_client/nfi/nfi_sck_server \
-I\$(top_srcdir)/include/xpn_client/nfi/nfi_tcp_server \
-I\$(top_srcdir)/include/xpn_client/nfi/nfi_local"



### BEGIN OF NFS BLOCK. Do not remove this line. ###
#
# Defines the necessary variables if nfs is enabled.
#
AC_ARG_ENABLE(  [nfs],
    [AS_HELP_STRING([--enable-nfs],[Enable NFS v2 module.])],
    [
      [CDEFS="$CDEFS -DENABLE_NFS"]
      [INCLUDEFLAGS="$INCLUDEFLAGS -I\$(top_srcdir)/include/xpn_client/nfi/nfi_nfs"]
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
      [INCLUDEFLAGS="$INCLUDEFLAGS -I\$(top_srcdir)/include/xpn_client/nfi/nfi_nfs3"]
      [NFI_NFS3="nfi_nfs3"]
      [NFI_NFS3_OBJECTS="\$(NFI_NFS3_OBJECTS)"]
    ]
)
AC_SUBST(NFI_NFS3)
AC_SUBST(NFI_NFS3_OBJECTS)
AM_CONDITIONAL([ENABLE_NFS3], [test "$NFI_NFS3" = "nfi_nfs3"])
### END OF NFS3 BLOCK. Do not remove this line. ###


### BEGIN OF MPI_SERVER BLOCK. Do not remove this line. ###
#
# Defines the necessary variables if mpi_server is enabled.
#
AC_ARG_ENABLE(  [mpi_server],
                [AS_HELP_STRING([--enable-mpi_server@<:@=/path/to/mpi/@:>@ (Don't use '~')],[Enable mpi_server module.])],
                [
                  [case "${enableval}" in
                    yes) CC="mpicc";;
                    no)  CC="mpicc" ;;
                    *)   CC="$enableval" ;;
                  esac]
                  [CDEFS="$CDEFS -DENABLE_MPI_SERVER"]
                  [INCLUDEFLAGS="$INCLUDEFLAGS -I\$(top_srcdir)/include/xpn_client/nfi/nfi_mpi_server"]
                  [NFI_MPI_SERVER="nfi_mpi_server"]
                  [NFI_MPI_SERVER_OBJECTS="\$(NFI_MPI_SERVER_OBJECTS)"]
                ]
)
AC_SUBST(CC)
AC_SUBST(NFI_MPI_SERVER)
AC_SUBST(NFI_MPI_SERVER_OBJECTS)
AM_CONDITIONAL([ENABLE_MPI_SERVER], [test "$NFI_MPI_SERVER" = "nfi_mpi_server"])
### END OF MPI_SERVER BLOCK. Do not remove this line. ###


### BEGIN OF SCK_SERVER BLOCK. Do not remove this line. ###
#
# Defines the necessary variables if gridftp is enabled.
#
AC_ARG_ENABLE(  [sck_server],
                [AS_HELP_STRING([--enable-sck_server],[Enable sck_server module.])],
                [
                  [CDEFS="$CDEFS -DENABLE_SCK_SERVER"]
                  [INCLUDEFLAGS="$INCLUDEFLAGS -I\$(top_srcdir)/include/xpn_client/nfi/nfi_sck_server"]
                  [NFI_SCK_SERVER="nfi_sck_server"]
                  [NFI_SCK_SERVER_OBJECTS="\$(NFI_SCK_SERVER_OBJECTS)"]
                ]
)
AC_SUBST(NFI_SCK_SERVER)
AC_SUBST(NFI_SCK_SERVER_OBJECTS)
AM_CONDITIONAL([ENABLE_SCK_SERVER], [test "$NFI_SCK_SERVER" = "nfi_sck_server"])
### END OF SCK_SERVER BLOCK. Do not remove this line. ###


### BEGIN OF MQTT BLOCK. Do not remove this line. ###
#
# Defines the necessary variables if gridftp is enabled.
#
AC_ARG_ENABLE(  [mosquitto],
                [AS_HELP_STRING([--enable-mosquitto],[Enable mosquitto module.])],
                [
                  [CDEFS="$CDEFS -DENABLE_TCP_SERVER -DENABLE_MOSQUITTO -DHAVE_MOSQUITTO_H"]
                  [INCLUDEFLAGS="$INCLUDEFLAGS -I\$(top_srcdir)/include/xpn_client/nfi/nfi_tcp_server"]
                  [NFI_TCP_SERVER="nfi_tcp_server"]
                  [NFI_TCP_SERVER_OBJECTS="\$(NFI_TCP_SERVER_OBJECTS)"]
                  [LIBLINK+=" -lmosquitto"]
                  [LIBS=$LFLAGS" "$LIBS" -lmosquitto"]
                ]
)
AC_SUBST(NFI_TCP_SERVER)
AC_SUBST(NFI_TCP_SERVER_OBJECTS)
AC_SUBST(LIBLINK)
AC_SUBST(LIBS)
AM_CONDITIONAL([ENABLE_TCP_SERVER], [test "$NFI_TCP_SERVER" = "nfi_tcp_server"])
### END OF MQTT BLOCK. Do not remove this line. ###



CPPFLAGS="$CDEFS $INCLUDEFLAGS $M_HEADERS $CPPFLAGS"
LDFLAGS="$LIBLINK $LIBS"
AC_SUBST(CPPFLAGS)
AC_SUBST(LDFLAGS)
AC_SUBST(LIBS)
AC_SUBST(M_HEADERS)



#
# Build makefile and headers...
#
AC_CONFIG_FILES([ \
      Makefile \
      src/base/Makefile \
      src/xpn_client/Makefile \
      src/utils/Makefile \
      src/bypass/Makefile \
      src/mpi_server/Makefile \
      src/sck_server/Makefile \
      src/tcp_server/Makefile \
      test/integrity/xpn/Makefile \
      test/integrity/mpi_connect_accept/Makefile \
      test/integrity/bypass_c/Makefile \
      test/performance/xpn/Makefile \
      test/performance/iop/Makefile \
      test/performance/mpi_pingpong/Makefile \
    ])
    
AC_OUTPUT
