
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


/* ... Include / Inclusion ........................................... */

  #include "xpn_conf_reader.h"


/* ... Functions / Funciones ......................................... */

int skip_spaces ( FILE *fp, long *line )
{
    int ch ;

    ch = getc(fp) ;
    while (ch != EOF)
    {
       if (strchr(" \t\n\r", ch) == NULL) {
           ungetc(ch, fp) ;
           return TOKEN ;
       }

       if ( (ch == '\n') || (ch == '\r') ) {
             (*line)++ ;
       }

       ch = getc(fp) ;
    }

    return EOF ;
}

int read_token ( FILE *fp, char *tok, long *line )
{
    int ch ;
    int i = 0 ;

    ch = getc(fp) ;
    if (ch == '[') { strcpy(tok, "["); return OPEN_BRACKET;  }
    if (ch == ']') { strcpy(tok, "]"); return CLOSE_BRACKET; }
    if (ch == '=') { strcpy(tok, "="); return EQUAL; }

    while (ch != EOF)
    {
       tok[i] = '\0' ;

       if (strchr("[]=", ch) != NULL) {
           ungetc(ch, fp) ;
           return TOKEN ;
       }

       if (strchr("\n\r", ch) != NULL) {
           (*line)++ ;
           return TOKEN ;
       }

       if (strchr(" \t", ch) != NULL) {
           return TOKEN ;
       }

       tok[i] = ch ;
       i++ ;
       ch = getc(fp) ;
    }

    tok[i] = '\0' ;
    return EOF ;
}

int next_token ( FILE *fp, char *tok, long *line )
{
    int elto ;

    elto = skip_spaces(fp, line) ;
    if (elto == EOF) {
        return EOF ;
    }

    return read_token(fp, tok, line) ;
}


//
// API
//

int xpn_conf_reader_free ( struct conf_file_data *conf_data )
{
    // check params
    if (NULL == conf_data) {
        fprintf(stderr, "xpn_conf_reader_free: ERROR: NULL conf_data argument.\n") ;
        return -1 ;
    }

    // remove existing allocated data
    for (int i=0; i<conf_data->partition_n; i++)
    {
         FREE_AND_NULL(conf_data->partitions[i].partition_name) ;

         for (int j=0; j<conf_data->partitions[i].server_n; j++) {
              FREE_AND_NULL(conf_data->partitions[i].servers[j]) ;
         }

         FREE_AND_NULL(conf_data->partitions[i].servers);
         conf_data->partitions[i].server_n = 0 ;
    }

    FREE_AND_NULL(conf_data->partitions) ;
    conf_data->partition_n = 0 ;

    // return OK
    return 0 ;
}

