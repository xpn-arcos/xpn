#include "tcpServer_d2xpn.h"
#include "tcpServer_params.h"
#include "xpn.h"
#include <sys/time.h>

#ifndef KB
#define KB 1024
#endif

#ifndef KB
#define MB (KB*KB)
#endif

int TCPSERVER_IOSIZE_INT;

#define DEFAULT_PATH "/tmp"

#define TCPSERVER_PATH_DEFAULT "/tmp"

extern struct tcpServer_param_st tcpServer_params;


/*****************************************************************/
pthread_mutex_t mutex_id = PTHREAD_MUTEX_INITIALIZER; 
int static_id = 0;
/*****************************************************************/


void generateName(char *file, char *new_file){
	char aux[255];
	int i,j;
	
	for(j=0,i=0;i<strlen(file);i++){
		switch(file[i]){
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

int mylock(char *file){
	//char new_file[255];
	int fd;

#if defined (DBG_XPN) || defined(_DBGXPN_)
  	printf("d2xpn: mylock(%s)\n",file);
#endif
	
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


int myunlock(int fd){
#if defined (DBG_XPN) || defined(_DBGXPN_)
  	printf("d2xpn: myunlock(%d)\n",fd);
#endif
	pthread_mutex_unlock(&mutex_id);
/*
  	flock(fd, LOCK_UN);
	close(fd);
*/	
	return 0;
}

int tcpServer_d2xpn(char *origen, char *destino, int opt)
{
  struct stat st;
  //struct stat st_xpn;
  //char s_exe [255];
  int fdp,fd,s,sp, ret,fd_lock;
  int sum = 0;
  char *tcpServer_path, new_path[255];
  int private_id; 
  char *global_transfer_buffer;

  double transfer_time;  
  struct timeval t1, t2;
	//pthread_mutex_lock(&mutex_id);
        private_id = static_id++;
	//pthread_mutex_unlock(&mutex_id);
	

#ifdef DBG_XPN
	printf("d2xpn(%d): Origen: %s\n", private_id, origen);
	printf("d2xpn(%d): Destino: %s\n", private_id, destino);
	fflush(stdout);
#endif



tcpServer_path = tcpServer_params.dirbase;
if(tcpServer_path  == NULL){
	tcpServer_path = TCPSERVER_PATH_DEFAULT; 
}

  sprintf(new_path, "%s/%s", tcpServer_path, destino); 

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
    printf("tcpServer_d2xpn: error in open(%s) fd (%d)\n",destino,fd);
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

 
  

  global_transfer_buffer = malloc(sizeof(char)*TCPSERVER_IOSIZE_INT);
  sum = 0;
  do{

#ifdef DBG_XPN
	printf("d2xpn(%d): antes read(%d,%d)\n", private_id,TCPSERVER_IOSIZE_INT, sum);
#endif
    sp = read(fd,global_transfer_buffer,TCPSERVER_IOSIZE_INT);
    //printf("antes de xpn_write(%d bytes) ...\n", s);
    if(s == -1){
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
  }while((s==TCPSERVER_IOSIZE_INT)&&(sp >= 0));
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

  printf("Name\t%s\tTransfer_time\t%f\tSize\t%d\n", origen, transfer_time, TCPSERVER_IOSIZE_INT);


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
