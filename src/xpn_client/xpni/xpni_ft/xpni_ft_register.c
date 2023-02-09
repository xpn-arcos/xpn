
   /* ... Include / Inclusion ........................................... */

      #include "xpni/xpni_ft/xpni_ft.h"


   /* ... Functions / Funciones ......................................... */

      int xpni_ft_register   ( xpni_fsi_t *fsi )
      {

        /* debugging */
        #if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_ft_register(%p); \n",
                   __FILE__,__LINE__,fsi);
        #endif

	/* check params */
	if (NULL == fsi)
		return (-1) ;

	/* register xpni_ft interface */
	fsi->fsi_name   = STRING_MISC_StrDup("xpni_ft") ;

	fsi->fsi_init       = xpni_ft_init ;
        fsi->fsi_destroy    = xpni_ft_destroy ;
        fsi->fsi_open       = xpni_ft_open ;
        fsi->fsi_creat      = xpni_ft_creat ;
        fsi->fsi_close      = xpni_ft_close ;
        fsi->fsi_sread      = xpni_ft_sread ;
        fsi->fsi_swrite     = xpni_ft_swrite ;
        fsi->fsi_lseek      = xpni_ft_lseek ;
        fsi->fsi_opendir    = xpni_ft_opendir ;
        fsi->fsi_register   = xpni_ft_register ;
        fsi->fsi_unregister = xpni_ft_unregister ;
        fsi->fsi_importFile = xpni_ft_importFile ;
        fsi->fsi_exportFile = xpni_ft_exportFile ;

	/* return ok */
	return (1) ;

      }


      int xpni_ft_unregister ( xpni_fsi_t *fsi )
      {

        /* debugging */
        #if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_ft_unregister(%p); \n",
                   __FILE__,__LINE__,fsi);
        #endif

	/* check params */
	if (NULL == fsi)
		return (-1) ;

	/* unregister xpni_ft interface */
	if (NULL != fsi->fsi_name) {
	    free(fsi->fsi_name) ;
	    fsi->fsi_name   = NULL ;
	}

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

