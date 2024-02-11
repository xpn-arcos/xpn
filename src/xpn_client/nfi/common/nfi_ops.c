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

#include "nfi/nfi_ops.h"


/* ... Const / Const ................................................. */


/* ... Global variables / Variables globales ........................ */


/* ... Functions / Funciones ......................................... */

//Perform the operation
void nfi_do_operation (struct st_th th_arg) 
{
  ssize_t aux, ret;

  debug_info("[TH_ID=%lu] [NFI_OPS] [nfi_do_operation] >> Begin\n", pthread_self());

  struct nfi_worker * wrk = (struct nfi_worker * )(th_arg.params);

  debug_info("[TH_ID=%lu] [NFI_OPS] [nfi_do_operation] op: %d\n", pthread_self(), wrk->arg.operation);

  ret = -1;
  switch (wrk->arg.operation) 
  {
    //File API
    case op_open:
      ret = wrk->server->ops->nfi_open(wrk->server, wrk->arg.url, wrk->arg.fh);
      break;
    case op_create:
      ret = wrk->server->ops->nfi_create(wrk->server, wrk->arg.url, wrk->arg.attr, wrk->arg.fh);
      break;
    case op_read:
      ret = 0;
      for (int i = 0; i < wrk->arg.n_io; i++) 
      {
        //TODO: wrk->arg.io[i].res = aux = wrk->server->ops->nfi_read(wrk->server,
        aux = wrk->server->ops->nfi_read(wrk->server, wrk->arg.fh, wrk->arg.io[i].buffer, wrk->arg.io[i].offset+XPN_HEADER_SIZE, wrk->arg.io[i].size);
        if (aux < 0) 
        {
          ret = aux;
          break;
        }

        ret = ret + aux;

        // Remove???
        /*if(wrk->arg.io[i].size > (unsigned int)aux){
          break;
        }*/
      }
      break;
    case op_write:
      ret = 0;
      for (int i = 0; i < wrk->arg.n_io; i++) 
      {
        //TODO: wrk->arg.io[i].res = aux = wrk->server->ops->nfi_write(wrk->server,
        aux = wrk->server->ops->nfi_write(wrk->server, wrk->arg.fh, wrk->arg.io[i].buffer, wrk->arg.io[i].offset+XPN_HEADER_SIZE, wrk->arg.io[i].size);
        if (aux < 0) 
        {
          ret = aux;
          break;
        }
        ret = ret + aux;
      }
      break;
    case op_close:
      ret = wrk->server->ops->nfi_close(wrk->server, wrk->arg.fh);
      break;
    case op_remove:
      ret = wrk->server->ops->nfi_remove(wrk->server, wrk->arg.url);
      break;
    case op_rename:
      ret = wrk->server->ops->nfi_rename(wrk->server, wrk->arg.url, wrk->arg.newurl);
      break;
    case op_getattr:
      ret = wrk->server->ops->nfi_getattr(wrk->server, wrk->arg.fh, wrk->arg.attr);
      break;
    case op_setattr:
      ret = wrk->server->ops->nfi_setattr(wrk->server, wrk->arg.fh, wrk->arg.attr);
      break;

    //Directory API
    case op_mkdir:
      ret = wrk->server->ops->nfi_mkdir(wrk->server, wrk->arg.url, wrk->arg.attr, wrk->arg.fh);
      break;
    case op_opendir:
      ret = wrk->server->ops->nfi_opendir(wrk->server, wrk->arg.url, wrk->arg.fh);
      break;
    case op_readdir:
      ret = wrk->server->ops->nfi_readdir(wrk->server, wrk->arg.fh, wrk->arg.entry);
      break;
    case op_closedir:
      ret = wrk->server->ops->nfi_closedir(wrk->server, wrk->arg.fh);
      break;
    case op_rmdir:
      ret = wrk->server->ops->nfi_rmdir(wrk->server, wrk->arg.url);
      break;

    // Import / Export API
    case op_preload:
      ret = wrk->server->ops->nfi_preload(wrk->server, wrk->arg.url, wrk->arg.virtual_path, wrk->arg.storage_path, wrk->arg.opt);
      break;
    case op_flush:
      ret = wrk->server->ops->nfi_flush(wrk->server, wrk->arg.url, wrk->arg.virtual_path, wrk->arg.storage_path, wrk->arg.opt);
      break;
    
    //FS API
    case op_statfs:
      ret = wrk->server->ops->nfi_statfs(wrk->server, wrk->arg.inf);
      break;
  }

  wrk->arg.result = ret;

  debug_info("[TH_ID=%lu] [NFI_OPS] [nfi_do_operation] >> End\n", pthread_self());
}


