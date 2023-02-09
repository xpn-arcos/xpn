

   /* ... Include / Inclusion ........................................... */

      #include "xpni/xpni_null/xpni_null.h"


   /* ... Functions / Funciones ......................................... */

      int xpni_null_close ( int fd )
      {

        /* debugging */
	#if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_null_close(%d); \n",
                   __FILE__,__LINE__,fd);
	#endif

	return xpni_lowfsi_close(xpni_fit_get_XPN_DATA_FD(fd));

      }


   /* ................................................................... */

