#include "nfi_tcp_server.h"

extern int errno;

/* this interface must be changed */
void tcp_server_err(int err)
{
	switch(err)
	{
		case TCP_SERVERERR_PARAM:
			errno = -1;
			break;
		
		case TCP_SERVERERR_MEMORY:
			errno = -1;
			break;
			
		case TCP_SERVERERR_URL:
			errno = -1;
			break;

		case TCP_SERVERERR_MNTCONNECTION:
			errno = -1;
			break;

		case TCP_SERVERERR_MOUNT:
			errno = -1;
			break;

		case TCP_SERVERERR_NFSCONNECTION:
			errno = -1;
			break;
			
		case TCP_SERVERERR_GETATTR:
			errno = -1;
			break;
	}
};
