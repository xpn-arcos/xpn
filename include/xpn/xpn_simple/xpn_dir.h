#ifndef _XPN_DIR_H
#define _XPN_DIR_H


#include "xpn.h"
#include "xpn_err.h"
#include "xpn_policy_init.h"
#include "xpn_policy_open.h"


 #ifdef  __cplusplus
    extern "C" {
 #endif

int xpn_mkdir(const char *path, mode_t perm) ;
int xpn_rmdir(const char *path) ;


 #ifdef  __cplusplus
     }
 #endif

#endif

