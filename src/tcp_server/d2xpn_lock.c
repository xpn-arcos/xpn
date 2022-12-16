
  /*
   *  Copyright 2000-2023 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
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


   /* ... Include / Inclusion ........................................... */

#include <sys/param.h>
#include <stdio.h>
#include <sys/file.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/errno.h>
#include "xpn.h"

#ifndef KB
#define KB 1024
#endif

#ifndef KB
#define MB (KB*KB)
#endif

#define DATAM (256*KB)

#define DEFAULT_PATH "/tmp"


   /* ... Functions / Funciones ......................................... */

void generateName(char *file, char *new_file)
{
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

int main(int argc, char *argv[])
{
  
  char *origen,*destino;
  int fdp,fd,s,sp, ret;
  //int fd_lock;
  char buffer[DATAM];
  struct stat st, st_xpn; 
  int sum = 0;

  if(argc !=3){
    printf("Incorrect number of parameters.\n");
    printf("Usage: %s <virtual path> <storage_path>\n", argv[0]);
    exit(0);
  }

  origen=argv[1]; //virtual file
  destino=argv[2]; //storage file

#ifdef _DBG_
	printf("d2xpn: Origen: %s\n",origen);
	printf("d2xpn: Destino: %s\n",destino);
	fflush(stdout);
#endif

#ifdef _DBG_
	printf("d2xpn: xpn_init()\n");
	fflush(stdout);
#endif
  if((fd=xpn_init())<0){
    xpn_destroy();	  
    printf("Error in init %d\n",fd);
    exit(-1);
  }


  
#ifdef _DBG_
	printf("d2xpn: stat(%s)\n",destino);
	fflush(stdout);
#endif
  ret = stat(destino, &st);
  if (ret == -1){
    xpn_destroy();
    printf("error in stat(%s)\n",destino);
    exit(-1);
  }

#ifdef _DBG_
	printf("d2xpn: xpn_stat(%s)\n",origen);
	fflush(stdout);
#endif
  ret = xpn_stat(origen, &st_xpn);
  if (ret != -1){

 	 if(st.st_size == st_xpn.st_size){
	    xpn_destroy();	  
	    printf("%s and %s are the same file\n", origen, destino);
	    exit(0);	    
	  }
  }



#ifdef _DBG_
	printf("d2xpn: open(%s, O_RDONLY)\n",destino);
	fflush(stdout);
#endif
  fd=open(destino,O_RDONLY);
  if(fd<0){
    xpn_destroy();	  
    printf("error in open fd = %d\n",fd);
    exit(-1);
  }  
  

  
#ifdef _DBG_
	printf("d2xpn: xpn_open(%s, O_CREAT|O_TRUNC|O_WRONLY, 0777)\n",origen);
#endif
  fdp = xpn_open(origen,O_CREAT|O_TRUNC|O_WRONLY, 0777); 
  if(fdp<0){
    xpn_destroy();	  
    printf("error in xpn_open fdp = %d\n",fdp);
    exit(-1);
  } 

 
  

  sum = 0;
  do{

#ifdef _DBG_
	printf("d2xpn: antes read(%d,%d)\n", DATAM, sum);
#endif
    sp = read(fd,buffer,DATAM);
    //printf("antes de xpn_write(%d bytes) ...\n", s);
    if(s == -1){
	    break;
    }
#ifdef _DBG_
	printf("d2xpn: desp. read(%d,%d)\n", sp, sum);
#endif

#ifdef _DBG_
	printf("d2xpn: antes write(%d,%d)\n", sp , sum);
#endif
    s = xpn_write(fdp, buffer, sp);
#ifdef _DBG_
	printf("d2xpn: desp write(%d,%d)\n", s , sum);
#endif
    //printf("despues de xpn_write ...\n");
    sum = sum + sp;

    //printf("Se han leido s=%d y escrito sp=%d\n", s, sp);
  }while((s==DATAM)&&(sp >= 0));

#ifdef _DBG_
	printf("d2xpn: (%s,%d)\n",origen,sum);
#endif

#ifdef _DBG_
	printf("d2xpn: close()\n");
#endif
  close(fd);
#ifdef _DBG_
	printf("d2xpn: xpn_close()\n");
#endif
  xpn_close(fdp);
#ifdef _DBG_
	printf("d2xpn: xpn_destroy()\n");
#endif
  xpn_destroy();

  exit(0);
}
