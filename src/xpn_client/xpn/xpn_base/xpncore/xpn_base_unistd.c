
  /*
   *  Copyright 2000-2023 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
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


#include "xpn/xpn_base/xpn_base_unistd.h"


extern struct xpn_filedesc *xpn_file_table[XPN_MAX_FILE];

//extern pthread_mutex_t global_mt;

void XpnShowFileTable();


/*
 * Internal
 */

int xpn_internal_creat(const char *path, mode_t perm, struct xpn_fh **vfh, struct xpn_metadata **mdata)
{
  char abs_path[PATH_MAX], url_serv[PATH_MAX];
  struct nfi_server **servers;
  struct xpn_metadata *mdata_aux;
  struct nfi_attr attr;
  int res, err, i, j, n, pd;
  struct xpn_fh *vfh_aux;

  XPN_DEBUG_BEGIN_ARGS1(path)

  if(path == NULL)
  {
    xpn_err(XPNERR_PARAM);
    res = -1;
    XPN_DEBUG_END
    return res;
  }

  res = XpnGetAbsolutePath(path, abs_path); // this function generates the absolute path 
  if(res<0)
  {
    xpn_err(XPNERR_PATH_NOEXIST);
    res = -1;
    XPN_DEBUG_END_ARGS1(path)
    return res;
  }

  pd = XpnGetPartition(abs_path); // return partition's id and remove partition name from abs_path 
  if(pd<0)
  {
    xpn_err(XPNERR_PART_NOEXIST);
    res = -1;
    XPN_DEBUG_END_ARGS1(path)
    return res;
  }

  /* params:
   * flag operation, partition id, absolute path, file descriptor, pointer to server
   */
  servers = NULL;
  n = XpnGetServers(op_xpn_creat, pd, abs_path, -1, &servers, XPN_DATA_SERVER);
  if(n<=0)
  {
    //free(servers);

    res = -1;
    XPN_DEBUG_END_ARGS1(path)
    return res;
  }

  bzero(&attr, sizeof(struct nfi_attr));
  attr.at_mode = perm;

  vfh_aux = (struct xpn_fh *)malloc(sizeof(struct xpn_fh));

  // vfh construction 
  if(vfh_aux == NULL)
  {
    xpn_err(XPNERR_NOMEMORY);
    free(servers);
    res = -1;
    XPN_DEBUG_END_ARGS1(path)
    return res;
  }

  vfh_aux->n_nfih = n;
  vfh_aux->nfih = (struct nfi_fhandle **)malloc(sizeof(struct nfi_fhandle*) * n);
  if(vfh_aux->nfih == NULL)
  {
    xpn_err(XPNERR_NOMEMORY);
    free(servers);
    res = -1;
    XPN_DEBUG_END_ARGS1(path)
    return res;
  }

  for(i=0;i<n;i++){
    vfh_aux->nfih[i] = NULL;
  }

  mdata_aux = (struct xpn_metadata *)malloc(sizeof(struct xpn_metadata));
  if(mdata_aux == NULL)
  {
    free(servers);
    // also indirect pointers 
    for(i=0;i<n;i++)
    {
      if(vfh_aux->nfih[i] != NULL)
      {
        if(vfh_aux->nfih[i]->priv_fh != NULL){
          vfh_aux->nfih[i]->server->ops->nfi_close( vfh_aux->nfih[i]->server, vfh_aux->nfih[i]);
        }
        free(vfh_aux->nfih[i]);
      }
    }

    free(vfh_aux->nfih);
    free(vfh_aux);
    free(mdata_aux);
    res = -1;
    XPN_DEBUG_END_ARGS1(path)
    return res;
  }

  XpnCreateMetadata(mdata_aux, pd, abs_path);

  for(j=0;j<n;j++)
  {
    i = XpnGetMetadataPos(mdata_aux, j);

    XpnGetURLServer(servers[i], abs_path, url_serv);
    vfh_aux->nfih[i] = (struct nfi_fhandle*)malloc(sizeof(struct nfi_fhandle));
    bzero(vfh_aux->nfih[i], sizeof(struct nfi_fhandle));
    if(vfh_aux->nfih[i] == NULL)
    {
      free(servers);
      res = -1;
      XPN_DEBUG_END_ARGS1(path)
      return res;
    }

    // Worker
    servers[i]->wrk->thread = servers[i]->xpn_thread;
    nfi_worker_do_create(servers[i]->wrk, url_serv, &attr, vfh_aux->nfih[i]);
  }

  // Wait
  err = 0;
  for(j=0;j<n;j++)
  {
    i = XpnGetMetadataPos(mdata_aux, j) ;
    res = nfiworker_wait(servers[i]->wrk) ;
    // error checking 
    if((res<0)&&(!err))
    {
      err = 1;
      // erase the file create before de server number i 
    /*
      for(j=0; j<i; j++){
        XpnGetURLServer(servers[j], abs_path, url_serv);
        nfi_worker_do_remove(servers[j]->wrk, url_serv);
        nfi_worker_wait(servers[j]->wrk);
      }
    }else{
      if((res>=0)&&(err)){
        XpnGetURLServer(servers[i], abs_path, url_serv);
        nfi_worker_do_remove(servers[i]->wrk, url_serv);
        nfi_worker_wait(servers[i]->wrk);
      }
    */
    }
  }

  // error checking 
  if(err)
  {
    xpn_err(XPNERR_CREATE);
    free(servers);
    for(i=0;i<n;i++)
    {
      if(vfh_aux->nfih[i] != NULL)
      {
        if(vfh_aux->nfih[i]->priv_fh != NULL){
          vfh_aux->nfih[i]->server->ops->nfi_close( vfh_aux->nfih[i]->server, vfh_aux->nfih[i]);
        }
        free(vfh_aux->nfih[i]);
      }
    }

    free(vfh_aux->nfih);
    res = -1;
    XPN_DEBUG_END_ARGS1(path)
    return res;
  }

  res = XpnUpdateMetadata(mdata_aux, n, servers, vfh_aux, abs_path);
  if(res<0)
  {
    free(servers);
    // also indirect pointers 
    for(i=0;i<n;i++)
    {
      if(vfh_aux->nfih[i] != NULL)
      {
        if(vfh_aux->nfih[i]->priv_fh != NULL){
          vfh_aux->nfih[i]->server->ops->nfi_close( vfh_aux->nfih[i]->server, vfh_aux->nfih[i]);
        }
        free(vfh_aux->nfih[i]);
      }
    }

    free(vfh_aux->nfih);
    free(vfh_aux);
    free(mdata_aux);
    res = -1;
    XPN_DEBUG_END_ARGS1(path)
    return res;
  }

  (*vfh )= vfh_aux;
  (*mdata )= mdata_aux;
  free(servers);

  xpn_err(XPNERR_PARAM);
  res = 0;
  XPN_DEBUG_END_ARGS1(path)
  return res;
}

