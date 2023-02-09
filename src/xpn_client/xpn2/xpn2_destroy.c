

   /* ... Include / Inclusion ........................................... */

      #include "xpn2.h"


   /* ... Functions / Funciones ......................................... */

      int xpn2_destroy ()
      {

        int i ;
        int ret ;


        /* Tracing */
#if defined(XPN2_TRACE)
        TRACE_MSG_PrintF(SPOT,"xpn2_destroy(); \n");
#endif

	/* Destroy each fs registered */
        for 
        (
             i=0; 
             xpni_fsit_is_correct(i);
             i++
        )
        {
		ret = xpni_fsit_FSI_DESTROY(i) ;
		if (ret < 0)
		{
#if defined(XPN2_DEBUG)
		    DEBUG_MSG_PrintF(ERROR,"xpn2_destroy(): xpni_fsit_FSI_DESTROY fails.\n") ;
#endif
		    return (-1) ;
		}

                ret = xpni_fsit_FSI_UNREGISTER(i) ;
		if (ret < 0)
		{
#if defined(XPN2_DEBUG)
		    DEBUG_MSG_PrintF(ERROR,"xpn2_destroy(): xpni_fsit_FSI_UNREGISTER fails.\n") ;
#endif
		    return (-1) ;
		}

		ret = xpni_fsit_delete(i) ;
		if (ret < 0)
		{
#if defined(XPN2_DEBUG)
		    DEBUG_MSG_PrintF(ERROR,"xpn2_destroy(): xpni_fsit_delete fails.\n") ;
#endif
		    return (-1) ;
		}
        }

        /* Return ok */
        return (1) ;

      }


   /* ................................................................... */

