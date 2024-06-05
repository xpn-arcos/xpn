
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

#include "nfi_mpi_server_comm.h"

/* ... Const / Const ................................................. */

/* ... Global variables / Variables globales ........................ */

/* ... Functions / Funciones ......................................... */

int nfi_mpi_server_comm_init(int xpn_thread) {
    int ret, provided, claimed;
    int flag = 0;

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
                return -1;
            }
        }
        // Threads enable
        else {
            debug_info("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_init] MPI Init with threads\n");

            ret = MPI_Init_thread(NULL, NULL, MPI_THREAD_MULTIPLE, &provided);
            if (MPI_SUCCESS != ret) {
                printf("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_init] ERROR: MPI_Init_thread fails\n");
                return -1;
            }

            MPI_Query_thread(&claimed);
            if (claimed != MPI_THREAD_MULTIPLE) {
                printf(
                    "[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_init] INFO: your MPI implementation seem not supporting "
                    "thereads\n");
            }
        }
    }

    // set is_mpi_server as the used protocol
    setenv("XPN_IS_MPI_SERVER", "1", 1);

    debug_info("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_init] >> End\n");

    // Return OK
    return 1;
}

int nfi_mpi_server_comm_destroy() {
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
            return -1;
        }
    }

    // Indicates mpi_server are the used protocolo
    unsetenv("XPN_IS_MPI_SERVER");

    debug_info("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_destroy] << End\n");

    // Return OK
    return 1;
}

int nfi_mpi_server_comm_connect(char *srv_name, char *port_name, MPI_Comm *out_comm) {
    int ret, err;
    int connection_socket;
    int rank;

    debug_info("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_connect] >> Begin\n");

    int version_len;
    char version[MPI_MAX_LIBRARY_VERSION_STRING];
    MPI_Get_library_version(version, &version_len);

    debug_info("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_connect] MPI Version: %s\n", version);

    MPI_Comm_set_errhandler(MPI_COMM_WORLD, MPI_ERRORS_RETURN);

    ret = MPI_Comm_rank(MPI_COMM_WORLD, &(rank));
    if (MPI_SUCCESS != ret) {
        printf("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_init] ERROR: MPI_Comm_rank fails\n", rank);
        return -1;
    }

    // Check if spawn_servers
    char * env_spawn_servers = getenv("XPN_SERVER_SPAWN");
    if (env_spawn_servers != NULL)
    {
        struct stat st;
        ret = filesystem_stat(env_spawn_servers,&st);
        if (ret < 0){
            printf("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_init] ERROR: xpn_server_spawn not found in path: \"%s\"\n", rank, env_spawn_servers);
            return -1;
        }

        int spawnError[1];
        MPI_Info info;
        MPI_Info_create(&info);
        MPI_Info_set(info, "host", srv_name);
        // For openmpi is necesary the next info:
        // MPI_Info_set(info, "add-host", srv_name);
        // MPI_Info_set(info, "PMIX_HOST", srv_name);
        // MPI_Info_set(info, "PMIX_ADD_HOST", srv_name);
        debug_info("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_connect] Server spawn %s\n", srv_name);
        ret = MPI_Comm_spawn(env_spawn_servers, MPI_ARGV_NULL, 1, info, 0, MPI_COMM_WORLD, out_comm, spawnError);
        if (*out_comm==MPI_COMM_NULL){
            debug_error("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_connect] ERROR: Server spawn\n");
            return -1;
        }
        MPI_Info_free(&info);
    }else{
        // Send connect intention
        if (rank == 0) {
            err = 0;
            ret = socket_client_connect(srv_name, &connection_socket);
            if (ret < 0) {
                debug_error("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_connect] ERROR: socket connect\n");
                err = -1;
                goto mpi_comm_socket_finish;
            }
            int buffer = SOCKET_ACCEPT_CODE;
            ret = socket_send(connection_socket, &buffer, sizeof(buffer));
            if (ret < 0) {
                debug_error("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_connect] ERROR: socket send\n");
                socket_close(connection_socket);
                err = -1;
                goto mpi_comm_socket_finish;
            }
            ret = socket_recv(connection_socket, port_name, MPI_MAX_PORT_NAME);
            if (ret < 0) {
                debug_error("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_connect] ERROR: socket read\n");
                socket_close(connection_socket);
                err = -1;
                goto mpi_comm_socket_finish;
            }
            socket_close(connection_socket);
            mpi_comm_socket_finish:
            debug_info("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_connect] Socket end, recv port: %s\n", port_name);
        }

        // Send port name to all ranks
        MPI_Bcast(&err, 1, MPI_INT, 0, MPI_COMM_WORLD);
        if (err == -1){
            return -1;
        }
        MPI_Bcast(port_name, MPI_MAX_PORT_NAME, MPI_CHAR, 0, MPI_COMM_WORLD);

        // Connect...
        debug_info("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_connect] Connect port %s\n", port_name);

        int connect_retries = 0;
        int errclass, resultlen;
        char err_buffer[MPI_MAX_ERROR_STRING];
        MPI_Info info;
        MPI_Info_create(&info);
        MPI_Info_set(info, "timeout", "1");
        do {
            ret = MPI_Comm_connect(port_name, MPI_INFO_NULL, 0, MPI_COMM_WORLD, out_comm);

            MPI_Error_class(ret, &errclass);
            MPI_Error_string(ret, err_buffer, &resultlen);

            if (MPI_SUCCESS != errclass) {
                XPN_DEBUG("%s", err_buffer);
                if (connect_retries == 0) {
                    char cli_name[HOST_NAME_MAX];
                    gethostname(cli_name, HOST_NAME_MAX);
                    printf("----------------------------------------------------------------\n");
                    printf("XPN Client %s : Waiting for servers being up and runing...\n", cli_name);
                    printf("----------------------------------------------------------------\n\n");
                }
                connect_retries++;
                sleep(1);
            }
        } while (MPI_SUCCESS != ret && connect_retries < 1);
        MPI_Info_free(&info);
    }

    if (MPI_SUCCESS != ret) {
        debug_error("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_connect] ERROR: MPI_Comm_connect fails\n");
        return -1;
    }

    debug_info("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_connect] << End\n");

    // Return OK
    return 1;
}

