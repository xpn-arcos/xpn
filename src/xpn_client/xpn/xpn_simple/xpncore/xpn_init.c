
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

  XPN_DEBUG_BEGIN;
  pthread_mutex_lock(&xpn_init_mutex);
  if ( xpn_initialize == 0){
    res = 0;
    goto cleanup_xpn_simple_destroy;
  }
  xpn_initialize = 0;
  if(xpn_parttable[0].id < 0)
  {
    res = 0;
    goto cleanup_xpn_simple_destroy;
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
cleanup_xpn_simple_destroy:
  pthread_mutex_unlock(&xpn_init_mutex);
  XPN_DEBUG_END;
  return res;
}

int xpn_init_partition( void )
{
  int res;
  int i,j;
  char *env_debug;
  int part_n = 0;
  char buff_value[PATH_MAX];
  struct conf_file_data conf_data = {0};

  env_debug = getenv("XPN_DEBUG");
  if ((env_debug != NULL) && (strlen(env_debug) > 0)){
    xpn_debug=1;
  }

  XPN_DEBUG_BEGIN;

  setbuf(stdout,NULL);
  setbuf(stderr,NULL);

  pthread_mutex_lock(&xpn_init_mutex);

  if(!xpn_initialize){
    XPN_DEBUG("Initializing");
  }
  else
  {
    XPN_DEBUG("Already initialized");
    res = 0;
    goto cleanup_xpn_init_partition;
  }

  res = XpnConfLoad(&conf_data);
  if (res != 0)
  {
    res = -1;
    goto cleanup_xpn_init_partition;
  }

  part_n = XpnConfGetNumPartitions(&conf_data);
  if (part_n <= 0)
  {
    fprintf(stderr, "xpn_init: Fail in parse config file\n");
    res = -1;
    goto cleanup_xpn_init_partition;
  }
  for (i = 0; i < part_n; i++)
  {
    xpn_parttable[i].id = i;

    res = XpnConfGetValue(&conf_data, XPN_CONF_TAG_PARTITION_NAME, buff_value, i);
    if (res != 0)
    {
      fprintf(stderr, "xpn_init: Error in conf_file: "XPN_CONF_TAG_PARTITION_NAME" not found in %d partition\n", i);
      res = -1;
      goto cleanup_xpn_init_partition;
    }
    strcpy(xpn_parttable[i].name, buff_value);
    XPN_DEBUG("Partition %d: name=%s", xpn_parttable[i].id, xpn_parttable[i].name);

    // Block_size
    res = XpnConfGetValue(&conf_data, XPN_CONF_TAG_BLOCKSIZE, buff_value, i);
    if (res != 0 || getSizeFactor(buff_value) == 1)
      strcpy(buff_value, XPN_CONF_DEFAULT_BLOCKSIZE);
    xpn_parttable[i].block_size =  getSizeFactor(buff_value);
    XPN_DEBUG("Partition %d: block_size=%ld", xpn_parttable[i].id, xpn_parttable[i].block_size);

    // Replication_level
    res = XpnConfGetValue(&conf_data, XPN_CONF_TAG_REPLICATION_LEVEL, buff_value, i);
    if (res != 0 || atoi(buff_value) < 0)
      strcpy(buff_value, XPN_CONF_DEFAULT_REPLICATION_LEVEL);
    xpn_parttable[i].replication_level = atoi(buff_value);
    XPN_DEBUG("Partition %d: replication_level=%d", xpn_parttable[i].id, xpn_parttable[i].replication_level);

    // data_nserv
    xpn_parttable[i].data_nserv = XpnConfGetNumServers(&conf_data, i);
    if (xpn_parttable[i].data_nserv <= 0)
    {
      fprintf(stderr, "xpn_init: Error in conf_file: %d servers found, minimum 1 in %d partition\n", xpn_parttable[i].data_nserv ,i);
      res = -1;
      goto cleanup_xpn_init_partition;
    }
    XPN_DEBUG("Partition %d: data_nserv=%d", xpn_parttable[i].id, xpn_parttable[i].data_nserv);
    
    xpn_parttable[i].data_serv=(struct nfi_server *)malloc(xpn_parttable[i].data_nserv*sizeof(struct nfi_server));
    if (xpn_parttable[i].data_serv == NULL)
    {
      fprintf(stderr, "xpn_init: Couldn't allocate memory\n");
      res = -1;
      goto cleanup_xpn_init_partition;
    }

    memset(xpn_parttable[i].data_serv, 0, xpn_parttable[i].data_nserv*sizeof(struct nfi_server));

    // Init all servers
    for(j=0;j<xpn_parttable[i].data_nserv;j++)
    {
      res = XpnInitServer(&conf_data, &(xpn_parttable[i]), &(xpn_parttable[i].data_serv[j]), j);
      if(res<0)
      {
        xpn_parttable[i].data_serv[j].error = -1;
      }
    }

    // Check if there are to much servers with errors than replication level permit
    int n_error = 0;
    for(j=0;j<xpn_parttable[i].data_nserv;j++)
    {
      if (xpn_parttable[i].data_serv[j].error != 0)
        n_error++;
      XPN_DEBUG("Partition %d: error server %d: %d",xpn_parttable[i].id,j,xpn_parttable[i].data_serv[j].error);
    }  
    XPN_DEBUG("Partition %d end", xpn_parttable[i].id);

    if (n_error > xpn_parttable[i].replication_level)
    {
      for(j=0;j<XPN_MAX_PART;j++){
        xpn_destroy_servers(&(xpn_parttable[j]));
      }
      fprintf(stderr,"xpn_init: More servers with errors (%d) than replication level permit (%d)\n",n_error,xpn_parttable[i].replication_level);

      res = -1;
      goto cleanup_xpn_init_partition;
    }
    
    // Check if there are to much parts
    if(i == XPN_MAX_PART)
    {
      for(j=0;j<XPN_MAX_PART;j++)
      {
        xpn_destroy_servers(&(xpn_parttable[j]));
      }
      fprintf(stderr,"xpn_init: Data configuration incorrect. Too much partitions \n");
      res = -1;
      goto cleanup_xpn_init_partition;
    }
  }
  
  /* Init the file table */
  res = xpn_init_file_table();
  if(res<0)
  {
    for(j=0;j<i;j++)
    {
      xpn_destroy_servers(&(xpn_parttable[j]));
    }
    res = -1;
    goto cleanup_xpn_init_partition;
  }

  /* Init the rest of elements of the table */
  for(j=0;j<i;j++)
  {
    xpn_parttable[i].id = -1;
  }
  xpn_init_cwd();
  xpn_initialize = 1;
  res = 0;
cleanup_xpn_init_partition:
  XpnConfFree(&conf_data);
  pthread_mutex_unlock(&xpn_init_mutex);

  XPN_DEBUG_END;
  return res;
}

int xpn_simple_init ( void )
{
  return xpn_init_partition();
}

