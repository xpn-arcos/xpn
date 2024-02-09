
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

#include "nfi_local.h"


/* ... Const / Const ................................................. */

#define FILESYSTEM_DLSYM 1


/* ... Global variables / Variables globales ........................ */


/* ... Auxiliar Functions / Funciones Auxiliares ..................... */

//Communication
int nfi_local_keep_connected ( struct nfi_server *serv )
{
  // check params...
  if (NULL == serv)
  {
    printf("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_keep_connected] ERROR: serv argument is NULL\n", -1);
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_keep_connected] >> Begin\n", serv->id);

#ifdef NFI_DYNAMIC
  if (serv->private_info == NULL)
  {
    debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_keep_connected] Server reconnect\n", server_aux->id);

    ret = nfi_local_reconnect(serv);
    if (ret < 0)
    {
      serv->private_info = NULL;
      return -1;
    }
  }
#endif

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_keep_connected] >> End\n", serv->id);

  return (serv->private_info != NULL);
}

void nfi_2_local_attr ( struct stat *att, struct nfi_attr *nfi_att )
{
  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_2_local_attr] >> Begin\n", -1);

  att->st_dev = nfi_att->st_dev;
  att->st_ino = nfi_att->st_ino;

  if (nfi_att->at_type == NFIFILE){
    att->st_mode = nfi_att->at_mode | S_IFREG; // protection
  }

  if (nfi_att->at_type == NFIDIR){
    att->st_mode = nfi_att->at_mode | S_IFDIR; // protection
  }

  att->st_nlink   = nfi_att->at_nlink;   // number of hard links
  att->st_uid     = nfi_att->at_uid;     // user ID of owner
  att->st_gid     = nfi_att->at_gid;     // group ID of owner
  att->st_size    = nfi_att->at_size;    // size
  att->st_blksize = nfi_att->at_blksize; // blocksize for filesystem I/O
  att->st_blocks  = nfi_att->at_blocks;  // number of blocks allocated
  att->st_atime   = nfi_att->at_atime;   // time of last access
  att->st_mtime   = nfi_att->at_mtime;   // time of last modification
  att->st_ctime   = nfi_att->at_ctime;   // time of last change

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_2_local_attr] >> End\n", -1);
}

void local_2_nfi_attr ( struct nfi_attr *nfi_att, struct stat *att )
{
  debug_info("[SERV_ID=%d] [NFI_LOCAL] [local_2_nfi_attr] >> Begin\n", -1);

  nfi_att->st_dev = att->st_dev;
  nfi_att->st_ino = att->st_ino;

  if (S_ISREG(att->st_mode)) {
    nfi_att->at_type = NFIFILE;
  }
  if (S_ISDIR(att->st_mode)) {
    nfi_att->at_type = NFIDIR;
  }

  nfi_att->at_mode    = att->st_mode&(S_IRWXU|S_IRWXG|S_IRWXO); // protection
  nfi_att->at_nlink   = att->st_nlink;                           // number of hard links
  nfi_att->at_uid     = att->st_uid;                             // user ID of owner
  nfi_att->at_gid     = att->st_gid;                             // group ID of owner
  nfi_att->at_size    = att->st_size;                            // total size, in bytes
  nfi_att->at_blksize = att->st_blksize;                         // blocksize for filesystem I/O
  nfi_att->at_blocks  = att->st_blocks;                          // number of blocks allocated
  nfi_att->at_atime   = att->st_atime;                           // time of last access
  nfi_att->at_mtime   = att->st_mtime;                           // time of last modification
  nfi_att->at_ctime   = att->st_ctime;                           // time of last change

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [local_2_nfi_attr] >> End\n", -1);
}

void local_2_nfi_info( __attribute__((__unused__)) struct nfi_info *nfi_inf, __attribute__((__unused__)) struct nfi_info *local_inf )
{
  debug_info("[SERV_ID=%d] [NFI_LOCAL] [local_2_nfi_info] >> Begin\n", -1);

  //TODO

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [local_2_nfi_info] >> End\n", -1);
}


/* ... Functions / Funciones ......................................... */

