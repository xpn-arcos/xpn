/*
 *  Copyright 2000-2023 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
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

/**
 * @file darray.h
 * @brief Header file to execute operations on dynamic arrays.
 *
 * Header file to insert, delete, find, 
 * get elements from a dynamic array.
 * 
 * @authors Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
 * @date  Dec 14, 2022
 * @bug No known bugs.
 */

#ifndef DARRAY_H
#define DARRAY_H

/************************************************
 *  ... Includes
 ***********************************************/
#include "all_system.h"
#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif
     /************************************************
      *  ... Macros
      ***********************************************/
/* type sizes */
#define c_POINTER sizeof(T_POINTER)
#define c_POINTERTABLA sizeof(t_pointerDArray)

     /************************************************
      *  ... Typedef
      ***********************************************/

     /*! \var typedef void *T_POINTER
          \brief A type definition for <TODO> .

          <TODO>.
     */
     typedef void *T_POINTER;

     /*! \var typedef T_POINTER *t_pointerDArray
          \brief A type definition for <TODO>.

          Dynamic array, NULL end.
     */
     typedef T_POINTER *t_pointerDArray;

     /************************************************
      *  ... Functions
      ***********************************************/

     /**
      * @brief Insert the element 'gptr' into the end of the dynamic array '*t'
      * that allready has '*n' elements.
      *
      * Inserts 'gptr' at the end of table 't', with '(*n)' pointers and
      * adds one more ( (*n) = (*n) + 1 )
      *
      * @param t a dynamic array.
      * @param n number of elements in the dynamic array.
      * @param gptr the element that has been inserted.
      * @return true (1) if element is inserted and error (-1) if
      *         a problem is found.
      * @see 'DARRAY_InsEndDarray2' if dynamic array is NULL terminated.
      */
     int8_t DARRAY_InsEndDarray(/*INOUT*/ t_pointerDArray *t,
                                /*INOUT*/ long *n,
                                /*IN*/ T_POINTER gptr);

     /**
      * @brief Remove the last element from the dynamic array.
      *
      * It deletes last element of dynamic array 't' of '*n' elements
      * and return that element in '*delGPtr'.
      *
      * @param t a dynamic array.
      * @param n number of elements in the dynamic array.
      * @param delGPtr element that has been removed.
      * @return true (1) if element is removed and error (-1) if
      *         a problem is found.
      * @see 'DARRAY_DelBeginDarray' to remove the first element.
      */
     int8_t DARRAY_DelEndDarray(/*INOUT*/ t_pointerDArray *t,
                                /*INOUT*/ long *n,
                                /*INOUT*/ T_POINTER *delGPtr);

     /**
      * @brief Remove the first element from the dynamic array.
      *
      * The first element of the dynamic array '*t' of '*n' elements is
      * removed and return that element in '*delGPtr'.
      *
      * @param t a dynamic array.
      * @param n number of elements in the dynamic array.
      * @param delGPtr element that has been removed.
      * @return true (1) if element is removed and error (-1) if
      *         a problem is found.
      * @see 'DARRAY_DelEndDarray' to remove the last element.
      */
     int8_t DARRAY_DelBeginDarray(/*INOUT*/ t_pointerDArray *t,
                                  /*INOUT*/ long *n,
                                  /*INOUT*/ T_POINTER *delGPtr);

     /**
      * @brief Remove the 'orden'th element from the dynamic array.
      *
      * It deletes the element of position 'order' from the dynamic array '*t',
      * decrements the number of elements '*n' in the dynamic array
      * and return that element that has been removed in '*delGPtr'.
      *
      * @param t a dynamic array.
      * @param n number of elements in the dynamic array.
      * @param orden position (begin at 0) of element to remove.
      * @param delGPtr element that has been removed.
      * @return true (1) if element is removed and error (-1) if
      *         a problem is found.
      * @see 'DARRAY_DelBeginDarray' to remove first element.
      */
     int8_t DARRAY_DelNFromDarray(/*IN*/ t_pointerDArray *t,
                                  /*INOUT*/ long *n,
                                  /*IN*/ long orden,
                                  /*INOUT*/ T_POINTER *delGPtr);

     /**
      * @brief Change the value of 'orden'th element from the dynamic array.
      *
      * The value of the 'orden'-th element of the dynamic array '*t' of
      * '*n' elements is changed to a new element ('nPtr').
      *
      * @param t a dynamic array.
      * @param n number of elements in the dynamic array.
      * @param orden position (begin at 0) of element to be changed.
      * @param nPtr element that has been changed.
      * @return true (1) if element is changed and error (-1) if
      *         a problem is found.
      */
     int8_t DARRAY_ChangeNFromDarray(/*IN*/ t_pointerDArray *t,
                                     /*INOUT*/ long *n,
                                     /*IN*/ long orden,
                                     /*INOUT*/ T_POINTER nPtr);

     /**
      * @brief Remove all '*n' elements from the dynamic array.
      *
      * It goes through the dynamic array '*t', from 0..n applying
      * the function 'freef' on each element to remove it.
      * When finished, it does a 'free(t)'.
      *
      * @param t a dynamic array.
      * @param n number of elements in the dynamic array.
      * @param freef the free function to be used.
      * @return true (1) if elements are freeded and error (-1) if
      *         a problem is found.
      */
     int8_t DARRAY_FreeEltosDarray(/*INOUT*/ t_pointerDArray *t,
                                   /*INOUT*/ long *n,
                                   /*IN*/ void (*freef)(T_POINTER));

     T_POINTER DARRAY_GetNFromDarray(/*IN*/ t_pointerDArray t,
                                     /*IN*/ long orden);
     /* - */
     /* - Retorna el elemento que ocupa la posicion 'orden' en la tabla.
          OJO, si orden es 3, retorna t[3] (es decir, el cuarto)
          NO comprueba que se salga de los limites del array. */

     /**
      * @brief Find i-th element in dynamic array.
      *
      * Find in the dynamic array, the first element that makes that,
      * (*findf)(gptr,darray element) == TRUE.
      * Execute '*findf' function with 'gptr' as one parameter
      * and i-th element as the other parameter, for all '*n' elements
      * at the dynamic array '*t'.
      * Using NULL elto as end tag.
      *
      * @param t a dynamic array.
      * @param n number of elements in the dynamic array.
      * @param gptr element for first param to 'findf'.
      * @param findf function to be used to compare i-th element and 'gptr'.
      * @return the element to be find or NULL.
      */
     T_POINTER DARRAY_FindEltoDarray(/*IN*/ t_pointerDArray t,
                                     /*INOUT*/ long n,
                                     /*IN*/ T_POINTER gptr,
                                     /*IN*/ int8_t (*findf)(T_POINTER, T_POINTER));

     /**
      * @brief Number of elements in the dynamic array.
      *
      * Return the number of elements in the 't' dynamic array,
      * in a NULL ended array.
      *
      * @param t a dynamic array.
      * @return the number of element in the 't' array.
      */
     long DARRAY_GetNDarray(/*IN*/ t_pointerDArray t);

     /**
      * @brief Remove all elements from the dynamic array.
      *
      * It goes through the dynamic array '*t', from 0..n applying
      * the function 'freef' on each element to remove it.
      * When finished, it does a 'free(t)'.
      *
      * @param t a dynamic array.
      * @param freef the free function to be used.
      * @return true (1) if elements are removed and error (-1) if
      *         a problem is found.
      */
     int8_t DARRAY_FreeEltosDarray2(/*INOUT*/ t_pointerDArray *t,
                                    /*IN*/ void (*freef)(T_POINTER));

     /**
      * @brief Remove the element with value 'elto' from the dynamic array.
      *
      * Try to delete the element 'elto' from the table 't' that is NULL ended.
      * It looks for it and if it is not found it returns FALSE.
      * If it finds it, it deletes it and adjusts the table.
      * It does not need the data 'number of elements' because
      * in the search it already goes through the table.
      *
      * @param t a dynamic array.
      * @param elto element that has been removed.
      * @return true (1) if element is removed and error (-1) if
      *         a problem is found.
      */
     int8_t DARRAY_DelEltoDarray(/*INOUT*/ t_pointerDArray *t,
                                 /*IN*/ T_POINTER elto);

     /**
      * @brief Insert the element into the end of the dynamic array.
      *
      * Insert the element 'gptr' into the end of the dynamic array '*t'
      * that is NULL ended. The difference with the previous function
      * is that, not knowing the number of elements, it 'counts' them,
      * which is inefficient. Remember, the table of pointers ends
      * with the NULL pointer.
      *
      * @param t a dynamic array.
      * @param gptr element that has been inserted.
      * @return true (1) if element is inserted and error (-1) if
      *         a problem is found.
      */
     int8_t DARRAY_InsEndDarray2(/*INOUT*/ t_pointerDArray *t,
                                 /*IN*/ T_POINTER gptr);

     /**
      * @brief Remove the last element from the dynamic array.
      *
      * It deletes the last element of the table 't' that is NULL ended,
      * and places in the parameter 'delGPtr' the deleted element,
      * in case we are interested. The difference with the previous
      * function is that, not knowing the number of elements,
      * it 'counts' them, something that is inefficient.
      *
      * @param t a dynamic array.
      * @param delGPtr element that has been removed.
      * @return true (1) if element is removed and error (-1) if
      *         a problem is found.
      */
     int8_t DARRAY_DelEndDarray2(/*INOUT*/ t_pointerDArray *t,
                                 /*INOUT*/ T_POINTER *delGPtr);

#define DARRAY_GetNFromDarray(t, orden) \
     (((t) == (t_pointerDArray)NULL) ? NULL : ((t)[(orden)]))

     /* ................................................................... */

#ifdef __cplusplus
}
#endif

#endif
