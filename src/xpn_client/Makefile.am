
###########
# XPN     #
###########

COMMON_HEADER=			@top_srcdir@/include/config.h \
				@top_srcdir@/include/config.h.in \
				@top_srcdir@/include/config_linux.h \
				@top_srcdir@/include/all_system.h \
				@top_srcdir@/include/xpn_client/expand.h \
				@top_srcdir@/include/xpn_client/expand_lib.h

BASE_HEADER=			@top_srcdir@/include/base/base_lib.h \
				@top_srcdir@/include/base/darray.h \
				@top_srcdir@/include/base/base_debug.h \
				@top_srcdir@/include/base/debug_msg.h \
				@top_srcdir@/include/base/debug_tags.h \
				@top_srcdir@/include/base/dtable.h \
				@top_srcdir@/include/base/math_misc.h \
				@top_srcdir@/include/base/string_misc.h \
				@top_srcdir@/include/base/time_misc.h \
				@top_srcdir@/include/base/trace_msg.h \
				@top_srcdir@/include/base/trace_tags.h \
				@top_srcdir@/include/base/urlstr.h \
				@top_srcdir@/include/base/ns.h \
				@top_srcdir@/include/base/syscall_proxies.h \
				@top_srcdir@/include/base/filesystem.h \
				@top_srcdir@/include/base/workers.h \
				@top_srcdir@/include/base/workers_ondemand.h \
				@top_srcdir@/include/base/workers_pool.h \
				@top_srcdir@/include/base/utils.h

NFI_LOCAL_HEADER=		@top_srcdir@/include/xpn_client/nfi/nfi_local/nfi_local_err.h \
				@top_srcdir@/include/xpn_client/nfi/nfi_local/nfi_local.h
### BEGIN OF NFI_MODULE_HEADER BLOCK. Do not remove this line. ###
### BEGIN OF NFI_NFS_HEADER BLOCK. Do not remove this line. ###
NFI_NFS_HEADER=			@top_srcdir@/include/xpn_client/nfi/nfi_nfs/nfi_nfs_err.h \
				@top_srcdir@/include/xpn_client/nfi/nfi_nfs/nfi_nfs.h \
				@top_srcdir@/include/xpn_client/nfi/nfi_nfs/nfs.h
### END OF NFI_NFS_HEADER BLOCK. Do not remove this line. ###
### BEGIN OF NFI_NFS3_HEADER BLOCK. Do not remove this line. ###
NFI_NFS3_HEADER=		@top_srcdir@/include/xpn_client/nfi/nfi_nfs3/nfi_nfs3_err.h \
				@top_srcdir@/include/xpn_client/nfi/nfi_nfs3/nfi_nfs3.h \
				@top_srcdir@/include/xpn_client/nfi/nfi_nfs3/nfs3.h
### END OF NFI_NFS3_HEADER BLOCK. Do not remove this line. ###
### BEGIN OF NFI_TCP_SERVER_HEADER BLOCK. Do not remove this line. ###
NFI_TCP_SERVER_HEADER=		@top_srcdir@/include/xpn_client/nfi/nfi_tcp_server/tcp_server_comm.h \
				@top_srcdir@/include/xpn_client/nfi/nfi_tcp_server/tcp_server_conf.h \
				@top_srcdir@/include/xpn_client/nfi/nfi_tcp_server/tcp_server.h \
				@top_srcdir@/include/xpn_client/nfi/nfi_tcp_server/nfi_tcp_server_err.h \
				@top_srcdir@/include/xpn_client/nfi/nfi_tcp_server/nfi_tcp_server.h
### END OF NFI_TCP_SERVER_HEADER BLOCK. Do not remove this line. ###
### BEGIN OF NFI_MPI_SERVER_HEADER BLOCK. Do not remove this line. ###
NFI_MPI_SERVER_HEADER=		@top_srcdir@/include/xpn_client/nfi/nfi_mpi_server/mpi_server_comm.h \
				@top_srcdir@/include/xpn_client/nfi/nfi_mpi_server/mpi_server_conf.h \
				@top_srcdir@/include/xpn_client/nfi/nfi_mpi_server/nfi_mpi_server_err.h \
				@top_srcdir@/include/xpn_client/nfi/nfi_mpi_server/nfi_mpi_server.h
