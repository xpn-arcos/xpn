#ifndef _NFI_WORKER_TASK_H_
#define _NFI_WORKER_TASK_H_


#include "all_system.h"
#include "debug_msg.h"
#include "workers.h"
#include "nfi.h"


 #ifdef  __cplusplus
    extern "C" {
 #endif


struct nfi_worker_io {
  off_t offset;
  size_t size;
  void *buffer;
};

struct nfi_worker_args {
  int operation;
  ssize_t result;

  char url[PATH_MAX];
  char virtual_path[PATH_MAX];
  char storage_path[PATH_MAX];
  char newurl[PATH_MAX];
  struct nfi_fhandle *fh;
  struct nfi_attr *attr;
  int opt;

  struct nfi_info *inf;

  int n_io;
  struct nfi_worker_io *io;
  char *entry;
  unsigned char *type;
};

struct nfi_worker {
  int thread;

  // OLD
  pthread_t pth;
  pthread_mutex_t mt;
  pthread_cond_t cnd;
  int ready;

  // NEW
  worker_t     wb ; 
  struct st_th warg ;

  struct nfi_server      *server;
  struct nfi_worker_args  arg; // TODO: Convert this into a list of 'struct nfi_worker_args' to make Expand reentrant
};


 #ifdef  __cplusplus
    }
 #endif


#endif

