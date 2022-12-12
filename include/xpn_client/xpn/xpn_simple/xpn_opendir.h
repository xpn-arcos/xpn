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


DIR* xpn_opendir(const char *path);
int xpn_closedir(DIR *dirp);
struct dirent* xpn_readdir(DIR *dirp);
void xpn_rewinddir(DIR *dirp);


 #ifdef  __cplusplus
     }
 #endif


#endif

