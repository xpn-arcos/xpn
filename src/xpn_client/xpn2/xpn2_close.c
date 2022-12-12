

   /* ... Include / Inclusion ........................................... */

      #include "xpn2.h"


   /* ... Functions / Funciones ......................................... */

      int xpn2_close ( int fd )
      {

	int ret ;


        /* Tracing */
#if defined(XPN2_TRACE)
	TRACE_MSG_PrintF(SPOT,"xpn2_close(%d)\n",fd) ;
#endif

        /* Check params */
	if ( ! xpni_fit_is_correct(fd) )
        {
#if defined(XPN2_DEBUG)
	    DEBUG_MSG_PrintF(ERROR,"xpn2_close(%d): unknow file descriptor.\n",fd) ;
#endif
    	    return (-1) ;
        }

        /* Close xpni file */
        xpni_fsit_FSI_CLOSE(fd) ;

        /* Close xpn2 file */
        ret = xpni_file_close(fd) ;
	if ( ret < 0 )
        {
#if defined(XPN2_DEBUG)
	    DEBUG_MSG_PrintF(ERROR,"xpn2_close(%d): cannot close file.\n",fd) ;
#endif
    	    return (-1) ;
        }

        /* Return ok */
	return (ret) ;

      }


   /* ................................................................... */

