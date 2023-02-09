#ifndef _XPN_OPENDIR_H
#define _XPN_OPENDIR_H

#include "xpn.h"
#include "xpn_open.h"


 #ifdef  __cplusplus
    extern "C" {
 #endif


struct __dirstream{
      int fd;
};


DIR *           xpn_simple_opendir(const char *path) ;
int             xpn_simple_closedir(DIR *dirp) ;
struct dirent * xpn_simple_readdir(DIR *dirp) ;
void            xpn_simple_rewinddir(DIR *dirp) ;


 #ifdef  __cplusplus
     }
 #endif


#endif

