
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


  /* ... Include / Inclusion ........................................... */

    #include "nfi/nfi_ops.h"
    #include "nfi.h"


  /* ... Functions / Funciones ......................................... */

  /*
   * Read the operation to realize
   */
  int nfi_do_operation ( struct nfi_worker *wrk ) 
  { 
    ssize_t aux, ret;

    //DEBUG_BEGIN() ;

    debug_info("[%s] %s(%lu) op: %d\n",__FILE__,__FUNCTION__,(unsigned long int)pthread_self(),wrk->arg.operation);

    ret = -1;
    switch (wrk->arg.operation)
    {
      case op_getattr:
        debug_info("[%s] nfi_worker_run(%lu) -> nfi_getattr\n",__FILE__,(unsigned long int)pthread_self());
        ret = wrk->server->ops->nfi_getattr(wrk->server, wrk->arg.fh, wrk->arg.attr) ;
        break;
      case op_setattr:
        debug_info("[%s] nfi_worker_run(%lu) -> nfi_setattr\n",__FILE__,(unsigned long int)pthread_self());
        ret = wrk->server->ops->nfi_setattr(wrk->server, wrk->arg.fh, wrk->arg.attr) ;
        break;
      case op_preload:
        debug_info("[%s] nfi_worker_run(%lu) -> nfi_preload\n",__FILE__,(unsigned long int)pthread_self());
        ret = wrk->server->ops->nfi_preload(wrk->server, wrk->arg.url, wrk->arg.virtual_path, wrk->arg.storage_path, wrk->arg.opt) ;
        break;
      case op_flush:
        debug_info("[%s] nfi_worker_run(%lu) -> nfi_flush\n",__FILE__,(unsigned long int)pthread_self());
        ret = wrk->server->ops->nfi_flush(wrk->server, wrk->arg.url, wrk->arg.virtual_path, wrk->arg.storage_path, wrk->arg.opt) ;
        break;
      case op_open:
        debug_info("[%s] nfi_worker_run(%lu) -> nfi_open\n",__FILE__,(unsigned long int)pthread_self());
        ret = wrk->server->ops->nfi_open(wrk->server, wrk->arg.url, wrk->arg.fh) ;
        break;
      case op_create:
        debug_info("[%s] nfi_worker_run(%lu) -> nfi_create\n",__FILE__,(unsigned long int)pthread_self());
        ret = wrk->server->ops->nfi_create(wrk->server, wrk->arg.url, wrk->arg.attr, wrk->arg.fh) ;
        break;
      case op_close:
        debug_info("[%s] nfi_worker_run(%lu) -> nfi_close\n",__FILE__,(unsigned long int)pthread_self());
        ret = wrk->server->ops->nfi_close(wrk->server, wrk->arg.fh) ;
        break;
      case op_remove:
        debug_info("[%s] nfi_worker_run(%lu) -> nfi_remove\n",__FILE__,(unsigned long int)pthread_self());
        ret = wrk->server->ops->nfi_remove(wrk->server, wrk->arg.url) ;
        break;
      case op_rename:
        debug_info("[%s] nfi_worker_run(%lu) -> nfi_rename\n",__FILE__,(unsigned long int)pthread_self());
        ret = wrk->server->ops->nfi_rename(wrk->server, wrk->arg.url, wrk->arg.newurl) ;
        break;
      case op_read:
        debug_info("[%s] nfi_worker_run(%lu) -> nfi_read\n",__FILE__,(unsigned long int)pthread_self());
        ret = 0;
        for(int i=0; i<wrk->arg.n_io;i++)
        {
          //TODO: wrk->arg.io[i].res = aux = wrk->server->ops->nfi_read(wrk->server,
          aux = wrk->server->ops->nfi_read(wrk->server, wrk->arg.fh, wrk->arg.io[i].buffer, wrk->arg.io[i].offset, wrk->arg.io[i].size) ;
          if(aux<0){
            ret = aux;
            break; //TODO: 'remove' ?????
          }

          ret = ret + aux;
          if(wrk->arg.io[i].size>(unsigned int)aux){
            break;
          }
        }
        break;
      case op_write:
        debug_info("[%s] nfi_worker_run(%lu) -> nfi_write\n",__FILE__,(unsigned long int)pthread_self());
        ret = 0;
        for(int i=0; i<wrk->arg.n_io;i++)
        {
          //TODO: wrk->arg.io[i].res = aux = wrk->server->ops->nfi_write(wrk->server,
          aux = wrk->server->ops->nfi_write(wrk->server, wrk->arg.fh, wrk->arg.io[i].buffer, wrk->arg.io[i].offset, wrk->arg.io[i].size) ;
          if(aux<0){
            ret = aux;
            break;
          }
          ret = ret + aux;
        }
        break;
      case op_mkdir:
        debug_info("[%s] nfi_worker_run(%lu) -> nfi_mkdir\n",__FILE__,(unsigned long int)pthread_self());
        ret = wrk->server->ops->nfi_mkdir(wrk->server, wrk->arg.url, wrk->arg.attr, wrk->arg.fh) ;
        break;
      case op_rmdir:
        debug_info("[%s] nfi_worker_run(%lu) -> nfi_rmdir\n",__FILE__,(unsigned long int)pthread_self());
        ret = wrk->server->ops->nfi_rmdir(wrk->server, wrk->arg.url) ;
        break;
      case op_opendir:
        debug_info("[%s] nfi_worker_run(%lu) -> nfi_opendir\n",__FILE__,(unsigned long int)pthread_self());
        ret = wrk->server->ops->nfi_opendir(wrk->server, wrk->arg.url, wrk->arg.fh) ;
        break;
      case op_readdir:
        debug_info("[%s] nfi_worker_run(%lu) -> nfi_readdir\n",__FILE__,(unsigned long int)pthread_self());
        ret = wrk->server->ops->nfi_readdir(wrk->server, wrk->arg.fh, wrk->arg.entry, wrk->arg.type) ;
        break;
      case op_closedir:
        debug_info("[%s] nfi_worker_run(%lu) -> nfi_closedir\n",__FILE__,(unsigned long int)pthread_self());
        ret = wrk->server->ops->nfi_closedir(wrk->server, wrk->arg.fh) ;
        break;
      case op_statfs:
        debug_info("[%s] nfi_worker_run(%lu) -> nfi_statfs\n",__FILE__,(unsigned long int)pthread_self());
        ret = wrk->server->ops->nfi_statfs(wrk->server, wrk->arg.inf) ;
        break;
    }

    debug_info("[%s] nfi_worker_run(%lu) -> end op: %d\n",__FILE__,(unsigned long int)pthread_self(),wrk->arg.operation);

    //DEBUG_END() ;

    return ret;
  }


  int nfi_worker_do_getattr(struct nfi_worker *wrk, struct nfi_fhandle *fh, struct nfi_attr *attr)
  {
    /* pack petition */
    wrk->arg.operation = op_getattr;
    wrk->arg.fh = fh;
    wrk->arg.attr = attr;

    debug_info("[%s] %s (%lu) with_threads = %d\n",__FILE__,__FUNCTION__,(unsigned long int)pthread_self(), wrk->thread);

    if(wrk->thread)
    {
      /* wake up thread */
      wrk->ready = 1;
      pthread_mutex_lock(&(wrk->mt)) ;
      if (wrk->ready){
        pthread_cond_signal(&(wrk->cnd)) ;
      }
      pthread_mutex_unlock(&(wrk->mt)) ;
    }
    else
    {
      wrk->arg.result = nfi_do_operation ( wrk );
    }

    return 0;
  }


  int nfi_worker_do_setattr(struct nfi_worker *wrk, struct nfi_fhandle *fh, struct nfi_attr *attr)
  {
    /* pack petition */
    wrk->arg.operation = op_setattr;
    wrk->arg.fh = fh;
    wrk->arg.attr = attr;

    debug_info("[%s] %s (%lu) with_threads = %d\n",__FILE__,__FUNCTION__,(unsigned long int)pthread_self(), wrk->thread);

    if(wrk->thread){
      /* wake up thread */
      wrk->ready = 1;
      pthread_mutex_lock(&(wrk->mt)) ;
      if (wrk->ready){
        pthread_cond_signal(&(wrk->cnd)) ;
      }
      pthread_mutex_unlock(&(wrk->mt)) ;
    }
    else
    {
      wrk->arg.result = nfi_do_operation ( wrk );
    }

    return 0;
  }


  int nfi_worker_do_flush(struct nfi_worker *wrk, char *url, char *virtual_path, char *storage_path, int opt)
  {
    /* pack petition */
    wrk->arg.operation = op_flush;
    strcpy(wrk->arg.url, url) ;
    strcpy(wrk->arg.virtual_path, virtual_path) ;
    strcpy(wrk->arg.storage_path, storage_path) ;
    wrk->arg.opt = opt;
    if(wrk->server->ops->nfi_flush == NULL)
    {
      wrk->arg.result = -1;
      return -1;
    }

    debug_info("[%s] %s (%lu) with_threads = %d\n",__FILE__,__FUNCTION__,(unsigned long int)pthread_self(), wrk->thread);

    if(wrk->thread)
    {
      /* wake up thread */
      wrk->ready = 1;
      pthread_mutex_lock(&(wrk->mt)) ;
      if (wrk->ready){
        pthread_cond_signal(&(wrk->cnd)) ;
      }
      pthread_mutex_unlock(&(wrk->mt)) ;
    }
    else
    {
      wrk->arg.result = nfi_do_operation ( wrk );
    }

    return 0;
  }


  int nfi_worker_do_preload(struct nfi_worker *wrk, char *url, char *virtual_path, char *storage_path, int opt)
  {
    /* pack petition */
    wrk->arg.operation = op_preload;
    strcpy(wrk->arg.url, url) ;
    strcpy(wrk->arg.virtual_path, virtual_path) ;
    strcpy(wrk->arg.storage_path, storage_path) ;
    wrk->arg.opt = opt;
    if(wrk->server->ops->nfi_preload == NULL)
    {
      wrk->arg.result = -1;
      return -1;
    }

    debug_info("[%s] %s (%lu) with_threads = %d\n",__FILE__,__FUNCTION__,(unsigned long int)pthread_self(), wrk->thread);

    if(wrk->thread)
    {
      /* wake up thread */
      wrk->ready = 1;
      pthread_mutex_lock(&(wrk->mt)) ;
      if (wrk->ready){
        pthread_cond_signal(&(wrk->cnd)) ;
      }
      pthread_mutex_unlock(&(wrk->mt)) ;
    }
    else
    {
      wrk->arg.result = nfi_do_operation ( wrk );
    }

    return 0;
  }


  int nfi_worker_do_open(struct nfi_worker *wrk, char *url, struct nfi_fhandle *fh)
  {
    /* pack petition */
    wrk->arg.operation = op_open;
    wrk->arg.fh = fh;
    strcpy(wrk->arg.url,url) ;

    debug_info("[%s] %s (%lu) with_threads = %d\n",__FILE__,__FUNCTION__,(unsigned long int)pthread_self(), wrk->thread);

    if(wrk->thread)
    {
      /* wake up thread */
      wrk->ready = 1;
      pthread_mutex_lock(&(wrk->mt)) ;
      if (wrk->ready){
        pthread_cond_signal(&(wrk->cnd)) ;
      }
      pthread_mutex_unlock(&(wrk->mt)) ;
    }
    else
    {
      wrk->arg.result = nfi_do_operation ( wrk );
    }

    return 0;
  }


  int nfi_worker_do_create(struct nfi_worker *wrk, char *url, struct nfi_attr *attr, struct nfi_fhandle  *fh)
  {
    /* pack petition */
    wrk->arg.operation = op_create;
    wrk->arg.fh = fh;
    strcpy(wrk->arg.url,url) ;
    wrk->arg.attr = attr;

    debug_info("[%s] %s (%lu) with_threads = %d\n",__FILE__,__FUNCTION__,(unsigned long int)pthread_self(), wrk->thread);

    if(wrk->thread)
    {
      /* wake up thread */
      wrk->ready = 1;
      pthread_mutex_lock(&(wrk->mt)) ;
      if (wrk->ready){
        pthread_cond_signal(&(wrk->cnd)) ;
      }
      pthread_mutex_unlock(&(wrk->mt)) ;

    }
    else{
      wrk->arg.result = nfi_do_operation ( wrk );
    }

    return 0;
  }


  int nfi_worker_do_close(struct nfi_worker *wrk, struct nfi_fhandle *fh)
  {
    /* pack petition */
    wrk->arg.operation = op_close;
    wrk->arg.fh = fh;

    debug_info("[%s] %s (%lu) with_threads = %d\n",__FILE__,__FUNCTION__,(unsigned long int)pthread_self(), wrk->thread);

    if(wrk->thread)
    {
      /* wake up thread */
      wrk->ready = 1;
      pthread_mutex_lock(&(wrk->mt)) ;
      if (wrk->ready){
        pthread_cond_signal(&(wrk->cnd)) ;
      }
      pthread_mutex_unlock(&(wrk->mt)) ;
    }
    else
    {
      wrk->arg.result = nfi_do_operation ( wrk );
    }

    return 0;
  }


  int nfi_worker_do_remove(struct nfi_worker *wrk, char *url)
  {
    /* pack petition */
    wrk->arg.operation = op_remove;
    strcpy(wrk->arg.url,url) ;

    debug_info("[%s] %s (%lu) with_threads = %d\n",__FILE__,__FUNCTION__,(unsigned long int)pthread_self(), wrk->thread);

    if(wrk->thread)
    {
      /* wake up thread */
      wrk->ready = 1;
      pthread_mutex_lock(&(wrk->mt)) ;
      if (wrk->ready){
        pthread_cond_signal(&(wrk->cnd)) ;
      }
      pthread_mutex_unlock(&(wrk->mt)) ;
    }
    else{
      wrk->arg.result = nfi_do_operation ( wrk );
    }

    return 0;
  }


  int nfi_worker_do_rename(struct nfi_worker *wrk, char *old_url, char *new_url)
  {
    /* pack petition */
    wrk->arg.operation = op_rename;
    strcpy(wrk->arg.url,old_url) ;
    strcpy(wrk->arg.newurl,new_url) ;

    debug_info("[%s] %s (%lu) with_threads = %d\n",__FILE__,__FUNCTION__,(unsigned long int)pthread_self(), wrk->thread);

    if(wrk->thread)
    {
      /* wake up thread */
      wrk->ready = 1;
      pthread_mutex_lock(&(wrk->mt)) ;
      if (wrk->ready){
        pthread_cond_signal(&(wrk->cnd)) ;
      }
      pthread_mutex_unlock(&(wrk->mt)) ;
    }
    else
    {
      wrk->arg.result = nfi_do_operation ( wrk );
    }

    return 0;
  }


  int nfi_worker_do_read(struct nfi_worker *wrk, struct nfi_fhandle *fh, struct nfi_worker_io *io, int n)
  {
    debug_info("[%s] %s (%lu) with_threads = %d\n",__FILE__,__FUNCTION__,(unsigned long int)pthread_self(), wrk->thread);

    if(wrk->thread)
    {
      /* wake up thread */
      pthread_mutex_lock(&(wrk->mt)) ;

      /* pack petition */
      wrk->arg.operation = op_read;
      wrk->arg.fh = fh;
      wrk->arg.io = io;
      wrk->arg.n_io = n;

      wrk->ready = 1;
      if (wrk->ready){
        pthread_cond_signal(&(wrk->cnd)) ;
      }
      pthread_mutex_unlock(&(wrk->mt)) ;
    }
    else{
      /* pack petition */
      wrk->arg.operation = op_read;
      wrk->arg.fh = fh;
      wrk->arg.io = io;
      wrk->arg.n_io = n;

      wrk->arg.result = nfi_do_operation ( wrk );
    }

    return 0;
  }


  int nfi_worker_do_write(struct nfi_worker *wrk, struct nfi_fhandle *fh, struct nfi_worker_io *io, int n)
  {
    debug_info("[%s] %s (%lu) with_threads = %d\n",__FILE__,__FUNCTION__,(unsigned long int)pthread_self(), wrk->thread);

    if(wrk->thread)
    {
      /* wake up thread */
      pthread_mutex_lock(&(wrk->mt)) ;

      /* pack petition */
      wrk->arg.operation = op_write;
      wrk->arg.fh = fh;
      wrk->arg.io = io;
      wrk->arg.n_io = n;

      wrk->ready = 1;
      if (wrk->ready){
        pthread_cond_signal(&(wrk->cnd)) ;
      }
      pthread_mutex_unlock(&(wrk->mt)) ;
    }
    else{
      /* pack petition */
      wrk->arg.operation = op_write;
      wrk->arg.fh = fh;
      wrk->arg.io = io;
      wrk->arg.n_io = n;

      wrk->arg.result = nfi_do_operation ( wrk );
    }

    return 0;
  }


  int nfi_worker_do_mkdir(struct nfi_worker *wrk, char *url, struct nfi_attr *attr, struct nfi_fhandle *fh)
  {
    /* pack petition */
    wrk->arg.operation = op_mkdir;
    strcpy(wrk->arg.url,url) ;
    wrk->arg.attr = attr;
    wrk->arg.fh = fh;

    debug_info("[%s] %s (%lu) with_threads = %d\n",__FILE__,__FUNCTION__,(unsigned long int)pthread_self(), wrk->thread);

    if(wrk->thread){
      /* wake up thread */
      wrk->ready = 1;
      pthread_mutex_lock(&(wrk->mt)) ;
      if (wrk->ready){
        pthread_cond_signal(&(wrk->cnd)) ;
      }
      pthread_mutex_unlock(&(wrk->mt)) ;
    }
    else
    {
      wrk->arg.result = nfi_do_operation ( wrk );
    }

    return 0;
  }


  int nfi_worker_do_rmdir(struct nfi_worker *wrk, char *url)
  {
    /* pack petition */
    wrk->arg.operation = op_rmdir;
    strcpy(wrk->arg.url,url) ;

    debug_info("[%s] %s (%lu) with_threads = %d\n",__FILE__,__FUNCTION__,(unsigned long int)pthread_self(), wrk->thread);

    if(wrk->thread)
    {
      /* wake up thread */
      wrk->ready = 1;
      pthread_mutex_lock(&(wrk->mt)) ;
      if (wrk->ready){
        pthread_cond_signal(&(wrk->cnd)) ;
      }
      pthread_mutex_unlock(&(wrk->mt)) ;
    }
    else{
      wrk->arg.result = nfi_do_operation ( wrk );
    }

    return 0;
  }


  int nfi_worker_do_opendir(struct nfi_worker *wrk, char *url, struct nfi_fhandle *fh)
  {
    /* pack petition */
    wrk->arg.operation = op_opendir;
    strcpy(wrk->arg.url,url);
    wrk->arg.fh = fh;

    debug_info("[%s] %s (%lu) with_threads = %d\n",__FILE__,__FUNCTION__,(unsigned long int)pthread_self(), wrk->thread);

    if(wrk->thread)
    {
      /* wake up thread */
      wrk->ready = 1;
      pthread_mutex_lock(&(wrk->mt)) ;
      if (wrk->ready){
        pthread_cond_signal(&(wrk->cnd)) ;
      }
      pthread_mutex_unlock(&(wrk->mt)) ;
    }
    else
    {
      wrk->arg.result = nfi_do_operation ( wrk );
    }

    return 0;
  }


  int nfi_worker_do_readdir(struct nfi_worker *wrk, struct nfi_fhandle *fh, char *entry, unsigned char *type)
  {
    /* pack petition */
    wrk->arg.operation = op_readdir;
    wrk->arg.type = type;
    wrk->arg.entry = entry;
    wrk->arg.fh = fh;

    debug_info("[%s] %s (%lu) with_threads = %d\n",__FILE__,__FUNCTION__,(unsigned long int)pthread_self(), wrk->thread);

    if(wrk->thread)
    {
      /* wake up thread */
      wrk->ready = 1;
      pthread_mutex_lock(&(wrk->mt)) ;
      if (wrk->ready){
        pthread_cond_signal(&(wrk->cnd)) ;
      }
      pthread_mutex_unlock(&(wrk->mt)) ;
    }
    else{
      wrk->arg.result = nfi_do_operation ( wrk );
    }

    return 0;
  }


  int nfi_worker_do_closedir(struct nfi_worker *wrk, struct nfi_fhandle *fh)
  {
    /* pack petition */
    wrk->arg.operation = op_closedir;
    wrk->arg.fh = fh;

    debug_info("[%s] %s (%lu) with_threads = %d\n",__FILE__,__FUNCTION__,(unsigned long int)pthread_self(), wrk->thread);

    if (wrk->thread)
    { 
      /* wake up thread */
      wrk->ready = 1;
      pthread_mutex_lock(&(wrk->mt)) ;
      if (wrk->ready){
        pthread_cond_signal(&(wrk->cnd)) ;
      }
      pthread_mutex_unlock(&(wrk->mt)) ;
    }
    else{
      wrk->arg.result = nfi_do_operation ( wrk );
    }

    return 0;
  }


  int nfi_worker_do_statfs(struct nfi_worker *wrk, struct nfi_info *inf)
  {
    /* pack petition */
    wrk->arg.operation = op_statfs;
    wrk->arg.inf = inf;

    debug_info("[%s] %s (%lu) with_threads = %d\n",__FILE__,__FUNCTION__,(unsigned long int)pthread_self(), wrk->thread);

    if (wrk->thread)
    {
      /* wake up thread */
      wrk->ready = 1;
      pthread_mutex_lock(&(wrk->mt)) ;
      if (wrk->ready){
        pthread_cond_signal(&(wrk->cnd)) ;
      }
      pthread_mutex_unlock(&(wrk->mt)) ;
    }
    else {
      wrk->arg.result = nfi_do_operation ( wrk );
    }

    return 0;
  }
