
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

      #include "xpn/xpn_simple/xpn_rw.h"


   /* ... Global vars. / Variables globales ............................. */

   // extern pthread_mutex_t global_mt;
      extern void XpnShowFileTable();


   /* ... Functions / Funciones ......................................... */

ssize_t xpn_simple_read ( int fd, void *buffer, size_t size )
{
  ssize_t res = -1;

  XPN_DEBUG_BEGIN_CUSTOM("%d, %zu", fd, size)

  if ((unsigned long)(size) > (unsigned long)(xpn_file_table[fd]->block_size))
  {
    res = xpn_pread(fd, buffer, size, xpn_file_table[fd]->offset);
  }
  else {
    res = xpn_sread(fd, buffer, size, xpn_file_table[fd]->offset);
  }

  XPN_DEBUG_END_CUSTOM("%d, %zu", fd, size)

  return res;
}

ssize_t xpn_simple_write ( int fd, const void *buffer, size_t size )
{
  ssize_t res = -1;

  XPN_DEBUG_BEGIN_CUSTOM("%d, %zu", fd, size)

  if ((unsigned long)(size) >= (unsigned long)(xpn_file_table[fd]->block_size))
  {
    res = xpn_pwrite(fd, buffer, size, xpn_file_table[fd]->offset);
  }
  else {
    res = xpn_swrite(fd, buffer, size, xpn_file_table[fd]->offset);
  }

  XPN_DEBUG_END_CUSTOM("%d, %zu", fd, size)

  return res;
}

ssize_t xpn_sread ( int fd, const void *buffer, size_t size, off_t offset )
{
  ssize_t res = -1;
  ssize_t count;
  off_t new_offset, l_offset;
  size_t l_size;
  int l_serv;
  struct nfi_server **servers;
  int n;

  res   = -1 ;
  count = 0 ;
  XPN_DEBUG_BEGIN_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)

  // (1) Check arguments...
  if ( (fd<0) || (fd>XPN_MAX_FILE) || (NULL == xpn_file_table[fd]) )
  {
     XpnShowFileTable();
     errno = EBADF;
     XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
     return -1;
  }

  if (buffer == NULL)
  {
     errno = EFAULT;
     XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
     return -1;
  }

  if (size == 0) {
     XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
     return 0;
  }

  if (xpn_file_table[fd]->mode == O_WRONLY) {
     XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
     return -1;
  }

  if (xpn_file_table[fd]->type == XPN_DIR) {
     XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
     return -1;
  }

  // (2) Get servers...
  servers = NULL;

  /* params for XpnGetServers:
   * flag operation, partition id, absolute path, file descriptor, pointer to servers */
  n = XpnGetServers(op_xpn_read, xpn_file_table[fd]->part->id, NULL, fd, &servers, XPN_DATA_SERVER);
  if (n <= 0)
  {
     if (servers != NULL) { free(servers); servers=NULL; }
     return -1;
  }

  new_offset = offset;
  count = 0;

  do
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
       if (res<0) {
           return -1;
       }
    }

    res = servers[l_serv]->ops->nfi_read(servers[l_serv], xpn_file_table[fd]->data_vfh->nfih[l_serv], (char *)buffer + count, l_offset+XPN_HEADER_SIZE, l_size) ;
    if (res<0) {
       return (0 == count) ? -1 : count ;
    }
    count = count + res;
    new_offset = offset + count;
  }
  while((size > (size_t) count) && (res > 0));

  if(count > 0){
    xpn_file_table[fd]->offset += count;
  }

  if (servers != NULL) { free(servers); servers=NULL; }

  return count;
}

