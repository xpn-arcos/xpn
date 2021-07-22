#include "nfi/nfi_gsoap/gsoap_lib.h"
extern int errno;

/* this interface must be changed */
void gsoap_err(int err){
	switch(err){
		case GSOAPERR_PARAM:
			errno = -1;
			break;
		
		case GSOAPERR_MEMORY:
			errno = -1;
			break;
			
		case GSOAPERR_URL:
			errno = -1;
			break;

		case GSOAPERR_MNTCONNECTION:
			errno = -1;
			break;

		case GSOAPERR_MOUNT:
			errno = -1;
			break;

		case GSOAPERR_NFSCONNECTION:
			errno = -1;
			break;
			
		case GSOAPERR_GETATTR:
			errno = -1;
			break;
	}
};
