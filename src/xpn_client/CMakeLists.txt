###########
# OBJECTS #
###########
set (BASE_OBJECTS	base/darray.c
			base/dtable.c
			base/math_misc.c
			base/trace_msg.c
			base/debug_msg.c
			base/string_misc.c
			base/time_misc.c
			base/urlstr.c
    )
set (NFI_COMMON_OBJECTS	nfi/common/nfi_worker.c)
set (NFI_LOCAL_OBJECTS	nfi/nfi_local/nfi_local.c
			nfi/nfi_local/nfi_local_err.c
    )
### BEGIN OF NFI_MODULE_OBJECTS BLOCK. Do not remove this line. ###
### BEGIN OF NFI_NFS_OBJECTS BLOCK. Do not remove this line. ###
set (NFI_NFS_OBJECTS	nfi/nfi_nfs/nfi_nfs.c
			nfi/nfi_nfs/nfi_nfs_err.c
			nfi/nfi_nfs/nfs.c
			nfi/nfi_nfs/nfs_clnt.c
			nfi/nfi_nfs/nfs_xdr.c
    )
### END OF NFI_NFS_OBJECTS BLOCK. Do not remove this line. ###
### BEGIN OF NFI_NFS3_OBJECTS BLOCK. Do not remove this line. ###
set (NFI_NFS3_OBJECTS	nfi/nfi_nfs3/nfi_nfs3.c
			nfi/nfi_nfs3/nfi_nfs3_err.c
			nfi/nfi_nfs3/nfs3.c
			nfi/nfi_nfs3/nfs3_clnt.c
			nfi/nfi_nfs3/nfs3_xdr.c
    )
### END OF NFI_NFS3_OBJECTS BLOCK. Do not remove this line. ###
### BEGIN OF NFI_FTP_OBJECTS BLOCK. Do not remove this line. ###
set (NFI_FTP_OBJECTS	nfi/nfi_ftp/ftp.c
			nfi/nfi_ftp/list_dir.c
			nfi/nfi_ftp/nfi_ftp.c
			nfi/nfi_ftp/nfi_ftp_err.c
    )
### END OF NFI_FTP_OBJECTS BLOCK. Do not remove this line. ###
### BEGIN OF NFI_XIO_OBJECTS BLOCK. Do not remove this line. ###
set (NFI_XIO_OBJECTS	nfi/nfi_xio/nfi_xio.c
			nfi/nfi_xio/nfi_xio_err.c
    )
### END OF NFI_XIO_OBJECTS BLOCK. Do not remove this line. ###
### BEGIN OF NFI_GSOAP_OBJECTS BLOCK. Do not remove this line. ###
set (NFI_GSOAP_OBJECTS	nfi/nfi_gsoap/nfi_gsoap.c
			nfi/nfi_gsoap/nfi_gsoap_err.c
    )
### END OF NFI_GSOAP_OBJECTS BLOCK. Do not remove this line. ###
### BEGIN OF NFI_GRIDFTP_OBJECTS BLOCK. Do not remove this line. ###
set (NFI_GRIDFTP_OBJECTS	nfi/nfi_gridftp/nfi_gridftp.c
				nfi/nfi_gridftp/nfi_gridftp_err.c
    )
### END OF NFI_GRIDFTP_OBJECTS BLOCK. Do not remove this line. ###
### BEGIN OF NFI_MPI_SERVER_OBJECTS BLOCK. Do not remove this line. ###
set (NFI_MPI_SERVER_OBJECTS	nfi/nfi_mpi_server/nfi_mpi_server.c
				nfi/nfi_mpi_server/mpi_server_comm.c
				nfi/nfi_mpi_server/nfi_mpi_server_err.c
    )
### END OF NFI_MPI_SERVER_OBJECTS BLOCK. Do not remove this line. ###
### BEGIN OF NFI_SCK_SERVER_OBJECTS BLOCK. Do not remove this line. ###
set (NFI_SCK_SERVER_OBJECTS	nfi/nfi_sck_server/nfi_sck_server.c
				nfi/nfi_sck_server/sck_server_comm.c
				nfi/nfi_sck_server/nfi_sck_server_err.c
    )
### END OF NFI_SCK_SERVER_OBJECTS BLOCK. Do not remove this line. ###
### BEGIN OF NFI_TCP_SERVER_OBJECTS BLOCK. Do not remove this line. ###
set (NFI_TCP_SERVER_OBJECTS	nfi/nfi_tcp_server/nfi_tcp_server.c
				nfi/nfi_tcp_server/tcp_server_comm.c
				nfi/nfi_tcp_server/nfi_tcp_server_err.c
    )
