

   /* ... Include / Inclusion ........................................... */

      #include "xpni/xpni_ft/xpni_ft.h"
      #include "xpni/xpni_ft/xpni_ft_metaData.h"


   /* ... Functions / Funciones ......................................... */

      int xpni_ft_open ( int fd )
      {

       char      *pfname ;
       int        ret ;

       /* debugging */
       #if defined(XPNI_DEBUG)
           printf("[%s:%d] xpni_ft_open(%d); \n",
                  __FILE__,__LINE__,fd);
       #endif

       /* check params */
       if ( ! xpni_fit_is_correct(fd) )
           return (-1) ;

       /* parity file name */
       if ( ! strcmp
              (
                (xpni_fit_get_XPN_FMETA(fd)).filesystem_tag,FS_TAG_RAID5OUTER
              )
       )
       {
	       /* parity file name */
               pfname = xpni_ft_metaData_getMetaDataFileName(fd) ;
	       if (NULL == pfname)
		   return (-1) ;

	       /* open parity file */
	       ret = xpni_lowfsi_open(pfname,
                                      O_CREAT|O_RDWR,
                                      S_IRWXU);
	       if (ret < 0)
		   return (-1) ;

	       /* update file description */
	       xpni_fit_set_XPN_MDATA_FD(fd,ret) ;

	       /* free resources */
	       free(pfname) ;
       }

       /* open data file */
       ret = xpni_lowfsi_open(xpni_fit_get_XPN_FNAME(fd),
                              xpni_fit_get_XPN_FLAG(fd),
                              xpni_fit_get_XPN_MODE(fd)) ;
       if (ret < 0)
       {
           xpni_lowfsi_close(xpni_fit_get_XPN_MDATA_FD(fd)) ;
           return (-1) ;
       }

       /* update file description */
       xpni_fit_set_XPN_DATA_FD(fd,ret) ;

       /* return xpn file descriptor */
       return ret ;

      }

      int xpni_ft_creat ( int fd )
      { 

       char      *pfname ;
       int        ret ;

       /* check params */
       if ( ! xpni_fit_is_correct(fd) )
           return (-1) ;

       /* parity file name */
       if ( ! strcmp
              (
                (xpni_fit_get_XPN_FMETA(fd)).filesystem_tag,
		FS_TAG_RAID5OUTER
              )
       )
       {
	       /* parity file name */
               pfname = xpni_ft_metaData_getMetaDataFileName(fd) ;
	       if (NULL == pfname)
		   return (-1) ;

	       /* open parity file */
	       ret = xpni_lowfsi_open(pfname,
                                      O_CREAT|O_RDWR,
                                      S_IRWXU);
	       if (ret < 0)
		   return (-1) ;

	       /* update file description */
	       xpni_fit_set_XPN_MDATA_FD(fd,ret) ;

	       /* free resources */
	       free(pfname) ;
       }

       /* open data file */
       ret = xpni_lowfsi_creat(xpni_fit_get_XPN_FNAME(fd),
                               xpni_fit_get_XPN_MODE(fd)) ;
       if (ret < 0)
       {
           xpni_lowfsi_close(xpni_fit_get_XPN_MDATA_FD(fd)) ;
           return (-1) ;
       }

       /* update file description */
       xpni_fit_set_XPN_DATA_FD(fd,ret) ;

       /* return xpn file descriptor */
       return ret ;

      }


   /* ................................................................... */