// File API
int nfi_worker_do_open (struct nfi_worker * wrk, char * url, struct nfi_fhandle * fh) 
{
  debug_info("[TH_ID=%lu] [NFI_OPS] [nfi_worker_do_open] >> Begin\n", pthread_self());

  // Pack request
  wrk->arg.operation = op_open;
  wrk->arg.fh = fh;
  strcpy(wrk->arg.url, url);

  // Do operation
  nfiworker_launch(nfi_do_operation, wrk);

  debug_info("[TH_ID=%lu] [NFI_OPS] [nfi_worker_do_open] >> End\n", pthread_self());

  return 0;
}

int nfi_worker_do_create (struct nfi_worker * wrk, char * url, struct nfi_attr * attr, struct nfi_fhandle * fh) 
{
  debug_info("[TH_ID=%lu] [NFI_OPS] [nfi_worker_do_create] >> Begin\n", pthread_self());

  // Pack request
  wrk->arg.operation = op_create;
  wrk->arg.fh = fh;
  wrk->arg.attr = attr;
  strcpy(wrk->arg.url, url);

  // Do operation
  nfiworker_launch(nfi_do_operation, wrk);

  debug_info("[TH_ID=%lu] [NFI_OPS] [nfi_worker_do_create] >> End\n", pthread_self());

  return 0;
}

int nfi_worker_do_read (struct nfi_worker * wrk, struct nfi_fhandle * fh, struct nfi_worker_io * io, int n) 
{
  debug_info("[TH_ID=%lu] [NFI_OPS] [nfi_worker_do_read] >> Begin\n", pthread_self());

  // Pack request
  wrk->arg.operation = op_read;
  wrk->arg.fh = fh;
  wrk->arg.io = io;
  wrk->arg.n_io = n;

  // Do operation
  nfiworker_launch(nfi_do_operation, wrk);

  debug_info("[TH_ID=%lu] [NFI_OPS] [nfi_worker_do_read] >> End\n", pthread_self());

  return 0;
}

int nfi_worker_do_write (struct nfi_worker * wrk, struct nfi_fhandle * fh, struct nfi_worker_io * io, int n) 
{
  debug_info("[TH_ID=%lu] [NFI_OPS] [nfi_worker_do_write] >> Begin\n", pthread_self());

  // Pack request
  wrk->arg.operation = op_write;
  wrk->arg.fh = fh;
  wrk->arg.io = io;
  wrk->arg.n_io = n;

  // Do operation
  nfiworker_launch(nfi_do_operation, wrk);

  debug_info("[TH_ID=%lu] [NFI_OPS] [nfi_worker_do_write] >> End\n", pthread_self());

  return 0;
}

int nfi_worker_do_close (struct nfi_worker * wrk, struct nfi_fhandle * fh) 
{
  debug_info("[TH_ID=%lu] [NFI_OPS] [nfi_worker_do_close] >> Begin\n", pthread_self());

  // Pack request
  wrk->arg.operation = op_close;
  wrk->arg.fh = fh;

  // Do operation
  nfiworker_launch(nfi_do_operation, wrk);

  debug_info("[TH_ID=%lu] [NFI_OPS] [nfi_worker_do_close] >> End\n", pthread_self());

  return 0;
}

