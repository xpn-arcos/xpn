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

char command[4*1024];
char src_path [PATH_MAX+5];
char dest_path [PATH_MAX+5];

int main(int argc, char *argv[])
{   
  int  ret, rank, size, is_end, blocksize;

  FILE *file;
  char entry [PATH_MAX];
  struct stat stat_buf;

  int fd_src, fd_dest;
  char *buf ;
  int buf_len;
  
  
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

  //
  // Initialize MPI
  //
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  blocksize = atoi(argv[3]) ;
  buf_len = size * blocksize;

  buf = (char *) malloc(buf_len) ;
  if (NULL == buf) {
    perror("malloc: ");
    return -1;
  }

  // if (Rank == 0)  {
  //     1- hace el ls para listar el contenido de la partición
  // }
  if (rank == 0)
  {
    // ls -d... -> https://stackoverflow.com/questions/246215/how-can-i-generate-a-list-of-files-with-their-absolute-path-in-linux
    sprintf(command, "tree -fainc %s | head -n -2 | tail -n +2  | sed 's|%s||g' > /tmp/partition_content.txt ", argv[1], argv[1]);
    system(command);

    file = fopen("/tmp/partition_content.txt", "r");
    if ( NULL == file )
    {
      perror("fopen: ");
      return -1;
    }
  }

  //
  // Barrier (for get file/directory listing)
  //
  MPI_Barrier(MPI_COMM_WORLD);

  //
  // While entry in listing... do
  //
  is_end = 0 ;
  while( ! is_end )
  {
    // if (Rank == 0) {
    //   leer línea
    // }
    if (rank == 0) 
    {
      ret = fscanf(file, "%s\n", entry);
      //printf("RET: %d\n", ret);
    }

    // mpi_bcast(ret, ...)
    // if (EOF) -> break
    MPI_Bcast(&ret, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (EOF == ret)
    {
      is_end = 1 ;
      break;
    }

    MPI_Bcast(entry, PATH_MAX, MPI_CHAR, 0, MPI_COMM_WORLD);

    //Generate source path
    sprintf( src_path, "%s/%s", argv[1], entry );

    // if (Rank == 0) {
    //    hacer stat línea
    // }
    if (rank == 0) 
    {
      //printf("ENTRY: %s\n", src_path);
      ret = stat(src_path, &stat_buf);
      if (ret < 0) {
        perror("stat: ");
      }
    }

    MPI_Bcast( &ret, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if ( ret < 0 )
    {
      continue;
    }

    //Generate destination path
    //sprintf( dest_path, "%s/%d/%s", argv[2], rank, entry ); //Debug local
    sprintf( dest_path, "%s/%s", argv[2], entry );

    MPI_Bcast( dest_path, PATH_MAX+1,       MPI_CHAR, 0, MPI_COMM_WORLD);
    MPI_Bcast(&stat_buf, sizeof(stat_buf), MPI_CHAR, 0, MPI_COMM_WORLD);

    if (S_ISDIR(stat_buf.st_mode))
    {
      ret = mkdir(dest_path, 0755);
      if ( ret < 0 )
      {
        perror("mkdir (%d): ", rank);
      }
    }
    else if (S_ISREG(stat_buf.st_mode))
    {      
      if (rank == 0)
      {
        fd_src = open(src_path, O_RDONLY);
        if ( NULL == file )
        {
          perror("open (%d): ", rank);
        }
      }

      MPI_Bcast(&fd_src, 1, MPI_INT, 0, MPI_COMM_WORLD);
      if ( fd_src < 0 )
      {
        continue;
      }

      fd_dest = open(dest_path, O_CREAT | O_WRONLY | O_TRUNC, 0755);
      if ( fd_dest < 0 )
      {
        perror("open (%d): ", rank);
      }

      ret = -1;
      do
      {
        if (rank == 0){
          ret = read(fd_src, buf, buf_len);
        }

        MPI_Bcast(&ret, 1, MPI_INT, 0, MPI_COMM_WORLD);
        if ( ret < 0 )
        {
          continue;
        }

        MPI_Bcast(buf, ret, MPI_CHAR, 0, MPI_COMM_WORLD);

        int ret2 ;
        int offset = rank * blocksize ;
        int to_write = MIN(blocksize, ret) ;
        do
        {
          ret2   = write(fd_dest, buf + offset, to_write);
          if ( ret2 < 0 )
          {
            perror("write (%d): ", rank);
          }
          
          offset = offset + (size * blocksize);
          //printf("rank %d; ret: %d; offset %d; nodes %d; blocksize %d\n", rank, ret2, offset, size, blocksize);
        }
        while(offset < ret);
      }
      while(ret > 0);

      if (rank == 0)
      {
        close(fd_src);
      }

      close(fd_dest);
    }

    MPI_Barrier(MPI_COMM_WORLD);
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







/*

   do
   {
       Bcast( stat.directoro | stat.fichero + path )

       if (stat.directorio)
       {
          mkdir(path)
       }
       else if (stat.fichero)
       {
          if (rank == 0) 
          {
              xpn_read(BUFFER, número_servidores_antiguos * nümero_servidores_nuevos * sizeof(bloque) )
          }

          bcast(BUFFER)

          bucle dependiendo cada uno de su Rank
                 ...
                 lseek + write(1 bloque)
                 ...

       }

       mpi_barrier();

   } while ( 1 )


 */
