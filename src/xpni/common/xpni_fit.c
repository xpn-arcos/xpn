

   /* ... Include / Inclusion ........................................... */

      #include "xpni/common/xpni_fit.h"


   /* ... Global var / Var Globales ..................................... */

      dtable_t XPNI_FIT_T = c_DTABLE_T_NULL;


   /* ... Functions / Funciones ......................................... */

      xpni_fi_t  *xpni_fit_new ( )
      {
	xpni_fi_t *xpni_fi_aux ;

        /* debugging */
	#if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_fit_new(); \n",
                   __FILE__,__LINE__);
	#endif

        /* alloc a new xpni_fi */
	xpni_fi_aux = (xpni_fi_t *)malloc(sizeof(xpni_fi_t));
	if (NULL == xpni_fi_aux)
		return NULL;

        /* set to default values */
        memset(xpni_fi_aux,0,sizeof(xpni_fi_t));

	xpni_fi_aux->xpni_fsd  = -1 ;
	xpni_fi_aux->xpn_fd[0] = -1 ;
	xpni_fi_aux->xpn_fd[1] = -1 ;

        /* return this new element */
	return xpni_fi_aux ;
      }


      int xpni_fit_insertNew ( )
      {
        xpni_fi_t *xpni_fi_aux ;

        /* debugging */
	#if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_fit_insertNew(); \n",
                   __FILE__,__LINE__);
	#endif

        /* create a new element */
        xpni_fi_aux = xpni_fit_new() ;
	if (NULL == xpni_fi_aux)
		return (-1);

	/* return index of this element */
	return dtable_insert(&XPNI_FIT_T,(void *)xpni_fi_aux) ;
      }

      int xpni_fit_insert  
      ( 
	  xpni_fi_t *xpni_fi
      )
      {
        /* debugging */
	#if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_fit_insert(%p); \n",
                   __FILE__,__LINE__,xpni_fi);
	#endif

	/* return index of this element */
	return dtable_insert(&XPNI_FIT_T,(void *)xpni_fi) ;
      }


      int xpni_fit_delete  
      ( 
          int fd
      )
      {
        /* debugging */
	#if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_fit_delete(%d); \n",
                   __FILE__,__LINE__,fd);
	#endif

	/* return 'ok' */
	return dtable_delete(&XPNI_FIT_T,fd) ;
      }


      xpni_fi_t *xpni_fit_get  
      ( 
          int fd
      )
      {
        /* debugging */
	#if defined(XPNI_DEBUG)
            printf("[%s:%d] xpni_fit_get(%d); \n",
                   __FILE__,__LINE__,fd);
	#endif

	/* return element */
	return (xpni_fi_t *)dtable_get(&XPNI_FIT_T,fd) ;
      }


   /* ................................................................... */

