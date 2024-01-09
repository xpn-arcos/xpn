
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

      #include "base/dtable.h"


   /* ... Functions / Funciones ......................................... */

     /**
      * Insert the element 'dt_info' into 'dt' dynamic table.
      * @param dt a dynamic table.
      * @param dt_info the element to be inserted.
      * @return true (1) if element is inserted and error (-1) if
      *         a problem is found.
      */
      int dtable_insert
      ( 
          /*INOUT*/  dtable_t  *dt,
	  /*IN*/     void      *dt_info
      )
      {
	int8_t    ret ;
	dtelto_t *dtelto_aux ;
	 
	/* check params */
	if (NULL == dt)
		return (-1) ;
	if (NULL == dt_info)
		return (-1) ;

	/* insert element */
	if ((-1) == dt->free_list)
	{
		/* alloc a new element */
		dtelto_aux = (dtelto_t *)malloc(sizeof(dtelto_t)) ;
		if (NULL == dtelto_aux)
		{
			return (-1) ;
		}

		ret = DARRAY_InsEndDarray(&(dt->dtable),
                                          &(dt->neltos),
                                           dtelto_aux) ;
		if (0 == ret)
		{
			free(dtelto_aux) ;
			return (-1) ;
		}

		dtelto_aux->dt_info   = dt_info ;
		dtelto_aux->myself    = (dt->neltos) - 1 ;
		dtelto_aux->next_free = (-1) ;
	}
	else
	{
	        dtelto_aux    = (dtelto_t *)DARRAY_GetNFromDarray(dt->dtable,
                                                      dt->free_list) ;
		dt->free_list = dtelto_aux->next_free ;

		dtelto_aux->dt_info   = dt_info ;
		dtelto_aux->next_free = (-1) ;
	}

	/* return index of this element */
	return dtelto_aux->myself ;
      }

     /**
      * Remove the element at 'fd' position into 'dt' dynamic table.
      * @param dt a dynamic table.
      * @param fd the element descriptor.
      * @return true (1) if element is removed and error (-1) if
      *         a problem is found.
      */
      int dtable_delete  
      ( 
          /*INOUT*/  dtable_t  *dt,
          /*IN*/     int        fd
      )
      {
	dtelto_t *dtelto_aux ;
	 
	/* check params */
	if (NULL == dt)
		return (-1) ;
	if (fd >= dt->neltos)
		return (-1) ;

	/* delete element */
	dtelto_aux = (dtelto_t *)DARRAY_GetNFromDarray(dt->dtable,fd) ;
	dtelto_aux->next_free = dt->free_list ;
	dt->free_list         = dtelto_aux->myself ;

	/* return ok */
	return (1) ;
      }

     /**
      * Get the element at 'fd' position into 'dt' dynamic table.
      * @param dt a dynamic table.
      * @param fd the element descriptor.
      * @return true (1) if element is removed and error (-1) if
      *         a problem is found.
      */
      void *dtable_get  
      ( 
          /*INOUT*/  dtable_t  *dt,
          /*IN*/     int        fd
      )
      {
	dtelto_t *dtelto_aux ;

	/* check params */
	if (NULL == dt)
		return NULL ;
	if (fd >= dt->neltos)
		return NULL ;

	/* get element */
	dtelto_aux = (dtelto_t *)DARRAY_GetNFromDarray(dt->dtable,fd) ;

	/* return the information */
	return dtelto_aux->dt_info ;
      }


   /* ................................................................... */

