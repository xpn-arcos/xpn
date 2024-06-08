
/*
 *  Copyright 2020-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Dario Muñoz Muñoz
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

#include "mpi_server_comm.h"


/* ... Const / Const ................................................. */


/* ... Global variables / Variables globales ........................ */


/* ... Functions / Funciones ......................................... */

// init, destroy
int mpi_server_comm_init ( int argc, char *argv[], int thread_mode, char * port_name )
{
  int  ret, provided, claimed;
  int rank = 0;

  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_init] >> Begin\n", rank);

  //Get timestap
  struct timeval t0;
  TIME_MISC_Timer(&t0);

  // MPI init
  // Threads disable
  if (!thread_mode)
  {
    debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_init] MPI Init without threads\n", rank);

    ret = MPI_Init(&(argc), &(argv));
    if (MPI_SUCCESS != ret)
    {
      printf("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_init] ERROR: MPI_Init fails\n", rank);
      return -1;
    }
  }
  // Threads enable
  else
  {
    debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_init] MPI Init with threads\n", rank);

    ret = MPI_Init_thread(&(argc), &(argv), MPI_THREAD_MULTIPLE, &provided);
    if (MPI_SUCCESS != ret)
    {
      printf("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_init] ERROR: MPI_Init_thread fails\n", rank);
      return -1;
    }

    MPI_Query_thread(&claimed);
    if (claimed == MPI_THREAD_SINGLE) {
      printf("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_init] INFO: your MPI implementation seem not supporting thereads\n", rank);
    }
  }

  // rank = comm_rank()
  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_init] Get MPI rank\n", rank);

  ret = MPI_Comm_rank(MPI_COMM_WORLD, &(rank));
  if (MPI_SUCCESS != ret)
  {
    printf("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_init] ERROR: MPI_Comm_rank fails\n", rank);
    return -1;
  }

  // Open server port...
  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_init] Open port\n", rank);

  ret = MPI_Open_port(MPI_INFO_NULL, port_name);
  if (MPI_SUCCESS != ret) {
    printf("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_init] ERROR: MPI_Open_port fails\n", rank);
    return -1;
  }

  // Print server init information
  MPI_Barrier(MPI_COMM_WORLD);

  struct timeval t1;
  struct timeval tf;
  float time;
  TIME_MISC_Timer(&t1);
  TIME_MISC_DiffTime(&t0, &t1, &tf);
  time = TIME_MISC_TimevaltoFloat(&tf);

  MPI_Comm_set_errhandler(MPI_COMM_WORLD, MPI_ERRORS_RETURN);

  if (rank == 0)
  {
    printf("\n\n");
    printf("Time to inizialize all servers: %f s\n", time);
    printf("\n");
    printf("---------------------------\n");
    printf("All XPN MPI servers running\n");
    printf("---------------------------\n");
    printf("\n\n");
  }

  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_init] server %d available at %s\n", rank, rank, port_name);
  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_init] server %d accepting...\n",    rank, rank);

  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_init] >> End\n", rank);

  return 1;
}

int mpi_server_comm_destroy ( char * port_name )
{
  int ret;

  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_destroy] >> Begin\n", 0);

  // Close port
  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_destroy] Close port\n", 0);

  MPI_Close_port(port_name);

  // Finalize
  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_destroy] MPI Finalize\n", 0);

  ret = MPI_Finalize();
  if (MPI_SUCCESS != ret)
  {
    printf("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_destroy] ERROR: MPI_Finalize fails\n", 0);
    return -1;
  }

  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_destroy] << End\n", 0);

  // Return OK
  return 1;
}

// accept, disconnect
int mpi_server_comm_accept ( char * port_name, MPI_Comm **new_sd )
{
  int ret;

  *new_sd = malloc(sizeof(MPI_Comm));
  if (*new_sd == NULL) {
    printf("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_accept] ERROR: Memory allocation\n", 0);
    return -1;
  }

  **new_sd = MPI_COMM_NULL;

  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_accept] >> Begin\n", 0);

  // Accept
  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_accept] Accept %ld\n", 0, **new_sd);

  ret = MPI_Comm_accept(port_name, MPI_INFO_NULL, 0, MPI_COMM_SELF, *new_sd);
  if (MPI_SUCCESS != ret)
  {
    printf("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_accept] ERROR: MPI_Comm_accept fails\n", 0);
    return -1;
  }

  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_accept] << End %ld\n", 0, **new_sd);

  return 0;
}

