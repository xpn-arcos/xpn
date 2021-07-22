#include "nfi_xio/xio_lib.h"
extern int errno;

/* this interface must be changed */
void xio_err(int err){
	switch(err){
		case XIOERR_PARAM:
			errno = -1;
			break;
		
		case XIOERR_MEMORY:
			errno = -1;
			break;
			
		case XIOERR_URL:
			errno = -1;
			break;

		case XIOERR_MNTCONNECTION:
			errno = -1;
			break;

		case XIOERR_MOUNT:
			errno = -1;
			break;

		case XIOERR_NFSCONNECTION:
			errno = -1;
			break;
			
		case XIOERR_GETATTR:
			errno = -1;
			break;
	}
};