### END OF NFI_TCP_SERVER_OBJECTS BLOCK. Do not remove this line. ###
### BEGIN OF NFI_IOPROXY_RPC_OBJECTS BLOCK. Do not remove this line. ###
set (NFI_IOPROXY_RPC_OBJECTS	nfi/nfi_ioproxy-rpc/nfi_ioproxy-rpc.c
				nfi/nfi_ioproxy-rpc/nfi_ioproxy-rpc_err.c
    )
### END OF NFI_IOPROXY_RPC_OBJECTS BLOCK. Do not remove this line. ###
### BEGIN OF NFI_HTTP_OBJECTS BLOCK. Do not remove this line. ###
set (NFI_HTTP_OBJECTS	nfi/nfi_http/nfi_http.c
			nfi/nfi_http/nfi_http_err.c
    )
### END OF NFI_HTTP_OBJECTS BLOCK. Do not remove this line. ###
### END OF NFI_MODULE_OBJECTS BLOCK. Do not remove this line. ###

set (NFI_OBJECTS	${NFI_COMMON_OBJECTS} ${NFI_LOCAL_OBJECTS})

### BEGIN OF NFI_OBJECTS BLOCK. Do not remove this line. ###
### BEGIN OF ENABLE_NFS_OBJECTS BLOCK. Do not remove this line. ###
if (ENABLE_NFS)
	set (NFI_OBJECTS ${NFI_OBJECTS} ${NFI_NFS_OBJECTS})
endif (ENABLE_NFS)
### END OF ENABLE_NFS_OBJECTS BLOCK. Do not remove this line. ###
### BEGIN OF ENABLE_NFS3_OBJECTS BLOCK. Do not remove this line. ###
if (ENABLE_NFS3)
	set (NFI_OBJECTS ${NFI_OBJECTS} ${NFI_NFS3_OBJECTS})
endif (ENABLE_NFS3)
### END OF ENABLE_NFS3_OBJECTS BLOCK. Do not remove this line. ###
### BEGIN OF ENABLE_FTP_OBJECTS BLOCK. Do not remove this line. ###
if (ENABLE_FTP)
	set (NFI_OBJECTS ${NFI_OBJECTS} ${NFI_FTP_OBJECTS})
endif (ENABLE_FTP)
### END OF ENABLE_FTP_OBJECTS BLOCK. Do not remove this line. ###
### BEGIN OF ENABLE_XIO_OBJECTS BLOCK. Do not remove this line. ###
if (ENABLE_XIO)
	set (NFI_OBJECTS ${NFI_OBJECTS} ${NFI_XIO_OBJECTS})
endif (ENABLE_XIO)
### END OF ENABLE_XIO_OBJECTS BLOCK. Do not remove this line. ###
### BEGIN OF ENABLE_GSOAP_OBJECTS BLOCK. Do not remove this line. ###
if (ENABLE_GSOAP)
	set (NFI_OBJECTS ${NFI_OBJECTS} ${NFI_GSOAP_OBJECTS})
endif (ENABLE_GSOAP)
### END OF ENABLE_GSOAP_OBJECTS BLOCK. Do not remove this line. ###
### BEGIN OF ENABLE_GRIDFTP_OBJECTS BLOCK. Do not remove this line. ###
if (ENABLE_GRIDFTP)
	set (NFI_OBJECTS ${NFI_OBJECTS} ${NFI_GRIDFTP_OBJECTS})
endif (ENABLE_GRIDFTP)
### END OF ENABLE_GRIDFTP_OBJECTS BLOCK. Do not remove this line. ###
### BEGIN OF ENABLE_TCP_SERVER_OBJECTS BLOCK. Do not remove this line. ###
if (ENABLE_TCP_SERVER)
	set (NFI_OBJECTS ${NFI_OBJECTS} ${NFI_TCP_SERVER_OBJECTS})
endif (ENABLE_TCP_SERVER)
### END OF ENABLE_TCP_SERVER_OBJECTS BLOCK. Do not remove this line. ###
### BEGIN OF ENABLE_MPI_SERVER_OBJECTS BLOCK. Do not remove this line. ###
if (ENABLE_MPI_SERVER)
	set (NFI_OBJECTS ${NFI_OBJECTS} ${NFI_MPI_SERVER_OBJECTS})
