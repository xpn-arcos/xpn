
/*
 *  Copyright 2020-2025 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Dario Muñoz Muñoz
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

#include "all_system.h"
#include "base/socket.h"
#include "base/utils.h"
#include "base/workers.h"
#include "xpn_server_comm.h"
#include "xpn_server_ops.h"
#include "xpn_server_params.h"


/* ... Global variables / Variables globales ........................ */

char serv_name[HOST_NAME_MAX];
xpn_server_param_st params;
worker_t worker1, worker2;
int the_end = 0;

/* ... Auxiliar Functions / Funciones Auxiliares ..................... */

void xpn_server_run ( struct st_th th )
{
    debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_run] >> Begin: OP '%s'; OP_ID %d\n", th.id, xpn_server_op2string(th.type_op), th.type_op);

    xpn_server_do_operation(&th, &the_end);

    debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_run] << End: OP:'%s'\n", th.id, xpn_server_op2string(th.type_op));
}

void xpn_server_dispatcher ( struct st_th th )
{
    int ret;

    debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_dispatcher] >> Begin\n", th.id);

    // check params...
    if (NULL == th.params) {
        debug_error("[TH_ID=%d] [XPN_SERVER] [xpn_server_dispatcher] ERROR: NULL arguments\n", th.id);
        return;
    }

    struct st_th th_arg;
    int disconnect = 0;
    while (!disconnect)
    {
        debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_dispatcher] Waiting for operation\n", th.id);

        ret = xpn_server_comm_read_operation((xpn_server_param_st *)th.params, th.comm, &(th.type_op),
                                             &(th.rank_client_id), &(th.tag_client_id));
        if (ret < 0) {
            debug_error("[TH_ID=%d] [XPN_SERVER] [xpn_server_dispatcher] ERROR: read operation fail\n", th.id);
            return;
        }

        debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_dispatcher] OP '%s'; OP_ID %d\n", th.id,
                   xpn_server_op2string(th.type_op), th.type_op);

        if (th.type_op == XPN_SERVER_DISCONNECT) {
            debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_dispatcher] DISCONNECT received\n", th.id);

            disconnect = 1;
            continue;
        }

        if (th.type_op == XPN_SERVER_FINALIZE) {
            debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_dispatcher] FINALIZE received\n", th.id);

            disconnect = 1;
            the_end = 1;
            continue;
        }

        // Launch worker per operation
        th_arg.params = &params;
        th_arg.comm = th.comm;
        th_arg.function = xpn_server_run;
        th_arg.type_op = th.type_op;
        th_arg.rank_client_id = th.rank_client_id;
        th_arg.tag_client_id = th.tag_client_id;
        th_arg.wait4me = FALSE;

        base_workers_launch(&worker2, &th_arg, xpn_server_run);

        debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_dispatcher] Worker launched\n", th.id);
    }

    debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_dispatcher] Client %d close\n", th.id, th.rank_client_id);

    xpn_server_comm_disconnect(th.params, th.comm);

    debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_dispatcher] End\n", th.id);
}

void xpn_server_accept ( void )
{
    debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_up] Start accepting\n", 0);
    int ret;
    void *comm = NULL;
    struct st_th th_arg;
    ret = xpn_server_comm_accept(&params, &comm);
    if (ret < 0) {
        return;
    }

    debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_up] Accept received\n", 0);

    // Launch dispatcher per aplication
    th_arg.params = &params;
    th_arg.comm = comm;
    th_arg.function = xpn_server_dispatcher;
    th_arg.type_op = 0;
    th_arg.rank_client_id = 0;
    th_arg.tag_client_id = 0;
    th_arg.wait4me = FALSE;

    base_workers_launch(&worker1, &th_arg, xpn_server_dispatcher);
}

void xpn_server_finish ( void )
{
    // Wait and finalize for all current workers
    debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_up] Workers destroy\n", 0);

    base_workers_destroy(&worker1);
    base_workers_destroy(&worker2);

    debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_up] mpi_comm destroy\n", 0);

    xpn_server_comm_destroy(&params);
}

