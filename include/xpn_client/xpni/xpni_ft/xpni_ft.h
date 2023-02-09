
#ifndef _XPNI_FT_H
#define _XPNI_FT_H

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
      #include "base/math_misc.h"


   /* ... Functions / Funciones ......................................... */

      int     xpni_ft_init    ( ) ;
      int     xpni_ft_destroy ( ) ;

      int     xpni_ft_register   ( xpni_fsi_t *fsi ) ;
      int     xpni_ft_unregister ( xpni_fsi_t *fsi ) ;

      int     xpni_ft_importFile ( int fd ) ;
      int     xpni_ft_exportFile ( int fd ) ;

      int     xpni_ft_open    ( int fd ) ;
      int     xpni_ft_creat   ( int fd ) ;
      int     xpni_ft_close   ( int fd ) ;

      ssize_t xpni_ft_sread   ( int fd, void *buffer, off_t offset, size_t size ) ;
      ssize_t xpni_ft_swrite  ( int fd, void *buffer, off_t offset, size_t size ) ;

      off_t   xpni_ft_lseek   ( int fd, off_t offset, int flag ) ;
      
      DIR    *xpni_ft_opendir ( int dd ) ;
      
      /*#NewInterfaceFunction.AsFunction3#*/


  /* .................................................................... */


 #ifdef  __cplusplus
    }
 #endif

#endif

