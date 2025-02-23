
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
#include "base/ns.h"
#include "base/socket.h"
#include "base/service_socket.h"
#include "base/utils.h"
#include "base/workers.h"
#include "xpn_server_comm.h"
#include "xpn_server_ops.h"
#include "xpn_server_params.h"


/* ... Global variables / Variables globales ........................ */

   char serv_name[HOST_NAME_MAX];
   xpn_server_param_st params;
   worker_t worker1, worker2, worker3;
   int the_end = 0;


/* ... Auxiliar Functions / Funciones Auxiliares ..................... */

void xpn_server_run ( struct st_th th )
{
    debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_run] >> Begin: OP '%s'; OP_ID %d\n", th.id, xpn_server_op2string(th.type_op), th.type_op);

    xpn_server_do_operation(&th, &the_end);

    if (th.close4me) {
        debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_run] Client close\n", th.id);
        xpn_server_comm_disconnect(th.params, th.comm);
    }

    debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_run] << End: OP:'%s'\n", th.id, xpn_server_op2string(th.type_op));
}


void xpn_server_dispatcher_connectionless ( struct st_th th )
{
    int ret;
    struct st_th th_arg;
    void *comm = NULL;

    debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_dispatcher_connectionless] >> Begin\n", th.id);

    // check params...
    if (NULL == th.params)
    {
        debug_error("[TH_ID=%d] [XPN_SERVER] [xpn_server_dispatcher_connectionless] ERROR: NULL arguments\n", th.id);
        debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_dispatcher_connectionless] End\n", th.id);
        return;
    }

    while (1)
    {
        debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_dispatcher_connectionless] Waiting in accept\n", th.id);
        ret = xpn_server_comm_accept(th.params, &comm, XPN_SERVER_CONNECTIONLESS);
        if (ret < 0)
        {
            printf("[TH_ID=%d] [XPN_SERVER] [xpn_server_dispatcher_connectionless] ERROR: accept fails\n", th.id);
            continue;
        }

        th.comm = comm ;

        debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_dispatcher_connectionless] Waiting for operation\n", th.id);
        ret = xpn_server_comm_read_operation((xpn_server_param_st *)th.params,
			                     th.comm, &(th.type_op),
                                             &(th.rank_client_id), &(th.tag_client_id));
        if (ret < 0)
        {
            printf("[TH_ID=%d] [XPN_SERVER] [xpn_server_dispatcher_connectionless] ERROR: read operation fail\n", th.id);
            continue;
        }
        if (0 == ret)
        {
            debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_dispatcher_connectionless] WARN: read operation found EOF\n", th.id);
            xpn_server_comm_disconnect(th.params, th.comm);
            continue;
        }

        debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_dispatcher_connectionless] OP '%s'; OP_ID %d\n", th.id, xpn_server_op2string(th.type_op), th.type_op);

        if (th.type_op == XPN_SERVER_DISCONNECT)
        {
            debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_dispatcher_connectionless] DISCONNECT received\n", th.id);
            continue ; // a connection-less client ask for unnecessary disconnect, ignored!
        }

        if (th.type_op == XPN_SERVER_FINALIZE)
        {
            debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_dispatcher_connectionless] FINALIZE received\n", th.id);
            the_end = 1;
	    break;
        }

        debug_info(" xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx \n") ;

        // Launch worker per operation
        th_arg.params         = &params;
        th_arg.comm           = th.comm;
        th_arg.function       = xpn_server_run;
        th_arg.type_op        = th.type_op;
        th_arg.rank_client_id = th.rank_client_id;
        th_arg.tag_client_id  = th.tag_client_id;
        th_arg.wait4me        = FALSE;
        th_arg.close4me       = TRUE;

        base_workers_launch(&worker2, &th_arg, xpn_server_run);
        debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_dispatcher_connectionless] Worker launched\n", th.id);
    }

    debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_dispatcher_connectionless] End\n", th.id);
}

