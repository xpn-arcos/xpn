

   /* ... Include / Inclusion ........................................... */

      #include "xpni/common/xpni_dir.h"


   /* ... Functions / Funciones ......................................... */

      /**
       * Get file metadata information (extended version)
       * Obtiene los metadatos de un archivo (versión extendida)
       *
       *    @param fmeta file metadata to be loaded.
       *    @param fname file name.
       *    @return true (1) if ok or error (-1) if any problem is found.
       *
       */
      int xpni_dir_initByName
      (
          fmeta_t *fmeta,
          char    *fname
      )
      {

        int   ret ;


        /* Debugging */
	#if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_dir_initByName(%p,%s); \n",
                   __FILE__,__LINE__,fmeta,fname);
	#endif

	/* Check params */
	if (NULL == fmeta)
	     return (-1) ;
	if (NULL == fname)
	     return (-1) ;

        /*
         * TODO: 
	 *   remove getting values based on file name and
         *   get block_size and servers_count from xpn API with dd
         *
	 *   IF (ALLREADY_OPENED) then fm_sc = xpn_file_table[dd]->mdata->nserv;
         */

	/* Set metadata information */
        ret = xpni_fmeta_initByValues(fmeta,
			              8192,1,1,"xpni_null",fname,
                                      0,NULL,NULL,NULL,NULL,NULL,NULL);

        /* Return ok */
	return (1);

      }

      int xpni_dir_open
      (
          char    *path
      )
      {
	char     *cpath ;
	fmeta_t   fmeta ;
	int       fsid ;
	int       dd ;


        /* Debugging */
	#if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_dir_open(%p); \n",
                   __FILE__,__LINE__,path);
	#endif

	/* Copy of path */
        cpath = STRING_MISC_StrDup(path) ;
	if (NULL == cpath)
		return (-1) ;

        /* Get metadata by file name */
        xpni_dir_initByName(&fmeta,cpath);
        fsid = xpni_fsit_getByName(fmeta.filesystem_tag) ;
        if ((-1) == fsid)
	{
	    xpni_fmeta_finalize(&fmeta) ;
	    free(cpath) ;
            return (-1) ;
	}

        /* Insert a new xpn2 file descriptor */
        dd = xpni_fit_insertNew() ;
        if ((-1) == dd)
	{
	    xpni_fmeta_finalize(&fmeta) ;
	    free(cpath) ;
            return (-1) ;
	}

	/* Set xpni file information */
        xpni_fit_set_XPNI_FSD(dd,fsid) ;
        xpni_fit_set_XPN_FNAME(dd,cpath) ;
        xpni_fit_set_XPN_FMETA(dd,fmeta) ;

	/* Return file descriptor */
	return (dd) ;
      }

      int xpni_dir_close 
      ( 
         int dd 
      )
      {

	fmeta_t  fmeta ;
	char    *fname ;


        /* Debugging */
	#if defined(XPN2_DEBUG)
            printf("[%s:%d] xpni_dir_close(%d); \n",
                   __FILE__,__LINE__,dd);
	#endif

        /* Free resources: file name */
        fname = xpni_fit_get_XPN_FNAME(dd) ;
	if (NULL != fname)
            free(fname) ;

        /* Free resources: file metadata */
	fmeta = xpni_fit_get_XPN_FMETA(dd) ; 
	xpni_fmeta_finalize(&fmeta) ;

	/* Close xpni file */
        xpni_fit_delete(dd) ;

        /* Return ok */
	return (1) ;

      }

      int xpni_dir_changeFileSystemTag 
      ( 
         int    dd,
         char  *fs_tag
      )
      {
        
        char       *fname_old ;
        fmeta_t     fmeta ;
        int         fsid_new ;
                                                                                                                            
        /* Debugging */
        #if defined(XPN2_DEBUG)
            printf("[%s:%d] xpni_dir_changeFileSystemTag(%d,%s); \n",
                   __FILE__,__LINE__,dd,fs_tag);
        #endif

        /* Get file system identification */
        fsid_new = xpni_fsit_getByName(fs_tag) ;
        if ((-1) == fsid_new)
            return (-1) ;

        /* Get metadata */
        fname_old = xpni_fit_get_XPN_FNAME(dd) ;
        xpni_dir_initByName(&fmeta,fname_old) ;

        /* Change metadata */
        xpni_fmeta_changeFileSystemTag(&fmeta,fs_tag) ;
        xpni_fit_set_XPN_FMETA(dd,fmeta) ;
        xpni_fit_set_XPNI_FSD(dd,fsid_new) ;

        /* Return ok */
	return (1) ;

      }


   /* ................................................................... */

