#ifndef _NFI_OPS_H_
#define _NFI_OPS_H_


#ifdef  __cplusplus
  extern "C" {
#endif

#ifndef XPN_HEADER_SIZE 
  #define XPN_HEADER_SIZE 8192
#endif   

#include "nfi_worker_task.h"
#include "nfi_worker.h"


enum nfi_work{
  noop        = -1,
  op_open     =  0,
  op_create   =  1,
  op_read     =  2,
  op_write    =  3,
  op_close    =  4,
  op_remove   =  5,
  op_rename   =  6,
  op_getattr  =  7,
  op_setattr  =  8,

  op_mkdir    = 20,
  op_rmdir    = 21,
  op_opendir  = 22,
  op_readdir  = 23,
  op_closedir = 24,

  op_statfs   = 60,
  op_preload  = 61,
  op_flush    = 62,
};

void nfi_do_operation ( struct st_th th_arg ) ;

int nfi_worker_do_open(struct nfi_worker *wrk, char *url, struct nfi_fhandle *fho);
int nfi_worker_do_create(struct nfi_worker *wrk, char *url, struct nfi_attr *attr, struct nfi_fhandle  *fh);
int nfi_worker_do_read( struct nfi_worker *wrk, struct nfi_fhandle *fh, struct nfi_worker_io *io,int n);
int nfi_worker_do_write(struct nfi_worker *wrk, struct nfi_fhandle *fh, struct nfi_worker_io *io,int n);
int nfi_worker_do_close(struct nfi_worker *wrk, struct nfi_fhandle *fh);

int nfi_worker_do_remove(struct nfi_worker *wrk, char *url);
int nfi_worker_do_rename(struct nfi_worker *wrk, char *old_url, char *new_url);
int nfi_worker_do_getattr(struct nfi_worker *wrk, struct nfi_fhandle *fh, struct nfi_attr *attr);
int nfi_worker_do_setattr(struct nfi_worker *wrk, struct nfi_fhandle *fh, struct nfi_attr *attr);

int nfi_worker_do_mkdir     (struct nfi_worker *wrk, char *url,               struct nfi_attr *attr, struct nfi_fhandle *fh);
int nfi_worker_do_opendir   (struct nfi_worker *wrk, char *url,               struct nfi_fhandle *fho);
int nfi_worker_do_readdir   (struct nfi_worker *wrk, struct nfi_fhandle *fhd, struct dirent *entry) ;
int nfi_worker_do_closedir  (struct nfi_worker *wrk, struct nfi_fhandle *fh);
int nfi_worker_do_rmdir     (struct nfi_worker *wrk, char *url);

int nfi_worker_do_statfs(struct nfi_worker *wrk, struct nfi_info *inf);
int nfi_worker_do_preload(struct nfi_worker *wrk, char *url, char *virtual_path,char *storage_path,int opt);
int nfi_worker_do_flush(struct nfi_worker *wrk, char *url, char *virtual_path,char *storage_path,int opt);

#ifdef  __cplusplus
  }
#endif


#endif

