#include "nfi_mpi_server.h"

extern int errno;

/* this interface must be changed */
void mpi_server_err(int err)
{
	switch(err)
	{
		case MPI_SERVERERR_PARAM:
			errno = -1;
			break;
		
		case MPI_SERVERERR_MEMORY:
			errno = -1;
			break;
			
		case MPI_SERVERERR_URL:
			errno = -1;
			break;

		case MPI_SERVERERR_MNTCONNECTION:
			errno = -1;
			break;

		case MPI_SERVERERR_MOUNT:
			errno = -1;
			break;

		case MPI_SERVERERR_NFSCONNECTION:
			errno = -1;
			break;
			
		case MPI_SERVERERR_GETATTR:
			errno = -1;
			break;
	}
}

