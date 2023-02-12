
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


#include "xpn/xpn_base/xpn_base_dir.h"


int xpn_base_mkdir(const char *path, mode_t perm)
{
  char abs_path[PATH_MAX], url_serv[PATH_MAX];
  struct nfi_server **servers;
  struct xpn_metadata *mdata_aux;
  struct nfi_attr attr;
  int ret, err, i, j, n, pd;
  struct xpn_fh *vfh_aux;

  if (path == NULL) {
    xpn_err(XPNERR_PARAM);
    return -1;
  }

  ret = XpnGetAbsolutePath(path, abs_path);
  if (ret<0) {
    xpn_err(XPNERR_PATH_NOEXIST);
    return -1;
  }

  pd = XpnGetPartition(abs_path);
  if (pd<0) {
    xpn_err(XPNERR_PART_NOEXIST);
    return -1;
  }

  /*
   * XpnGetServers: flag operation, partition id., absolute path, file descriptor, pointer to server
   */
  servers = NULL;
  n = XpnGetServers(op_xpn_mkdir, pd, abs_path, -1, &servers, XPN_DATA_SERVER);
  if (n<=0) {
    return -1;
  }

  bzero(&attr, sizeof(struct nfi_attr));
  attr.at_mode = S_IFDIR  | perm;

  /* construccion del vfh */
  vfh_aux = (struct xpn_fh *)malloc(sizeof(struct xpn_fh));
  if (vfh_aux == NULL)
  {
    xpn_err(XPNERR_NOMEMORY);
    FREE_AND_NULL(servers);
    return -1;
  }

  vfh_aux->n_nfih = n;
  vfh_aux->nfih = (struct nfi_fhandle **)malloc(sizeof(struct nfi_fhandle*) * n);
  if (vfh_aux->nfih == NULL)
  {
    FREE_AND_NULL(servers);
    FREE_AND_NULL(vfh_aux);
    xpn_err(XPNERR_NOMEMORY);
    return -1;
  }

  for(i=0;i<n;i++)
  {
    vfh_aux->nfih[i] = NULL;

    XpnGetURLServer(servers[i], abs_path, url_serv);

    vfh_aux->nfih[i] = (struct nfi_fhandle*)malloc(sizeof(struct nfi_fhandle));
    memset(vfh_aux->nfih[i], 0, sizeof(struct nfi_fhandle));
    if (vfh_aux->nfih[i] == NULL) {
      FREE_AND_NULL(servers);
      return -1;
    }

    // Worker
    nfi_worker_do_mkdir(servers[i]->wrk, url_serv, &attr, vfh_aux->nfih[i]);
  }

  // Wait
  err = 0;
  for (i=0; i<n; i++)
  {
    ret = nfiworker_wait(servers[i]->wrk);

    // error control
    if ((ret<0) && (!err)) {
        err = 1;
    }
  }

  // Error checking
  if (err)
  {
    FREE_AND_NULL(servers);
    xpn_err(XPNERR_REMOVE);
    return -1;
  }

  mdata_aux = (struct xpn_metadata *)malloc(sizeof(struct xpn_metadata));
  if (mdata_aux == NULL)
  {
    free(servers);

    /* tambien los punteros indirectos */
    for (i=0; i<n; i++)
    {
      if (vfh_aux->nfih[i] != NULL) {
        if (vfh_aux->nfih[i]->priv_fh != NULL) {
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

  XpnCreateMetadata(mdata_aux, pd, abs_path);
  ret = XpnUpdateMetadata(mdata_aux, n, servers, vfh_aux, abs_path);

  if (ret<0)
  {
    free(servers);
    free(vfh_aux);
    free(mdata_aux);

    return -1;
  }

  free(servers);

  return 0;
}


int xpn_base_rmdir(const char *path)
{
  char abs_path[PATH_MAX], url_serv[PATH_MAX];
  int ret, err, i, n, pd;
  struct nfi_server **servers;

  if(path == NULL) {
    xpn_err(XPNERR_PARAM);
    return -1;
  }

  ret = XpnGetAbsolutePath(path, abs_path); /* esta funcion genera el path absoluto */
  if(ret<0) {
    xpn_err(XPNERR_PATH_NOEXIST);
    return -1;
  }

  pd = XpnGetPartition(abs_path); /* return partition's id */
  if(pd<0) {
    xpn_err(XPNERR_PART_NOEXIST);
    return -1;
  }

  /* XpnGetServers: flag operation, partition id, absolute path, file descriptor, pointer to server */
  servers = NULL;
  n = XpnGetServers(op_xpn_rmdir, pd, abs_path, -1, &servers, XPN_DATA_SERVER);
  if (n<=0){
      return -1;
  }

  for (i=0; i<n; i++)
  {
    XpnGetURLServer(servers[i], abs_path, url_serv);
    nfi_worker_do_rmdir(servers[i]->wrk, url_serv);
  }

  // Wait
  err = 0;
  for (i=0; i<n; i++)
  {
    ret = nfiworker_wait(servers[i]->wrk);
    // Error checking
    if ((ret<0) && (!err)) {
        err = 1;
    }
  }

  // Error checking
  if (err) {
    free(servers);
    xpn_err(XPNERR_REMOVE);
    return -1;
  }

  free(servers);
  return 0;
}


DIR *xpn_base_opendir(const char *path)
{
  DIR *dirp = NULL;
  int res = -1;
  //char path_aux[255];
  char path_aux[PATH_MAX];

  XPN_DEBUG_BEGIN_ARGS1(path)

  if ((path == NULL)||(strlen(path)==0)||(strlen(path)>PATH_MAX))
  {
    errno = ENOENT;
    XPN_DEBUG_END_ARGS1(path)
    return NULL;
  }

  strcpy(path_aux,path);

  //if(path_aux[strlen(path_aux)-1] != '/')
  //    path_aux[strlen(path_aux)-1] = '/';

  if (path_aux[strlen(path)-1] != '/')
  {
    path_aux[strlen(path)] = '/';
    path_aux[strlen(path)+1] = '\0';
  }

  res = xpn_base_open(path_aux, O_RDONLY, 0);
  if (res < 0)
  {
    errno = ENOENT;
    XPN_DEBUG_END_ARGS1(path)
    return NULL;
  }

  dirp = (DIR *)malloc(sizeof(DIR));
  if (dirp == NULL)
  {
    XPN_DEBUG_END_ARGS1(path)
    return NULL;
  }
  dirp->fd = res;

  XPN_DEBUG_END_ARGS1(path)

  return dirp;
}


struct dirent* xpn_base_readdir(DIR *dirp)
{
  int res;
  struct dirent *dirnt = NULL;

  XPN_DEBUG_BEGIN

  if((NULL == dirp)||(dirp->fd<0)||(dirp->fd>XPN_MAX_FILE-1)){
    // set errno
    return NULL;
  }
  
  if(xpn_file_table[dirp->fd] == NULL){
    // xpn_err
    return NULL;
  }


  dirnt = (struct dirent *)malloc(sizeof(struct dirent));
  memset(dirnt, 0, sizeof(struct dirent));

  /*
  printf("xpn: struct dirent {             %d-%p\n",sizeof(struct dirent),dirnt);
  printf("    ino_t          d_ino;        %d+%d\n",sizeof(ino_t),(int)&dirnt->d_ino-(int)dirnt);
  printf("    off_t          d_off;        %d+%d\n",sizeof(off_t),(int)&dirnt->d_off-(int)dirnt);
  printf("    unsigned short d_reclen;     %d+%d\n",sizeof(unsigned short),(int)&dirnt->d_reclen-(int)dirnt);
  printf("    unsigned char  d_type;       %d+%d\n",sizeof(unsigned char),(int)&dirnt->d_type-(int)dirnt);
  printf("    char           d_name[256];  %d+%d\n",sizeof(struct dirent)-sizeof(ino_t)-sizeof(off_t)-sizeof(unsigned short)-sizeof(unsigned char),(int)&dirnt->d_name-(int)dirnt);
  printf("}\n");
  */

  res = XpnGetEntry(dirp->fd, dirnt->d_name, &(dirnt->d_type));
  if(res != 0)
  {
    free(dirnt);
    XPN_DEBUG_END
    return NULL;
  }

  XPN_DEBUG_END
  return dirnt;
}


int xpn_base_closedir(DIR *dirp)
{
  int i;
  
  if((NULL == dirp)||(dirp->fd<0)||(dirp->fd>XPN_MAX_FILE-1)){
    // set errno
    return -1;
  }
  
  if(xpn_file_table[dirp->fd] == NULL){
    // xpn_err
    return -1;
  }

  xpn_file_table[dirp->fd]->links--;

  if(xpn_file_table[dirp->fd]->links == 0)
  {
    for(i=0;i<xpn_file_table[dirp->fd]->data_vfh->n_nfih;i++)
    {
      if(xpn_file_table[dirp->fd]->data_vfh->nfih[i] != NULL)
      {
        if(xpn_file_table[dirp->fd]->data_vfh->nfih[i]->priv_fh != NULL){
          xpn_file_table[dirp->fd]->data_vfh->nfih[i]->server->ops->nfi_closedir( xpn_file_table[dirp->fd]->data_vfh->nfih[i]->server, xpn_file_table[dirp->fd]->data_vfh->nfih[i]);
        }

        free(xpn_file_table[dirp->fd]->data_vfh->nfih[i]);
      }
    }

    free(xpn_file_table[dirp->fd]->data_vfh->nfih);
    free(xpn_file_table[dirp->fd]->data_vfh);

    free(xpn_file_table[dirp->fd]->mdata->policy);
    free(xpn_file_table[dirp->fd]->mdata);

    free(xpn_file_table[dirp->fd]);
    xpn_file_table[dirp->fd] = NULL;
  }


  free(dirp);

  // set errno
  xpn_err(XPN_OK);
  return 0;
  //return -1;
}


void xpn_base_rewinddir(__attribute__((__unused__)) DIR *dirp)
{
  
}

