#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

#include "mpi.h"


int list (char * dir_name, FILE * fd)
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
    fprintf(fd, "%s\n", path);

    ret = stat(path, &stat_buf);
    if (ret < 0) {
        perror("stat: ");
        printf("%s\n", path);
        entry = readdir(dir);
        continue;
    }

    if (S_ISDIR(stat_buf.st_mode))
    {
      list(path, fd);
    }

    entry = readdir(dir);
  }

  closedir(dir);
  
  return 0;
}

int main(int argc, char *argv[])
{
  FILE * fd;

  if(argc < 3){
    printf("Too few arguments: xpn_ls <directory path> <output_file>\n");
    return -1;
  }

  MPI_Init(&argc, &argv);

  fd = fopen(argv[2], "w");
  if ( fd == NULL )
  {
    perror("fopen: ");
    return -1;
  }

  list(argv[1], fd);

  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();
  
  return 0;
}