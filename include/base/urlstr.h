#ifndef _URLSTR_H
#define _URLSTR_H


   /* ... Include / Inclusion ........................................... */

      #include "all_system.h"
      #include "string_misc.h"


 #ifdef  __cplusplus
    extern "C" {
 #endif


   /* ... Functions / Funciones ......................................... */

      int8_t    URLSTR_ParseURL 
      (
         /*IN */ char  *urlstr,
          /*OUT*/ char **protocol,
          /*OUT*/ char **user,
          /*OUT*/ char **machine,
          /*OUT*/ int   *port,
          /*OUT*/ char **file,
          /*OUT*/ char **relative,
          /*OUT*/ char **params
      ) ;
      /* - Fill 'url' with information from 'str' string. */
      /* - Rellena 'url' con la informacion de 'str'.  */


  /* .................................................................... */


 #ifdef  __cplusplus
    }
 #endif

#endif

