

   /* ... Include / Inclusion ........................................... */

      #include "xpni/xpni_log/xpni_log.h"
      #include "xpni/xpni_log/xpni_log_elog.h"


   /* ... Functions / Funciones ......................................... */

      int xpni_log_importFile ( int fd )  
      {

	int    ret ;
	struct timeval t1, t2 ;


        /* debugging */
	#if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_log_importFile(%d); \n",
                   __FILE__,__LINE__,fd);
	#endif

	/* nothing to do, by now... */
	TIME_MISC_Timer(&t1);
	ret = 1;
	TIME_MISC_Timer(&t2);

        /* record event */
	xpni_log_elog(&t1,&t2,"xpn_ioctl.importFile!",fd,-1,-1);

        /* return 'ret' */
	return ret ;

      }


   /* ................................................................... */

