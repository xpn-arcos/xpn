

   /* ... Include / Inclusion ........................................... */

      #include "xpni/xpni_log/xpni_log.h"
      #include "xpni/xpni_log/xpni_log_elog.h"


   /* ... Functions / Funciones ......................................... */

      int xpni_log_destroy ()
      {

	int    ret ;
	struct timeval t1, t2 ;


        /* debugging */
	#if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_log_destroy(); \n",
                   __FILE__,__LINE__);
	#endif

        /* passthru... */
	TIME_MISC_Timer(&t1);
	ret = xpni_lowfsi_destroy();
	TIME_MISC_Timer(&t2);

        /* record event */
	xpni_log_elog(&t1,&t2,"xpni_lowfsi_destroy",-1,-1,-1);

        /* return 'ret' */
	return ret ;

      }


   /* ................................................................... */

