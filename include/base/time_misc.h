/*
 *  Copyright 2000-2023 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez
 * Garcia, Borja Bergua Guerra
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
 * @file time_misc.h
 * @brief Header file to time operations.
 *
 * Header file to perform time operations (get current time, compare times, convert, etc).
 *
 * @authors Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia,
 * Borja Bergua Guerra
 * @date  Jul 22, 2021
 * @bug No known bugs.
 */

#ifndef _TIME_MISC_H
#define _TIME_MISC_H

/************************************************
 *  ... Includes
 ***********************************************/
#include "all_system.h"

#ifdef __cplusplus
extern "C" {
#endif

/************************************************
 *  ... Constants
 ***********************************************/
#define USECPSEC 1000000

/************************************************
 *  ... Functions
 ***********************************************/

/**
 * Get a timestamp 't' at current time.
 * @param t the timestamp.
 * @par Returns
 *   Nothing.
 *
 */
void base_time_misc_timer(struct timeval *t);

/**
 * Compute 'dig = tn - to'.
 * @param to initial instant.
 * @param tn final instant.
 * @param dif the substaction value ('tn - to').
 * @par Returns
 *   Nothing.
 *
 */
void base_time_misc_diff_time(struct timeval *to, struct timeval *tn, struct timeval *dif);

/**
 * Compute 'sum = tn + to'.
 * @param to initial instant.
 * @param tn final instant.
 * @param sum the addition value ('tn + to').
 * @par Returns
 *   Nothing.
 *
 */
void base_time_misc_add_time(struct timeval *to, struct timeval *tn, struct timeval *sum);

/**
 * Return a timestamp as seconds.
 * @param timet the timestamp.
 * @return timestamp as seconds.
 */
float base_time_misc_timeval_to_seconds(struct timeval *timet);

/**
 * Return a timestamp as microseconds.
 * @param timet the timestamp.
 * @return timestamp as microseconds.
 */
float base_time_misc_timeval_to_microseconds(struct timeval *timet);

/* ................................................................... */

#ifdef __cplusplus
}
#endif

#endif /* _TIME_MISC_H */
