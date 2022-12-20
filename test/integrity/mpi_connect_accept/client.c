#include "mpi.h"
#include <string.h>

int main( int argc, char **argv )
{
    MPI_Comm server;
    double buf[1024];
    char port_name[MPI_MAX_PORT_NAME];

    MPI_Init( &argc, &argv );
    strcpy(port_name, argv[1] );/* assume server's name is cmd-line arg */

    MPI_Comm_connect( port_name, MPI_INFO_NULL, 0, MPI_COMM_WORLD,  &server );

    MPI_Send( buf, 1, MPI_DOUBLE, 0, 2, server );
    MPI_Send( buf, 0, MPI_DOUBLE, 0, 1, server );

    MPI_Comm_disconnect( &server );

    MPI_Finalize();
    return 0;
}

