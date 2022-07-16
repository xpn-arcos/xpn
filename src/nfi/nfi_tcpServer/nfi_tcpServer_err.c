#include "tcpServer_lib.h"

extern int errno;

/* this interface must be changed */
void tcpServer_err(int err){
	switch(err){
		case TCPSERVERERR_PARAM:
			errno = -1;
			break;
		
		case TCPSERVERERR_MEMORY:
			errno = -1;
			break;
			
		case TCPSERVERERR_URL:
			errno = -1;
			break;

		case TCPSERVERERR_MNTCONNECTION:
			errno = -1;
			break;

		case TCPSERVERERR_MOUNT:
			errno = -1;
			break;

		case TCPSERVERERR_NFSCONNECTION:
			errno = -1;
			break;
			
		case TCPSERVERERR_GETATTR:
			errno = -1;
			break;
	}
};
