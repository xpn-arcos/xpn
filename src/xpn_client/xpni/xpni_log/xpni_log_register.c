

   /* ... Include / Inclusion ........................................... */

      #include "xpni/xpni_log/xpni_log.h"
      #include "xpni/xpni_log/xpni_log_elog.h"


   /* ... Functions / Funciones ......................................... */

      int xpni_log_register   ( xpni_fsi_t *fsi )
      {

        /* debugging */
	#if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_log_register(%p); \n",
                   __FILE__,__LINE__,fsi);
	#endif

	/* check params */
	if (NULL == fsi)
		return (-1) ;

	/* register xpni_log interface */
	fsi->fsi_name   = STRING_MISC_StrDup("xpni_log") ;

	fsi->fsi_init       = xpni_log_init ;
        fsi->fsi_destroy    = xpni_log_destroy ;
        fsi->fsi_open       = xpni_log_open ;
        fsi->fsi_creat      = xpni_log_creat ;
        fsi->fsi_close      = xpni_log_close ;
        fsi->fsi_sread      = xpni_log_sread ;
        fsi->fsi_swrite     = xpni_log_swrite ;
        fsi->fsi_lseek      = xpni_log_lseek ;
        fsi->fsi_opendir    = xpni_log_opendir ;
        fsi->fsi_importFile = xpni_log_importFile ;
        fsi->fsi_exportFile = xpni_log_exportFile ;
        fsi->fsi_register   = xpni_log_register ;
        fsi->fsi_unregister = xpni_log_unregister ;


	/* return ok */
	return (1) ;

      }

      int xpni_log_unregister ( xpni_fsi_t *fsi )
      {

        /* debugging */
	#if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_log_unregister(%p); \n",
                   __FILE__,__LINE__,fsi);
	#endif

	/* check params */
	if (NULL == fsi)
		return (-1) ;

	/* unregister xpni_log interface */
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

