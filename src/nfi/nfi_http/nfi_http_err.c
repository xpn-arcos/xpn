#include "nfi/nfi_http/http_lib.h"
extern int errno;

/* this interface must be changed */
void http_err(int err){
	switch(err){
		case HTTPERR_PARAM:
			errno = -1;
			break;
		
		case HTTPERR_MEMORY:
			errno = -1;
			break;
			
		case HTTPERR_URL:
			errno = -1;
			break;

		case HTTPERR_MNTCONNECTION:
			errno = -1;
			break;

		case HTTPERR_MOUNT:
			errno = -1;
			break;

		case HTTPERR_NFSCONNECTION:
			errno = -1;
			break;
			
		case HTTPERR_GETATTR:
			errno = -1;
			break;
	}
};