### END OF NFI_MPI_SERVER_HEADER BLOCK. Do not remove this line. ###
### BEGIN OF NFI_HTTP_HEADER BLOCK. Do not remove this line. ###
NFI_HTTP_HEADER=		@top_srcdir@/include/xpn_client/nfi/nfi_http/nfi_http_err.h \
				@top_srcdir@/include/xpn_client/nfi/nfi_http/nfi_http.h \
				@top_srcdir@/include/xpn_client/nfi/nfi_http/http_lib.h
### END OF NFI_HTTP_HEADER BLOCK. Do not remove this line. ###
### END OF NFI_MODULE_HEADER BLOCK. Do not remove this line. ###

NFI_HEADER=			@top_srcdir@/include/xpn_client/nfi/nfi.h \
				@top_srcdir@/include/xpn_client/nfi/nfi_lib.h \
				@top_srcdir@/include/xpn_client/nfi/nfi_worker_task.h \
				@top_srcdir@/include/xpn_client/nfi/nfi_ops.h \
				@top_srcdir@/include/xpn_client/nfi/nfi_worker.h

NFI_HEADER+=$(NFI_LOCAL_HEADER)

### BEGIN OF NFI_HEADER BLOCK. Do not remove this line. ###
### BEGIN OF ENABLE_NFS_HEADER BLOCK. Do not remove this line. ###
if ENABLE_NFS
NFI_HEADER+=$(NFI_NFS_HEADER)
endif
### END OF ENABLE_NFS_HEADER BLOCK. Do not remove this line. ###
### BEGIN OF ENABLE_NFS3_HEADER BLOCK. Do not remove this line. ###
if ENABLE_NFS3
NFI_HEADER+=$(NFI_NFS3_HEADER)
endif
### END OF ENABLE_NFS3_HEADER BLOCK. Do not remove this line. ###
### BEGIN OF ENABLE_TCP_SERVER_HEADER BLOCK. Do not remove this line. ###
if ENABLE_TCP_SERVER
NFI_HEADER+=$(NFI_TCP_SERVER_HEADER)
endif
### END OF ENABLE_TCP_SERVER_HEADER BLOCK. Do not remove this line. ###
### BEGIN OF ENABLE_MPI_SERVER_HEADER BLOCK. Do not remove this line. ###
if ENABLE_MPI_SERVER
NFI_HEADER+=$(NFI_MPI_SERVER_HEADER)
endif
### END OF ENABLE_MPI_SERVER_HEADER BLOCK. Do not remove this line. ###
### END OF NFI_HEADER BLOCK. Do not remove this line. ###

