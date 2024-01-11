
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
  #include <dirent.h>
  #include <string.h>
  #include <sys/stat.h>
  #include "mpi.h"


/* ... Functions / Funciones ......................................... */

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
      printf("ERROR: too few arguments.\n");
      printf("Usage: %s <directory path> <output_file>\n", argv[0]);
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


/* ................................................................... */

