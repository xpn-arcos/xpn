

   /* ... Include / Inclusion ........................................... */

      #include "xpn2.h"


   /* ... Functions / Funciones ......................................... */

      int xpn2_opendir ( char *path )
      {

        DIR  *ret ;
        int   dd ;
  

        /* Tracing */
#if defined(XPN2_TRACE)
        TRACE_MSG_PrintF(SPOT,"xpn2_opendir(%s); \n",path);
#endif

	/* Open xpn2 file */
	dd = xpni_dir_open(path) ;
        if ((-1) == dd) 
	{
#if defined(XPN2_DEBUG)
           DEBUG_MSG_PrintF(ERROR,
			    "xpn2_opendir(%s): xpni_dir_open fails.\n",
			    path,flag,mode);
#endif
	   return (-1) ;
        }

        /* Open xpni file */
        ret = xpni_fsit_FSI_OPENDIR(dd) ;
        if (NULL == ret)
        {
#if defined(XPN2_DEBUG)
            DEBUG_MSG_PrintF(ERROR,
			     "xpn2_opendir(%s): xpni_fsit_FSI_OPENDIR fails.\n",
			     path,flag,mode);
#endif
            xpni_dir_close(dd) ;
            return (-1) ;
        }

        /* Return 'dd' */
        return (int)dd ;  /* sizeof(int) === sizeof(DIR *) */

      }


   /* ................................................................... */

