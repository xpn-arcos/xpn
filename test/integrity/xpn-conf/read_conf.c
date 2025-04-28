
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


#define TOKEN           100
#define OPEN_BRACKET    101
#define CLOSE_BRACKET   102
#define EQUAL           103


int skip_spaces ( FILE *fp, long *line )
{
    char ch ;

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
    char ch ;
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


////////////////////

  #define KB 1024

  #define FREE_AND_NULL(ptr) \
           if ((ptr) != NULL) { free((ptr)); (ptr) = NULL; }

  #define XPN_CONF                           "XPN_CONF"
  #define XPN_CONF_TAG_PARTITION             "[partition]"
  #define XPN_CONF_TAG_PARTITION_NAME        "partition_name"
  #define XPN_CONF_TAG_REPLICATION_LEVEL     "replication_level"
  #define XPN_CONF_TAG_BLOCKSIZE             "bsize"
  #define XPN_CONF_TAG_SERVER_URL            "server_url"
  #define XPN_CONF_DEFAULT_REPLICATION_LEVEL 0
  #define XPN_CONF_DEFAULT_BLOCKSIZE         512*KB

  #define XPN_CONF_DEFAULT                   "xpn.conf"

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


int XpnConfFree ( struct conf_file_data *conf_data )
{
    if (NULL == conf_data->partitions) {
	return 0 ;
    }

    for (int i=0; i<conf_data->partition_n; i++)
    {
	 FREE_AND_NULL(conf_data->partitions[i].partition_name) ;

         for (int j=0; j<conf_data->partitions[i].server_n; j++) {
	      FREE_AND_NULL(conf_data->partitions[i].servers[j]) ;
	 }
         FREE_AND_NULL(conf_data->partitions[i].servers);
    }
    FREE_AND_NULL(conf_data->partitions) ;

    return 0 ;
}

int XpnConfLoad ( struct conf_file_data *conf_data )
{
    char *fname ;
    char  conf[1024];
    FILE *fd;
    int   elto ;
    char  tok[1024] ;
    char  key[1024] ;
    char  value[1024] ;
    int   current_partition ;
    int   current_server ;

    // initial values
    conf_data->lines_n     = 1L;
    conf_data->partition_n = 0;
    conf_data->partitions  = NULL ;

    // get file name
    fname = getenv("XPN_CONF") ;
    if (fname != NULL)
         strcpy(conf, fname);
    else strcpy(conf, XPN_CONF_DEFAULT);

    // open file
    fd = fopen(conf, "r");
    if (NULL == fd)
    {
        fprintf(stderr, "XpnLoadConf: Can't open %s\n", conf) ;
        goto cleanup_error_XpnConfLoad;
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
	  fprintf(stderr, "XpnLoadConf: malloc for '%s' fails\n", conf) ;
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
          printf("%ld: %s = %s\n", conf_data->lines_n, key, value) ;  // DEBUG

          // server_url = sck_server://localhost/tmp/work/data
          if (strcmp(key, XPN_CONF_TAG_SERVER_URL) == 0)
          {
              conf_data->partitions[current_partition].server_n++;
              current_server = conf_data->partitions[current_partition].server_n - 1 ;

	                  conf_data->partitions[current_partition].servers = realloc(conf_data->partitions[current_partition].servers, (conf_data->partitions[current_partition].server_n)*sizeof(char *)) ;
	      if (NULL == conf_data->partitions[current_partition].servers)
	      {
	  	  fprintf(stderr, "XpnLoadConf: malloc for '%s' fails\n", conf) ;
                  goto cleanup_error_XpnConfLoad;
	      }

	                  conf_data->partitions[current_partition].servers[current_server] = strdup(value) ;
	      if (NULL == conf_data->partitions[current_partition].servers[current_server])
	      {
		  fprintf(stderr, "XpnLoadConf: malloc for '%s' fails\n", conf) ;
                  goto cleanup_error_XpnConfLoad;
	      }
          }
          // bsize = 512k
	  else if (strcasecmp(key, XPN_CONF_TAG_BLOCKSIZE) == 0)
          {
	      conf_data->partitions[current_partition].bsize = atoi(value) ;
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
	      fprintf(stderr, "XpnLoadConf: unknown key '%s'\n", key) ;
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
    XpnConfFree(conf_data);
    return -1;
}


struct conf_file_data conf_data ;

int main ( int argc, char *argv[] )
{
    XpnConfLoad(&conf_data) ;
    XpnConfFree(&conf_data) ;

    return 0 ;
}

