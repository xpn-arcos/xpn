

   /* ... Include / Inclusion ........................................... */

      #include "xpni/xpni_null/xpni_null.h"


   /* ... Functions / Funciones ......................................... */

      int xpni_null_init ()
      {

        /* debugging */
	#if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_null_init(); \n",
                   __FILE__,__LINE__);
	#endif

	return xpni_lowfsi_init();

      }


   /* ................................................................... */