ssize_t xpn_swrite ( int fd, const void *buffer, size_t size, off_t offset )
{
  ssize_t res = -1;
  ssize_t count = 0;
  off_t new_offset, l_offset;
  size_t l_size;
  int l_serv;
  struct nfi_server **servers;
  int n;

  XPN_DEBUG_BEGIN_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)

  // (1) Check arguments...
  if ( (fd<0) || (fd>XPN_MAX_FILE) )
  {
     XpnShowFileTable();
     errno = EBADF;
     XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)

     return -1;
  }

  if ( (xpn_file_table[fd] == NULL) || (buffer == NULL) )
  {
     errno = EFAULT;
     XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
     return -1;
  }

  if (size == 0)
  {
     XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
     return 0;
  }

  if (xpn_file_table[fd]->mode == O_RDONLY)
  {
     XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
     return -1;
  }

  if (xpn_file_table[fd]->type == XPN_DIR)
  {
     XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
     return -1;
  }

  // (2) Get servers...
  servers = NULL;

  /* params for XpnGetServers:
   * flag operation, partition id, absolute path, file descriptor, pointer to servers */
  n = XpnGetServers(op_xpn_write, xpn_file_table[fd]->part->id, NULL, fd, &servers, XPN_DATA_SERVER);
  if (n<=0)
  {
    if (servers != NULL) { free(servers); servers=NULL; }
    return -1;
  }

  new_offset = offset;
  count = 0;

  do
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
      if (res<0) {
         return -1;
      }
    }

    res = servers[l_serv]->ops->nfi_write(servers[l_serv], xpn_file_table[fd]->data_vfh->nfih[l_serv], (char *)buffer + count, l_offset+XPN_HEADER_SIZE, l_size) ;
    if (res<0) {
      return (0 == count) ? -1 : count ;
    }

    count = count + res;
    new_offset = offset + count;
  }
  while((size > (size_t) count) && (res > 0));

  if (servers != NULL) { free(servers); servers=NULL; }

  if(count > 0){
    xpn_file_table[fd]->offset += count;
  }

  return count;
}


int xpn_paux_free ( int n, struct nfi_server ***servers, struct nfi_worker_io ***io, int **ion, ssize_t **res_v )
{
      int j ;

      // free *servers
      if ( (*servers) != NULL) { 
         free( (*servers) ); 
	       (*servers) = NULL;
      }

      // free *io
      if ( (*ion) != NULL)
      {
           for (j=0; j<n; j++)
           {
	             if ( (*io)[j] != NULL) {
                    free((*io)[j]);
                    (*io)[j] = NULL ;
	             }
           }

           free((*io));
           (*io) = NULL;
      }

      // free *ion
      if ( (*ion) != NULL) { 
           free( (*ion) ); 
	         (*ion) = NULL;
      }

      // free *res_v
      if ( (*res_v) != NULL) { 
           free( (*res_v) ); 
	         (*res_v) = NULL;
      }

      return  1;
}

