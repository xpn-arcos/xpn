
###########
# XPN     #
###########

COMMON_HEADER=			@top_srcdir@/include/all_system.h \
				@top_srcdir@/include/xpn_client/expand.h \
				@top_srcdir@/include/xpn_client/expand_lib.h

BASE_HEADER=			@top_srcdir@/include/base/

XPN_SERVER_HEADER=		@top_srcdir@/include/xpn_server/xpn_server_params.h \
				@top_srcdir@/include/xpn_server/xpn_server_conf.h \
				@top_srcdir@/include/xpn_server/xpn_server_ops.h \
				@top_srcdir@/include/xpn_server/xpn_server_comm.h
MPI_SERVER_HEADER=		@top_srcdir@/include/xpn_server/mpi_server/mpi_server_comm.h
SCK_SERVER_HEADER=		@top_srcdir@/include/xpn_server/sck_server/sck_server_comm.h

SERVER_HEADER=$(XPN_SERVER_HEADER)
SERVER_HEADER+=$(COMMON_HEADER)
SERVER_HEADER+=$(BASE_HEADER)

if ENABLE_MPI_SERVER
SERVER_HEADER+=$(MPI_SERVER_HEADER)
endif

if ENABLE_SCK_SERVER
SERVER_HEADER+=$(SCK_SERVER_HEADER)
endif


###########
# OBJECTS #
###########

BASE_OBJECTS=		@top_srcdir@/src/base/darray.c \
			@top_srcdir@/src/base/dtable.c \
			@top_srcdir@/src/base/math_misc.c \
			@top_srcdir@/src/base/trace_msg.c \
			@top_srcdir@/src/base/debug_msg.c \
			@top_srcdir@/src/base/string_misc.c \
			@top_srcdir@/src/base/path_misc.c \
			@top_srcdir@/src/base/time_misc.c \
			@top_srcdir@/src/base/urlstr.c \
			@top_srcdir@/src/base/ns.c \
			@top_srcdir@/src/base/socket.c \
			@top_srcdir@/src/base/syscall_proxies.c \
			@top_srcdir@/src/base/filesystem.c \
			@top_srcdir@/src/base/workers.c \
			@top_srcdir@/src/base/workers_ondemand.c \
			@top_srcdir@/src/base/workers_pool.c \
			@top_srcdir@/src/base/utils.c

XPN_SERVER_OBJECTS=	@top_srcdir@/src/xpn_server/xpn_server.c \
			@top_srcdir@/src/xpn_server/xpn_server_params.c \
			@top_srcdir@/src/xpn_server/xpn_server_ops.c \
			@top_srcdir@/src/xpn_server/xpn_server_comm.c

MPI_SERVER_OBJECTS=	@top_srcdir@/src/xpn_server/mpi_server/mpi_server_comm.c
SCK_SERVER_OBJECTS=	@top_srcdir@/src/xpn_server/sck_server/sck_server_comm.c
					
SERVER_OBJECTS=$(XPN_SERVER_OBJECTS)
SERVER_OBJECTS+=$(BASE_OBJECTS)

if ENABLE_MPI_SERVER
SERVER_OBJECTS+=$(MPI_SERVER_OBJECTS)
endif

if ENABLE_SCK_SERVER
SERVER_OBJECTS+=$(SCK_SERVER_OBJECTS)
endif



############
# BINARIES #
############

bin_PROGRAMS = xpn_server 
xpn_server_SOURCES =  $(SERVER_OBJECTS) $(SERVER_HEADER)
bin_SCRIPTS = xpn_stop_server$(EXEEXT) xpn_terminate_server$(EXEEXT) xpn_server_spawn$(EXEEXT)

CLEANFILES = $(bin_SCRIPTS)

xpn_stop_server$(EXEEXT): xpn_server$(EXEEXT)
	$(LN_S) -f xpn_server$(EXEEXT) xpn_stop_server$(EXEEXT)

xpn_terminate_server$(EXEEXT): xpn_server$(EXEEXT)
	$(LN_S) -f xpn_server$(EXEEXT) xpn_terminate_server$(EXEEXT)

xpn_server_spawn$(EXEEXT): xpn_server$(EXEEXT)
	$(LN_S) -f xpn_server$(EXEEXT) xpn_server_spawn$(EXEEXT)

install-exec-hook:
	$(LN_S) -f $(DESTDIR)$(bindir)/xpn_server$(EXEEXT) $(DESTDIR)$(bindir)/xpn_stop_server$(EXEEXT)
	$(LN_S) -f $(DESTDIR)$(bindir)/xpn_server$(EXEEXT) $(DESTDIR)$(bindir)/xpn_terminate_server$(EXEEXT)
	$(LN_S) -f $(DESTDIR)$(bindir)/xpn_server$(EXEEXT) $(DESTDIR)$(bindir)/xpn_server_spawn$(EXEEXT)