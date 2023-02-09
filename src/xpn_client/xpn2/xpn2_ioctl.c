

   /* ... Include / Inclusion ........................................... */

      #include "xpn2.h"


   /* ... Functions / Funciones ......................................... */

      int xpn2_ioctl_setXpn2Interface ( int fd, char *fs_tag )
      {

	int         ret ;
        int         fsid_new ;


        /* Tracing */
#if defined(XPN2_TRACE)
        TRACE_MSG_PrintF(SPOT,"xpn2_ioctl_setXpn2Interface(%d,%s); \n",fd,fs_tag);
#endif

        /* Get FS identification from FS tag */
        fsid_new = xpni_fsit_getByName(fs_tag) ;
        if (fsid_new < 0)
            return fsid_new ;

        /* Export process */
	ret = xpni_fsit_FSI_EXPORTFILE(fd) ;
        if (ret < 0)
            return ret ;

        /* Change metadata */
	xpni_file_changeFileSystemTag(fd,fs_tag) ;

        /* Import process */
	ret = xpni_fsit_FSI_IMPORTFILE(fd) ;
        if (ret < 0)
            return ret ;

        /* Return ok */
	return (1) ;

      }

      int xpn2_ioctl ( int fd, int request, ... )
      {

        /* Tracing */
#if defined(XPN2_TRACE)
        TRACE_MSG_PrintF(SPOT,"xpn2_ioctl(%d,%d); \n",fd,request);
#endif

        /* Check params */
	if ( ! xpni_fit_is_correct(fd) )
        {
#if defined(XPN2_DEBUG)
            DEBUG_MSG_PrintF(ERROR,"xpn2_ioctl(%d,%d): unknow file descriptor.\n",fd,request);
#endif
    	    return (-1) ;
        }

	if ( (request < XPN2_IOCTL_FIRST) || (request > XPN2_IOCTL_LAST) )
        {
#if defined(XPN2_DEBUG)
            DEBUG_MSG_PrintF(ERROR,"xpn2_ioctl(%d,%d): unknow request command.\n",fd,request);
#endif
    	    return (-1) ;
        }

        /* Process request ... */
        switch (request)
        {
           case XPN2_IOCTL_SETXPN2INTERFACE:
                {
		     va_list     ap ;
		     int         ret ;
                     char       *fs_tag ;

		     /* Get ap argument list */
		     va_start(ap, request);
		     fs_tag = va_arg(ap,char *);
		     va_end(ap);

                     /* Check params */
                     if (NULL == fs_tag)
                     {
#if defined(XPN2_DEBUG)
                         DEBUG_MSG_PrintF(ERROR,"xpn2_ioctl(%d,%d): fs_tag is NULL!.\n",fd,request);
#endif
                         return (-1) ;
                     }

		     /* Get ap argument list */
                     ret = xpn2_ioctl_setXpn2Interface(fd,fs_tag) ;
                     if (ret < 0)
                     {
#if defined(XPN2_DEBUG)
                         DEBUG_MSG_PrintF(ERROR,"xpn2_ioctl(%d,%d): request fails!.\n",fd,request);
#endif
                         return (ret) ;
                     }
                }
                break ;

           default:
                {
#if defined(XPN2_DEBUG)
                     DEBUG_MSG_PrintF(ERROR,"xpn2_ioctl(%d,%d): request handle not in source file!.\n",fd,request);
#endif
    	             return (-1) ;
                }
        }

        /* Return ok */
	return (1) ;

      }


   /* ................................................................... */

