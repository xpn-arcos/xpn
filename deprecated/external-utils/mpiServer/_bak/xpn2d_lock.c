
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
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/errno.h>
#include "expand.h"

#ifndef KB
#define KB 1024
#endif

#ifndef KB
#define MB (KB*KB)
#endif

#define DATAM (256*KB)

#define DEFAULT_PATH "/tmp"


   /* ... Functions / Funciones ......................................... */

void generateName(char *file, char *new_file){
	char aux[255];
	int i,j;
	
	for(j=0,i=0;i<strlen(file);i++){
		switch(file[i]){
			case '/':
				aux[j] = '_';		
				break;
			case '_':				
				aux[j] = '_';
				j++;
				aux[j] = '_';		
				break;				
			default:
				aux[j] = file[i];
				break;
		}
		j++;
	}
	aux[j] = '\0';
	
	sprintf(new_file,"%s/%s", DEFAULT_PATH, aux);
	return;
}

int mylock(char *file){
	char new_file[255];
	int fd;

	
	generateName(file, new_file);
	fd = open(new_file, O_CREAT|O_TRUNC|O_RDWR, 0777);
	if(fd == -1){
		return -1;
	}
 	flock(fd, LOCK_EX);

	return fd;
}


int myunlock(int fd){
  	flock(fd, LOCK_UN);
	close(fd);
	
	return 0;
}


int main(int argc, char *argv[])
{
  
  char *origen,*destino;
  int fdp,fd,s,sp, fd_lock, ret;
  char buffer[DATAM];
  struct stat st, st_xpn; 

  if(argc !=3){
    printf("Incorrect number of parameters.\n");
    printf("Usage: %s <virtual path> <storage path>\n", argv[0]);
    exit(0);
  }

  origen=argv[1];
  destino=argv[2];


  fd_lock = mylock(destino);
  if(fd_lock == -1){
	printf("Err: lock %s\n",destino);
	exit(-1);	  
  }

  
  if((fd=xpn_init())<0){
    printf("Error in init %d\n",fd);
    myunlock(fd_lock);
    exit(-1);
  }

  ret = xpn_stat(origen, &st_xpn);
  if (ret == -1){
    myunlock(fd_lock);
    xpn_destroy();	  
    printf("error in stat(%s)\n",destino);
    exit(-1);
  }

  
  ret = stat(destino, &st);
  if (ret != -1){
	  if(st.st_size == st_xpn.st_size){
	    myunlock(fd_lock);
	    xpn_destroy();	  
	    printf("%s and %s are the same file\n", origen, destino);
	    exit(0);	    
	  }
  }




  fdp=xpn_open(origen, O_RDONLY);
  if(fdp<0){
    myunlock(fd_lock);
    xpn_destroy();	  
    printf("error in xpn_open fd = %d\n",fdp);
    exit(-1);
  }  
  

  
  fd = open(destino, O_CREAT|O_TRUNC|O_WRONLY,0777); 
  if(fd<0){
    myunlock(fd_lock);
    xpn_destroy();	  
    printf("error in open fdp = %d\n",fd);
    exit(-1);
  } 

 
  

  //sum = 0;
  do{

    s = xpn_read(fdp,buffer,DATAM);
    //printf("antes de xpn_write(%d bytes) ...\n", s);
    if(s == -1){
	    break;
    }
    sp = write(fd,buffer,s);
    //printf("despues de xpn_write ...\n");
    //sum = sum +s;

    //printf("Se han leido s=%d y escrito sp=%d\n", s, sp);
  }while((s==DATAM)&&(sp >= 0));

  close(fd);
  xpn_close(fdp);
  myunlock(fd_lock);
  xpn_destroy();

  exit(0);
}

   /* ................................................................... */

