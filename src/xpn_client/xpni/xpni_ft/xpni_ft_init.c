

   /* ... Include / Inclusion ........................................... */

      #include "xpni/xpni_ft/xpni_ft.h"


   /* ... Functions / Funciones ......................................... */

      int xpni_ft_init ()
      {
        /* debugging */
	#if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_ft_init(); \n",
                   __FILE__,__LINE__);
	#endif

	return xpni_lowfsi_init();
      }


   /* ................................................................... */