int mpi_server_comm_disconnect ( MPI_Comm *fd )
{
  int ret;

  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_disconnect] >> Begin\n", 0);

  if (*fd == MPI_COMM_NULL)
  {
    printf("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_disconnect] ERROR: The MPI_Comm is NULL\n", 0);
    return 1;
  }

  // Disconnect
  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_disconnect] Disconnect\n", 0);

  ret = MPI_Comm_disconnect(fd);
  if (MPI_SUCCESS != ret)
  {
    printf("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_disconnect] ERROR: MPI_Comm_disconnect fails\n", 0);
    return -1;
  }
  free(fd);
  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_disconnect] << End\n", 0);

  // Return OK
  return 1;
}

ssize_t mpi_server_comm_read_operation ( MPI_Comm *fd, int *op, int *rank_client_id, int *tag_client_id )
{
  int ret;
  MPI_Status status;
  int msg[2];

  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_read_operation] >> Begin\n", 0);

  // Get message
  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_read_operation] Read operation %p\n", 0, fd);

  ret = MPI_Recv(msg, 2, MPI_INT, MPI_ANY_SOURCE, 0, *fd, &status);
  if (MPI_SUCCESS != ret) {
    debug_warning("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_read_operation] ERROR: MPI_Recv fails\n", 0);
  }

  *rank_client_id = status.MPI_SOURCE;
  *tag_client_id  = msg[0];
  *op             = msg[1];

  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_read_operation] MPI_Recv (MPI SOURCE %d, MPI_TAG %d, OP %d, MPI_ERROR %d)\n", 0, *rank_client_id, *rank_client_id, *op, status.MPI_ERROR);
  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_read_operation] << End\n", 0);

  // Return OK
  return 0;
}


ssize_t mpi_server_comm_write_data ( MPI_Comm *fd, char *data, ssize_t size, int rank_client_id, int tag_client_id )
{
  int ret;

  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_write_data] >> Begin\n", 0);

  if (size == 0) {
      return 0;
  }
  if (size < 0)
  {
    printf("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_write_data] ERROR: size < 0\n", 0);
    return -1;
  }

  // Send message
  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_write_data] Write data tag %d\n", 0, tag_client_id);

  ret = MPI_Send(data, size, MPI_CHAR, rank_client_id, tag_client_id, *fd);
  if (MPI_SUCCESS != ret) {
    debug_warning("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_write_data] ERROR: MPI_Send fails\n", 0);
  }

  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_write_data] << End\n", 0);

  // Return bytes written
  return size;
}

ssize_t mpi_server_comm_read_data ( MPI_Comm *fd, char *data, ssize_t size, int rank_client_id, int tag_client_id )
{
  int ret;
  MPI_Status status;

  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_read_data] >> Begin\n", 0);

  if (size == 0) {
    return  0;
  }
  if (size < 0)
  {
    printf("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_read_data] ERROR: size < 0\n", 0);
    return  -1;
  }

  // Get message
  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_read_data] Read data tag %d\n", 0, tag_client_id);

  ret = MPI_Recv(data, size, MPI_CHAR, rank_client_id, tag_client_id, *fd, &status);
  if (MPI_SUCCESS != ret) {
    debug_warning("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_read_data] ERROR: MPI_Recv fails\n", 0);
  }

  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_read_data] MPI_Recv (MPI SOURCE %d, MPI_TAG %d, MPI_ERROR %d)\n", 0, status.MPI_SOURCE, status.MPI_TAG, status.MPI_ERROR);
  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_read_data] << End\n", 0);

  // Return bytes read
  return size;
}


/* ................................................................... */