void xpn_server_dispatcher ( struct st_th th )
{
    int ret;
    struct st_th th_arg;

    debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_dispatcher] >> Begin\n", th.id);

    // check params...
    if (NULL == th.params) 
    {
        debug_error("[TH_ID=%d] [XPN_SERVER] [xpn_server_dispatcher] ERROR: NULL arguments\n", th.id);
        debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_dispatcher] End\n", th.id);
        return;
    }

    int disconnect = 0;
    while (!disconnect)
    {
        debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_dispatcher] Waiting for operation\n", th.id);

        ret = xpn_server_comm_read_operation((xpn_server_param_st *)th.params,
			                     th.comm, &(th.type_op),
                                             &(th.rank_client_id), &(th.tag_client_id));
        if (ret < 0) {
            printf("[TH_ID=%d] [XPN_SERVER] [xpn_server_dispatcher] ERROR: read operation fail\n", th.id);
            return;
        }

        debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_dispatcher] OP '%s'; OP_ID %d\n", th.id, xpn_server_op2string(th.type_op), th.type_op);

        if (th.type_op == XPN_SERVER_DISCONNECT)
        {
            debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_dispatcher] DISCONNECT received\n", th.id);

            disconnect = 1;
            continue;
        }

        if (th.type_op == XPN_SERVER_FINALIZE)
        {
            debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_dispatcher] FINALIZE received\n", th.id);

            disconnect = 1;
            the_end = 1;
            continue;
        }

        debug_info(" xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx \n") ;

        // Launch worker per operation
        th_arg.params         = &params;
        th_arg.comm           = th.comm;
        th_arg.function       = xpn_server_run;
        th_arg.type_op        = th.type_op;
        th_arg.rank_client_id = th.rank_client_id;
        th_arg.tag_client_id  = th.tag_client_id;
        th_arg.wait4me        = FALSE;
        th_arg.close4me       = FALSE;

        base_workers_launch(&worker2, &th_arg, xpn_server_run);
        debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_dispatcher] Worker launched\n", th.id);
    }

    debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_dispatcher] Client %d close\n", th.id, th.rank_client_id);
    xpn_server_comm_disconnect(th.params, th.comm);

    debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_dispatcher] End\n", th.id);
}

void xpn_server_launch_worker ( worker_t *w, void *comm, void (*function)(struct st_th) )
{
    struct st_th th_arg;

    // Launch dispatcher per aplication
    th_arg.params         = &params;
    th_arg.comm           = comm;
    th_arg.type_op        = 0;
    th_arg.rank_client_id = 0;
    th_arg.tag_client_id  = 0;
    th_arg.wait4me        = FALSE;
    th_arg.function       = function;

    base_workers_launch(w, &th_arg, function);
}

int xpn_server_init ( void )
{
    int ret ;

    // * mpi_comm initialization
    debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_up] xpn_comm initialization\n", 0);

    ret = xpn_server_comm_init(&params);
    if (ret < 0) 
    {
        printf("[TH_ID=%d] [XPN_SERVER] [xpn_server_up] ERROR: xpn_comm initialization fails\n", 0);
        return -1;
    }

    // * Workers initialization
    debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_up] Workers initialization\n", 0);

    ret = base_workers_init(&worker1, params.thread_mode_connections);
    if (ret < 0)
    {
        printf("[TH_ID=%d] [XPN_SERVER] [xpn_server_up] ERROR: Workers 1 initialization fails\n", 0);
        return -1;
    }

    ret = base_workers_init(&worker2, params.thread_mode_operations);
    if (ret < 0)
    {
        printf("[TH_ID=%d] [XPN_SERVER] [xpn_server_up] ERROR: Workers 2 initialization fails\n", 0);
        return -1;
    }

    ret = base_workers_init(&worker3, TH_OP);
    if (ret < 0)
    {
        printf("[TH_ID=%d] [XPN_SERVER] [xpn_server_up] ERROR: Workers 3 initialization fails\n", 0);
        return -1;
    }

    return 0;
}

