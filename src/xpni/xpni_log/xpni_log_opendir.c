


   /* ... Include / Inclusion ........................................... */

      #include "xpni/xpni_log/xpni_log.h"
      #include "xpni/xpni_log/xpni_log_elog.h"


   /* ... Functions / Funciones ......................................... */

      DIR * xpni_log_opendir ( int dd )  
      {

        DIR  *ret;
	struct timeval  t1, t2 ;


        /* debugging */
	#if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_log_opendir(%d); \n",
                   __FILE__,__LINE__,dd);
	#endif

        /* passthru... */
	TIME_MISC_Timer(&t1);
	ret = xpni_lowfsi_opendir(xpni_fit_get_XPN_FNAME(dd));
	TIME_MISC_Timer(&t2);

        /* record event */
	xpni_log_elog(&t1,&t2,"xpni_lowfsi_opendir",dd,-1,-1);

        /* update directory description */
        if ((int)ret >= 0) {
            xpni_fit_set_XPN_DATA_FD(dd,(int)ret) ; /* sizeof(int) === sizeof(DIR *) */
	}

        /* return xpn directory descriptor */
	return ret;

      }


   /* ................................................................... */

