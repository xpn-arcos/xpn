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
 * @file dtable.h
 * @brief Dynamic tables.
 *
 * Header file to execute operations (insert, delete, get) on dynamic table.
 *
 * @authors Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
 * @date  Jul 22, 2021
 * @bug No known bugs.
 */

#ifndef _DTABLE_H
#define _DTABLE_H

/************************************************
 *  ... Includes
 ***********************************************/
#include "darray.h"

#ifdef __cplusplus
extern "C"
{
#endif

   /************************************************
    *  ... Typedef
    ***********************************************/

   /** @struct dtelto_t
    *  This is a struct
    *
    *  @var dtelto_t::dt_info
    *    A 'TODO'.
    *  @var dtelto_t::next_free
    *    A 'TODO'.
    *  @var dtelto_t::myself
    *    A 'TODO'.
    */
   typedef struct
   {
      void *dt_info;
      int next_free;
      int myself;
   } dtelto_t;

   /** @struct dtable_t
    *  This is a struct
    *
    *  @var dtable_t::dtable
    *    A 'TODO'.
    *  @var dtable_t::neltos
    *    A 'TODO'.
    *  @var dtable_t::free_list
    *    A 'TODO'.
    */
   typedef struct
   {
      t_pointerDArray dtable;
      long neltos;
      long free_list;
   } dtable_t;

   /************************************************
    *  ... Constants
    ***********************************************/
#define c_DTABLE_T sizeof(dtable_t)
#define c_DTELTO_T sizeof(dtelto_t)
#define c_DTABLE_T_NULL \
   {                    \
      NULL, 0, -1       \
   }

   /************************************************
    *  ... Functions
    ***********************************************/

   /**
    * @brief Insert element.
    *
    * Insert the element 'dt_info' into 'dt' dynamic table.
    *
    * @param dt a dynamic table.
    * @param dt_info the element to be inserted.
    * @return true (1) if element is inserted and error (-1) if
    *         a problem is found.
    */
   int dtable_insert(/*INOUT*/ dtable_t *dt,
                     /*IN*/ void *dt_info);

   /**
    * @brief Remove element.
    *
    * Remove the element at 'fd' position into 'dt' dynamic table.
    *
    * @param dt a dynamic table.
    * @param fd the element descriptor.
    * @return true (1) if element is removed and error (-1) if
    *         a problem is found.
    */
   int dtable_delete(/*INOUT*/ dtable_t *dt,
                     /*IN*/ int fd);

   /**
    * @brief Get element.
    *
    * Get the element at 'fd' position into 'dt' dynamic table.
    *
    * @param dt a dynamic table.
    * @param fd the element descriptor.
    * @par Returns
    *      Nothing.
    */
   void *dtable_get(/*INOUT*/ dtable_t *dt,
                    /*IN*/ int fd);

#ifdef __cplusplus
}
#endif

#endif
