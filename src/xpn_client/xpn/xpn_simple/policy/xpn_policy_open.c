
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


#include "xpn/xpn_simple/xpn_policy_open.h"





void XpnGetURLServer(struct nfi_server *serv, const char *abs_path, char *url_serv)
{
  // char dir[PATH_MAX];
  // char dir_aux[PATH_MAX];
  // int abs_path_len;
  // int put_slash;
  // int pos_abs_path;
  // int pos_dir;

  // memccpy(dir, abs_path, 0, PATH_MAX-1);
  // abs_path_len = strlen(abs_path);
  // put_slash = 1;
  // pos_abs_path = 0;
  // pos_dir = 0;
  // TODO: check if necesary for NFS multiple "///"
  // while (pos_abs_path < abs_path_len)
  // {
  //   if (abs_path[pos_abs_path] == '/')
  //   {
  //     if (put_slash)
  //     {
  //       dir_aux[pos_dir] = abs_path[pos_abs_path];
  //       pos_dir++;
  //       put_slash = 0;
  //     }
  //   } 
  //   else
  //   {
  //     dir_aux[pos_dir] = abs_path[pos_abs_path];
  //     pos_dir++;
  //     put_slash = 1;
  //   }
  //   pos_abs_path++;
  // }
  // dir_aux[pos_dir] = '\0';
  // strcpy(dir,dir_aux);
  sprintf(url_serv,"%s%s",serv->url, abs_path);
}





/**
 * TODO:
 *   ** fd < 0 => XpnGetServers_data_by_path(....) ;
 *   ** fd > 0 => XpnGetServers_data_by_fd(....) ;
 * 
 * Returns the number of the data servers, and a pointer to the array.
 *
 * @param op Flag operation.
 * @param pd Partition id.
 * @param abs_path Absolute path.
 * @param fd File descriptor.
 * @param servers[out] The data servers to be obtained, must not free.
 *
 * @return The number of data servers on success or -1 on error,
 */
int XpnGetServers(int pd, int fd, struct nfi_server **servers)
{
  // struct nfi_server **serv;
  int i, n;

  n = 1; // TODO: check default value of 1

  /* params op and abs_path are not used in this version */
  if (pd >= 0)
  {
    i = 0;
    while((i<XPN_MAX_PART) && (xpn_parttable[i].id != pd)){
      i++;
    }

    if(i == XPN_MAX_PART){
      return -1;
    }

    n = xpn_parttable[i].data_nserv;
    
    (*servers) = xpn_parttable[i].data_serv;
  }
  else if (fd >=0)
  {
    if((fd>XPN_MAX_FILE)||(xpn_file_table[fd] == NULL)){
      return -1;
    }

    n = xpn_file_table[fd]->data_vfh->n_nfih;

    (*servers) = xpn_file_table[fd]->part->data_serv;
  }
  else{
    return -1;
  }

  return n;
}


// TODO: move to metadata file
void XpnPrintMetadata(struct xpn_metadata *mdata)
{
  int i;
  printf("magic_number: %c%c%c\n", mdata->magic_number[0], mdata->magic_number[1], mdata->magic_number[2]);
  printf("version: %d\n", mdata->version);
  printf("type: %d\n", mdata->type);
  printf("block_size: %zd\n", mdata->block_size);
  printf("file_size: %zd\n", mdata->file_size);
  printf("replication_level: %d\n", mdata->replication_level);
  printf("first_node: %d\n", mdata->first_node);

  printf("data_nserv: ");
  for(i = 0; i < XPN_METADATA_MAX_RECONSTURCTIONS; i++) {
    printf("%d ", mdata->data_nserv[i]);
  }
  printf("\n");

  printf("offsets: ");
  for(i = 0; i < XPN_METADATA_MAX_RECONSTURCTIONS; i++) {
    printf("%d ", mdata->offsets[i]);
  }
  printf("\n");

  printf("distribution_policy: %d\n", mdata->distribution_policy);
}

