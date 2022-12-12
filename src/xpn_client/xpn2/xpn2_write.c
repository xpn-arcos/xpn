

   /* ... Include / Inclusion ........................................... */

      #include "xpn2.h"


   /* ... Functions / Funciones ......................................... */

      ssize_t xpn2_swrite(int fd, void *buffer, off_t offset, size_t size)
      {

	ssize_t ret ;


        /* Tracing */
#if defined(XPN2_TRACE)
        TRACE_MSG_PrintF(SPOT,"xpn2_swrite(%d,%p,%d,%d);\n",fd,buffer,(int)offset,size);
#endif

        /* Check params */
	if ( ! xpni_fit_is_correct(fd) )
        {
#if defined(XPN2_DEBUG)
            TRACE_MSG_PrintF(SPOT,"xpn2_swrite(%d,%p,%d,%d);\n",fd,buffer,(int)offset,size);
#endif
    	    return (-1) ;
        }

        /* Process request */
      	ret = xpni_fsit_FSI_SWRITE(fd,buffer,offset,size);
	if ( ret < 0 )
        {
#if defined(XPN2_DEBUG)
            TRACE_MSG_PrintF(SPOT,"xpn2_swrite(%d,%p,%d,%d);\n",fd,buffer,(int)offset,size);
#endif
    	    return ret ;
        }

	return ret ;

      }


ssize_t xpn2_write(int fd, void *buffer, size_t size)
{
	int ret;
	ret = xpn2_swrite(fd, buffer, xpn_file_table[fd]->offset, size);
	if(ret>0)
		xpn_file_table[fd]->offset += ret;
	return ret;
}



   /* ................................................................... */

