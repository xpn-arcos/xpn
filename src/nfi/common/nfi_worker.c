
#include "expand.h"
#include "nfi/nfi_worker.h"


#ifdef POSIX_THREADS
static int create_m = 0;
pthread_mutex_t global_mt;
pthread_cond_t global_cnd;
int global_busy;
#endif

#ifdef POSIX_THREADS
void *nfi_worker_run ( struct nfi_worker *args )
{
  struct nfi_worker *wrk;
  ssize_t aux, ret;
  int i;

  pthread_mutex_lock(&(global_mt)) ;
  wrk = args;
  global_busy = 0;
  pthread_cond_signal(&(global_cnd)) ;
  pthread_mutex_unlock(&(global_mt)) ;

  while(1){
	  pthread_mutex_lock(&(wrk->mt)) ;
	  while(!(wrk->ready))
		  pthread_cond_wait(&(wrk->cnd),
				    &(wrk->mt)) ;

	  XPN_DEBUG("[NFI_WORKER] nfi_worker_run(%lu) op: %d",(unsigned long int)pthread_self(),wrk->arg.operation)

	  switch (wrk->arg.operation){
		  case op_getattr:
			  XPN_DEBUG("[NFI_WORKER] nfi_worker_run(%lu) -> nfi_getattr",(unsigned long int)pthread_self())
			  ret = wrk->server->ops->nfi_getattr(wrk->server,
					   		wrk->arg.fh,
							wrk->arg.attr) ;
			  break;
		  case op_setattr:
			  XPN_DEBUG("[NFI_WORKER] nfi_worker_run(%lu) -> nfi_setattr",(unsigned long int)pthread_self())
			  ret = wrk->server->ops->nfi_setattr(wrk->server,
					   		wrk->arg.fh,
							wrk->arg.attr) ;
			  break;
		  case op_preload:
			  XPN_DEBUG("[NFI_WORKER] nfi_worker_run(%lu) -> nfi_preload",(unsigned long int)pthread_self())
			  ret = wrk->server->ops->nfi_preload(wrk->server,
					   		wrk->arg.url,
					   		wrk->arg.virtual_path,
					   		wrk->arg.storage_path,
							wrk->arg.opt) ;
			  break;
		  case op_flush:
			  XPN_DEBUG("[NFI_WORKER] nfi_worker_run(%lu) -> nfi_flush",(unsigned long int)pthread_self())
			  ret = wrk->server->ops->nfi_flush(wrk->server,
					   		wrk->arg.url,
					   		wrk->arg.virtual_path,
					   		wrk->arg.storage_path,
							wrk->arg.opt) ;
			  break;
		  case op_open:
			  XPN_DEBUG("[NFI_WORKER] nfi_worker_run(%lu) -> nfi_open",(unsigned long int)pthread_self())
			  ret = wrk->server->ops->nfi_open(wrk->server,
					   		wrk->arg.url,
							wrk->arg.fh) ;
			  break;
		  case op_create:
			  XPN_DEBUG("[NFI_WORKER] nfi_worker_run(%lu) -> nfi_create",(unsigned long int)pthread_self())
			  ret = wrk->server->ops->nfi_create(wrk->server,
					   		wrk->arg.url,
							wrk->arg.attr,
							wrk->arg.fh) ;
			  break;
		  case op_close:
			  XPN_DEBUG("[NFI_WORKER] nfi_worker_run(%lu) -> nfi_close",(unsigned long int)pthread_self())
			  ret = wrk->server->ops->nfi_close(wrk->server,
					   		wrk->arg.fh) ;
			  break;
		  case op_remove:
			  XPN_DEBUG("[NFI_WORKER] nfi_worker_run(%lu) -> nfi_remove",(unsigned long int)pthread_self())
			  ret = wrk->server->ops->nfi_remove(wrk->server,
					   		wrk->arg.url) ;
			  break;
		  case op_rename:
			  XPN_DEBUG("[NFI_WORKER] nfi_worker_run(%lu) -> nfi_rename",(unsigned long int)pthread_self())
			  ret = wrk->server->ops->nfi_rename(wrk->server,
					   		wrk->arg.url,
							wrk->arg.newurl) ;
			  break;
		  case op_read:
			  XPN_DEBUG("[NFI_WORKER] nfi_worker_run(%lu) -> nfi_read",(unsigned long int)pthread_self())
			  ret = 0;
			  for(i=0; i<wrk->arg.n_io;i++){
				 //TODO: wrk->arg.io[i].res = aux = wrk->server->ops->nfi_read(wrk->server,
				 aux = wrk->server->ops->nfi_read(wrk->server,
					   		wrk->arg.fh,
							wrk->arg.io[i].buffer,
							wrk->arg.io[i].offset,
							wrk->arg.io[i].size) ;

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
			  XPN_DEBUG("[NFI_WORKER] nfi_worker_run(%lu) -> nfi_write",(unsigned long int)pthread_self())
			  ret = 0;
			  for(i=0; i<wrk->arg.n_io;i++){
				 //TODO: wrk->arg.io[i].res = aux = wrk->server->ops->nfi_write(wrk->server,
			  	aux = wrk->server->ops->nfi_write(wrk->server,
					   		wrk->arg.fh,
							wrk->arg.io[i].buffer,
							wrk->arg.io[i].offset,
							wrk->arg.io[i].size) ;
			   	if(aux<0){
					ret = aux;
					break;
				}
			   	ret = ret + aux;
			  }
			  break;
		  case op_mkdir:
			  XPN_DEBUG("[NFI_WORKER] nfi_worker_run(%lu) -> nfi_mkdir",(unsigned long int)pthread_self())
			  ret = wrk->server->ops->nfi_mkdir(wrk->server,
					   		wrk->arg.url,
							wrk->arg.attr,
							wrk->arg.fh) ;
			  break;
		  case op_rmdir:
			  XPN_DEBUG("[NFI_WORKER] nfi_worker_run(%lu) -> nfi_rmdir",(unsigned long int)pthread_self())
			  ret = wrk->server->ops->nfi_rmdir(wrk->server,
					   		wrk->arg.url) ;
			  break;
		  case op_opendir:
			  XPN_DEBUG("[NFI_WORKER] nfi_worker_run(%lu) -> nfi_opendir",(unsigned long int)pthread_self())
			  ret = wrk->server->ops->nfi_opendir(wrk->server,
					   		wrk->arg.url,
							wrk->arg.fh) ;
			  break;
		  case op_readdir:
			  XPN_DEBUG("[NFI_WORKER] nfi_worker_run(%lu) -> nfi_readdir",(unsigned long int)pthread_self())
			  ret = wrk->server->ops->nfi_readdir(wrk->server,
					   		wrk->arg.fh,   
							wrk->arg.entry,
							wrk->arg.type) ;
			  break;
		  case op_closedir:
			  XPN_DEBUG("[NFI_WORKER] nfi_worker_run(%lu) -> nfi_closedir",(unsigned long int)pthread_self())
			  ret = wrk->server->ops->nfi_closedir(wrk->server,
					   		wrk->arg.fh) ;
			  break;
		  case op_statfs:
			  XPN_DEBUG("[NFI_WORKER] nfi_worker_run(%lu) -> nfi_statfs",(unsigned long int)pthread_self())
			  ret = wrk->server->ops->nfi_statfs(wrk->server,
					   		wrk->arg.inf) ;
			  break;
	  }

	  XPN_DEBUG("[NFI_WORKER] nfi_worker_run(%lu) -> end op: %d",(unsigned long int)pthread_self(),wrk->arg.operation)

	  wrk->arg.result = ret;
	  wrk->ready = 0;
	  pthread_cond_signal(&(wrk->cnd)) ;
          pthread_mutex_unlock(&(wrk->mt)) ;
  }

  return NULL ;
}
#endif



int nfi_worker_thread ( struct nfi_worker *wrk, int thread )
{
    wrk->thread = thread;

    return wrk->thread;
}



int nfi_worker_init(struct nfi_worker *wrk, struct nfi_server *serv, int thread)
{
#ifdef POSIX_THREADS
  pthread_attr_t attr;
  int ret;

  if (!global_busy)
  {
	  /* init the global condition and mutex */
	  global_busy = 1;
	  if(!create_m){
		  create_m = 1;
		  pthread_mutex_init(&(global_mt), NULL) ;
		  pthread_cond_init(&(global_cnd), NULL) ;
	  }
  }
#endif

  /* init the nfi_worker */
  /* pointer reference to the server */
  wrk->server = serv;

#ifdef POSIX_THREADS
  thread = 1; // FIXME: Needed since the last changes in the threads architecture
  wrk->thread = thread;

  if(thread){
	  pthread_mutex_init(&(wrk->mt), NULL) ;
	  pthread_cond_init(&(wrk->cnd), NULL) ;

  	/* create the thread */
	pthread_attr_init(&attr) ;
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED) ;
        pthread_attr_setstacksize (&attr , (256*KB) ) ;

	XPN_DEBUG("[NFI_WORKER] pthread_create(%lu)",(unsigned long int)pthread_self())

	  ret = pthread_create(&(wrk->pth),
        	         &attr,
			 (void *(*)(void *))(nfi_worker_run),
	                 (void *)wrk) ;

	  // prevent a memory leak
	  pthread_detach(wrk->pth) ; // Although it doesn't seem to work

	  pthread_attr_destroy(&attr) ;

	 if(ret != 0){
		perror("nfi_worker_init:  Error pthread_create ") ;
	}

	  pthread_mutex_lock(&(global_mt)) ;
	  while(global_busy){
	     pthread_cond_wait(&(global_cnd),&(global_mt)) ;
	  }
	  global_busy = 1;
	  pthread_mutex_unlock(&(global_mt)) ;
  }
#else
	XPN_DEBUG("[NFI_WORKER] no_threads(%lu)",(unsigned long int)pthread_self())
#endif
  return 0;
}





