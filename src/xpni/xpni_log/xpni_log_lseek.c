

   /* ... Include / Inclusion ........................................... */

      #include "xpni/xpni_log/xpni_log.h"
      #include "xpni/xpni_log/xpni_log_elog.h"


   /* ... Functions / Funciones ......................................... */

      off_t  xpni_log_lseek ( int fd, off_t offset, int flag )
      {

	int    data_fd ;
	int    ret ;
	struct timeval t1, t2 ;


        /* debugging */
	#if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_log_lseek(%d,%d,%d); \n",
                   __FILE__,__LINE__,fd,offset,flag);
	#endif

	/* passthru... */
	data_fd = xpni_fit_get_XPN_DATA_FD(fd);
	TIME_MISC_Timer(&t1);
	ret = xpni_lowfsi_lseek(data_fd,offset,flag);
	TIME_MISC_Timer(&t2);

        /* record event */
	xpni_log_elog(&t1,&t2,"xpni_lowfsi_lseek",fd,-1,-1);

        /* return 'ret' */
	return ret ;

      }


   /* ................................................................... */