int xpn_internal_open ( const char *path, struct xpn_fh *vfh, struct xpn_metadata *mdata, int mode )
{
  char abs_path[PATH_MAX];
  struct nfi_server **servers;
  int n, pd, i;
  int res = -1;

  XPN_DEBUG_BEGIN_ARGS1(path)

  res = XpnGetAbsolutePath(path, abs_path); // this function generates the absolute path 
  if (res < 0)
  {
    xpn_err(XPNERR_PATH_NOEXIST);
    XPN_DEBUG_END_ARGS1(path)
    return res;
  }

  pd = XpnGetPartition(abs_path); // returns partition id and remove partition name from abs_path 
  if (pd < 0)
  {
    xpn_err(XPNERR_PART_NOEXIST);
    XPN_DEBUG_END_ARGS1(path)
    return pd;
  }

  res = XpnSearchFile(abs_path);
  if (res < 0)
  {
    // FIXME: CAUTION, this caused the call to fail some changes before, although now it seems to work.
    /* params:
     * flag operation, partition id, absolute path, file descriptor, pointer to server
     */
    servers = NULL;
    n = XpnGetServers(op_xpn_open, pd, abs_path, -1, &servers, XPN_DATA_SERVER);
    if (n <= 0)
    {
      //free(servers);
      XPN_DEBUG_END_ARGS1(path)
      return res;
    }

    if (mdata == NULL)
    {
      mdata = (struct xpn_metadata *)malloc(sizeof(struct xpn_metadata));
      if(mdata == NULL)
      {
        free(servers);
        XPN_DEBUG_END_ARGS1(path)
        return res;
      }
      if(vfh == NULL)
      {
        vfh = (struct xpn_fh *)malloc(sizeof(struct xpn_fh));
        if(vfh == NULL)
        {
          free(servers);
          XPN_DEBUG_END_ARGS1(path)
          return res;
        }

        vfh->n_nfih = n;
        vfh->nfih = (struct nfi_fhandle **)malloc(sizeof(struct nfi_fhandle *)*n);
        for(i=0;i<n;i++){
          vfh->nfih[i] = NULL;
        }
      }
      // read the metadata 
      memset(mdata, 0, sizeof(struct xpn_metadata));
      res = XpnReadMetadata(mdata, n, servers, vfh, abs_path, pd);
      if(res<0)
      {
        free(servers);
        free(vfh->nfih);
        free(vfh);
        free(mdata);
        XPN_DEBUG_END_ARGS1(path)
        return res;
      }
    }

    free(servers);
    res = XpnSearchSlotFile(pd, abs_path, vfh, mdata, mode);
  }
  else
  {
    xpn_file_table[res]->links++;
  }

  XPN_DEBUG_END_ARGS1(path)
  return res;
}

int xpn_internal_remove(const char *path)
{
  char abs_path[PATH_MAX], url_serv[PATH_MAX];
  int res, err, i, n, pd;
  struct nfi_server **servers;

  if(path == NULL)
  {
    xpn_err(XPNERR_PARAM);
    return -1;
  }

  res = XpnGetAbsolutePath(path, abs_path); // esta funcion genera el path absoluto 
  if(res<0)
  {
    xpn_err(XPNERR_PATH_NOEXIST);
    return -1;
  }

  pd = XpnGetPartition(abs_path); // return partition's id 
  if(pd<0)
  {
    xpn_err(XPNERR_PART_NOEXIST);
    return -1;
  }

  /* params:
   * flag operation, partition id, absolute path, file descriptor, pointer to server
   */
  servers = NULL;
  n = XpnGetServers(op_xpn_remove, pd, abs_path, -1, &servers, XPN_DATA_SERVER);
  if(n<=0){
    // free(servers); 
    return -1;
  }

  for(i=0;i<n;i++)
  {
    XpnGetURLServer(servers[i], abs_path, url_serv);

    // Worker
    servers[i]->wrk->thread = servers[i]->xpn_thread;
    nfi_worker_do_remove(servers[i]->wrk, url_serv);
  }

  // Wait
  err = 0;
  for(i=0;i<n;i++)
  {
    res = nfiworker_wait(servers[i]->wrk);
    // error checking 
    if((res<0)&&(!err)){
      err = 1;
    }
  }

  // error checking 
  if(err)
  {
    xpn_err(XPNERR_REMOVE);
    free(servers);
    return -1;
  }

  free(servers);
  xpn_err(XPN_OK);
  return 0;
}


/*
 * API
 */

