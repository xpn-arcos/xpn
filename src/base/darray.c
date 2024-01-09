
/*
 *  Copyright 2020-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
 *
 *  This file is part of Expand.
 *
 *  Expand is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Expand is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with Expand.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


   /* ... Include / Inclusion ........................................... */

      #include "base/darray.h"


   /* ... Functions / Funciones ......................................... */

      /**
       * Insert the element 'gptr' into the end of the dynamic array '*t'
       * that allready has '*n' elements.
       * @param t a dynamic array.
       * @param n number of elements in the dynamic array.
       * @param gptr the element that has been inserted.
       * @return true (1) if element is inserted and error (-1) if
       *         a problem is found.
       * @see 'DARRAY_InsEndDarray2' if dynamic array is NULL terminated.
       */
      int8_t       DARRAY_InsEndDarray ( /*INOUT*/ t_pointerDArray  *t,
                                         /*INOUT*/ long             *n,
                                         /*IN*/    T_POINTER         gptr )
      {
        /* check params */
        if (NULL == n) 
	    return (-1);

        /* Insert into the End of Darray */
        if ( (*t)==(t_pointerDArray)NULL )
           {
             (*t) = (t_pointerDArray) malloc (2*c_POINTER) ;
             if (NULL==(*t)) 
		 return (-1);

             (*n)    = 1 ;
             (*t)[0] = gptr ;
             (*t)[1] = (T_POINTER)NULL ;
           } 
        else  /* ( (*t) != (t_pointerDArray)NULL ) */
           {
             t_pointerDArray taux ;
      
             (*n)++ ;
             taux=(t_pointerDArray) realloc((*t),((*n)+1)*c_POINTER) ;
             if (NULL==taux) 
		 return (-1);

             (*t)=taux ;
             taux[(*n)]=(T_POINTER)NULL ;
             taux[(*n)-1]=gptr ;
           }

	/* return ok */
        return (1) ;
      }
   
      /**
       * Remove the last element from the dynamic array '*t' of
       * '*n' elements and return that element in '*delGPtr'.
       * @param t a dynamic array.
       * @param n number of elements in the dynamic array.
       * @param delGPtr element that has been removed.
       * @return true (1) if element is removed and error (-1) if
       *         a problem is found.
       * @see 'DARRAY_DelBeginDarray' to remove the first element.
       */
      int8_t       DARRAY_DelEndDarray ( /*INOUT*/ t_pointerDArray  *t,
                                         /*INOUT*/ long             *n,
                                         /*INOUT*/ T_POINTER        *delGPtr ) 
      {
        /* check params */
	if (NULL == t)
	    return (-1);
	if (NULL == (*t))
	    return (-1);
	if (NULL == n)
	    return (-1);

        /* DelEndDarray */
        if ((*n)!=0)
           {
             (*n)-- ;
             (*delGPtr)=(*t)[(*n)] ;
             (*t)[(*n)]=(T_POINTER)NULL ;
             (*t)=(t_pointerDArray)realloc(*t,((*n)+1)*c_POINTER) ;
           }
        if ((*n)==0)
           {
             free(*t) ;
	     (*t)=(t_pointerDArray)(T_POINTER)NULL;
           }

	/* return ok */
        return (1) ;
      }
   
      /**
       * Remove the first element from the dynamic array '*t' of
       * '*n' elements and return that element in '*delGPtr'.
       * @param t a dynamic array.
       * @param n number of elements in the dynamic array.
       * @param delGPtr element that has been removed.
       * @return true (1) if element is removed and error (-1) if
       *         a problem is found.
       * @see 'DARRAY_DelEndDarray' to remove the last element.
       */
      int8_t       DARRAY_DelBeginDarray ( /*INOUT*/ t_pointerDArray *t,
                                           /*INOUT*/ long            *n,
                                           /*INOUT*/ T_POINTER       *delGPtr ) 
      {
        /* check params */
	if (NULL == t)
	    return (-1);
	if (NULL == (*t))
	    return (-1);
	if (NULL == n)
	    return (-1);

        /* DelBeginDarray */
        if ((*n)!=0)
           {
             (*n)-- ;
             (*delGPtr)=(*t)[0] ;
             memmove( &((*t)[0]), 
                              &((*t)[1]),
                              (*n)*c_POINTER ) ;
             (*t)[(*n)]=(T_POINTER)NULL ;
             (*t)=(t_pointerDArray)realloc(*t,((*n)+1)*c_POINTER) ;
   
           }
        if ((*n)==0)
           {
             free(*t) ;
	     (*t)=NULL ;
           }

	/* return ok */
        return (1) ;
      }
   
      /**
       * Remove the 'orden'th element from the dynamic array '*t' of
       * '*n' elements and return that element in '*delGPtr'.
       * @param t a dynamic array.
       * @param n number of elements in the dynamic array.
       * @param orden position (begin at 0) of element to remove.
       * @param delGPtr element that has been removed.
       * @return true (1) if element is removed and error (-1) if
       *         a problem is found.
       * @see 'DARRAY_DelBeginDarray' to remove first element.
       */
      int8_t       DARRAY_DelNFromDarray ( /*IN*/    t_pointerDArray *t,
                                           /*INOUT*/ long            *n,
                                           /*IN*/    long             orden,
                                           /*INOUT*/ T_POINTER       *delGPtr )
      {
        /* check params */
	if (NULL == t)
	    return (-1);
	if (NULL == (*t))
	    return (-1);
	if (NULL == n)
	    return (-1);
	if (orden>(*n))
	    return (-1);

        /* _DelNFromDarray */
        if ((*n)!=0)
           {
             (*delGPtr)=(*t)[orden] ;
             memmove( &((*t)[orden]), 
                              &((*t)[orden+1]),
                              ((*n)-orden+1)*c_POINTER ) ;
             (*n)-- ;
             (*t)=(t_pointerDArray)realloc(*t,((*n)+1)*c_POINTER) ;
           }
        if ((*n)==0)
           {
             free(*t) ;
	     (*t) = NULL ;
           }

	/* return ok */
        return (1) ;
      }

      /**
       * Change the value of 'orden'th element from the dynamic array '*t' of
       * '*n' elements with value 'nPtr'.
       * @param t a dynamic array.
       * @param n number of elements in the dynamic array.
       * @param orden position (begin at 0) of element to be changed.
       * @param nPtr element that has been changed.
       * @return true (1) if element is changed and error (-1) if
       *         a problem is found.
       */
      int8_t       DARRAY_ChangeNFromDarray ( /*IN*/    t_pointerDArray *t,
                                              /*INOUT*/ long            *n,
                                              /*IN*/    long             orden,
                                              /*INOUT*/ T_POINTER        nPtr )
      {
        /* check params */
	if (NULL == t)
	    return (-1);
	if (NULL == (*t))
	    return (-1);
	if (NULL == n)
	    return (-1);
	if (orden>(*n))
	    return (-1);

        /* ChangeNFromDarray */
        (*t)[orden] = nPtr ;

	/* return ok */
        return (1) ;
      }
   
      /**
       * Remove all '*n' elements from the dynamic array '*t' by using
       * '*freef' function on each one.
       * @param t a dynamic array.
       * @param n number of elements in the dynamic array.
       * @param freef the free function to be used.
       * @return true (1) if elements are freeded and error (-1) if
       *         a problem is found.
       */
      int8_t       DARRAY_FreeEltosDarray ( /*INOUT*/ t_pointerDArray *t,
                                            /*INOUT*/ long     *n,
                                            /*IN*/    void (*freef)(T_POINTER) ) 
      {
        /* check params */
	if (NULL == t)
	    return (-1);
	if (NULL == n)
	    return (-1);

        /* FreeEltosDarray */
        if ((*t)!=NULL)
           {
             long     i ;
   
             for (i=0; (*t)[i]; i++) 
                 {
                   freef( (*t)[i] ) ;
                 }
             free(*t) ;
	     (*t) = NULL ;
           }
        (*n) = 0L ;

	/* return ok */
        return (1) ;
      }
   
      /**
       * Execute '*findf' function with 'gptr' as one parameter
       * and i-th element as the other parameter, for all '*n' elements
       * at the dynamic array '*t'.
       * @param t a dynamic array.
       * @param n number of elements in the dynamic array.
       * @param gptr element for first param to 'findf'.
       * @param findf function to be used to compare i-th element and 'gptr'.
       * @return the element to be find or NULL.
       */
      T_POINTER  DARRAY_FindEltoDarray ( /*IN*/     t_pointerDArray  t,  
                                         /*INOUT*/  long             n,
                                         /*IN*/     T_POINTER        gptr,
                                         /*IN*/     int8_t (*findf)(T_POINTER,T_POINTER) )
      {   
        long  i ;
 
        /* FindEltoDarray */
        for (i=0; i<n; i++)
            {
              if ((*findf)(gptr,t[i]))
                  return (t[i]) ;
            }

        return NULL ; 
      }
             

      /*  using NULL elto is end  *
       * ------------------------ */

      /**
       * Return the number of element in the 't' dynamic array,
       * in a NULL ended array.
       * @param t a dynamic array.
       * @return the number of element in the 't' array.
       */
      long         DARRAY_GetNDarray ( /*IN*/ t_pointerDArray t ) 
      {
        register long     i ;
   
	/* searching... */
        i=0 ;
        if (t)
            for(; (t[i]!=NULL); i++) ;

	/* return number of elements */
        return i ;
      }
   
      /**
       * Remove all elements from the dynamic array '*t' by using
       * '*freef' function on each one.
       * @param t a dynamic array.
       * @param freef the free function to be used.
       * @return true (1) if elements are removed and error (-1) if
       *         a problem is found.
       */
      int8_t       DARRAY_FreeEltosDarray2 ( /*INOUT*/ t_pointerDArray *t,
                                             /*IN*/    void (*freef)(T_POINTER) ) 
      {
        /* check params */
	if (NULL == t)
	    return (-1);

	/* searching... */
        if ((*t)!=NULL)
           {
             long     i ;
   
             for (i=0; (*t)[i]; i++) 
                  freef((*t)[i]) ;

             free(*t) ;
	     (*t) = NULL ;
           }

	/* return ok */
        return (1) ;
      }
   
      /**
       * Remove the element with value 'elto' from the dynamic array '*t' 
       * that is NULL ended.
       * @param t a dynamic array.
       * @param elto element that has been removed.
       * @return true (1) if element is removed and error (-1) if
       *         a problem is found.
       */
      int8_t       DARRAY_DelEltoDarray ( /*INOUT*/ t_pointerDArray *t,
                                          /*IN*/    T_POINTER elto ) 
      {
        long     c, pentry ;
   
        /* check params */
	if (NULL == (*t))
	    return (-1);
	if (NULL == elto)
	    return (-1);

	/* DelEltoDarray */
        for (c=0; ((*t)[c] && ((*t)[c]!=elto)); c++) ; 
        if ( ((*t)[c]) == (t_pointerDArray)NULL )
           {
             return (-1) ;
           }
        pentry=c ;
        c++;
        for(; ((*t)[c]!=NULL); c++) ;
        memmove( &((*t)[pentry]),
                 &((*t)[pentry+1]),
                  (c-pentry)*c_POINTER) ;
        (*t)=(t_pointerDArray)realloc(*t,c*c_POINTER) ;
        if (c==1)
           { 
             free(*t) ;
	     (*t) = NULL ;
             /* there was only a element that was deleted... and NULL, of course */
           }

	/* return ok */
        return (1) ;
      }
   
      /**
       * Insert the element 'gptr' into the end of the dynamic array '*t'
       * that is NULL ended.
       * @param t a dynamic array.
       * @param gptr element that has been inserted.
       * @return true (1) if element is inserted and error (-1) if
       *         a problem is found.
       */
      int8_t       DARRAY_InsEndDarray2 ( /*INOUT*/ t_pointerDArray *t,
                                          /*IN*/    T_POINTER gptr ) 
      {
        long     i ;    /* i is # eltos! */
   
	/* searching... */
        i=0 ;
        if ( (*t)!=(t_pointerDArray)NULL )
           {
             for (; ((*t)[i]!=NULL); i++) ;  
           }

	/* InsEndDarray */
        return DARRAY_InsEndDarray(t,&i,gptr) ;
      } 
   
      /**
       * Remove the last from the dynamic array '*t' 
       * that is NULL ended, and return that element in 'delGPtr'.
       * @param t a dynamic array.
       * @param delGPtr element that has been removed.
       * @return true (1) if element is removed and error (-1) if
       *         a problem is found.
       */
      int8_t       DARRAY_DelEndDarray2 ( /*INOUT*/ t_pointerDArray *t,
                                          /*INOUT*/ T_POINTER *delGPtr ) 
      {
        long     i ;   /* i is the position of NULL element! */
   
        /* check params */
	if (NULL == (*t))
	    return (-1);

	/* searching... */
        for(i=0; ((*t)[i]!=NULL); i++) ;

	/* DelEndDarray */
        return DARRAY_DelEndDarray(t,&i,delGPtr) ;
      } 
   
      /**
       * Return the 'orden'-th element in the 't' dynamic array,
       * in a NULL ended array.
       * @param t a dynamic array.
       * @param orden position (begin at 0) of element to return.
       * @return the element at this position or NULL.
       */
      T_POINTER  DARRAY_GetNFromDarray2 ( /*IN*/ t_pointerDArray t,
                                          /*IN*/ long            orden ) 
      {
        long     i ;
   
        /* check params */
	if (NULL == t)
	    return (T_POINTER)NULL;

	/* searching... */
        for(i=0; ((t[i]!=NULL) && (i!=orden)); i++) ;
        if (i!=orden)
             return (T_POINTER)NULL ;

	/* return element */
        return (t[orden]) ;
      }
   
   
   /* ................................................................... */