int nfi_local_init ( char *url, struct nfi_server *serv, __attribute__((__unused__)) struct nfi_attr_server *attr )
{
  int ret;
  char server[PATH_MAX], dir[PATH_MAX], prt[PATH_MAX];
  struct nfi_local_server *server_aux;
  
  // check params...
  if (serv == NULL) {
    printf("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_init] ERROR: serv argument is NULL\n", -1);
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_init] >> Begin\n", serv->id);

  // new nfi_ops with local functions...
  serv->ops = (struct nfi_ops *)malloc(sizeof(struct nfi_ops));
  NULL_RET_ERR(serv->ops, LOCAL_ERR_MEMORY);

  // Fill serv->ops...
  bzero(serv->ops, sizeof(struct nfi_ops));
  serv->ops->nfi_reconnect  = nfi_local_reconnect;
  serv->ops->nfi_disconnect = nfi_local_disconnect;

  serv->ops->nfi_open       = nfi_local_open;
  serv->ops->nfi_create     = nfi_local_create;
  serv->ops->nfi_read       = nfi_local_read;
  serv->ops->nfi_write      = nfi_local_write;
  serv->ops->nfi_close      = nfi_local_close;
  serv->ops->nfi_remove     = nfi_local_remove;
  serv->ops->nfi_rename     = nfi_local_rename;
  serv->ops->nfi_getattr    = nfi_local_getattr;
  serv->ops->nfi_setattr    = nfi_local_setattr;

  serv->ops->nfi_opendir    = nfi_local_opendir;
  serv->ops->nfi_mkdir      = nfi_local_mkdir;
  serv->ops->nfi_readdir    = nfi_local_readdir;
  serv->ops->nfi_closedir   = nfi_local_closedir;
  serv->ops->nfi_rmdir      = nfi_local_rmdir;

  serv->ops->nfi_preload    = nfi_local_preload;
  serv->ops->nfi_flush      = nfi_local_flush;

  serv->ops->nfi_statfs     = nfi_local_statfs;

  // ParseURL...
  ret = ParseURL(url, prt, NULL, NULL, server, NULL, dir);
  if (ret < 0)
  {
    printf("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_init] ERROR: incorrect url '%s'.\n", serv->id, url);
    local_err(LOCAL_ERR_URL);
    FREE_AND_NULL(serv->ops);
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_init] ParseURL(%s)= %s; %s\n", serv->id, url, server, dir);

  // new nfi_local_server...
  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_init] Copy nfi_local_server structure\n", serv->id);

  server_aux = (struct nfi_local_server *)malloc(sizeof(struct nfi_local_server));
  if (server_aux == NULL)
  {
    printf("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_init] ERROR: out of memory.\n", serv->id);
    FREE_AND_NULL(serv->ops);
    return -1;
  }

  // private_info...
  serv->private_info = (void *)server_aux;

  // Initialize params
  memset(server_aux, 0, sizeof(struct nfi_local_server));

  // thread mode
  serv->xpn_thread = TH_NOT;
  char *env_thread = getenv("XPN_THREAD");
  if (env_thread != NULL) {
    serv->xpn_thread = atoi(env_thread);
  }

  // copy 'url' string...
  serv->url = strdup(url);
  NULL_RET_ERR(serv->url, MPI_SERVER_ERR_MEMORY);

  // new server wrk...
  serv->wrk = (struct nfi_worker *)malloc(sizeof(struct nfi_worker));
  memset(serv->wrk, 0, sizeof(struct nfi_worker));
  serv->wrk->server = serv;

  // Initialize workers
  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_init] Initialize workers\n", serv->id);

  ret = nfiworker_init(serv);

  // Server conection
  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_init] Server conection\n", serv->id);

  ret = nfi_local_connect(serv, url, prt, server, dir);
  if (ret < 0) {
    FREE_AND_NULL(serv->ops);
    FREE_AND_NULL(server_aux);
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_init] << End\n", serv->id);

  return 0;
}

int nfi_local_destroy ( struct nfi_server *serv )
{
  struct nfi_local_server *server_aux;

  // check params...
  if (serv == NULL) {
    printf("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_destroy] ERROR: serv argument is NULL\n", -1);
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_destroy] >> Begin\n", serv->id);

  // private_info...
  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_destroy] Get server private info\n", serv->id);

  server_aux = (struct nfi_local_server *) (serv->private_info);
  if (server_aux == NULL) {
    return -1;
  }
  
  // Workers destroy...
  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_destroy] Destroy workers\n", serv->id);

  nfiworker_destroy(serv);

  // free private_info, 'url' string and 'server' string...
  FREE_AND_NULL(serv->ops);
  FREE_AND_NULL(serv->private_info);
  FREE_AND_NULL(serv->url);
  FREE_AND_NULL(serv->server);

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_destroy] << End\n", serv->id);

  return 0;
}

