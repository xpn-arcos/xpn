

   /* ... Include / Inclusion ........................................... */

      #include "xpni/common/xpni_fsit.h"


   /* ... Global var / Var Globales ..................................... */

      dtable_t XPNI_FSIT_T = c_DTABLE_T_NULL;


   /* ... Functions / Funciones ......................................... */

      xpni_fsi_t  *xpni_fsit_new ( )
      {
	xpni_fsi_t *xpni_fsi_aux ;

        /* debugging */
	#if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_fsit_new(); \n",
                   __FILE__,__LINE__);
	#endif

	/* alloc a new xpni_fsi */
	xpni_fsi_aux = (xpni_fsi_t *)malloc(sizeof(xpni_fsi_t));
	if (NULL == xpni_fsi_aux)
		return NULL;

	/* set to default values */
	memset(xpni_fsi_aux,0,sizeof(xpni_fsi_t));

	/* return this new element */
	return xpni_fsi_aux ;
      }


      int xpni_fsit_insertNew ( )
      {
	xpni_fsi_t *xpni_fsi_aux ;

        /* debugging */
	#if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_fsit_insertNew(); \n",
                   __FILE__,__LINE__);
	#endif

        /* create a new element */
        xpni_fsi_aux = xpni_fsit_new() ;
	if (NULL == xpni_fsi_aux)
            return (-1) ;

	/* return index of this element */
	return dtable_insert(&XPNI_FSIT_T,(void *)xpni_fsi_aux) ;
      }

      int xpni_fsit_insert  
      ( 
	  xpni_fsi_t *xpni_fsi
      )
      {
        /* debugging */
	#if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_fsit_insert(%p); \n",
                   __FILE__,__LINE__,xpni_fsi);
	#endif

	/* return index of this element */
	return dtable_insert(&XPNI_FSIT_T,(void *)xpni_fsi) ;
      }


      int xpni_fsit_delete  
      ( 
          int fd
      )
      {
        /* debugging */
	#if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_fsit_delete(%d); \n",
                   __FILE__,__LINE__,fd);
	#endif

	/* return 'ok' */
	return dtable_delete(&XPNI_FSIT_T,fd) ;
      }


      xpni_fsi_t *xpni_fsit_get  
      ( 
          int fd
      )
      {
        /* debugging */
	#if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_fsit_get(%d); \n",
                   __FILE__,__LINE__,fd);
	#endif

	/* return element */
	return (xpni_fsi_t *)dtable_get(&XPNI_FSIT_T,fd) ;
      }


      int         xpni_fsit_getByName
      ( 
          char *fsi_name
      )
      {
        xpni_fsi_t  *dtelto;
        int          i ;

        /* debugging */
	#if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_fsit_getByName(%s); \n",
                   __FILE__,__LINE__,fsi_name);
	#endif

	/* find a element by its name */
        i=0 ;
        dtelto=(xpni_fsi_t *)dtable_get(&XPNI_FSIT_T,i) ;
        while (NULL != dtelto)
        {
             if ( ! strncmp
	            (
		      dtelto->fsi_name,
		      fsi_name,
		      strlen(dtelto->fsi_name)
		    )
		) {
                 return i;
	     }

             i++ ;
             dtelto=(xpni_fsi_t *)dtable_get(&XPNI_FSIT_T,i) ;
        }

	/* return not found */
	return (-1) ;
      }


   /* ................................................................... */