int nfi_worker_do_remove (struct nfi_worker * wrk, char * url) 
{
  debug_info("[TH_ID=%lu] [NFI_OPS] [nfi_worker_do_remove] >> Begin\n", pthread_self());

  // Pack request
  wrk->arg.operation = op_remove;
  strcpy(wrk->arg.url, url);

  // Do operation
  nfiworker_launch(nfi_do_operation, wrk);

  debug_info("[TH_ID=%lu] [NFI_OPS] [nfi_worker_do_remove] >> End\n", pthread_self());

  return 0;
}

int nfi_worker_do_rename (struct nfi_worker * wrk, char * old_url, char * new_url) 
{
  debug_info("[TH_ID=%lu] [NFI_OPS] [nfi_worker_do_rename] >> Begin\n", pthread_self());

  // Pack request
  wrk->arg.operation = op_rename;
  strcpy(wrk->arg.url, old_url);
  strcpy(wrk->arg.newurl, new_url);

  // Do operation
  nfiworker_launch(nfi_do_operation, wrk);

  debug_info("[TH_ID=%lu] [NFI_OPS] [nfi_worker_do_rename] >> End\n", pthread_self());

  return 0;
}

int nfi_worker_do_getattr (struct nfi_worker * wrk, struct nfi_fhandle * fh, struct nfi_attr * attr) 
{
  debug_info("[TH_ID=%lu] [NFI_OPS] [nfi_worker_do_getattr] >> Begin\n", pthread_self());

  // Pack request
  wrk->arg.operation = op_getattr;
  wrk->arg.fh = fh;
  wrk->arg.attr = attr;

  // Do operation
  nfiworker_launch(nfi_do_operation, wrk);

  debug_info("[TH_ID=%lu] [NFI_OPS] [nfi_worker_do_getattr] >> End\n", pthread_self());

  return 0;
}

int nfi_worker_do_setattr (struct nfi_worker * wrk, struct nfi_fhandle * fh, struct nfi_attr * attr) 
{

  debug_info("[TH_ID=%lu] [NFI_OPS] [nfi_worker_do_setattr] >> Begin\n", pthread_self());

  // Pack request
  wrk->arg.operation = op_setattr;
  wrk->arg.fh = fh;
  wrk->arg.attr = attr;

  // Do operation
  nfiworker_launch(nfi_do_operation, wrk);

  debug_info("[TH_ID=%lu] [NFI_OPS] [nfi_worker_do_setattr] >> End\n", pthread_self());

  return 0;
}


//Directory API
int nfi_worker_do_mkdir (struct nfi_worker * wrk, char * url, struct nfi_attr * attr, struct nfi_fhandle * fh) 
{
  debug_info("[TH_ID=%lu] [NFI_OPS] [nfi_worker_do_mkdir] >> Begin\n", pthread_self());

  // Pack request
  wrk->arg.fh = fh;
  wrk->arg.attr = attr;
  wrk->arg.operation = op_mkdir;
  strcpy(wrk->arg.url, url);

  // Do operation
  nfiworker_launch(nfi_do_operation, wrk);

  debug_info("[TH_ID=%lu] [NFI_OPS] [nfi_worker_do_mkdir] >> End\n", pthread_self());

  return 0;
}

int nfi_worker_do_opendir (struct nfi_worker * wrk, char * url, struct nfi_fhandle * fh) 
{
  debug_info("[TH_ID=%lu] [NFI_OPS] [nfi_worker_do_opendir] >> Begin\n", pthread_self());

  // Pack request
  wrk->arg.operation = op_opendir;
  strcpy(wrk->arg.url, url);
  wrk->arg.fh = fh;

  // Do operation
  nfiworker_launch(nfi_do_operation, wrk);

  debug_info("[TH_ID=%lu] [NFI_OPS] [nfi_worker_do_opendir] >> End\n", pthread_self());

  return 0;
}

