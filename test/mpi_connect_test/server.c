
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>


int main ( int argc, char **argv )
{
	int provided, data, ret;
	int initial_rank, initial_size;
	char port_name[MPI_MAX_PORT_NAME];
	MPI_Comm inter_comm;
	MPI_Status status;
	MPI_Info info;


	printf(" >> Server\n") ;
	printf(" >> Hydra: %s\n", getenv("I_MPI_HYDRA_NAMESERVER")) ;

	printf(" >> before MPI_Init\n") ;
	//MPI_Init(&argc, &argv);
	MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);

	MPI_Comm_rank(MPI_COMM_SELF, &initial_rank);
	MPI_Comm_size(MPI_COMM_SELF, &initial_size);
	printf(" >> after  MPI_Init\n") ;

	// Open port...
	printf(" >> before MPI_Open_port\n") ;
	ret = MPI_Open_port(MPI_INFO_NULL, port_name);
	if (ret != MPI_SUCCESS){
	    printf(" >> MPI_Open_port: ERROR\n") ;
	    return -1;
	}
	printf(" >> after  MPI_Open_port\n") ;

	// Publish...
	printf(" >> before MPI_Publish_name\n") ;
	MPI_Info_create(&info);
	MPI_Info_set(info, "ompi_global_scope", "true");
	ret = MPI_Publish_name("mpiserver.1234", info, port_name);
	if (ret != MPI_SUCCESS){
	    printf(" >> MPI_Publish_name: ERROR\n") ;
	    return -1;
	}
	printf(" >> after  MPI_Publish_name (%s)\n", port_name) ;

	// Accept...
	printf(" >> before MPI_Comm_accept\n") ;
	ret = MPI_Comm_accept(port_name, MPI_INFO_NULL, 0, MPI_COMM_SELF, &inter_comm);
	if (MPI_SUCCESS != ret) {
	    printf(" >> MPI_Comm_accept: ERROR\n") ;
	    return -1;
	}
	printf(" >> after  MPI_Comm_accept\n") ;

	// Receive data...
	printf(" >> before MPI_Recv\n") ;
	ret = MPI_Recv(&data, 1, MPI_INT, MPI_ANY_SOURCE, 0, inter_comm, &status);
	if (MPI_SUCCESS != ret) {
	    printf(" >> MPI_Recv: ERROR\n") ;
	    return -1;
	}
	printf(" >> after  MPI_Recv\n") ;

	// Disconnect...
	printf(" >> before MPI_Comm_disconnect\n") ;
	ret = MPI_Comm_disconnect(&inter_comm);
	if (MPI_SUCCESS != ret) {
	    printf(" >> MPI_Comm_disconnect: ERROR\n") ;
	    return -1;
	}
	printf(" >> after  MPI_Comm_disconnect\n") ;

	// Close port...
	printf(" >> before MPI_Close_port\n") ;
	ret = MPI_Close_port(port_name);
	if (MPI_SUCCESS != ret) {
	    printf(" >> MPI_Close_port: ERROR\n") ;
	    return -1;
	}
	printf(" >> after  MPI_Close_port\n") ;

	// Unpublish...
	printf(" >> before MPI_Unpublish_name\n") ;
	ret = MPI_Unpublish_name("mpiserver.1234", info, port_name);
	if (MPI_SUCCESS != ret) {
	    printf(" >> MPI_Unpublish_name: ERROR\n") ;
	    return -1;
	}
	printf(" >> before MPI_Unpublish_name\n") ;

	// Finalize...
	printf(" >> before MPI_Finalize\n") ;
	MPI_Finalize();
	printf(" >> before MPI_Finalize\n") ;

	return 0;
}