int xpn_base_creat(const char *path, mode_t perm)
{
  struct xpn_fh *vfh;
  struct xpn_metadata *mdata;
  int res;

  XPN_DEBUG_BEGIN_ARGS1(path)

  if((path == NULL)||(strlen(path)>PATH_MAX))
  {
    res = -1;
    XPN_DEBUG_END_ARGS1(path)
    return res;
  }

  vfh = NULL;
  mdata = NULL;
  xpn_internal_remove(path);

  res = xpn_internal_creat(path, perm, &vfh, &mdata);
  if (res < 0)
  {
    XPN_DEBUG_END_ARGS1(path)
    return res;
  }

  res = xpn_internal_open(path, vfh, mdata, perm);
  if (res < 0)
  {
    XPN_DEBUG_END_ARGS1(path)
    return res;
  }

  xpn_err(XPN_OK);
  XPN_DEBUG_END_ARGS1(path)
  return res;
}

int xpn_base_open(const char *path, int flags, mode_t mode)
{
  struct xpn_fh *vfh;
  struct xpn_metadata *mdata;
  int md;
  int res = -1;

  XPN_DEBUG_BEGIN_ARGS1(path)

  if((path == NULL)||(strlen(path)>PATH_MAX))
  {
    XPN_DEBUG_END_ARGS1(path)
    return res;
  }

  if ((flags & O_CREAT) > 0)
  {
    if (mode > 0177777)
    {
      XPN_DEBUG_END_ARGS1(path)
      return res;
    }
  }

  vfh = NULL;
  mdata = NULL;

  /*
  flags = flags & (O_TRUNC | O_CREAT | O_RDWR | O_WRONLY |  O_RDONLY);
  if( (flags != (O_TRUNC | O_CREAT | O_RDWR )) &&
      (flags != (O_TRUNC | O_CREAT | O_WRONLY )) &&
      (flags != (O_TRUNC | O_CREAT | O_RDONLY )) &&
      (flags != (O_CREAT | O_RDWR )) &&
      (flags != (O_CREAT | O_WRONLY)) &&
      (flags != (O_CREAT | O_RDONLY )) &&
      (flags != (O_RDWR )) &&
      (flags != (O_RDONLY)) &&
      (flags != (O_WRONLY)) ){
    XPN_DEBUG_END_ARGS1(path)
    return res;
  }
  */
  if( (O_WRONLY == (flags & O_WRONLY)) || (O_RDWR == (flags & O_RDWR)) )
  {
    if(O_TRUNC == (flags & O_TRUNC)){
      res = xpn_internal_remove(path);
    }
    if(  (O_TRUNC == (flags & O_TRUNC)) ||((O_CREAT == (flags & O_CREAT)) && (mode != 0))  )
    {
      //mode = mode & MASK;
      res = xpn_internal_creat(path, mode, &vfh, &mdata);
      if (res < 0)
      {
        XPN_DEBUG_END_ARGS1(path)
        return res;
      }
    }
  }

  md = (flags & O_RDWR)|(flags & O_RDONLY)|(flags & O_WRONLY);

  res = xpn_internal_open(path, vfh, mdata, md);

  xpn_err(XPN_OK);

  XPN_DEBUG_END_ARGS1(path)
  return res;
}

int xpn_base_close(int fd)
{
  int res, i;

  XPN_DEBUG_BEGIN_CUSTOM("%d", fd)

  if((fd<0)||(fd>XPN_MAX_FILE-1))
  {
    // xpn_err 
    res = -1;
    XPN_DEBUG_END_CUSTOM("%d", fd)
    return res;
  }

  if(xpn_file_table[fd] == NULL)
  {
    // xpn_err 
    res = -1;
    XPN_DEBUG_END_CUSTOM("%d", fd)
    return res;
  }

  /*
  int n_threads = 0;
  struct nfi_server **servers;
  servers = NULL;
  int n = XpnGetServers(op_xpn_close, xpn_file_table[fd]->part->id, NULL, -1, &servers, XPN_DATA_SERVER);
  if(n<=0){
    free(servers);
    res = -1;
    return res;
  }
  */

  xpn_file_table[fd]->links--;
  if(xpn_file_table[fd]->links == 0)
  {
    for(i=0;i<xpn_file_table[fd]->data_vfh->n_nfih;i++)
    {
      if(xpn_file_table[fd]->data_vfh->nfih[i] != NULL)
      {
        if(xpn_file_table[fd]->data_vfh->nfih[i]->priv_fh != NULL)
        {
          //n_threads++;

          // Worker
          //servers[i]->wrk->thread = servers[i]->xpn_thread;
          //nfi_worker_do_close(servers[i]->wrk, xpn_file_table[fd]->data_vfh->nfih[i]);

          //TODO: old
          xpn_file_table[fd]->data_vfh->nfih[i]->server->ops->nfi_close( xpn_file_table[fd]->data_vfh->nfih[i]->server, xpn_file_table[fd]->data_vfh->nfih[i]);
        }
        //TODO: old
        free(xpn_file_table[fd]->data_vfh->nfih[i]);
      }
    }

    // Wait
    /*int err = 0;
    for(int j=0;j<n_threads;j++)
    {
      res = nfi_worker_wait(servers[j]->wrk);
      if((res<0)&&(!err)){
        err = 1;
      }
    }*/

    /*for(i=0;i<xpn_file_table[fd]->data_vfh->n_nfih;i++)
    {
      if(xpn_file_table[fd]->data_vfh->nfih[i] != NULL)
      {
        if(xpn_file_table[fd]->data_vfh->nfih[i]->priv_fh != NULL){
          free(xpn_file_table[fd]->data_vfh->nfih[i]);
        }
      }
    }*/

    //free(servers);
    free(xpn_file_table[fd]->data_vfh->nfih);
    free(xpn_file_table[fd]->data_vfh);
    free(xpn_file_table[fd]->mdata->policy);
    free(xpn_file_table[fd]->mdata);
    free(xpn_file_table[fd]);
    xpn_file_table[fd] = NULL;

    // error checking
    /*if(err){
      return -1;
    } */
  }
  xpn_err(XPN_OK);

  res = 0;
  XPN_DEBUG_END_CUSTOM("%d", fd)
  return res;
}


