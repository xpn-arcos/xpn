
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


#include "xpn/xpn_simple/xpn_init.h"


struct xpn_partition xpn_parttable[XPN_MAX_PART];
pthread_mutex_t xpn_init_mutex = PTHREAD_MUTEX_INITIALIZER;
int xpn_debug=0;
int xpn_initialize=0;

int xpn_destroy_servers(struct xpn_partition *part)
{
  int i;
  struct nfi_server *serv;

  for(i=0;i<part->data_nserv;i++)
  {
    serv = part->data_serv;
    if(serv[i].ops != NULL)
    {
      part->data_serv[i].ops->nfi_disconnect(&(serv[i]));

      //part->data_serv[i].ops->nfi_destroy(&(serv[i]));
      if(serv[i].ops != NULL){
        free(serv[i].ops);
      }
    }
  }

  free(part->data_serv);

  for(i=0;i<part->meta_nserv;i++)
  {
    serv = part->meta_serv;
    if(serv[i].ops != NULL)
    {
      part->meta_serv[i].ops->nfi_disconnect(&(serv[i]));
      //part->meta_serv[i].ops->nfi_destroy(&(serv[i]));
      if(serv[i].ops != NULL){
        free(serv[i].ops);
      }
    }
  }

  free(part->meta_serv);

  return 0;
}

int xpn_simple_mark_error_server(int index)
{
  struct nfi_server *serv;
  int i = 0;
  while(xpn_parttable[i].id != -1)
    {
      serv = xpn_parttable[i].data_serv;
      if(serv[index].ops != NULL)
      {
        xpn_parttable[i].data_serv[index].ops->nfi_disconnect(&(serv[index]));

      xpn_parttable[i].data_serv[index].error = -1;
      }
      i++;
    }
    
  
  return 0;
}


int xpn_simple_destroy ( void )
{
  int res = 0;
  int i;

  XPN_DEBUG_BEGIN
  xpn_initialize = 0;
  if(xpn_parttable[0].id < 0)
  {
    res = 0;
    XPN_DEBUG_END
    return res;
  }

  xpn_destroy_file_table();
  nfi_worker_destroy();
  i = 0;

  while(xpn_parttable[i].id != -1)
  {
    xpn_destroy_servers(&(xpn_parttable[i]));
    xpn_parttable[i].id = -1;
    i++;
  }

  res = 0;
  XPN_DEBUG_END
  return res;
}

