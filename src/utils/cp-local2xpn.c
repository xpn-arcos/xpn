
  /*
   *  Copyright 2000-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
   *
   *  This file is part of Expand.
   *
   *  Expand is free software: you can redistribute it and/or modify
   *  it under the terms of the GNU Lesser General Public License as published by
   *  the Free Software Foundation, either version 3 of the License, or
   *  (at your option) any later version.
   *
   *  Expand is distributed in the hope that it will be useful,
   *  but WITHOUT ANY WARRANTY; without even the implied warranty of
   *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   *  GNU Lesser General Public License for more details.
   *
   *  You should have received a copy of the GNU Lesser General Public License
   *  along with Expand.  If not, see <http://www.gnu.org/licenses/>.
   *
   */


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "all_system.h"
#include "xpn.h"


#define DATAM (256*KB)


int main(int argc, char *argv[])
{
  char *origen,*destino;
  int fdp,fd,s,sp,sum;
  char buffer[DATAM];

  // Arguments
  if(argc !=3){
    printf("Incorrect number of parameters.\n");
    printf("Usage: %s <origin> <destination>\n",argv[0]);
    exit(0);
  }

  origen=argv[1];
  destino=argv[2];
  
  // XPN
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