ssize_t xpn_base_read(int fd, void *buffer, size_t size)
{
  ssize_t res = -1;

  XPN_DEBUG_BEGIN_CUSTOM("%d, %zu", fd, size)

  if (xpn_file_table[fd] != NULL)
  {
    if ((unsigned long)(size) > (unsigned long)(xpn_file_table[fd]->block_size))
    {
      res = xpn_pread(fd, buffer, size, xpn_file_table[fd]->offset);
    }
    else {
      res = xpn_sread(fd, buffer, size, xpn_file_table[fd]->offset);
    }
  }
  else 
  {
    XpnShowFileTable();
    errno = EBADF;
  }

  XPN_DEBUG_END_CUSTOM("%d, %zu", fd, size)
  return res;
}

ssize_t xpn_base_write ( int fd, const void *buffer, size_t size )
{
  ssize_t res = -1;

  XPN_DEBUG_BEGIN_CUSTOM("%d, %zu", fd, size)

  if (xpn_file_table[fd] != NULL)
  {
    if ((unsigned long)(size) >= (unsigned long)(xpn_file_table[fd]->block_size))
    {
      res = xpn_pwrite(fd, buffer, size, xpn_file_table[fd]->offset);
    }
    else {
      res = xpn_swrite(fd, buffer, size, xpn_file_table[fd]->offset);
    }
  }
  else
  {
    XpnShowFileTable();
    errno = EBADF;
  }

  XPN_DEBUG_END_CUSTOM("%d, %zu", fd, size)
  return res;
}

ssize_t xpn_sread(int fd, const void *buffer, size_t size, off_t offset)
{
  off_t new_offset, l_offset;
  size_t initial_size;
  size_t l_size;
  int l_serv;
  size_t count = 0;
  struct nfi_server **servers;
  int n, res;

  XPN_DEBUG_BEGIN_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)

  if((fd<0)||(fd>XPN_MAX_FILE)||(xpn_file_table[fd] == NULL))
  {
    XpnShowFileTable();
    errno = EBADF;
    res = -1;
    XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
    return res;
  }

  if(buffer == NULL)
  {
    errno = EFAULT;
    res = -1;
    XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
    return res;
  }

  if(size == 0)
  {
    res = 0;
    XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
    return res;
  }

  if(xpn_file_table[fd]->mode == O_WRONLY)
  {
    res = -1;
    XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
    return res;
  }

  if(xpn_file_table[fd]->type == XPN_DIR)
  {
    res = -1;
    XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
    return res;
  }

  /* params:
   * flag operation, partition id, absolute path, file descriptor,
   * pointer to servers */
  servers = NULL;
  n = XpnGetServers(op_xpn_read, xpn_file_table[fd]->part->id, NULL, fd, &servers, XPN_DATA_SERVER);
  if(n<=0)
  {
    if (servers != NULL){ 
      free(servers); servers=NULL;
    }
    return -1;
  }

  initial_size = size;
  new_offset = offset;
  count = 0;

  while(size > count)
  {
    XpnGetBlock(fd, new_offset, &l_offset, &l_serv);

    l_size = xpn_file_table[fd]->block_size - (new_offset%xpn_file_table[fd]->block_size);

    // If l_size > the remaining bytes to read/write, then adjust l_size
    if ((size - count) < l_size){
      l_size = size - count;
    }

    if (xpn_file_table[fd]->data_vfh->nfih[l_serv] == NULL)
    {
      res = XpnGetFh( xpn_file_table[fd]->mdata, &(xpn_file_table[fd]->data_vfh->nfih[l_serv]), servers[l_serv], xpn_file_table[fd]->path);
      if(res<0){
        break;
      }
    }

    res = servers[l_serv]->ops->nfi_read(servers[l_serv], xpn_file_table[fd]->data_vfh->nfih[l_serv], (char *)buffer + count, l_offset, l_size) ;
    if(res<0){
      break;
    }

    count = l_size + count;
    new_offset = offset + count;
  }

  if (servers != NULL) {
    free(servers); servers=NULL;
  }

  if (size == count){
    return initial_size;
  }
  else{
    return -1;
  }
}

