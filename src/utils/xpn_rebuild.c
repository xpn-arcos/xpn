#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <linux/limits.h>
#include <sys/stat.h>

#include "mpi.h"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define HEADER_SIZE (0)

char command[4*1024];
char src_path [PATH_MAX+5];
char dest_path [PATH_MAX+5];

int main(int argc, char *argv[])
{   
  int  ret, rank, size, is_end, blocksize;

  FILE *file = NULL;
  char entry [PATH_MAX];
  struct stat stat_buf;

  int fd_src, fd_dest;
  char *buf ;
  int buf_len;
  int offset_src ;
  int cont, cont2 ;
  
  
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

  blocksize = atoi(argv[3]) ;

  buf = (char *) malloc(blocksize) ;
  if (NULL == buf) {
    perror("malloc: ");
    return -1;
  }

  // ls -d... -> https://stackoverflow.com/questions/246215/how-can-i-generate-a-list-of-files-with-their-absolute-path-in-linux
  sprintf(command, "tree -fainc %s | head -n -2 | tail -n +2  | sed 's|%s||g' > /tmp/partition_content.txt ", argv[1], argv[1]);
  ret = system(command);

  file = fopen("/tmp/partition_content.txt", "r");
  if ( NULL == file )
  {
      perror("fopen: ");
      return -1;
  }

  //
  // While entry in listing... do
  //
  is_end = 0 ;
  while( ! is_end )
  {
    ret = fscanf(file, "%s\n", entry);
    if (EOF == ret)
    {
      is_end = 1 ;
      continue;
    }

    //Generate source path
    sprintf( src_path, "%s/%s", argv[1], entry );

    ret = stat(src_path, &stat_buf);
    if (ret < 0) {
        perror("stat: ");
        continue;
    }

    //Generate destination path
    //sprintf( dest_path, "%s/%s", argv[2], entry );
    sprintf( dest_path, "%s/%d/%s", argv[2], rank, entry );


    if (S_ISDIR(stat_buf.st_mode))
    {
      ret = mkdir(dest_path, 0755);
      if ( ret < 0 )
      {
        perror("mkdir: ");
      }
    }
    else if (S_ISREG(stat_buf.st_mode))
    {      
        fd_src = open(src_path, O_RDONLY);
        if ( NULL == file )
        {
          perror("open: ");
          continue;
        }

        fd_dest = open(dest_path, O_CREAT | O_WRONLY | O_TRUNC, 0755);
        if ( fd_dest < 0 )
        {
          perror("open: ");
          continue;
        }

        // Write header
        ret = write(fd_dest, buf, HEADER_SIZE); // TODO: buf MUST be the header

        offset_src = rank * blocksize ;
        do
        {
          lseek(fd_src, offset_src, SEEK_SET) ;

          cont = 0;
          buf_len = blocksize;
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

          //printf("rank %d; ret: %d; offset %d; nodes %d; blocksize %d\n", rank, ret2, offset, size, blocksize);

          offset_src = offset_src + (size * blocksize) ;
        }
        while(cont > 0);

        close(fd_src);
        close(fd_dest);
    }
  }

  if (rank == 0)
  {
    ret = unlink("/tmp/partition_content.txt");
    if ( -1 == ret )
    {
      perror("unlink: ");
      return -1;
    }
  }

  free(buf);

  MPI_Finalize();

  return 0;
}
