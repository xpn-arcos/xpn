
#ifndef _XPNI_LOG_ELOG_H
#define _XPNI_LOG_ELOG_H

 #ifdef  __cplusplus
    extern "C" {
 #endif


   /* ... Include / Inclusion ........................................... */

      #include "xpn.h"
      #include "expand.h"


   /* ... Functions / Funciones ......................................... */

      int xpni_log_elog 
	  ( 
             /*IN*/  struct timeval *t1,
             /*IN*/  struct timeval *t2,
             /*IN*/  char           *action,
             /*IN*/  int             object,
             /*IN*/  int             offset,
             /*IN*/  int             size
	  ) ;

      int xpni_log_vprintf
	  ( 
	     /*IN*/  char           *ename, 
             ...
	  ) ;


  /* .................................................................... */


 #ifdef  __cplusplus
    }
 #endif

#endif

