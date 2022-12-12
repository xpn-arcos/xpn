

   /* ... Include / Inclusion ........................................... */

      #include "xpni/xpni_ft/xpni_ft.h"
      #include "xpni/xpni_ft/xpni_ft_metaData.h"


   /* ... Functions / Funciones ......................................... */

      int xpni_ft_exportFile_r5i ( __attribute__((__unused__)) int fd )  
      {

        /* debugging */
	#if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_ft_exportFile_r5i(%d); \n",
                   __FILE__,__LINE__,fd);
	#endif

	/* 
	 * TODO: Rebuild data file removing parity
	 */

	/* return ok */
	return (1) ;

      }

      int xpni_ft_exportFile_r5o ( int fd )  
      {

	char  *pfname ;
	int    ret ;

        /* debugging */
	#if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_ft_exportFile_r5o(%d); \n",
                   __FILE__,__LINE__,fd);
	#endif

        /* parity file name */
        pfname = xpni_ft_metaData_getMetaDataFileName(fd) ;
        if (NULL == pfname)
	    return (-1) ;

        /* unlink file */
	ret = xpn_unlink(pfname) ;

        /* free parity file name */
        free(pfname) ;

	/* return 'ok' */
	return (ret) ;

      }

      int xpni_ft_exportFile ( int fd )  
      {
        char *fmeta_fsTag;     /* File metadata (file system tag) */
        ssize_t ret;           /* Returned value from called functions */


        /* debugging */
        #if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_ft_exportFile(%d); \n",
                   __FILE__,__LINE__,fd);
        #endif

        /* check params */
        if ( ! xpni_fit_is_correct(fd) )
            return (-1) ;

        /* Select file system type */
        fmeta_fsTag = xpni_fit_get_XPN_FMETA(fd).filesystem_tag ;

        if (!strncmp(fmeta_fsTag,FS_TAG_RAID5INNER,strlen(FS_TAG_RAID5INNER)))
            ret = xpni_ft_exportFile_r5i(fd) ;

        else if (!strncmp(fmeta_fsTag,FS_TAG_RAID5OUTER,strlen(FS_TAG_RAID5OUTER)))
            ret = xpni_ft_exportFile_r5o(fd) ;

        else 
	{
            ret = (-1);
        #if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_ft_exportFile(%d,%p,%d,%d): Unknow file system tag: '%s' (not %s or %s)\n",
                   __FILE__,__LINE__,fd,buffer,(int)offset,size,fmeta_fsTag,FS_TAG_RAID5INNER,FS_TAG_RAID5OUTER);
        #endif
	}

        /* Return bytes readed */
        return ret ;
      }


   /* ................................................................... */

