#include "mpiServer_d2xpn.h"
#include "mpiServer_params.h"
#include "xpn.h"
#include <sys/time.h>

#ifndef KB
#define KB 1024
#endif

#ifndef KB
#define MB (KB*KB)
#endif

int MPISERVER_IOSIZE_INT;

#define DEFAULT_PATH "/tmp"
#define MPISERVER_PATH_DEFAULT "/tmp"


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

int mylock(char *file)
{
	//char new_file[255];
	int fd;

  	debug_info("d2xpn: mylock(%s)\n",file);
	
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
  	debug_info("d2xpn: mylock(%s) -> %d\n",file,fd);

	return fd;
}

int myunlock ( int fd )
{
  	debug_info("d2xpn: myunlock(%d)\n",fd);
	pthread_mutex_unlock(&mutex_id);
/*
  	flock(fd, LOCK_UN);
	close(fd);
*/	
	return 0;
}

int mpiServer_d2xpn ( mpiServer_param_st *params, char *origen, char *destino, int opt )
{
        struct stat st;
        //struct stat st_xpn;
        //char s_exe [255];
        int fdp,fd,s,sp, ret,fd_lock;
        int sum = 0;
        char *mpiServer_path, new_path[255];
        //int private_id; 
        char *global_transfer_buffer;
      
        double transfer_time;  
        struct timeval t1, t2;
      	//pthread_mutex_lock(&mutex_id);
              //private_id = static_id++;
      	//pthread_mutex_unlock(&mutex_id);
      	
      
      	debug_info("d2xpn(%d): Origen: %s\n",  private_id, origen);
      	debug_info("d2xpn(%d): Destino: %s\n", private_id, destino);
      
      mpiServer_path = params->dirbase;
      if(mpiServer_path  == NULL){
      	mpiServer_path = MPISERVER_PATH_DEFAULT; 
      }
      
        sprintf(new_path, "%s/%s", mpiServer_path, destino); 
      
      /*
       * Deberia comprobar si puedo hacer el lock,
       * si no devolver resultado para que lea del 
       * 
       */
      
        gettimeofday(&t1, NULL);
        fd_lock = mylock(origen);
        if (fd_lock == -1) {
      	    printf("Err: lock %s\n", destino);
      	    perror("Error: lock");
      	    return(-1);	  
        }
      
      	debug_info("d2xpn(%d): xpn_stat(%s)\n", private_id, origen);
       
        ret = stat(new_path, &st);
        debug_info("d2xpn(%d): stat(%s) = %d\n", private_id, new_path, ret);
        if (0 ==  ret)
        {
      	    debug_info("d2xpn(%d): %s (%s) is stored in cache\n", private_id, destino, origen);
      	    myunlock(fd_lock);
      	    return(0);	    
        }
      
      	debug_info("d2xpn(%d): xpn_init()\n",private_id);
        if ((fd=xpn_init())<0){
             myunlock(fd_lock);
             printf("Error in init %d\n",fd);
             return(-1);
        }
      
      	debug_info("d2xpn(%d): open(%s, O_RDONLY)\n",private_id, destino);
        fd=open(destino,O_RDONLY);
        if (fd<0){
            myunlock(fd_lock);
            //xpn_destroy();	  
            printf("mpiServer_d2xpn: error in open(%s) fd (%d)\n",destino,fd);
            return(-1);
        }  
        
      	debug_info("d2xpn(%d): begin xpn_open(%s, O_CREAT|O_TRUNC|O_WRONLY, 0777)\n",private_id, origen);
        fdp = xpn_open(origen,O_CREAT|O_TRUNC|O_WRONLY, 0777); 
      	debug_info("d2xpn(%d): end xpn_open(%s, O_CREAT|O_TRUNC|O_WRONLY, 0777) = %d\n",private_id, origen, fdp);
        if (fdp<0){
            myunlock(fd_lock);
            //xpn_destroy();	  
            printf("error in xpn_open fdp = %d\n",fdp);
            return(-1);
        } 
      
        global_transfer_buffer = malloc(sizeof(char)*MPISERVER_IOSIZE_INT);
        sum = 0;
        do {
		debug_info("d2xpn(%d): before read(%d,%d)\n", private_id,MPISERVER_IOSIZE_INT, sum);
		sp = read(fd,global_transfer_buffer,MPISERVER_IOSIZE_INT);
		debug_info("d2xpn(%d): desp. read(%d,%d)\n", private_id, sp, sum);
		if (s == -1) {
		    break;
		}
	      
		debug_info("d2xpn(%d): antes write(%d,%d)\n", private_id, sp , sum);
		s = xpn_write(fdp, global_transfer_buffer, sp);
		debug_info("d2xpn(%d): desp write(%d,%d)\n", private_id, s, sum);
	      
		sum = sum + sp;
	      
		//printf("Se han leido s=%d y escrito sp=%d\n", s, sp);
        } while ((s==MPISERVER_IOSIZE_INT)&&(sp >= 0));
        free(global_transfer_buffer);
      
        debug_info("d2xpn(%d): (%s,%d)\n", private_id, origen, sum);
      
        debug_info("d2xpn(%d): close()\n",private_id);
        close(fd);
      
        debug_info("d2xpn(%d): xpn_close()\n",private_id);
        xpn_close(fdp);
      
        debug_info("d2xpn(%d): move %s -> %s\n", private_id, destino, origen);
        myunlock(fd_lock);
        debug_info("d2xpn(%d): xpn_destroy()\n", private_id);
      
        gettimeofday(&t2, NULL);
        transfer_time = (t2.tv_sec + t2.tv_usec/1000000.0) - (t1.tv_sec + t1.tv_usec/1000000.0);
        printf("Name\t%s\tTransfer_time\t%f\tSize\t%d\n", origen, transfer_time, MPISERVER_IOSIZE_INT);
      
        return(0);
}

