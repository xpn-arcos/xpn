
/*
 *  Copyright 2020-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos
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

#include "workers_ondemand.h"


/* ... Const / Const ................................................. */


/* ... Global variables / Variables globales ........................ */


/* ... Auxiliar functions / Funciones auxiliares ......................................... */

void *worker_run ( void *arg )
{
  struct st_th th;

  debug_info("[WORKERS_ONDEMAND] [worker_run] >> Begin\n");

  // get initial pointers...
  struct st_th *th_aux = (struct st_th *)arg;
  if (NULL == th_aux)
  {
    debug_info("[WORKERS_ONDEMAND] [worker_run] ERROR: arg == NULL\n");
    return NULL;
  }

  worker_ondemand_t *w_aux = (worker_ondemand_t *)th_aux->w;
  if (NULL == w_aux)
  {
    debug_info("[WORKERS_ONDEMAND] [worker_run] ERROR: arg->w == NULL\n");
    return NULL;
  }

  struct st_th *th_shadow = (struct st_th *)(th_aux->v);
  if (NULL == th_shadow)
  {
    debug_info("[WORKERS_ONDEMAND] [worker_run] ERROR: arg->v == NULL\n");
    return NULL;
  }

  // prolog: copy arguments and update n_workers
  debug_info("[WORKERS_ONDEMAND] [worker_run] Copy arguments\n");

  pthread_mutex_lock(&(w_aux->m_worker));
  memcpy(&th, arg, sizeof(struct st_th));
  w_aux->busy_worker = FALSE;
  w_aux->n_workers++;
  pthread_cond_broadcast(&(w_aux->c_worker)); // pthread_cond_signal(&c_worker);
  pthread_mutex_unlock(&(w_aux->m_worker));

  // do function code...
  debug_info("[WORKERS_ONDEMAND] [worker_run] execute function\n");

  th.function(th);

  // do wakeup worker_ondemand_wait(...) if needed
  debug_info("[WORKERS_ONDEMAND] [worker_run] wait4me\n");

  if ( TRUE == th.wait4me )
  {
    pthread_mutex_lock(&(th_shadow->m_wait));
    th_shadow->r_wait = FALSE;
    pthread_cond_broadcast(&(th_shadow->c_wait));
    pthread_mutex_unlock(&(th_shadow->m_wait));
  }

  // epilog: update n_workers
  debug_info("[WORKERS_ONDEMAND] [worker_run] thread exit\n");

  pthread_mutex_lock(&(w_aux->m_worker));
  w_aux->n_workers--;
  pthread_cond_broadcast(&(w_aux->c_nworkers)); // pthread_cond_signal(&c_nworkers);
  pthread_mutex_unlock(&(w_aux->m_worker));

  pthread_exit(0);

  debug_info("[WORKERS_ONDEMAND] [worker_run] >> End\n");

  return NULL;
}


/* ... Functions / Funciones ......................................... */

int worker_ondemand_init ( worker_ondemand_t *w )
{
  debug_info("[WORKERS_ONDEMAND] [worker_ondemand_init] >> Begin\n");

  w->busy_worker = TRUE;
  w->n_workers   = 0L;

  pthread_cond_init (&(w->c_worker),   NULL);
  pthread_cond_init (&(w->c_nworkers), NULL);
  pthread_mutex_init(&(w->m_worker),   NULL);

  debug_info("[WORKERS_ONDEMAND] [worker_ondemand_init] >> End\n");

  return 0;
}

int worker_ondemand_launch ( worker_ondemand_t *w, struct st_th *th_arg, void (*worker_function)(struct st_th) )
{
  int ret;
  pthread_attr_t th_attr;
  pthread_t      th_worker;
  static int     th_cont = 0;

  debug_info("[WORKERS_ONDEMAND] [worker_ondemand_launch] >> Begin\n");

  pthread_attr_init(&th_attr);
  pthread_attr_setdetachstate(&th_attr, PTHREAD_CREATE_DETACHED);
  pthread_attr_setstacksize  (&th_attr, STACK_SIZE);
  w->busy_worker = TRUE;

  // prepare arguments...
  th_arg->id       = th_cont++;
  th_arg->function = worker_function;
  th_arg->w        = (void *)w;
  th_arg->v        = (void *)th_arg;

  // create thread...
  debug_info("[WORKERS_ONDEMAND] [worker_ondemand_launch] create_thread\n");

  ret = pthread_create(&th_worker, &th_attr, (void *(*)(void *))(worker_run), (void *)th_arg);
  if (ret != 0)
  {
    debug_error("[WORKERS_ONDEMAND] [worker_ondemand_launch] ERROR: pthread_create fail\n");
    perror("[WORKERS_ONDEMAND] [worker_ondemand_launch] ERROR: create_thread: ");
  }

  // wait to copy args...
  debug_info("[WORKERS_ONDEMAND] [worker_ondemand_launch] lock worker_run\n");

  pthread_mutex_lock(&(w->m_worker));
  while (w->busy_worker == TRUE)
  {
    debug_info("[WORKERS_ONDEMAND] [worker_ondemand_launch] wait worker_run\n");
    pthread_cond_wait(&(w->c_worker), &(w->m_worker));
  }

  debug_info("[WORKERS_ONDEMAND] [worker_ondemand_launch] busy_worker= TRUE worker_run\n");

  w->busy_worker = TRUE;

  debug_info("[WORKERS_ONDEMAND] [worker_ondemand_launch] unlock worker_run\n");

  pthread_mutex_unlock(&(w->m_worker));

  debug_info("[WORKERS_ONDEMAND] [worker_ondemand_launch] >> End\n");

  return 0;
}

int worker_ondemand_wait ( struct st_th *th_arg )
{
  debug_info("[WORKERS_ONDEMAND] [worker_ondemand_wait] >> Begin\n");

  pthread_mutex_lock(&(th_arg->m_wait));
  while (th_arg->r_wait == TRUE) {
    pthread_cond_wait(&(th_arg->c_wait), &(th_arg->m_wait));
  }

  th_arg->r_wait = TRUE;
  pthread_mutex_unlock(&(th_arg->m_wait));

  debug_info("[WORKERS_ONDEMAND] [worker_ondemand_wait] >> End\n");

  return 0;
}

void workers_ondemand_destroy ( worker_ondemand_t *w )
{
  debug_info("[WORKERS_ONDEMAND] [workers_ondemand_destroy] >> Begin\n");

  // wait to n_workers be zero...
  debug_info("[WORKERS_ONDEMAND] [workers_ondemand_destroy] lock workers_ondemand_wait\n");

  pthread_mutex_lock(&(w->m_worker));
  while (w->n_workers != 0)
  {
    debug_info("[WORKERS_ONDEMAND] [workers_ondemand_destroy] wait workers_ondemand_wait\n");
    pthread_cond_wait(&(w->c_nworkers), &(w->m_worker));
  }

  debug_info("[WORKERS_ONDEMAND] [workers_ondemand_destroy] unlock workers_ondemand_wait\n");
  pthread_mutex_unlock(&(w->m_worker));

  // destroy resources
  debug_info("[WORKERS_ONDEMAND] [workers_ondemand_destroy] destroy resocurces\n");

  pthread_cond_destroy  (&(w->c_worker));
  pthread_cond_destroy  (&(w->c_nworkers));
  pthread_mutex_destroy (&(w->m_worker));

  debug_info("[WORKERS_ONDEMAND] [workers_ondemand_destroy] >> End\n");
}


/* ................................................................... */
