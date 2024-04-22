
  /*
   *  Copyright 2000-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra, Dario Muñoz Muñoz
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

  #include "xpn/xpn_simple/xpn_rw.h"
  #include "ns.h"


   /* ... Global vars. / Variables globales ............................. */

   // extern pthread_mutex_t global_mt;
      extern void XpnShowFileTable();


   /* ... Functions / Funciones ......................................... */

ssize_t xpn_simple_read ( int fd, void *buffer, size_t size )
{
  ssize_t res = -1;

  XPN_DEBUG_BEGIN_CUSTOM("%d, %zu", fd, size);

  // (1) Check arguments...
  if ( (fd<0) || (fd>XPN_MAX_FILE) || (NULL == xpn_file_table[fd]) )
  {
     XpnShowFileTable();
     errno = EBADF;
     XPN_DEBUG_END_CUSTOM("%d, %zu", fd, size);
     return -1;
  }

  if (buffer == NULL)
  {
     errno = EFAULT;
     XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)xpn_file_table[fd]->offset);
     return -1;
  }

  if (size == 0) {
     XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)xpn_file_table[fd]->offset);
     return 0;
  }

  if (xpn_file_table[fd]->flags == O_WRONLY) {
     errno = EBADF;
     XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)xpn_file_table[fd]->offset);
     return -1;
  }

  if (xpn_file_table[fd]->type == XPN_DIR) {
     errno = EISDIR;
     XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)xpn_file_table[fd]->offset);
     return -1;
  }

  if ((unsigned long)(size) > (unsigned long)(xpn_file_table[fd]->block_size))
  {
    res = xpn_pread(fd, buffer, size, xpn_file_table[fd]->offset);
  }
  else {
    res = xpn_sread(fd, buffer, size, xpn_file_table[fd]->offset);
  }

  XPN_DEBUG_END_CUSTOM("%d, %zu", fd, size);

  return res;
}

ssize_t xpn_simple_write ( int fd, const void *buffer, size_t size )
{
  ssize_t res = -1;

  XPN_DEBUG_BEGIN_CUSTOM("%d, %zu", fd, size)

  // (1) Check arguments...
  if ( (fd<0) || (fd>XPN_MAX_FILE) )
  {
     XpnShowFileTable();
     errno = EBADF;
     XPN_DEBUG_END_CUSTOM("%d, %zu", fd, size);

     return -1;
  }

  if ( (xpn_file_table[fd] == NULL) || (buffer == NULL) )
  {
     errno = EFAULT;
     XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)xpn_file_table[fd]->offset);
     return -1;
  }

  if (size == 0)
  {
     XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)xpn_file_table[fd]->offset);
     return 0;
  }

  if (xpn_file_table[fd]->flags == O_RDONLY)
  {
     errno = EBADF;  
     XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)xpn_file_table[fd]->offset);
     return -1;
  }

  if (xpn_file_table[fd]->type == XPN_DIR)
  {
     errno = EISDIR;
     XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)xpn_file_table[fd]->offset);
     return -1;
  }
  // action
  if ((unsigned long)(size) >= (unsigned long)(xpn_file_table[fd]->block_size) || xpn_file_table[fd]->part->replication_level > 0)
  {
    res = xpn_pwrite(fd, buffer, size, xpn_file_table[fd]->offset);
  }
  else {
    res = xpn_swrite(fd, buffer, size, xpn_file_table[fd]->offset);
  }

  XPN_DEBUG_END_CUSTOM("%d, %zu", fd, size);

  return res;
}