int xpn_server_finish ( void )
{
    // Wait and finalize for all current workers
    debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_up] Workers destroy\n", 0);
    base_workers_destroy(&worker1);
    base_workers_destroy(&worker2);
    base_workers_destroy(&worker3);

    debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_up] mpi_comm destroy\n", 0);
    xpn_server_comm_destroy(&params);

    return 0;
}


/* ... Functions / Funciones ......................................... */

// Start servers
int xpn_server_up ( void )
{
    int ret;
    int server_socket, port;
    int connection_socket;
    int recv_code = 0;
    int await_stop = 0;
    void *comm = NULL;

    debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_up] >> Begin\n", 0);

    // Initialize server
    ret = xpn_server_init() ;
    if (ret < 0)
    {
        printf("[TH_ID=%d] [XPN_SERVER] [xpn_server_init] ERROR: initialization fails\n", 0);
        return -1;
    }

    debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_up] Control socket initialization\n", 0);

    port = utils_getenv_int("XPN_SCK_PORT", DEFAULT_XPN_SCK_PORT) ;
    ret  = socket_server_create(&server_socket, port);
    if (ret < 0)
    {
        printf("[TH_ID=%d] [XPN_SERVER] [xpn_server_up] ERROR: Socket initialization fails\n", 0);
        return -1;
    }

    // One thread for connection-less clients...
    xpn_server_launch_worker(&worker3, NULL, xpn_server_dispatcher_connectionless);

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
                ret = socket_send(connection_socket, params.port_name, MAX_PORT_NAME_LENGTH);
        		if (ret < 0) continue;
        		ret = xpn_server_comm_accept(&params, &comm, XPN_SERVER_CONNECTION);
        		if (ret < 0) continue;
        		xpn_server_launch_worker(&worker1, comm, xpn_server_dispatcher) ;
                break;

            case SOCKET_ACCEPT_CODE_NO_CONN:
                socket_send(connection_socket, params.port_name_no_conn, MAX_PORT_NAME_LENGTH);
                break;

            case SOCKET_FINISH_CODE:
            case SOCKET_FINISH_CODE_AWAIT:
                xpn_server_finish();
                the_end = 1;
                if (recv_code == SOCKET_FINISH_CODE_AWAIT)
                {
                    await_stop = 1;
                }
                break;

            default:
                debug_info("[TH_ID=%d] [XPN_SERVER %s] [xpn_server_up] >> Socket recv unknown code %d\n", 0, params.srv_name, recv_code);
                break;
        }

        if (await_stop == 0)
        {
            socket_close(connection_socket);
        }
    }

    socket_close(server_socket);

    if (await_stop == 1)
    {
        socket_send(connection_socket, &recv_code, sizeof(recv_code));
        socket_close(connection_socket);
    }

    debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_up] >> End\n", 0);
    return 0;
}

