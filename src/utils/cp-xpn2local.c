
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


#define DATAM (64*KB)


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

