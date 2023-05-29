/*
 *  Copyright 2020-2023 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
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
 * @file dtable.c
 * @brief File to 'TODO'.
 *
 * File to 'TODO'.
 *
 * @authors Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
 * @date  Jul 22, 2021
 * @bug No known bugs.
 */

/************************************************
 *  ... Includes
 ***********************************************/
#include "base/dtable.h"

/************************************************
 *  ... Functions
 ***********************************************/

int dtable_insert(
	/*INOUT*/ dtable_t *dt,
	/*IN*/ void *dt_info)
{
	int8_t ret;
	dtelto_t *dtelto_aux;

	/* check params */
	if (NULL == dt)
		return (-1);
	if (NULL == dt_info)
		return (-1);

	/* insert element */
	if ((-1) == dt->free_list)
	{
		/* alloc a new element */
		dtelto_aux = (dtelto_t *)malloc(sizeof(dtelto_t));
		if (NULL == dtelto_aux)
		{
			return (-1);
		}

		ret = base_darray_insert_end(&(dt->dtable),
								  &(dt->neltos),
								  dtelto_aux);
		if (0 == ret)
		{
			free(dtelto_aux);
			return (-1);
		}

		dtelto_aux->dt_info = dt_info;
		dtelto_aux->myself = (dt->neltos) - 1;
		dtelto_aux->next_free = (-1);
	}
	else
	{
		dtelto_aux = (dtelto_t *)base_darray_get_orden_element(dt->dtable,
													   dt->free_list);
		dt->free_list = dtelto_aux->next_free;

		dtelto_aux->dt_info = dt_info;
		dtelto_aux->next_free = (-1);
	}

	/* return index of this element */
	return dtelto_aux->myself;
}

int dtable_delete(
	/*INOUT*/ dtable_t *dt,
	/*IN*/ int fd)
{
	dtelto_t *dtelto_aux;

	/* check params */
	if (NULL == dt)
		return (-1);
	if (fd >= dt->neltos)
		return (-1);

	/* delete element */
	dtelto_aux = (dtelto_t *)base_darray_get_orden_element(dt->dtable, fd);
	dtelto_aux->next_free = dt->free_list;
	dt->free_list = dtelto_aux->myself;

	/* return ok */
	return (1);
}

void *dtable_get(
	/*INOUT*/ dtable_t *dt,
	/*IN*/ int fd)
{
	dtelto_t *dtelto_aux;

	/* check params */
	if (NULL == dt)
		return NULL;
	if (fd >= dt->neltos)
		return NULL;

	/* get element */
	dtelto_aux = (dtelto_t *)base_darray_get_orden_element(dt->dtable, fd);

	/* return the information */
	return dtelto_aux->dt_info;
}

/* ................................................................... */
