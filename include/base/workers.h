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
 * @file workers.h
 * @brief Workers.
 *
 * Header file where functions are defined to manage the creation,
 * deletion and execution of threads or workers.
 * There are two types of threads/workers: on demand and pool.
 * An on-demand worker is executed according to an operation code
 * and a pool of workers follows the producer/consumer model of
 * operations on a buffer or intermediate queue.
 *
 * @authors Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos
 * @date  Jul 22, 2021
 * @bug No known bugs.
 */

#ifndef _WORKERS_H_
#define _WORKERS_H_

/************************************************
 *  ... Includes
 ***********************************************/
#include "all_system.h"
#include "base/debug_msg.h"
#include "base/trace_msg.h"
#include "workers_common.h"
#include "workers_ondemand.h"
#include "workers_pool.h"

/************************************************
 *  ... Constants
 ***********************************************/

// No threads are executed
#define TH_NOT 0  
// A pool of N threads is executed where there is an intermediate queue and there are producers and consumers of operations.
#define TH_POOL 1  
// A thread is created according to an operation. That is, on demand.
#define TH_OP 2  

/************************************************
 *  ... Datatype
 ***********************************************/

/** @struct worker_t
 *  This is a struct
 *
 *  @var worker_t::w1
 *    A 'TODO'.
 *  @var worker_t::w2
 *    A 'TODO'.
 *  @var worker_t::thread_mode
 *    A 'TODO'.
 */
typedef struct {
    worker_ondemand_t w1;
    worker_pool_t w2;
    int thread_mode;
} worker_t;

/************************************************
 *  ... Functions: API
 ***********************************************/

/**
 * @brief Workers init.
 *
 * This function is used to create the threads,
 * on demand or pool..., all the condition variables, mutex,
 * create all the necessary infrastructure to execute the threads/workers.
 *
 * @param w 'TODO'.
 * @param thread_mode 'TODO'.
 *
 * @return 'TODO'.
 */
int base_workers_init(worker_t *w, int thread_mode);

/**
 * @brief Workers destroy.
 *
 * Terminate all the threads before the end of the application.
 *
 * @param w 'TODO'.
 *
 * @par Returns
 *    Nothing.
 */
void base_workers_destroy(worker_t *w);

/**
 * @brief Workers launch.
 *
 * Execute function on a worker/thread, depending if it is on demand it will start the thread
 * and if it is pool directly executes the thread.
 *
 * @param w 'TODO'.
 * @param th_arg 'TODO'.
 * @param worker_function 'TODO'.
 *
 * @return 'TODO'.
 */
int base_workers_launch(worker_t *w, struct st_th *th_arg, void (*worker_function)(struct st_th));

/**
 * @brief Workers wait.
 *
 * Wait for the execution of a worker.
 *
 * @param w 'TODO'.
 * @param th_arg 'TODO'.
 *
 * @return 'TODO'.
 */
int base_workers_wait(worker_t *w, struct st_th *th_arg);

#endif
