

   /* ... Include / Inclusion ........................................... */

      #include "xpni/common/xpni_file.h"


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
      int xpni_file_initByName
      (
          fmeta_t *fmeta,
          char    *fname
      )
      {

        int   ret ;
        char  fm_fstag[1*1024] ;
        int   fm_sc ;
        int   fm_bs ;
	int   fm_ss ;
	char *fm_pn ;
        char *fname_aux1, *fname_aux2 ;


        /* Debugging */
	#if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_file_initByName(%p,%s); \n",
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
     *   get block_size and servers_count from xpn API with fd
     *
     *   IF (ALLREADY_OPENED) then fm_sc = xpn_file_table[fd]->mdata->nserv;
     *   ELSE (there is a xpn_stat ?)
     */

	fname_aux1 = STRING_MISC_StrDup(fname) ;
        if (NULL == fname_aux1) {
            printf("[%s:%d] xpni_file_initByName(%p,%s): can't parsing file name.\n",
                   __FILE__,__LINE__,fmeta,fname);
        }

	/* '/PNFS/file': skip first '/' */
	fname_aux2 = fname_aux1;
	if ('/' == fname_aux2[0])
	    fname_aux2 ++ ; 

	/* '/PNFS/file': search second '/' */
	fm_pn = fname_aux2 ;
	fname_aux2 = strchr(fname_aux2,'/') ;
	if (NULL != fname_aux2) 
	{
		fname_aux2[0] = '\0';
		fname_aux2 ++ ;
	}
	else
	{
		fname_aux2 = fname_aux1 ;
		fm_pn = "";
	}

	/* Extract fields from file name */
        ret = sscanf(fname_aux2,
                     "xpn-%[^-]-%d-%d-%d",
                     fm_fstag,
                     &fm_sc,
                     &fm_ss,
                     &fm_bs) ;
        if (ret < 4) {
            printf("[%s:%d] xpni_file_initByName(%p,%s): can't parsing file name.\n",
                   __FILE__,__LINE__,fmeta,fname);
        }

	/* Set metadata information */
        ret = xpni_fmeta_initByValues(fmeta,
			              fm_bs,fm_sc,fm_ss,fm_fstag,fm_pn,
                                      0,NULL,NULL,NULL,NULL,NULL,NULL);

	/* Free resources */
	free(fname_aux1) ;

        /* Return 'ret' */
	return ret;

      }

      /**
       * Get file name from metadata information (extended version)
       * Obtiene el nombre del archivo a partir de sus metadatos (versión extendida)
       *
       *    @param fmeta file metadata to be loaded.
       *    @return a allocated string with file named value.
       *
       */
      char * xpni_file_namedByMeta
      (
          fmeta_t *fmeta
      )
      {

        char *fname_aux ;

        /* Debugging */
	#if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_file_namedByMeta(%p); \n",
                   __FILE__,__LINE__,fmeta);
	#endif

	/* Check params */
	if (NULL == fmeta)
	     return NULL ;

        /*
         * TODO: 
	 *   update file metadata.
         */
        fname_aux = STRING_MISC_Dsprintf("%s/xpn-%s-%d-%d-%d",
                                         fmeta->partition_name,
                                         fmeta->filesystem_tag,
                                         fmeta->servers_count,
                                         fmeta->start_server,
                                         fmeta->block_size) ;
        if (NULL == fname_aux) {
            printf("[%s:%d] xpni_file_namedByMeta(%p): can't print file name.\n",
                   __FILE__,__LINE__,fmeta);
        }

        /* Return file name */
	return fname_aux ;

      }

      int xpni_file_open
      (
          char    *path,
          int      flag,
          int      mode
      )
      {
	char     *cpath ;
	fmeta_t   fmeta ;
	int       fsid ;
	int       fd ;


        /* Debugging */
	#if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_file_open(%p,%d,%d); \n",
                   __FILE__,__LINE__,path,flag,mode);
	#endif

	/* Copy of path */
        cpath = STRING_MISC_StrDup(path) ;
	if (NULL == cpath)
		return (-1) ;

        /* Get metadata by file name */
        xpni_file_initByName(&fmeta,cpath);
        fsid = xpni_fsit_getByName(fmeta.filesystem_tag) ;
        if ((-1) == fsid)
	{
	    xpni_fmeta_finalize(&fmeta) ;
	    free(cpath) ;
            return (-1) ;
	}

        /* Insert a new xpn2 file descriptor */
        fd = xpni_fit_insertNew() ;
        if ((-1) == fd)
	{
	    xpni_fmeta_finalize(&fmeta) ;
	    free(cpath) ;
            return (-1) ;
	}

	/* Set xpni file information */
        xpni_fit_set_XPNI_FSD(fd,fsid) ;
        xpni_fit_set_XPN_FNAME(fd,cpath) ;
        xpni_fit_set_XPN_FMETA(fd,fmeta) ;
        xpni_fit_set_XPN_FLAG(fd,flag) ;
        xpni_fit_set_XPN_MODE(fd,mode) ;

	/* Return file descriptor */
	return (fd) ;
      }

      int xpni_file_close 
      ( 
         int fd 
      )
      {

	fmeta_t  fmeta ;
	char    *fname ;


        /* Debugging */
	#if defined(XPN2_DEBUG)
            printf("[%s:%d] xpni_file_close(%d); \n",
                   __FILE__,__LINE__,fd);
	#endif

        /* Free resources: file name */
        fname = xpni_fit_get_XPN_FNAME(fd) ;
	if (NULL != fname)
            free(fname) ;

        /* Free resources: file metadata */
	fmeta = xpni_fit_get_XPN_FMETA(fd) ; 
	xpni_fmeta_finalize(&fmeta) ;

	/* Close xpni file */
        xpni_fit_delete(fd) ;

        /* Return ok */
	return (1) ;

      }

      int xpni_file_changeFileSystemTag 
      ( 
         int    fd,
         char  *fs_tag
      )
      {
        
        char       *fname_old ;
        fmeta_t     fmeta ;
        int         fsid_new ;
                                                                                                                            
                                                                                                                            
        /* Debugging */
        #if defined(XPN2_DEBUG)
            printf("[%s:%d] xpni_file_changeFileSystemTag(%d,%s); \n",
                   __FILE__,__LINE__,fd,fs_tag);
        #endif

        /* Get file system identification */
        fsid_new = xpni_fsit_getByName(fs_tag) ;
        if ((-1) == fsid_new)
            return (-1) ;

        /* Get metadata */
        fname_old = xpni_fit_get_XPN_FNAME(fd) ;
        xpni_file_initByName(&fmeta,fname_old) ;

        /* Change metadata */
        xpni_fmeta_changeFileSystemTag(&fmeta,fs_tag) ;
        xpni_fit_set_XPN_FMETA(fd,fmeta) ;
        xpni_fit_set_XPNI_FSD(fd,fsid_new) ;

        /* Return ok */
	return (1) ;

      }


   /* ................................................................... */

