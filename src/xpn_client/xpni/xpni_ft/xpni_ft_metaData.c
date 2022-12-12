

   /* ... Include / Inclusion ........................................... */

      #include "xpni/xpni_ft/xpni_ft_metaData.h"


   /* ... Functions / Funciones ......................................... */

      char *xpni_ft_metaData_getMetaDataFileName ( int fd )
      {
        char  *pfname ;
        int    pfname_len ;

        /* debugging */
        #if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_ft_metaData_getMetaDataFileName(%d); \n",
                   __FILE__,__LINE__,fd);
        #endif

        pfname_len = strlen(xpni_fit_get_XPN_FNAME(fd)) + 
                     strlen(".xpni_ft") + 
                     1 /* '\0'*/ ;

        pfname = (char *)malloc(pfname_len) ;
        if (NULL == pfname)
 	    return NULL ;

	strcpy(pfname,xpni_fit_get_XPN_FNAME(fd)) ;
	strcat(pfname,".xpni_ft") ;

        return pfname ;
      }


   /* ................................................................... */

