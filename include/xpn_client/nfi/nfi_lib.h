#ifndef _NFI_LIB_H_
#define _NFI_LIB_H_

#include "nfi.h"
#include "nfi_worker.h"
#include "base/string_misc.h"

/* protocols */
#include "nfi_local.h"

// BEGIN OF ENABLE_MODULE BLOCK. Do not remove this line. //
// BEGIN OF ENABLE_NFS BLOCK. Do not remove this line. //
#ifdef ENABLE_NFS
        #include "nfi_nfs.h"
#endif
// END OF ENABLE_NFS BLOCK. Do not remove this line. //
// BEGIN OF ENABLE_NFS3 BLOCK. Do not remove this line. //
#ifdef ENABLE_NFS3
        #include "nfi_nfs3.h"
#endif
// END OF ENABLE_NFS3 BLOCK. Do not remove this line. //
// BEGIN OF ENABLE_TCP_SERVER BLOCK. Do not remove this line. //
#ifdef ENABLE_TCP_SERVER
        #include "nfi_tcp_server.h"
#endif
// END OF ENABLE_TCP_SERVER BLOCK. Do not remove this line. //
// BEGIN OF ENABLE_MPI_SERVER BLOCK. Do not remove this line. //
#ifdef ENABLE_MPI_SERVER
        #include "nfi_mpi_server.h"
#endif
// END OF ENABLE_MPI_SERVER BLOCK. Do not remove this line. //
// END OF ENABLE_MODULE BLOCK. Do not remove this line. //

#endif
