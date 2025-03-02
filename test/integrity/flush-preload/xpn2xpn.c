#include <sys/param.h>
#include <stdio.h>
#include <sys/file.h>
#include <sys/fcntl.h>
//#include <sys/stat.h>
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

#define DATAM (64*KB)

int main(int argc, char *argv[])
{
  
  char *origen,*destino;
  int fdp,fd,s,sp,sum;
  char buffer[DATAM];

  if(argc !=3){
    printf("Incorrect number of parameters.\n");
    printf("Usage: %s <origin> <destination>\n",argv[0]);
    exit(0);
  }

  origen=argv[1];
  destino=argv[2];
  //setenv("XPN_CONF", "/export/home/pato11-1/proyectos/xpn/expand-2.0/test/xpn/prueba/", 1);  
  if((fd=xpn_init())<0){
    printf("Error in init %d\n",fd);
    exit(-1);
  }
 
  fdp = xpn_open(destino,O_TRUNC|O_CREAT|O_RDWR,0777); 
	  
  if(fdp<0){
    printf("error in open fdp = %d\n",fdp);
    exit(-1);
  }  
  fd=open(origen,0);
  if(fd<0){
    printf("error in open fd = %d\n",fd);
    exit(-1);
  }  

  sum = 0;
  do{

    s=read(fd,buffer,DATAM);
    //printf("antes de xpn_write(%d bytes) ...\n", s);
    sp=xpn_write(fdp,buffer,s);
    //printf("despues de xpn_write ...\n");
    sum = sum +s;

    //printf("Se han leido s=%d y escrito sp=%d\n", s, sp);
  }while((s==DATAM)&&(sp >= 0));

  close(fd);
  xpn_close(fdp);
  xpn_destroy();

  exit(0);
}
