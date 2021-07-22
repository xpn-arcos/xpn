#ifndef _NFI_GSOAP_ERR_H_
#define _NFI_GSOAP_ERR_H_


 #ifdef  __cplusplus
    extern "C" {
 #endif


enum nfi_gsoap_err{
	GSOAPERR_PARAM = 0,
	GSOAPERR_MEMORY = 1,
	GSOAPERR_URL = 2,
	GSOAPERR_MNTCONNECTION = 3,
	GSOAPERR_MOUNT = 4,
	GSOAPERR_NFSCONNECTION = 5,		
	GSOAPERR_GETATTR = 6,
	GSOAPERR_LOOKUP = 7,
	GSOAPERR_READ = 8,
	GSOAPERR_WRITE = 9,
	GSOAPERR_CREATE = 10,
	GSOAPERR_REMOVE = 11,
	GSOAPERR_MKDIR = 12,
	GSOAPERR_READDIR = 13,
	GSOAPERR_STATFS = 14,
	GSOAPERR_NOTDIR = 15,
};


void gsoap_err(int err);


 #ifdef  __cplusplus
     }
 #endif


#endif