int nfi_local_connect ( struct nfi_server *serv, __attribute__((__unused__)) char *url, __attribute__((__unused__)) char* prt, __attribute__((__unused__)) char* server, __attribute__((__unused__)) char* dir )
{
  struct nfi_local_server *server_aux;

  // check params...
  if (serv == NULL) {
    printf("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_connect] ERROR: serv argument is NULL\n", -1);
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_connect] >> Begin\n", serv->id);

  // private_info...
  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_connect] Get server private info\n", serv->id);

  server_aux = (struct nfi_local_server *) (serv->private_info);
  if (server_aux == NULL) {
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_connect] << End\n", serv->id);

  return 0;
}

int nfi_local_disconnect ( struct nfi_server *serv )
{
  struct nfi_local_server *server_aux;

  // check params...
  if (serv == NULL) {
    printf("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_disconnect] ERROR: serv argument is NULL\n", -1);
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_disconnect] >> Begin\n", serv->id);

  // private_info...
  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_disconnect] Get server private info\n", serv->id);

  server_aux = (struct nfi_local_server *) (serv->private_info);
  if (server_aux == NULL) {
    return -1;
  }

  FREE_AND_NULL(serv->private_info);

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_disconnect] << End\n", serv->id);

  return 0;
}

int nfi_local_reconnect ( struct nfi_server *serv ) //TODO
{
  // Don't see the serv retult
  int ret;
  char   dir[PATH_MAX];
  struct nfi_local_server *server_aux;

  // check params...
  if (serv == NULL) {
    printf("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_reconnect] ERROR: serv argument is NULL\n", -1);
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_reconnect] >> Begin\n", serv->id);

  // parse url...
  ret = ParseURL(serv->url, NULL, NULL, NULL, NULL, NULL, dir);
  if (ret < 0) {
    printf("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_reconnect] ERROR: url %s incorrect.\n", serv->id, serv->url);
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_reconnect] ParseURL(%s)= %s\n", serv->id, serv->url, dir);

  // new nfi_local_server...
  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_reconnect] Copy nfi_local_server structure\n", serv->id);

  server_aux = (struct nfi_local_server *)malloc(sizeof(struct nfi_local_server));
  NULL_RET_ERR(server_aux, LOCAL_ERR_MEMORY);

  strcpy(server_aux->path, dir);
  serv->private_info = (void *)server_aux;
  
  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_reconnect] << End\n", serv->id);

  return 0;
}

// File API
int nfi_local_open ( struct nfi_server *serv, char *url, struct nfi_fhandle *fho )
{
  int ret;
  char dir[PATH_MAX];
  struct nfi_local_fhandle *fh_aux;

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_open] >> Begin\n", serv->id);

  // Check arguments...
  NULL_RET_ERR(serv, LOCAL_ERR_PARAM);
  NULL_RET_ERR(fho,  LOCAL_ERR_PARAM);
  nfi_local_keep_connected(serv);
  NULL_RET_ERR(serv->private_info, LOCAL_ERR_PARAM);

  // from url -> server + dir
  ret = ParseURL(url, NULL, NULL, NULL, NULL, NULL, dir);
  if (ret < 0)
  {
    printf("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_open] ERROR: incorrect url '%s'.\n", serv->id, url);
    local_err(LOCAL_ERR_URL);
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_open] ParseURL(%s)= %s\n", serv->id, url, dir);

  //Copy url
  fho->url = strdup(url);
  NULL_RET_ERR(fho->url, LOCAL_ERR_MEMORY);

  fh_aux = (struct nfi_local_fhandle *)malloc(sizeof(struct nfi_local_fhandle));
  NULL_RET_ERR(fh_aux, LOCAL_ERR_MEMORY);
  bzero(fh_aux, sizeof(struct nfi_local_fhandle));

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_open] nfi_local_open(%s)\n", serv->id, dir);

  fh_aux->fd = real_posix_open2(dir, O_RDWR, S_IRWXU);
  if (fh_aux->fd < 0)
  {
    debug_error("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_open] ERROR: real_posix_open2 fails to open '%s' in server %s.\n", serv->id, dir, serv->server);
    FREE_AND_NULL(fh_aux);
    FREE_AND_NULL(fho->url);
    return -1;
  }

  strcpy(fh_aux->path, dir);

  fho->type    = NFIFILE;
  fho->priv_fh = NULL;
  fho->server = serv;
  fho->priv_fh = (void *) fh_aux;

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_open] nfi_local_open(%s)=%d\n", serv->id, dir, fh_aux->fd);
  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_open] << End\n", serv->id);

  return 0;
}