int nfi_worker_do_getattr(struct nfi_worker *wrk,
			struct nfi_fhandle *fh,
			struct nfi_attr *attr)
{
  /* pack petition */
  wrk->arg.operation = op_getattr;
  wrk->arg.fh = fh;
  wrk->arg.attr = attr;
#ifdef POSIX_THREADS
  if(wrk->thread){
	XPN_DEBUG("[NFI_WORKER] nfi_worker_do_getattr(%lu) with_threads",(unsigned long int)pthread_self())

	  /* wake up thread */
	  wrk->ready = 1;
	  pthread_mutex_lock(&(wrk->mt)) ;
	  if (wrk->ready)
	     pthread_cond_signal(&(wrk->cnd)) ;
	  pthread_mutex_unlock(&(wrk->mt)) ;
  }
  else{
#endif
	XPN_DEBUG("[NFI_WORKER] nfi_worker_do_getattr(%lu) without_threads",(unsigned long int)pthread_self())
	wrk->arg.result = wrk->server->ops->nfi_getattr(wrk->server,
					    wrk->arg.fh,
					    wrk->arg.attr) ;
#ifdef POSIX_THREADS
  }
#endif
  return 0;

}

int nfi_worker_do_setattr(struct nfi_worker *wrk,
			struct nfi_fhandle *fh,
			struct nfi_attr *attr)
{