ssize_t xpn_pread ( int fd, void *buffer, size_t size, off_t offset )
{
  ssize_t res = -1;
  ssize_t *res_v, total;
  int n, i, j, err;
  struct nfi_server **servers;
  struct nfi_worker_io **io;
  int *ion, max;
  void *new_buffer;

  XPN_DEBUG_BEGIN_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)

  // (1) check arguments
  if ( (fd<0) || (fd>XPN_MAX_FILE) || (NULL == xpn_file_table[fd]) )
  {
     XpnShowFileTable();
     errno = EBADF;
     res = -1;
     XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
     return res;
  }

  if (buffer == NULL)
  {
     errno = EFAULT;
     res = -1;
     XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
     return res;
  }

  if (size == 0)
  {
     res = 0;
     XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
     return res;
  }

  if (xpn_file_table[fd]->mode == O_WRONLY)
  {
     res = -1;
     XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
     return res;
  }

  if (xpn_file_table[fd]->type == XPN_DIR)
  {
     res = -1;
     XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
     return res;
  }

  // (2) memory allocation for internal information
  servers = NULL;
  io      = NULL;
  ion     = NULL;
  res_v   = NULL;

  /* params of XpnGetServers:
   * flag operation, partition id, absolute path, file descriptor, pointer to servers */
  n = XpnGetServers(op_xpn_read, xpn_file_table[fd]->part->id, NULL, fd, &servers, XPN_DATA_SERVER);
  if (n<=0)
  {
     if (servers != NULL) { free(servers); servers=NULL; }

     res = -1;
     XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
     return res;
  }

  io = (struct nfi_worker_io **)malloc(sizeof(struct nfi_worker_io *)*n);
  if (io == NULL)
  {
     if (servers != NULL) { free(servers); servers=NULL; }

     res = -1;
     XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
     return res;
  }

  ion = (int *)malloc(sizeof(int)*n);
  if (ion == NULL)
  {
     if (servers != NULL) { free(servers); servers=NULL; }
     if (io      != NULL) { free(io);      io=NULL; }

     res = -1;
     XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
     return res;
  }

  res_v = (ssize_t *)malloc(sizeof(ssize_t)*n);
  if (res_v == NULL)
  {
     if (servers != NULL) { free(servers); servers=NULL; }
     if (io      != NULL) { free(io);      io=NULL; }
     if (ion     != NULL) { free(ion);     ion=NULL; }

     return -1;
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
      xpn_paux_free(n, &servers, &io, &ion, &res_v) ;

      res = -1;
      XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
      return res;
    }

    io[i][0].offset = 0;
    io[i][0].size = 0;
  }

  // Calculate which blocks to read from each server
  new_buffer = XpnReadBlocks(fd, buffer, size, offset, &io, &ion, n);
  if (new_buffer == NULL)
  {
      xpn_paux_free(n, &servers, &io, &ion, &res_v) ;
      return -1;
  }

  // TODO: Expand is not reentrant. Using only one 'wrk' (struct nfi_worker) inside servers[i] (struct nfi_server) does not allow two nfi operations at the same time
  // pthread_mutex_lock(&(global_mt)); // It is better to do this in clients when necessary

  // operation
  for (j=0; j<n; j++)
  {
    i = XpnGetMetadataPos(xpn_file_table[fd]->mdata, j);

    if (ion[i] != 0)
    {
      res = XpnGetFh( xpn_file_table[fd]->mdata, &(xpn_file_table[fd]->data_vfh->nfih[i]), servers[i], xpn_file_table[fd]->path);
      if (res < 0)
      {
         xpn_paux_free(n, &servers, &io, &ion, &res_v) ;
	       if (new_buffer != NULL) { free(new_buffer); new_buffer = NULL; }

         res = -1;
         XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
         return res;
      }

      // Worker
      servers[i]->wrk->thread = servers[i]->xpn_thread;
      nfi_worker_do_read(servers[i]->wrk, xpn_file_table[fd]->data_vfh->nfih[i], io[i], ion[i]);
    }
  }

  // results...
  err = 0;
  for (i=0; i<n; i++)
  {
     if (ion[i] != 0)
     {
        res_v[i] = nfiworker_wait(servers[i]->wrk);
        if (res_v[i] < 0) {
            err = 1;
        }
     }
  }

  //pthread_mutex_unlock(&(global_mt));

  total = -1;
  if (!err)
  {
     total = XpnReadGetTotalBytes(fd, res_v, n);

     if (total > 0) {
         xpn_file_table[fd]->offset += total;
     }
  }
  res = total;

  XpnReadBlocksFinish(fd, buffer, size, offset, &io, &ion, n, new_buffer);
  xpn_paux_free(n, &servers, &io, &ion, &res_v) ;
  // if (new_buffer != NULL) { free(new_buffer); new_buffer = NULL; } <- XpnReadBlocksFinish(...)

  XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)

  return res;
}

