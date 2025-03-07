
/*
 *  Copyright 2020-2025 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Dario Muñoz Muñoz
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


#ifndef _XPN_CONF_READER_H_
#define _XPN_CONF_READER_H_

  #ifdef  __cplusplus
    extern "C" {
  #endif

  /* ... Include / Inclusion ........................................... */

     #include "all_system.h"
     #include "base/utils.h"
     #include "base/path_misc.h"


  /* ... Defines / Definiciones ........................................ */

     #define TOKEN           100
     #define OPEN_BRACKET    101
     #define CLOSE_BRACKET   102
     #define EQUAL           103

     #define MAX_TOKEN_LEN   1024

     #define XPN_CONF                           "XPN_CONF"
     #define XPN_CONF_DEFAULT                   "/etc/xpn/xpn.conf"
 //  #define XPN_CONF_DEFAULT                   "xpn.conf"

     #define XPN_CONF_TAG_PARTITION             "[partition]"
     #define XPN_CONF_TAG_PARTITION_NAME        "partition_name"
     #define XPN_CONF_TAG_REPLICATION_LEVEL     "replication_level"
     #define XPN_CONF_TAG_BLOCKSIZE             "bsize"
     #define XPN_CONF_TAG_SERVER_URL            "server_url"

     #define XPN_CONF_DEFAULT_REPLICATION_LEVEL 0
     #define XPN_CONF_DEFAULT_BLOCKSIZE         512*KB


  /* ... Data structures / Estructuras de datos ........................ */

     struct conf_file_data_partition
     {
       char   *partition_name;
       int     replication_level;
       long    bsize;
       int     server_n;           // Array of number of servers in partition
       char  **servers;            // The pointers to the servers
     };

     struct conf_file_data
     {
       long    lines_n;            // Number of lines
       int     partition_n;        // Number of partitions
       struct conf_file_data_partition *partitions ;
     };


  /* ... Functions / Funciones ......................................... */

     int skip_spaces ( FILE *fp, long *line ) ;
     int read_token  ( FILE *fp, char *tok, long *line ) ;
     int next_token  ( FILE *fp, char *tok, long *line ) ;

     int xpn_conf_reader_load ( struct conf_file_data *conf_data, char *fname ) ;
     int xpn_conf_reader_free ( struct conf_file_data *conf_data ) ;
     int xpn_conf_reader_show ( FILE *fd, struct conf_file_data *conf_data ) ;

     int xpn_conf_reader_get_num_partitions ( struct conf_file_data *conf_data ) ;
     int xpn_conf_reader_get_num_servers    ( struct conf_file_data *conf_data, int partition_index ) ;
     int xpn_conf_reader_get_server         ( struct conf_file_data *conf_data, char *value, int partition, int server ) ;
     int xpn_conf_reader_get_value          ( struct conf_file_data *conf_data, int partition_index, char *key, char *value ) ;

     // Old API
     int  XpnConfLoad             ( struct conf_file_data *conf_data );
     void XpnConfFree             ( struct conf_file_data *conf_data );

     int  XpnConfGetServer        ( struct conf_file_data *conf_data, char *value, int partition, int server ) ;
     int  XpnConfGetValue         ( struct conf_file_data *conf_data, char *key, char *value, int partition );
     int  XpnConfGetNumPartitions ( struct conf_file_data *conf_data );
     int  XpnConfGetNumServers    ( struct conf_file_data *conf_data, int partition_index );


  /* ................................................................... */

  #ifdef  __cplusplus
    }
  #endif

#endif
