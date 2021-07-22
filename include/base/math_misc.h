

 #ifndef _MATH_MISC_H
 #define _MATH_MISC_H

   /* ... Include / Inclusion ........................................... */

      #include "all_system.h"


 #ifdef  __cplusplus
    extern "C" {
 #endif


   /* ... Functions / Funciones ......................................... */

      int MATH_MISC_hash
      (
          char *file,
          int  nServ
      ) ;

      int MATH_MISC_locateInRAID5withInternalParity 
      (
          int i,    /* block index */
          int m,    /* how many servers */
          int *SP,  /* Server for Parity */
          int *IP,  /* Index for Parity */
          int *SD,  /* Server for Data */
          int *ID   /* Index for Data */
      ) ;

      int MATH_MISC_locateInRAID5withExternalParity 
      (
          int i,    /* block index */
          int m,    /* how many servers */
          int *SP,  /* Server for Parity */
          int *IP,  /* Index for Parity */
          int *SD,  /* Server for Data */
          int *ID   /* Index for Data */
      ) ;

      int MATH_MISC_Xor
      (
         char *block_result,
         char *block_1,
         char *block_2,
         int   block_size
      ) ;

      int MATH_MISC_Xor3
      (
          char *block_result,
          char *block_1,
          char *block_2,
          char *block_3,
          int   block_size
      ) ;

      int MATH_MISC_XorN
      (
          char  *block_result,
          char **blocks,
          int    nblocks,
          int    block_size
      ) ;


  /* .................................................................... */


 #ifdef  __cplusplus
    }
 #endif

 #endif /* _MATH_MISC_H */