int nfi_local_create ( struct nfi_server *serv,  char *url, struct nfi_attr *attr, struct nfi_fhandle *fh )
{
  int ret;
  char   dir[PATH_MAX];
  struct nfi_local_fhandle *fh_aux;
  struct stat st;

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_create] >> Begin\n", serv->id);

  // Check arguments...
  NULL_RET_ERR(serv, LOCAL_ERR_PARAM);
  NULL_RET_ERR(attr, LOCAL_ERR_PARAM);
  nfi_local_keep_connected(serv);
  NULL_RET_ERR(serv->private_info, LOCAL_ERR_PARAM);

  // url -> server + dir
  ret = ParseURL(url, NULL, NULL, NULL, NULL, NULL, dir);
  if (ret < 0)
  {
    debug_error("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_create] ERROR: incorrect url '%s'.\n", serv->id, url);
    local_err(LOCAL_ERR_URL);
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_create] ParseURL(%s)= %s\n", serv->id, url, dir);

  // private_info file handle
  fh_aux = (struct nfi_local_fhandle *)malloc(sizeof(struct nfi_local_fhandle));
  NULL_RET_ERR(fh_aux, LOCAL_ERR_MEMORY);
  bzero(fh_aux, sizeof(struct nfi_local_fhandle));
  
  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_create] nfi_local_create(%s)\n", serv->id, dir);

  fh_aux->fd = real_posix_open2(dir, O_CREAT|O_RDWR|O_TRUNC, attr->at_mode);
  if (fh_aux->fd < 0)
  {
    debug_error("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_create] ERROR: real_posix_open2 fails to open '%s' in server %s.\n", serv->id, dir, serv->server);
    FREE_AND_NULL(fh_aux);
    return -1;
  }

  // Get stat of the file
  memset(&st, 0, sizeof(struct stat));

  ret = real_posix_stat(dir, &st);
  if (ret < 0)
  {
    debug_error("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_create] ERROR: real_posix_stat fails to stat '%s' in server %s.\n", serv->id, dir, serv->server);
    FREE_AND_NULL(fh->url);
    FREE_AND_NULL(fh_aux);
    return -1;
  }

  fh->type   = NFIFILE;
  fh->server = serv;
  fh->priv_fh = (void *)fh_aux;

  fh->url = strdup(url);
  if (fh->url == NULL)
  {
    local_err(LOCAL_ERR_MEMORY);
    FREE_AND_NULL(fh_aux);
    return -1;
  }

  local_2_nfi_attr(attr, &st);

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_create] >> End\n", serv->id);

  return 0;
}

ssize_t nfi_local_read ( struct nfi_server *serv, struct nfi_fhandle *fh, void *buffer, off_t offset, size_t size )
{
  ssize_t ret;
  struct nfi_local_fhandle *fh_aux;

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_read] >> Begin\n", serv->id);

  // Check arguments...
  NULL_RET_ERR(serv, LOCAL_ERR_PARAM);
  NULL_RET_ERR(fh,   LOCAL_ERR_PARAM);
  nfi_local_keep_connected(serv);
  NULL_RET_ERR(serv->private_info, LOCAL_ERR_PARAM);

  // private_info file handle
  fh_aux = (struct nfi_local_fhandle *) fh->priv_fh;

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_read] nfi_local_read(%d, %ld, %ld)\n", serv->id, fh_aux->fd, offset, size);

  real_posix_lseek(fh_aux->fd, offset, SEEK_SET); //TODO: check error

  ret = real_posix_read(fh_aux->fd, buffer, size);
  if (ret < 0)
  {
    debug_error("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_read] ERROR: real_posix_read reads zero bytes from '%d' in server %s\n", serv->id, fh_aux->fd, serv->server);
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_read] nfi_local_read(%d, %ld, %ld)=%ld\n", serv->id, fh_aux->fd, offset, size, ret);
  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_read] >> End\n", serv->id);

  return ret;
}

ssize_t nfi_local_write ( struct nfi_server *serv, struct nfi_fhandle *fh, void *buffer, off_t offset, size_t size )
{
  ssize_t ret;
  struct nfi_local_fhandle *fh_aux;

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_write] >> Begin\n", serv->id);

  // Check arguments...
  if (size == 0){
    return 0;
  }

  NULL_RET_ERR(serv, LOCAL_ERR_PARAM);
  NULL_RET_ERR(fh,   LOCAL_ERR_PARAM);
  nfi_local_keep_connected(serv);
  NULL_RET_ERR(serv->private_info, LOCAL_ERR_PARAM);

  // private_info file handle
  fh_aux = (struct nfi_local_fhandle *) fh->priv_fh;
  
  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_write] nfi_local_write(%d, %ld, %ld)\n", serv->id, fh_aux->fd, offset, size);

  real_posix_lseek(fh_aux->fd, offset, SEEK_SET); //TODO: check error

  ret = real_posix_write(fh_aux->fd, buffer, size);
  if (ret < 0)
  {
    debug_error("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_write] ERROR: real_posix_write writes zero bytes from '%d' in server %s\n", serv->id, fh_aux->fd, serv->server);
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_write] nfi_local_write(%d, %ld, %ld)=%ld\n", serv->id, fh_aux->fd, offset, size, ret);
  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_write] >> End\n", serv->id);

  return ret;
}

