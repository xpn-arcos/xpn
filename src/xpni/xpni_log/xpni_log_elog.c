

   /* ... Include / Inclusion ........................................... */

      #include "xpni/xpni_log/xpni_log.h"
      #include "xpni/xpni_log/xpni_log_elog.h"


   /* ... Variables / Variabls .......................................... */

      FILE            *xpni_log_lfile ;
      pthread_mutex_t  xpni_log_fastmutex = PTHREAD_MUTEX_INITIALIZER ;


   /* ... Functions / Funciones ......................................... */

      int xpni_log_elog 
      ( 
         /*IN*/  struct timeval *t1,
         /*IN*/  struct timeval *t2,
         /*IN*/  char           *action,
         /*IN*/  int             object,
         /*IN*/  int             offset,
         /*IN*/  int             size
      )
      {

         struct timeval  diff ;
         float           diff_ms ;
         float           t1_ms ;
	 /*
         long            t1_s, t1_m, t1_h, t1_D, t1_Y ;
	 */
	 char           *object_name ;
	 int             ret ;


         /* debugging */
	 #if defined(XPNI_DEBUG)
             printf("[%s:%d] xpni_log_elog(%p,%p,%s,%d); \n",
                    __FILE__,__LINE__,t1,t2,action,object);
	 #endif

	 /* get time as msec ... */
         TIME_MISC_DiffTime(t1,t2,&diff);
         diff_ms = TIME_MISC_TimevaltoMicro(&diff);
         t1_ms   = TIME_MISC_TimevaltoMicro(t1);

	 /*
	 t1_s  = t1_ms / 1000000 ;
	 t1_m  = t1_s  / 60 ;   
	         t1_s = t1_s - t1_m*60 ;
	 t1_h  = t1_m  / 60 ;   
	         t1_m = t1_m - t1_h*60 ;
	 t1_D  = t1_h  / 24 ;   
	         t1_h = t1_h - t1_D*24 ;
	 t1_Y  = t1_D  / 365.25 ; 
	         t1_D = t1_D - t1_Y*365.25 ;
	 t1_Y  = t1_Y + 1970 ;
	 */

	 /* get object name... */
	 if (-1 != object)
             object_name = xpni_fit_get_XPN_FNAME(object) ;
	 else
             object_name = "null";

	 /* write event to log... */
         ret = xpni_log_vprintf("%f %f %d %s %d %d %s\n",
				 t1_ms,diff_ms,
                                 object,action,offset,size,
				 object_name) ;
	 /*
         ret = xpni_log_vprintf("%4.0ld-%2.0ld-%2.0ld-%2.0ld-%2.0ld %f %d %s %d %d %s\n",
                                 t1_Y,t1_D,t1_h,t1_m,t1_s,diff_ms,
                                 object,action,offset,size,
				 object_name) ;
	 */

	 /* return 'ret' */
         return ret ;

      }

      int xpni_log_vprintf
      (
         /*IN*/   char *einfo,
         ...
      )
      {

        va_list  vl ;
	char    *fname ;
	int      ret ;


        va_start(vl,einfo) ;

        /* debugging */
	#if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_log_elog(%s); \n",
                   __FILE__,__LINE__,ename);
	#endif

	/* record event */
	pthread_mutex_lock(&xpni_log_fastmutex) ;

	   if (NULL == xpni_log_lfile)
	   {
	       fname = STRING_MISC_Dsprintf("xpni_log.%d",getpid()) ;
	       xpni_log_lfile = fopen(fname,"a") ;
	       free(fname) ;
	   }

	   if (NULL != xpni_log_lfile)
	   {
	       ret = vfprintf(xpni_log_lfile,einfo,vl) ; 
	   }

	pthread_mutex_unlock(&xpni_log_fastmutex) ;

	/* return 'ret' */
        va_end(vl) ;
	return ret;

      }


   /* ................................................................... */

