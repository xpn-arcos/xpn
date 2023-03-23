#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE (8192*1024) 

int main(int argc, char *argv[])
{

	char buf[BUFFER_SIZE];
	int ret ,fd_src, fd_dest, bytes;
	int valor_close;

	if(argc < 3)
	{
		printf("Too few  arguments ./xpn_copy <src_path> <dest_path>\n");
		return -1;
	}

	fd_src = open(argv[1], O_RDONLY);
	if(fd_src == -1){
		perror("open 1: ");
		return -1;
	}

	fd_dest = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0755);
	if(fd_dest == -1)
	{
		perror("open 2: ");
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
  }
  while(cont > 0);

	close(fd_src);
	close(fd_dest);
	
	return 0;
}

