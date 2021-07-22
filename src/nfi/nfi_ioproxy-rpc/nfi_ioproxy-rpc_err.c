#include "nfi_ioproxy-rpc/ioproxy-rpc_lib.h"
extern int errno;

/* this interface must be changed */
void ioproxy-rpc_err(int err){
	switch(err){
		case IOPROXY_RPCERR_PARAM:
			errno = -1;
			break;
		
		case IOPROXY_RPCERR_MEMORY:
			errno = -1;
			break;
			
		case IOPROXY_RPCERR_URL:
			errno = -1;
			break;

		case IOPROXY_RPCERR_MNTCONNECTION:
			errno = -1;
			break;

		case IOPROXY_RPCERR_MOUNT:
			errno = -1;
			break;

		case IOPROXY_RPCERR_NFSCONNECTION:
			errno = -1;
			break;
			
		case IOPROXY_RPCERR_GETATTR:
			errno = -1;
			break;
	}
};
