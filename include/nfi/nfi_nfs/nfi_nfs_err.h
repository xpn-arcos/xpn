#ifndef _NFI_NFS_ERR_H_
#define _NFI_NFS_ERR_H_

#include "nfs.h"


 #ifdef  __cplusplus
    extern "C" {
 #endif


enum nfi_nfs_err{
	NFSERR_PARAM = 0,
	NFSERR_MEMORY = 1,
	NFSERR_URL = 2,
	NFSERR_MNTCONNECTION = 3,
	NFSERR_MOUNT = 4,
	NFSERR_NFSCONNECTION = 5,		
	NFSERR_GETATTR = 6,
	NFSERR_LOOKUP = 7,
	NFSERR_READ = 8,
	NFSERR_WRITE = 9,
	NFSERR_CREATE = 10,
	NFSERR_REMOVE = 11,
	NFSERR_MKDIR = 12,
	NFSERR_READDIR = 13,
	NFSERR_STATFS = 14,
};


void nfs_err(int err);


 #ifdef  __cplusplus
     }
 #endif


#endif
