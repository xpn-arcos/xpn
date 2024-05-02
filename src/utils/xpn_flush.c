
/*
 *  Copyright 2020-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Dario Muñoz Muñoz
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
  #include "xpn/xpn_simple/xpn_policy_rw.h"


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

  int copy(char * entry, int is_file, char * dir_name, char * dest_prefix, int blocksize, int replication_level, int rank, int size)
  {  
    int  ret;

    int fd_src, fd_dest, replication = 0;
    char *buf ;
    int buf_len;
    off64_t offset_dest ;
    off_t offset_src;
    ssize_t read_size, write_size;
    struct stat st, st_src;

    //Alocate buffer
    buf_len = blocksize;
    buf = (char *) malloc(buf_len) ;
    if (NULL == buf) {
      perror("malloc: ");
      return -1;
    }

    //Generate source path
    strcpy(src_path, entry);

    //Generate destination path
    char * aux_entry = entry + strlen(dir_name);
    sprintf( dest_path, "%s/%s", dest_prefix, aux_entry );

    if (rank == 0){
      printf("%s\n", aux_entry);
    }

    ret = stat(src_path, &st_src);
    if (ret < 0){
      perror("stat: ");
      free(buf) ;
      return -1;
    }
    if (!is_file)
    {
      if (rank == 0)
      {
        if (stat(dest_path, &st) == -1) {
          ret = mkdir(dest_path, st_src.st_mode);
          if ( ret < 0 )
          {
            perror("mkdir: ");
            free(buf) ;
            return -1;
          }
        }
      }
    }
    else if (is_file)
    {      
      fd_src = open64(src_path, O_RDONLY | O_LARGEFILE);
      if ( fd_src < 0 )
      {
        perror("open 1: ");
        free(buf) ;
        return -1;
      }

      if (rank == 0)
      {
        fd_dest = creat(dest_path, st_src.st_mode);
        if ( fd_dest < 0 ){
          perror("creat 1: ");
        }else{
          close(fd_dest);
        }
      }
      MPI_Bcast(&fd_dest, 1, MPI_INT, 0, MPI_COMM_WORLD);
      if (fd_dest < 0)
      {
        free(buf) ;
        return -1;
      }
      fd_dest = open64(dest_path, O_WRONLY | O_LARGEFILE);
      if ( fd_dest < 0 )
      {
        perror("open 2: ");
        free(buf) ;
        return -1;
      }

      off64_t ret_1;
      offset_src = 0;

      do
      { 
        //TODO: check when the server has error and data is corrupt for fault tolerance
        XpnCalculateBlockInvert(blocksize, replication_level, size, rank, offset_src, &offset_dest, &replication);
        
        if(offset_src > st_src.st_size){
          break;
        }
        if (replication != 0){
          offset_src += blocksize;
          continue;
        }

        ret_1 = lseek64(fd_src, offset_src + HEADER_SIZE, SEEK_SET) ;
        if (ret_1 < 0) {
          perror("lseek: ");
          break;
        }
        read_size = filesystem_read(fd_src, buf, buf_len);
        if (read_size <= 0){
          break;
        }

        ret_1 = lseek64(fd_dest, offset_dest, SEEK_SET) ;
        if (ret_1 < 0) {
          perror("lseek: ");
          break;
        }
        write_size = filesystem_write(fd_dest, buf, read_size);
        if (write_size != read_size){
          perror("write: ");
          break;
        }
        offset_src += blocksize;
      }
      while(read_size > 0);

      close(fd_src);
      close(fd_dest);
    }
    
    free(buf);
    return 0;
  }


  int list (char * dir_name, char * dest_prefix, int blocksize, int replication_level, int rank, int size)
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
      copy(path, is_file, dir_name, dest_prefix, blocksize, replication_level, rank, size);

      if (S_ISDIR(stat_buf.st_mode))
      {
        char path_dst [PATH_MAX];
        sprintf(path_dst, "%s/%s", dest_prefix, entry->d_name);
        list(path, path_dst, blocksize, replication_level, rank, size);
      }

      entry = readdir(dir);
    }

    closedir(dir);

    return 0;
  }


  int main(int argc, char *argv[])
  {   
    int rank, size;
    int replication_level = 0;
    int blocksize = 524288;
    double start_time;
    //
    // Check arguments...
    //
    if ( argc < 3 )
    {
      printf("Usage:\n");
      printf(" ./%s <origin partition> <destination local path> <optional destination block size> <optional replication level>\n", argv[0]);
      printf("\n");
      return -1;
    }
    
    if ( argc >= 5){
      replication_level = atoi(argv[4]);
    }
    if ( argc >= 4){
      blocksize = atoi(argv[3]);
    }
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    start_time = MPI_Wtime();
    if (rank == 0){
      printf("Copying from %s to %s blocksize %d replication_level %d \n", argv[1], argv[2], blocksize, replication_level);
    }
    list (argv[1], argv[2], blocksize, replication_level, rank, size);
    if (rank == 0){
      printf("Flush elapsed time %f mseg\n", (MPI_Wtime() - start_time)*1000);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();

    return 0;
  }

/* ................................................................... */
