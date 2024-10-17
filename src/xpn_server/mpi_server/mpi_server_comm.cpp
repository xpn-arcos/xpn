
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
#include "mpi_server_comm.hpp"
#include "base_cpp/debug.hpp"
#include "base_cpp/ns.hpp"

namespace XPN
{

mpi_server_control_comm::mpi_server_control_comm(xpn_server_params &params) : m_thread_mode(params.have_threads())
{
  int ret, provided, claimed;

  debug_info("[Server="<<ns::get_host_name()<<"] [MPI_SERVER_CONTROL_COMM] [mpi_server_control_comm_init] >> Begin");

  // MPI init
  // Threads disable
  if (!m_thread_mode)
  {
    debug_info("[Server="<<ns::get_host_name()<<"] [MPI_SERVER_CONTROL_COMM] [mpi_server_control_comm_init] MPI Init without threads");

    ret = MPI_Init(&(params.argc), &(params.argv));
    if (MPI_SUCCESS != ret)
    {
      debug_error("[Server="<<ns::get_host_name()<<"] [MPI_SERVER_CONTROL_COMM] [mpi_server_control_comm_init] ERROR: MPI_Init fails");
      return;
    }
  }
  // Threads enable
  else
  {
    debug_info("[Server="<<ns::get_host_name()<<"] [MPI_SERVER_CONTROL_COMM] [mpi_server_control_comm_init] MPI Init with threads");

    ret = MPI_Init_thread(&(params.argc), &(params.argv), MPI_THREAD_MULTIPLE, &provided);
    if (MPI_SUCCESS != ret)
    {
      debug_error("[Server="<<ns::get_host_name()<<"] [MPI_SERVER_CONTROL_COMM] [mpi_server_control_comm_init] ERROR: MPI_Init_thread fails");
      return;
    }

    MPI_Query_thread(&claimed);
    if (claimed == MPI_THREAD_SINGLE) {
      debug_error("[Server="<<ns::get_host_name()<<"] [MPI_SERVER_CONTROL_COMM] [mpi_server_control_comm_init] INFO: your MPI implementation seem not supporting thereads");
    }
  }

  debug_info("[Server="<<ns::get_host_name()<<"] [MPI_SERVER_CONTROL_COMM] [mpi_server_control_comm_init] Get MPI rank");

  ret = MPI_Comm_rank(MPI_COMM_WORLD, &(m_rank));
  if (MPI_SUCCESS != ret)
  {
    debug_error("[Server="<<ns::get_host_name()<<"] [MPI_SERVER_CONTROL_COMM] [mpi_server_control_comm_init] ERROR: MPI_Comm_rank fails");
    return;
  }

  ret = MPI_Comm_size(MPI_COMM_WORLD, &(m_size));
  if (MPI_SUCCESS != ret)
  {
    debug_error("[Server="<<ns::get_host_name()<<"] [MPI_SERVER_CONTROL_COMM] [mpi_server_control_comm_init] ERROR: MPI_Comm_rank fails");
    return;
  }
  // Open server port...
  debug_info("[Server="<<ns::get_host_name()<<"] [MPI_SERVER_CONTROL_COMM] [mpi_server_control_comm_init] Open port");

  ret = MPI_Open_port(MPI_INFO_NULL, m_port_name.data());
  if (MPI_SUCCESS != ret) {
    debug_error("[Server="<<ns::get_host_name()<<"] [MPI_SERVER_CONTROL_COMM] [mpi_server_control_comm_init] ERROR: MPI_Open_port fails");
    return;
  }

  // Print server init information
  MPI_Barrier(MPI_COMM_WORLD);

  MPI_Comm_set_errhandler(MPI_COMM_WORLD, MPI_ERRORS_RETURN);

  debug_info("[Server="<<ns::get_host_name()<<"] [MPI_SERVER_CONTROL_COMM] [mpi_server_control_comm_init] server "<<m_rank<<" available at"<< m_port_name);
  debug_info("[Server="<<ns::get_host_name()<<"] [MPI_SERVER_CONTROL_COMM] [mpi_server_control_comm_init] server "<<m_rank<<" accepting...");

  debug_info("[Server="<<ns::get_host_name()<<"] [MPI_SERVER_CONTROL_COMM] [mpi_server_control_comm_init] >> End");
}

mpi_server_control_comm::~mpi_server_control_comm() 
{
  int ret;

  debug_info("[Server="<<ns::get_host_name()<<"] [MPI_SERVER_CONTROL_COMM] [mpi_server_control_comm_destroy] >> Begin");

  // Close port
  debug_info("[Server="<<ns::get_host_name()<<"] [MPI_SERVER_CONTROL_COMM] [mpi_server_control_comm_destroy] Close port");

  MPI_Close_port(m_port_name.data());

  // Finalize
  debug_info("[Server="<<ns::get_host_name()<<"] [MPI_SERVER_CONTROL_COMM] [mpi_server_control_comm_destroy] MPI Finalize");

  ret = MPI_Finalize();
  if (MPI_SUCCESS != ret)
  {
    print("[Server="<<ns::get_host_name()<<"] [MPI_SERVER_CONTROL_COMM] [mpi_server_control_comm_destroy] ERROR: MPI_Finalize fails");
    return;
  }

  debug_info("[Server="<<ns::get_host_name()<<"] [MPI_SERVER_CONTROL_COMM] [mpi_server_control_comm_destroy] << End");
}

// accept, disconnect
xpn_server_comm* mpi_server_control_comm::accept ( )
{
  int ret;

  MPI_Comm comm;

  debug_info("[Server="<<ns::get_host_name()<<"] [MPI_SERVER_CONTROL_COMM] [mpi_server_control_comm_accept] >> Begin");

  // Accept
  debug_info("[Server="<<ns::get_host_name()<<"] [MPI_SERVER_CONTROL_COMM] [mpi_server_control_comm_accept] Accept");

  ret = MPI_Comm_accept(m_port_name.data(), MPI_INFO_NULL, 0, MPI_COMM_SELF, &comm);
  if (MPI_SUCCESS != ret)
  {
    print("[Server="<<ns::get_host_name()<<"] [MPI_SERVER_CONTROL_COMM] [mpi_server_control_comm_accept] ERROR: MPI_Comm_accept fails");
    exit(EXIT_FAILURE);
  }

  debug_info("[Server="<<ns::get_host_name()<<"] [MPI_SERVER_CONTROL_COMM] [mpi_server_control_comm_accept] << End");
  
  return new (std::nothrow) mpi_server_comm(comm);
}

void mpi_server_control_comm::disconnect ( xpn_server_comm *comm )
{
  int ret;

  mpi_server_comm *in_comm = static_cast<mpi_server_comm*>(comm);

  debug_info("[Server="<<ns::get_host_name()<<"] [MPI_SERVER_CONTROL_COMM] [mpi_server_control_comm_disconnect] >> Begin");

  if (in_comm->m_comm == MPI_COMM_NULL)
  {
    print("[Server="<<ns::get_host_name()<<"] [MPI_SERVER_CONTROL_COMM] [mpi_server_control_comm_disconnect] ERROR: The MPI_Comm is NULL");
    return;
  }

  // Disconnect
  debug_info("[Server="<<ns::get_host_name()<<"] [MPI_SERVER_CONTROL_COMM] [mpi_server_control_comm_disconnect] Disconnect");

  ret = MPI_Comm_disconnect(&in_comm->m_comm);
  if (MPI_SUCCESS != ret)
  {
    print("[Server="<<ns::get_host_name()<<"] [MPI_SERVER_CONTROL_COMM] [mpi_server_control_comm_disconnect] ERROR: MPI_Comm_disconnect fails");
    return;
  }

  delete comm;
  debug_info("[Server="<<ns::get_host_name()<<"] [MPI_SERVER_CONTROL_COMM] [mpi_server_control_comm_disconnect] << End");
}

int64_t mpi_server_comm::read_operation ( xpn_server_ops &op, int &rank_client_id, int &tag_client_id )
{
  int ret;
  MPI_Status status;
  int msg[2];

  debug_info("[Server="<<ns::get_host_name()<<"] [MPI_SERVER_COMM] [mpi_server_comm_read_operation] >> Begin");

  // Get message
  debug_info("[Server="<<ns::get_host_name()<<"] [MPI_SERVER_COMM] [mpi_server_comm_read_operation] Read operation");

  ret = MPI_Recv(msg, 2, MPI_INT, MPI_ANY_SOURCE, 0, m_comm, &status);
  if (MPI_SUCCESS != ret) {
    debug_warning("[Server="<<ns::get_host_name()<<"] [MPI_SERVER_COMM] [mpi_server_comm_read_operation] ERROR: MPI_Recv fails");
  }

  rank_client_id = status.MPI_SOURCE;
  tag_client_id  = msg[0];
  op             = static_cast<xpn_server_ops>(msg[1]);

  debug_info("[Server="<<ns::get_host_name()<<"] [MPI_SERVER_COMM] [mpi_server_comm_read_operation] MPI_Recv (MPI SOURCE "<<status.MPI_SOURCE<<", MPI_TAG "<<status.MPI_TAG<<", MPI_ERROR "<<status.MPI_ERROR<<")");
  debug_info("[Server="<<ns::get_host_name()<<"] [MPI_SERVER_COMM] [mpi_server_comm_read_operation] << End");

  // Return OK
  return 0;
}


int64_t mpi_server_comm::read_data ( void *data, int64_t size, int rank_client_id, int tag_client_id )
{
  int ret;
  MPI_Status status;

  debug_info("[Server="<<ns::get_host_name()<<"] [MPI_SERVER_COMM] [mpi_server_comm_read_data] >> Begin");

  if (size == 0) {
    return  0;
  }
  if (size < 0)
  {
    print("[Server="<<ns::get_host_name()<<"] [MPI_SERVER_COMM] [mpi_server_comm_read_data] ERROR: size < 0");
    return  -1;
  }

  // Get message
  debug_info("[Server="<<ns::get_host_name()<<"] [MPI_SERVER_COMM] [mpi_server_comm_read_data] Read data tag "<< tag_client_id);

  ret = MPI_Recv(data, size, MPI_CHAR, rank_client_id, tag_client_id, m_comm, &status);
  if (MPI_SUCCESS != ret) {
    debug_warning("[Server="<<ns::get_host_name()<<"] [MPI_SERVER_COMM] [mpi_server_comm_read_data] ERROR: MPI_Recv fails");
  }

  debug_info("[Server="<<ns::get_host_name()<<"] [MPI_SERVER_COMM] [mpi_server_comm_read_data] MPI_Recv (MPI SOURCE "<<status.MPI_SOURCE<<", MPI_TAG "<<status.MPI_TAG<<", MPI_ERROR "<<status.MPI_ERROR<<")");
  debug_info("[Server="<<ns::get_host_name()<<"] [MPI_SERVER_COMM] [mpi_server_comm_read_data] << End");

  // Return bytes read
  return size;
}

int64_t mpi_server_comm::write_data ( const void *data, int64_t size, int rank_client_id, int tag_client_id )
{
  int ret;

  debug_info("[Server="<<ns::get_host_name()<<"] [MPI_SERVER_COMM] [mpi_server_comm_write_data] >> Begin");

  if (size == 0) {
      return 0;
  }
  if (size < 0)
  {
    print("[Server="<<ns::get_host_name()<<"] [MPI_SERVER_COMM] [mpi_server_comm_write_data] ERROR: size < 0");
    return -1;
  }

  // Send message
  debug_info("[Server="<<ns::get_host_name()<<"] [MPI_SERVER_COMM] [mpi_server_comm_write_data] Write data tag "<< tag_client_id);

  ret = MPI_Send(data, size, MPI_CHAR, rank_client_id, tag_client_id, m_comm);
  if (MPI_SUCCESS != ret) {
    debug_warning("[Server="<<ns::get_host_name()<<"] [MPI_SERVER_COMM] [mpi_server_comm_write_data] ERROR: MPI_Send fails");
  }

  debug_info("[Server="<<ns::get_host_name()<<"] [MPI_SERVER_COMM] [mpi_server_comm_write_data] << End");

  // Return bytes written
  return size;
}

} // namespace XPN