ssize_t xpn_pwrite ( int fd, const void *buffer, size_t size, off_t offset )
{
  ssize_t res = -1;
  ssize_t *res_v, total;
  int n, i, j, err;
  struct nfi_server **servers;
  struct nfi_worker_io **io;
  int *ion, max;
  void *new_buffer;

  XPN_DEBUG_BEGIN_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)

  // (1) check arguments
  if ( (fd<0) || (fd>XPN_MAX_FILE) )
  {
     XpnShowFileTable();
     errno = EBADF;
     XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)

     return -1;
  }

  if ( (xpn_file_table[fd] == NULL) || (buffer == NULL) )
  {
     errno = EFAULT;
     XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
     return -1;
  }

  if (size == 0)
  {
     XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
     return 0;
  }

  if (xpn_file_table[fd]->mode == O_RDONLY)
  {
     XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
     return -1;
  }

  if (xpn_file_table[fd]->type == XPN_DIR)
  {
     XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
     return -1;
  }

  // (2) memory allocation for internal information
  servers = NULL;
  io      = NULL;
  ion     = NULL;
  res_v   = NULL;

  /* params of XpnGetServers:
   * flag operation, partition id, absolute path, file descriptor, pointer to servers */
  n = XpnGetServers(op_xpn_write, xpn_file_table[fd]->part->id, NULL, fd, &servers, XPN_DATA_SERVER);
  if (n<=0)
  {
     if (servers != NULL) { free(servers); servers=NULL; }

     res = -1;
     XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
     return res;
  }

  io = (struct nfi_worker_io **)malloc(sizeof(struct nfi_worker_io *)*n);
  if (io == NULL)
  {
     if (servers != NULL) { free(servers); servers=NULL; }

     return -1 ;
  }

  ion = (int *)malloc(sizeof(int)*n);
  if (ion == NULL)
  {
     if (servers != NULL) { free(servers); servers=NULL; }
     if (io      != NULL) { free(io);      io=NULL; }

     return -1 ;
  }

  res_v = (ssize_t *)malloc(sizeof(ssize_t)*n);
  if(res_v == NULL)
  {
     if (servers != NULL) { free(servers); servers=NULL; }
     if (io      != NULL) { free(io);      io=NULL; }
     if (ion     != NULL) { free(ion);     ion=NULL; }

     return -1;
  }

  bzero(io,    n*sizeof(struct nfi_worker_io *));
  bzero(ion,   n*sizeof(int));
  bzero(res_v, n*sizeof(ssize_t));

  // calculate the maximum number of operations
  max = (size/xpn_file_table[fd]->block_size) + 1;
  if (size%xpn_file_table[fd]->block_size != 0){
      max++;
  }

  // create nfi_worker_io structs
  for (i=0; i<n; i++)
  {
      io[i] = (struct nfi_worker_io *)malloc(max*sizeof(struct nfi_worker_io));
      if (NULL == io[i])
      {
         xpn_paux_free(n, &servers, &io, &ion, &res_v) ;
         return -1;
      }

      io[i][0].offset = 0;
      io[i][0].size = 0;
  }

  // Calculate which blocks to write to each server
  new_buffer = XpnWriteBlocks(fd, buffer, size, offset, &io, &ion, n);
  if (new_buffer == NULL)
  {
      xpn_paux_free(n, &servers, &io, &ion, &res_v) ;
      return -1;
  }

  // TODO: Expand is not reentrant. Using only one 'wrk' (struct nfi_worker) inside servers[i] (struct nfi_server) does not allow two nfi operations at the same time
  //pthread_mutex_lock(&(global_mt)); // It is better to do this in clients when necessary

  for(j=0; j<n; j++)
  {
    i = XpnGetMetadataPos(xpn_file_table[fd]->mdata, j);

    if (ion[i] != 0)
    {
      res = XpnGetFh( xpn_file_table[fd]->mdata, &(xpn_file_table[fd]->data_vfh->nfih[i]), servers[i], xpn_file_table[fd]->path);
      if (res<0)
      {
         xpn_paux_free(n, &servers, &io, &ion, &res_v) ;
         if (new_buffer != NULL) { free(new_buffer); new_buffer = NULL; }
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

  // get results...
  err = 0;
  for(i=0; i<n; i++)
  {
    if(ion[i] != 0)
    {
      res_v[i] = nfiworker_wait(servers[i]->wrk);
      if (res_v[i] < 0) {
        err = 1;
      }
    }
  }

  // pthread_mutex_unlock(&(global_mt));

  total = -1;
  if (!err)
  {
    total = XpnWriteGetTotalBytes(fd, res_v, n);

    if (total > 0) {
        xpn_file_table[fd]->offset += total;
    }
  }
  res = total;

  XpnWriteBlocksFinish(fd, buffer, size, offset, &io, &ion, n, new_buffer);
  xpn_paux_free(n, &servers, &io, &ion, &res_v) ;
  // if (new_buffer != NULL) { free(new_buffer); new_buffer = NULL; } <- XpnReadBlocksFinish(...)

  XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
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

