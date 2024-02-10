
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

/* ... Include / Inclusion ........................................... */

#include "nfi/nfi_worker.h"
#include "nfi/nfi_ops.h"


/* ... Const / Const ................................................. */


/* ... Global variables / Variables globales ........................ */

static int      create_m = 0;
pthread_mutex_t global_mt;
pthread_cond_t  global_cnd;
int             global_busy;


/* ... Functions / Funciones ......................................... */

void * nfi_worker_run(struct nfi_worker * args) 
{
  struct nfi_worker * wrk;
  ssize_t ret;
  int is_true;

  debug_info("[NFI_WORKER] [nfi_worker_run] >> Begin\n");

  pthread_mutex_lock(&(global_mt));
  wrk = args;
  global_busy = 0;
  pthread_cond_signal(&(global_cnd));
  pthread_mutex_unlock(&(global_mt));

  ret = -1;
  is_true = 1;
  while (is_true) 
  {
    pthread_mutex_lock(&(wrk->mt));
    while (!(wrk->ready)) {
      pthread_cond_wait(&(wrk->cnd), &(wrk->mt));
    }

    // ret = nfi_do_operation(wrk);
    ret = 1; // TMP

    wrk->arg.result = ret;
    wrk->ready = 0;
    pthread_cond_signal(&(wrk->cnd));
    pthread_mutex_unlock(&(wrk->mt));
  }

  debug_info("[NFI_WORKER] [nfi_worker_run] >> End\n");

  return NULL;
}

int nfi_worker_init(struct nfi_worker * wrk, struct nfi_server * serv, int thread) 
{
  int ret;
  pthread_attr_t attr;

  debug_info("[NFI_WORKER] [nfi_worker_init] >> Begin\n");

  if (!global_busy) 
  {
    // init the global condition and mutex
    debug_info("[NFI_WORKER] [nfi_worker_init] Init the global condition and mutex\n");

    global_busy = 1;
    if (!create_m) 
    {
      create_m = 1;
      pthread_mutex_init(&(global_mt), NULL);
      pthread_cond_init(&(global_cnd), NULL);
    }
  }

  // init the nfi_worker
  // pointer reference to the server
  wrk->server = serv;

  thread = 1; // FIXME: Needed since the last changes in the threads architecture
  wrk->thread = thread;

  if (thread) 
  {
    pthread_mutex_init(&(wrk->mt), NULL);
    pthread_cond_init(&(wrk->cnd), NULL);

    // create the thread
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_attr_setstacksize(&attr, (256 * KB));

    debug_info("[NFI_WORKER] [nfi_worker_init] pthread_create(%lu)\n", pthread_self());

    ret = pthread_create(&(wrk->pth), &attr, (void * ( * )(void * ))(nfi_worker_run), (void * ) wrk);

    // prevent a memory leak
    pthread_detach(wrk->pth); // Although it doesn't seem to work
    pthread_attr_destroy(&attr);

    if (ret != 0) {
      perror("[NFI_WORKER] [nfi_worker_init] ERROR: pthread_create failed");
    }

    pthread_mutex_lock(&(global_mt));
    while (global_busy) {
      pthread_cond_wait(&(global_cnd), &(global_mt));
    }
    global_busy = 1;
    pthread_mutex_unlock(&(global_mt));
  }

  debug_info("[NFI_WORKER] [nfi_worker_init] >> End\n");

  return 0;
}

// OLD
ssize_t nfi_worker_wait(struct nfi_worker * wrk) 
{
  ssize_t ret = 0;

  debug_info("[NFI_WORKER] [nfi_worker_wait] >> Begin\n");

  if (wrk->thread) 
  {
    debug_info("[NFI_WORKER] [nfi_worker_wait] nfi_worker_wait(%lu) with_threads\n", pthread_self());
    pthread_mutex_lock(&(wrk->mt));

    while (wrk->ready) {
      pthread_cond_wait(&(wrk->cnd), &(wrk->mt));
    }
  }

  ret = wrk->arg.result;
  wrk->arg.result = 0;

  if (wrk->thread) 
  {
    debug_info("[NFI_WORKER] [nfi_worker_wait] nfi_worker_unlock(%lu) with_threads", pthread_self());
    pthread_mutex_unlock(&(wrk->mt));
  } 
  else {
    debug_info("[NFI_WORKER] [nfi_worker_wait] nfi_worker_wait(%lu) without_threads", pthread_self());
  }

  debug_info("[NFI_WORKER] [nfi_worker_wait] >> End\n");

  return ret;
}

int nfi_worker_end(struct nfi_worker * wrk) 
{
  debug_info("[NFI_WORKER] [nfi_worker_end] >> Begin\n");

  if (wrk->thread)
  {
    debug_info("[NFI_WORKER] [nfi_worker_wait] nfi_worker_end(%lu) with_threads", pthread_self());

    pthread_mutex_destroy(&(wrk->mt));
    pthread_cond_destroy(&(wrk->cnd));
  }

  free(wrk);
  wrk = NULL;

  debug_info("[NFI_WORKER] [nfi_worker_end] >> End\n");

  return 0;
}

int nfi_worker_destroy()
{
  debug_info("[NFI_WORKER] [nfi_worker_destroy] >> Begin\n");

  pthread_mutex_destroy(&(global_mt));
  pthread_cond_destroy(&(global_cnd));

  debug_info("[NFI_WORKER] [nfi_worker_destroy] >> End\n");

  return 0;
}

// NEW
int nfiworker_init (struct nfi_server * serv) 
{
  int ret = -1;

  debug_info("[NFI_WORKER] [nfiworker_init] >> Begin\n");

  ret = base_workers_init(&(serv->wrk->wb), serv->xpn_thread);

  debug_info("[NFI_WORKER] [nfiworker_init] >> End\n");

  return ret;
}

int nfiworker_launch (void( * worker_function)(struct st_th), struct nfi_worker * wrk) 
{
  int ret = -1;

  debug_info("[NFI_WORKER] [nfiworker_launch] >> Begin\n");

  // initialize wrk->warg...
  memset(&(wrk->warg), 0, sizeof(struct st_th));
  wrk->warg.params = (void * ) wrk;
  wrk->warg.function = worker_function;

  // wrk->warg.th_worker = <NULL>;
  pthread_mutex_init(&(wrk->warg.m_wait), NULL);
  pthread_cond_init(&(wrk->warg.c_wait), NULL);
  wrk->warg.r_wait = TRUE;
  wrk->warg.wait4me = TRUE;

  ret = base_workers_launch(&(wrk->wb), &(wrk->warg), worker_function);

  debug_info("[NFI_WORKER] [nfiworker_launch] >> End\n");

  return ret;
}

ssize_t nfiworker_wait(struct nfi_worker * wrk) 
{
  ssize_t ret;

  debug_info("[NFI_WORKER] [nfiworker_wait] >> Begin\n");

  base_workers_wait(&(wrk->wb), &(wrk->warg));
  ret = wrk->arg.result;

  debug_info("[NFI_WORKER] [nfiworker_wait] >> End\n");

  return ret;
}

void nfiworker_destroy(struct nfi_server * serv) 
{
  debug_info("[NFI_WORKER] [nfiworker_destroy] >> Begin\n");

  if (serv->xpn_thread != TH_NOT) {
    base_workers_destroy(&(serv->wrk->wb));
  }

  debug_info("[NFI_WORKER] [nfiworker_destroy] >> End\n");
}


/* ................................................................... */
