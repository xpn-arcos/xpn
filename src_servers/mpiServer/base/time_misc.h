

 #ifndef _TIME_MISC_H
 #define _TIME_MISC_H

   /* ... Include / Inclusion ........................................... */

      #include "all_system.h"


 #ifdef  __cplusplus
    extern "C" {
 #endif


   /* ... Ctes / Constantes ............................................. */

      #define USECPSEC		1000000


   /* ... Functions / Funciones ......................................... */

      void TIME_MISC_Timer
      (
           struct timeval * t
      );

      void TIME_MISC_DiffTime
      (
           struct timeval * to,
           struct timeval * tn,
           struct timeval * dif
      );

      void TIME_MISC_AddTime
      (
           struct timeval * to,
           struct timeval * tn,
           struct timeval * sum
      );

      float TIME_MISC_TimevaltoFloat
      (
           struct timeval* timet
      );

      float TIME_MISC_TimevaltoMicro
      (
           struct timeval* timet
      );


   /* ................................................................... */


 #ifdef  __cplusplus
    }
 #endif

 #endif /* _TIME_MISC_H */

