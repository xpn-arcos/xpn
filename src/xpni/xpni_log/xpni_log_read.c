

   /* ... Include / Inclusion ........................................... */

      #include "xpni/xpni_log/xpni_log.h"
      #include "xpni/xpni_log/xpni_log_elog.h"


   /* ... Functions / Funciones ......................................... */

      ssize_t xpni_log_sread
      (
         int fd, 
         void *buffer, 
         off_t offset, 
         size_t size
      )
      {

	int             data_fd ;
	int             ret ;
	struct timeval  t1, t2 ;


        /* debugging */
	#if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_log_sread(%d,%p,%d,%d); \n",
                   __FILE__,__LINE__,fd,buffer,(int)offset,size);
	#endif

        /* passthru... */
	data_fd = xpni_fit_get_XPN_DATA_FD(fd);
	TIME_MISC_Timer(&t1);
	ret = xpni_lowfsi_pread(data_fd,
                                buffer,
                                offset,
                                size);
	TIME_MISC_Timer(&t2);

        /* record event */
	xpni_log_elog(&t1,&t2,"xpni_lowfsi_sread",fd,offset,size);

        /* return 'ret' */
	return ret;

      }


   /* ................................................................... */

