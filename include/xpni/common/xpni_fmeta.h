
#ifndef _XPNI_META_H
#define _XPNI_META_H

 #ifdef  __cplusplus
    extern "C" {
 #endif


   /* ... Include / Inclusion ........................................... */

      #include "xpn.h"
      #include "base/string_misc.h"
      #include "xpni/common/xpni_fstag.h"


   /* ... Types / Tipos ................................................. */

      typedef
      struct
      {
         int       block_size;
         int       servers_count;
         int       start_server;
         char     *filesystem_tag;
         char     *partition_name;
         int       nerrors;
         int      *servers;
         int      *servers_status;
         int      *Ld;
         int      *Lr;
         int      *Ld2r;
         int      *Lr2d;

      } fmeta_t ;


   /* ... Functions / Funciones ......................................... */

      int xpni_fmeta_initByValues
      (
          fmeta_t  *fmeta,
          int       block_size,
          int       servers_count,
          int       start_server,
          char     *filesystem_tag,
          char     *partition_name,
          int       nerrors,
          int      *servers,
          int      *servers_status,
          int      *Ld,
          int      *Lr,
          int      *Ld2r,
          int      *Lr2d
      ) ;

      int xpni_fmeta_finalize   
      ( 
          fmeta_t *fmeta 
      ) ;

      int xpni_fmeta_changeFileSystemTag 
      ( 
          fmeta_t *fmeta, 
          char    *new_fs_tag 
      ) ;


  /* .................................................................... */


 #ifdef  __cplusplus
    }
 #endif

#endif

