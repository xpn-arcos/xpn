

   /* ... Include / Inclusion ........................................... */

      #include "xpni/xpni_null/xpni_null.h"


   /* ... Functions / Funciones ......................................... */

      off_t  xpni_null_lseek ( int fd, off_t offset, int flag )
      {

        /* debugging */
	#if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_null_lseek(%d); \n",
                   __FILE__,__LINE__,fd);
	#endif

        /* lseek file: data only */
	return xpni_lowfsi_lseek(xpni_fit_get_XPN_DATA_FD(fd),offset,flag);

      }


   /* ................................................................... */

