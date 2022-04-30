#include <stdio.h>
#include "mpi.h"

#define MPISERVER_FINALIZE 16


int main(int argc, char *argv[])
{   
  int rank, nservers, ret, buf;
  char port_name[MPI_MAX_PORT_NAME];
  char srv_name[1024] ;
  MPI_Comm server;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  
  // Lookup port name
  sprintf(srv_name, "mpiServer.%d", 0) ;
  ret = MPI_Lookup_name(srv_name, MPI_INFO_NULL, port_name) ;
  if (MPI_SUCCESS != ret) {
      printf("MPI_Lookup_name fails\n") ;
      return -1 ;
  }
  // Connect with servers
  ret = MPI_Comm_connect( port_name, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &server );
  if (MPI_SUCCESS != ret) {
      printf("MPI_Comm_connect fails\n") ;
      return -1 ;
  }

  //Get number of servers
  MPI_Comm_remote_size(server, &nservers);

  //Finalize all servers
  MPI_Barrier(MPI_COMM_WORLD);

  if (rank == 0)
  {
    for (int i = 0; i < nservers; i++) { 
      buf = MPISERVER_FINALIZE; 
      MPI_Send( &buf, 1, MPI_INT, i, 0, server );
    }
  }
  
  MPI_Comm_disconnect( &server );
  MPI_Finalize();

  return 0;
}