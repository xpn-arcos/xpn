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

#define MIN(a,b) (((a)<(b))?(a):(b))
#define HEADER_SIZE (0)

char command[4*1024];
char src_path [PATH_MAX+5];
char dest_path [PATH_MAX+5];

int copy(char * entry, char * dir_name, char * dest_prefix, int blocksize, int rank, int size)
{   
  int  ret;

  struct stat stat_buf;

  int fd_src, fd_dest;
  char *buf ;
  int buf_len;
  int offset_src ;
  int cont, cont2 ;
  
  buf = (char *) malloc(blocksize + 1) ;
  if (NULL == buf) {
    perror("malloc: ");
    return -1;
  }

  //Generate source path
  //sprintf( src_path, "%s/%s", argv[1], entry );
  strcpy(src_path, entry);
  ret = stat(src_path, &stat_buf);
  if (ret < 0) {
    perror("stat: ");
    printf("[ERROR] %s\n", src_path);
    return -1;
  }

  //Generate destination path
  char * aux_entry = entry + strlen(dir_name);
  sprintf( dest_path, "%s/%s", dest_prefix, aux_entry );

  if (S_ISDIR(stat_buf.st_mode))
  {
    ret = mkdir(dest_path, 0755);
    if ( ret < 0 )
    {
      perror("mkdir: ");
      return -1;
    }
  }
  else if (S_ISREG(stat_buf.st_mode))
  {      
    fd_src = open(src_path, O_RDONLY);
    if ( fd_src < 0 )
    {
      perror("open 2: ");
      return -1;
    }

    fd_dest = open(dest_path, O_CREAT | O_WRONLY | O_TRUNC, 0755);
    if ( fd_dest < 0 )
    {
      perror("open 1: ");
      return -1;
    }

    // Write header
    ret = write(fd_dest, buf, HEADER_SIZE); // TODO: buf MUST be the header

    offset_src = rank * blocksize ;
    do
    {
      ret = lseek(fd_src, offset_src, SEEK_SET) ;
      if (ret < 0) {
        perror("lseek: ");
        return -1;
      }

      cont = 0;
      buf_len = blocksize;
      memset(buf, 0, buf_len);
      do {
        ret = read(fd_src, buf + cont, buf_len);
        cont    = cont + ret ;
        buf_len = buf_len - ret ;
      } while ( (cont < buf_len) && (ret != 0) );

      cont2 = 0;
      buf_len = cont;
      do {
        ret = write(fd_dest, buf + cont2, buf_len);
        cont2    = cont2 + ret ;
        buf_len  = buf_len - ret ;
      } while ( (cont2 < buf_len) && (ret != 0) );

      //printf("rank %d; ret: %d; offset %d; nodes %d; blocksize %d\n", rank, ret, offset_src, size, blocksize);
      //printf("Buf: %s\n", buf);

      offset_src = offset_src + (size * blocksize) ;
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
    copy(path, dir_name, dest_prefix, blocksize, rank, size);

    ret = stat(path, &stat_buf);
    if (ret < 0) {
        perror("stat: ");
        printf("%s\n", path);
        entry = readdir(dir);
        continue;
    }

    if (S_ISDIR(stat_buf.st_mode))
    {
      list(path, dest_prefix, blocksize, rank, size);
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

  list (argv[1], argv[2], atoi(argv[3]), rank, size);

  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();

  return 0;
}