int XpnCreateMetadata(struct xpn_metadata *mdata, int pd, const char *path)
{
  int part_id = 0,res = 0;
  XPN_DEBUG_BEGIN_CUSTOM("%s", path);

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
  mdata->magic_number[0] = XPN_MAGIC_NUMBER[0];
  mdata->magic_number[1] = XPN_MAGIC_NUMBER[1];
  mdata->magic_number[2] = XPN_MAGIC_NUMBER[2];
  mdata->data_nserv[0]   = xpn_parttable[part_id].data_nserv;
  mdata->version      = XPN_METADATA_VERSION;
  mdata->type         = 0;
  mdata->block_size   = xpn_parttable[part_id].block_size;
  mdata->replication_level   = xpn_parttable[part_id].replication_level;

  mdata->first_node = hash(path, xpn_parttable[part_id].data_nserv);
  mdata->distribution_policy = XPN_METADATA_DISTRIBUTION_ROUND_ROBIN;

  XPN_DEBUG_END_CUSTOM("%s", path);
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
  if(mdata == NULL){
    return -1;
  }

  if(pos < 0) {
    pos = (mdata->first_node)%(mdata->data_nserv[0]);
  }
  else{
    pos = (mdata->first_node+pos)%(mdata->data_nserv[0]);
  }

  return pos;
}


int XpnUpdateMetadata(struct xpn_metadata *mdata, int nserv, struct nfi_server *servers, const char *path, int replication_level, int only_file_size)
{
  int master_node, res, serv_node, err;
  char url_serv[PATH_MAX];
  XPN_DEBUG_BEGIN_CUSTOM("%s", path);

  master_node = hash(path, nserv);
  mdata->first_node = master_node;
  for (int i = 0; i < replication_level+1; i++)
  {
    serv_node = (master_node+i) % nserv;
    XpnGetURLServer(&servers[serv_node], path, url_serv);
    servers[serv_node].wrk->thread = servers[serv_node].xpn_thread;
    XPN_DEBUG("Write metadata to server: %d url: %s", serv_node, url_serv);
    nfi_worker_do_write_mdata(servers[serv_node].wrk, url_serv, mdata, only_file_size);
  }
  
  err = 0;
  for (int i = 0; i < replication_level; i++)
  {
    serv_node = (master_node+i) % nserv;
    res = nfiworker_wait(servers[serv_node].wrk);
    if(res < 0){
      err = -1;
    }
  }
  res = err;
  if (xpn_debug){ XpnPrintMetadata(mdata); }
  XPN_DEBUG_END_CUSTOM("%s", path);
  return res;
}

int XpnReadMetadata(struct xpn_metadata *mdata, int nserv, struct nfi_server *servers, const char *path, int replication_level)
{ 
  int master_node, res, i;
  char url_serv[PATH_MAX];
  XPN_DEBUG_BEGIN_CUSTOM("%s", path);

  master_node = hash(path, nserv);
  
  for (i = 0; i < replication_level; i++)
  {
    master_node = (master_node+i)%nserv;
    if (servers[master_node].error != -1){
      break;
    }
  }
  XpnGetURLServer(&servers[master_node], path, url_serv);
  servers[master_node].wrk->thread = servers[master_node].xpn_thread;
  XPN_DEBUG("Read metadata from server: %d url: %s", master_node, url_serv);
  nfi_worker_do_read_mdata(servers[master_node].wrk, url_serv, mdata);
  res = nfiworker_wait(servers[master_node].wrk);

  if (xpn_debug){ XpnPrintMetadata(mdata); }
  XPN_DEBUG_END_CUSTOM("%s", path);
  return res;
}


