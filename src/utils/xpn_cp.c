
/*
 *  Copyright 2020-2023 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos
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
      #include <sys/types.h>
      #include <sys/stat.h>
      #include <fcntl.h>
      #include <unistd.h>

   /* ... Const / Const ................................................. */

      #define BUFFER_SIZE (8192*1024)

      char buf[BUFFER_SIZE];


   /* ... Functions / Funciones ......................................... */

      int do_cp ( char *src_path, char *dest_path )
      {
	int ret, fd_src, fd_dest ;

	fd_src = open(src_path, O_RDONLY);
	if (fd_src < 0) {
	  perror("open source: ");
	  return -1;
	}

	fd_dest = open(dest_path, O_WRONLY | O_CREAT | O_TRUNC, 0755);
	if (fd_dest < 0)
	{
	  perror("open destination: ");
	  return -1;
	}

	int cont  = 0;
	int cont2 = 0;
	int buf_len = BUFFER_SIZE;

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

      int do_xx ( char *src_path, char *dest_path )
      {
      /*
stat src_path
if IS_FILE -> do_cp y return
if IS_DIR -> ls y con cada entrada... do_cp... recursivo
       */

	return 0;
      }


      int main ( int argc, char *argv[] )
      {
	int ret;

	printf("\n");
	printf("  xpn_cp\n");
	printf(" --------\n");
	printf("\n");
	printf("  Usage:\n");
	printf("  %s ./xpn_copy <src_path> <dest_path>\n", argv[0]) ;
	printf("\n");

	if (argc < 3)
	{
	   printf("  >> ERROR: too few arguments :-(\n");
	   printf("\n");
	   return -1;
	}

        ret = do_cp(argv[1], argv[2]) ;

	return ret ;
      }


   /* ................................................................... */

