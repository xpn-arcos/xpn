
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


#include "xpn/xpn_simple/xpn_policy_open.h"



ssize_t XpnGetSizeThreads(struct xpn_partition *p)
{
  return p->size_threads;
}


void XpnGetURLServer(struct nfi_server *serv, char *abs_path, char *url_serv)
{
  char dir[PATH_MAX];
  char dir_aux[PATH_MAX];
  int abs_path_len;
  int put_slash;
  int pos_abs_path;
  int pos_dir;

  memccpy(dir, abs_path, 0, PATH_MAX-1);
  abs_path_len = strlen(abs_path);
  put_slash = 1;
  pos_abs_path = 0;
  pos_dir = 0;

  while (pos_abs_path < abs_path_len)
  {
    if (abs_path[pos_abs_path] == '/')
    {
      if (put_slash)
      {
        dir_aux[pos_dir] = abs_path[pos_abs_path];
        pos_dir++;
        put_slash = 0;
      }
    } 
    else
    {
      dir_aux[pos_dir] = abs_path[pos_abs_path];
      pos_dir++;
      put_slash = 1;
    }
    pos_abs_path++;
  }
  dir_aux[pos_dir] = '\0';
  strcpy(dir,dir_aux);
  sprintf(url_serv,"%s%s",serv->url, dir);
}





/**
 * TODO:
 *   ** int op -> not used (!)
 *   ** XpnGetServers(.... int type==XPN_META_SERVER) ->  XpnGetServers_mdata(....) ;
 *   ** fd < 0 => XpnGetServers_data_by_path(....) ;
 *   ** fd > 0 => XpnGetServers_data_by_fd(....) ;
 * 
 * Returns the data or metadata servers.
 *
 * @param op Flag operation.
 * @param pd Partition id.
 * @param abs_path Absolute path.
 * @param fd File descriptor.
 * @param servers[out] The data or metadata servers to be obtained.
 * @param type Metadata or data server.
 *
 * @return The number of data or metadata servers on success or -1 on error,
 */
int XpnGetServers(int op, int pd, __attribute__((__unused__)) char *abs_path, int fd, struct nfi_server ***servers, int type)
{
  struct nfi_server **serv;
  int i, j, n;

  n = 1; // TODO: check default value of 1

  /* params op and abs_path are not used in this version */
  switch(op)
  {
    default:
      if (pd >= 0)
      {
        i = 0;
        while((i<XPN_MAX_PART) && (xpn_parttable[i].id != pd)){
          i++;
        }

        if(i == XPN_MAX_PART){
          return -1;
        }

        switch(type)
        {
          case  XPN_DATA_SERVER:
            n = xpn_parttable[i].data_nserv;
            break;
          case  XPN_META_SERVER:
            n = xpn_parttable[i].meta_nserv;
            break;
        }

        serv = (struct nfi_server **)malloc(sizeof(struct nfi_server *) * n);
        if(serv == NULL){
          /* xpn_err()*/
          return -1;
        }

        for(j=0;j<n;j++)
        {
          switch(type)
          {
            case  XPN_DATA_SERVER:
              serv[j] = &(xpn_parttable[i].data_serv[j]);
              break;
            case  XPN_META_SERVER:
              serv[j] = &(xpn_parttable[i].meta_serv[j]);
              break;
          }
        }
        (*servers) = serv;
      }
      else if (fd >=0)
      {
        if((fd>XPN_MAX_FILE)||(xpn_file_table[fd] == NULL)){
          return -1;
        }

        switch(type)
        {
          case  XPN_DATA_SERVER:
            n = xpn_file_table[fd]->data_vfh->n_nfih;
            break;
          case  XPN_META_SERVER:
            n = xpn_file_table[fd]->meta_vfh->n_nfih;
            break;
        }

        serv = (struct nfi_server **)malloc(sizeof(struct nfi_server*) * n);
        if(serv == NULL){
          /* xpn_err()*/
          return -1;
        }

        for(j=0;j<n;j++)
        {
          switch(type)
          {
            case  XPN_DATA_SERVER:
              serv[j] = &(xpn_file_table[fd]->part->data_serv[j]);
              break;
            case  XPN_META_SERVER:
              serv[j] = &(xpn_file_table[fd]->part->meta_serv[j]);
              break;
          }
        }

        (*servers) = serv;
      }
      else{
        return -1;
      }
      break;
  }

  return n;
}


