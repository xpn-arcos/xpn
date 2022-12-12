
#ifndef _XPNI_LOG_H
#define _XPNI_LOG_H

 #ifdef  __cplusplus
    extern "C" {
 #endif


   /* ... Include / Inclusion ........................................... */

      #include "xpn.h"
      #include "expand.h"
      #include "base/time_misc.h"
      #include "xpni/common/xpni_fit.h"
      #include "xpni/common/xpni_fsit.h"
      #include "xpni/common/xpni_file.h"
      #include "xpni/common/xpni_lowfsi.h"


   /* ... Functions / Funciones ......................................... */

      int     xpni_log_init    ( ) ;
      int     xpni_log_destroy ( ) ;

      int     xpni_log_register   ( xpni_fsi_t *fsi ) ;
      int     xpni_log_unregister ( xpni_fsi_t *fsi ) ;

      int     xpni_log_importFile ( int fd ) ;
      int     xpni_log_exportFile ( int fd ) ;

      int     xpni_log_open    ( int fd ) ;
      int     xpni_log_creat   ( int fd ) ;
      int     xpni_log_close   ( int fd ) ;

      ssize_t xpni_log_sread   ( int fd, void *buffer, off_t offset, size_t size ) ;
      ssize_t xpni_log_swrite  ( int fd, void *buffer, off_t offset, size_t size ) ;

      off_t   xpni_log_lseek   ( int fd, off_t offset, int flag ) ;
      
      DIR    *xpni_log_opendir	( int dd ) ;

      /*#NewInterfaceFunction.AsFunction3#*/


  /* .................................................................... */


 #ifdef  __cplusplus
    }
 #endif

#endif

