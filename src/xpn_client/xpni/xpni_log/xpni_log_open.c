

   /* ... Include / Inclusion ........................................... */

      #include "xpni/xpni_log/xpni_log.h"
      #include "xpni/xpni_log/xpni_log_elog.h"


   /* ... Functions / Funciones ......................................... */

      int xpni_log_open ( int fd )
      {

	int             ret ;
	struct timeval  t1, t2 ;
        xpni_fi_t      *xpni_fi1 ;


        /* debugging */
	#if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_log_open(%d); \n",
                   __FILE__,__LINE__,fd);
	#endif

        /* check params */
        xpni_fi1 = xpni_fit_get(fd) ;
        if (NULL == xpni_fi1) {
	    return (-1) ;
	}

	/* passthru... */
	TIME_MISC_Timer(&t1);
        ret = xpni_lowfsi_open(xpni_fit_get_XPN_FNAME(fd),
                               xpni_fit_get_XPN_FLAG(fd),
                               xpni_fit_get_XPN_MODE(fd)) ;
	TIME_MISC_Timer(&t2);

        /* record event */
	xpni_log_elog(&t1,&t2,"xpni_lowfsi_open",fd,-1,-1);

        /* update file description */
        if (ret >= 0) {
            xpni_fit_set_XPN_DATA_FD(fd,ret) ;
	}

        /* return xpn file descriptor */
        return ret ;

      }


      int xpni_log_creat ( int fd )
      { 

        int             ret ;
	struct timeval  t1, t2 ;
        xpni_fi_t *xpni_fi1 ;

        /* debugging */
	#if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_log_creat(%d); \n",
                   __FILE__,__LINE__,fd);
	#endif

        /* check params */
        xpni_fi1 = xpni_fit_get(fd) ;
        if (NULL == xpni_fi1)
	    return (-1) ;

	/* passthru... */
	TIME_MISC_Timer(&t1);
        ret = xpni_lowfsi_creat(xpni_fit_get_XPN_FNAME(fd),
                                xpni_fit_get_XPN_MODE(fd)) ;
	TIME_MISC_Timer(&t2);

        /* record event */
	xpni_log_elog(&t1,&t2,"xpn_creat",fd,-1,-1);

        /* update file description */
        if (ret >= 0)
            xpni_fit_set_XPN_DATA_FD(fd,ret) ;

        /* return xpn file descriptor */
        return ret ;

      }


   /* ................................................................... */

