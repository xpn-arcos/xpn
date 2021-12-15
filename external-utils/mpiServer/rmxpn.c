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




int main(int argc, char *argv[])
{
  
  char *destino;
  int fdp,fd;
  if(argc !=2){
    printf("Incorrect number of parameters. Usage \"removexpn <path>\"\n");
    exit(0);
  }
  destino=argv[1];

  
  if((fd=xpn_init())<0){
    printf("Error in init %d\n",fd);
    exit(-1);
  }
  
  fdp = xpn_unlink(destino); 
	  
  if(fdp<0){
    printf("error in remove fdp = %d\n",fdp);
    exit(-1);
  } 
  else printf("Remove successful\n");

  xpn_destroy();
  exit(0);
}