int XpnGetFh( struct xpn_metadata *mdata, struct nfi_fhandle **fh, struct nfi_server *servers, char *path)
{
  int res = 0;
  char url_serv[PATH_MAX];
  struct nfi_fhandle *fh_aux;

  XPN_DEBUG_BEGIN_CUSTOM("%s",path);
  if(mdata == NULL){
    return -1;
  }

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
  servers->wrk->thread = servers->xpn_thread;
  nfi_worker_do_open(servers->wrk, url_serv, O_RDWR | O_CREAT, S_IRWXU, fh_aux);
  res = nfiworker_wait(servers->wrk);

  if(res<0)
  {
    free(fh_aux);
    XPN_DEBUG_END
    return -1;
  }

  (*fh) = fh_aux;

  XPN_DEBUG_END
  return 0;
}

int XpnGetFhDir( struct xpn_metadata *mdata, struct nfi_fhandle **fh, struct nfi_server *servers, char *path)
{
  int res = 0;
  char url_serv[PATH_MAX];
  struct nfi_fhandle *fh_aux;

  XPN_DEBUG_BEGIN_CUSTOM("%s",path);
  if(mdata == NULL){
    return -1;
  }

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
  servers->wrk->thread = servers->xpn_thread;
  nfi_worker_do_opendir(servers->wrk, url_serv, fh_aux);
  res = nfiworker_wait(servers->wrk);

  if(res<0)
  {
    free(fh_aux);
    XPN_DEBUG_END
    return -1;
  }

  (*fh) = fh_aux;

  XPN_DEBUG_END
  return 0;
}


int XpnGetAtribFd ( int fd, struct stat *st )
{
  int res, i, n;
  struct nfi_server *servers;
  struct nfi_attr attr;
  struct xpn_metadata mdata;

  XPN_DEBUG_BEGIN_CUSTOM("%d", fd)

  servers = NULL;
  n = XpnGetServers(xpn_file_table[fd]->part->id, fd, &servers);
  if (n<=0)
  {
    XPN_DEBUG_END_CUSTOM("%d", fd)
    return -1;
  }  

  memset(&attr, 0, sizeof(struct nfi_attr));
  memset(st,    0, sizeof(struct stat));

  int master_node = xpn_file_table[fd]->mdata->first_node;
  for (i = 0; i < xpn_file_table[fd]->part->replication_level; i++)
  {
    master_node = (master_node+i)%n;
    if (servers[master_node].error != -1){
      break;
    }
  }

  res = XpnGetFh(xpn_file_table[fd]->mdata, &(xpn_file_table[fd]->data_vfh->nfih[master_node]), &servers[master_node], xpn_file_table[fd]->path);
  if (res<0 && xpn_file_table[fd]->part->data_serv[master_node].error != -1)
  {
    XPN_DEBUG_END_CUSTOM("%d", fd)
    return res;
  }
  nfi_worker_do_getattr(servers[master_node].wrk, xpn_file_table[fd]->data_vfh->nfih[master_node], &attr);
  res = nfiworker_wait(servers[master_node].wrk);
  if (res < 0)
  {
    XPN_DEBUG_END_CUSTOM("%d", fd)
    return res;
  }

  XpnReadMetadata(&mdata, n, servers, xpn_file_table[fd]->path, xpn_file_table[fd]->part->replication_level);
  if (XPN_CHECK_MAGIC_NUMBER(&mdata)){
    st->st_size = mdata.file_size;
  }else{
    st->st_size = attr.at_size;
  }

  // st->st_size = XpnGetRealFileSize(xpn_file_table[fd]->part, attr, n);

  st->st_dev     = attr.st_dev;       // device
  st->st_ino     = attr.st_ino;       // inode

  st->st_mode    = attr.at_mode ;     // protection

  if (0 == attr.at_type){             // It is a file
    st->st_mode = S_IFREG | st->st_mode;
  }
  if (1 == attr.at_type){             // It is a directory
    st->st_mode = S_IFDIR | st->st_mode;
  }

  st->st_nlink   = attr.at_nlink;     // number of hard links
  st->st_uid     = getuid() ;            // user ID of owner
  st->st_gid     = getgid() ;            // group ID of owner
  //st->st_blksize = xpn_file_table[pd]->block_size ;  /* blocksize for filesystem I/O // TODO
  st->st_blocks  = attr.at_blocks ;   // number of blocks allocated
  st->st_atime   = attr.at_atime ;    // time of last access
  st->st_mtime   = attr.at_mtime ;    // time of last modification
  st->st_ctime   = attr.at_ctime ;    // time of last change

  XPN_DEBUG_END_CUSTOM("%d", fd);
  return res;
}

