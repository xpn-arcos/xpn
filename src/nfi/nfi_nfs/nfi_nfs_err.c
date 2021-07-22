#include "nfi/nfi_nfs/nfs_lib.h"

extern int errno;

/* this interface must be changed */
void nfs_err(int err){
	switch(err){
		case NFSERR_PARAM:
			errno = -1;
			break;
		
		case NFSERR_MEMORY:
			errno = -1;
			break;
			
		case NFSERR_URL:
			errno = -1;
			break;

		case NFSERR_MNTCONNECTION:
			errno = -1;
			break;

		case NFSERR_MOUNT:
			errno = -1;
			break;

		case NFSERR_NFSCONNECTION:
			errno = -1;
			break;
			
		case NFSERR_GETATTR:
			errno = -1;
			break;
	}
};
