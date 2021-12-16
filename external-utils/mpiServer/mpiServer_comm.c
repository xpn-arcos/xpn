
#include "mpiServer_comm.h"


int     mpiServer_comm_init      ( mpiServer_param_st *params )
{
	int  ret ;

        debug_printf("[COMM] begin mpiServer_comm_init(...)\n") ;

	ret = MPI_Init(&argc, &argv) ;
        ret = MPI_Comm_size(MPI_COMM_WORLD, &(params->size)) ;
        ret = MPI_Comm_rank(MPI_COMM_WORLD, &(params->rank)) ;
 	ret = MPI_Open_port(MPI_INFO_NULL,  &(params->port_name)) ;

        debug_printf("[COMM] server %d available at %s\n", params->rank, params->port_name) ;
        debug_printf("[COMM] server %d  accepting...\n",   params->rank) ;
        debug_printf("[COMM] end mpiServer_comm_init(...)\n") ;

	return 0 ;
}

void mpiServer_close_comm ( void )
{
	// TODO:
        // int mpiServer_close_comm( int fd ) ; <- add one argument
	// MPI_Comm_disconnect((MPI_Comm *)&fd) ; <- call disconnect...
	// TODO
}

int mpiServer_accept_comm ( void )
{
	int ret ;
	MPI_Comm client ;

        debug_printf("[COMM] begin mpiServer_accept_comm()\n") ;
	ret = MPI_Comm_accept(port_name, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &client);
        debug_printf("[COMM] end mpiServer_accept_comm()\n") ;

	return (int)client ;
}

int mpiServer_destroy_comm()
{
	// TODO:
        // int mpiServer_destroy_comm( *params ) ; <- add one argument
	// MPI_...close_port(...) ; <- call disconnect...
	return 0;
}

ssize_t mpiServer_comm_writedata ( int fd, char *data, ssize_t size, char *id )
{
	int ret ;

	debug_printf("[COMM] server: begin comm_writedata(...)\n") ;

	if (size == 0){
	    return 0;
	}
	if (size < 0){
	    return -1;
	}
	
        ret = MPI_Send(data, size, MPI_CHAR, 0, 0, (MPI_Comm)fd) ;

	debug_printf("[COMM] server: end comm_writedata(...)\n") ;

	return size;
}

ssize_t mpiServer_comm_readdata(int fd, char *data, ssize_t size, char *id)
{
	int ret ;
	MPI_Status status ;

	debug_printf("[COMM] server: begin comm_readdata(...)\n") ;

	if (size == 0) {
	    return  0;
	}
	if (size < 0){
	    return  -1;
	}
	
	ret = MPI_Recv(data, size, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, (MPI_Comm)fd, &status);

	debug_printf("[COMM] server: end comm_readdata(...)\n") ;

	return size;
}