// TODO: move to metadata file


int XpnCreateMetadata(struct xpn_metadata *mdata, int pd, char *path)
{
  int part_id = 0;

  if(mdata == NULL){
    return -1;
  }

  //TODO pd == xpn_parttable[i].id
  while((part_id<XPN_MAX_PART) && (xpn_parttable[part_id].id != pd)){
    part_id++;
  }

  if (part_id == XPN_MAX_PART){
    return -1;
  }

  /* initial values */
  bzero(mdata, sizeof(struct xpn_metadata));
  mdata->meta_nserv   = xpn_parttable[part_id].meta_nserv;
  mdata->data_nserv   = xpn_parttable[part_id].data_nserv;
  mdata->id           = 0;
  mdata->version      = 1;
  mdata->type         = 0;
  mdata->block_size   = xpn_parttable[part_id].block_size;
  mdata->type_policy  = xpn_parttable[part_id].type;

  if (mdata->type_policy < 0) {
      return -1;
  }

  switch (mdata->type_policy)
  {
    default:
      mdata->policy = (struct policy *)malloc(sizeof(struct policy));
      if (NULL == mdata->policy) {
        return -1;
      }

      ((struct policy *)mdata->policy)->first_node = hash(path, xpn_parttable[part_id].data_nserv);


      mdata->policy_size = sizeof(struct policy);
      break;
  }
  return 0;
}

/*
 * TODO: XpnGetMetadataPos -> xpn_mdata_associated_server
 *   (in) Logical server    0      1       3      4
 *
 *                                 ^
 *                                 |
 *   (in)                       *master*
 *                                 |
 *                                 v
 *
 *   (out) Real Servers     3      0       1      2
 * 
 */
int XpnGetMetadataPos(struct xpn_metadata *mdata, int pos)
{
  struct policy *p;

  if(mdata == NULL){
    return -1;
  }

  if (mdata->type_policy < 0) {
    return -1;
  }

  switch(mdata->type_policy)
  {
    default:
      p = (struct policy *) mdata->policy;
      if (p == NULL){
        return pos;
      }
      if(pos < 0) {
        pos = (p->first_node)%(mdata->data_nserv);
      }
      else{
        pos = (p->first_node+pos)%(mdata->data_nserv);
      }
      break;
  }

  return pos;
}


//TODO: we think that this function is used to write metadata into the metadata header (todo: really write into file)
int XpnUpdateMetadata( __attribute__((__unused__)) struct xpn_metadata *mdata,
                       __attribute__((__unused__)) int nserv,
                       __attribute__((__unused__)) struct nfi_server **servers,
                       __attribute__((__unused__)) struct xpn_fh *fh,
                       __attribute__((__unused__)) char *path)
{
  // TODO
  return 0;
}



//TODO: we think that this function is used to read metadata from the metadata header (todo: really read header)
int XpnReadMetadata ( struct xpn_metadata *mdata, __attribute__((__unused__)) int nserv, struct nfi_server **servers, struct xpn_fh *fh, char *path, int pd )
{
  int res, n, i;

  XPN_DEBUG_BEGIN

  if(mdata == NULL){
    return -1;
  }

  if (mdata->type_policy < 0) {
    return -1;
  }

  switch(mdata->type_policy)
  {
    default:
      n = hash(path, nserv);

      res = XpnGetFh(mdata, &(fh->nfih[n]), servers[n], path);
      if(res < 0)
      {
        XPN_DEBUG_END
        return -1;
      }

      XpnCreateMetadata(mdata, pd, path);

      if(fh->nfih[n]->type == NFIDIR)
      {
        i = 0;
        while((i<XPN_MAX_PART) && (xpn_parttable[i].id != pd)){
          i++;
        }

        if(i == XPN_MAX_PART)
        {
          XPN_DEBUG_END
          return -1;
        }

        mdata->type = XPN_DIR;
        XPN_DEBUG_END
        return XPN_DIR;
      }
       
    break;
    
  }

  XPN_DEBUG_END
  return XPN_FILE;
}


