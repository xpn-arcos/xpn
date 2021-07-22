

 #ifndef DEBUG_TAGS_H
 #define DEBUG_TAGS_H

   /* ... Include / Inclusion ........................................... */

      #include "all_system.h"


 #ifdef  __cplusplus
    extern "C" {
 #endif


   /* ... Consts / Constantes ........................................... */

      #define      INFO __LINE__,__FILE__,getpid(),100
      #define   WARNING __LINE__,__FILE__,getpid(),101
      #define     ERROR __LINE__,__FILE__,getpid(),102

      #define  DEFCON_1 __LINE__,__FILE__,getpid(),110
      #define  DEFCON_2 __LINE__,__FILE__,getpid(),110
      #define  DEFCON_3 __LINE__,__FILE__,getpid(),111
      #define  DEFCON_4 __LINE__,__FILE__,getpid(),111
      #define  DEFCON_5 __LINE__,__FILE__,getpid(),112
      #define  DEFCON_6 __LINE__,__FILE__,getpid(),112


   /* ................................................................... */

 #ifdef  __cplusplus
    }
 #endif


 #endif

