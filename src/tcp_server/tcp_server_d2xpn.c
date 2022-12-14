
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


   /* ... Include / Inclusion ........................................... */

#include "tcp_server_d2xpn.h"
#include "tcp_server_params.h"
#include "xpn.h"
#include <sys/time.h>

#ifndef KB
#define KB 1024
#endif

#ifndef KB
#define MB (KB*KB)
#endif

#define DEFAULT_PATH "/tmp"
#define TCP_SERVER_PATH_DEFAULT "/tmp"


   /* ... Global variables / Variables globales ......................... */

int TCP_SERVER_IOSIZE_INT;
extern struct tcp_server_param_st tcp_server_params;
pthread_mutex_t mutex_id = PTHREAD_MUTEX_INITIALIZER;
int static_id = 0;


   /* ... Functions / Funciones ......................................... */

void generateName(char *file, char *new_file)
{
	long unsigned i, j;
	char aux[255];
	
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
	//char new_file[255];
	int fd;

#if defined (DBG_XPN) || defined(_DBGXPN_)
  	printf("d2xpn: mylock(%s)\n", file);
#endif

	// check params...
	if (NULL == file) {
	    return -1 ;
	}
	
	pthread_mutex_lock(&mutex_id);
	fd = 0;
/*
	generateName(file, new_file);
	fd = open(new_file, O_CREAT|O_TRUNC|O_RDWR, 0777);
	if(fd == -1){
		return -1;
	}
 	flock(fd, LOCK_EX);
*/
#if defined (DBG_XPN) || defined(_DBGXPN_)
  	printf("d2xpn: mylock(%s) -> %d\n",file,fd);
#endif

	return fd;
}

int myunlock ( int fd )
{
#if defined (DBG_XPN) || defined(_DBGXPN_)
  	printf("d2xpn: myunlock(%d)\n",fd);
#endif

	// check params...
	if (fd < 0)  {
	    return -1 ;
	}

	pthread_mutex_unlock(&mutex_id);
/*
  	flock(fd, LOCK_UN);
	close(fd);
*/	

	return 0;
}

