

   /* ... Include / Inclusion ........................................... */

      #include "xpni/xpni_ft/xpni_ft.h"


   /* ... Functions / Funciones ......................................... */

      off_t   xpni_ft_lseek   ( int fd, off_t offset, int flag )
      {

        /* debugging */
	#if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_ft_lseek(%d); \n",
                   __FILE__,__LINE__,fd);
	#endif

        /* check params */
        if ( ! xpni_fit_is_correct(fd) )
            return (-1) ;

        /* lseek file: data only */
	xpni_lowfsi_lseek(xpni_fit_get_XPN_DATA_FD(fd),offset,flag);

        /* return ok */
        return (1) ;

      }


   /* ................................................................... */

