
#include <stdio.h>
#include "mpi.h"


int main( int argc, char **argv )
{
    MPI_Comm   client;
    MPI_Status status;
    char   port_name[MPI_MAX_PORT_NAME];
    double buf[1024];
    int    size, again;

    MPI_Init( &argc, &argv );
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Open_port(MPI_INFO_NULL, port_name);
    printf("%s\n",port_name);

    while (1)
    {
        MPI_Comm_accept( port_name, MPI_INFO_NULL, 0, MPI_COMM_WORLD,  &client );

        again = 1;
        while (again)
	{
            MPI_Recv( buf, 1024, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, client, &status );
            switch (status.MPI_TAG)
	    {
                case 0: MPI_Comm_free( &client );
                        MPI_Close_port(port_name);
                        MPI_Finalize();
                        return 0;

                case 1: MPI_Comm_disconnect( &client );
                        again = 0;
                        break;

                case 2: /* do something */
                        // ...

                default:
                        /* Unexpected message type */
                        MPI_Abort( MPI_COMM_WORLD, 1 );
            }
        }
    } // while

    return 0;
}

