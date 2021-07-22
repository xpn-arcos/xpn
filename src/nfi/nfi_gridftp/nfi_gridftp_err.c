#include "nfi_gridftp/gridftp_lib.h"
extern int errno;

/* this interface must be changed */
void gridftp_err(int err){
	switch(err){
		case GRIDFTPERR_PARAM:
			errno = -1;
			break;
		
		case GRIDFTPERR_MEMORY:
			errno = -1;
			break;
			
		case GRIDFTPERR_URL:
			errno = -1;
			break;

		case GRIDFTPERR_MNTCONNECTION:
			errno = -1;
			break;

		case GRIDFTPERR_MOUNT:
			errno = -1;
			break;

		case GRIDFTPERR_NFSCONNECTION:
			errno = -1;
			break;
			
		case GRIDFTPERR_GETATTR:
			errno = -1;
			break;
	}
};
