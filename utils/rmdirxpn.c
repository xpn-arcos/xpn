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




int main(int argc, char *argv[])
{
  
  char *destino;
  int fdp,fd;
  if(argc !=2){
    printf("Incorrect number of parameters. Usage \"rmdirxpn <path>\"\n");
    exit(0);
  }
  destino=argv[1];

  
  if((fd=xpn_init())<0){
    printf("Error in init %d\n",fd);
    exit(-1);
  }
  
  fdp = xpn_rmdir(destino); 
	  
  if(fdp<0){
    printf("error in rmdir fdp = %d\n",fdp);
    exit(-1);
  } 
  else printf("Rmdir successful\n");

  xpn_destroy();
  exit(0);
}
