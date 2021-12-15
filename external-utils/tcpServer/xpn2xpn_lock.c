#include <sys/param.h>
#include <stdio.h>
#include <sys/file.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/errno.h>
#include "expand.h"

#ifndef KB
#define KB 1024
#endif

#ifndef KB
#define MB (KB*KB)
#endif

#define DATAM (256*KB)

#define DEFAULT_PATH "/tmp"

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

int tcplock(char *file){
	char new_file[255];
	int fd;

	
	generateName(file, new_file);
	fd = open(new_file, O_CREAT|O_TRUNC|O_RDWR, 0777);
	if(fd == -1){
		return -1;
	}
 	flock(fd, LOCK_EX);

	return fd;
}


int tcpunlock(int fd){
  	flock(fd, LOCK_UN);
	close(fd);
	
	return 0;
}


int main(int argc, char *argv[])
{
  
  char *origen,*destino;
  int fdp,fd,s,sp, ret,fd_lock;
  char buffer[DATAM];
  struct stat st, st_xpn; 

  if(argc !=3){
    printf("Incorrect number of parameters.\n");
    printf("Usage: %s <vpath source> <vpath destination>\n", argv[0]);
    exit(0);
  }

  origen=argv[1];
  destino=argv[2];


  fd_lock = tcplock(origen);
  if(fd_lock == -1){
	printf("Err: lock %s\n",destino);
	exit(-1);	  
  }

  
  if((fd=xpn_init())<0){
    tcpunlock(fd_lock);
    xpn_destroy();	  
    printf("Error in init %d\n",fd);
    exit(-1);
  }


  
  ret = xpn_stat(origen, &st);
  if (ret == -1){
    tcpunlock(fd_lock);
    xpn_destroy();
    printf("error in xpn_stat(%s)\n",destino);
    exit(-1);
  }

  ret = xpn_stat(destino, &st_xpn);
  if (ret != -1){
 	 if(st.st_size == st_xpn.st_size){
	    tcpunlock(fd_lock);
	    xpn_destroy();	  
	    printf("%s and %s are the same file\n", origen, destino);
	    exit(0);	    
	  }
  }



  fd = xpn_open(origen, O_RDONLY);
  if(fd<0){
    tcpunlock(fd_lock);
    xpn_destroy();	  
    printf("error in open fd = %d\n",fd);
    exit(-1);
  }  
  

  
  fdp = xpn_open(destino ,O_CREAT|O_TRUNC|O_WRONLY, 0777); 
  if(fdp<0){
    tcpunlock(fd_lock);
    xpn_destroy();	  
    printf("error in xpn_open fdp = %d\n",fdp);
    exit(-1);
  } 

 
  

  //sum = 0;
  do{

    sp = xpn_read(fd, buffer, DATAM);
    //printf("antes de xpn_write(%d bytes) ...\n", s);
    if(s == -1){
	    break;
    }
    s = xpn_write(fdp, buffer, sp);
    //printf("despues de xpn_write ...\n");
    //sum = sum + sp;

    //printf("Se han leido s=%d y escrito sp=%d\n", s, sp);
  }while((s==DATAM)&&(sp >= 0));

  xpn_close(fd);
  xpn_close(fdp);
  tcpunlock(fd_lock);
  xpn_destroy();

  exit(0);
}
