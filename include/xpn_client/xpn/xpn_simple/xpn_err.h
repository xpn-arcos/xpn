#ifndef _XPN_ERR_H_
#define _XPN_ERR_H_


#include "xpn.h"
#include "nfi/nfi_lib.h"


 #ifdef  __cplusplus
    extern "C" {
 #endif

enum xpn_err_code{
        XPN_OK = 0,
        XPNERR_PARAM = 1,
        XPNERR_MEMORY = 2,
        XPNERR_INVALURL = 3,
        XPNERR_CONF = 4,
	XPNERR_INITSERV = 5, 
        XPNERR_GETATTR = 6,
        XPNERR_LOOKUP = 7,
        XPNERR_READ = 8,
        XPNERR_WRITE = 9,
        XPNERR_CREATE = 10,
        XPNERR_REMOVE = 11,
        XPNERR_MKDIR = 12,
        XPNERR_READDIR = 13,
        XPNERR_STATFS = 14,
	XPNERR_PATH_NOEXIST = 15,
	XPNERR_PART_NOEXIST = 16,
	XPNERR_NOMEMORY = 17,
};




extern int errno;
extern int xpn_errno;

int xpn_err(int err);


 #ifdef  __cplusplus
     }
 #endif

#endif

