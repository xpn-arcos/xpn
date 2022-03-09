#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "mpi.h"

#define BLOCKSIZE (512 * 1024)

int main(int argc, char *argv[])
{   
	int  rank, size, ret;
	int  fd_dest, fd_orig;
	char buffer [BLOCKSIZE];
	char path [1024];


	if ( argc < 2 )
	{
		printf("Please, enter final file path\n");
		return -1;
	}


	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);


	//Open origin file
	sprintf(path, "/local_test/test%d/preload_test.txt", rank);
	fd_orig = open(path, O_RDONLY);
	if (fd_orig == -1)
	{
		printf("Error on open operation\n");
		return -1;
	}


	//Create new file
	fd_dest = open(argv[1], O_WRONLY | O_CREAT, 0755);
	if (fd_dest == -1)
	{
		printf("Error on open operation\n");
		return -1;
	}


	int cont = BLOCKSIZE * rank;
	int read_bytes, write_bytes;

	do{
		read_bytes = read(fd_orig, &buffer, BLOCKSIZE);
		if(read_bytes == -1){
			printf("Error on read operation\n");
			return -1;
		}

		if (read_bytes > 0)
		{
			lseek (fd_dest, cont, SEEK_SET);

			write_bytes = write(fd_dest, &buffer, read_bytes);
			if(write_bytes==-1){
				printf("Error on read operation\n");
				return -1;
			}
		}

		cont = cont + (BLOCKSIZE * size);

	}while(read_bytes == BLOCKSIZE);

	close(fd_orig);
	close(fd_dest);

	MPI_Finalize();

	return 0;
}