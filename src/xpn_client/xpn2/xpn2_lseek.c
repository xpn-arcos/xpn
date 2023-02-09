

   /* ... Include / Inclusion ........................................... */

      #include "xpn2.h"


   /* ... Functions / Funciones ......................................... */

      off_t  xpn2_lseek ( int fd, off_t offset, int flag )
      {

	int ret ;


        /* Tracing */
#if defined(XPN2_TRACE)
	TRACE_MSG_PrintF(SPOT,"xpn2_lseek(%d,%d,%d)\n",fd,offset,flag) ;
#endif

        /* Check params */
	if ( ! xpni_fit_is_correct(fd) )
        {
#if defined(XPN2_DEBUG)
	    DEBUG_MSG_PrintF(ERROR,"xpn2_close(%d,%d,%d): unknow file descriptor.\n",fd,offset,flag) ;
#endif
    	    return (-1) ;
        }

        /* Lseek xpni file */
        ret = xpni_fsit_FSI_LSEEK(fd,offset,flag) ;

        /* Return 'ret' */
	return (ret) ;

      }


   /* ................................................................... */