// Start servers spawn
int xpn_is_server_spawned ( void )
{
    int ret = 0;

    debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_is_server_spawned] >> Begin\n", 0);

    #ifndef ENABLE_MPI_SERVER
    printf("WARNING: if you have not compiled XPN with the MPI server then you cannot use spawn server.\n");
    #else
    // Initialize server
    // mpi_comm initialization
    debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_is_server_spawned] mpi_comm initialization\n", 0);
    ret = PMPI_Init(&params.argc, &params.argv);

    // TODO: check if necesary bypass the bypass with dlysm RTLD_NEXT
    filesystem_low_set(RTLD_NEXT);

    // Workers initialization
    debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_is_server_spawned] Workers initialization\n", 0);

    // in spawn there are no connections so server is secuential
    ret = base_workers_init(&worker1, TH_NOT);
    if (ret < 0) {
        printf("[TH_ID=%d] [XPN_SERVER] [xpn_is_server_spawned] ERROR: Workers initialization fails\n", 0);
        return -1;
    }

    ret = base_workers_init(&worker2, params.thread_mode_operations);
    if (ret < 0) 
    {
        printf("[TH_ID=%d] [XPN_SERVER] [xpn_is_server_spawned] ERROR: Workers initialization fails\n", 0);
        return -1;
    }

    debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_is_server_spawned] Get parent\n", 0);
    struct st_th th_arg;
    MPI_Comm *parent;

    parent = (MPI_Comm *)malloc(sizeof(MPI_Comm));
    if (NULL == parent) 
    {
        debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_is_server_spawned] ERROR: Memory allocation\n", 0);
        return -1;
    }

    ret = MPI_Comm_get_parent(parent);
    if ( (ret < 0) || (MPI_COMM_NULL == *parent) ) 
    {
        debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_is_server_spawned] ERROR: parent not found\n", 0);
        return -1;
    }

    // Launch dispatcher per aplication
    th_arg.params = &params;
    th_arg.comm = parent;
    th_arg.function = xpn_server_dispatcher;
    th_arg.type_op = 0;
    th_arg.rank_client_id = 0;
    th_arg.tag_client_id = 0;
    th_arg.wait4me = FALSE;

    base_workers_launch(&worker1, &th_arg, xpn_server_dispatcher);

    base_workers_destroy(&worker1);
    base_workers_destroy(&worker2);
    PMPI_Finalize();
    #endif

    debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_is_server_spawned] >> End\n", 0);
    return ret ;
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

    // Open host file
    debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_down] Open host file %s\n", 0, params.shutdown_file);

    file = fopen(params.shutdown_file, "r");
    if (file == NULL) {
        debug_info("[TH_ID=%d] [XPN_SERVER] [xpn_server_down] ERROR: invalid file %s\n", 0, params.shutdown_file);
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
    int port = utils_getenv_int("XPN_SCK_PORT", DEFAULT_XPN_SCK_PORT) ;

    while (fscanf(file, "%[^\n] ", srv_name) != EOF)
    {
        printf(" * Stopping server (%s)\n", srv_name);
        ret = socket_client_connect(srv_name, port, &sockets[i]);
        if (ret < 0) 
        {
            printf("[TH_ID=%d] [XPN_SERVER] [xpn_server_down] ERROR: socket connection %s\n", 0, srv_name);
            continue;
        }

        ret = socket_send(sockets[i], &buffer, sizeof(buffer));
        if (ret < 0) 
        {
            printf("[TH_ID=%d] [XPN_SERVER] [xpn_server_down] ERROR: socket send %s\n", 0, srv_name);
        }

        if (params.await_stop == 0)
        {
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
            if (ret < 0) 
            {
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


// Main
int main ( int argc, char *argv[] )
{
    int   ret = -1;
    char *exec_name = NULL;
    int   port ;
    int   req_id ;

    // Initializing...
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    // Get arguments..
    debug_info("[TH_ID=%d] [XPN_SERVER] [main] Get server params\n", 0);

    ret = xpn_server_params_get(&params, argc, argv);
    if (ret < 0) 
    {
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
    if (strcasecmp(exec_name, "xpn_server_spawn") == 0)
    {
        debug_info("[TH_ID=%d] [XPN_SERVER] [main] Spawn server\n", 0);
        ret = xpn_is_server_spawned();
    }
    else if (strcasecmp(exec_name, "xpn_stop_server") == 0)
    {
        debug_info("[TH_ID=%d] [XPN_SERVER] [main] Down servers\n", 0);
        ret = xpn_server_down();
    }
    else if (strcasecmp(exec_name, "xpn_terminate_server") == 0)
    {
        debug_info("[TH_ID=%d] [XPN_SERVER] [main] Terminate server\n", 0);

        printf(" * Stopping server (%s)\n", params.srv_name);
        port   = utils_getenv_int("XPN_SCK_PORT", DEFAULT_XPN_SCK_PORT) ;
        req_id = SOCKET_FINISH_CODE;
        ret    = sersoc_do_send(params.srv_name, port, req_id) ;
    }
    else
    {
        debug_info("[TH_ID=%d] [XPN_SERVER] [main] Up servers\n", 0);
        ret = xpn_server_up();
    }

    return ret;
}

/* ................................................................... */