ssize_t xpn_pread(int fd, void *buffer, size_t size, off_t offset)
{
  ssize_t *res_v, total;
  int res, n, i, j, err;
  struct nfi_server **servers;
  struct nfi_worker_io **io;
  int *ion, max;
  void *new_buffer;

  XPN_DEBUG_BEGIN_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)

  if((fd<0)||(fd>XPN_MAX_FILE)||(xpn_file_table[fd] == NULL))
  {
    XpnShowFileTable();
    errno = EBADF;
    res = -1;
    XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
    return res;
  }

  if(buffer == NULL)
  {
    errno = EFAULT;
    res = -1;
    XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
    return res;
  }

  if(size == 0)
  {
    res = 0;
    XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
    return res;
  }

  if(xpn_file_table[fd]->mode == O_WRONLY)
  {
    res = -1;
    XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
    return res;
  }

  if(xpn_file_table[fd]->type == XPN_DIR)
  {
    res = -1;
    XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
    return res;
  }

  /* params:
   * flag operation, partition id, absolute path, file descriptor,
   * pointer to servers */
  servers = NULL;
  n = XpnGetServers(op_xpn_read, xpn_file_table[fd]->part->id, NULL, fd, &servers, XPN_DATA_SERVER);
  if(n<=0)
  {
    if (servers != NULL) { free(servers); servers=NULL; }

    res = -1;
    XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
    return res;
  }

  io = (struct nfi_worker_io **)malloc(sizeof(struct nfi_worker_io *)*n);
  if(io == NULL)
  {
    if (servers != NULL) {
      free(servers); servers=NULL;
    }

    res = -1;
    XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
    return res;
  }

  ion = (int *)malloc(sizeof(int)*n);
  if(ion == NULL)
  {
    if (servers != NULL) {
      free(servers); servers=NULL;
    }
    free(io);

    res = -1;
    XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
    return res;
  }

  bzero(ion, sizeof(int)*n);

  // calculate the maximum number of operations
  max = (size/xpn_file_table[fd]->block_size) + 1;
  if(size%xpn_file_table[fd]->block_size != 0){
    max++;
  }

  // create
  for(i=0; i<n; i++)
  {
    io[i] = (struct nfi_worker_io *)malloc(sizeof(struct nfi_worker_io)*max);
    if(io[i] == NULL)
    {
      if (servers != NULL) {
        free(servers); servers=NULL;
      }

      for(j=0;j<i;j++){
        free(io[j]);
      }
      free(io);
      free(ion);

      res = -1;
      XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
      return res;
    }
  }

  for(i=0; i<n; i++)
  {
    io[i][0].offset = 0;
    io[i][0].size = 0;
  }

  // Calculate which blocks to read from each server
  new_buffer = XpnReadBlocks(fd, buffer, size, offset, &io, &ion, n);
  if(new_buffer == NULL)
  {
    if (servers != NULL) {
      free(servers); servers=NULL;
    }

    for(i=0;i<n;i++){
      free(io[i]);
    }

    free(io);
    free(ion);
    return -1;
  }

  // TODO: Expand is not reentrant. Using only one 'wrk' (struct nfi_worker) inside servers[i] (struct nfi_server) does not allow two nfi operations at the same time
  //pthread_mutex_lock(&(global_mt)); // It is better to do this in clients when necessary

  // operation
  for(j=0; j<n; j++)
  {
    i = XpnGetMetadataPos(xpn_file_table[fd]->mdata, j);

    if(ion[i] != 0)
    {
      res = XpnGetFh( xpn_file_table[fd]->mdata, &(xpn_file_table[fd]->data_vfh->nfih[i]), servers[i], xpn_file_table[fd]->path);
      if(res<0){
        if (servers != NULL) {
          free(servers); servers=NULL;
        }

        for(i=0;i<n;i++){
          free(io[i]);
        }

        free(io);
        free(ion);

        res = -1;
        XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
        return res;
      }

      //Worker
      servers[i]->wrk->thread = servers[i]->xpn_thread;
      nfi_worker_do_read(servers[i]->wrk, xpn_file_table[fd]->data_vfh->nfih[i], io[i], ion[i]);
    }
  }

  res_v = (ssize_t *)malloc(sizeof(ssize_t)*n);
  if(res_v == NULL)
  {
    if (servers != NULL) {
      free(servers); servers=NULL;
    }

    for(j=0;j<n;j++){
      free(io[j]);
    }

    free(io);
    free(ion);

    return -1;
  }

  bzero(res_v, sizeof(ssize_t)*n);

  err = 0;
  for(i=0; i<n; i++)
  {
    res_v[i] = nfiworker_wait(servers[i]->wrk);
    if (res_v[i] < 0) {
      err = 1;
    }
  }

  //pthread_mutex_unlock(&(global_mt));

  XpnReadBlocksFinish(fd, buffer, size, offset, &io, &ion, n, new_buffer);

  total = -1;
  if (!err)
  {
    total = XpnReadGetTotalBytes(fd, res_v, n);

    if(total > 0){
      xpn_file_table[fd]->offset += total;
    }
  }

  if (servers != NULL) {
    free(servers); servers=NULL;
  }

  for(j=0;j<n;j++){
    free(io[j]);
  }

  free(io);
  free(ion);

  res = total;
  XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
  return res;
}

ssize_t xpn_swrite(int fd, const void *buffer, size_t size, off_t offset)
{
  off_t new_offset, l_offset;
  size_t initial_size;
  size_t l_size;
  int l_serv;
  size_t count = 0;
  struct nfi_server **servers;
  int n, res;

  XPN_DEBUG_BEGIN_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)

  if((fd<0)||(fd>XPN_MAX_FILE)){
    return -1;
  }

  if((xpn_file_table[fd] == NULL)||(buffer == NULL)){
    return -1;
  }

  if(size == 0){
    return 0;
  }

  if(xpn_file_table[fd]->mode == O_RDONLY){
    return -1;
  }

  if(xpn_file_table[fd]->type == XPN_DIR){
    return -1;
  }

  /* params:
   * flag operation, partition id, absolute path, file descriptor,
   * pointer to servers */
  servers = NULL;
  n = XpnGetServers(op_xpn_write, xpn_file_table[fd]->part->id, NULL, fd, &servers, XPN_DATA_SERVER);
  if(n<=0){
    if (servers != NULL) {
      free(servers); servers=NULL;
    }

    return -1;
  }

  initial_size = size;
  new_offset = offset;
  count = 0;

  while(size > count)
  {
    XpnGetBlock(fd, new_offset, &l_offset, &l_serv);

    l_size = xpn_file_table[fd]->block_size - (new_offset%xpn_file_table[fd]->block_size);

    // If l_size > the remaining bytes to read/write, then adjust l_size
    if ((size - count) < l_size){
      l_size = size - count;
    }

    if (xpn_file_table[fd]->data_vfh->nfih[l_serv] == NULL)
    {
      res = XpnGetFh( xpn_file_table[fd]->mdata, &(xpn_file_table[fd]->data_vfh->nfih[l_serv]), servers[l_serv], xpn_file_table[fd]->path);
      if(res<0){
        break;
      }
    }

    res = servers[l_serv]->ops->nfi_write(servers[l_serv], xpn_file_table[fd]->data_vfh->nfih[l_serv], (char *)buffer + count, l_offset, l_size) ;
    if(res<0){
      break;
    }

    count = l_size + count;
    new_offset = offset + count;
  }

  if (servers != NULL) {
    free(servers); servers=NULL;
  }

  if (size == count){
    return initial_size;
  }
  else{
    return -1;
  }
}

