
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

#include "xpn_server_comm.h"
#include "socket.h"

#ifdef ENABLE_MPI_SERVER
#include "mpi_server_comm.h"
#endif
#ifdef ENABLE_MPI_SERVER
#include "sck_server_comm.h"
#endif

/* ... Const / Const ................................................. */


/* ... Global variables / Variables globales ........................ */


/* ... Functions / Funciones ......................................... */

// init, destroy
int xpn_server_comm_init ( xpn_server_param_st *params )
{
  int ret = -1;

  switch (params->server_type)
  {
  #ifdef ENABLE_MPI_SERVER
  case XPN_SERVER_TYPE_MPI:
    ret = mpi_server_comm_init( params->argc, params->argv, params->thread_mode_connections, params->port_name );
    break;
  #endif
  #ifdef ENABLE_SCK_SERVER
  case XPN_SERVER_TYPE_SCK:
    ret = sck_server_comm_init( &params->server_socket, params->port_name );
    break;
  #endif
  
  default:        
    printf("[XPN_SERVER] [xpn_server_comm_init] server_type '%d' not recognized\n",params->server_type);
    break;
  }

  return ret;
}

int xpn_server_comm_destroy ( xpn_server_param_st *params )
{  
  int ret = -1;

  switch (params->server_type)
  {
  #ifdef ENABLE_MPI_SERVER
  case XPN_SERVER_TYPE_MPI:
    ret = mpi_server_comm_destroy( params->port_name );
    break;
  #endif
  #ifdef ENABLE_SCK_SERVER
  case XPN_SERVER_TYPE_SCK:
    ret = socket_close( params->server_socket );
    break;
  #endif
  
  default:
    printf("[XPN_SERVER] [xpn_server_comm_destroy] server_type '%d' not recognized\n",params->server_type);
    break;
  }

  return ret;
}

int xpn_server_comm_accept ( xpn_server_param_st *params, void **new_sd )
{
  int ret = -1;

  switch (params->server_type)
  {
  #ifdef ENABLE_MPI_SERVER
  case XPN_SERVER_TYPE_MPI:
    ret = mpi_server_comm_accept( params->port_name, (MPI_Comm **)new_sd );
    break;
  #endif
  #ifdef ENABLE_SCK_SERVER
  case XPN_SERVER_TYPE_SCK:
    ret = sck_server_comm_accept( params->server_socket, (int **)new_sd );
    break;
  #endif
  
  default:
    printf("[XPN_SERVER] [xpn_server_comm_accept] server_type '%d' not recognized\n",params->server_type);
    break;
  }

  return ret;
}

int xpn_server_comm_disconnect ( xpn_server_param_st *params, void *sd )
{
  int ret = -1;

  switch (params->server_type)
  {
  #ifdef ENABLE_MPI_SERVER
  case XPN_SERVER_TYPE_MPI:
    ret = mpi_server_comm_disconnect( (MPI_Comm *)sd );
    break;
  #endif
  #ifdef ENABLE_SCK_SERVER
  case XPN_SERVER_TYPE_SCK:
    ret = sck_server_comm_disconnect( (int *)sd );
    break;
  #endif
  
  default:
    printf("[XPN_SERVER] [xpn_server_comm_disconnect] server_type '%d' not recognized\n",params->server_type);
    break;
  }

  return ret;
}

ssize_t xpn_server_comm_read_operation ( xpn_server_param_st *params, void *sd, int *op, int *rank_client_id, int *tag_client_id )
{  
  ssize_t ret = -1;

  switch (params->server_type)
  {
  #ifdef ENABLE_MPI_SERVER
  case XPN_SERVER_TYPE_MPI:
    ret = mpi_server_comm_read_operation( (MPI_Comm *)sd, op, rank_client_id, tag_client_id );
    break;
  #endif
  #ifdef ENABLE_SCK_SERVER
  case XPN_SERVER_TYPE_SCK:
    ret = socket_recv(*(int*)sd, op, sizeof(*op));
    break;
  #endif
  
  default:
    printf("[XPN_SERVER] [xpn_server_comm_read_operation] server_type '%d' not recognized\n",params->server_type);
    break;
  }

  return ret;
}

ssize_t xpn_server_comm_write_data ( xpn_server_param_st *params, void *sd, char *data, ssize_t size, int rank_client_id, int tag_client_id )
{  
  ssize_t ret = -1;

  switch (params->server_type)
  {
  #ifdef ENABLE_MPI_SERVER
  case XPN_SERVER_TYPE_MPI:
    ret = mpi_server_comm_write_data( (MPI_Comm *)sd, data, size, rank_client_id, tag_client_id );
    break;
  #endif
  #ifdef ENABLE_SCK_SERVER
  case XPN_SERVER_TYPE_SCK:
    ret = socket_send(*(int*)sd, data, size);
    break;
  #endif
  
  default:
    printf("[XPN_SERVER] [xpn_server_comm_write_data] server_type '%d' not recognized\n",params->server_type);
    break;
  }

  return ret;
}

ssize_t xpn_server_comm_read_data ( xpn_server_param_st *params, void *sd, char *data, ssize_t size, int rank_client_id, int tag_client_id )
{  
  ssize_t ret = -1;

  switch (params->server_type)
  {
  #ifdef ENABLE_MPI_SERVER
  case XPN_SERVER_TYPE_MPI:
    ret = mpi_server_comm_read_data( (MPI_Comm *)sd, data, size, rank_client_id, tag_client_id );
    break;
  #endif
  #ifdef ENABLE_SCK_SERVER
  case XPN_SERVER_TYPE_SCK:
    ret = socket_recv(*(int*)sd, data, size);
    break;
  #endif
  
  default:
    printf("[XPN_SERVER] [xpn_server_comm_read_data] server_type '%d' not recognized\n",params->server_type);
    break;
  }

  return ret;
}


/* ................................................................... */

