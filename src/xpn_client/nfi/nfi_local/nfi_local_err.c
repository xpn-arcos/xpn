
#include "nfi/nfi_local/local_lib.h"

extern int errno;

/* this interface must be changed */
void local_err ( int err )
{
	switch (err)
	{
		case LOCALERR_PARAM:
			errno = -1;
			break;
		
		case LOCALERR_MEMORY:
			errno = -1;
			break;
			
		case LOCALERR_URL:
			errno = -1;
			break;

		case LOCALERR_MNTCONNECTION:
			errno = -1;
			break;

		case LOCALERR_MOUNT:
			errno = -1;
			break;

		case LOCALERR_NFSCONNECTION:
			errno = -1;
			break;
			
		case LOCALERR_GETATTR:
			errno = -1;
			break;
	}
}

