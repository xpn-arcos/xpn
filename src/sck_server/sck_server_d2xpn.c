
/*
 *  Copyright 2020-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos
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

#include "all_system.h"
#include "sck_server_d2xpn.h"
#include "sck_server_params.h"

#include "xpn.h"
#include <sys/time.h>


/* ... Constants / Constantes ........................................ */


/* ... Global variables / Variables globales ......................... */

 pthread_mutex_t mutex_id  = PTHREAD_MUTEX_INITIALIZER;
 int             static_id = 0;


/* ... Functions / Funciones ......................................... */

 void sck_server_d2xpn_generate_name ( char *file, char *new_file )
 {
    char          aux[255];
    unsigned long i;
    long          j;

    debug_info("[SCK_SERVER_D2XPN] [sck_server_d2xpn_generate_name] >> Begin\n");
 
    for (j=0,i=0; i<strlen(file); i++)
    {
      switch(file[i])
      {
        case '/':
          aux[j] = '_';       
          break;
        case '_':               
          aux[j] = '_';
          j++;
          aux[j] = '_';       
          break;              
        default:
          aux[j] = file[i];
          break;
      }
      j++;
    }
    aux[j] = '\0';
 
    sprintf(new_file,"%s/%s", SCK_SERVER_DIRBASE_DEFAULT, aux);

    debug_info("[SCK_SERVER_D2XPN] [sck_server_d2xpn_generate_name] >> End\n");

    return;
 }

 int sck_server_d2xpn_lock ( char *file )
 {
    int fd;

    debug_info("[SCK_SERVER_D2XPN] [sck_server_d2xpn_lock] >> Begin\n");

    if (NULL == file)
    {
      printf("[SCK_SERVER_D2XPN] [sck_server_d2xpn_lock] ERROR: invalid file\n");
      return -1 ;
    }
 
    // lock with pthread...
    fd = 0;
    pthread_mutex_lock(&mutex_id);

    debug_info("[SCK_SERVER_D2XPN] [sck_server_d2xpn_lock] >> End: fd: %d\n", fd);

    return fd;
 }

 int sck_server_d2xpn_unlock ( int fd )
 {
    debug_info("[SCK_SERVER_D2XPN] [sck_server_d2xpn_unlock] >> Begin: fd: %d\n", fd);

    if (fd < 0)
    {
      printf("[SCK_SERVER_D2XPN] [sck_server_d2xpn_unlock] ERROR: invalid fd %d\n", fd);
      return -1 ;
    }

    // unlock with pthread...
    pthread_mutex_unlock(&mutex_id);

    debug_info("[SCK_SERVER_D2XPN] [sck_server_d2xpn_unlock] >> End\n");

    return 0;
 }

 int sck_server_d2xpn ( sck_server_param_st *params, char *src, char *dst )
 {
    struct stat st;
    int fdp,fd,ret,fd_lock;
    char *sck_server_path, new_path[2*PATH_MAX];

    debug_info("[SCK_SERVER_D2XPN] [sck_server_d2xpn] >> Begin: src: %s; dst %s; rank %d\n", src, dst, params->rank);

    // Get server path
    debug_info("[SCK_SERVER_D2XPN] [sck_server_d2xpn] Get server path\n");

    sck_server_path = params->dirbase;
    if (sck_server_path  == NULL) {
      sck_server_path = SCK_SERVER_DIRBASE_DEFAULT;
    }

    sprintf(new_path, "%s/%s", sck_server_path, dst);

    debug_info("[SCK_SERVER_D2XPN] [sck_server_d2xpn] Destination file server path\n");

    /*
     * I should check if I can do the lock, if not return result to read from the
     */

    //TODO
    /*
    double transfer_time;
    struct timeval t1, t2;
    gettimeofday(&t1, NULL);
    */

    debug_info("[SCK_SERVER_D2XPN] [sck_server_d2xpn] Lock src file\n");

    fd_lock = sck_server_d2xpn_lock(src);
    if (fd_lock == -1)
    {
      printf("[SCK_SERVER_D2XPN] [sck_server_d2xpn] ERROR: Lock source file fails\n");
      return -1;  
    }

    debug_info("[SCK_SERVER_D2XPN] [sck_server_d2xpn] Stat of destination file\n");

    ret = stat(new_path, &st);
    if (0 ==  ret)
    {
      debug_info("[SCK_SERVER_D2XPN] [sck_server_d2xpn] Destination file is stored in cache\n");
      sck_server_d2xpn_unlock(fd_lock);
      return 0;
    }

    // XPN Initialization
    debug_info("[SCK_SERVER_D2XPN] [sck_server_d2xpn] xpn initialization\n");

    if ( (fd = xpn_init()) < 0 )
    {
      sck_server_d2xpn_unlock(fd_lock);
      printf("[SCK_SERVER_D2XPN] [sck_server_d2xpn] ERROR: xpn_init fails\n");
      return -1;
    }

    debug_info("[SCK_SERVER_D2XPN] [sck_server_d2xpn] Source file open\n");

    fd = open(src,O_RDONLY);
    if (fd < 0)
    {
      sck_server_d2xpn_unlock(fd_lock);
      printf("[SCK_SERVER_D2XPN] [sck_server_d2xpn] ERROR: Source file open fails\n");
      //xpn_destroy();    
      return -1;
    }

    debug_info("[SCK_SERVER_D2XPN] [sck_server_d2xpn] Destination file open\n");

    fdp = xpn_open(new_path,O_CREAT|O_TRUNC|O_WRONLY, 0777);
    if (fdp < 0)
    {
      sck_server_d2xpn_unlock(fd_lock);
      printf("[SCK_SERVER_D2XPN] [sck_server_d2xpn] ERROR: Source file open fails\n");
      //xpn_destroy();    
      return -1;
    }

    //TODO
    /*
    int s,sp;
    int sum = 0;
    char *global_transfer_buffer;

    global_transfer_buffer = malloc(sizeof(char)*(params->IOsize * KB));
    sum = 0;

    do
    {
      debug_info("[SCK_SERVER_D2XPN] [sck_server_d2xpn] Source file read\n");

      sp = read(fd,global_transfer_buffer,params->IOsize * KB);
      if (s == -1) {
        break;
      }
 
      debug_info("[SCK_SERVER_D2XPN] [sck_server_d2xpn] Destination file write\n");
      s = xpn_write(fdp, global_transfer_buffer, sp);
 
      sum = sum + sp;
 
    } while ((s==(params->IOsize * KB))&&(sp >= 0));

    free(global_transfer_buffer);

    debug_info("[SCK_SERVER_D2XPN] [sck_server_d2xpn] Source file close\n");

    close(fd);

    debug_info("[SCK_SERVER_D2XPN] [sck_server_d2xpn] Destination file open\n");

    xpn_close(fdp);

    debug_info("[SCK_SERVER_D2XPN] [sck_server_d2xpn] Unlock src file\n");

    sck_server_d2xpn_unlock(fd_lock);

    gettimeofday(&t2, NULL);
    transfer_time = (t2.tv_sec + t2.tv_usec/1000000.0) - (t1.tv_sec + t1.tv_usec/1000000.0);

    printf("Name\t%s\tTransfer_time\t%f\tSize\t%d\n", origen, transfer_time, (params->IOsize * KB));
    */

    return 0;
}


/* ................................................................... */
