
#include <stdlib.h>
#include <sys/param.h>
#include <stdio.h>
#include <sys/file.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/errno.h>
#include "xpn.h"

#define DATAM (64*KB)

#ifndef KB
	#define KB	(1024)
#endif

int main(int argc, char *argv[])
{

  char *origen,*destino;
  int fdp,fd,s,sp,sum;
  char buffer[DATAM];

  // Arguments
  if(argc !=3){
    printf("Incorrect number of parameters\n");
    exit(0);
  }
  
  origen=argv[1];
  destino=argv[2];
 
  // XPN
  if((fd=xpn_init())<0){
    printf("Error in init %d\n",fd);
    exit(-1);
  }

  fd=creat(destino,0777);
  if(fd<0){
    printf("error in create %d\n",fd);
    exit(-1);
  }
    
  fd=open(destino,O_RDWR);
  if(fd<0){
    printf("error in open %d\n",fd);
    exit(-1);
  }

  fdp=xpn_open(origen,O_RDONLY);
  if(fdp<0){
    printf("error in xpn_open source = %s -> %d \n",origen,fdp);
    exit(-1);
  }
  sum = 0;
  do{
    sp=xpn_read(fdp,buffer,DATAM);
    if(sp<0){
      printf("Error in read %d\n",sp);
      exit(-1);
    }
    
    s=write(fd,buffer,sp);
    if(s<0){
      printf("Error in write %d\n",s);
      exit(-1);
    }
    
    sum = sum +sp;
  }while(sp==DATAM);

  close(fd);
  xpn_close(fdp);
  xpn_destroy();
  exit(0);
}