int XpnGetAtribPath ( char * path, struct stat *st )
{
  int res, i, n, pd;
  char url_serv[PATH_MAX];
  char aux_path[PATH_MAX];
  struct nfi_server *servers;
  struct nfi_attr attr;
  struct xpn_metadata mdata;
  struct nfi_fhandle vfh_aux;

  XPN_DEBUG_BEGIN_CUSTOM("%s", path)
  strcpy(aux_path, path);

  pd = XpnGetPartition(aux_path); // returns partition id and remove partition name from path 
  if (pd < 0)
  {
    errno = ENOENT;
    XPN_DEBUG_END_ARGS1(aux_path)
    return pd;
  }

  servers = NULL;
  n = XpnGetServers(pd, -1, &servers);
  if(n<=0){
    return -1;
  }

  memset(&attr, 0, sizeof(struct nfi_attr));
  memset(&vfh_aux, 0, sizeof(struct nfi_fhandle));

  int master_node = hash(path, n);
  if (strlen(aux_path) == 0){
    aux_path[0] = '/';
    aux_path[1] = '\0';
  }

  for (i = 0; i < XpnSearchPart(pd)->replication_level; i++)
  {
    master_node = (master_node+i)%n;
    if (servers[master_node].error != -1){
      break;
    }
  }
  
  XpnGetURLServer(&servers[master_node], aux_path, url_serv);
  vfh_aux.url = url_serv;
  // Worker
  nfi_worker_do_getattr(servers[master_node].wrk, &vfh_aux, &attr);
  res = nfiworker_wait(servers[master_node].wrk);
  if (res < 0)
  {
    XPN_DEBUG_END_CUSTOM("%s", path)
    return res;
  }

  XpnReadMetadata(&mdata, n, servers, aux_path, XpnSearchPart(pd)->replication_level);
  if (XPN_CHECK_MAGIC_NUMBER(&mdata)){
    st->st_size = mdata.file_size;
  }else{
    st->st_size = attr.at_size;
  }

  // st->st_size = XpnGetRealFileSize(&(xpn_parttable[pd]), attr, n);

  st->st_dev     = attr.st_dev;       // device
  st->st_ino     = attr.st_ino;       // inode

  st->st_mode    = attr.at_mode ;     // protection

  if (0 == attr.at_type){             // It is a file
    st->st_mode = S_IFREG | st->st_mode;
  }
  if (1 == attr.at_type){             // It is a directory
    st->st_mode = S_IFDIR | st->st_mode;
  }

  st->st_nlink   = attr.at_nlink;     // number of hard links
  st->st_uid     = getuid() ;            // user ID of owner
  st->st_gid     = getgid() ;            // group ID of owner
  //st->st_blksize = xpn_file_table[pd]->block_size ;  /* blocksize for filesystem I/O // TODO
  st->st_blocks  = attr.at_blocks ;   // number of blocks allocated
  st->st_atime   = attr.at_atime ;    // time of last access
  st->st_mtime   = attr.at_mtime ;    // time of last modification
  st->st_ctime   = attr.at_ctime ;    // time of last change

  XPN_DEBUG_END_CUSTOM("%s", path)

  return res;
}

/*
   int XpnPutAtrib(struct xpn_fh *fh, struct stat *st)
   {
  return 0;
   }
*/

