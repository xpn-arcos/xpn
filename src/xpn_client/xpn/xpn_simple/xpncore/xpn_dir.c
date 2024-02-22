
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


#include "xpn/xpn_simple/xpn_dir.h"

#include "xpn/xpn_simple/xpn_open.h"


int xpn_simple_mkdir(const char *path, mode_t perm)
{
  char abs_path[PATH_MAX], url_serv[PATH_MAX];
  struct nfi_server **servers;
  struct xpn_metadata *mdata_aux;
  struct nfi_attr attr;
  int ret, err, i, j, n, pd;
  struct xpn_fh *vfh_aux;

  if(path == NULL)
  {
    xpn_err(XPNERR_PARAM);
    return -1;
  }

  ret = XpnGetAbsolutePath(path, abs_path); /* esta funcion genera el path absoluto */
  if(ret<0)
  {
    xpn_err(XPNERR_PATH_NOEXIST);
    return -1;
  }

  pd = XpnGetPartition(abs_path); /* return partition's id */
  if(pd<0)
  {
    xpn_err(XPNERR_PART_NOEXIST);
    return -1;
  }

  //Check if directory exist
  struct stat sb;
  ret = xpn_simple_stat(path, &sb);
  if (ret == 0)
  {
    errno = EEXIST;
    return -1;
  }


  /* params:
   * flag operation , partition id,absolute path, file descript., pointer to server*/
  servers = NULL;
  n = XpnGetServers(op_xpn_mkdir, pd, abs_path, -1, &servers, XPN_DATA_SERVER);
  if(n<=0){
    /*free(servers);*/
    return -1;
  }
  bzero(&attr, sizeof(struct nfi_attr));

  attr.at_mode = S_IFDIR  | perm;

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

    XpnGetURLServer(servers[i], abs_path, url_serv);

    vfh_aux->nfih[i] = (struct nfi_fhandle*)malloc(sizeof(struct nfi_fhandle));
    memset(vfh_aux->nfih[i], 0, sizeof(struct nfi_fhandle));
    if(vfh_aux->nfih[i] == NULL)
    {
      free(servers);
      return -1;
    }
    // Worker
    nfi_worker_do_mkdir(servers[i]->wrk, url_serv, &attr, vfh_aux->nfih[i]);

  }
  // Wait
  err = 0;
  for(i=0;i<n;i++)
  {
    ret = nfiworker_wait(servers[i]->wrk);

    // Control error
    if((ret<0)&&(!err))
    {
      /* erase the file create before de server number i */
      err = 1;
      for(j=0; j<i; j++)
      {
        /*
        XpnGetURLServer(servers[j], abs_path, url_serv);
        nfi_worker_do_rmdir(servers[j]->wrk, url_serv);
        nfi_worker_wait(servers[j]->wrk);
        */
      }
    }
    else
    {
      if((ret>=0)&&(err))
      {
        /*
        XpnGetURLServer(servers[i], abs_path, url_serv);
        nfi_worker_do_rmdir(servers[i]->wrk, url_serv);
        nfi_worker_wait(servers[i]->wrk);
        */
      }
    }
  }

  // Control error
  /*if(err)
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

    return -1;
  }*/

  // Error checking
  if(err)
  {
    xpn_err(XPNERR_REMOVE);
    free(servers);
    return -1;
  }

  mdata_aux = (struct xpn_metadata *)malloc(sizeof(struct xpn_metadata));
  if(mdata_aux == NULL)
  {
    free(servers);
    /* tambien los punteros indirectos */
    for(i=0;i<n;i++){
      if(vfh_aux->nfih[i] != NULL){
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

  XpnCreateMetadata(mdata_aux, pd, abs_path);
  ret = XpnUpdateMetadata(mdata_aux, n, servers, vfh_aux, abs_path);

  if(ret<0)
  {
    free(servers);
    /* tambien los punteros indirectos */
    free(vfh_aux);
    free(mdata_aux);

    return -1;
  }

  /*

  (*vfh )= vfh_aux;
  (*mdata )= mdata_aux;

  */

  free(servers);
  return 0;
}

int xpn_simple_rmdir(const char *path)
{
  char abs_path[PATH_MAX], url_serv[PATH_MAX];
  int ret, err, i, n, pd;
  struct nfi_server **servers;

  if(path == NULL)
  {
    xpn_err(XPNERR_PARAM);
    return -1;
  }

  ret = XpnGetAbsolutePath(path, abs_path); /* esta funcion genera el path absoluto */
  if(ret<0)
  {
    xpn_err(XPNERR_PATH_NOEXIST);
    return -1;
  }

  pd = XpnGetPartition(abs_path); /* return partition's id */
  if(pd<0)
  {
    xpn_err(XPNERR_PART_NOEXIST);
    return -1;
  }

  /* params:
   * flag operation , partition id,absolute path, file descript., pointer to server*/
  servers = NULL;
  n = XpnGetServers(op_xpn_rmdir, pd, abs_path, -1, &servers, XPN_DATA_SERVER);
  if(n<=0){
   /* free(servers); */
    return -1;
  }

  for(i=0;i<n;i++)
  {
    XpnGetURLServer(servers[i], abs_path, url_serv);
    // Worker
    nfi_worker_do_rmdir(servers[i]->wrk, url_serv);

  }

  // Wait
  err = 0;
  for (i=0;i<n;i++)
  {
    ret = nfiworker_wait(servers[i]->wrk);
    // Error checking
    if((ret<0)&&(!err)){
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

  free(servers);
  return 0;
}

