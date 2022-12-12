

   /* ... Include / Inclusion ........................................... */

      #include "xpn2.h"


   /* ... Globals / V. Globales ......................................... */

      int (*fsi_reg[])   ( xpni_fsi_t * ) = 
                                      { 
				        xpni_null_register,   
				        xpni_ft_register,   
				        xpni_log_register, 
                                        NULL 
                                      } ;

      int (*fsi_unreg[]) ( xpni_fsi_t * ) = 
                                      { 
				        xpni_null_unregister, 
				        xpni_ft_unregister, 
				        xpni_log_unregister, 
                                        NULL 
                                      } ;


   /* ... Functions / Funciones ......................................... */

      int xpn2_init ()
      {

        int i, j ;
	int ret ;
	int fd ;


        /* Tracing */
#if defined(XPN2_TRACE)
        TRACE_MSG_PrintF(SPOT,"xpn2_init(); \n");
#endif

	/* Register all Expand-based file systems */
        for
        (
	   i=0;
	   fsi_reg[i];
	   i++
        )
        {
                fd = xpni_fsit_insertNew() ;
		if (fd < 0)
		{
#if defined(XPN2_DEBUG)
		    DEBUG_MSG_PrintF(ERROR,"xpn2_init(): xpni_fsit_insertNew fails.\n") ;
#endif
		    return (-1) ;
		}

		ret = (fsi_reg[i])(xpni_fsit_get(fd)) ;
		if (ret < 0)
		{
#if defined(XPN2_DEBUG)
		    DEBUG_MSG_PrintF(ERROR,"xpn2_init(): (fsi_reg[i])(xpni_fsit_get(fd)) fails.\n") ;
#endif
		    return (-1) ;
		}

                ret = xpni_fsit_FSI_INIT(fd) ;
		if (ret < 0)
		{
#if defined(XPN2_DEBUG)
		        DEBUG_MSG_PrintF(ERROR,"xpn2_init(): xpni_fsit_FSI_INIT fails.\n") ;
#endif
			for (j=i; j>0; j--)
			{
                        	xpni_fsit_FSI_DESTROY(fd) ;
			}

			return (-1) ;
		}
        }

	/* Return ok */
	return (1) ;

      }


   /* ................................................................... */

