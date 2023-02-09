

   /* ... Include / Inclusion ........................................... */

      #include "xpn2.h"


   /* ... Functions / Funciones ......................................... */

      int xpn2_open ( char *path, int flag, ... )
      {

        va_list     ap ;
        mode_t      mode ;
        int         ret ;
        int         fd ;
  
        /* Get ap argument list */
        va_start(ap, flag);
        mode = va_arg(ap, mode_t);
        va_end(ap);

        /* Tracing */
#if defined(XPN2_TRACE)
        TRACE_MSG_PrintF(SPOT,"xpn2_open(%s,%d,%d); \n",path,flag,mode);
#endif

	/* Open xpn2 file */
	fd = xpni_file_open(path,flag,mode) ;
        if ((-1) == fd) 
	{
#if defined(XPN2_DEBUG)
           DEBUG_MSG_PrintF(ERROR,"xpn2_open(%s,%d,%d): xpni_file_open fails.\n",path,flag,mode);
#endif
	   return (-1) ;
        }

        /* Open xpni file */
        ret = xpni_fsit_FSI_OPEN(fd) ;
        if ((-1) == ret)
        {
#if defined(XPN2_DEBUG)
            DEBUG_MSG_PrintF(ERROR,"xpn2_open(%s,%d,%d): xpni_fsit_FSI_OPEN fails.\n",path,flag,mode);
#endif
            xpni_file_close(fd) ;
            return (-1) ;
        }

        /* Return 'fd' */
        return fd ;

      }


   /* ................................................................... */