/* ... Functions / Funciones ......................................... */

// Start servers
int xpn_server_up ( void )
{
    int ret;
    int server_socket;
    int connection_socket;
    int recv_code = 0;
    int await_stop = 0;

    debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_up] >> Begin\n", 0);

    /*
    printf("\n");
    printf(" ----------------\n");
    printf(" Starting servers (%s)\n", serv_name);
    printf(" ----------------\n");
    printf("\n");
    */

    // Initialize server
    // * mpi_comm initialization
    debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_up] mpi_comm initialization\n", 0);

    ret = xpn_server_comm_init(&params);
    if (ret < 0) {
        printf("[TH_ID=%d] [XPN_SERVER] [xpn_server_up] ERROR: mpi_comm initialization fails\n", 0);
        return -1;
    }

    // * Workers initialization
    debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_up] Workers initialization\n", 0);

    ret = base_workers_init(&worker1, params.thread_mode_connections);
    if (ret < 0) {
        printf("[TH_ID=%d] [XPN_SERVER] [xpn_server_up] ERROR: Workers initialization fails\n", 0);
        return -1;
    }

    ret = base_workers_init(&worker2, params.thread_mode_operations);
    if (ret < 0) {
        printf("[TH_ID=%d] [XPN_SERVER] [xpn_server_up] ERROR: Workers initialization fails\n", 0);
        return -1;
    }

    debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_up] Control socket initialization\n", 0);
    ret = socket_server_create(&server_socket);
    if (ret < 0) {
        printf("[TH_ID=%d] [XPN_SERVER] [xpn_server_up] ERROR: Socket initialization fails\n", 0);
        return -1;
    }

    the_end = 0;
    while (!the_end)
    {
        debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_up] Listening to conections\n", 0);
        ret = socket_server_accept(server_socket, &connection_socket);
        if (ret < 0) continue;

        ret = socket_recv(connection_socket, &recv_code, sizeof(recv_code));
        if (ret < 0) continue;

        debug_info("[TH_ID=%d] [XPN_SERVER %s] [xpn_server_up] socket recv: %d \n", 0, params.srv_name, recv_code);
        switch (recv_code)
        {
            case SOCKET_ACCEPT_CODE:
                 socket_send(connection_socket, params.port_name, MPI_MAX_PORT_NAME);
                 xpn_server_accept();
                 break;

            case SOCKET_FINISH_CODE:
                 the_end = 1;
                 xpn_server_finish();
                 break;

            case SOCKET_FINISH_CODE_AWAIT:
                 the_end = 1;
                 await_stop = 1;
                 xpn_server_finish();
                 break;

            default:
                 debug_info("[TH_ID=%d] [XPN_SERVER %s] [xpn_server_up] >> Socket recv unknown code %d\n", 0,
                            params.srv_name, recv_code);
                 break;
        }

        if (await_stop == 0){
            socket_close(connection_socket);
        }
    }

    socket_close(server_socket);

    if (await_stop == 1){
        socket_send(connection_socket, &recv_code, sizeof(recv_code));
        socket_close(connection_socket);
    }

    debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_up] >> End\n", 0);
    return 0;
}

