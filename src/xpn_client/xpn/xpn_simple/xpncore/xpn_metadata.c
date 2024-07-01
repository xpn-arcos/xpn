
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


#include "xpn/xpn_simple/xpn_metadata.h"
#include "xpn/xpn_simple/xpn_policy_open.h"

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