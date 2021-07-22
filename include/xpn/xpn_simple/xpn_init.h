#ifndef _XPN_INIT_H
#define _XPN_INIT_H

#include "all_system.h"
#include "xpn_policy_init.h"
#include "xpn_cwd.h"
#include "xpn_file.h"


 #ifdef  __cplusplus
    extern "C" {
 #endif


#define XPN_MAX_PART 128

extern struct xpn_partition xpn_parttable[XPN_MAX_PART];

int xpn_init();

int xpn_init_partition(char *partname);

int xpn_destroy();


 #ifdef  __cplusplus
     }
 #endif

#endif

