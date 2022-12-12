

   /* ... Include / Inclusion ........................................... */

      #include "xpni/xpni_ft/xpni_ft.h"


   /* ... Functions / Funciones ......................................... */

      int xpni_ft_close ( int fd )
      {

        /* debugging */
	#if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_ft_close(%d); \n",
                   __FILE__,__LINE__,fd);
	#endif

        /* check params */
        if ( ! xpni_fit_is_correct(fd) )
            return (-1) ;

        /* close file: data and metadata */
        if ( ! strcmp
              (
                (xpni_fit_get_XPN_FMETA(fd)).filesystem_tag,FS_TAG_RAID5OUTER
              )
        )
        {
	   xpni_lowfsi_close(xpni_fit_get_XPN_MDATA_FD(fd));
        }

	xpni_lowfsi_close(xpni_fit_get_XPN_DATA_FD(fd));

        /* return ok */
        return (1) ;

      }


   /* ................................................................... */

