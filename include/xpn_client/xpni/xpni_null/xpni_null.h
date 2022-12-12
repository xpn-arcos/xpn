
#ifndef _XPNI_NULL_H
#define _XPNI_NULL_H

 #ifdef  __cplusplus
    extern "C" {
 #endif


   /* ... Include / Inclusion ........................................... */

      #include "xpn.h"
      #include "expand.h"
      #include "xpni/common/xpni_fit.h"
      #include "xpni/common/xpni_fsit.h"
      #include "xpni/common/xpni_file.h"
      #include "xpni/common/xpni_lowfsi.h"


   /* ... Functions / Funciones ......................................... */

      int     xpni_null_init    ( ) ;
      int     xpni_null_destroy ( ) ;

      int     xpni_null_register   ( xpni_fsi_t *fsi ) ;
      int     xpni_null_unregister ( xpni_fsi_t *fsi ) ;

      int     xpni_null_importFile ( int fd ) ;
      int     xpni_null_exportFile ( int fd ) ;

      int     xpni_null_open    ( int fd ) ;
      int     xpni_null_creat   ( int fd ) ;
      int     xpni_null_close   ( int fd ) ;

      ssize_t xpni_null_sread   ( int fd, void *buffer, off_t offset, size_t size ) ;
      ssize_t xpni_null_swrite  ( int fd, void *buffer, off_t offset, size_t size ) ;

      off_t   xpni_null_lseek   ( int fd, off_t offset, int flag ) ;
      
      DIR    *xpni_null_opendir	( int dd ) ;

      /*#NewInterfaceFunction.AsFunction3#*/


  /* .................................................................... */


 #ifdef  __cplusplus
    }
 #endif

#endif