XPN_HEADER= 		@top_srcdir@/include/xpn_client/xpn.h
			@top_srcdir@/include/xpn_client/xpn/xpn_lib.h \
			@top_srcdir@/include/xpn_client/xpn/xpn_simple/xpn_cwd.h \
			@top_srcdir@/include/xpn_client/xpn/xpn_simple/xpn_dir.h \
			@top_srcdir@/include/xpn_client/xpn/xpn_simple/xpn_err.h \
			@top_srcdir@/include/xpn_client/xpn/xpn_simple/xpn_file.h \
			@top_srcdir@/include/xpn_client/xpn/xpn_simple/xpn.h \
			@top_srcdir@/include/xpn_client/xpn/xpn_simple/xpn_init.h \
			@top_srcdir@/include/xpn_client/xpn/xpn_simple/xpn_opendir.h \
			@top_srcdir@/include/xpn_client/xpn/xpn_simple/xpn_open.h \
			@top_srcdir@/include/xpn_client/xpn/xpn_simple/xpn_policy_cwd.h \
			@top_srcdir@/include/xpn_client/xpn/xpn_simple/xpn_policy_init.h \
			@top_srcdir@/include/xpn_client/xpn/xpn_simple/xpn_policy_opendir.h \
			@top_srcdir@/include/xpn_client/xpn/xpn_simple/xpn_policy_open.h \
			@top_srcdir@/include/xpn_client/xpn/xpn_simple/xpn_policy_rw.h \
			@top_srcdir@/include/xpn_client/xpn/xpn_simple/xpn_rw.h \
			@top_srcdir@/include/xpn_client/xpn/xpn_simple/xpn_simple_lib.h \
			@top_srcdir@/include/xpn_client/xpn/xpn_simple/loc_incl.h \
			@top_srcdir@/include/xpn_client/xpni/xpni.h \
			@top_srcdir@/include/xpn_client/xpni/xpni_null/xpni_null.h \
			@top_srcdir@/include/xpn_client/xpni/xpni_log/xpni_log.h \
			@top_srcdir@/include/xpn_client/xpni/xpni_log/xpni_log_elog.h \
			@top_srcdir@/include/xpn_client/xpni/xpni_ft/xpni_ft.h \
			@top_srcdir@/include/xpn_client/xpni/xpni_ft/xpni_ft_metaData.h \
			@top_srcdir@/include/xpn_client/xpni/xpni_ft/xpni_ft_read.h \
			@top_srcdir@/include/xpn_client/xpni/xpni_ft/xpni_ft_write.h \
			@top_srcdir@/include/xpn_client/xpni/common/xpni_lowfsi_xpn.h \
			@top_srcdir@/include/xpn_client/xpni/common/xpni_lowfsi_printf.h \
			@top_srcdir@/include/xpn_client/xpni/common/xpni_lowfsi_posix.h \
			@top_srcdir@/include/xpn_client/xpni/common/xpni_lowfsi.h \
			@top_srcdir@/include/xpn_client/xpni/common/xpni_fstag.h \
			@top_srcdir@/include/xpn_client/xpni/common/xpni_fsit.h \
			@top_srcdir@/include/xpn_client/xpni/common/xpni_fmeta.h \
			@top_srcdir@/include/xpn_client/xpni/common/xpni_fit.h \
			@top_srcdir@/include/xpn_client/xpni/common/xpni_file.h \
			@top_srcdir@/include/xpn_client/xpni/common/xpni_dir.h \
			@top_srcdir@/include/xpn_client/xpn2.h


###########
# OBJECTS #
###########

BASE_OBJECTS=		@top_srcdir@/src/base/darray.c \
			@top_srcdir@/src/base/dtable.c \
			@top_srcdir@/src/base/math_misc.c \
			@top_srcdir@/src/base/trace_msg.c \
			@top_srcdir@/src/base/debug_msg.c \
			@top_srcdir@/src/base/string_misc.c \
			@top_srcdir@/src/base/time_misc.c \
			@top_srcdir@/src/base/urlstr.c \
			@top_srcdir@/src/base/ns.c \
			@top_srcdir@/src/base/syscall_proxies.c \
			@top_srcdir@/src/base/filesystem.c \
			@top_srcdir@/src/base/workers.c \
			@top_srcdir@/src/base/workers_ondemand.c \
			@top_srcdir@/src/base/workers_pool.c \
			@top_srcdir@/src/base/utils.c

NFI_COMMON_OBJECTS=	@top_srcdir@/src/xpn_client/nfi/common/nfi_ops.c \
			@top_srcdir@/src/xpn_client/nfi/common/nfi_worker.c
NFI_LOCAL_OBJECTS=	@top_srcdir@/src/xpn_client/nfi/nfi_local/nfi_local.c \
			@top_srcdir@/src/xpn_client/nfi/nfi_local/nfi_local_err.c
### BEGIN OF NFI_MODULE_OBJECTS BLOCK. Do not remove this line. ###
### BEGIN OF NFI_NFS_OBJECTS BLOCK. Do not remove this line. ###
NFI_NFS_OBJECTS=	@top_srcdir@/src/xpn_client/nfi/nfi_nfs/nfi_nfs.c \
			@top_srcdir@/src/xpn_client/nfi/nfi_nfs/nfi_nfs_err.c \
			@top_srcdir@/src/xpn_client/nfi/nfi_nfs/nfs.c \
			@top_srcdir@/src/xpn_client/nfi/nfi_nfs/nfs_clnt.c \
			@top_srcdir@/src/xpn_client/nfi/nfi_nfs/nfs_xdr.c