  /* pack petition */
  wrk->arg.operation = op_setattr;
  wrk->arg.fh = fh;
  wrk->arg.attr = attr;
#ifdef POSIX_THREADS
  if(wrk->thread){
	  /* wake up thread */
	  wrk->ready = 1;
	  pthread_mutex_lock(&(wrk->mt)) ;
	  if (wrk->ready)
	     pthread_cond_signal(&(wrk->cnd)) ;
	  pthread_mutex_unlock(&(wrk->mt)) ;
  }else{
#endif
          wrk->arg.result = wrk->server->ops->nfi_getattr(wrk->server,
 	                                                  wrk->arg.fh,
							  wrk->arg.attr) ;
#ifdef POSIX_THREADS
  }
#endif
  return 0;
}

int nfi_worker_do_flush(struct nfi_worker *wrk,
			char *url,
			char *virtual_path,
			char *storage_path,
			int opt)
{
  /* pack petition */
  wrk->arg.operation = op_flush;
  strcpy(wrk->arg.url, url) ;
  strcpy(wrk->arg.virtual_path, virtual_path) ;
  strcpy(wrk->arg.storage_path, storage_path) ;
  wrk->arg.opt = opt;
  if(wrk->server->ops->nfi_flush == NULL){
 	wrk->arg.result = -1;
  	return -1;
  }
#ifdef POSIX_THREADS
  if(wrk->thread){
	  /* wake up thread */
	  wrk->ready = 1;
	  pthread_mutex_lock(&(wrk->mt)) ;
	  if (wrk->ready)
	     pthread_cond_signal(&(wrk->cnd)) ;
	  pthread_mutex_unlock(&(wrk->mt)) ;
  }
  else{
#endif
	wrk->arg.result = wrk->server->ops->nfi_flush(wrk->server,
					    wrk->arg.url,
					    wrk->arg.virtual_path,
					    wrk->arg.storage_path,
					    wrk->arg.opt) ;
#ifdef POSIX_THREADS
  }
#endif
  return 0;
}


