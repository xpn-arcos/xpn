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
#ifndef MB
#define MB (KB*KB)
#endif
#define DATAM (64*KB)

int main(int argc, char *argv[])
{

  char *origen,*destino;
  int fd;
  if(argc !=3){
    printf("Incorrect number of parameters\n");
    exit(0);
  }
  
  origen=argv[1];
  destino=argv[2];

 
  /***********************/

  if((fd=xpn_init())<0){
    printf("Error in init %d\n",fd);
    exit(-1);
  }

  if(xpn_preload(destino, origen) != 0){
  	printf("Error en el xpn_preload()\n");
  }else{
 	printf("Ok...\n");
  }
  
  xpn_destroy();
  exit(0);
}