int nfi_mpi_server_comm_disconnect(MPI_Comm *comm) {
    int ret;

    debug_info("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_disconnect] >> Begin\n");

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &(rank));
    if (rank == 0) {
        debug_info("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_disconnect] Send disconnect message\n");
        ret = nfi_mpi_server_comm_write_operation(*comm, XPN_SERVER_DISCONNECT);
        if (ret < 0) {
            printf("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_disconnect] ERROR: nfi_mpi_server_comm_write_operation fails\n");
            return -1;
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);

    // Disconnect
    debug_info("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_disconnect] Disconnect\n");

    ret = MPI_Comm_disconnect(comm);
    if (MPI_SUCCESS != ret) {
        printf("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_disconnect] ERROR: MPI_Comm_disconnect fails\n");
        return -1;
    }

    debug_info("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_disconnect] << End\n");

    // Return OK
    return 1;
}

ssize_t nfi_mpi_server_comm_write_operation(MPI_Comm fd, int op) {
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

    ret = MPI_Send(msg, 2, MPI_INT, 0, 0, fd);
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

ssize_t nfi_mpi_server_comm_write_data(MPI_Comm fd, char *data, ssize_t size) {
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

    ret = MPI_Send(data, size, MPI_CHAR, 0, tag, fd);
    if (MPI_SUCCESS != ret) {
        printf("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_write_data] ERROR: MPI_Send fails\n");
        size = 0;
    }

    debug_info("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_write_data] << End\n");

    // Return bytes written
    return size;
}

ssize_t nfi_mpi_server_comm_read_data(MPI_Comm fd, char *data, ssize_t size) {
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

    ret = MPI_Recv(data, size, MPI_CHAR, 0, tag, fd, &status);
    if (MPI_SUCCESS != ret) {
        printf("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_read_data] ERROR: MPI_Recv fails\n");
        size = 0;
    }

    debug_info("[NFI_MPI_SERVER_COMM] [nfi_mpi_server_comm_read_data] << End\n");

    // Return bytes read
    return size;
}

/* ................................................................... */
