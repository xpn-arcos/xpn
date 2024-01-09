
/*
 *  Copyright 2020-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos
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

  #include <stdio.h>
  #include <unistd.h>
  #include <sys/types.h>
  #include <stdlib.h>
  #include <string.h>
  #include <fcntl.h>
  #include <linux/limits.h>
  #include <sys/stat.h>
  #include <dirent.h>
  #include "mpi.h"


/* ... Const / Const ................................................. */

  #ifndef _LARGEFILE_SOURCE
  #define _LARGEFILE_SOURCE
  #endif

  #ifndef _FILE_OFFSET_BITS
  #define _FILE_OFFSET_BITS 64
  #endif

  #define MIN(a,b) (((a)<(b))?(a):(b))
  #define HEADER_SIZE 8192

  char command[4*1024];
  char src_path [PATH_MAX+5];
  char dest_path [PATH_MAX+5];


/* ... Functions / Funciones ......................................... */

  int copy(char * entry, int is_file, char * dir_name, char * dest_prefix, int blocksize, int rank, int size)
  {  
    int  ret;

    int fd_src, fd_dest;
    char *buf ;
    int buf_len;
    off64_t offset_dest ;
    int cont, cont2 ;

    printf("=");
    
    buf = (char *) malloc(blocksize + 1) ;
    if (NULL == buf) {
      perror("malloc: ");
      return -1;
    }

    //Generate source path
    //sprintf( src_path, "%s/%s", argv[1], entry );
    strcpy(src_path, entry);

    //Generate destination path
    char * aux_entry = entry + strlen(dir_name);
    sprintf( dest_path, "%s/%s", dest_prefix, aux_entry );

    if (!is_file)
    {
      ret = mkdir(dest_path, 0755);
      if ( ret < 0 )
      {
        perror("mkdir: ");
        free(buf) ;
        return -1;
      }
    }
    else if (is_file)
    {      
      fd_src = open64(src_path, O_RDONLY | O_LARGEFILE);
      if ( fd_src < 0 )
      {
        perror("open 2: ");
        free(buf) ;
        return -1;
      }

      if (rank == 0)
      {
        fd_dest = open64(dest_path, O_CREAT | O_WRONLY | O_TRUNC | O_LARGEFILE, 0755);
        if ( fd_dest < 0 )
        {
          perror("open 1: ");
          free(buf) ;
          return -1;
        }
        MPI_Barrier(MPI_COMM_WORLD);
      }
      else
      {
        MPI_Barrier(MPI_COMM_WORLD);

        fd_dest = open64(dest_path, O_WRONLY | O_LARGEFILE);
        if ( fd_dest < 0 )
        {
          perror("open 1: ");
          free(buf) ;
          return -1;
        }
      }

      //offset_dest = rank * blocksize ;
      off64_t ret_1;
      ret_1 = lseek64(fd_src, HEADER_SIZE, SEEK_SET) ;
      if (ret_1 < 0) {
        perror("lseek: ");
        return -1;
      }

      offset_dest = rank * blocksize ;

      do
      { 
        cont = 0;
        buf_len = blocksize;
        memset(buf, 0, buf_len);
        do {
          ret = read(fd_src, buf + cont, buf_len);
          cont    = cont + ret ;
          buf_len = buf_len - ret ;
        } while ( (cont < buf_len) && (ret != 0) );


        off64_t ret_2;
        ret_2 = lseek64(fd_dest, offset_dest, SEEK_SET) ;
        if (ret_2 < 0) {
          //perror("lseek: ");
          break;
        }

        cont2 = 0;
        buf_len = cont;
        do {
          ret = write(fd_dest, buf + cont2, buf_len);
          cont2    = cont2 + ret ;
          buf_len  = buf_len - ret ;
        } while ( (cont2 < cont) && (ret != 0) );

        //printf("rank %d; ret: %d; offset %ld; nodes %d; blocksize %d\n", rank, ret, offset_dest, size, blocksize);
        //printf("Buf: %s\n", buf);

        offset_dest = offset_dest + (size * blocksize) ;
      }
      while(cont > 0);

      close(fd_src);
      close(fd_dest);
    }
    
    free(buf);
    return 0;
  }


  int list (char * dir_name, char * dest_prefix, int blocksize, int rank, int size)
  {
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

      ret = stat(path, &stat_buf);
      if (ret < 0) 
      {
        perror("stat: ");
        printf("%s\n", path);
        entry = readdir(dir);
        continue;
      }

      int is_file = !S_ISDIR(stat_buf.st_mode);
      copy(path, is_file, dir_name, dest_prefix, blocksize, rank, size);

      if (S_ISDIR(stat_buf.st_mode))
      {
        char path_dst [PATH_MAX];
        sprintf(path_dst, "%s/%s", dest_prefix, entry->d_name);
        list(path, path_dst, blocksize, rank, size);
      }

      entry = readdir(dir);
    }

    closedir(dir);

    return 0;
  }


  int main(int argc, char *argv[])
  {   
    int rank, size;

    //
    // Check arguments...
    //
    if ( argc < 4 )
    {
      printf("Usage:\n");
      printf(" ./%s <origin partition> <destination local path> <destination block size>\n", argv[0]);
      printf("\n");
      return -1;
    }

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    printf("Copying... \n");
    list (argv[1], argv[2], atoi(argv[3]), rank, size);
    printf("\n");

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();

    return 0;
  }

/* ................................................................... */