### END OF NFI_NFS_OBJECTS BLOCK. Do not remove this line. ###
### BEGIN OF NFI_NFS3_OBJECTS BLOCK. Do not remove this line. ###
NFI_NFS3_OBJECTS=	@top_srcdir@/src/xpn_client/nfi/nfi_nfs3/nfi_nfs3.c \
			@top_srcdir@/src/xpn_client/nfi/nfi_nfs3/nfi_nfs3_err.c \
			@top_srcdir@/src/xpn_client/nfi/nfi_nfs3/nfs3.c \
			@top_srcdir@/src/xpn_client/nfi/nfi_nfs3/nfs3_clnt.c \
			@top_srcdir@/src/xpn_client/nfi/nfi_nfs3/nfs3_xdr.c
### END OF NFI_NFS3_OBJECTS BLOCK. Do not remove this line. ###
### BEGIN OF NFI_TCP_SERVER_OBJECTS BLOCK. Do not remove this line. ###
NFI_TCP_SERVER_OBJECTS=	@top_srcdir@/src/xpn_client/nfi/nfi_tcp_server/nfi_tcp_server.c \
			@top_srcdir@/src/xpn_client/nfi/nfi_tcp_server/tcp_server_comm.c \
			@top_srcdir@/src/xpn_client/nfi/nfi_tcp_server/nfi_tcp_server_err.c
### END OF NFI_TCP_SERVER_OBJECTS BLOCK. Do not remove this line. ###
### BEGIN OF NFI_MPI_SERVER_OBJECTS BLOCK. Do not remove this line. ###
NFI_MPI_SERVER_OBJECTS=	@top_srcdir@/src/xpn_client/nfi/nfi_mpi_server/nfi_mpi_server.c \
			@top_srcdir@/src/xpn_client/nfi/nfi_mpi_server/mpi_server_comm.c \
			@top_srcdir@/src/xpn_client/nfi/nfi_mpi_server/nfi_mpi_server_err.c
### END OF NFI_MPI_SERVER_OBJECTS BLOCK. Do not remove this line. ###
### BEGIN OF NFI_HTTP_OBJECTS BLOCK. Do not remove this line. ###
NFI_HTTP_OBJECTS=	@top_srcdir@/src/xpn_client/nfi/nfi_http/nfi_http.c \
			@top_srcdir@/src/xpn_client/nfi/nfi_http/nfi_http_err.c
### END OF NFI_HTTP_OBJECTS BLOCK. Do not remove this line. ###
### END OF NFI_MODULE_OBJECTS BLOCK. Do not remove this line. ###

NFI_OBJECTS=$(NFI_COMMON_OBJECTS) $(NFI_LOCAL_OBJECTS)

### BEGIN OF NFI_OBJECTS BLOCK. Do not remove this line. ###
### BEGIN OF ENABLE_NFS_OBJECTS BLOCK. Do not remove this line. ###
if ENABLE_NFS
NFI_OBJECTS+=$(NFI_NFS_OBJECTS)
endif
### END OF ENABLE_NFS_OBJECTS BLOCK. Do not remove this line. ###
### BEGIN OF ENABLE_NFS3_OBJECTS BLOCK. Do not remove this line. ###
if ENABLE_NFS3
NFI_OBJECTS+=$(NFI_NFS3_OBJECTS)
endif
### END OF ENABLE_NFS3_OBJECTS BLOCK. Do not remove this line. ###
### BEGIN OF ENABLE_TCP_SERVER_OBJECTS BLOCK. Do not remove this line. ###
if ENABLE_TCP_SERVER
NFI_OBJECTS+=$(NFI_TCP_SERVER_OBJECTS)
endif
### END OF ENABLE_TCP_SERVER_OBJECTS BLOCK. Do not remove this line. ###
### BEGIN OF ENABLE_MPI_SERVER_OBJECTS BLOCK. Do not remove this line. ###
if ENABLE_MPI_SERVER
NFI_OBJECTS+=$(NFI_MPI_SERVER_OBJECTS)
endif
### END OF ENABLE_MPI_SERVER_OBJECTS BLOCK. Do not remove this line. ###
### END OF NFI_OBJECTS BLOCK. Do not remove this line. ###

