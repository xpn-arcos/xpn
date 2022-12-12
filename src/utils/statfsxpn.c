
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


int main(int argc, char *argv[])
{
  char *destino;
  int fdp,fd;

  // Arguments
  if(argc !=2){
    printf("Incorrect number of parameters. Usage \"mkdirxpn <path>\"\n");
    exit(0);
  }
  destino=argv[1];

  // XPN
  if((fd=xpn_init())<0){
    printf("Error in init %d\n",fd);
    exit(-1);
  }
  
  fdp = xpn_mkdir(destino,777); 
	  
  if(fdp<0){
    printf("error in mkdir fdp = %d\n",fdp);
    exit(-1);
  } 

  xpn_destroy();
  exit(0);
}

