
#ifndef _DTABLE_H
#define _DTABLE_H

   /* ... Include / Inclusion ........................................... */

      #include "darray.h"


 #ifdef  __cplusplus
    extern "C" {
 #endif


   /* ... Types / Tipos ................................................. */

      typedef
      struct
      {

         void       *dt_info ;
	 int         next_free ;
	 int         myself ;

      } dtelto_t ;

      typedef
      struct
      {

         t_pointerDArray  dtable ;
         long             neltos ;
         long             free_list ;

      } dtable_t ;


   /* ... Constants / Constantes ........................................ */

      #define c_DTABLE_T    sizeof(dtable_t)
      #define c_DTELTO_T    sizeof(dtelto_t)

      #define c_DTABLE_T_NULL  {NULL,0,-1}


   /* ... Functions / Funciones ......................................... */

      int    dtable_insert ( dtable_t *dt, void *dt_info ) ;
      int    dtable_delete ( dtable_t *dt, int   fd ) ;
      void  *dtable_get    ( dtable_t *dt, int   fd ) ;


   /* ................................................................... */


 #ifdef  __cplusplus
    }
 #endif

#endif

