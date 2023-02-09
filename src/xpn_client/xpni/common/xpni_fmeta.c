
   /* ... Include / Inclusion ........................................... */

      #include "xpni/common/xpni_fmeta.h"


   /* ... Functions / Funciones ......................................... */

      int xpni_fmeta_initByValues
      (
          fmeta_t  *fmeta,
          int       block_size,
          int       servers_count,
          int       start_server,
          char     *filesystem_tag,
          char     *partition_name,
          int       nerrors,
          int      *servers,
          int      *servers_status,
          int      *Ld,
          int      *Lr,
          int      *Ld2r,
          int      *Lr2d
      )
      {

        /* debugging */
	#if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_fmeta_initByValues(%p,%d,%d,%d,%s,%d,%p,%p,%p,%p,%p,%p); \n",
                   __FILE__,__LINE__,
                   fmeta,block_size,servers_count,start_server,
                   filesystem_tag,
                   nerrors,servers,servers_status,Ld,Lr,Ld2r,Lr2d);
	#endif

	/* check params */
	if (NULL == fmeta)
	     return (-1) ;

	/* set metadata information */
	fmeta->block_size     = block_size ;
	fmeta->servers_count  = servers_count ;
        fmeta->start_server   = start_server ;
	fmeta->filesystem_tag = STRING_MISC_StrDup(filesystem_tag) ;
	fmeta->partition_name = STRING_MISC_StrDup(partition_name) ;
        fmeta->nerrors        = nerrors ;
        fmeta->servers        = servers ;
        fmeta->servers_status = servers_status ;
        fmeta->Ld             = Ld ;
        fmeta->Lr             = Lr ;
        fmeta->Ld2r           = Ld2r ;
        fmeta->Lr2d           = Lr2d ;

        /* return ok */
	return (1);

      }

      int xpni_fmeta_finalize
      (
          fmeta_t *fmeta 
      )
      {

        /* debugging */
	#if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_fmeta_finalize(%p); \n",
                   __FILE__,__LINE__,fmeta);
	#endif

	/* check params */
	if (NULL == fmeta)
	     return (-1) ;

	/* free resources */
	if (NULL != fmeta->filesystem_tag)
	{
		free(fmeta->filesystem_tag) ;
		fmeta->filesystem_tag = NULL ;
	}

	if (NULL != fmeta->partition_name)
	{
		free(fmeta->partition_name) ;
		fmeta->partition_name = NULL ;
	}

        /* return ok */
	return (1);
      }

      int xpni_fmeta_changeFileSystemTag
      (
          fmeta_t *fmeta,
	  char    *new_fs_tag
      )
      {

        /* debugging */
	#if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_fmeta_changeFileSystemTag(%p,%s); \n",
                   __FILE__, __LINE__, fmeta, (new_fs_tag) ? new_fs_tag : "NULL");
	#endif

	/* check params */
	if (NULL == fmeta)
	     return (-1) ;
	if (NULL == new_fs_tag)
	     return (-1) ;

	/* free resources */
	if (NULL != fmeta->filesystem_tag)
	    free(fmeta->filesystem_tag) ;

	/* set new tag */
	fmeta->filesystem_tag = STRING_MISC_StrDup(new_fs_tag) ;

        /* return ok */
	return (1);
      }


   /* ................................................................... */

