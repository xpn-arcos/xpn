
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
    #include "tcp_server_params.h"
    #include "tcp_server_d2xpn.h"

    #include "xpn.h"
    #include <sys/time.h>


   /* ... Constants / Constantes ........................................ */

    #define DEFAULT_PATH "/tmp"
    #define TCP_SERVER_PATH_DEFAULT "/tmp"


   /* ... Global variables / Variables globales ......................... */

    pthread_mutex_t mutex_id = PTHREAD_MUTEX_INITIALIZER;
    int static_id = 0;


   /* ... Functions / Funciones ......................................... */

    void generateName ( char *file, char *new_file )
    {
       char aux[255];
       unsigned long i;
          long j;
    
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
    
       sprintf(new_file,"%s/%s", DEFAULT_PATH, aux);
       return;
    }

    int mylock ( char *file )
    {
       int fd;

       debug_info("d2xpn: mylock(%s)\n", file);
       if (NULL == file) {
         return -1 ;
       }
    
       // lock with pthread...
       fd = 0;
       pthread_mutex_lock(&mutex_id);

       debug_info("d2xpn: mylock(%s) -> %d\n", file, fd);

       return fd;
    }

    int myunlock ( int fd )
    {
       debug_info("d2xpn: myunlock(%d)\n", fd);
       if (fd < 0) {
         return -1 ;
       }

       // unlock with pthread...
       pthread_mutex_unlock(&mutex_id);

       return 0;
    }

    int tcp_server_d2xpn ( tcp_server_param_st *params, char *origen, char *destino )
    {
       struct stat st;
       int fdp,fd,ret,fd_lock;
       char *tcp_server_path, new_path[2*PATH_MAX];


       debug_info("d2xpn(%d): Origen: %s\n",  params->rank, origen) ;
       debug_info("d2xpn(%d): Destino: %s\n", params->rank, destino) ;

       tcp_server_path = params->dirbase;
       if (tcp_server_path  == NULL) {
         tcp_server_path = TCP_SERVER_PATH_DEFAULT;
       }

       sprintf(new_path, "%s/%s", tcp_server_path, destino);

       /*
      * Deberia comprobar si puedo hacer el lock,
      * si no devolver resultado para que lea del
      *
      */

       /*
    * TODO
       double transfer_time;
       struct timeval t1, t2;
        
    gettimeofday(&t1, NULL);
    */
       fd_lock = mylock(origen);
       if (fd_lock < 0) {
          printf("Err: lock %s\n", destino);
          perror("Error: lock");
          return(-1); 
       }

       debug_info("d2xpn(%d): xpn_stat(%s)\n", params->rank, origen);

       ret = stat(new_path, &st);
       debug_info("d2xpn(%d): stat(%s) = %d\n", params->rank, new_path, ret);
       if (0 ==  ret)
       {
          debug_info("d2xpn(%d): %s (%s) is stored in cache\n", params->rank, destino, origen);
          myunlock(fd_lock);
          return(0);  
       }

       debug_info("d2xpn(%d): xpn_init()\n",params->rank);
       if ((fd=xpn_init())<0){
           myunlock(fd_lock);
           printf("Error in init %d\n",fd);
           return(-1);
       }

       debug_info("d2xpn(%d): open(%s, O_RDONLY)\n",params->rank, destino);
       fd=open(destino,O_RDONLY);
       if (fd<0){
          myunlock(fd_lock);
          //xpn_destroy();  
          printf("tcp_server_d2xpn: error in open(%s) fd (%d)\n",destino,fd);
          return(-1);
       }

       debug_info("d2xpn(%d): begin xpn_open(%s, O_CREAT|O_TRUNC|O_WRONLY, 0777)\n",params->rank, origen);
       fdp = xpn_open(origen,O_CREAT|O_TRUNC|O_WRONLY, 0777);
       debug_info("d2xpn(%d): end xpn_open(%s, O_CREAT|O_TRUNC|O_WRONLY, 0777) = %d\n",params->rank, origen, fdp);
       if (fdp<0){
          myunlock(fd_lock);
          //xpn_destroy();  
          printf("error in xpn_open fdp = %d\n",fdp);
          return(-1);
       }

       //TODO
       /*
       int s,sp;
       int sum = 0;
       char *global_transfer_buffer;
     global_transfer_buffer = malloc(sizeof(char)*(params->IOsize * KB));
       sum = 0;
       do {
      debug_info("d2xpn(%d): before read(%d,%d)\n", params->rank, params->IOsize * KB, sum);
      sp = read(fd,global_transfer_buffer,params->IOsize * KB);
      debug_info("d2xpn(%d): desp. read(%d,%d)\n", params->rank, sp, sum);
      if (s < 0) {
        break;
      }
    
      debug_info("d2xpn(%d): antes write(%d,%d)\n", params->rank, sp , sum);
      s = xpn_write(fdp, global_transfer_buffer, sp);
      debug_info("d2xpn(%d): desp write(%d,%d)\n", params->rank, s, sum);
    
      sum = sum + sp;
    
      //printf("Se han leido s=%d y escrito sp=%d\n", s, sp);
       } while ((s==(params->IOsize * KB))&&(sp >= 0));
       free(global_transfer_buffer);

       debug_info("d2xpn(%d): (%s,%d)\n", params->rank, origen, sum);

       debug_info("d2xpn(%d): close()\n",params->rank);
       close(fd);

       debug_info("d2xpn(%d): xpn_close()\n",params->rank);
       xpn_close(fdp);

       debug_info("d2xpn(%d): move %s -> %s\n", params->rank, destino, origen);
       myunlock(fd_lock);
       debug_info("d2xpn(%d): xpn_destroy()\n", params->rank);

       gettimeofday(&t2, NULL);
       transfer_time = (t2.tv_sec + t2.tv_usec/1000000.0) - (t1.tv_sec + t1.tv_usec/1000000.0);
       printf("Name\t%s\tTransfer_time\t%f\tSize\t%d\n", origen, transfer_time, (params->IOsize * KB));
     */

       return(0);
    }


   /* ................................................................... */