ssize_t xpn_pwrite(int fd, const void *buffer, size_t size, off_t offset)
{
  ssize_t *res_v, total;
  int res, n, i, j, err;
  struct nfi_server **servers;
  struct nfi_worker_io **io;
  int *ion, max;
  void *new_buffer;

  XPN_DEBUG_BEGIN_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)

  if((fd<0)||(fd>XPN_MAX_FILE)){
    return -1;
  }

  if((xpn_file_table[fd] == NULL)||(buffer == NULL)){
    return -1;
  }

  if(size == 0){
    return 0;
  }

  if(xpn_file_table[fd]->mode == O_RDONLY){
    return -1;
  }

  if(xpn_file_table[fd]->type == XPN_DIR){
    return -1;
  }

  /* params:
   * flag operation, partition id, absolute path, file descriptor,
   * pointer to servers */
  servers = NULL;
  n = XpnGetServers(op_xpn_write, xpn_file_table[fd]->part->id, NULL, fd, &servers, XPN_DATA_SERVER);
  if(n<=0){
    if (servers != NULL) {
      free(servers); servers=NULL;
    }

    res = -1;
    XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
    return res;
  }

  io = (struct nfi_worker_io **)malloc(sizeof(struct nfi_worker_io *)*n);
  if(io == NULL)
  {
    if (servers != NULL) {
      free(servers); servers=NULL;
    }

    res = -1;
    return res;
  }

  ion = (int *)malloc(sizeof(int)*n);
  if(ion == NULL)
  {
    if (servers != NULL) {
      free(servers); servers=NULL;
    }

    free(io);
    res = -1;
    return res;
  }

  bzero(ion, sizeof(int)*n);

  // calculate the maximum number of operations
  max = (size/xpn_file_table[fd]->block_size) + 1;
  if(size%xpn_file_table[fd]->block_size != 0){
    max++;
  }

  // create
  for(i=0; i<n; i++)
  {
    io[i] = (struct nfi_worker_io *)malloc(sizeof(struct nfi_worker_io)*max);
    if(io[i] == NULL)
    {
      if (servers != NULL) {
        free(servers); servers=NULL;
      }

      for(j=0;j<i;j++){
        free(io[j]);
      }

      free(io);
      free(ion);
      return -1;
    }
  }

  for(i=0; i<n; i++)
  {
    io[i][0].offset = 0;
    io[i][0].size = 0;
  }

  // Calculate which blocks to write to each server
  new_buffer = XpnWriteBlocks(fd, buffer, size, offset, &io, &ion, n);
  if(new_buffer == NULL)
  {
    if (servers != NULL) {
      free(servers); servers=NULL;
    }

    for(i=0;i<n;i++){
      free(io[i]);
    }

    free(io);
    free(ion);
    return -1;
  }

  // TODO: Expand is not reentrant. Using only one 'wrk' (struct nfi_worker) inside servers[i] (struct nfi_server) does not allow two nfi operations at the same time
  //pthread_mutex_lock(&(global_mt)); // It is better to do this in clients when necessary

  for(j=0; j<n; j++)
  {
    i = XpnGetMetadataPos(xpn_file_table[fd]->mdata, j);

    if(ion[i] != 0)
    {
      res = XpnGetFh( xpn_file_table[fd]->mdata, &(xpn_file_table[fd]->data_vfh->nfih[i]), servers[i], xpn_file_table[fd]->path);
      if(res<0){
        if (servers != NULL) {
          free(servers); servers=NULL;
        }

        for(i=0;i<n;i++){
          free(io[i]);
        }

        free(io);
        free(ion);
        return -1;
      }

      /*
      switch(xpn_file_table[fd]->size_threads)
      {
        case -1:
          flag_thread = 0;
          break;
        case 0:
          flag_thread = 1;
          break;
        default:
          if(xpn_file_table[fd]->size_threads >= size){
            flag_thread = 1;
          }else{
            flag_thread = 0;
          }
      }
      */

      //Worker
      servers[i]->wrk->thread = servers[i]->xpn_thread;
      nfi_worker_do_write(servers[i]->wrk, xpn_file_table[fd]->data_vfh->nfih[i], io[i], ion[i]);
    }
  }

  res_v = (ssize_t *)malloc(sizeof(ssize_t)*n);
  if(res_v == NULL)
  {
    if (servers != NULL) {
      free(servers); servers=NULL;
    }

    for(j=0;j<n;j++){
      free(io[j]);
    }

    free(io);
    free(ion);

    return -1;
  }

  bzero(res_v, sizeof(ssize_t)*n);

  err = 0;
  for(i=0; i<n; i++)
  {
    res_v[i] = nfiworker_wait(servers[i]->wrk);
    if (res_v[i] < 0) {
      err = 1;
    }
  }

  //pthread_mutex_unlock(&(global_mt));

  XpnWriteBlocksFinish(fd, buffer, size, offset, &io, &ion, n, new_buffer);

  total = -1;
  if (!err)
  {
    total = XpnWriteGetTotalBytes(fd, res_v, n);

    if(total > 0){
      xpn_file_table[fd]->offset += total;
    }
  }

  if (servers != NULL) {
    free(servers); servers=NULL;
  }

  for(j=0;j<n;j++){
    free(io[j]);
  }

  free(io);
  free(ion);

  res = total;
  XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
  return res;
}