int nfi_worker_do_preload(struct nfi_worker *wrk,
			char *url,
			char *virtual_path,
			char *storage_path,
			int opt)
{
  /* pack petition */


  wrk->arg.operation = op_preload;
  strcpy(wrk->arg.url, url) ;
  strcpy(wrk->arg.virtual_path, virtual_path) ;
  strcpy(wrk->arg.storage_path, storage_path) ;
  wrk->arg.opt = opt;
  if(wrk->server->ops->nfi_preload == NULL){
 	wrk->arg.result = -1;
  	return -1;
  }
#ifdef POSIX_THREADS
  if(wrk->thread){
	  XPN_DEBUG("[NFI_WORKER] nfi_worker_do_preload(%lu) with_threads",(unsigned long int)pthread_self())
	  /* wake up thread */
	  wrk->ready = 1;
	  pthread_mutex_lock(&(wrk->mt)) ;
	  if (wrk->ready)
	     pthread_cond_signal(&(wrk->cnd)) ;
	  pthread_mutex_unlock(&(wrk->mt)) ;
  }
  else{
#endif
	XPN_DEBUG("[NFI_WORKER] nfi_worker_do_preload(%lu) without_threads",(unsigned long int)pthread_self())
	wrk->arg.result = wrk->server->ops->nfi_preload(wrk->server,
					    wrk->arg.url,
					    wrk->arg.virtual_path,
					    wrk->arg.storage_path,
					    wrk->arg.opt) ;
#ifdef POSIX_THREADS
  }
#endif
  return 0;

}



int nfi_worker_do_open(struct nfi_worker *wrk,
			char *url,
			struct nfi_fhandle *fh)
{

  /* pack petition */
  wrk->arg.operation = op_open;
  wrk->arg.fh = fh;
  strcpy(wrk->arg.url,url) ;
#ifdef POSIX_THREADS
  if(wrk->thread){
	  XPN_DEBUG("[NFI_WORKER] nfi_worker_do_open(%lu) with_threads",(unsigned long int)pthread_self())
	  /* wake up thread */
	  wrk->ready = 1;
	  pthread_mutex_lock(&(wrk->mt)) ;
	  if (wrk->ready)
	     pthread_cond_signal(&(wrk->cnd)) ;
	  pthread_mutex_unlock(&(wrk->mt)) ;
  }else{
#endif
	  XPN_DEBUG("[NFI_WORKER] nfi_worker_do_open(%lu) without_threads",(unsigned long int)pthread_self())
          wrk->arg.result = wrk->server->ops->nfi_open(wrk->server,
                         		               wrk->arg.url,
                                        	       wrk->arg.fh) ;
#ifdef POSIX_THREADS
  }
#endif
  return 0;
}

