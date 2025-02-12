
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


#define DATAM (512*KB)

int copyFile(char * origen, char * destino, int is_file)
{
  // printf("name:%s dest_name:%s\n", origen, destino);
  int fdp,fd,s,sp,sum, ret;
  char buffer[DATAM];

  if (!is_file)
  {
    ret = xpn_mkdir(destino, 0755);
    if ( ret < 0 )
    {
      printf("mkdir %s: %d %s\n",destino,errno,strerror(errno));
      return -1;
    }
    return 0;
  }

  fdp = xpn_open(destino,O_TRUNC|O_CREAT|O_RDWR,0777); 
  
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
    s=read(fd,buffer,DATAM);
    sp=xpn_write(fdp,buffer,s);
    printf(".");
    sum = sum +s;
  }while((s==DATAM)&&(sp >= 0));
  printf("\n");
  printf("total leidos %d bytes\n", sum);

  close(fd);
  xpn_close(fdp);
  return 0;
}

int list (char * dir_name, char * dest_prefix)
{
  // printf("name:%s dest_name:%s\n", dir_name, dest_prefix);
  int ret;
  DIR* dir = NULL;
  struct stat stat_buf;
  char path [PATH_MAX];

  dir = opendir(dir_name);
  if(dir == NULL)
  {
    perror("opendir:");
    return -1;
  }
  
  struct dirent* entry;
  entry = readdir(dir);

  while(entry != NULL)
  {
    if (! strcmp(entry->d_name, ".")){
      entry = readdir(dir);
      continue;
    }

    if (! strcmp(entry->d_name, "..")){
      entry = readdir(dir);
      continue;
    }

    sprintf(path, "%s/%s", dir_name, entry->d_name);
    char path_dst [PATH_MAX];
    sprintf(path_dst, "%s/%s", dest_prefix, entry->d_name);

    ret = stat(path, &stat_buf);
    if (ret < 0) 
    {
      perror("stat: ");
      printf("%s\n", path);
      entry = readdir(dir);
      continue;
    }

    int is_file = !S_ISDIR(stat_buf.st_mode);
    copyFile(path, path_dst, is_file);

    if (S_ISDIR(stat_buf.st_mode))
    {
      list(path, path_dst);
    }

    entry = readdir(dir);
  }

  closedir(dir);

  return 0;
}

int main(int argc, char *argv[])
{
  char *origen,*destino;
  int ret;
  // char buffer[DATAM];
  struct stat stat_buf;

  // Arguments
  if(argc !=3){
    printf("Incorrect number of parameters.\n");
    printf("Usage: %s <origin> <destination>\n",argv[0]);
    exit(0);
  }

  origen=argv[1];
  destino=argv[2];
  
  // XPN
  if((ret=xpn_init())<0){
    printf("Error in init %d\n",ret);
    exit(-1);
  }
  
  stat(origen, &stat_buf);
  if (S_ISDIR(stat_buf.st_mode))
  {
    xpn_mkdir(destino, 0755);
    list(origen, destino);
  }else{
    copyFile(origen, destino, 1);
  }
  xpn_destroy();

  exit(0);
}

