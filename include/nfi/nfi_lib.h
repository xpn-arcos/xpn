#ifndef _NFI_LIB_H_
#define _NFI_LIB_H_

#include "nfi.h"
#include "nfi_worker.h"
#include "base/string_misc.h"

/* protocols */
#include "nfi_local/local_lib.h"
// BEGIN OF ENABLE_MODULE BLOCK. Do not remove this line. //
// BEGIN OF ENABLE_NFS BLOCK. Do not remove this line. //
#ifdef ENABLE_NFS
	#include "nfi_nfs/nfs_lib.h"
#endif
// END OF ENABLE_NFS BLOCK. Do not remove this line. //
// BEGIN OF ENABLE_NFS3 BLOCK. Do not remove this line. //
#ifdef ENABLE_NFS3
	#include "nfi_nfs3/nfs3_lib.h"
#endif
// END OF ENABLE_NFS3 BLOCK. Do not remove this line. //
// BEGIN OF ENABLE_FTP BLOCK. Do not remove this line. //
#ifdef ENABLE_FTP
	#include "nfi_ftp/ftp_lib.h"
#endif
// END OF ENABLE_FTP BLOCK. Do not remove this line. //
// BEGIN OF ENABLE_XIO BLOCK. Do not remove this line. //
#ifdef ENABLE_XIO
	#include "nfi_xio/xio_lib.h"
#endif
// END OF ENABLE_XIO BLOCK. Do not remove this line. //
// BEGIN OF ENABLE_GSOAP BLOCK. Do not remove this line. //
#ifdef ENABLE_GSOAP
	#include "nfi_gsoap/gsoap_lib.h"
#endif
// END OF ENABLE_GSOAP BLOCK. Do not remove this line. //
// BEGIN OF ENABLE_GRIDFTP BLOCK. Do not remove this line. //
#ifdef ENABLE_GRIDFTP
	#include "nfi_gridftp/gridftp_lib.h"
#endif
// END OF ENABLE_GRIDFTP BLOCK. Do not remove this line. //
// BEGIN OF ENABLE_TCPSERVER BLOCK. Do not remove this line. //
#ifdef ENABLE_TCPSERVER
	#include "nfi_tcpServer/tcpServer_lib.h"
#endif
// END OF ENABLE_TCPSERVER BLOCK. Do not remove this line. //
// BEGIN OF ENABLE_MPISERVER BLOCK. Do not remove this line. //
#ifdef ENABLE_MPISERVER
	#include "nfi_mpiServer/mpiServer_lib.h"
#endif
// END OF ENABLE_MPISERVER BLOCK. Do not remove this line. //
// BEGIN OF ENABLE_IOPROXY_RPC BLOCK. Do not remove this line. //
#ifdef ENABLE_IOPROXY_RPC
	#include "nfi_ioproxy-rpc/ioproxy-rpc_lib.h"
#endif
// END OF ENABLE_IOPROXY_RPC BLOCK. Do not remove this line. //
// BEGIN OF ENABLE_HTTP BLOCK. Do not remove this line. //
#ifdef ENABLE_HTTP
	#include "nfi_http/http_lib.h"
#endif
// END OF ENABLE_HTTP BLOCK. Do not remove this line. //
// END OF ENABLE_MODULE BLOCK. Do not remove this line. //

#endif
