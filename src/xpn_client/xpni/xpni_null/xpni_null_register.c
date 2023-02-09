

   /* ... Include / Inclusion ........................................... */

      #include "xpni/xpni_null/xpni_null.h"


   /* ... Functions / Funciones ......................................... */

      int xpni_null_register   ( xpni_fsi_t *fsi )
      {

        /* debugging */
	#if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_null_register(%p); \n",
                   __FILE__,__LINE__,fsi);
	#endif

	/* check params */
	if (NULL == fsi)
		return (-1) ;

	/* register xpni_null interface */
	fsi->fsi_name   = STRING_MISC_StrDup("xpni_null") ;

	fsi->fsi_init       = xpni_null_init ;
        fsi->fsi_destroy    = xpni_null_destroy ;
        fsi->fsi_open       = xpni_null_open ;
        fsi->fsi_creat      = xpni_null_creat ;
        fsi->fsi_close      = xpni_null_close ;
        fsi->fsi_sread      = xpni_null_sread ;
        fsi->fsi_swrite     = xpni_null_swrite ;
        fsi->fsi_lseek      = xpni_null_lseek ;
        fsi->fsi_opendir    = xpni_null_opendir ;
        fsi->fsi_register   = xpni_null_register ;
        fsi->fsi_unregister = xpni_null_unregister ;
        fsi->fsi_importFile = xpni_null_importFile ;
        fsi->fsi_exportFile = xpni_null_exportFile ;

	/* return ok */
	return (1) ;

      }

      int xpni_null_unregister ( xpni_fsi_t *fsi )
      {

        /* debugging */
	#if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_null_unregister(%p); \n",
                   __FILE__,__LINE__,fsi);
	#endif

	/* check params */
	if (NULL == fsi)
		return (-1) ;

	/* unregister xpni_null interface */
	free(fsi->fsi_name) ;

	fsi->fsi_init       = NULL ;
        fsi->fsi_destroy    = NULL ;
        fsi->fsi_open       = NULL ;
        fsi->fsi_creat      = NULL ;
        fsi->fsi_close      = NULL ;
        fsi->fsi_sread      = NULL ;
        fsi->fsi_swrite     = NULL ;
        fsi->fsi_lseek      = NULL ;
        fsi->fsi_register   = NULL ;
        fsi->fsi_unregister = NULL ;
        fsi->fsi_importFile = NULL ;
        fsi->fsi_exportFile = NULL ;

	/* return ok */
	return (1) ;

      }


   /* ................................................................... */

