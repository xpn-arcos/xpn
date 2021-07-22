

   /* ... Include / Inclusion ........................................... */

      #include "xpni/xpni_ft/xpni_ft.h"


   /* ... Functions / Funciones ......................................... */

      int xpni_ft_destroy ()
      {
        /* debugging */
	#if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_ft_destroy(); \n",
                   __FILE__,__LINE__);
	#endif

	/* TODO: bug in xpn_destroy */
#if (0)
	return xpni_lowfsi_destroy();
#else
	return (1);
#endif

      }


   /* ................................................................... */

