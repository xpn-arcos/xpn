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
// END OF ENABLE_MODULE BLOCK. Do not remove this line. //

#endif
