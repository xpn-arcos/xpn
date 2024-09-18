
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

#include "base_cpp/xpn_socket.hpp"
#include "base_cpp/ns.hpp"
#include "nfi_mpi_server_comm.hpp"
#include "xpn_server/xpn_server_ops.hpp"
#include "base_cpp/xpn_env.hpp"

#include <chrono>
#include <thread>

namespace XPN
{

nfi_mpi_server_control_comm::nfi_mpi_server_control_comm() {
    int ret, provided, claimed;
    int flag = 0;
    int xpn_thread = xpn_env::get_instance().xpn_thread;

    debug_info("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_init] >> Begin\n");

    // MPI_Init
    MPI_Initialized(&flag);

    if (!flag) {
        // TODO: server->argc, server->argv from upper layers?

        // Threads disable
        if (!xpn_thread) {
            debug_info("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_init] MPI Init without threads\n");

            ret = MPI_Init(NULL, NULL);
            if (MPI_SUCCESS != ret) {
                printf("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_init] ERROR: MPI_Init fails\n");
            }
        }
        // Threads enable
        else {
            debug_info("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_init] MPI Init with threads\n");

            ret = MPI_Init_thread(NULL, NULL, MPI_THREAD_MULTIPLE, &provided);
            if (MPI_SUCCESS != ret) {
                printf("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_init] ERROR: MPI_Init_thread fails\n");
            }

            MPI_Query_thread(&claimed);
            if (claimed != MPI_THREAD_MULTIPLE) {
                printf(
                    "[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_init] INFO: your MPI implementation seem not supporting "
                    "thereads\n");
            }
        }
    }

    ret = MPI_Comm_rank(MPI_COMM_WORLD, &m_rank);
    if (MPI_SUCCESS != ret) {
        printf("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_init] ERROR: MPI_Comm_rank fails\n", m_rank);
    }

    ret = MPI_Comm_size(MPI_COMM_WORLD, &m_size);
    if (MPI_SUCCESS != ret) {
        printf("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_init] ERROR: MPI_Comm_rank fails\n", m_size);
    }

    // set is_mpi_server as the used protocol
    setenv("XPN_IS_MPI_SERVER", "1", 1);

    debug_info("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_init] >> End\n");
}

nfi_mpi_server_control_comm::~nfi_mpi_server_control_comm() {
    int ret;

    debug_info("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_destroy] >> Begin\n");

    MPI_Barrier(MPI_COMM_WORLD);

    // Finalize
    int flag = 0;
    MPI_Initialized(&flag);

    if (!flag) {
        debug_info("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_destroy] MPI Finalize\n");

        ret = PMPI_Finalize();
        if (MPI_SUCCESS != ret) {
            printf("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_destroy] ERROR: PMPI_Finalize fails\n");
        }
    }

    // Indicates mpi_server are the used protocolo
    unsetenv("XPN_IS_MPI_SERVER");

    debug_info("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_destroy] << End\n");
}

nfi_xpn_server_comm* nfi_mpi_server_control_comm::connect(const std::string &srv_name) {
    int ret, err;
    int connection_socket;
    int buffer = xpn_socket::ACCEPT_CODE;
    char port_name[MAX_PORT_NAME];
    MPI_Comm out_comm;

    debug_info("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_connect] >> Begin\n");

    int version_len;
    char version[MPI_MAX_LIBRARY_VERSION_STRING];
    MPI_Get_library_version(version, &version_len);

    debug_info("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_connect] MPI Version: %s\n", version);

    MPI_Comm_set_errhandler(MPI_COMM_WORLD, MPI_ERRORS_RETURN);

    // Send connect intention
    if (m_rank == 0) {
        err = 0;
        ret = xpn_socket::client_connect(srv_name, connection_socket);
        if (ret < 0) {
            // Do one retry in 1 second
            std::this_thread::sleep_for(std::chrono::seconds(1));
            ret = xpn_socket::client_connect(srv_name, connection_socket);
            if (ret < 0) {
                debug_error("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_connect] ERROR: socket connect\n");
                err = -1;
                goto mpi_comm_socket_finish;
            }
        }
        ret = xpn_socket::send(connection_socket, &buffer, sizeof(buffer));
        if (ret < 0) {
            debug_error("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_connect] ERROR: socket send\n");
            xpn_socket::close(connection_socket);
            err = -1;
            goto mpi_comm_socket_finish;
        }
        ret = xpn_socket::recv(connection_socket, port_name, MAX_PORT_NAME);
        if (ret < 0) {
            debug_error("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_connect] ERROR: socket read\n");
            xpn_socket::close(connection_socket);
            err = -1;
            goto mpi_comm_socket_finish;
        }
        xpn_socket::close(connection_socket);
        mpi_comm_socket_finish:
        debug_info("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_connect] Socket end, recv port: %s\n", port_name);
    }

    // Send port name to all ranks
    MPI_Bcast(&err, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (err == -1){
        return nullptr;
    }
    MPI_Bcast(port_name, MAX_PORT_NAME, MPI_CHAR, 0, MPI_COMM_WORLD);

    // Connect...
    debug_info("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_connect] Connect port %s\n", port_name);

    int connect_retries = 0;
    int errclass, resultlen;
    char err_buffer[MPI_MAX_ERROR_STRING];
    MPI_Info info;
    MPI_Info_create(&info);
    MPI_Info_set(info, "timeout", "1");
    do {
        ret = MPI_Comm_connect(port_name, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &out_comm);

        MPI_Error_class(ret, &errclass);
        MPI_Error_string(ret, err_buffer, &resultlen);

        if (MPI_SUCCESS != errclass) {
            XPN_DEBUG("%s", err_buffer);
            if (connect_retries == 0) {
                printf("----------------------------------------------------------------\n");
                printf("XPN Client %s : Waiting for servers being up and runing...\n", ns::get_host_name().c_str());
                printf("----------------------------------------------------------------\n\n");
            }
            connect_retries++;
            sleep(1);
        }
    } while (MPI_SUCCESS != ret && connect_retries < 1);
    MPI_Info_free(&info);

    if (MPI_SUCCESS != ret) {
        debug_error("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_connect] ERROR: MPI_Comm_connect fails\n");
        return nullptr;
    }

    debug_info("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_connect] << End\n");

    // Return OK
    return new nfi_mpi_server_comm(out_comm, m_rank, m_size);
}

void nfi_mpi_server_control_comm::disconnect(nfi_xpn_server_comm *comm) {
    int ret;

    nfi_mpi_server_comm *in_comm = static_cast<nfi_mpi_server_comm*>(comm);
    debug_info("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_disconnect] >> Begin\n");

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &(rank));
    if (rank == 0) {
        debug_info("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_disconnect] Send disconnect message\n");
        ret = in_comm->write_operation(XPN_SERVER_DISCONNECT);
        if (ret < 0) {
            printf("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_disconnect] ERROR: nfi_mpi_server_comm_write_operation fails\n");
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);

    // Disconnect
    debug_info("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_disconnect] Disconnect\n");

    ret = MPI_Comm_disconnect(&in_comm->m_comm);
    if (MPI_SUCCESS != ret) {
        printf("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_disconnect] ERROR: MPI_Comm_disconnect fails\n");
    }

    debug_info("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_disconnect] << End\n");
}

int64_t nfi_mpi_server_comm::write_operation(int op) {
    int ret;
    int msg[2];
    int eclass, len;
    char estring[MPI_MAX_ERROR_STRING];

    debug_info("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_write_operation] >> Begin\n");

    // Message generation
    msg[0] = (int)(pthread_self() % 32450) + 1;
    msg[1] = (int)op;

    // Send message
    debug_info("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_write_operation] Write operation send tag %d\n", msg[0]);

    ret = MPI_Send(msg, 2, MPI_INT, 0, 0, m_comm);
    if (MPI_SUCCESS != ret) {
        printf("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_write_operation] ERROR: MPI_Send < 0 : %d\n", ret);
        MPI_Error_class(ret, &eclass);
        MPI_Error_string(ret, estring, &len);
        printf("Error %d: %s\n", eclass, estring);
        fflush(stdout);
        return -1;
    }

    debug_info("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_write_operation] << End\n");

    // Return OK
    return 0;
}

int64_t nfi_mpi_server_comm::write_data(const void *data, int64_t size) {
    int ret;

    debug_info("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_write_data] >> Begin\n");

    // Check params
    if (size == 0) {
        return 0;
    }
    if (size < 0) {
        printf("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_write_data] ERROR: size < 0\n");
        return -1;
    }

    int tag = (int)(pthread_self() % 32450) + 1;

    // Send message
    debug_info("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_write_data] Write data tag %d\n", tag);

    ret = MPI_Send(data, size, MPI_CHAR, 0, tag, m_comm);
    if (MPI_SUCCESS != ret) {
        printf("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_write_data] ERROR: MPI_Send fails\n");
        size = 0;
    }

    debug_info("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_write_data] << End\n");

    // Return bytes written
    return size;
}

int64_t nfi_mpi_server_comm::read_data(void *data, ssize_t size) {
    int ret;
    MPI_Status status;

    debug_info("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_read_data] >> Begin\n");

    // Check params
    if (size == 0) {
        return 0;
    }
    if (size < 0) {
        printf("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_read_data] ERROR: size < 0\n");
        return -1;
    }

    int tag = (int)(pthread_self() % 32450) + 1;

    // Get message
    debug_info("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_read_data] Read data tag %d\n", tag);

    ret = MPI_Recv(data, size, MPI_CHAR, 0, tag, m_comm, &status);
    if (MPI_SUCCESS != ret) {
        printf("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_read_data] ERROR: MPI_Recv fails\n");
        size = 0;
    }

    debug_info("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_read_data] << End\n");

    // Return bytes read
    return size;
}

} // namespace XPN