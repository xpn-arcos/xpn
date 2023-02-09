
#ifndef _XPNI_FT_READ_NOFAIL_H
#define _XPNI_FT_READ_NOFAIL_H

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

      ssize_t xpni_ft_sread_r5i
      (
         int fd,
         void *buffer,
         off_t offset,
         size_t size
      ) ;

      ssize_t xpni_ft_sread_r5o
      (
         int fd,
         void *buffer,
         off_t offset,
         size_t size
      ) ;

      ssize_t xpni_ft_sread_nofail_r5i
      (
         int fd,
         void *buffer,
         off_t offset,
         size_t size
      ) ;

      ssize_t xpni_ft_sread_nofail_r5o
      (
         int fd,
         void *buffer,
         off_t offset,
         size_t size
      ) ;

      ssize_t xpni_ft_sread_fail_r5i
      (
         int fd,
         void *buffer,
         off_t offset,
         size_t size
      ) ;

      ssize_t xpni_ft_sread_fail_r5o
      (
         int fd,
         void *buffer,
         off_t offset,
         size_t size
      ) ;


  /* .................................................................... */


 #ifdef  __cplusplus
    }
 #endif

#endif