off_t xpn_base_lseek ( int fd, off_t offset, int flag )
{
  struct stat st;

  switch (flag)
  {
    case SEEK_SET:
      if(offset<0)
      {
        errno = EINVAL;
        return (off_t)-1;
      }
      else{
        xpn_file_table[fd]->offset = offset;
      }
      break;

    case SEEK_CUR:
      if(xpn_file_table[fd]->offset+offset<0)
      {
        errno = EINVAL;
        return (off_t)-1;
      }
      else{
        xpn_file_table[fd]->offset += offset;
      }
      break;

    case SEEK_END:
      if(xpn_base_fstat(fd, &st)<0)
      {
        errno = EBADF;
        return (off_t)-1;
      }
      if(st.st_size + offset<0)
      {
        errno = EINVAL;
        return (off_t)-1;
      }
      else{
        xpn_file_table[fd]->offset = st.st_size + offset;
      }
      break;

    default:
      errno = EINVAL;
      return (off_t)-1;
  }

  return xpn_file_table[fd]->offset;
}


int xpn_base_unlink(const char *path)
{
  int res;

  XPN_DEBUG_BEGIN_ARGS1(path)

  res = xpn_internal_remove(path);

  XPN_DEBUG_END_ARGS1(path)
  return res;
}

int xpn_base_rename(const char *path, const char *newpath)
{
  char abs_path[PATH_MAX], url_serv[PATH_MAX];
  char newabs_path[PATH_MAX], newurl_serv[PATH_MAX];
  struct nfi_server **servers;
  struct xpn_metadata *mdata_aux;
  int res, err, i, n, pd, newpd;
  struct xpn_fh *vfh_aux;

  if(path == NULL)
  {
    xpn_err(XPNERR_PARAM);
    return -1;
  }

  if(newpath == NULL)
  {
    xpn_err(XPNERR_PARAM);
    return -1;
  }

  res = XpnGetAbsolutePath(path, abs_path); // esta funcion genera el path absoluto 
  if(res<0)
  {
    xpn_err(XPNERR_PATH_NOEXIST);
    return -1;
  }

  res = XpnGetAbsolutePath(newpath, newabs_path); // esta funcion genera el path absoluto 
  if(res<0)
  {
    xpn_err(XPNERR_PATH_NOEXIST);
    return -1;
  }

  pd = XpnGetPartition(abs_path); // return partition's id 
  if(pd<0)
  {
    xpn_err(XPNERR_PART_NOEXIST);
    return -1;
  }

  newpd = XpnGetPartition(newabs_path); // return partition's id 
  if(newpd<0)
  {
    xpn_err(XPNERR_PART_NOEXIST);
    return -1;
  }

  if(pd != newpd){
    return -1;
  }

  /* params:
   * flag operation, partition id, absolute path, file descriptor, pointer to server
   */
  servers = NULL;
  n = XpnGetServers(op_xpn_rename, pd, abs_path, -1, &servers, XPN_DATA_SERVER);
  if(n<=0){
    //free(servers);
    return -1;
  }

  vfh_aux = (struct xpn_fh *)malloc(sizeof(struct xpn_fh));

  // construccion del vfh 
  if(vfh_aux == NULL)
  {
    xpn_err(XPNERR_NOMEMORY);
    free(servers);
    return -1;
  }

  vfh_aux->n_nfih = n;
  vfh_aux->nfih = (struct nfi_fhandle **)malloc(sizeof(struct nfi_fhandle*) * n);
  if(vfh_aux->nfih == NULL)
  {
    xpn_err(XPNERR_NOMEMORY);
    free(servers);
    return -1;
  }

  for(i=0;i<n;i++){
    vfh_aux->nfih[i] = NULL;
  }

  mdata_aux = (struct xpn_metadata *)malloc(sizeof(struct xpn_metadata));
  if(mdata_aux == NULL)
  {
    free(servers);
    free(vfh_aux->nfih);
    free(vfh_aux);
    return -1;
  }

  memset(mdata_aux, 0, sizeof(struct xpn_metadata));

  res = XpnReadMetadata(mdata_aux, n, servers, vfh_aux, abs_path, pd);
  if(res<0)
  {
    free(servers);
    // tambien los punteros indirectos 
    for(i=0;i<n;i++)
    {
      if(vfh_aux->nfih[i] != NULL)
      {
        if(vfh_aux->nfih[i]->priv_fh != NULL){
          vfh_aux->nfih[i]->server->ops->nfi_close( vfh_aux->nfih[i]->server, vfh_aux->nfih[i]);
        }

        free(vfh_aux->nfih[i]);
      }
    }
    free(vfh_aux->nfih);
    free(vfh_aux);
    free(mdata_aux);

    return -1;
  }

  res = XpnUpdateMetadata(mdata_aux, n, servers, vfh_aux, newabs_path);
  if(res<0)
  {
    free(servers);
    // tambien los punteros indirectos 
    for(i=0;i<n;i++)
    {
      if(vfh_aux->nfih[i] != NULL)
      {
        if(vfh_aux->nfih[i]->priv_fh != NULL){
          vfh_aux->nfih[i]->server->ops->nfi_close( vfh_aux->nfih[i]->server, vfh_aux->nfih[i]);
        }

        free(vfh_aux->nfih[i]);
      }
    }
    free(vfh_aux->nfih);
    free(vfh_aux);
    free(mdata_aux);

    return -1;
  }

  for(i=0;i<n;i++)
  {
    XpnGetURLServer(servers[i], abs_path, url_serv);
    XpnGetURLServer(servers[i], newabs_path, newurl_serv);

    // Worker
    servers[i]->wrk->thread = servers[i]->xpn_thread;
    nfi_worker_do_rename( servers[i]->wrk, url_serv, newurl_serv);
  }

  err = 0;
  for(i=0;i<n;i++)
  {
    res = nfiworker_wait(servers[i]->wrk) ;
    if((!err)&&(res<0)){
      err = 1;
    }
  }

  free(servers);
  // tambien los punteros indirectos 
  for(i=0;i<n;i++)
  {
    if(vfh_aux->nfih[i] != NULL)
    {
      if(vfh_aux->nfih[i]->priv_fh != NULL){
        vfh_aux->nfih[i]->server->ops->nfi_close( vfh_aux->nfih[i]->server, vfh_aux->nfih[i]);
      }
      free(vfh_aux->nfih[i]);
    }
  }

  free(vfh_aux->nfih);
  free(vfh_aux);
  free(mdata_aux);

  // error checking 
  if(err)
  {
    res = XpnUpdateMetadata(mdata_aux, n, servers, vfh_aux, newabs_path);
    if(res<0)
    {
      free(servers);
      // tambien los punteros indirectos 
      for(i=0;i<n;i++)
      {
        if(vfh_aux->nfih[i] != NULL)
        {
          if(vfh_aux->nfih[i]->priv_fh != NULL){
            vfh_aux->nfih[i]->server->ops->nfi_close( vfh_aux->nfih[i]->server, vfh_aux->nfih[i]);
          }

          free(vfh_aux->nfih[i]);
        }
      }
      free(vfh_aux->nfih);
      free(vfh_aux);
      free(mdata_aux);

      return -1;
    }
  }
  return 0;
}