// Stop servers
int xpn_server_down ( void )
{
    char srv_name[1024];
    FILE *file;
    int ret;
    int buffer = SOCKET_FINISH_CODE;
    if (params.await_stop == 1){
        buffer = SOCKET_FINISH_CODE_AWAIT;
    }

    debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_down] >> Begin\n", 0);

    /*
    printf("\n");
    printf(" ----------------\n");
    printf(" Stopping servers (%s)\n", serv_name);
    printf(" ----------------\n");
    printf("\n");
    */

    // Open host file
    debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_down] Open host file %s\n", 0, params.shutdown_file);

    file = fopen(params.shutdown_file, "r");
    if (file == NULL) {
        printf("[TH_ID=%d] [XPN_SERVER] [xpn_server_down] ERROR: invalid file %s\n", 0, params.shutdown_file);
        return -1;
    }

    int num_serv = 0;
    while (fscanf(file, "%[^\n] ", srv_name) != EOF)
    {
        num_serv ++;
    }

    rewind(file);
    int *sockets = malloc(num_serv * sizeof(int));
    int i = 0;
    while (fscanf(file, "%[^\n] ", srv_name) != EOF)
    {
        printf(" * Stopping server (%s)\n", srv_name);
        ret = socket_client_connect(srv_name, &sockets[i]);
        if (ret < 0) {
            printf("[TH_ID=%d] [XPN_SERVER] [xpn_server_down] ERROR: socket connection %s\n", 0, srv_name);
            continue;
        }

        ret = socket_send(sockets[i], &buffer, sizeof(buffer));
        if (ret < 0) {
            printf("[TH_ID=%d] [XPN_SERVER] [xpn_server_down] ERROR: socket send %s\n", 0, srv_name);
        }
        
        if (params.await_stop == 0){
            socket_close(sockets[i]);
        }

        i++;
    }

    rewind(file);
    i = 0;
    while (fscanf(file, "%[^\n] ", srv_name) != EOF)
    {
        if (params.await_stop == 1){
            ret = socket_recv(sockets[i], &buffer, sizeof(buffer));
            if (ret < 0) {
                printf("[TH_ID=%d] [XPN_SERVER] [xpn_server_down] ERROR: socket recv %s\n", 0, srv_name);
            }
            socket_close(sockets[i]);
        }

        i++;
    }

    free(sockets);

    // Close host file
    debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_down] Close host file\n", 0);

    fclose(file);

    debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_up] >> End\n", 0);

    return 0;
}

int xpn_server_terminate ( void )
{
    int ret;
    int buffer = SOCKET_FINISH_CODE;
    int connection_socket;

    printf(" * Stopping server (%s)\n", params.srv_name);
    /*
    printf("\n");
    printf(" ----------------\n");
    printf(" Stopping server (%s)\n", params.srv_name);
    printf(" ----------------\n");
    printf("\n");
    */

    ret = socket_client_connect(params.srv_name, &connection_socket);
    if (ret < 0) {
        printf("[TH_ID=%d] [XPN_SERVER] [xpn_server_down] ERROR: socket connection %s\n", 0, params.srv_name);
        return -1 ;
    }

    ret = socket_send(connection_socket, &buffer, sizeof(buffer));
    if (ret < 0) {
        printf("[TH_ID=%d] [XPN_SERVER] [xpn_server_down] ERROR: socket send %s\n", 0, params.srv_name);
        return -1 ;
    }

    socket_close(connection_socket);
    return 0;
}

// Main
int main ( int argc, char *argv[] )
{
    int ret = -1;
    char *exec_name = NULL;

    // Initializing...
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    // Get arguments..
    debug_info("[TH_ID=%d] [XPN_SERVER] [main] Get server params\n", 0);

    ret = xpn_server_params_get(&params, argc, argv);
    if (ret < 0) {
        xpn_server_params_show_usage();
        return -1;
    }

    exec_name = basename(argv[0]);
    gethostname(serv_name, HOST_NAME_MAX);

    // Welcome...
    printf("\n");
    printf(" + xpn_server\n");
    printf(" | ----------\n");

    // Show configuration...
    printf(" | * action=%s\n", exec_name);
    printf(" | * host=%s\n", serv_name);
    xpn_server_params_show(&params);

    // Do associate action...
    if (strcasecmp(exec_name, "xpn_stop_server") == 0)
    {
        debug_info("[TH_ID=%d] [XPN_SERVER] [main] Down servers\n", 0);
        ret = xpn_server_down();
    }
    else if (strcasecmp(exec_name, "xpn_terminate_server") == 0)
    {
        debug_info("[TH_ID=%d] [XPN_SERVER] [main] Terminate server\n", 0);
        ret = xpn_server_terminate();
    }
    else
    {
        debug_info("[TH_ID=%d] [XPN_SERVER] [main] Up servers\n", 0);
        ret = xpn_server_up();
    }

    return ret;
}

/* ................................................................... */