int tcp_server_d2xpn ( char *origen, char *destino, int opt )
{
  struct stat st;
  //struct stat st_xpn;
  //char s_exe [255];
  int fdp,fd,s,sp, ret,fd_lock;
  int sum = 0;
  char *tcp_server_path, new_path[255];
#ifdef DBG_XPN
  int private_id;
#endif
  char *global_transfer_buffer;

  double transfer_time;
  struct timeval t1, t2;


  if (opt < 0) {
#ifdef DBG_XPN
      printf("d2xpn(%d): warning opt is %d\n",  private_id, opt);
#endif
  }

	//pthread_mutex_lock(&mutex_id);
#ifdef DBG_XPN
        private_id = static_id++;
#endif
	//pthread_mutex_unlock(&mutex_id);
	

#ifdef DBG_XPN
	printf("d2xpn(%d): Origen: %s\n",  private_id, origen);
	printf("d2xpn(%d): Destino: %s\n", private_id, destino);
	fflush(stdout);
#endif

tcp_server_path = tcp_server_params.dirbase;
if(tcp_server_path  == NULL){
	tcp_server_path = TCP_SERVER_PATH_DEFAULT;
}

  sprintf(new_path, "%s/%s", tcp_server_path, destino);

/*
 * Deberia comprobar si puedo hacer el lock,
 * si no devolver resultado para que lea del
 *
 */

  gettimeofday(&t1, NULL);
  fd_lock = mylock(origen);
  if(fd_lock == -1){
	printf("Err: lock %s\n",destino);
	perror("Error: lock");
	return(-1);	
  }

//sprintf(s_exe,"ls -l %s",new_path);
//system(s_exe);


#ifdef DBG_XPN
	printf("d2xpn(%d): xpn_stat(%s)\n",private_id,origen);
	fflush(stdout);
#endif


  ret = stat(new_path, &st);
#if defined( DBG_XPN) || defined(_DBGXPN_)
  printf("d2xpn(%d): stat(%s) = %d\n", private_id,new_path, ret);
#endif
  if (0 ==  ret){
//sprintf(s_exe,"ls -l %s",new_path);
//system(s_exe);
#if defined( DBG_XPN) || defined(_DBGXPN_)
	    printf("d2xpn(%d): %s (%s) is stored in cache\n", private_id, destino, origen);
#endif
	    myunlock(fd_lock);
	    //xpn_destroy();	
	    //printf("d2xpn: %s and %s are the same file\n", origen, destino);
	    return(0);	
  }



#ifdef DBG_XPN
	printf("d2xpn(%d): xpn_init()\n",private_id);
	fflush(stdout);
#endif
  if((fd=xpn_init())<0){
    myunlock(fd_lock);
    printf("Error in init %d\n",fd);
    return(-1);
  }



#ifdef DBG_XPN
	printf("d2xpn(%d): open(%s, O_RDONLY)\n",private_id,destino);
	fflush(stdout);
#endif
  fd=open(destino,O_RDONLY);
  if(fd<0){
    myunlock(fd_lock);
    //xpn_destroy();	
    printf("tcp_server_d2xpn: error in open(%s) fd (%d)\n",destino,fd);
    return(-1);
  }



#ifdef DBG_XPN
	printf("d2xpn(%d): begin xpn_open(%s, O_CREAT|O_TRUNC|O_WRONLY, 0777)\n",private_id,origen);
#endif
  fdp = xpn_open(origen,O_CREAT|O_TRUNC|O_WRONLY, 0777);
#ifdef DBG_XPN
	printf("d2xpn(%d): end xpn_open(%s, O_CREAT|O_TRUNC|O_WRONLY, 0777) = %d\n",private_id,origen,fdp);
#endif
  if(fdp<0){
    myunlock(fd_lock);
    //xpn_destroy();	
    printf("error in xpn_open fdp = %d\n",fdp);
    return(-1);
  }




  global_transfer_buffer = malloc(sizeof(char)*TCP_SERVER_IOSIZE_INT);
  sum = 0;
  do{

#ifdef DBG_XPN
	printf("d2xpn(%d): antes read(%d,%d)\n", private_id,TCP_SERVER_IOSIZE_INT, sum);
#endif
    sp = read(fd,global_transfer_buffer,TCP_SERVER_IOSIZE_INT);
    //printf("antes de xpn_write(%d bytes) ...\n", s);
    if(sp == -1){
	    break;
    }
#ifdef DBG_XPN
	printf("d2xpn(%d): desp. read(%d,%d)\n", private_id, sp, sum);
#endif

#ifdef DBG_XPN
	printf("d2xpn(%d): antes write(%d,%d)\n", private_id, sp , sum);
#endif
    s = xpn_write(fdp, global_transfer_buffer, sp);
#ifdef DBG_XPN
	printf("d2xpn(%d): desp write(%d,%d)\n", private_id, s, sum);
#endif
    //printf("despues de xpn_write ...\n");
    sum = sum + sp;

    //printf("Se han leido s=%d y escrito sp=%d\n", s, sp);
  }while((s==TCP_SERVER_IOSIZE_INT)&&(sp >= 0));
  free(global_transfer_buffer);

#ifdef DBG_XPN
	printf("d2xpn(%d): (%s,%d)\n", private_id, origen, sum);
#endif

#ifdef DBG_XPN
	printf("d2xpn(%d): close()\n",private_id);
#endif
  close(fd);
#ifdef DBG_XPN
	printf("d2xpn(%d): xpn_close()\n",private_id);
#endif
  xpn_close(fdp);

//sprintf(s_exe,"ls -l %s",new_path);
//system(s_exe);


/***************************************************************
  ret = stat(destino, &st);
  if (ret != 0){
        printf("error in stat(%s)\n",destino);
  }else{

          ret = xpn_stat(origen, &st_xpn);
        if (ret == 0){

                 if(st.st_size == st_xpn.st_size){
                        printf("%s and %s are the same file(%d)\n", origen, destino, (int)st.st_size/KB);
                }else{
                        printf("%s and %s are diferent (%d, %d)\n", origen, destino, (int)st_xpn.st_size/KB,(int)st.st_size/KB);
                }
        }else{
                printf("error in stat(%s)\n",destino);
        }
  }
***************************************************************/
#if defined (DBG_XPN) || defined(_DBGXPN_)
	printf("d2xpn(%d): move %s -> %s\n", private_id, destino, origen);
#endif
  myunlock(fd_lock);
#ifdef DBG_XPN
	printf("d2xpn(%d): xpn_destroy()\n", private_id);
#endif
  //xpn_destroy();

  gettimeofday(&t2, NULL);

  transfer_time = (t2.tv_sec + t2.tv_usec/1000000.0) - (t1.tv_sec + t1.tv_usec/1000000.0);

  printf("Name\t%s\tTransfer_time\t%f\tSize\t%d\n", origen, transfer_time, TCP_SERVER_IOSIZE_INT);


/*
  ret = stat(destino, &st);
  if (ret != 0){
    	printf("error in stat(%s)\n",destino);
  }else{

	  ret = xpn_stat(origen, &st_xpn);
  	if (ret == 0){

        	 if(st.st_size == st_xpn.st_size){
            		printf("%s and %s are the same file(%d)\n", origen, destino, (int)st.st_size/KB);
          	}else{
            		printf("%s and %s are diferent (%d, %d)\n", origen, destino, (int)st_xpn.st_size/KB,(int)st.st_size/KB);
   		}
  	}else{
    		printf("error in stat(%s)\n",destino);
	}
  }
*/
  return(0);
}