int xpn_base_fstat(int fd, struct stat *sb)
{
  int res;

  XPN_DEBUG_BEGIN_CUSTOM("%d", fd)

  if(fd<0)
  {
    errno = EBADF;
    res = -1;
    XPN_DEBUG_END_CUSTOM("%d", fd)
    return res;
  }

  res = XpnGetAtribFd(fd , sb);

  XPN_DEBUG_END_CUSTOM("%d", fd)

  return res;
}


// FIXME: If the user has already opened the max number of files he is allowed to, then this call will fail with EMFILE.
// FIXME: xpn_base_stat really need to perform xpn_open+opendir?
int xpn_base_stat(const char *path, struct stat *sb)
{
  char abs_path[PATH_MAX];
  char abs_path2[PATH_MAX];
  int res, pd;
  //DIR *dir;
  //char *new_path;

  XPN_DEBUG_BEGIN_ARGS1(path)

  if ((path == NULL) || (strlen(path) == 0))
  {
    //xpn_err(XPNERR_PARAM);
    errno = ENOENT;
    res = -1;
    XPN_DEBUG_END_ARGS1(path)
    return res;
  }

  res = XpnGetAbsolutePath(path, abs_path); // this function generates the absolute path 
  if(res<0)
  {
    xpn_err(XPNERR_PATH_NOEXIST);
    res = -1;
    XPN_DEBUG_END_ARGS1(path)
    return res;
  }

  strcpy(abs_path2, abs_path);

  pd = XpnGetPartition(abs_path2); // return partition's id and remove partition name from abs_path 
  if(pd<0)
  {
    xpn_err(XPNERR_PART_NOEXIST);
    res = -1;
    XPN_DEBUG_END_ARGS1(path)
    return res;
  }

  res = XpnGetAtribPath(abs_path2, sb);

  XPN_DEBUG_END_ARGS1(path)
  return res;
}

int xpn_base_chown(__attribute__((__unused__)) const char *path, __attribute__((__unused__)) uid_t owner, __attribute__((__unused__)) gid_t group)
{
  return 0;
}

int xpn_base_fchown(int __attribute__((__unused__)) fd, __attribute__((__unused__)) uid_t owner, __attribute__((__unused__)) gid_t group)
{
  return 0;
}

int xpn_base_chmod(__attribute__((__unused__)) const char *path, __attribute__((__unused__)) mode_t mode)
{
  return 0;
}

int xpn_base_fchmod(__attribute__((__unused__)) int fd, __attribute__((__unused__)) mode_t mode)
{
  return 0;
}

int xpn_base_truncate(__attribute__((__unused__)) const char *path, __attribute__((__unused__)) off_t length)
{
  return 0;
}

int xpn_base_ftruncate(__attribute__((__unused__)) int fd, __attribute__((__unused__)) off_t length)
{
  return 0;
}

int xpn_base_dup(int fd)
{
  int i;

  if((fd > XPN_MAX_FILE-1)||(fd <0)){
    return -1;
  }

  if(xpn_file_table[fd] == NULL){
    return -1;
  }

  i =0;
  while((i<XPN_MAX_FILE-1)&&(xpn_file_table[i] != NULL)){
    i++;
  }
  if(i == XPN_MAX_FILE){
    // xpn_err() ? 
    return -1;
  }
  xpn_file_table[i] = xpn_file_table[fd];
  xpn_file_table[fd]->links++;

  return i;
}

int xpn_base_dup2(int fd, int fd2)
{
  if((fd > XPN_MAX_FILE-1)||(fd <0)){
    return -1;
  }
  if(xpn_file_table[fd] == NULL){
    return -1;
  }
  if((fd2 > XPN_MAX_FILE-1)||(fd2 <0)){
    return -1;
  }
  if(xpn_file_table[fd2] != NULL){
    return -1;
  }

  xpn_file_table[fd2] = xpn_file_table[fd];
  xpn_file_table[fd]->links++;

  return 0;
}

