

   /* ... Include / Inclusion ........................................... */

      #include "xpni/xpni_null/xpni_null.h"


   /* ... Functions / Funciones ......................................... */

      ssize_t xpni_null_sread
      (
         int fd, 
         void *buffer, 
         off_t offset, 
         size_t size
      )
      {

        /* debugging */
	#if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_null_sread(%d,%p,%lu,%lu); \n",
                   __FILE__,__LINE__,fd,buffer,(unsigned long)offset,(unsigned long)size);
	#endif

	return xpni_lowfsi_pread(xpni_fit_get_XPN_DATA_FD(fd),
                                 buffer,
                                 offset,
                                 size);

      }


   /* ................................................................... */

