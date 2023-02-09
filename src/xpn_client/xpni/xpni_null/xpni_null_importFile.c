

   /* ... Include / Inclusion ........................................... */

      #include "xpni/xpni_null/xpni_null.h"


   /* ... Functions / Funciones ......................................... */

      int xpni_null_importFile ( __attribute__((__unused__)) int fd )  
      {

        /* debugging */
	#if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_null_importFile(%d); \n",
                   __FILE__,__LINE__,fd);
	#endif

	/* return ok, nothing to do by now... */
	return (1) ;

      }


   /* ................................................................... */