int xpn_conf_reader_load ( struct conf_file_data *conf_data, char *conf )
{
    FILE *fd;
    int   elto ;
    char  tok[MAX_TOKEN_LEN] ;
    char  key[MAX_TOKEN_LEN] ;
    char  value[MAX_TOKEN_LEN] ;
    int   current_partition ;
    int   current_server ;

    // check params
    if (NULL == conf_data) {
        fprintf(stderr, "xpn_conf_reader_load: ERROR: NULL conf_data argument.\n") ;
        return -1 ;
    }
    if (NULL == conf) {
        fprintf(stderr, "xpn_conf_reader_load: ERROR: NULL file name argument.\n") ;
        return -1 ;
    }

    // initial values
    conf_data->lines_n     = 1L;
    conf_data->partition_n = 0;
    conf_data->partitions  = NULL ;

    // open file
    fd = fopen(conf, "r");
    if (NULL == fd)
    {
        fprintf(stderr, "xpn_conf_reader_load: Can't open %s\n", conf) ;
        return -1;
    }

    // inside file...
    elto = next_token(fd, tok, &(conf_data->lines_n)) ;
    while (feof(fd) == 0)
    {
       // MATCH "[partition]"
       if (elto != OPEN_BRACKET) {
           printf("[%s:%ld] %s\n", conf, conf_data->lines_n, "ERROR: missing open bracket.\n") ;
           goto cleanup_error_XpnConfLoad;
       }

       elto = next_token(fd, value, &(conf_data->lines_n)) ;
       if (strcasecmp(value, "partition")) {
           printf("[%s:%ld] %s\n", conf, conf_data->lines_n, "ERROR: missing partition section.\n") ;
           goto cleanup_error_XpnConfLoad;
       }

       elto = next_token(fd, tok, &(conf_data->lines_n)) ;
       if (elto != CLOSE_BRACKET) {
           printf("[%s:%ld] %s\n", conf, conf_data->lines_n, "ERROR: missing partition section.\n") ;
           goto cleanup_error_XpnConfLoad;
       }

       // PROCESS partition...
       conf_data->partition_n++;
       current_partition = conf_data->partition_n - 1 ;

       conf_data->partitions = realloc(conf_data->partitions, conf_data->partition_n * sizeof(struct conf_file_data_partition));
       if (NULL == conf_data->partitions)
       {
          fprintf(stderr, "xpn_conf_reader_load: malloc for '%s' fails\n", conf) ;
          goto cleanup_error_XpnConfLoad;
       }
       conf_data->partitions[current_partition].partition_name    = NULL ; // [P1] -> strdup(value)
       conf_data->partitions[current_partition].replication_level = XPN_CONF_DEFAULT_REPLICATION_LEVEL ;
       conf_data->partitions[current_partition].bsize             = XPN_CONF_DEFAULT_BLOCKSIZE ;
       conf_data->partitions[current_partition].server_n          = 0 ;
       conf_data->partitions[current_partition].servers           = NULL ;

       // fields of partition...
       while (feof(fd) == 0)
       {
          elto = next_token(fd, key, &(conf_data->lines_n)) ;
          if (elto == OPEN_BRACKET) break ;

          // MATCH "<field> = <value>"
          if (elto != TOKEN) {
              printf("[%s:%ld] %s\n", conf, conf_data->lines_n, "ERROR: missing key.\n") ;
              goto cleanup_error_XpnConfLoad;
          }

          elto = next_token(fd, tok, &(conf_data->lines_n)) ;
          if (elto != EQUAL) {
              printf("[%s:%ld] %s\n", conf, conf_data->lines_n, "ERROR: missing equal.\n") ;
              goto cleanup_error_XpnConfLoad;
          }

          elto = next_token(fd, value, &(conf_data->lines_n)) ;
          if (elto != TOKEN) {
              printf("[%s:%ld] %s\n", conf, conf_data->lines_n, "ERROR: missing value.\n") ;
              goto cleanup_error_XpnConfLoad;
          }

          // PROCESS <field> = <value>
          //printf("%ld: %s = %s\n", conf_data->lines_n, key, value) ;  // DEBUG

          // server_url = sck_server://localhost/tmp/work/data
          if (strcmp(key, XPN_CONF_TAG_SERVER_URL) == 0)
          {
              conf_data->partitions[current_partition].server_n++;
              current_server = conf_data->partitions[current_partition].server_n - 1 ;

                          conf_data->partitions[current_partition].servers = realloc(conf_data->partitions[current_partition].servers, (conf_data->partitions[current_partition].server_n)*sizeof(char *)) ;
              if (NULL == conf_data->partitions[current_partition].servers)
              {
                  fprintf(stderr, "xpn_conf_reader_load: malloc for '%s' fails\n", conf) ;
                  goto cleanup_error_XpnConfLoad;
              }

                          conf_data->partitions[current_partition].servers[current_server] = strdup(value) ;
              if (NULL == conf_data->partitions[current_partition].servers[current_server])
              {
                  fprintf(stderr, "xpn_conf_reader_load: malloc for '%s' fails\n", conf) ;
                  goto cleanup_error_XpnConfLoad;
              }
          }
          // bsize = 512k
          else if (strcasecmp(key, XPN_CONF_TAG_BLOCKSIZE) == 0)
          {
              conf_data->partitions[current_partition].bsize = getSizeFactor(value) ;
          }
          // replication_level = 0
          else if (strcasecmp(key, XPN_CONF_TAG_REPLICATION_LEVEL) == 0)
          {
              conf_data->partitions[current_partition].replication_level = atoi(value) ;
          }
          // partition_name = P1
          else if (strcasecmp(key, XPN_CONF_TAG_PARTITION_NAME) == 0)
          {
              conf_data->partitions[current_partition].partition_name = strdup(value) ;
          }
          else
          {
              fprintf(stderr, "xpn_conf_reader_load: unknown key '%s'\n", key) ;
              goto cleanup_error_XpnConfLoad;
          }

          // for looping...
          elto = skip_spaces(fd, &(conf_data->lines_n)) ;
       }
    }

    fclose(fd) ;
    return 0 ;

cleanup_error_XpnConfLoad:
    fclose(fd) ;
    xpn_conf_reader_free(conf_data);
    return -1;
}


int xpn_conf_reader_show ( FILE *fd, struct conf_file_data *conf_data )
{
    // check params
    if (NULL == fd) {
        fprintf(stderr, "xpn_conf_reader_free: ERROR: NULL file descriptor argument.\n") ;
        return -1 ;
    }
    if (NULL == conf_data) {
        fprintf(stderr, "xpn_conf_reader_free: ERROR: NULL conf_data argument.\n") ;
        return -1 ;
    }

    // remove existing allocated data
    for (int i=0; i<conf_data->partition_n; i++)
    {
         fprintf(fd, " [%d] partition: %s\n", i,         conf_data->partitions[i].partition_name) ;

         fprintf(fd, "     ** bsize: %ld\n",             conf_data->partitions[i].bsize) ;
         fprintf(fd, "     ** replication level: %d\n",  conf_data->partitions[i].replication_level) ;
         for (int j=0; j<conf_data->partitions[i].server_n; j++) {
              fprintf(fd, "     ** server %d: %s\n", j,  conf_data->partitions[i].servers[j]) ;
         }
    }

    // return OK
    return 0 ;
}


