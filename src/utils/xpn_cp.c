
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
  #include <sys/stat.h>
  #include <fcntl.h>
  #include <dirent.h>
  #include <string.h>
  #include <errno.h>


/* ... Const / Const ................................................. */

  #define BUFFER_SIZE (8192*1024)
  char buf[BUFFER_SIZE];


/* ... Functions / Funciones ......................................... */

  int do_cp ( char *src_path, char *dest_path )
  {
    int ret, fd_src, fd_dest ;
    int cont  = 0;
    int cont2 = 0;
    int buf_len = BUFFER_SIZE;

    fd_src = open(src_path, O_RDONLY);
    if (fd_src < 0) {
      perror("open source: ");
      return -1;
    }

    // TODO: mkdir any directory from dest_path before open it, just in case parent directories doesn't exists...

    fd_dest = open(dest_path, O_WRONLY | O_CREAT | O_TRUNC, 0755);
    if (fd_dest < 0)
    {
      perror("open destination: ");
      return -1;
    }

    do
    {
      cont = 0;
      buf_len = BUFFER_SIZE;
      do {
        ret = read(fd_src, buf + cont, buf_len);
        if (ret == -1)
        {
          perror("read: ");
          return -1;
        }

        cont    = cont + ret ;
        buf_len = buf_len - ret ;
      } while ( (cont < buf_len) && (ret > 0) );

      cont2 = 0;
      buf_len = cont;

      do {
        ret = write(fd_dest, buf + cont2, buf_len);
        if (ret == -1)
        {
          perror("write: ");
          return -1;
        }

        cont2    = cont2 + ret ;
        buf_len  = buf_len - ret ;
      } while ( (cont2 < cont) && (ret > 0) );

    } while(cont > 0);

    close(fd_src);
    close(fd_dest);
    
    return 0;
  }

  int do_cp_recursive ( char *src_path, char *dest_path )
  {
    struct stat stat_buf;
    int ret = 0;

    ret = stat(src_path, &stat_buf);
    if (ret < 0) {
      perror("stat source: ");
      printf("ERROR at '%s': stat failed\n", src_path);
      return -1;
    }

    if (S_ISREG(stat_buf.st_mode))
    {
      printf("=");
      //printf("cp '%s' '%s' ...\n", src_path, dest_path);
      return do_cp(src_path, dest_path) ;
    }
    else if (S_ISDIR(stat_buf.st_mode))
    {
      DIR * dir = NULL;
      struct dirent * entry = NULL;
      char path_src [PATH_MAX];
      char path_dst [PATH_MAX];

      // mkdir in destination
      //printf("mkdir '%s' ...\n", dest_path);
      ret = mkdir(dest_path, stat_buf.st_mode);
      if ((ret == -1) && (errno != EEXIST))
      {
        perror("mkdir:");
        return -1;
      }

      // traverse a src tree...
      dir = opendir(src_path);
      if (NULL == dir) {
        perror("opendir:");
        return -1;
      }
    
      entry = readdir(dir);
      while(entry != NULL)
      {
        if (! strcmp(entry->d_name, ".")) {
          entry = readdir(dir);
          continue;
        }
        if (! strcmp(entry->d_name, "..")) {
          entry = readdir(dir);
          continue;
        }

        sprintf(path_src, "%s/%s",  src_path, entry->d_name);
        sprintf(path_dst, "%s/%s", dest_path, entry->d_name);
        do_cp_recursive(path_src, path_dst) ;

        entry = readdir(dir);
      }

      closedir(dir);
    }
    else
    {
      printf("ERROR at '%s': not a directory or file\n", src_path);
    }

    return ret;
  }


  int main ( int argc, char *argv[] )
  {
    int ret;

    if (argc < 3)
    {
      printf("[ERROR]: too few arguments\n");
      printf("\n");

      printf("  Usage:\n");
      printf("  %s <src_path> <dest_path>\n", argv[0]) ;
      printf("\n");

      return -1;
    }

    printf("Copying... \n");
    ret = do_cp_recursive(argv[1], argv[2]) ;
    printf("\n");

    return ret ;
  }


/* ................................................................... */