endif (ENABLE_MPI_SERVER)
### END OF ENABLE_MPI_SERVER_OBJECTS BLOCK. Do not remove this line. ###
### BEGIN OF ENABLE_IOPROXY_RPC_OBJECTS BLOCK. Do not remove this line. ###
if (ENABLE_IOPROXY_RPC)
	set (NFI_OBJECTS ${NFI_OBJECTS} ${NFI_IOPROXY_RPC_OBJECTS})
endif (ENABLE_IOPROXY_RPC)
### END OF ENABLE_IOPROXY_RPC_OBJECTS BLOCK. Do not remove this line. ###
### BEGIN OF ENABLE_HTTP_OBJECTS BLOCK. Do not remove this line. ###
if (ENABLE_HTTP)
	set (NFI_OBJECTS ${NFI_OBJECTS} ${NFI_HTTP_OBJECTS})
endif (ENABLE_HTTP)
### END OF ENABLE_HTTP_OBJECTS BLOCK. Do not remove this line. ###
### END OF NFI_OBJECTS BLOCK. Do not remove this line. ###

set (XPN_CORE_OBJECTS	xpn/xpn_simple/xpncore/xpn_cwd.c
			xpn/xpn_simple/xpncore/xpn_dir.c
			xpn/xpn_simple/xpncore/xpn_err.c
			xpn/xpn_simple/xpncore/xpn_file.c
			xpn/xpn_simple/xpncore/xpn_init.c
			xpn/xpn_simple/xpncore/xpn_open.c
			xpn/xpn_simple/xpncore/xpn_opendir.c
			xpn/xpn_simple/xpncore/xpn_rw.c
			xpn/xpn_simple/xpncore/xpn_fopen.c
			xpn/xpn_simple/xpncore/xpn_fclose.c
			xpn/xpn_simple/xpncore/xpn_fread.c
			xpn/xpn_simple/xpncore/xpn_fwrite.c
			xpn/xpn_simple/xpncore/xpn_fflush.c
			xpn/xpn_simple/xpncore/xpn_fillbuf.c
			xpn/xpn_simple/xpncore/xpn_flushbuf.c
			xpn/xpn_simple/xpncore/xpn_fseek.c
			xpn/xpn_simple/xpncore/xpn_ftell.c
#			xpn/xpn_simple/xpncore/xpn_fileno.c
#			xpn/xpn_simple/xpncore/xpn_f.c
#			xpn/xpn_simple/xpncore/xpn_setbuf.c
#			xpn/xpn_simple/xpncore/xpn_setvbuf.c
    )
set (XPN_POLICY_OBJECTS	xpn/xpn_simple/policy/xpn_policy_cwd.c
			xpn/xpn_simple/policy/xpn_policy_init.c
			xpn/xpn_simple/policy/xpn_policy_open.c
			xpn/xpn_simple/policy/xpn_policy_opendir.c
			xpn/xpn_simple/policy/xpn_policy_rw.c
    )

set (XPN_OBJECTS	${XPN_CORE_OBJECTS} ${XPN_POLICY_OBJECTS})