int nfi_worker_do_create(struct nfi_worker *wrk,
			char *url,
			struct nfi_attr *attr,
			struct nfi_fhandle  *fh)
{
  /* pack petition */
  wrk->arg.operation = op_create;
  wrk->arg.fh = fh;
  strcpy(wrk->arg.url,url) ;
  wrk->arg.attr = attr;


#ifdef POSIX_THREADS
  if(wrk->thread){
	  XPN_DEBUG("[NFI_WORKER] nfi_worker_do_create(%lu) with_threads",(unsigned long int)pthread_self())
	  /* wake up thread */
	  wrk->ready = 1;
	  pthread_mutex_lock(&(wrk->mt)) ;
	  if (wrk->ready)
	     pthread_cond_signal(&(wrk->cnd)) ;
	  pthread_mutex_unlock(&(wrk->mt)) ;

  }else{
#endif
	  XPN_DEBUG("[NFI_WORKER] nfi_worker_do_create(%lu) without_threads",(unsigned long int)pthread_self())
          wrk->arg.result = wrk->server->ops->nfi_create(wrk->server,
	                                     wrk->arg.url,
	                                     wrk->arg.attr,
	                                     wrk->arg.fh) ;
#ifdef POSIX_THREADS
  }
#endif
  return 0;
}

int nfi_worker_do_close(struct nfi_worker *wrk,
			struct nfi_fhandle *fh)
{


  /* pack petition */
  wrk->arg.operation = op_close;
  wrk->arg.fh = fh;
#ifdef POSIX_THREADS  
  if(wrk->thread){
	 XPN_DEBUG("[NFI_WORKER] nfi_worker_do_close(%lu) with_threads",(unsigned long int)pthread_self())
  	 /* wake up thread */
	 wrk->ready = 1;
	 pthread_mutex_lock(&(wrk->mt)) ;
	 if (wrk->ready)
	    pthread_cond_signal(&(wrk->cnd)) ;
	 pthread_mutex_unlock(&(wrk->mt)) ;
  }else{
#endif
	 XPN_DEBUG("[NFI_WORKER] nfi_worker_do_close(%lu) without_threads",(unsigned long int)pthread_self())
  	 wrk->arg.result = wrk->server->ops->nfi_close(wrk->server,
	 						wrk->arg.fh) ;
#ifdef POSIX_THREADS
  }
#endif
  return 0;
}

int nfi_worker_do_remove(struct nfi_worker *wrk,
			char *url)
{


  /* pack petition */
  wrk->arg.operation = op_remove;
  strcpy(wrk->arg.url,url) ;
#ifdef POSIX_THREADS
  if(wrk->thread){
	  XPN_DEBUG("[NFI_WORKER] nfi_worker_do_remove(%lu) with_threads",(unsigned long int)pthread_self())

	  /* wake up thread */
	  wrk->ready = 1;
	  pthread_mutex_lock(&(wrk->mt)) ;
	  if (wrk->ready)
	     pthread_cond_signal(&(wrk->cnd)) ;
	  pthread_mutex_unlock(&(wrk->mt)) ;
  }else{

#endif
	XPN_DEBUG("[NFI_WORKER] nfi_worker_do_remove(%lu) without_threads",(unsigned long int)pthread_self())
  	wrk->arg.result = wrk->server->ops->nfi_remove(wrk->server,
					wrk->arg.url) ;
#ifdef POSIX_THREADS
  }
#endif
  return 0;
}

