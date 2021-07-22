#ifndef _NFI_XIO_ERR_H_
#define _NFI_XIO_ERR_H_


 #ifdef  __cplusplus
    extern "C" {
 #endif


enum nfi_xio_err{
	XIOERR_PARAM = 0,
	XIOERR_MEMORY = 1,
	XIOERR_URL = 2,
	XIOERR_MNTCONNECTION = 3,
	XIOERR_MOUNT = 4,
	XIOERR_NFSCONNECTION = 5,		
	XIOERR_GETATTR = 6,
	XIOERR_LOOKUP = 7,
	XIOERR_READ = 8,
	XIOERR_WRITE = 9,
	XIOERR_CREATE = 10,
	XIOERR_REMOVE = 11,
	XIOERR_MKDIR = 12,
	XIOERR_READDIR = 13,
	XIOERR_STATFS = 14,
	XIOERR_NOTDIR = 15,
};


void xio_err(int err);


 #ifdef  __cplusplus
     }
 #endif


#endif
