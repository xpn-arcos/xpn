#ifndef _NFI_GRIDFTP_ERR_H_
#define _NFI_GRIDFTP_ERR_H_


 #ifdef  __cplusplus
    extern "C" {
 #endif


enum nfi_gridftp_err{
	GRIDFTPERR_PARAM = 0,
	GRIDFTPERR_MEMORY = 1,
	GRIDFTPERR_URL = 2,
	GRIDFTPERR_MNTCONNECTION = 3,
	GRIDFTPERR_MOUNT = 4,
	GRIDFTPERR_NFSCONNECTION = 5,		
	GRIDFTPERR_GETATTR = 6,
	GRIDFTPERR_LOOKUP = 7,
	GRIDFTPERR_READ = 8,
	GRIDFTPERR_WRITE = 9,
	GRIDFTPERR_CREATE = 10,
	GRIDFTPERR_REMOVE = 11,
	GRIDFTPERR_MKDIR = 12,
	GRIDFTPERR_READDIR = 13,
	GRIDFTPERR_STATFS = 14,
	GRIDFTPERR_NOTDIR = 15,
};


void gridftp_err(int err);


 #ifdef  __cplusplus
     }
 #endif


#endif
