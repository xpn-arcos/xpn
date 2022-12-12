
   /* ... Include / Inclusion ........................................... */

      #include "xpni/xpni_ft/xpni_ft.h"
      #include "xpni/xpni_ft/xpni_ft_read.h"


   /* ... Functions / Funciones ......................................... */

      ssize_t xpni_ft_sread_r5i
      (
        int fd, 
        void *buffer, 
        off_t offset, 
        size_t size
      )
      {
        char *fmeta_fsTag;     /* File metadata (file system tag) */
        int   fmeta_nerrors;   /* File metadata (number of errors) */
        ssize_t   ret;         /* Returned value from called functions */


        /* debugging */
        #if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_ft_sread_r5i(%d,%p,%d,%d); \n",
                   __FILE__,__LINE__,fd,buffer,(int)offset,size);
        #endif

        /* check params */
        if ( ! xpni_fit_is_correct(fd) )
            return (-1) ;

        /* Select (file system type) x (status) */
        fmeta_fsTag   = xpni_fit_get_XPN_FMETA(fd).filesystem_tag ;
        fmeta_nerrors = xpni_fit_get_XPN_FMETA(fd).nerrors ;

        if (!strncmp(fmeta_fsTag,FS_TAG_RAID5INNER,strlen(FS_TAG_RAID5INNER)))
	{
		switch (fmeta_nerrors)
		{
			case 0:
                             ret = xpni_ft_sread_nofail_r5i(fd,buffer,(int)offset,size) ;
			     break;

			case 1:
                             ret = xpni_ft_sread_fail_r5i(fd,buffer,(int)offset,size) ;
			     break;

			default:
                             ret = (-1);
#if defined(XPNI_DEBUG)
                             printf("[%s:%d] xpni_ft_sread_r5i(%d,%p,%d,%d): %d fail(s)\n",
                                    __FILE__,__LINE__,fd,buffer,(int)offset,size,fmeta_nerrors);
#endif
			     break;
		}
	}

        else 
	{
            ret = (-1);
#if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_ft_sread_r5i(%d,%p,%d,%d): Unknow file system tag: '%s' (not %s or %s)\n",
                   __FILE__,__LINE__,fd,buffer,(int)offset,size,
		   fmeta_fsTag,FS_TAG_RAID5INNER,FS_TAG_RAID5OUTER);
#endif
	}

        /* Return bytes written */
        return ret ;
      }


   /* ................................................................... */

