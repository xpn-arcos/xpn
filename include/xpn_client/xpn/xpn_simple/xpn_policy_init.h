
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

#ifndef _XPN_POLICY_INIT_H
#define _XPN_POLICY_INIT_H

  #ifdef  __cplusplus
    extern "C" {
  #endif


  /* ... Include / Inclusion ........................................... */

  #include "xpn.h"
  #include "xpn_init.h"


  /* ... Const / Const ................................................. */

  #define XPN_CONF                           "XPN_CONF"
  #define XPN_CONF_TAG_PARTITION             "[partition]"
  #define XPN_CONF_TAG_PARTITION_NAME        "partition_name"
  #define XPN_CONF_TAG_REPLICATION_LEVEL     "replication_level"
  #define XPN_CONF_TAG_BLOCKSIZE             "bsize"
  #define XPN_CONF_TAG_SERVER_URL            "server_url"
  #define XPN_CONF_DEFAULT_REPLICATION_LEVEL "0"
  #define XPN_CONF_DEFAULT_BLOCKSIZE         "512K"

  #define XPN_CONF_DEFAULT  "/etc/xpn/xpn.conf"


  /* ... Data structures / Estructuras de datos ........................ */

  struct conf_file_data
  {
    char *data;               //All the data
    int lines_n;          //Number of lines
    char **lines;             //The pointers to the lines
    int partition_n;       //Number of partitions
    int *server_n;         //Array of number of servers in partition
    int *server_url_index;    //Array of index to line of server_url in lines
  };

  /* ... Functions / Funciones ......................................... */
  
  int XpnConfGetValueRept(struct conf_file_data *conf_data, char *key, char *value, int partition, int rept);
  int XpnConfGetValue(struct conf_file_data *conf_data, char *key, char *value, int partition);
  int XpnConfGetNumPartitions(struct conf_file_data *conf_data);
  int XpnConfGetNumServers(struct conf_file_data *conf_data, int partition_index);
  int XpnConfLoad(struct conf_file_data *conf_data);
  void XpnConfFree(struct conf_file_data *conf_data);
  
  int XpnInitServer(struct conf_file_data *conf_data, struct xpn_partition * part, struct nfi_server * serv, int server_num);
  
  struct xpn_partition*    XpnSearchPart(int pd);
  int XpnGetPartition(char *path);


  /* ................................................................... */

  #ifdef  __cplusplus
    }
  #endif

#endif
