#include "mpiServer_lib.h"

extern int errno;

/* this interface must be changed */
void mpiServer_err(int err)
{
	switch(err)
	{
		case MPISERVERERR_PARAM:
			errno = -1;
			break;
		
		case MPISERVERERR_MEMORY:
			errno = -1;
			break;
			
		case MPISERVERERR_URL:
			errno = -1;
			break;

		case MPISERVERERR_MNTCONNECTION:
			errno = -1;
			break;

		case MPISERVERERR_MOUNT:
			errno = -1;
			break;

		case MPISERVERERR_NFSCONNECTION:
			errno = -1;
			break;
			
		case MPISERVERERR_GETATTR:
			errno = -1;
			break;
	}
}