int nfi_worker_do_rename(struct nfi_worker *wrk,
			char *old_url,
			char *new_url)
{

  /* pack petition */
  wrk->arg.operation = op_rename;
  strcpy(wrk->arg.url,old_url) ;
  strcpy(wrk->arg.newurl,new_url) ;
#ifdef POSIX_THREADS
  if(wrk->thread){
	  /* wake up thread */
	  wrk->ready = 1;
	  pthread_mutex_lock(&(wrk->mt)) ;
	  if (wrk->ready)
	     pthread_cond_signal(&(wrk->cnd)) ;
	  pthread_mutex_unlock(&(wrk->mt)) ;
  }else{
#endif
	  XPN_DEBUG("[NFI_WORKER] nfi_worker_do_remove(%lu) without_threads",(unsigned long int)pthread_self())
  	  wrk->arg.result = wrk->server->ops->nfi_rename(wrk->server,
					wrk->arg.url,
					wrk->arg.newurl) ;
#ifdef POSIX_THREADS
  }
#endif
  return 0;
}

int nfi_worker_do_read(struct nfi_worker *wrk,
		       struct nfi_fhandle *fh,
		       struct nfi_worker_io *io,
		       int n)
{
  ssize_t aux, ret;
  int i;
#ifdef POSIX_THREADS
  if(wrk->thread){
	  XPN_DEBUG("[NFI_WORKER] nfi_worker_do_read(%lu) with_threads",(unsigned long int)pthread_self())
	  /* wake up thread */
	  pthread_mutex_lock(&(wrk->mt)) ;

	  /* pack petition */
	  wrk->arg.operation = op_read;
	  wrk->arg.fh = fh;
	  wrk->arg.io = io;
	  wrk->arg.n_io = n;

	  wrk->ready = 1;
	  if (wrk->ready)
	     pthread_cond_signal(&(wrk->cnd)) ;
	  pthread_mutex_unlock(&(wrk->mt)) ;
  }else{
#endif
	  XPN_DEBUG("[NFI_WORKER] nfi_worker_do_read(%lu) without_threads",(unsigned long int)pthread_self())

	  /* pack petition */
	  wrk->arg.operation = op_read;
	  wrk->arg.fh = fh;
	  wrk->arg.io = io;
	  wrk->arg.n_io = n;

  	  ret = 0;
          for(i=0; i<wrk->arg.n_io;i++){
          	//TODO: wrk->arg.io[i].res = wrk->server->ops->nfi_read(wrk->server,
          	aux = wrk->server->ops->nfi_read(wrk->server,
		                                 wrk->arg.fh,
		                                 wrk->arg.io[i].buffer,
		                                 wrk->arg.io[i].offset,
		                                 wrk->arg.io[i].size) ;
         
		if(aux<0){
                      ret = aux;
                      break;
                }
                ret = ret + aux;
		if(wrk->arg.io[i].size>(unsigned int)aux){
			break;
		}
          }
	  wrk->arg.result = ret;
#ifdef POSIX_THREADS
  }
#endif
  return 0;
}

int nfi_worker_do_write(struct nfi_worker *wrk,
		        struct nfi_fhandle *fh,
		        struct nfi_worker_io *io,
		        int n)
{
  ssize_t aux, ret;
  int i;
#ifdef POSIX_THREADS
  if(wrk->thread){
	XPN_DEBUG("[NFI_WORKER] nfi_worker_do_write(%lu) with_threads",(unsigned long int)pthread_self())
  	/* wake up thread */
  	pthread_mutex_lock(&(wrk->mt)) ;

	/* pack petition */
	wrk->arg.operation = op_write;
	wrk->arg.fh = fh;
	wrk->arg.io = io;
	wrk->arg.n_io = n;

  	wrk->ready = 1;
  	if (wrk->ready)
     		pthread_cond_signal(&(wrk->cnd)) ;
	pthread_mutex_unlock(&(wrk->mt)) ;
  }else{
#endif
	XPN_DEBUG("[NFI_WORKER] nfi_worker_do_write(%lu) without_threads",(unsigned long int)pthread_self())

	  /* pack petition */
	  wrk->arg.operation = op_write;
	  wrk->arg.fh = fh;
	  wrk->arg.io = io;
	  wrk->arg.n_io = n;

	  ret = 0;
          for(i=0; i<wrk->arg.n_io;i++){
          	//io[i].res = wrk->server->ops->nfi_write(wrk->server,
          	aux = wrk->server->ops->nfi_write(wrk->server,
		                                 wrk->arg.fh,
		                                 wrk->arg.io[i].buffer,
		                                 wrk->arg.io[i].offset,
		                                 wrk->arg.io[i].size) ;
                if(aux<0){
                      ret = aux;
                      break;
                }
                ret = ret + aux;
          }
	  wrk->arg.result = ret;
#ifdef POSIX_THREADS
  }
#endif

  return 0;
}

