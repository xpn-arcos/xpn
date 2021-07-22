


   /* ... Include / Inclusion ........................................... */

      #include "xpni/xpni_ft/xpni_ft.h"


   /* ... Functions / Funciones ......................................... */

      DIR   *xpni_ft_opendir ( int dd )  
      {

        DIR  *ret;


        /* debugging */
	#if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_ft_opendir(%d); \n",
                   __FILE__,__LINE__,dd);
	#endif

        /* open directory */
	ret = xpni_lowfsi_opendir(xpni_fit_get_XPN_FNAME(dd));

        /* update directory description */
        xpni_fit_set_XPN_DATA_FD(dd,(int)ret) ; /* sizeof(int) === sizeof(DIR *) */

	return ret;

      }


   /* ................................................................... */

