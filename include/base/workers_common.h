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
 * @file workers_common.h
 * @brief Header file to 'TODO'.
 *
 * Header file to 'TODO'.
 *
 * @authors Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos
 * @date  Jul 22, 2021
 * @bug No known bugs.
 */

#ifndef _WORKERS_COMMON_H_
#define _WORKERS_COMMON_H_

/************************************************
 *  ... Includes
 ***********************************************/
#include "all_system.h"
#include "base/debug_msg.h"
#include "base/trace_msg.h"

/************************************************
 *  ... Constants
 ***********************************************/

#define MAX_THREADS 2048
#define MAX_OPERATIONS 1024
#define STACK_SIZE (256 * KB)

/************************************************
 *  ... Datatype
 ***********************************************/

/** @struct st_th
 *  This is a struct
 *
 *  @var st_th::params
 *    A 'TODO'.
 *  @var st_th::function
 *    A 'TODO'.
 *  @var st_th::id
 *    A 'TODO'.
 *  @var st_th::type_op
 *    A 'TODO'.
 *  @var st_th::rank_client_id
 *    A 'TODO'.
 *  @var st_th::sd
 *    A 'TODO'.
 *  @var st_th::w
 *    A 'TODO'.
 *  @var st_th::v
 *    A 'TODO'.
 *  @var st_th::th_worker
 *    A 'TODO'.
 *  @var st_th::m_wait
 *    A 'TODO'.
 *  @var st_th::c_wait
 *    A 'TODO'.
 *  @var st_th::r_wait
 *    A 'TODO'.
 *  @var st_th::wait4me
 *    A 'TODO'.
 */
struct st_th
{
  void *params;
  void (*function)(struct st_th);

  // server stuff
  int id;
  int type_op;
  int rank_client_id;
  long sd;

  // w: worker_ondemand/worker_pool as void *
  void *w;
  // v: original st_th as void *
  void *v;

  // client stuff
  pthread_t th_worker;
  pthread_mutex_t m_wait;
  pthread_cond_t c_wait;
  int r_wait;
  int wait4me; // (wait4me==1) ? launch + wait : launch
};

#endif