int XpnGetFh( struct xpn_metadata *mdata, struct nfi_fhandle **fh, struct nfi_server *servers, char *path)
{
  int res;
  char url_serv[PATH_MAX];
  struct nfi_fhandle *fh_aux;

  XPN_DEBUG_BEGIN

  if(mdata == NULL){
    return -1;
  }

  if (mdata->type_policy < 0)
  {
    XPN_DEBUG_END
    return -1;
  }

  switch(mdata->type_policy)
  {
    default:
      if((*fh) != NULL)
      {
        XPN_DEBUG_END
        return 0;
      }

      fh_aux = (struct nfi_fhandle *) malloc(sizeof(struct nfi_fhandle));
      if(fh_aux == NULL)
      {
        XPN_DEBUG_END
        return -1;
      }

      memset(fh_aux, 0, sizeof(struct nfi_fhandle));

      XpnGetURLServer(servers, path, url_serv);

      // Default Value (if file, else directory)
      res = servers->ops->nfi_open(servers, url_serv, fh_aux);
      if (res<0) {
          res = servers->ops->nfi_opendir(servers, url_serv, fh_aux); // FIXME: When do we do nfi_closedir()?
      }

      if(res<0)
      {
        free(fh_aux);
        XPN_DEBUG_END
        return -1;
      }

      (*fh) = fh_aux;
      break;
  }

  XPN_DEBUG_END
  return 0;
}


int XpnGetAtribFd ( int fd, struct stat *st )
{
  int ret, res, i, n, err;
  struct nfi_server **servers;
  struct nfi_attr *attr;

  XPN_DEBUG_BEGIN_CUSTOM("%d", fd)

  servers = NULL;
  n = XpnGetServers(op_xpn_getattr, xpn_file_table[fd]->part->id, NULL, fd, &servers, XPN_DATA_SERVER);
  if (n<=0)
  {
    XPN_DEBUG_END_CUSTOM("%d", fd)
    return -1;
  }

  attr = (struct nfi_attr *) malloc(n * sizeof(struct nfi_attr));
  if (NULL == attr) {
    free(servers);
    perror("malloc: ");
    return -1;
  }

  memset(attr, 0, n* sizeof(struct nfi_attr));
  memset(st,    0, sizeof(struct stat));

  for(i=0;i<n;i++)
  {
    res = XpnGetFh(xpn_file_table[fd]->mdata, &(xpn_file_table[fd]->data_vfh->nfih[i]), servers[i], xpn_file_table[fd]->path);
    if (res<0)
    {
      XPN_DEBUG_END_CUSTOM("%d", fd)
      return res;
    }

    // Worker
    nfi_worker_do_getattr(servers[i]->wrk, xpn_file_table[fd]->data_vfh->nfih[i], &(attr[i]));
  }

  // Wait
  err = 0;
  for(i=0;i<n;i++)
  {
    ret = nfiworker_wait(servers[i]->wrk);

    // Control error
    if((ret<0)&&(!err))
    {
      // erase the file create before de server number i
      err = 1;
    }
  }

  // Error checking
  if(err)
  {
    xpn_err(XPNERR_REMOVE);
    free(servers);
    return -1;
  }

  st->st_size = 0;
  for(i=0;i<n;i++)
  {
    if (attr[i].at_size > 0){
      st->st_size += (attr[i].at_size - XPN_HEADER_SIZE); // total size, in bytes
    }

    if (1 == attr[i].at_type)  // It is a directory
    {
      break;
    }
  }

  int master_node = ((struct policy *)xpn_file_table[fd]->mdata->policy)->first_node;
  //int master_node = ((struct policy *)mdata->policy)->first_node;

  st->st_dev     = attr[master_node].st_dev;       // device
  st->st_ino     = attr[master_node].st_ino;       // inode

  st->st_mode    = attr[master_node].at_mode ;     // protection

  if (0 == attr[master_node].at_type){             // It is a file
    st->st_mode = S_IFREG | st->st_mode;
  }
  if (1 == attr[master_node].at_type){             // It is a directory
    st->st_mode = S_IFDIR | st->st_mode;
  }

  st->st_nlink   = attr[master_node].at_nlink;     // number of hard links
  st->st_uid     = getuid() ;            // user ID of owner
  st->st_gid     = getgid() ;            // group ID of owner
  //st->st_blksize = xpn_file_table[pd]->block_size ;  /* blocksize for filesystem I/O // TODO
  st->st_blocks  = attr[master_node].at_blocks ;   // number of blocks allocated
  st->st_atime   = attr[master_node].at_atime ;    // time of last access
  st->st_mtime   = attr[master_node].at_mtime ;    // time of last modification
  st->st_ctime   = attr[master_node].at_ctime ;    // time of last change

  free(servers);
  free(attr);

  return ret;
}