XPN_CORE_OBJECTS=			@top_srcdir@/src/xpn_client/xpn/xpn_simple/xpncore/xpn_cwd.c \
					@top_srcdir@/src/xpn_client/xpn/xpn_simple/xpncore/xpn_dir.c \
					@top_srcdir@/src/xpn_client/xpn/xpn_simple/xpncore/xpn_err.c \
					@top_srcdir@/src/xpn_client/xpn/xpn_simple/xpncore/xpn_file.c \
					@top_srcdir@/src/xpn_client/xpn/xpn_simple/xpncore/xpn_init.c \
					@top_srcdir@/src/xpn_client/xpn/xpn_simple/xpncore/xpn_open.c \
					@top_srcdir@/src/xpn_client/xpn/xpn_simple/xpncore/xpn_opendir.c \
					@top_srcdir@/src/xpn_client/xpn/xpn_simple/xpncore/xpn_rw.c \
					@top_srcdir@/src/xpn_client/xpn/xpn_simple/xpncore/xpn_fopen.c \
					@top_srcdir@/src/xpn_client/xpn/xpn_simple/xpncore/xpn_fclose.c \
					@top_srcdir@/src/xpn_client/xpn/xpn_simple/xpncore/xpn_fread.c \
					@top_srcdir@/src/xpn_client/xpn/xpn_simple/xpncore/xpn_fwrite.c \
					@top_srcdir@/src/xpn_client/xpn/xpn_simple/xpncore/xpn_fflush.c \
					@top_srcdir@/src/xpn_client/xpn/xpn_simple/xpncore/xpn_fillbuf.c \
					@top_srcdir@/src/xpn_client/xpn/xpn_simple/xpncore/xpn_flushbuf.c \
					@top_srcdir@/src/xpn_client/xpn/xpn_simple/xpncore/xpn_fseek.c \
					@top_srcdir@/src/xpn_client/xpn/xpn_simple/xpncore/xpn_ftell.c \
					@top_srcdir@/src/xpn_client/xpn/xpn_simple/xpncore/xpn_fileno.c \
					@top_srcdir@/src/xpn_client/xpn/xpn_simple/xpncore/xpn_setbuf.c \
					@top_srcdir@/src/xpn_client/xpn/xpn_simple/xpncore/xpn_setvbuf.c

XPN_POLICY_OBJECTS=			@top_srcdir@/src/xpn_client/xpn/xpn_simple/policy/xpn_policy_cwd.c \
					@top_srcdir@/src/xpn_client/xpn/xpn_simple/policy/xpn_policy_err.c \
					@top_srcdir@/src/xpn_client/xpn/xpn_simple/policy/xpn_policy_init.c \
					@top_srcdir@/src/xpn_client/xpn/xpn_simple/policy/xpn_policy_open.c \
					@top_srcdir@/src/xpn_client/xpn/xpn_simple/policy/xpn_policy_opendir.c \
					@top_srcdir@/src/xpn_client/xpn/xpn_simple/policy/xpn_policy_rw.c

XPN_OBJECTS=$(XPN_CORE_OBJECTS) $(XPN_POLICY_OBJECTS)