set (XPNI_OBJECTS	xpni/common/xpni_fit.c
			xpni/common/xpni_fmeta.c
			xpni/common/xpni_fsit.c
			xpni/common/xpni_file.c
			xpni/common/xpni_dir.c
			xpni/xpni_ft/xpni_ft_close.c
			xpni/xpni_ft/xpni_ft_destroy.c
			xpni/xpni_ft/xpni_ft_exportFile.c
			xpni/xpni_ft/xpni_ft_importFile.c
			xpni/xpni_ft/xpni_ft_init.c
			xpni/xpni_ft/xpni_ft_lseek.c
			xpni/xpni_ft/xpni_ft_metaData.c
			xpni/xpni_ft/xpni_ft_open.c
			xpni/xpni_ft/xpni_ft_read.c
			xpni/xpni_ft/xpni_ft_read_fail_r5i.c
			xpni/xpni_ft/xpni_ft_read_fail_r5o.c
			xpni/xpni_ft/xpni_ft_read_nofail_r5i.c
			xpni/xpni_ft/xpni_ft_read_nofail_r5o.c
			xpni/xpni_ft/xpni_ft_read_r5i.c
			xpni/xpni_ft/xpni_ft_read_r5o.c
			xpni/xpni_ft/xpni_ft_register.c
			xpni/xpni_ft/xpni_ft_write.c
			xpni/xpni_ft/xpni_ft_write_fail_r5i.c
			xpni/xpni_ft/xpni_ft_write_fail_r5o.c
			xpni/xpni_ft/xpni_ft_write_nofail_r5i.c
			xpni/xpni_ft/xpni_ft_write_nofail_r5o.c
			xpni/xpni_ft/xpni_ft_write_r5i.c
			xpni/xpni_ft/xpni_ft_write_r5o.c
			xpni/xpni_ft/xpni_ft_opendir.c
			xpni/xpni_null/xpni_null_close.c
			xpni/xpni_null/xpni_null_destroy.c
			xpni/xpni_null/xpni_null_init.c
			xpni/xpni_null/xpni_null_open.c
			xpni/xpni_null/xpni_null_read.c
			xpni/xpni_null/xpni_null_register.c
			xpni/xpni_null/xpni_null_write.c
			xpni/xpni_null/xpni_null_importFile.c
			xpni/xpni_null/xpni_null_exportFile.c
			xpni/xpni_null/xpni_null_lseek.c
			xpni/xpni_null/xpni_null_opendir.c
			xpni/xpni_log/xpni_log_close.c
			xpni/xpni_log/xpni_log_destroy.c
			xpni/xpni_log/xpni_log_exportFile.c
			xpni/xpni_log/xpni_log_importFile.c
			xpni/xpni_log/xpni_log_init.c
			xpni/xpni_log/xpni_log_lseek.c
			xpni/xpni_log/xpni_log_open.c
			xpni/xpni_log/xpni_log_read.c
			xpni/xpni_log/xpni_log_register.c
			xpni/xpni_log/xpni_log_write.c
			xpni/xpni_log/xpni_log_opendir.c
			xpni/xpni_log/xpni_log_elog.c
    )
set (XPN2_OBJECTS	xpn2/xpn2_close.c
			xpn2/xpn2_destroy.c
			xpn2/xpn2_init.c
			xpn2/xpn2_open.c
			xpn2/xpn2_creat.c
			xpn2/xpn2_read.c
			xpn2/xpn2_write.c
			xpn2/xpn2_ioctl.c
			xpn2/xpn2_lseek.c
    )

set (LIBXPN_SOURCES ${BASE_OBJECTS} ${NFI_OBJECTS} ${XPN_OBJECTS} ${XPNI_OBJECTS} ${XPN2_OBJECTS})

set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

option (BUILD_SHARED_LIBS "Build shared library (Default: ON)." ON)
add_library(xpn ${LIBXPN_SOURCES})


#TODO: Cannot build both static and shared libraries compiling sources only once.

#add_library(xpn-static STATIC ${LIBXPN_SOURCES})
#add_library(xpn-static SHARED ${LIBXPN_SOURCES})
#target_link_libraries (xpn-static "${CMAKE_LINK_LIBRARY_FLAG}")

# Use:
# cmake -D BUILD_SHARED_LIBS=1 .
# http://www.cmake.org/Wiki/CMake_FAQ#Does_that_mean_I_have_to_build_all_my_library_objects_twice.2C_once_for_shared_and_once_for_static.3F.21_I_don.27t_like_that.21
#if (${BUILD_SHARED_LIBS})
#	add_library(xpn SHARED ${LIBXPN_SOURCES})
#	IF(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND CMAKE_COMPILER_IS_GNUCC)
#		#ADD_LIBRARY(xpn-static STATIC -fPIC)
#		TARGET_LINK_LIBRARIES(xpn -Wl,-whole-archive xpn-static -Wl,-no-whole-archive) # FIXME: Shared lib currently not working if using this!
#	ENDIF(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND CMAKE_COMPILER_IS_GNUCC)
#endif (${BUILD_SHARED_LIBS})

# http://www.cmake.org/Wiki/CMake_FAQ#How_do_I_make_my_shared_and_static_libraries_have_the_same_root_name.2C_but_different_suffixes.3F
# The library target "xpn" already has a default OUTPUT_NAME of "xpn", so we don't need to change it.
# The library target "xpn-static" has a default OUTPUT_NAME of "xpn-static", so change it.
#SET_TARGET_PROPERTIES(xpn-static PROPERTIES OUTPUT_NAME "xpn")
# Now the library target "xpn-static" will be named "xpn.lib" with MS tools.
# This conflicts with the "xpn.lib" import library corresponding to "xpn.dll",
# so we add a "lib" prefix (which is default on other platforms anyway):
#SET_TARGET_PROPERTIES(xpn-static PROPERTIES PREFIX "lib")