int XpnGetAtribPath ( char * path, struct stat *st )
{
  int ret, res, err, i, n, pd;
  char url_serv[PATH_MAX];
  char aux_path[PATH_MAX];
  struct nfi_server **servers;
  struct nfi_attr *attr;
  struct xpn_fh *vfh_aux;

  //XPN_DEBUG_BEGIN_CUSTOM("%s", path)

  strcpy(aux_path, path);

  pd = XpnGetPartition(aux_path); // returns partition id and remove partition name from path 
  if (pd < 0)
  {
    xpn_err(XPNERR_PART_NOEXIST);
    XPN_DEBUG_END_ARGS1(aux_path)
    return pd;
  }

  /* params:
   * flag operation , partition id,absolute path, file descript., pointer to server*/
  servers = NULL;
  n = XpnGetServers(op_xpn_getattr, pd, aux_path, -1, &servers, XPN_DATA_SERVER);
  if(n<=0){
    /*free(servers);*/
    return -1;
  }

  attr = (struct nfi_attr *) malloc(n * sizeof(struct nfi_attr));
  if (NULL == attr) {
    free(servers);
    perror("malloc: ");
    return -1;
  }
  
  memset(attr, 0, n* sizeof(struct nfi_attr));

  vfh_aux = (struct xpn_fh *)malloc(sizeof(struct xpn_fh));
  /* construccion del vfh */
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
    free(vfh_aux);
    xpn_err(XPNERR_NOMEMORY);
    free(servers);
    return -1;
  }

  for(i=0;i<n;i++)
  {
    vfh_aux->nfih[i] = NULL;

    XpnGetURLServer(servers[i], aux_path, url_serv);

    vfh_aux->nfih[i] = (struct nfi_fhandle*)malloc(sizeof(struct nfi_fhandle));
    memset(vfh_aux->nfih[i], 0, sizeof(struct nfi_fhandle));
    if(vfh_aux->nfih[i] == NULL)
    {
      free(servers);
      return -1;
    }

    vfh_aux->nfih[i]->url = url_serv;

    // Worker
    nfi_worker_do_getattr(servers[i]->wrk, vfh_aux->nfih[i], &(attr[i]));
  }

  // Wait
  err = 0;
  for(i=0;i<n;i++)
  {
    ret = nfiworker_wait(servers[i]->wrk);

    // Control error
    if((ret<0)&&(!err))
    {
      // erase the file create before de server number i
      err = 1;
    }
  }

  // Error checking
  if(err)
  {
    xpn_err(XPNERR_REMOVE);
    free(servers);
    return -1;
  }

  st->st_size = 0;
  for(i=0;i<n;i++)
  {
    if (attr[i].at_size > 0){
      st->st_size += (attr[i].at_size - XPN_HEADER_SIZE); // total size, in bytes
    }

    if (1 == attr[i].at_type)  // It is a directory
    {
      break;
    }
  }

  int master_node = hash(path, n);

  st->st_dev     = attr[master_node].st_dev;       // device
  st->st_ino     = attr[master_node].st_ino;       // inode

  st->st_mode    = attr[master_node].at_mode ;     // protection

  if (0 == attr[master_node].at_type){             // It is a file
    st->st_mode = S_IFREG | st->st_mode;
  }
  if (1 == attr[master_node].at_type){             // It is a directory
    st->st_mode = S_IFDIR | st->st_mode;
  }

  st->st_nlink   = attr[master_node].at_nlink;     // number of hard links
  st->st_uid     = getuid() ;            // user ID of owner
  st->st_gid     = getgid() ;            // group ID of owner
  //st->st_blksize = xpn_file_table[pd]->block_size ;  /* blocksize for filesystem I/O // TODO
  st->st_blocks  = attr[master_node].at_blocks ;   // number of blocks allocated
  st->st_atime   = attr[master_node].at_atime ;    // time of last access
  st->st_mtime   = attr[master_node].at_mtime ;    // time of last modification
  st->st_ctime   = attr[master_node].at_ctime ;    // time of last change

  free(servers);
  free(attr);

  return ret;
}

/*
   int XpnPutAtrib(struct xpn_fh *fh, struct stat *st)
   {
  return 0;
   }
*/

