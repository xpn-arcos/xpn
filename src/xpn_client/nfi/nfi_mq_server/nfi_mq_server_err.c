#include "nfi_mq_server.h"

extern int errno;

// TODO: this interface must be changed
void mq_server_err(int err)
{
	switch(err)
	{
		case MQ_SERVERERR_PARAM:
			errno = -1;
			break;
		
		case MQ_SERVERERR_MEMORY:
			errno = -1;
			break;
			
		case MQ_SERVERERR_URL:
			errno = -1;
			break;

		case MQ_SERVERERR_MNTCONNECTION:
			errno = -1;
			break;

		case MQ_SERVERERR_MOUNT:
			errno = -1;
			break;

		case MQ_SERVERERR_NFSCONNECTION:
			errno = -1;
			break;
			
		case MQ_SERVERERR_GETATTR:
			errno = -1;
			break;
	}
}