int nfi_local_close ( struct nfi_server *serv,  struct nfi_fhandle *fh )
{
  int ret = -1;
  struct nfi_local_fhandle *fh_aux;

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_close] >> Begin\n", serv->id);

  // Check arguments...
  NULL_RET_ERR(serv, LOCAL_ERR_PARAM);
  NULL_RET_ERR(fh,   LOCAL_ERR_PARAM);
  nfi_local_keep_connected(serv);
  NULL_RET_ERR(serv->private_info, LOCAL_ERR_PARAM);
  
  // private_info file handle
  fh_aux = (struct nfi_local_fhandle *) fh->priv_fh;

  if (fh_aux != NULL)
  {
    debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_close] nfi_local_close(%d)\n", serv->id, fh_aux->fd);

    ret = real_posix_close(fh_aux->fd);

    debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_close] nfi_local_close(%d)=%d\n", serv->id, fh_aux->fd, ret);
  }

  // free memory
  FREE_AND_NULL(fh->priv_fh);
  FREE_AND_NULL(fh->url);
  fh->type   = NFINULL;
  fh->server = NULL;

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_close] >> End\n", serv->id);

  return ret;
}


int nfi_local_remove ( struct nfi_server *serv,  char *url )
{
  int ret;
  char dir[PATH_MAX];

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_remove] >> Begin\n", serv->id);

  // Check arguments...
  NULL_RET_ERR(serv, LOCAL_ERR_PARAM);
  NULL_RET_ERR(url,  LOCAL_ERR_PARAM);
  nfi_local_keep_connected(serv);
  NULL_RET_ERR(serv->private_info, LOCAL_ERR_PARAM);

  // from url -> server + dir
  ret = ParseURL(url, NULL, NULL, NULL, NULL, NULL, dir);
  if (ret < 0)
  {
    printf("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_remove] ERROR: incorrect url '%s'.\n", serv->id, url);
    local_err(LOCAL_ERR_URL);
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_remove] ParseURL(%s)= %s\n", serv->id, url, dir);

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_remove] nfi_local_remove(%s)\n", serv->id, dir);

  ret = real_posix_unlink(dir);
  if (ret < 0)
  {
    debug_error("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_remove] ERROR: real_posix_unlink fails to unlink '%s' in server %s.\n", serv->id, dir, serv->server);
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_remove] nfi_local_remove(%s)=%d\n", serv->id, dir, ret);
  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_remove] >> End\n", serv->id);

  return ret;
}

int nfi_local_rename (struct nfi_server *serv, char *old_url, char *new_url )
{
  int  ret;
  char old_path[PATH_MAX], new_path[PATH_MAX];

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_rename] >> Begin\n", serv->id);

  // Check arguments...
  NULL_RET_ERR(serv,     LOCAL_ERR_PARAM);
  NULL_RET_ERR(old_url,  LOCAL_ERR_PARAM);
  NULL_RET_ERR(new_url,  LOCAL_ERR_PARAM);
  nfi_local_keep_connected(serv);
  NULL_RET_ERR(serv->private_info, LOCAL_ERR_PARAM);

  // Get fields...
  ret = ParseURL(old_url, NULL, NULL, NULL, NULL, NULL, old_path);
  if (ret < 0)
  {
    printf("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_rename] ERROR: incorrect url '%s'.\n", serv->id, old_url);
    local_err(LOCAL_ERR_URL);
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_rename] ParseURL(%s)= %s\n", serv->id, old_url, old_path);

  ret = ParseURL(new_url, NULL, NULL, NULL, NULL, NULL, new_path);
  if (ret < 0)
  {
    printf("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_rename] ERROR: incorrect url '%s'.\n", serv->id, new_url);
    local_err(LOCAL_ERR_URL);
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_rename] ParseURL(%s)= %s\n", serv->id, new_path, new_path);

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_rename] nfi_local_rename(%s,%s)\n", serv->id, old_path, new_path);

  ret = real_posix_rename(old_path, new_path);
  if (ret < 0)
  {
    debug_error("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_rename] ERROR: real_posix_rename fails to rename '%s' in server %s.\n", serv->id, old_path, serv->server);
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_rename] nfi_local_rename(%s,%s)=%d\n", serv->id, old_path, new_path, ret);
  debug_info("[NFI_LOCAL] [nfi_local_rename] >> End\n");

  return ret;
}

