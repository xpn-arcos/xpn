#
#  Expand's configure.ac
#  ---------------------
#
#   Process this file with autoconf to produce a configure script.
#

#
# begin
#
AC_INIT([Expand],[3.3.0],[fgcarbal@inf.uc3m.es],[expand])
AC_CONFIG_AUX_DIR(./config)
AC_CONFIG_HEADERS([include/config.h])
AC_PROG_INSTALL
AC_PROG_RANLIB
AC_PROG_LN_S



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
# Check for compiler
#
dnl If the environment variable CC is set, its value will be taken as the name of the C compiler to use. 
dnl Otherwise, search for a C compiler under a series of likely names, trying gcc and cc first. 
dnl Regardless, the output variable CC is set to the chosen compiler.

AC_PROG_CC([cc gcc icc mpifcc mpicc])
AM_PROG_CC_C_O

MPICC_INTEL=$(mpicc --version | grep "Intel" | wc -l)
if test "x$MPICC_INTEL" == "x1"; then
   AC_DEFINE([HAVE_ICC], [1], [we have icc as mpicc compiler])
fi

MPICC_ANY=$(mpicc --version | wc -l)
if test "$MPICC_ANY" -ne "0"; then
   AC_DEFINE([HAVE_MPICC], [1], [we have an mpicc compiler])
fi

AC_SUBST(CC)


#
# Check for compiler
#

AC_PROG_MAKE_SET

if test "$target_os" = "IRIX6.5" -o "$target_os" = "irix6.5"
then
   MAKE="gmake"
else
   MAKE="make"
fi

AC_SUBST(MAKE)



#
# Checks for libraries.
#
AC_CHECK_LIB(pthread,pthread_create)
AC_CHECK_LIB(dl,dlopen)
AC_CHECK_LIB(mosquitto,mosquitto_lib_init)



#
# Checks for header files.
#
## AC_CHECK_INCLUDES_DEFAULT
AC_PROG_EGREP

AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS(fcntl.h string.h strings.h unistd.h sys/ioctl.h time.h sys/time.h netinet/tcp.h netinet/in.h pthread.h sys/param.h dirent.h rpc/rpc.h rpc/clnt.h rpc/types.h mpi.h mosquitto.h)



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
AC_SYS_LARGEFILE



#
# Checks for library functions.
#
AC_FUNC_VPRINTF
AC_CHECK_FUNCS(gethostname gettimeofday select socket strdup strerror)
AC_CHECK_FUNCS(memcmp memcpy memmove memset)



#
# Set initial variables depending on $target_cpu, $target_os, etc.
#

CFLAGS_IPATH="-I../../bin/mosquitto/include       "
CFLAGS_IPATH+="-I../../../bin/mosquitto/include    -I../../../xpn/include    -I../../../xpn/include/xpn_client    -I../../../xpn/include/base "
CFLAGS_IPATH+="-I../../../../bin/mosquitto/include -I../../../../xpn/include -I../../../../xpn/include/xpn_client -I../../../../xpn/include/base "

CFLAGS_LPATH="-L../../bin/mosquitto/lib         -L../../xpn/lib "
CFLAGS_LPATH+="-L../../../bin/mosquitto/lib     -L../../../xpn/lib "
CFLAGS_LPATH+="-L../../../../bin/mosquitto/lib  -L../../../../xpn/lib "

 CFLAGS_DEBUG="-g -O3 -Wall -Wextra"
#CFLAGS_DEBUG="-g -Wall -Wextra"
#CFLAGS_DEBUG="-g -g3 -ggdb -Wall -Wextra"

CFLAGS_CDEFS=""
CFLAGS_SWITCHES=""


AC_MSG_RESULT("$target_os");
case "$target_os" in
  Linux* | linux*)
    OS="__LINUX__"
    CFLAGS_CDEFS+=" -D_GNU_SOURCE"
    ;;
  AIX*   | aix*)
    OS="__AIX__"
    CFLAGS_CDEFS+=" -D_LARGE_FILES=1"
    ;;
  IRIX*  | irix*)
    OS="__IRIX__"
    ;;
  *)
    OS="__LINUX__"
    AC_MSG_RESULT("GNU/Linux operating by default will be used.");
    ;;
esac

AC_CHECK_SIZEOF([int *])
SIZEOF_INT_P=$ac_cv_sizeof_int_p
if test "x$SIZEOF_INT_P" == "x8"; then
    CFLAGS_CDEFS+=" -DHAVE_64BITS "
    AC_MSG_RESULT("64-bits by default will be used.");
else
    CFLAGS_CDEFS+=" -DHAVE_32BITS"
    AC_MSG_RESULT("32-bits by default will be used.");
fi


LIBLINK=$LIBS
CDEFS="$CFLAGS_CDEFS -D_REENTRANT -DHAVE_CONFIG_H"
CFLAGS="$CFLAGS_SWITCHES $CFLAGS_DEBUG -Wall -Wextra -fPIC -std=c11 $CDEFS $CFLAGS_IPATH"

AC_MSG_RESULT("CFLAGS   so far: "$CFLAGS);
AC_MSG_RESULT("CDEFS    so far: "$CDEFS);
AC_MSG_RESULT("LIBLINK  so far: "$LIBLINK);

AC_SUBST(OS)
AC_SUBST(CFLAGS)
AC_SUBST(CDEFS)
AC_SUBST(LIBLINK)



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
-I\$(top_srcdir)/include/xpn_client/nfi/nfi_local \
-I\$(top_srcdir)/include/xpn_client/nfi/nfi_xpn_server \
-I\$(top_srcdir)/include/xpn_server/ \
-I\$(top_srcdir)/include/xpn_server/mpi_server/ \
-I\$(top_srcdir)/include/xpn_server/sck_server/"


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
# Defines the necessary variables if sck_server is enabled.
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
# Defines the necessary variables if mq_server is enabled.
#
AC_ARG_ENABLE(  [mosquitto],
                [AS_HELP_STRING([--enable-mosquitto],[Enable mosquitto module.])],
                [
                  [CDEFS="$CDEFS -DENABLE_SCK_SERVER -DENABLE_MOSQUITTO -DHAVE_MOSQUITTO_H"]
                  [INCLUDEFLAGS="$INCLUDEFLAGS -I\$(top_srcdir)/include/xpn_client/nfi/nfi_sck_server"]
                  [NFI_SCK_SERVER="nfi_sck_server"]
                  [NFI_SCK_SERVER_OBJECTS="\$(NFI_SCK_SERVER_OBJECTS)"]
                  [LIBLINK+=" -lmosquitto"]
                  [LIBS=$CFLAGS_LPATH" "$LIBS" -lmosquitto"]
                ]
)
AC_SUBST(NFI_SCK_SERVER)
AC_SUBST(NFI_SCK_SERVER_OBJECTS)
AC_SUBST(LIBLINK)
AC_SUBST(LIBS)
AM_CONDITIONAL([ENABLE_SCK_SERVER], [test "$NFI_SCK_SERVER" = "nfi_sck_server"])
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
      src/xpn_server/Makefile \
      test/integrity/xpn/Makefile \
      test/integrity/xpn-iot/Makefile \
      test/integrity/xpn-conf/Makefile \
      test/integrity/mpi_connect_accept/Makefile \
      test/integrity/bypass_c/Makefile \
      test/integrity/xpn_metadata/Makefile \
      test/performance/xpn/Makefile \
      test/performance/iop/Makefile \
      test/performance/mpi_pingpong/Makefile \
      test/performance/xpn-fault-tolerant/Makefile \
    ])
    
AC_OUTPUT

