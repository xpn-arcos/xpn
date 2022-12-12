

   /* ... Include / Inclusion ........................................... */

      #include "xpni/xpni_null/xpni_null.h"


   /* ... Functions / Funciones ......................................... */

      int xpni_null_open ( int fd )
      {

       xpni_fi_t *xpni_fi1 ;
       int ret ;

        /* debugging */
	#if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_null_open(%d); \n",
                   __FILE__,__LINE__,fd);
	#endif

       /* check params */
       xpni_fi1 = xpni_fit_get(fd) ;
       if (NULL == xpni_fi1)
	   return (-1) ;

       /* open file */
       ret = xpni_lowfsi_open(xpni_fit_get_XPN_FNAME(fd),
                              xpni_fit_get_XPN_FLAG(fd),
                              xpni_fit_get_XPN_MODE(fd)) ;
       if (ret < 0)
	   return (-1) ;

       /* update file description */
       xpni_fit_set_XPN_DATA_FD(fd,ret) ;

       /* return xpn file descriptor */
       return ret ;

      }


      int xpni_null_creat ( int fd )
      { 

       xpni_fi_t *xpni_fi1 ;
       int ret ;

        /* debugging */
	#if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_null_creat(%d); \n",
                   __FILE__,__LINE__,fd);
	#endif

       /* check params */
       xpni_fi1 = xpni_fit_get(fd) ;
       if (NULL == xpni_fi1)
	   return (-1) ;

       /* create file */
       ret = xpni_lowfsi_creat(xpni_fit_get_XPN_FNAME(fd),
                               xpni_fit_get_XPN_MODE(fd)) ;
       if (ret < 0)
	   return (-1) ;

       /* update file description */
       xpni_fit_set_XPN_DATA_FD(fd,ret) ;

       /* return xpn file descriptor */
       return ret ;

      }


   /* ................................................................... */