XPNI_OBJECTS=				@top_srcdir@/src/xpn_client/xpni/common/xpni_fit.c \
					@top_srcdir@/src/xpn_client/xpni/common/xpni_fmeta.c \
					@top_srcdir@/src/xpn_client/xpni/common/xpni_fsit.c \
					@top_srcdir@/src/xpn_client/xpni/common/xpni_file.c \
					@top_srcdir@/src/xpn_client/xpni/common/xpni_dir.c \
					@top_srcdir@/src/xpn_client/xpni/xpni_ft/xpni_ft_close.c \
					@top_srcdir@/src/xpn_client/xpni/xpni_ft/xpni_ft_destroy.c \
					@top_srcdir@/src/xpn_client/xpni/xpni_ft/xpni_ft_exportFile.c \
					@top_srcdir@/src/xpn_client/xpni/xpni_ft/xpni_ft_importFile.c \
					@top_srcdir@/src/xpn_client/xpni/xpni_ft/xpni_ft_init.c \
					@top_srcdir@/src/xpn_client/xpni/xpni_ft/xpni_ft_lseek.c \
					@top_srcdir@/src/xpn_client/xpni/xpni_ft/xpni_ft_metaData.c \
					@top_srcdir@/src/xpn_client/xpni/xpni_ft/xpni_ft_open.c \
					@top_srcdir@/src/xpn_client/xpni/xpni_ft/xpni_ft_read.c \
					@top_srcdir@/src/xpn_client/xpni/xpni_ft/xpni_ft_register.c \
					@top_srcdir@/src/xpn_client/xpni/xpni_ft/xpni_ft_write.c \
					@top_srcdir@/src/xpn_client/xpni/xpni_ft/xpni_ft_opendir.c \
					@top_srcdir@/src/xpn_client/xpni/xpni_null/xpni_null_close.c \
					@top_srcdir@/src/xpn_client/xpni/xpni_null/xpni_null_destroy.c \
					@top_srcdir@/src/xpn_client/xpni/xpni_null/xpni_null_init.c \
					@top_srcdir@/src/xpn_client/xpni/xpni_null/xpni_null_open.c \
					@top_srcdir@/src/xpn_client/xpni/xpni_null/xpni_null_read.c \
					@top_srcdir@/src/xpn_client/xpni/xpni_null/xpni_null_register.c \
					@top_srcdir@/src/xpn_client/xpni/xpni_null/xpni_null_write.c \
					@top_srcdir@/src/xpn_client/xpni/xpni_null/xpni_null_importFile.c \
					@top_srcdir@/src/xpn_client/xpni/xpni_null/xpni_null_exportFile.c \
					@top_srcdir@/src/xpn_client/xpni/xpni_null/xpni_null_lseek.c \
					@top_srcdir@/src/xpn_client/xpni/xpni_null/xpni_null_opendir.c \
					@top_srcdir@/src/xpn_client/xpni/xpni_log/xpni_log_close.c \
					@top_srcdir@/src/xpn_client/xpni/xpni_log/xpni_log_destroy.c \
					@top_srcdir@/src/xpn_client/xpni/xpni_log/xpni_log_exportFile.c \
					@top_srcdir@/src/xpn_client/xpni/xpni_log/xpni_log_importFile.c \
					@top_srcdir@/src/xpn_client/xpni/xpni_log/xpni_log_init.c \
					@top_srcdir@/src/xpn_client/xpni/xpni_log/xpni_log_lseek.c \
					@top_srcdir@/src/xpn_client/xpni/xpni_log/xpni_log_open.c \
					@top_srcdir@/src/xpn_client/xpni/xpni_log/xpni_log_read.c \
					@top_srcdir@/src/xpn_client/xpni/xpni_log/xpni_log_register.c \
					@top_srcdir@/src/xpn_client/xpni/xpni_log/xpni_log_write.c \
					@top_srcdir@/src/xpn_client/xpni/xpni_log/xpni_log_opendir.c \
					@top_srcdir@/src/xpn_client/xpni/xpni_log/xpni_log_elog.c

XPN2_OBJECTS=		@top_srcdir@/src/xpn_client/xpn2/xpn2_close.c \
					@top_srcdir@/src/xpn_client/xpn2/xpn2_destroy.c \
					@top_srcdir@/src/xpn_client/xpn2/xpn2_init.c \
					@top_srcdir@/src/xpn_client/xpn2/xpn2_open.c \
					@top_srcdir@/src/xpn_client/xpn2/xpn2_creat.c \
					@top_srcdir@/src/xpn_client/xpn2/xpn2_read.c \
					@top_srcdir@/src/xpn_client/xpn2/xpn2_write.c \
					@top_srcdir@/src/xpn_client/xpn2/xpn2_ioctl.c \
					@top_srcdir@/src/xpn_client/xpn2/xpn2_lseek.c


#############
# LIBRARIES #
#############

OBJEXT            = o
lib_LIBRARIES     = libxpn.a
libxpn_a_SOURCES  = $(BASE_OBJECTS) $(NFI_OBJECTS) $(XPN_OBJECTS) $(XPNI_OBJECTS) $(XPN2_OBJECTS)
AM_CFLAGS         = -DASYNC_CLOSE=1 -DFILESYSTEM_DLSYM=1 

noinst_HEADERS         = $(BASE_HEADER)
nobase_includedir      = $(includedir)/xpn_client
nobase_include_HEADERS = $(COMMON_HEADER) $(BASE_HEADER) $(NFI_HEADER) $(XPN_HEADER)

install-data-hook:
	mkdir -p $(includedir)
	[ -d $(includedir)/../../include ] && cp  -a $(includedir)/../../include/*  $(includedir)/
	[ -d $(includedir)/../../include ] && rm -fr $(includedir)/../../include

