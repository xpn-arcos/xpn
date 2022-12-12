
#ifndef _XPNI_FILE_H
#define _XPNI_FILE_H

 #ifdef  __cplusplus
    extern "C" {
 #endif


   /* ... Include / Inclusion ........................................... */

      #include "xpn.h"
      #include "xpni/common/xpni_fit.h"
      #include "xpni/common/xpni_fsit.h"
      #include "xpni/common/xpni_fmeta.h"


   /* ... Functions / Funciones ......................................... */

      int xpni_file_initByName
      (
          fmeta_t *fmeta,
          char    *fname
      ) ;

      char * xpni_file_namedByMeta
      (
          fmeta_t *fmeta
      ) ;

      int xpni_file_open
      (
          char    *path,
          int      flag,
          int      mode
      ) ;

      int xpni_file_close
      (
          int      fd
      ) ;

      int xpni_file_changeFileSystemTag
      (
         int       fd,
         char     *fs_tag
      ) ;


  /* .................................................................... */


 #ifdef  __cplusplus
    }
 #endif

#endif

