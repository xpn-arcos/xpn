
#ifndef _XPN2_H
#define _XPN2_H


   /* ... Include / Inclusion ........................................... */

      #include "base/trace_msg.h"
      #include "base/debug_msg.h"

      #include "xpni/xpni.h"


 #ifdef  __cplusplus
    extern "C" {
 #endif

   /* ... Const / Constantes ............................................ */

      #define XPN2_IOCTL_SETXPN2INTERFACE     0x20000010

      #define XPN2_IOCTL_FIRST                XPN2_IOCTL_SETXPN2INTERFACE
      #define XPN2_IOCTL_LAST                 XPN2_IOCTL_SETXPN2INTERFACE


   /* ... Functions / Funciones ......................................... */

      int     xpn2_init    ( ) ;
      int     xpn2_destroy ( ) ;

      int     xpn2_open    ( char *path, int flag, ... ) ;
      int     xpn2_creat   ( char *path, mode_t mode ) ;
      int     xpn2_close   ( int fd ) ;

      ssize_t xpn2_swrite  ( int fd, void *buffer, off_t offset, size_t size ) ;
      ssize_t xpn2_sread   ( int fd, void *buffer, off_t offset, size_t size ) ;

      int     xpn2_ioctl   ( int fd, int request, ... ) ;

      off_t   xpn2_lseek   ( int fd, off_t offset, int flag ) ;


  /* .................................................................... */


 #ifdef  __cplusplus
    }
 #endif

#endif

