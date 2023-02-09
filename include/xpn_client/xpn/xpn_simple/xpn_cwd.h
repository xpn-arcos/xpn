#ifndef _XPN_CWD_H
#define _XPN_CWD_H


#include "xpn.h"
#include "xpn_err.h"
#include "xpn_policy_cwd.h"


 #ifdef  __cplusplus
    extern "C" {
 #endif

struct xpn_cwd{
	char xpn_cwd_path[MAX_PATH_LEN];	
};

extern struct xpn_cwd xpn_cwddir;

void xpn_init_cwd();
char* xpn_getcwd(char *path, size_t size);

int xpn_chdir(char *path);


 #ifdef  __cplusplus
     }
 #endif

#endif