int xpn_init_partition(__attribute__((__unused__)) char *partname)
{
  int res;
  int i,j;
  struct conf_connect_st *fd;
  xpn_initialize = 0;
  char *env_debug;

  env_debug = getenv("XPN_DEBUG");
  if ((env_debug != NULL) && (strlen(env_debug) > 0)){
    xpn_debug=1;
  }

  XPN_DEBUG_BEGIN

  setbuf(stdout,NULL);
  setbuf(stderr,NULL);

  pthread_mutex_lock(&xpn_init_mutex);

  if(!xpn_initialize){
    XPN_DEBUG("Initializing\n");
  }
  else
  {
    XPN_DEBUG("Already initialized\n");

    pthread_mutex_unlock(&xpn_init_mutex);

    res = 0;
    XPN_DEBUG_END
    return res;
  }

  fd = XpnPartitionOpen(); /* XpnPartitionOpen */
  if(fd == NULL)
  {
    xpn_err(XPNERR_CONF);
    fprintf(stderr,"xpn_init: Can't open partition data\n");

    pthread_mutex_unlock(&xpn_init_mutex);

    res = -1;
    XPN_DEBUG_END
    return res;
  }

  i = 0;
  while((res = XpnGetNextPartition(fd, xpn_parttable[i].name)) > 0)
  {
    xpn_parttable[i].initialized = 0;

    //TODO: XpnGetIdPartition does not take into account that xpn_init xpn_destroy and xpn_init are done in one run
    // xpn_parttable[i].id = XpnGetIdPartition(fd, xpn_parttable[i].name);
    xpn_parttable[i].id = i;
    XPN_DEBUG("Partition %d: name=%s", xpn_parttable[i].id, xpn_parttable[i].name);
    
    /* compruebo los errores???? */
    if(XpnGetInfoPartition(fd, &(xpn_parttable[i]))==-1)
    {
      fprintf(stderr, "(4)xpn_init: %s info incomplete.\n", xpn_parttable[i].name);

      pthread_mutex_unlock(&xpn_init_mutex);

      res = -1;
      XPN_DEBUG_END
      return res;
    }    
    XPN_DEBUG("Partition %d: replication_level=%d", xpn_parttable[i].id, xpn_parttable[i].replication_level);

    xpn_parttable[i].data_nserv = XpnGetNumServersPartition(fd, &(xpn_parttable[i]) , XPN_DATA_SERVER);
    XPN_DEBUG("Partition %d: data_nserv=%d\n", xpn_parttable[i].id, xpn_parttable[i].data_nserv);

    xpn_parttable[i].data_serv=(struct nfi_server *)malloc(xpn_parttable[i].data_nserv*sizeof(struct nfi_server));
    if (xpn_parttable[i].data_serv == NULL)
    {
      fprintf(stderr, "xpn_init: Couldn't allocate memory\n");

      pthread_mutex_unlock(&xpn_init_mutex);

      res = -1;
      XPN_DEBUG_END
      return res;
    }

    memset(xpn_parttable[i].data_serv, 0, xpn_parttable[i].data_nserv*sizeof(struct nfi_server));

    for(j=0;j<xpn_parttable[i].data_nserv;j++)
    {
      //TODO: AQUI??

      res = XpnGetServer(fd, &(xpn_parttable[i]), &(xpn_parttable[i].data_serv[j]), XPN_DATA_SERVER);

      if(res<0){
        // Mark server with error for fault tolerant
        xpn_parttable[i].data_serv[j].error = -1;
        // XpnPartitionClose(fd);
        // for(j=0;j<i;j++){
        //   xpn_destroy_servers(&(xpn_parttable[j]));
        // }
        // fprintf(stderr,"xpn_init: Data configuration incorrect\n");

        // pthread_mutex_unlock(&xpn_init_mutex);

        // res = -1;
        // XPN_DEBUG_END
        // return res;
      }
    }
    
    int n_error = 0;
    for(j=0;j<xpn_parttable[i].data_nserv;j++)
    {
      if (xpn_parttable[i].data_serv[j].error != 0)
        n_error++;
      XPN_DEBUG("Partition %d: error server %d: %d",xpn_parttable[i].id,j,xpn_parttable[i].data_serv[j].error)
    }  
    XPN_DEBUG("Partition %d end", xpn_parttable[i].id);

    if (n_error > xpn_parttable[i].replication_level){
      XpnPartitionClose(fd);
      for(j=0;j<XPN_MAX_PART;j++){
        xpn_destroy_servers(&(xpn_parttable[j]));
      }
      fprintf(stderr,"xpn_init: More servers with errors (%d) than replication level permit (%d)\n",n_error,xpn_parttable[i].replication_level);

      pthread_mutex_unlock(&xpn_init_mutex);

      res = -1;
      XPN_DEBUG_END
      return res;
    }
    
    i++;
    if(i == XPN_MAX_PART)
    {
      XpnPartitionClose(fd);
      for(j=0;j<XPN_MAX_PART;j++){
        xpn_destroy_servers(&(xpn_parttable[j]));
      }
      fprintf(stderr,"xpn_init: Data configuration incorrect. Too much partitions \n");

      pthread_mutex_unlock(&xpn_init_mutex);

      res = -1;
      XPN_DEBUG_END
      return res;
    }
  } // while

  if(res<0)
  {
    XpnPartitionClose(fd);
    for(j=0;j<i;j++){
      xpn_destroy_servers(&(xpn_parttable[j]));
    }

    fprintf(stderr,"xpn_init: Data configuration incorrect.\n");

    pthread_mutex_unlock(&xpn_init_mutex);

    res = -1;
    XPN_DEBUG_END
    return res;
  }

  XpnPartitionClose(fd);

  /* Init the file table */
  res = xpn_init_file_table();
  if(res<0)
  {
    for(j=0;j<i;j++){
      xpn_destroy_servers(&(xpn_parttable[j]));
    }

    pthread_mutex_unlock(&xpn_init_mutex);

    res = -1;
    XPN_DEBUG_END
    return res;
  }

  /* Init the rest of elements of the table */
  for(j=0;j<i;j++){
    xpn_parttable[i].id = -1;
  }
  xpn_init_cwd();
  xpn_initialize = 1;

  pthread_mutex_unlock(&xpn_init_mutex);

  res = 0;
  XPN_DEBUG_END

  return res;
}

int xpn_simple_init ( void )
{
  return xpn_init_partition(NULL);
}

