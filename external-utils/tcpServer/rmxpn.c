
/*
 *  Copyright 2020-2022 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
 *
 *  This file is part of mpiServer.
 *
 *  mpiServer is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  mpiServer is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with mpiServer.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


   /* ... Include / Inclusion ........................................... */

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


   /* ... Functions / Funciones ......................................... */

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
