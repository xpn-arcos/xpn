
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


#include "xpn/xpn_base/xpn_base_bulk.h"


extern struct xpn_filedesc *xpn_file_table[XPN_MAX_FILE];


int xpn_base_preload(const char *virtual_path, const char *storage_path)
{
  char abs_path[PATH_MAX], url_serv[PATH_MAX];
  struct nfi_server **servers;
  struct xpn_metadata *mdata;
  int res, i, n, pd;

  XPN_DEBUG_BEGIN_ARGS2(virtual_path, storage_path)

  if(virtual_path == NULL)
  {
    xpn_err(XPNERR_PARAM);
    res = -1;
    XPN_DEBUG_END
    return res;
  }

  if(storage_path == NULL)
  {
    xpn_err(XPNERR_PARAM);
    res = -1;
    XPN_DEBUG_END
    return res;
  }

  res = XpnGetAbsolutePath(virtual_path, abs_path); // esta funcion genera el path absoluto 
  if(res<0)
  {
    xpn_err(XPNERR_PATH_NOEXIST);
    res = -1;
    XPN_DEBUG_END
    return res;
  }

  pd = XpnGetPartition(abs_path); // return partition's id 
  if(pd<0)
  {
    xpn_err(XPNERR_PART_NOEXIST);
    res = -1;
    XPN_DEBUG_END
    return res;
  }

  /* params:
   * flag operation, partition id, absolute path, file descriptor, pointer to server
   */
  servers = NULL;
  n = XpnGetServers(op_xpn_flush, pd, abs_path, -1, &servers, XPN_DATA_SERVER);
  if(n<=0)
  {
    //free(servers);
    res = -1;
    XPN_DEBUG_END
    return res;
  }

  mdata = (struct xpn_metadata *)malloc(sizeof(struct xpn_metadata));
  if(mdata == NULL)
  {
    free(servers);
    free(mdata);

    res = -1;
    XPN_DEBUG_END
    return res;
  }

  XpnCreateMetadata(mdata, pd, abs_path);

  i = XpnGetMetadataPos(mdata, -1); //TODO: pasar por parametro
  printf("metadata %d\n", i) ;

  for (int j = 0; j < n; ++j)
  {
    XpnGetURLServer(servers[j], abs_path, url_serv);

    // Worker
    servers[i]->wrk->thread = servers[i]->xpn_thread;
    nfi_worker_do_preload( servers[j]->wrk, url_serv, (char *)url_serv, (char *)storage_path, 1);
  }

  // Wait
  for (int j = 0; j < n; ++j){
    res = nfiworker_wait(servers[i]->wrk) ;
  }

  free(servers);
  free(mdata);

  // error checking 
  if(res)
  {
    xpn_err(XPNERR_PARAM);
    res = -1;
    XPN_DEBUG_END
    return res;
  }

  res = 0;
  XPN_DEBUG_END
  return res;
}


int xpn_base_flush(const char *virtual_path, const char *storage_path)
{
  char abs_path[PATH_MAX], url_serv[PATH_MAX];
  struct nfi_server **servers;
  struct xpn_metadata *mdata;
  int res, i, n, pd;


  if(virtual_path == NULL)
  {
    xpn_err(XPNERR_PARAM);
    return -1;
  }

  if(storage_path == NULL)
  {
    xpn_err(XPNERR_PARAM);
    return -1;
  }

  res = XpnGetAbsolutePath(virtual_path, abs_path); // esta funcion genera el path absoluto 
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
  n = XpnGetServers(op_xpn_flush, pd, abs_path, -1, &servers, XPN_DATA_SERVER);
  if(n<=0){
    //free(servers);
    return -1;
  }

  mdata = (struct xpn_metadata *)malloc(sizeof(struct xpn_metadata));
  if(mdata == NULL)
  {
    free(servers);
    free(mdata);

    return -1;
  }

  XpnCreateMetadata(mdata, pd, abs_path);

  i = XpnGetMetadataPos(mdata, -1);
  // TODO: i variable is not used, next line avoid compilation warning
  i = i ;

  for (int j = 0; j < n; ++j)
  {
    XpnGetURLServer(servers[j], abs_path, url_serv);

    // Worker
    servers[i]->wrk->thread = servers[i]->xpn_thread;
    nfi_worker_do_flush(servers[j]->wrk, url_serv, (char *)url_serv, (char *)storage_path, 1);
  }

  // Wait
  for (int j = 0; j < n; ++j){
    res = nfiworker_wait(servers[i]->wrk) ;
  }

  free(servers);
  free(mdata);

  // error checking 
  /*if(err){
    xpn_err(XPNERR_PARAM);
    return -1;
  }*/

  return 0;
}