ssize_t xpn_sread ( int fd, const void *buffer, size_t size, off_t offset )
{
  ssize_t res = -1;
  ssize_t count;
  off_t new_offset, l_offset;
  size_t l_size;
  int l_serv;
  struct nfi_server *servers = NULL;
  int n;

  res   = -1 ;
  count = 0 ;
  XPN_DEBUG_BEGIN_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset);

  // (1) Check arguments in xpn_simple_read

  // (2) Get servers...
  servers = NULL;

  n = XpnGetServers(xpn_file_table[fd]->part->id, fd, &servers);
  if (n <= 0)
  {
    count = -1;
    goto cleanup_xpn_sread;
  }

  new_offset = offset;
  count = 0;

  char *hostip = ns_get_host_ip();
  char hostname[1024];
  ns_get_hostname(hostname);
  int serv_client = -1;
  for(int i=0; i<n; i++){
    XPN_DEBUG("serv_client: %d serv_url: %s client: %s name: %s", serv_client, servers[i].url, hostip, hostname);

    if (strstr(servers[i].url, hostip) != NULL || strstr(servers[i].url, hostname) != NULL){
      serv_client = i;
      XPN_DEBUG("serv_client: %d serv_url: %s client: %s", serv_client, servers[serv_client].url, hostip);
    }
  }

  do
  {
    XpnReadGetBlock(fd, new_offset, serv_client, &l_offset, &l_serv);

    l_size = xpn_file_table[fd]->block_size - (new_offset%xpn_file_table[fd]->block_size);

    // If l_size > the remaining bytes to read/write, then adjust l_size
    if ((size - count) < l_size){
       l_size = size - count;
    }

    if (xpn_file_table[fd]->data_vfh->nfih[l_serv] == NULL)
    {
      res = XpnGetFh( xpn_file_table[fd]->mdata, &(xpn_file_table[fd]->data_vfh->nfih[l_serv]), &servers[l_serv], xpn_file_table[fd]->path);
      if (res<0) {
        count = -1;
        goto cleanup_xpn_sread;
      }
    }

    res = servers[l_serv].ops->nfi_read(&servers[l_serv], xpn_file_table[fd]->data_vfh->nfih[l_serv], (char *)buffer + count, l_offset+XPN_HEADER_SIZE, l_size) ;
    if (res<0) {
      count = (0 == count) ? -1 : count;
      goto cleanup_xpn_sread;
    }
    count = count + res;
    new_offset = offset + count;
  }
  while((size > (size_t) count) && (res > 0));

  if(count > 0){
    xpn_file_table[fd]->offset += count;
  }

cleanup_xpn_sread:
  res = count;
  XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset);
  return res;
}

ssize_t xpn_swrite( int fd, const void *buffer, size_t size, off_t offset )
{
  ssize_t res = -1;
  ssize_t count = 0;
  off_t new_offset, l_offset;
  size_t l_size;
  int l_serv, res_aux;
  struct nfi_server *servers = NULL;
  int n;

  XPN_DEBUG_BEGIN_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset);

  // (1) Check arguments in xpn_simple_write

  // (2) Get servers...
  servers = NULL;

  n = XpnGetServers(xpn_file_table[fd]->part->id, fd, &servers);
  if (n<=0)
  {
    count = -1;
    goto cleanup_xpn_swrite;
  }

  new_offset = offset;
  count = 0;
  do
  {
    for (int i = 0; i < xpn_file_table[fd]->part->replication_level + 1; i++)
    {
      res_aux = XpnWriteGetBlock(fd, new_offset, i, &l_offset, &l_serv);
			if (res_aux != -1){        
        l_size = xpn_file_table[fd]->block_size - (new_offset%xpn_file_table[fd]->block_size);

        // If l_size > the remaining bytes to read/write, then adjust l_size
        if ((size - count) < l_size){
        l_size = size - count;
        }

        if (xpn_file_table[fd]->data_vfh->nfih[l_serv] == NULL)
        {
          res = XpnGetFh( xpn_file_table[fd]->mdata, &(xpn_file_table[fd]->data_vfh->nfih[l_serv]), &servers[l_serv], xpn_file_table[fd]->path);
          if(res<0){
            count = -1;
            goto cleanup_xpn_swrite;
          }
        }

        res = servers[l_serv].ops->nfi_write(&servers[l_serv], xpn_file_table[fd]->data_vfh->nfih[l_serv], (char *)buffer + count, l_offset+XPN_HEADER_SIZE, l_size) ;
        XPN_DEBUG("l_serv = %d, l_offset = %lld, l_size = %lld", l_serv, (long long)l_offset, (long long)l_size);
        if (res<0) {
          count = (0 == count) ? -1 : count;
          goto cleanup_xpn_swrite;
        }
      }
    }
    count = count + res;
    new_offset = offset + count;
  }
  while((size > (size_t) count) && (res > 0));

cleanup_xpn_swrite:
  if(count > 0){
    xpn_file_table[fd]->offset += count;
  }
  res = count;
  XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset);
  return res;
}

