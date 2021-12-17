
#include "mpiServer_comm.h"


int     mpiServer_comm_init      ( mpiServer_param_st *params )
{
        int ret, claimed, provided ;
	char srv_name[1024] ;

        debug_info("[COMM] begin mpiServer_comm_init(...)\n") ;

        // MPI_Init
        ret = MPI_Init_thread(&(params->argc), &(params->argv), MPI_THREAD_MULTIPLE, &provided) ;
        if (MPI_SUCCESS != ret) {
            debug_error("Server[%d]: MPI_Init fails :-(", -1) ;
            return -1 ;
        }

        // params->rank = comm_rank()
        ret = MPI_Comm_rank(MPI_COMM_WORLD, &(params->rank)) ;
        if (MPI_SUCCESS != ret) {
            debug_error("Server[%d]: MPI_Comm_rank fails :-(", params->rank) ;
            return -1 ;
        }

        // params->size = comm_size()
        ret = MPI_Comm_size(MPI_COMM_WORLD, &(params->size)) ;
        if (MPI_SUCCESS != ret) {
            debug_error("Server[%d]: MPI_Comm_size fails :-(", params->rank) ;
            return -1 ;
        }

        // Open server port...
        ret = MPI_Open_port(MPI_INFO_NULL, &(params->port_name)) ;
        if (MPI_SUCCESS != ret) {
            debug_error("Server[%d]: MPI_Open_port fails :-(", params->rank) ;
            return -1 ;
        }

        // Publish port name
        sprintf(srv_name, "mpiServer.%d", params->rank) ;

        MPI_Info info ;
        MPI_Info_create(&info) ;
        MPI_Info_set(info, "ompi_global_scope", "true") ;

        ret = MPI_Publish_name(srv_name, info, params->port_name) ;
        if (MPI_SUCCESS != ret) {
            debug_error("Server[%d]: MPI_Publish_name fails :-(", params->rank) ;
            return -1 ;
        }

        debug_info("[COMM] server %d available at %s\n", params->rank, params->port_name) ;
        debug_info("[COMM] server %d  accepting...\n",   params->rank) ;
        debug_info("[COMM] end mpiServer_comm_init(...)\n") ;

	// Return OK
	return 0 ;
}

void mpiServer_close_comm ( mpiServer_param_st *params )
{
	// TODO:
        // int mpiServer_close_comm( int fd ) ; <- add one argument
	// MPI_Comm_disconnect((MPI_Comm *)&fd) ; <- call disconnect...
	// TODO
}

int mpiServer_accept_comm ( mpiServer_param_st *params )
{
	int ret ;
	MPI_Comm client ;

        debug_info("[COMM] begin mpiServer_accept_comm()\n") ;

        // Accept
        ret = MPI_Comm_accept(params->port_name, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &(params->client)) ;
        if (MPI_SUCCESS != ret) {
            debug_error("Server[%d]: MPI_Comm_accept fails :-(", params->rank) ;
            return -1 ;
        }

        debug_info("[COMM] end mpiServer_accept_comm()\n") ;

        // Return client MPI_Comm
	return (int)client ;
}

int mpiServer_destroy_comm ( mpiServer_param_st *params )
{
	// TODO:
        // int mpiServer_destroy_comm( *params ) ; <- add one argument
	// MPI_...close_port(...) ; <- call disconnect...
	return 0;
}

ssize_t mpiServer_comm_writedata ( mpiServer_param_st *params, int fd, char *data, ssize_t size, char *id )
{
	int ret ;

	debug_info("[COMM] server: begin comm_writedata(...)\n") ;

	// Check params
	if (size == 0){
	    return 0;
	}
	if (size < 0){
	    return -1;
	}
	
        // Send message
        ret = MPI_Send(data, size, MPI_CHAR, 0, 0, (MPI_Comm)fd) ;
        if (MPI_SUCCESS != ret) {
            debug_warning("Server[%d]: MPI_Recv fails :-(", params->rank) ;
        }

	debug_info("[COMM] server: end comm_writedata(...)\n") ;

        // Return bytes written
	return size;
}

ssize_t mpiServer_comm_readdata ( mpiServer_param_st *params, int fd, char *data, ssize_t size, char *id )
{
	int ret ;
	MPI_Status status ;

	debug_info("[COMM] server: begin comm_readdata(...)\n") ;

	// Check params
	if (size == 0) {
	    return  0;
	}
	if (size < 0){
	    return  -1;
	}
	
        // Get message
	ret = MPI_Recv(data, size, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, (MPI_Comm)fd, &status);
        if (MPI_SUCCESS != ret) {
            debug_warning("Server[%d]: MPI_Recv fails :-(", params->rank) ;
        }

	debug_info("[COMM] server: end comm_readdata(...)\n") ;

        // Return bytes read
	return size;
}