int nfi_worker_do_mkdir(struct nfi_worker *wrk,
			char *url,
			struct nfi_attr *attr,
			struct nfi_fhandle *fh)
{

  /* pack petition */
  wrk->arg.operation = op_mkdir;
  strcpy(wrk->arg.url,url) ;
  wrk->arg.attr = attr;
  wrk->arg.fh = fh;
#ifdef POSIX_THREADS
  if(wrk->thread){
	  /* wake up thread */
	  wrk->ready = 1;
	  pthread_mutex_lock(&(wrk->mt)) ;
	  if (wrk->ready)
	     pthread_cond_signal(&(wrk->cnd)) ;
	  pthread_mutex_unlock(&(wrk->mt)) ;
  }else{
#endif
	wrk->arg.result = wrk->server->ops->nfi_mkdir(wrk->server,
	               			              wrk->arg.url,
	                              		      wrk->arg.attr,
	                              		      wrk->arg.fh) ;
#ifdef POSIX_THREADS
  }
#endif

  return 0;
}

int nfi_worker_do_rmdir(struct nfi_worker *wrk,
			char *url)
{

  /* pack petition */
  wrk->arg.operation = op_rmdir;
  strcpy(wrk->arg.url,url) ;
#ifdef POSIX_THREADS
  if(wrk->thread){
	  /* wake up thread */
	  wrk->ready = 1;
	  pthread_mutex_lock(&(wrk->mt)) ;
	  if (wrk->ready)
	     pthread_cond_signal(&(wrk->cnd)) ;
	  pthread_mutex_unlock(&(wrk->mt)) ;
  }else{
#endif
	  wrk->arg.result= wrk->server->ops->nfi_rmdir(wrk->server,
						       wrk->arg.url) ;
#ifdef POSIX_THREADS
  }
#endif
  return 0;
}

int nfi_worker_do_opendir(struct nfi_worker *wrk,
			char *url,
			struct nfi_fhandle *fh)
{


  /* pack petition */
  wrk->arg.operation = op_opendir;
  strcpy(wrk->arg.url,url) ;
  wrk->arg.fh = fh;
#ifdef POSIX_THREADS
  if(wrk->thread){
	  /* wake up thread */
	  wrk->ready = 1;
	  pthread_mutex_lock(&(wrk->mt)) ;
	  if (wrk->ready)
	     pthread_cond_signal(&(wrk->cnd)) ;
	  pthread_mutex_unlock(&(wrk->mt)) ;
  }else{

#endif
	  wrk->arg.result = wrk->server->ops->nfi_opendir(wrk->server,
			  		      wrk->arg.url,
					      wrk->arg.fh) ;
#ifdef POSIX_THREADS
  }
#endif
  return 0;
}