int nfi_local_getattr ( struct nfi_server *serv,  struct nfi_fhandle *fh, struct nfi_attr *attr )
{
  int  ret;
  char dir[PATH_MAX];
  //struct nfi_local_fhandle *fh_aux;
  struct stat st;

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_getattr] >> Begin\n", serv->id);

  // check arguments...
  NULL_RET_ERR(serv,            LOCAL_ERR_PARAM);
  NULL_RET_ERR(fh,              LOCAL_ERR_PARAM);
  NULL_RET_ERR(attr,            LOCAL_ERR_PARAM);
  nfi_local_keep_connected(serv);
  NULL_RET_ERR(serv->private_info, LOCAL_ERR_PARAM);


  ret = ParseURL(fh->url, NULL, NULL, NULL, NULL,  NULL,  dir);
  if (ret < 0)
  {
    printf("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_getattr] ERROR: incorrect url '%s'.\n", serv->id, fh->url);
    local_err(LOCAL_ERR_URL);
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_getattr] ParseURL(%s)= %s\n", serv->id, fh->url, dir);
  
  // copy private information...
  //fh_aux = (struct nfi_local_fhandle *) fh->priv_fh; //TODO: fstat

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_getattr] nfi_local_getattr(%s)\n", serv->id, dir);

  ret = real_posix_stat(dir, &st);
  if (ret < 0)
  {
    debug_error("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_getattr] ERROR: real_posix_stat fails to stat '%s' in server %s.\n", serv->id, dir, serv->server);
    return ret;
  }

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_getattr] nfi_local_getattr(%s)=%d\n", serv->id, dir, ret);

  local_2_nfi_attr(attr, &st);

  debug_info("[NFI_LOCAL] [nfi_local_getattr] >> End\n");

  return ret;
}

int nfi_local_setattr ( struct nfi_server *serv,  struct nfi_fhandle *fh, struct nfi_attr *attr )
{
  struct stat st;

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_setattr] >> Begin\n", serv->id);

  // Check arguments...
  NULL_RET_ERR(serv, LOCAL_ERR_PARAM);
  NULL_RET_ERR(fh,   LOCAL_ERR_PARAM);
  NULL_RET_ERR(attr, LOCAL_ERR_PARAM);
  NULL_RET_ERR(fh->priv_fh, LOCAL_ERR_PARAM);
  nfi_local_keep_connected(serv);
  NULL_RET_ERR(serv->private_info, LOCAL_ERR_PARAM);

  // TODO: setattr

  nfi_2_local_attr(&st, attr);

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_setattr] >> End\n", serv->id);

  return 0;
}

// Directories API
int nfi_local_mkdir ( struct nfi_server *serv,  char *url, struct nfi_attr *attr, struct nfi_fhandle *fh )
{
  int    ret;
  char   dir[PATH_MAX];
  struct nfi_local_fhandle *fh_aux;
  struct stat st;

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_mkdir] >> Begin\n", serv->id);

  // Check arguments...
  NULL_RET_ERR(serv, LOCAL_ERR_PARAM);
  NULL_RET_ERR(attr, LOCAL_ERR_PARAM);
  nfi_local_keep_connected(serv);
  NULL_RET_ERR(serv->private_info, LOCAL_ERR_PARAM);

  // from url -> server + dir
  ret = ParseURL(url, NULL, NULL, NULL, NULL, NULL, dir);
  if (ret < 0)
  {
    printf("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_mkdir] ERROR: incorrect url '%s'.\n", serv->id, url);
    local_err(LOCAL_ERR_URL);
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_mkdir] ParseURL(%s)= %s\n", serv->id, url, dir);

  // private_info file handle
  fh_aux = (struct nfi_local_fhandle *)malloc(sizeof(struct nfi_local_fhandle));
  NULL_RET_ERR(fh_aux, LOCAL_ERR_MEMORY);
  bzero(fh_aux, sizeof(struct nfi_local_fhandle));

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_mkdir] nfi_local_mkdir(%s)\n", serv->id, dir);

  ret = real_posix_mkdir(dir, /*attr->at_mode*/ 0777);
  if ((ret < 0) && (errno != EEXIST))
  {
    debug_error("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_mkdir] ERROR: real_posix_mkdir fails to mkdir '%s' in server %s.\n", serv->id, dir, serv->server);
    FREE_AND_NULL(fh_aux);
    return -1;
  }

  ret = real_posix_stat(dir, &st);
  if (ret < 0)
  {
    debug_error("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_mkdir] ERROR: real_posix_stat fails to stat '%s' in server %s.\n", serv->id, dir, serv->server);
    return ret;
  }

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_mkdir] nfi_local_mkdir(%s)=%d\n", serv->id, dir, ret);

  fh->type = NFIDIR;
  fh->priv_fh = (void *)fh_aux;

  fh->url = STRING_MISC_StrDup(url);
  if (fh->url == NULL)
  {
    local_err(LOCAL_ERR_MEMORY);
    FREE_AND_NULL(fh_aux);
    return -1;
  }

  local_2_nfi_attr(attr, &st);

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_mkdir] >> End\n", serv->id);

  return ret;
}

