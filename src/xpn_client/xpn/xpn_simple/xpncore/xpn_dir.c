
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
  struct nfi_server *servers;
  int res = 0, err, i, n, pd;

  XPN_DEBUG_BEGIN_CUSTOM("%s, %d", path, perm);
  
  if(path == NULL)
  {
    errno = EINVAL;
    XPN_DEBUG_END;
    return -1;
  }

  res = XpnGetAbsolutePath(path, abs_path);
  if(res<0)
  {
    errno = ENOENT;
    XPN_DEBUG_END_ARGS1(path);
    return -1;
  }

  pd = XpnGetPartition(abs_path);
  if(pd<0)
  {
    errno = ENOENT;
    XPN_DEBUG_END_ARGS1(path);
    return -1;
  }

  servers = NULL;
  n = XpnGetServers(pd, -1, &servers);
  if(n<=0){
    XPN_DEBUG_END_ARGS1(path);
    return -1;
  }

  for(i=0;i<n;i++)
  {
    XpnGetURLServer(&servers[i], abs_path, url_serv);
    // Worker
    nfi_worker_do_mkdir(servers[i].wrk, url_serv, perm, NULL, NULL);
  }
  // Wait
  err = 0;
  for(i=0;i<n;i++)
  {
    res = nfiworker_wait(servers[i].wrk);
    if (res < 0) {
      err = 1;
    }
  }
  // Error checking
  if (err)
  {
    XPN_DEBUG_END_ARGS1(path);
    return -1;
  }

  // TODO: metadata
  // mdata_aux = (struct xpn_metadata *)malloc(sizeof(struct xpn_metadata));
  // if(mdata_aux == NULL)
  // {
  //   free(mdata_aux);

  //   return -1;
  // }

  // XpnCreateMetadata(mdata_aux, pd, abs_path);
  // ret = XpnUpdateMetadata(mdata_aux, n, servers, vfh_aux, abs_path);

  // if(ret<0)
  // {
  //   free(mdata_aux);

  //   return -1;
  // }

  XPN_DEBUG_END_ARGS1(path);
  return 0;
}

int xpn_simple_rmdir(const char *path)
{
  char abs_path[PATH_MAX], url_serv[PATH_MAX];
  int res = 0, err, i, n, pd;
  struct nfi_server *servers;

  XPN_DEBUG_BEGIN_CUSTOM("%s", path);

  if(path == NULL)
  {
    errno = EINVAL;
    XPN_DEBUG_END;
    return -1;
  }

  res = XpnGetAbsolutePath(path, abs_path);
  if(res<0)
  {
    errno = ENOENT;
    XPN_DEBUG_END_ARGS1(path);
    return -1;
  }

  pd = XpnGetPartition(abs_path);
  if(pd<0)
  {
    errno = ENOENT;
    XPN_DEBUG_END_ARGS1(path);
    return -1;
  }

  /* params:
   * flag operation , partition id,absolute path, file descript., pointer to server*/
  servers = NULL;
  n = XpnGetServers(pd, -1, &servers);
  if(n<=0){
    XPN_DEBUG_END_ARGS1(path);
    return -1;
  }
  int master_node = hash((char *)path, n);
  servers[master_node].wrk->arg.is_master_node = 1;
  for(i=0;i<n;i++)
  {
    XpnGetURLServer(&servers[i], abs_path, url_serv);
    if (i != master_node){
      servers[i].wrk->arg.is_master_node = 0;
    }
    // Worker
    servers[i].wrk->thread = servers[i].xpn_thread;
    nfi_worker_do_rmdir(servers[i].wrk, url_serv);
  }

  // Wait
  err = 0;
  for (i=0;i<n;i++)
  {
    res = nfiworker_wait(servers[i].wrk);
    // Error checking
    if((res<0)&&(!err)){
      err = 1;
    }
  }

  // Error checking
  if(err){
    res = -1;
  }

  XPN_DEBUG_END_ARGS1(path);
  return res;
}