ssize_t xpn_pread ( int fd, void *buffer, size_t size, off_t offset )
{
  ssize_t res = -1, total;
  ssize_t *res_v = NULL;
  int n, i, j, err, max;
  struct nfi_server *servers = NULL;
  struct nfi_worker_io **io = NULL;
  int *ion = NULL;
  void *new_buffer = NULL;

  XPN_DEBUG_BEGIN_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset);

  // (1) check arguments in xpn_simple_read

  n = XpnGetServers(xpn_file_table[fd]->part->id, fd, &servers);
  if (n<=0)
  {
    res = -1;
    goto cleanup_xpn_pread;
  }

  io = (struct nfi_worker_io **)malloc(sizeof(struct nfi_worker_io *)*n);
  if (io == NULL)
  {
    res = -1;
    goto cleanup_xpn_pread;
  }

  ion = (int *)malloc(sizeof(int)*n);
  if (ion == NULL)
  {
    res = -1;
    goto cleanup_xpn_pread;
  }

  res_v = (ssize_t *)malloc(sizeof(ssize_t)*n);
  if (res_v == NULL)
  {
    res = -1;
    goto cleanup_xpn_pread;
  }

  bzero(io,    n*sizeof(struct nfi_worker_io *));
  bzero(ion,   n*sizeof(int));
  bzero(res_v, n*sizeof(ssize_t));

  // compute the maximum number of operations
  max = (size/xpn_file_table[fd]->block_size) + 1;
  if (size%xpn_file_table[fd]->block_size != 0) {
      max++;
  }

  // create nfi_worker_io structs
  for (i=0; i<n; i++)
  {
    io[i] = (struct nfi_worker_io *)malloc(sizeof(struct nfi_worker_io)*max);
    if (io[i] == NULL)
    {
      res = -1;
      goto cleanup_xpn_pread;
    }

    io[i][0].offset = 0;
    io[i][0].size = 0;
  }
  
  char *hostip = ns_get_host_ip();
  char hostname[1024];
  ns_get_hostname(hostname);
  int serv_client = -1;
  for(int i=0; i<n; i++){
    XPN_DEBUG("serv_client: %d serv_url: %s client: %s name: %s", serv_client, servers[i].url, hostip, hostname);

    if (strstr(servers[i].url, hostip) != NULL || strstr(servers[i].url, hostname) != NULL){
      serv_client = i;
      XPN_DEBUG("serv_client: %d serv_url: %s client: %s", serv_client, servers[serv_client].url, hostip);
    }
  }

  // Calculate which blocks to read from each server
  new_buffer = XpnReadBlocks(fd, buffer, size, offset, serv_client, &io, &ion, n);
  if(new_buffer == NULL)
  {
    res = -1;
    goto cleanup_xpn_pread;
  }

  // operation
  for (j=0; j<n; j++)
  {
    i = XpnGetMetadataPos(xpn_file_table[fd]->mdata, j);

    if (ion[i] != 0)
    {
      res = XpnGetFh( xpn_file_table[fd]->mdata, &(xpn_file_table[fd]->data_vfh->nfih[i]), &servers[i], xpn_file_table[fd]->path);
      if (res < 0)
      {
        res = -1;
        goto cleanup_xpn_pread;
      }

      // Worker
      servers[i].wrk->thread = servers[i].xpn_thread;
      nfi_worker_do_read(servers[i].wrk, xpn_file_table[fd]->data_vfh->nfih[i], io[i], ion[i]);
    }
  }

  // results...
  err = 0;
  for (i=0; i<n; i++)
  {
     if (ion[i] != 0)
     {
        res_v[i] = nfiworker_wait(servers[i].wrk);
        if (res_v[i] < 0) {
            err = 1;
        }
     }
  }

  total = -1;
  if (!err)
  {
    total = XpnReadGetTotalBytes(res_v, n);

     if (total > 0) {
         xpn_file_table[fd]->offset += total;
     }
  }
  res = total;

  XpnReadBlocksFinish(fd, buffer, size, offset, serv_client, &io, &ion, n, new_buffer);

cleanup_xpn_pread:
  if (ion != NULL){
    for (j=0; j<n; j++){ 
      FREE_AND_NULL(io[j]);
    }
  }
  FREE_AND_NULL(io);
  FREE_AND_NULL(ion);
  FREE_AND_NULL(res_v);
  FREE_AND_NULL(new_buffer);
  XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset);
  return res;
}

