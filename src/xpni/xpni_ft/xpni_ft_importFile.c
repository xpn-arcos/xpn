

   /* ... Include / Inclusion ........................................... */

      #include "xpni/xpni_ft/xpni_ft.h"
      #include "xpni/xpni_ft/xpni_ft_metaData.h"
      #include "xpni/xpni_ft/xpni_ft_write.h"


   /* ... Functions / Funciones ......................................... */

      int xpni_ft_importFile_r5i ( int fd )  
      {

        int       ret ;
	int       fd_aux, fd_aux2 ;
	char     *pfname, *oldname ;
	int       readed, writed ;
	int       len_d, off_d ;
	char     *buf_d ;
        fmeta_t   fmeta ;
        int       fsid ;


        /* debugging */
	#if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_ft_importFile_r5i(%d); \n",
                   __FILE__,__LINE__,fd);
	#endif


        /* old file metainfo */
        oldname = xpni_fit_get_XPN_FNAME(fd) ;
	fmeta   = xpni_fit_get_XPN_FMETA(fd) ;

        /* new file name */
	pfname = xpni_file_namedByMeta(&fmeta) ;
        if (NULL == pfname) {
 	    return (-1) ;
        }

        /* open parity file as auxiliar for data+parity */
        fd_aux = xpni_lowfsi_open(pfname,
			          O_CREAT|O_RDWR,
			          S_IRWXU);
        if (fd_aux < 0) {
	    return (-1) ;
        }

	/* alloc input/output buffer */
	len_d = 32*1024 ;
	buf_d = (char *)malloc(len_d) ;
	if (NULL == buf_d) {
	    free(pfname) ;
  	    return (-1) ;
	}

	/* looping... */
	fd_aux2 = xpni_fit_get_XPN_DATA_FD(fd) ;
	xpni_fit_set_XPN_DATA_FD(fd,fd_aux) ;

	off_d = 0 ;
	do
	{
	   readed = xpni_lowfsi_pread(fd_aux2,buf_d,off_d,len_d) ;
	   writed = xpni_ft_swrite_r5i(fd,buf_d,off_d,len_d) ;
	   off_d = off_d + len_d ;

	} while ( 
		   (readed > 0) && (readed==writed) 
		) ;

	/* close files */
	xpni_lowfsi_close(fd_aux2) ;
	xpni_lowfsi_close(xpni_fit_get_XPN_DATA_FD(fd)) ;

	/* update file description: fd */
        fd_aux = xpni_lowfsi_open(pfname,
			          O_CREAT|O_RDWR,
			          S_IRWXU) ;
        xpni_fit_set_XPN_DATA_FD(fd,fd_aux) ;

	/* update file description: fsid */
        fsid = xpni_fsit_getByName(FS_TAG_RAID5INNER) ;
        if ((-1) == fsid)
        {
            free(pfname) ;
            return (-1) ;
        }
        xpni_fit_set_XPNI_FSD(fd,fsid) ;

	/* update file description: fmeta */
	ret = xpni_file_changeFileSystemTag(fd,FS_TAG_RAID5INNER) ;
        if ((-1) == ret)
        {
            free(pfname) ;
            return (-1) ;
        }

	/* update file description: file name */
        xpni_fit_set_XPN_FNAME(fd,pfname) ;

	/* free old resources */
	xpni_lowfsi_unlink(oldname) ;
	free(oldname) ;

        /* finishing ok */
	return (1) ;
      }

      int xpni_ft_importFile_r5o ( int fd )  
      {

	int       ret;
	int       i;
	char     *pfname ;
	fmeta_t   fmeta;
	int       readed ;
	int       len_d, len_p, len_a ;
	char     *buf_d, *buf_p, **buf_a ;
	int       offset_p, block_p, row_p, col_p ;
	int       offset_d, block_d, row_d ;
	int       block_per_row ;


        /* debugging */
	#if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_ft_importFile_r5o(%d); \n",
                   __FILE__,__LINE__,fd);
	#endif

        /* parity file name */
	pfname = xpni_ft_metaData_getMetaDataFileName(fd) ;
        if (NULL == pfname)
 	    return (-1) ;

        /* open parity file */
        ret = xpni_lowfsi_open(pfname,
			       O_CREAT|O_RDWR,
			       S_IRWXU);
        if (ret < 0)
	    return (-1) ;

        /* update file description */
        xpni_fit_set_XPN_MDATA_FD(fd,ret) ;

        /* free resources */
	free(pfname) ;

	/* get file metadata */
	fmeta = xpni_fit_get_XPN_FMETA(fd) ;

	/* alloc buffers */
	block_per_row = fmeta.servers_count - 1 ;

	len_p = fmeta.block_size ;
	buf_p = (char *)malloc(len_p) ;
	if (NULL == buf_p)
		return (-1) ;

	len_d = block_per_row * fmeta.block_size ;
	buf_d = (char *)malloc(len_d) ;
	if (NULL == buf_d)
	{
		free(buf_p) ;
		return (-1) ;
	}

	len_a = block_per_row ;
	buf_a = (char **)malloc(len_a*sizeof(char *)) ;
	if (NULL == buf_a)
	{
		free(buf_p) ;
		free(buf_d) ;
		return (-1) ;
	}

	/* read data and write associated parity */
	readed   = len_d ;
	offset_d = 0 ;
	while (readed == len_d)
	{
	   block_d  = (offset_d / fmeta.block_size) + ( (offset_d % fmeta.block_size) != 0 ) ;
	   row_d    = (block_d  / block_per_row)    + ( (block_d  % block_per_row) != 0 ) ;
	   row_p    = row_d / fmeta.servers_count ;
	   col_p    = block_per_row - (row_d % fmeta.servers_count) ;
	   block_p  = (fmeta.servers_count*row_p + col_p) ;
	   offset_p = (fmeta.servers_count*row_p + col_p) * fmeta.block_size ;

	   /* readed = read 'block_per_row' data blocks */
           readed = xpni_lowfsi_pread(xpni_fit_get_XPN_DATA_FD(fd),
		 		      buf_d,
				      offset_d,
				      len_d);

	   /* compute parity block */
	   for (i=0; i<block_per_row; i++)
		   buf_a[i] = buf_d+i*fmeta.block_size ;

           ret = MATH_MISC_XorN(buf_p,
                                buf_a,
                                len_a,
                                fmeta.block_size);

	   /* write '1' parity block */
           ret = xpni_lowfsi_pwrite(xpni_fit_get_XPN_MDATA_FD(fd),
                                    buf_p,
                                    offset_p,
                                    fmeta.block_size);

	   /* go to next data 'row' */
	   offset_d = offset_d + len_d ;
	}

	/* return ok */
	return (1) ;

      }

      int xpni_ft_importFile ( int fd )  
      {
        char *fmeta_fsTag;     /* File metadata (file system tag) */
        ssize_t ret;           /* Returned value from called functions */


        /* debugging */
        #if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_ft_importFile(%d); \n",
                   __FILE__,__LINE__,fd);
        #endif

        /* check params */
        if ( ! xpni_fit_is_correct(fd) )
            return (-1) ;

        /* Select file system type */
        fmeta_fsTag = xpni_fit_get_XPN_FMETA(fd).filesystem_tag ;

        if (!strncmp(fmeta_fsTag,FS_TAG_RAID5INNER,strlen(FS_TAG_RAID5INNER)))
            ret = xpni_ft_importFile_r5i(fd) ;

        else if (!strncmp(fmeta_fsTag,FS_TAG_RAID5OUTER,strlen(FS_TAG_RAID5OUTER)))
            ret = xpni_ft_importFile_r5o(fd) ;

        else 
	{
            ret = (-1);
        #if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_ft_importFile(%d,%p,%d,%d): Unknow file system tag: '%s' (not %s or %s)\n",
                   __FILE__,__LINE__,fd,buffer,(int)offset,size,fmeta_fsTag,FS_TAG_RAID5INNER,FS_TAG_RAID5OUTER);
        #endif
	}

        /* Return bytes readed */
        return ret ;
      }


   /* ................................................................... */

