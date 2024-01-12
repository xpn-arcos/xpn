
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


int main(int argc, char *argv[])
{
  char *destination;
  int fdp,fd;

  // Arguments
  if(argc !=2) {
    printf("ERROR: Incorrect number of parameters.\n") ;
    printf("Usage \"%s <path>\"\n", argv[0]);
    exit(0);
  }

  // XPN
  if((fd=xpn_init())<0){
    printf("Error in init %d\n",fd);
    exit(-1);
  }
  
  destination=argv[1];
  // TODO: xpn_mkdir -> xpn_statfs
  fdp = xpn_mkdir(destination,777); 
	  
  if(fdp<0){
    printf("error in mkdir fdp = %d\n",fdp);
    exit(-1);
  } 

  xpn_destroy();
  exit(0);
}

