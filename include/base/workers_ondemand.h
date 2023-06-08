/*
 *  Copyright 2020-2023 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos
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
 * @file workers_ondemand.h
 * @brief Workers On Demand.
 *
 * Header file where functions are defined to manage the creation,
 * deletion and execution of threads or workers ondemand.
 *
 * @authors Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos
 * @date  Jul 22, 2021
 * @bug No known bugs.
 */

#ifndef _WORKERS_ONDEMAND_H_
#define _WORKERS_ONDEMAND_H_

#include "all_system.h"
#include "workers_common.h"

/************************************************
 *  ... Datatypes
 ***********************************************/

/** @struct worker_ondemand_t
 *  This is a struct
 *
 *  @var worker_ondemand_t::busy_worker
 *   Indicates that the thread is busy and thus determine if you can send load or not.
 *  @var worker_ondemand_t::m_worker
 *    A 'TODO'.
 *  @var worker_ondemand_t::c_worker
 *    A 'TODO'.
 *  @var worker_ondemand_t::c_nworkers
 *    A 'TODO'.
 *  @var worker_ondemand_t::n_workers
 *    Number of workers that have been launched.
 */
typedef struct {
    // number of active threads (launch + destroy)
    int busy_worker;
    pthread_mutex_t m_worker;
    pthread_cond_t c_worker;
    pthread_cond_t c_nworkers;
    long n_workers;

} worker_ondemand_t;

/************************************************
 *  ... Functions
 ***********************************************/

/**
 * @brief Worker ondemand init.
 *
 * This function is used to create the threads.
 *
 * @param w 'TODO'.
 * @return 'TODO'.
 */
int base_worker_ondemand_init(worker_ondemand_t *w);

/**
 * @brief Worker ondemand destroy.
 *
 * Terminate all the threads before the end of the application.
 *
 * @param w 'TODO'.
 * @par Returns
 *    Nothing.
 */
void base_workers_ondemand_destroy(worker_ondemand_t *w);

/**
 * @brief Worker ondemand launch.
 *
 * A thread is executed according to an operation. That is, on demand.
 *
 * @param w 'TODO'.
 * @param th_arg 'TODO'.
 * @param worker_function 'TODO'.
 * @return 'TODO'.
 */
int base_worker_ondemand_launch(worker_ondemand_t *w, struct st_th *th_arg, void (*worker_function)(struct st_th));

/**
 * @brief Worker ondemand wait.
 *
 * Wait for a worker.
 *
 * @param th_arg 'TODO'.
 * @return 'TODO'.
 */
int base_worker_ondemand_wait(struct st_th *th_arg);

#endif