int nfi_worker_do_readdir(struct nfi_worker *wrk, struct nfi_fhandle *fh, char *entry, unsigned char *type)
{


  /* pack petition */
  wrk->arg.operation = op_readdir;
  wrk->arg.type = type;
  wrk->arg.entry = entry;
  wrk->arg.fh = fh;

#ifdef POSIX_THREADS
  if(wrk->thread){
  	/* wake up thread */
	wrk->ready = 1;
	pthread_mutex_lock(&(wrk->mt)) ;
	if (wrk->ready)
 	    pthread_cond_signal(&(wrk->cnd)) ;
	pthread_mutex_unlock(&(wrk->mt)) ;
  }else{
#endif

        wrk->arg.result = wrk->server->ops->nfi_readdir(wrk->server,
                                            wrk->arg.fh,
                                            wrk->arg.entry,
                                            wrk->arg.type
					    ) ;
#ifdef POSIX_THREADS
  }
#endif

  return 0;
}

int nfi_worker_do_closedir(struct nfi_worker *wrk, struct nfi_fhandle *fh)
{
  /* pack petition */
  wrk->arg.operation = op_closedir;
  wrk->arg.fh = fh;

#ifdef POSIX_THREADS
  if (wrk->thread) { 
  	/* wake up thread */
  	wrk->ready = 1;
  	pthread_mutex_lock(&(wrk->mt)) ;
  	if (wrk->ready)
     		pthread_cond_signal(&(wrk->cnd)) ;

  	pthread_mutex_unlock(&(wrk->mt)) ;
  }else{
#endif

     wrk->arg.result = wrk->server->ops->nfi_closedir(wrk->server, wrk->arg.fh) ;

#ifdef POSIX_THREADS
  }
#endif
  return 0;
}

int nfi_worker_do_statfs(struct nfi_worker *wrk, struct nfi_info *inf)
{
    /* pack petition */
    wrk->arg.operation = op_statfs;
    wrk->arg.inf = inf;

#ifdef POSIX_THREADS
    if (wrk->thread){
 	/* wake up thread */
  	wrk->ready = 1;
  	pthread_mutex_lock(&(wrk->mt)) ;
  	if (wrk->ready)
     		pthread_cond_signal(&(wrk->cnd)) ;
  	pthread_mutex_unlock(&(wrk->mt)) ;
    } else {
#endif

    wrk->arg.result = wrk->server->ops->nfi_statfs(wrk->server, wrk->arg.inf) ;

#ifdef POSIX_THREADS
    }
#endif

    return 0;
}

ssize_t nfi_worker_wait ( struct nfi_worker *wrk )
{
    ssize_t ret;
    ret = 0;

#ifdef POSIX_THREADS
    if (wrk->thread)
    {
	XPN_DEBUG("[NFI_WORKER] nfi_worker_wait(%lu) with_threads",(unsigned long int)pthread_self())
  	pthread_mutex_lock(&(wrk->mt)) ;

	while (wrk->ready){
	         pthread_cond_wait(&(wrk->cnd),
			           &(wrk->mt)) ;
 	}
    }
#endif

    ret = wrk->arg.result;
    wrk->arg.result = 0;

#ifdef POSIX_THREADS
    if (wrk->thread) {
	XPN_DEBUG("[NFI_WORKER] nfi_worker_unlock(%lu) with_threads",(unsigned long int)pthread_self())
  	pthread_mutex_unlock(&(wrk->mt)) ;
    } else {
	XPN_DEBUG("[NFI_WORKER] nfi_worker_wait(%lu) without_threads",(unsigned long int)pthread_self())
    }
#endif

    return ret;
}

int nfi_worker_end(struct nfi_worker *wrk)
{
#ifdef POSIX_THREADS
    if (wrk->thread)
    {
	XPN_DEBUG("[NFI_WORKER] nfi_worker_end(%lu) with_threads",(unsigned long int)pthread_self())
  	pthread_mutex_destroy(&(wrk->mt)) ;
 	pthread_cond_destroy(&(wrk->cnd)) ;
    }
#endif

    free(wrk) ;
    return 0;
}

int nfi_worker_destroy()
{
#ifdef POSIX_THREADS
    pthread_mutex_destroy(&(global_mt)) ;
    pthread_cond_destroy(&(global_cnd)) ;
#endif

    return 0;
}