int nfi_local_opendir ( struct nfi_server *serv,  char *url, struct nfi_fhandle *fho )
{
  int    ret;
  char   dir[PATH_MAX];
  struct nfi_local_fhandle *fh_aux;

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_opendir] >> Begin\n", serv->id);

  // Check arguments...
  NULL_RET_ERR(serv, LOCAL_ERR_PARAM);
  NULL_RET_ERR(url,  LOCAL_ERR_PARAM);
  NULL_RET_ERR(fho,  LOCAL_ERR_PARAM);
  nfi_local_keep_connected(serv);
  NULL_RET_ERR(serv->private_info, LOCAL_ERR_PARAM);

  // from url -> server + dir
  ret = ParseURL(url, NULL, NULL, NULL, NULL, NULL, dir);
  if (ret < 0)
  {
    printf("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_opendir] ERROR: incorrect url '%s'.\n", serv->id, url);
    local_err(LOCAL_ERR_URL);
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_opendir] ParseURL(%s)= %s\n", serv->id, url, dir);

  // Copy url
  fho->url = strdup(url);
  NULL_RET_ERR(fho->url, LOCAL_ERR_MEMORY);

  // private_info file handle
  fh_aux = (struct nfi_local_fhandle *)malloc(sizeof(struct nfi_local_fhandle));
  NULL_RET_ERR(fh_aux, LOCAL_ERR_MEMORY);
  bzero(fh_aux, sizeof(struct nfi_local_fhandle));

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_opendir] nfi_local_opendir(%s)\n", serv->id, dir);

  fh_aux->dir = real_posix_opendir(dir);
  if (fh_aux->dir == NULL)
  {
    debug_error("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_opendir] ERROR: real_posix_opendir fails to opendir '%s' in server %s.\n", serv->id, dir, serv->server);
    FREE_AND_NULL(fh_aux);
    FREE_AND_NULL(fho->url);
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_opendir] nfi_local_opendir(%s)=%p\n", serv->id, dir, fh_aux->dir);

  strcpy(fh_aux->path, dir);
  fho->type    = NFIDIR;
  fho->server  = serv;
  fho->priv_fh = (void *) fh_aux;

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_opendir] >> End\n", serv->id);

  return 0;
}

int nfi_local_readdir ( struct nfi_server *serv,  struct nfi_fhandle *fh, struct dirent *entry )
{
  struct nfi_local_fhandle *fh_aux;
  struct dirent *ent;

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_readdir] >> Begin\n", serv->id);

  // Check arguments...
  NULL_RET_ERR(serv,        LOCAL_ERR_PARAM);
  NULL_RET_ERR(fh,          LOCAL_ERR_PARAM);
  NULL_RET_ERR(fh->priv_fh, LOCAL_ERR_PARAM);
  if (fh->type != NFIDIR) {
    local_err(LOCAL_ERR_NOTDIR);
    return -1;
  }
  nfi_local_keep_connected(serv);
  NULL_RET_ERR(serv->private_info, LOCAL_ERR_PARAM);

  // private_info file handle
  fh_aux = (struct nfi_local_fhandle *)fh->priv_fh;

  // cleaning entry values...
  memset(entry, 0, sizeof(struct dirent));

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_readdir] nfi_local_readdir(%p)\n", serv->id, fh_aux->dir);

  ent = real_posix_readdir(fh_aux->dir);
  if (ent == NULL)
  {
    debug_error("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_readdir] ERROR: real_posix_readdir fails to open '%p' in server %s.\n", serv->id, fh_aux->dir, serv->server);
    return -1;
  }

  memcpy(entry, ent, sizeof(struct dirent));

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_readdir] nfi_local_readdir(%p)=%p\n", serv->id, fh_aux->dir, entry);
  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_readdir] >> End\n", serv->id);

  return 0;
}

