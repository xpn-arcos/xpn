

   /* ... Include / Inclusion ........................................... */

      #include "xpn2.h"


   /* ... Functions / Funciones ......................................... */

      int xpn2_creat ( char *path, mode_t mode )
      { 

        int         ret ;
        int         fd ;
        
        /* Tracing */
#if defined(XPN2_TRACE)
        TRACE_MSG_PrintF(SPOT,"xpn2_creat(%s,%d); \n",path,mode);
#endif

	/* Open xpn2 file */
	fd = xpni_file_open(path,O_CREAT,mode) ;
        if ((-1) == fd) 
	{
#if defined(XPN2_DEBUG)
            DEBUG_MSG_PrintF(ERROR,"xpn2_creat(%s,%d): xpni_file_open fails.\n",path,mode);
#endif
	    return (-1) ;
        }

        /* Create file */
        ret = xpni_fsit_FSI_CREAT(fd) ;
        if ((-1) == ret)
        {
#if defined(XPN2_DEBUG)
            DEBUG_MSG_PrintF(ERROR,"xpn2_creat(%s,%d): xpni_fsit_FSI_CREAT fails.\n",path,mode);
#endif
            xpni_file_close(fd) ;
            return (-1) ;
        }
      
        /* Return 'fd' */
        return fd ;

      }
      

   /* ................................................................... */

