#include "myServer_lib.h"

extern int errno;

/* this interface must be changed */
void myServer_err(int err){
	switch(err){
		case MYSERVERERR_PARAM:
			errno = -1;
			break;
		
		case MYSERVERERR_MEMORY:
			errno = -1;
			break;
			
		case MYSERVERERR_URL:
			errno = -1;
			break;

		case MYSERVERERR_MNTCONNECTION:
			errno = -1;
			break;

		case MYSERVERERR_MOUNT:
			errno = -1;
			break;

		case MYSERVERERR_NFSCONNECTION:
			errno = -1;
			break;
			
		case MYSERVERERR_GETATTR:
			errno = -1;
			break;
	}
};
