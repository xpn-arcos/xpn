
/*
 *  Copyright 2000-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
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


#ifndef DARRAY_H
#define DARRAY_H

  #ifdef  __cplusplus
    extern "C" {
  #endif


  /* ... Include / Inclusion ........................................... */

  #include "all_system.h"
  #include <stdlib.h>


  /* ... Consts / Constantes ............................................ */

  // type sizes
  #define c_POINTER        sizeof(T_POINTER)
  #define c_POINTERTABLA   sizeof(t_pointerDArray)

  typedef 
  void *T_POINTER;

  typedef 
  T_POINTER  *t_pointerDArray; // Dynamic array, NULL end


  /* ... Data structures / Estructuras de datos ........................ */


  /* ... Functions / Funciones ......................................... */


  int8_t         DARRAY_InsEndDarray ( /*INOUT*/ t_pointerDArray *t,
                                       /*INOUT*/ long            *n,
                                       /*IN*/    T_POINTER        gptr );
  /* - Inserts 'gptr' at the end of 't', with '(*n)' eltos and
       adds one more ( (*n) = (*n) + 1 ) */
  /* - Inserta el puntero 'gptr' al final de la tabla 't'
       que tiene ya 'n' punteros. En la funcion se incrementa
       en uno 'n', pues al final, tendra un elemento mas : 'gptr' */

  int8_t         DARRAY_DelEndDarray ( /*INOUT*/ t_pointerDArray *t,
                                       /*INOUT*/ long            *n,
                                       /*INOUT*/ T_POINTER       *delGPtr );
  /* - It deletes last element of 't' and subtracts one element
       ( (*n) = (*n) - 1 ) */
  /* - Borra el ultimo elemento de la tabla 't', decrementa el
       numero de elementos en tabla ('n') y coloca en el parametro
       delGPtr el elemento borrado, por si nos interesa */

  int8_t         DARRAY_DelBeginDarray ( /*INOUT*/ t_pointerDArray *t,
                                         /*INOUT*/ long            *n,
                                         /*INOUT*/ T_POINTER       *delGPtr );
  /* - */
  /* - Borra el primer elemento de la tabla 't', decrementa el
       numero de elementos en tabla ('n') y coloca en el parametro
       delGPtr el elemento borrado, por si nos interesa */

  int8_t         DARRAY_DelNFromDarray ( /*IN*/    t_pointerDArray *t,
                                         /*INOUT*/ long            *n,
                                         /*IN*/    long             orden,
                                         /*INOUT*/ T_POINTER       *delGPtr );
  /* - */
  /* - Borra el elemento de posicion 'orden' de la tabla 't', decrementa
       el numero de elementos en tabla ('n') y coloca en el parametro
       delGPtr el elemento borrado, por si nos interesa */

  int8_t         DARRAY_ChangeNFromDarray ( /*IN*/    t_pointerDArray *t,
                                            /*INOUT*/ long            *n,
                                            /*IN*/    long             orden,
                                            /*INOUT*/ T_POINTER        nPtr );
  /* - */
  /* - Cambia Darray[orden] a 'nPtr' */

  int8_t         DARRAY_FreeEltosDarray ( /*INOUT*/ t_pointerDArray *t,
                                          /*INOUT*/ long            *n,
                                          /*IN*/    void (*freef)(T_POINTER) );
  /* - */
  /* - Se recorre la tabla, del 0..N aplicando la funcion 'freef'
       sobre cada elemento. Cuando ha terminado, hace un 'free(t)' */

  T_POINTER    DARRAY_GetNFromDarray ( /*IN*/ t_pointerDArray t,
                                       /*IN*/ long     orden );
  /* - */
  /* - Retorna el elemento que ocupa la posicion 'orden' en la tabla.
       OJO, si orden es 3, retorna t[3] (es decir, el cuarto) 
       NO comprueba que se salga de los limites del array. */

  T_POINTER    DARRAY_FindEltoDarray ( /*IN*/     t_pointerDArray  t,
                                       /*INOUT*/  long             n,
                                       /*IN*/     T_POINTER        gptr,
                                       /*IN*/     int8_t (*findf)(T_POINTER,T_POINTER) );
  /* - */
  /* - Busca el array dinamico, el primer elemento que haga que,
       (*findf)(gptr,<elemento darray) == TRUE. */


  /*  Using NULL elto as end tag  *
   * ---------------------------- */
  long           DARRAY_GetNDarray ( /*IN*/ t_pointerDArray t );
  /* - */
  /* - Retorna el numero de elementos de la tabla 't' */

  int8_t         DARRAY_FreeEltosDarray2 ( /*INOUT*/ t_pointerDArray *t,
                                           /*IN*/    void (*freef)(T_POINTER) );
  /* - */
  /* - Se recorre la tabla, del 0..N aplicando la funcion 'freef'
       sobre cada elemento. Cuando ha terminado, hace un 'free(t)' */

  int8_t         DARRAY_DelEltoDarray ( /*INOUT*/ t_pointerDArray *t,
                                        /*IN*/    T_POINTER elto );
  /* - */
  /* - Intenta borra el elemento 'elto' de la tabla 't'. Lo busca
       y si no lo encuentra retorna FALSE. Si lo encuentra lo borra
       y ajusta la tabla. No necesita el dato 'numero de elementos'
       pues en la busqueda ya recorre la tabla */

  int8_t         DARRAY_InsEndDarray2 ( /*INOUT*/ t_pointerDArray *t,
                                        /*IN*/    T_POINTER gptr ); 
  /* - */
  /* - Inserta el puntero 'gptr' al final de la tabla 't'.
       La diferencia con la anterior funcion es que, al no
       conocerse el numero de elementos, 'los cuenta', algo que
       es ineficiente. Recuerdo, la tabla de punteros termina
       con el puntero NULL */

  int8_t         DARRAY_DelEndDarray2 ( /*INOUT*/ t_pointerDArray *t,
                                        /*INOUT*/ T_POINTER *delGPtr ); 
  /* - */
  /* - Borra el ultimo elemento de la tabla 't' y coloca en el 
       parametro 'delGPtr' el elemento borrado, por si nos interesa.
       La diferencia con la anterior funcion es que, al no
       conocerse el numero de elementos, 'los cuenta', algo que
       es terriblemente ineficiente. */


  /* ................................................................... */

  #define      DARRAY_GetNFromDarray(t,orden)    \
               ( ((t)==(t_pointerDArray)NULL) ? NULL : ((t)[(orden)]) ) 


  /* ................................................................... */


  #ifdef  __cplusplus
    }
  #endif

#endif
