
#ifndef _XPNI_DIR_H
#define _XPNI_DIR_H

 #ifdef  __cplusplus
    extern "C" {
 #endif


   /* ... Include / Inclusion ........................................... */

      #include "xpn.h"
      #include "xpni/common/xpni_fit.h"
      #include "xpni/common/xpni_fsit.h"
      #include "xpni/common/xpni_fmeta.h"


   /* ... Functions / Funciones ......................................... */

      int xpni_dir_open
      (
          char    *path
      ) ;

      int xpni_dir_close
      (
          int      dd
      ) ;

      int xpni_dir_changeFileSystemTag
      (
         int       dd,
         char     *fs_tag
      ) ;


  /* .................................................................... */


 #ifdef  __cplusplus
    }
 #endif

#endif