int nfi_local_closedir ( struct nfi_server *serv,  struct nfi_fhandle *fh )
{
  struct nfi_local_fhandle *fh_aux;

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_closedir] >> Begin\n", serv->id);

  // Check arguments...
  NULL_RET_ERR(serv, LOCAL_ERR_PARAM);
  NULL_RET_ERR(fh,   LOCAL_ERR_PARAM);
  nfi_local_keep_connected(serv);
  NULL_RET_ERR(serv->private_info, LOCAL_ERR_PARAM);

  // Do closedir
  if (fh->priv_fh != NULL){
    // private_info file handle
    fh_aux = (struct nfi_local_fhandle *) fh->priv_fh;

    debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_closedir] nfi_local_closedir(%p)\n", serv->id, fh_aux->dir);

    real_posix_closedir(fh_aux->dir);

    debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_closedir] nfi_local_closedir(%p)=%d\n", serv->id, fh_aux->dir, 0);
  }

  // free memory
  FREE_AND_NULL(fh->priv_fh);
  FREE_AND_NULL(fh->url);

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_closedir] >> End\n", serv->id);

  return 0;
}

int nfi_local_rmdir ( struct nfi_server *serv,  char *url )
{
  int ret;
  char dir[PATH_MAX];

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_rmdir] >> Begin\n", serv->id);

  // Check arguments...
  NULL_RET_ERR(serv, LOCAL_ERR_PARAM);
  NULL_RET_ERR(url,  LOCAL_ERR_PARAM);
  nfi_local_keep_connected(serv);
  NULL_RET_ERR(serv->private_info, LOCAL_ERR_PARAM);

  // from url -> server + dir
  ret = ParseURL(url, NULL, NULL, NULL, NULL, NULL, dir);
  if (ret < 0)
  {
    printf("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_rmdir] ERROR: incorrect url '%s'.\n", serv->id, url);
    local_err(LOCAL_ERR_URL);
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_rmdir] ParseURL(%s)= %s\n", serv->id, url, dir);

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_rmdir] nfi_local_rmdir(%s)\n", serv->id, dir);

  ret = real_posix_rmdir(dir);
  if (ret < 0)
  {
    debug_error("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_rmdir] ERROR: real_posix_rmdir fails to rm '%s' in server %s.\n", serv->id, dir, serv->server);
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_rmdir] nfi_local_rmdir(%s)=%d\n", serv->id, dir, ret);
  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_rmdir] >> End\n", serv->id);

  return 0;
}

int nfi_local_statfs ( __attribute__((__unused__)) struct nfi_server *serv, __attribute__((__unused__)) struct nfi_info *inf )
{
  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_statfs] >> Begin\n", serv->id);

  //TODO
  /*
  struct local_info localinf;
  int ret;
        struct nfi_local_server *server_aux;

  // Check params
  if (serv == NULL) {
      debug_error("serv argument is NULL.\n");
      return -1;
  }

  if (inf == NULL) {
      debug_error("inf argument is NULL.\n");
      return -1;
  }

  // Check fields...
        nfi_local_keep_connected(serv);
  if (serv->private_info == NULL) {
      debug_error("serv->private_info field is NULL.\n");
      return -1;
  }

  server_aux = (struct nfi_local_server *)serv->private_info;
  if (server_aux == NULL) {
      debug_error("LOCAL_ERR_MEMORY\n");
      return -1;
  }
  ret = local_statfs(server_aux->fh, &localinf, server_aux->cl);
  if (ret <0) {
    debug_error("LOCAL_ERR_STATFS");
    return -1;
  }

  local_2_nfi_info(inf, &localinf);
  */

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_statfs] >> End\n", serv->id);

  return 0;
}


int nfi_local_preload(__attribute__((__unused__)) struct nfi_server *serv, __attribute__((__unused__)) char *url, __attribute__((__unused__)) char *virtual_path, __attribute__((__unused__)) char *storage_path, __attribute__((__unused__)) int opt)
{
  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_preload] >> Begin\n", serv->id);

  //TODO

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_preload] >> End\n", serv->id);

  return 0;
}


int nfi_local_flush ( __attribute__((__unused__)) struct nfi_server *serv,  __attribute__((__unused__)) char *url, __attribute__((__unused__)) char *virtual_path, __attribute__((__unused__)) char *storage_path, __attribute__((__unused__)) int opt )
{
  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_flush] >> Begin\n", serv->id);

  //TODO

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_flush] >> End\n", serv->id);

  return 0;
}


/* ................................................................... */
