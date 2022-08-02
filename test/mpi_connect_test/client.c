
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <mpi.h>

#define SERVICE_NAME "test.1"

int main(int argc, char **argv)
{
	int  ret ;
	int  provided, flag, claimed;
	int  initial_rank, initial_size ;
	char port_name[MPI_MAX_PORT_NAME] ;
	MPI_Comm inter_comm ;


	setbuf(stdout,NULL);
	printf(" >> Client\n") ;

	printf(" >> before MPI_Init\n") ;
	MPI_Initialized(&flag);
	if (!flag)
	{
		ret = MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
		if (MPI_SUCCESS != ret)
		{
	            printf(" >> MPI_Init_thread: ERROR\n") ;
		    return -1;
		}
	}

	MPI_Query_thread(&claimed);
	if (claimed != MPI_THREAD_MULTIPLE) {
	    printf(" >> MPI_Query_thread: MPI_THREAD_MULTIPLE not supported\n");
	}
	printf(" >> after  MPI_Init\n") ;

	MPI_Comm_rank(MPI_COMM_SELF, &initial_rank);
	MPI_Comm_size(MPI_COMM_SELF, &initial_size);

	// Lookup...
	if (initial_rank == 0)
	{
	        printf(" >> before MPI_Lookup_name\n") ;
	        memset(port_name, 0, MPI_MAX_PORT_NAME) ;
		ret = MPI_Lookup_name(SERVICE_NAME, MPI_INFO_NULL, port_name);
		if (MPI_SUCCESS != ret) {
	            printf(" >> MPI_Lookup_name: ERROR\n") ;
		    return -1;
		}
	        printf(" >> after  MPI_Lookup_name('%s') -> %d\n", port_name, ret) ;
	}

	// Connect...
	printf(" >> before MPI_Comm_connect\n") ;
	ret = MPI_Comm_connect(port_name, MPI_INFO_NULL, 0, MPI_COMM_SELF, &inter_comm);
	if (MPI_SUCCESS != ret) {
	    printf(" >> MPI_Comm_connect: ERROR\n") ;
	    return -1;
	}
	printf(" >> after  MPI_Comm_connect\n") ;

	// Send...
	int data = 0;
	ret = MPI_Send(&data, 1, MPI_INT, 0, 0, inter_comm);
	if (MPI_SUCCESS != ret) {
	    printf(" >> MPI_Send: ERROR\n") ;
	    return -1;
	}

	// Disconnect...
	printf(" >> before MPI_Comm_disconnect\n") ;
	ret = MPI_Comm_disconnect(&inter_comm);
	if (MPI_SUCCESS != ret) {
	    printf(" >> MPI_Comm_disconnect: ERROR\n") ;
	    return -1;
	}
	printf(" >> after  MPI_Comm_disconnect\n") ;

	// Finalize...
	printf(" >> before MPI_Finalize\n") ;
	MPI_Finalize();
	printf(" >> after  MPI_Finalize\n") ;
	return 0;
}

