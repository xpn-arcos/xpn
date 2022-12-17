

   /* ... Include / Inclusion ........................................... */

      #include "xpni/xpni_ft/xpni_ft_read.h"


   /* ... Functions / Funciones ......................................... */

      ssize_t xpni_ft_sread_nofail_r5o
      (
         int fd,  
         void *buffer,  
         off_t offset,  
         size_t size
      )
      {
        /* debugging */
        #if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_ft_sread_r5o(%d,%p,%lu,%lu); \n",
                   __FILE__,__LINE__,fd,buffer,(unsigned long)offset,(unsigned long)size);
        #endif

	return xpni_lowfsi_pread(xpni_fit_get_XPN_DATA_FD(fd),
	 		         buffer,
			         (int)offset,
			         size) ;
      }


   /* ................................................................... */

