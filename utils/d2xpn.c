#include <stdlib.h>
#include <sys/param.h>
#include <stdio.h>
#include <sys/file.h>
#include <sys/fcntl.h>
//#include <sys/stat.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/errno.h>
#include "xpn.h"

#ifndef KB
	#define KB 1024
#endif

#ifndef MB
	#define MB (KB*KB)
#endif
#define DATAM (256*KB)


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
  
  if((fd=xpn_init())<0){
    printf("Error in init %d\n",fd);
    exit(-1);
  }
  
  //printf("antes de open \n");	  
  fdp = xpn_open(destino,O_TRUNC|O_CREAT|O_RDWR,0777); 
  //fdp = xpn_open(destino,O_CREAT|O_RDWR,0777); 
  //printf("desp de open %d\n",fdp);	  
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

  //printf("antes de read....\n");
    s=read(fd,buffer,DATAM);
  //printf("leidos %d bytes\n", s);
  //printf("antes de write....\n");
    sp=xpn_write(fdp,buffer,s);
  //printf("escritos %d bytes\n", sp);
  printf(".");
    sum = sum +s;

  }while((s==DATAM)&&(sp >= 0));
  printf("\n");
  printf("total leidos %d bytes\n", sum);

  close(fd);
  xpn_close(fdp);
  xpn_destroy();

  exit(0);
}
