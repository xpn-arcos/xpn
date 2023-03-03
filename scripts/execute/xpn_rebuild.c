#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <linux/limits.h>

//#include "mpi.h"


/*int copy_dir (path)
{

}*/

int copy_file (char * src_path, char * dest_path) //TODO: parallel MPI
{
  int src_fd, dest_fd, nbytes;
  char buf[1024];


  printf("ORIGEN:  %s\n",  src_path);
  printf("DESTINO: %s\n", dest_path);


  /*src_fd = open(src_path, O_RDONLY);
  if (src_fd < 0)
  {
    printf("ERROR: \n"); //TODO
    return -1;
  }

  dest_fd = open(dest_path, O_RDWR | O_CREAT | O_TRUNC, 0755);
  if (dest_fd < 0)
  {
    printf("ERROR: \n"); //TODO
    return -1;
  }


  do
  {
    nbytes = read(src_fd, buf, 1024);
    if (nbytes > 0)
    {
      write(dest_fd, buf, nbytes);
    }
  }while(nbytes > 0);


  close(src_fd);
  close(dest_fd);*/

  return 1;
}








int main(int argc, char *argv[])
{   
  int  rank, size;
  FILE *file;
  char command[1024];
  char line [1024];
  char src_path  [PATH_MAX];
  char dest_path [PATH_MAX];

  //MPI: crear árbol

  if ( argc < 3 )
  {
    printf("Please, enter origin partition and destination path\n");
    return -1;
  }

  /*MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);*/

  sprintf( command, "ls -ARl %s > /tmp/partition_content.txt", argv[1] );
  system( command );

  file = fopen("/tmp/partition_content.txt", "r");
  if ( NULL == file )
  {
    printf("ERROR: \n"); //TODO
    return -1;
  }

  
  while (!feof(file))
  {
    fscanf(file, "%s\n", line); //Ajustar

    printf("------- %s --------\n", line);

    if (line[0] == 'd')
    {
      printf("DIR\n");
    }
    if (line[0] == '-')
    {
      printf("FILE\n");
    }

    /*sprintf(  src_path, "%s/%s", argv[1], line );
    sprintf( dest_path, "%s/%s", argv[2], line );

    copy_file (src_path, dest_path);*/
  }

  //TODO: ver si es el directorio por los :
  // fichero llamar a cp copy_file (char * src_path, char * dest_path)
  // directorio crear
  

  return 0;
}