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
 * @file workers.c
 * @brief File to 'TODO'.
 *
 * File to 'TODO'.
 *
 * @authors Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos
 * @date  Jul 22, 2021
 * @bug No known bugs.
 */

/************************************************
 *  ... Includes
 ***********************************************/
#include "workers.h"

/************************************************
 *  ... Functions
 ***********************************************/
int workers_init(worker_t *w, int thread_mode)
{
  // check arguments...
  if (NULL == w)
  {
    debug_error("[WORKER] worker_init with NULL worker_t\n");
    return -1;
  }

  // initialize...
  w->thread_mode = thread_mode;

  switch (w->thread_mode)
  {
  case TH_OP:
    debug_info("[WORKER] worker_ondemand_init\n");
    worker_ondemand_init(&(w->w1));
    break;

  case TH_POOL:
    debug_info("[WORKER] worker_pool_init\n");
    worker_pool_init(&(w->w2));
    break;

  case TH_NOT:
    debug_info("[WORKER] worker without threads\n");
    break;

  default:
    debug_info("[WORKER]: ERROR on thread_mode(%d).\n", w->thread_mode);
    return -1;
    break;
  }

  return 1;
}

int workers_launch(worker_t *w, struct st_th *th_arg, void (*worker_function)(struct st_th))
{
  // check arguments...
  if (NULL == w)
  {
    debug_error("[WORKER] worker_launch with NULL worker_t\n");
    return -1;
  }

  // lauch worker...
  switch (w->thread_mode)
  {
  case TH_OP:
    debug_info("[WORKER] worker_ondemand_launch\n");
    worker_ondemand_launch(&(w->w1), th_arg, worker_function);
    break;

  case TH_POOL:
    debug_info("[WORKER] worker_pool_enqueue\n");
    worker_pool_enqueue(&(w->w2), th_arg, worker_function); // Enqueue the operation on the buffer
    break;

  case TH_NOT:
    debug_info("[WORKER] worker without threads\n");
    worker_function(*th_arg);
    break;

  default:
    debug_info("[WORKER]: ERROR on thread_mode(%d).\n", w->thread_mode);
    return -1;
    break;
  }

  return 1;
}

int workers_wait(worker_t *w, struct st_th *th_arg)
{
  // check arguments...
  if (NULL == w)
  {
    debug_error("[WORKER] worker_launch_nfi with NULL worker_t\n");
    return -1;
  }

  switch (w->thread_mode)
  {
  case TH_OP:
    debug_info("[WORKER] worker_ondemand_wait\n");
    worker_ondemand_wait(th_arg);
    break;

  case TH_POOL:
    debug_info("[WORKER] worker_pool_wait\n");
    worker_pool_wait(th_arg);
    break;

  case TH_NOT:
    debug_info("[WORKER] worker_wait without threads\n");
    break;

  default:
    debug_info("[WORKER]: ERROR on thread_mode(%d).\n", w->thread_mode);
    return -1;
    break;
  }

  return 1;
}

void workers_destroy(worker_t *w)
{
  // check arguments...
  if (NULL == w)
  {
    debug_error("[WORKER] worker_destroy with NULL worker_t\n");
    return;
  }

  // destroy...
  switch (w->thread_mode)
  {
  case TH_OP:
    debug_info("[WORKER] workers_ondemand_destroy\n");
    workers_ondemand_destroy(&(w->w1));
    break;

  case TH_POOL:
    debug_info("[WORKER] worker_pool_destroy\n");
    worker_pool_destroy(&(w->w2)); // Destroy worker pool
    break;

  case TH_NOT:
    debug_info("[WORKER] worker without threads\n");
    break;

  default:
    debug_info("[WORKER]: ERROR on thread_mode(%d).\n", w->thread_mode);
    break;
  }
}

/* ................................................................... */
