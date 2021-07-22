#ifndef _NFI_HTTP_ERR_H_
#define _NFI_HTTP_ERR_H_


 #ifdef  __cplusplus
    extern "C" {
 #endif


enum nfi_http_err{
	HTTPERR_PARAM = 0,
	HTTPERR_MEMORY = 1,
	HTTPERR_URL = 2,
	HTTPERR_MNTCONNECTION = 3,
	HTTPERR_MOUNT = 4,
	HTTPERR_NFSCONNECTION = 5,		
	HTTPERR_GETATTR = 6,
	HTTPERR_LOOKUP = 7,
	HTTPERR_READ = 8,
	HTTPERR_WRITE = 9,
	HTTPERR_CREATE = 10,
	HTTPERR_REMOVE = 11,
	HTTPERR_MKDIR = 12,
	HTTPERR_READDIR = 13,
	HTTPERR_STATFS = 14,
	HTTPERR_NOTDIR = 15,
};


void http_err(int err);


 #ifdef  __cplusplus
     }
 #endif


#endif