ssize_t xpn_pwrite(int fd, const void *buffer, size_t size, off_t offset)
{
  ssize_t res = -1, total;
  ssize_t *res_v = NULL;
  int n, i, j, err, max;
  struct nfi_server *servers = NULL;
  struct nfi_worker_io **io = NULL;
  int *ion = NULL;
  void *new_buffer = NULL;

  XPN_DEBUG_BEGIN_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset);

  // (1) check arguments in xpn_simple_write

  n = XpnGetServers(xpn_file_table[fd]->part->id, fd, &servers);
  if (n<=0)
  {
    res = -1;
    goto cleanup_xpn_pwrite;
  }

  io = (struct nfi_worker_io **)malloc(sizeof(struct nfi_worker_io *)*n);
  if (io == NULL)
  {
    res = -1;
    goto cleanup_xpn_pwrite;
  }

  ion = (int *)malloc(sizeof(int)*n);
  if (ion == NULL)
  {
    res = -1;
    goto cleanup_xpn_pwrite;
  }

  res_v = (ssize_t *)malloc(sizeof(ssize_t)*n);
  if(res_v == NULL)
  {
    res = -1;
    goto cleanup_xpn_pwrite;
  }

  bzero(io,    n*sizeof(struct nfi_worker_io *));
  bzero(ion,   n*sizeof(int));
  bzero(res_v, n*sizeof(ssize_t));

  // calculate the maximum number of operations
  max = (size/xpn_file_table[fd]->block_size) + 1;
  if (size%xpn_file_table[fd]->block_size != 0){
      max++;
  }
  max *= xpn_file_table[fd]->part->replication_level + 1;

  // create nfi_worker_io structs
  for (i=0; i<n; i++)
  {
      io[i] = (struct nfi_worker_io *)malloc(max*sizeof(struct nfi_worker_io));
      if (NULL == io[i])
      {
        res = -1;
        goto cleanup_xpn_pwrite;
      }

      io[i][0].offset = 0;
      io[i][0].size = 0;
  }

  // Calculate which blocks to write to each server
  new_buffer = XpnWriteBlocks(fd, buffer, size, offset, &io, &ion, n);
  if (new_buffer == NULL)
  {
    res = -1;
    goto cleanup_xpn_pwrite;
  }

  for(j=0; j<n; j++)
  {
    i = XpnGetMetadataPos(xpn_file_table[fd]->mdata, j);

    if (ion[i] != 0)
    {
      res = XpnGetFh( xpn_file_table[fd]->mdata, &(xpn_file_table[fd]->data_vfh->nfih[i]), &servers[i], xpn_file_table[fd]->path);
      if (res<0)
      {
        res = -1;
        goto cleanup_xpn_pwrite;
      }

      //Worker
      servers[i].wrk->thread = servers[i].xpn_thread;
      nfi_worker_do_write(servers[i].wrk, xpn_file_table[fd]->data_vfh->nfih[i], io[i], ion[i]);
    }
  }

  // get results...
  err = 0;
  for(i=0; i<n; i++)
  {
    if(ion[i] != 0)
    {
      res_v[i] = nfiworker_wait(servers[i].wrk);
      if (res_v[i] < 0) {
        err = 1;
      }
    }
  }

  total = -1;
  if (!err)
  {
    total = XpnWriteGetTotalBytes(res_v, n, &io, ion, servers) / (xpn_file_table[fd]->part->replication_level+1);

    if (total > 0) {
        xpn_file_table[fd]->offset += total;
    }
  }
  res = total;
  
cleanup_xpn_pwrite:
  if (ion != NULL){
    for (j=0; j<n; j++){ 
      FREE_AND_NULL(io[j]);
    }
  }
  FREE_AND_NULL(io);
  FREE_AND_NULL(ion);
  FREE_AND_NULL(res_v);
  FREE_AND_NULL(new_buffer);
  XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset);
  return res;
}

off_t xpn_simple_lseek ( int fd, off_t offset, int flag )
{
  struct stat st;

  switch (flag)
  {
    case SEEK_SET:
         if (offset < 0)
         {
           errno = EINVAL;
           return (off_t)-1;
         }
         else {
           xpn_file_table[fd]->offset = offset;
         }
         break;

    case SEEK_CUR:
         if (xpn_file_table[fd]->offset+offset<0)
         {
           errno = EINVAL;
           return (off_t)-1;
         }
         else {
           xpn_file_table[fd]->offset += offset;
         }
         break;

    case SEEK_END:
         if (xpn_simple_fstat(fd, &st)<0)
         {
           errno = EBADF;
           return (off_t)-1;
         }
         if (st.st_size + offset<0)
         {
           errno = EINVAL;
           return (off_t)-1;
         }
         else {
           xpn_file_table[fd]->offset = st.st_size + offset;
         }
      break;

    default:
         errno = EINVAL;
         return (off_t)-1;
  }

  return xpn_file_table[fd]->offset;
}


   /* ................................................................... */

