
#include "nfi_nfs3.h"

extern int errno;

/* this interface must be changed */
void nfs3_err(int err)
{
	switch(err)
	{
	    /*
		case NFSERR_PARAM:
			errno = -1;
			break;
		
		case NFSERR_MEMORY:
			errno = -1;
			break;
			
		case NFSERR_URL:
			errno = -1;
			break;

		case NFSERR_MNTCONNECTION:
			errno = -1;
			break;

		case NFSERR_MOUNT:
			errno = -1;
			break;

		case NFSERR_NFSCONNECTION:
			errno = -1;
			break;
			
		case NFSERR_GETATTR:
			errno = -1;
			break;

**/			
		case NFS3_OK:
			errno = 0;
			break;
	
		case NFS3ERR_NOENT:
			errno = ENOENT;
			break;
		case NFS3ERR_ACCES:
			errno = EACCES;
			break;
		case NFS3ERR_NODEV:
			errno = ENODEV;
			break;
		case NFS3ERR_NOTDIR:
			errno = ENOTDIR;
			break;
		case NFS3ERR_EXIST:
			errno = EACCES;
			break;
		case NFS3ERR_ISDIR:
			errno = EISDIR;
			break;
		case NFS3ERR_NAMETOOLONG:
			errno = E2BIG;
			break;

		case NFS3ERR_FBIG:
			errno = EFBIG;
			break;

		case NFS3ERR_PERM:
		case NFS3ERR_IO:
		case NFS3ERR_NXIO:
		case NFS3ERR_XDEV:
		case NFS3ERR_INVAL:
		case NFS3ERR_NOSPC:
		case NFS3ERR_ROFS:
		case NFS3ERR_MLINK:
		case NFS3ERR_NOTEMPTY:
		case NFS3ERR_DQUOT:
		case NFS3ERR_STALE:
		case NFS3ERR_REMOTE:
		case NFS3ERR_BADHANDLE:
		case NFS3ERR_NOT_SYNC:
		case NFS3ERR_BAD_COOKIE:
		case NFS3ERR_NOTSUPP:
		case NFS3ERR_TOOSMALL:
		case NFS3ERR_SERVERFAULT:
		case NFS3ERR_BADTYPE:
		case NFS3ERR_JUKEBOX:
		case NFS3ERR_FPRINTNOTFOUND:
		case NFS3ERR_ABORTED:
			errno = ENOENT;
			break;

		default:
			 errno = 0;
	}
};