int nfi_worker_do_readdir (struct nfi_worker * wrk, struct nfi_fhandle * fh, struct dirent * entry) 
{
  debug_info("[TH_ID=%lu] [NFI_OPS] [nfi_worker_do_readdir] >> Begin\n", pthread_self());

  // Pack request
  wrk->arg.operation = op_readdir;
  wrk->arg.entry = entry;
  wrk->arg.fh = fh;

  // Do operation
  nfiworker_launch(nfi_do_operation, wrk);

  debug_info("[TH_ID=%lu] [NFI_OPS] [nfi_worker_do_readdir] >> End\n", pthread_self());

  return 0;
}

int nfi_worker_do_closedir (struct nfi_worker * wrk, struct nfi_fhandle * fh) 
{
  debug_info("[TH_ID=%lu] [NFI_OPS] [nfi_worker_do_closedir] >> Begin\n", pthread_self());

  // Pack request
  wrk->arg.fh = fh;
  wrk->arg.operation = op_closedir;

  // Do operation
  nfiworker_launch(nfi_do_operation, wrk);

  debug_info("[TH_ID=%lu] [NFI_OPS] [nfi_worker_do_closedir] >> End\n", pthread_self());

  return 0;
}

int nfi_worker_do_rmdir (struct nfi_worker * wrk, char * url) 
{
  debug_info("[TH_ID=%lu] [NFI_OPS] [nfi_worker_do_rmdir] >> Begin\n", pthread_self());

  // Pack request
  wrk->arg.operation = op_rmdir;
  strcpy(wrk->arg.url, url);

  // Do operation
  nfiworker_launch(nfi_do_operation, wrk);

  debug_info("[TH_ID=%lu] [NFI_OPS] [nfi_worker_do_rmdir] >> End\n", pthread_self());

  return 0;
}

//Import / Export API
int nfi_worker_do_flush (struct nfi_worker * wrk, char * url, char * virtual_path, char * storage_path, int opt) 
{
  debug_info("[TH_ID=%lu] [NFI_OPS] [nfi_worker_do_flush] >> Begin\n", pthread_self());

  if (wrk->server->ops->nfi_flush == NULL) 
  {
    wrk->arg.result = -1;
    return -1;
  }

  // Pack request
  wrk->arg.operation = op_flush;
  strcpy(wrk->arg.url, url);
  strcpy(wrk->arg.virtual_path, virtual_path);
  strcpy(wrk->arg.storage_path, storage_path);
  wrk->arg.opt = opt;

  // Do operation
  nfiworker_launch(nfi_do_operation, wrk);

  debug_info("[TH_ID=%lu] [NFI_OPS] [nfi_worker_do_flush] >> End\n", pthread_self());

  return 0;
}

int nfi_worker_do_preload (struct nfi_worker * wrk, char * url, char * virtual_path, char * storage_path, int opt) 
{
  debug_info("[TH_ID=%lu] [NFI_OPS] [nfi_worker_do_preload] >> Begin\n", pthread_self());

  if (wrk->server->ops->nfi_preload == NULL) 
  {
    wrk->arg.result = -1;
    return -1;
  }

  // Pack request
  wrk->arg.operation = op_preload;
  strcpy(wrk->arg.url, url);
  strcpy(wrk->arg.virtual_path, virtual_path);
  strcpy(wrk->arg.storage_path, storage_path);
  wrk->arg.opt = opt;

  // Do operation
  nfiworker_launch(nfi_do_operation, wrk);

  debug_info("[TH_ID=%lu] [NFI_OPS] [nfi_worker_do_preload] >> End\n", pthread_self());

  return 0;
}

//FS API
int nfi_worker_do_statfs (struct nfi_worker * wrk, struct nfi_info * inf) 
{
  debug_info("[TH_ID=%lu] [NFI_OPS] [nfi_worker_do_statfs] >> Begin\n", pthread_self());

  // Pack request
  wrk->arg.operation = op_statfs;
  wrk->arg.inf = inf;

  // Do operation
  nfiworker_launch(nfi_do_operation, wrk);

  debug_info("[TH_ID=%lu] [NFI_OPS] [nfi_worker_do_statfs] >> End\n", pthread_self());

  return 0;
}


/* ................................................................... */