int xpn_conf_reader_get_num_partitions ( struct conf_file_data *conf_data )
{
    // check params
    if (NULL == conf_data) {
        fprintf(stderr, "xpn_conf_reader_get_num_partitions: ERROR: NULL conf_data argument.\n") ;
        return -1 ;
    }

    return conf_data->partition_n ;
}

int xpn_conf_reader_get_num_servers ( struct conf_file_data *conf_data, int partition_index )
{
    // check params
    if (NULL == conf_data) {
        fprintf(stderr, "xpn_conf_reader_get_num_servers: ERROR: NULL conf_data argument.\n") ;
        return -1 ;
    }

    // check ranges
    if (partition_index >= conf_data->partition_n) {
        fprintf(stderr, "xpn_conf_reader_get_num_servers: ERROR: partition index '%d' out of range.\n", partition_index) ;
	return -1 ;
    }

    return conf_data->partitions[partition_index].server_n ;
}

int xpn_conf_reader_get_server ( struct conf_file_data *conf_data, char *value, int partition, int server )
{
    // check params
    if (NULL == conf_data) {
        fprintf(stderr, "xpn_conf_reader_get_server: ERROR: NULL conf_data argument.\n") ;
        return -1 ;
    }
    if (NULL == value) {
        fprintf(stderr, "xpn_conf_reader_get_server: ERROR: NULL value argument.\n") ;
        return -1 ;
    }

    // check ranges
    if (partition >= conf_data->partition_n) {
        fprintf(stderr, "xpn_conf_reader_get_server: ERROR: partition index '%d' out of range.\n", partition) ;
	return -1 ;
    }
    if (server >= conf_data->partitions[partition].server_n) {
        fprintf(stderr, "xpn_conf_reader_get_server: ERROR: server index '%d' out of range.\n", server) ;
	return -1 ;
    }

    strcpy(value, conf_data->partitions[partition].servers[server]) ;
    return 0 ;
}

int xpn_conf_reader_get_value ( struct conf_file_data *conf_data, int partition_index, char *key, char *value )
{
    // check params
    if (NULL == conf_data) {
        fprintf(stderr, "xpn_conf_reader_get_value: ERROR: NULL conf_data argument.\n") ;
        return -1 ;
    }
    if (NULL == key) {
        fprintf(stderr, "xpn_conf_reader_get_value: ERROR: NULL key argument.\n") ;
        return -1 ;
    }
    if (NULL == value) {
        fprintf(stderr, "xpn_conf_reader_get_value: ERROR: NULL value argument.\n") ;
        return -1 ;
    }

    // check ranges
    if (partition_index >= conf_data->partition_n) {
        fprintf(stderr, "xpn_conf_reader_get_value: ERROR: partition index '%d' out of range.\n", partition_index) ;
	return -1 ;
    }

    // bsize = 512k
    if (strcasecmp(key, XPN_CONF_TAG_BLOCKSIZE) == 0)
    {
	sprintf(value, "%ld", conf_data->partitions[partition_index].bsize) ;
    }
    // replication_level = 0
    else if (strcasecmp(key, XPN_CONF_TAG_REPLICATION_LEVEL) == 0)
    {
	sprintf(value, "%d", conf_data->partitions[partition_index].replication_level) ;
    }
    // partition_name = P1
    else if (strcasecmp(key, XPN_CONF_TAG_PARTITION_NAME) == 0)
    {
	strcpy(value, conf_data->partitions[partition_index].partition_name) ;
    }
    else
    {
        fprintf(stderr, "xpn_conf_reader_get_value: unknown key '%s'\n", key) ;
	return -1 ;
    }

    // return OK
    return 0 ;
}


//
// Old API
//

int XpnConfGetServer ( struct conf_file_data *conf_data, char *value, int partition, int server )
{
    return xpn_conf_reader_get_server (conf_data, value, partition, server) ;
}

int XpnConfGetValue ( struct conf_file_data *conf_data, char *key, char *value, int partition )
{
    return xpn_conf_reader_get_value(conf_data, partition, key, value) ;
}

int XpnConfGetNumPartitions ( struct conf_file_data *conf_data )
{
    return xpn_conf_reader_get_num_partitions(conf_data) ;
}

int XpnConfGetNumServers ( struct conf_file_data *conf_data, int partition_index )
{
    return xpn_conf_reader_get_num_servers(conf_data, partition_index) ;
}

int XpnConfLoad ( struct conf_file_data *conf_data )
{
    int ret ;
    char  conf[KB] ;
    char *env_val ;

    env_val = getenv(XPN_CONF) ;
    if (env_val != NULL)
         strcpy(conf, env_val);
    else strcpy(conf, XPN_CONF_DEFAULT);

    ret = xpn_conf_reader_load(conf_data, conf) ;
//  printf(" ******************\n") ;
//  xpn_conf_reader_show(stderr, conf_data) ;
//  printf(" ******************\n") ;

    return ret ;
}

void XpnConfFree(struct conf_file_data *conf_data)
{
    xpn_conf_reader_free(conf_data) ;
}


/* ................................................................... */

