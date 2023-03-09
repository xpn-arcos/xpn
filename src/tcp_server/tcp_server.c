/*
 *  Copyright 2020-2023 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos
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

#include "base/utils.h"

#include "tcp_server_params.h"

#include "tcp_server_ops.h"

#include "base/workers.h"

#include "tcp_server_comm.h"

#include "tcp_server_d2xpn.h"

#include "base/ns_tcp.h"


/* ... Global variables / Variables globales ......................... */

tcp_server_param_st params;
worker_t worker;
int the_end = 0;



/* ... Auxiliar Functions / Funciones Auxiliares ..................... */

void tcp_server_run(struct st_th th) 
{
    debug_info("[TCP-SERVER] (ID=%d): begin to do operation '%s' OP_ID %d\n", th.id, tcp_server_op2string(th.type_op), th.type_op);

    tcp_server_do_operation( & th, & the_end);

    debug_info("[TCP-SERVER] (ID=%d) end to do operation '%s'\n", th.id, tcp_server_op2string(th.type_op));
}

void tcp_server_dispatcher(struct st_th th) 
{
    int ret;

    // check params...
    if (NULL == th.params) 
    {
        printf("[WORKERS ID=%d] ERROR: NULL arguments", th.id);
        return;
    }

    struct st_th th_arg;
    int disconnect = 0;

    while (!disconnect) 
    {
        ret = tcp_server_comm_read_operation(th.params, (int) th.sd, (char * ) & (th.type_op), 1, & (th.rank_client_id));
        if (ret == -1) 
        {
            debug_info("[TCP-SERVER] ERROR: tcp_server_comm_readdata fail\n");
            return;
        }

        if (th.type_op == TCP_SERVER_DISCONNECT || th.type_op == TCP_SERVER_FINALIZE) 
        {
            debug_info("[TCP-SERVER] INFO: DISCONNECT received\n");
            disconnect = 1;
            continue;
        }

        // Launch worker per operation
        th_arg.params = & params;
        th_arg.sd = (int) th.sd;
        th_arg.function = tcp_server_run;
        th_arg.type_op = th.type_op;
        th_arg.rank_client_id = th.rank_client_id;
        th_arg.wait4me = FALSE;

        workers_launch( & worker, & th_arg, tcp_server_run);
    }

    debug_info("[TCP-SERVER] tcp_server_worker_run (ID=%d) close\n", th.rank_client_id);

    tcpClient_comm_close((int) th.sd); // NUEVO
}



/* ... Functions / Funciones ......................................... */

int tcp_server_up(void) 
{
    int sd;
    struct st_tcp_server_msg head;
    int rank_client_id;
    struct st_th th_arg;
    int ret;

    // Initialize
    debug_msg_init();
    tcp_server_comm_init( & params);
    workers_init( & worker, params.thread_mode);

    // Initialize semaphore for server disks
    ret = sem_init( & (params.disk_sem), 0, 1);
    if (ret == -1) 
    {
        printf("[MAIN] ERROR: semaphore initialize fails\n");
        return -1;
    }

    // Initialize semaphore for clients
    char serv_name[HOST_NAME_MAX];
    gethostname(serv_name, HOST_NAME_MAX);
    sprintf(params.sem_name_server, "%s%d", serv_name, getpid());

    sem_t * sem_server = sem_open(params.sem_name_server, O_CREAT, 0777, 1);
    if (sem_server == 0) 
    {
        printf("[MAIN] ERROR: semaphore open fails\n");
        return -1;
    }

    // Loop: receiving + processing
    the_end = 0;
    while (!the_end) 
    {
        debug_info("[TCP-SERVER] tcp_server_accept_comm()\n");

        params.client = 0; // TO DO - What is this

        sd = tcp_server_comm_accept( & params);
        if (sd == -1) 
        {
            continue;
        }

        ret = tcp_server_comm_read_operation( & params, sd, (char * ) & (head.type), 1, & (rank_client_id));
        if (ret == -1) 
        {
            printf("[TCP-SERVER] ERROR: tcp_server_comm_readdata fail\n");
            return -1;
        }

        if (head.type == TCP_SERVER_FINALIZE) 
        {
            the_end = 1;
            continue;
        }

        //Launch dispatcher per application
        th_arg.params = & params;
        th_arg.sd = sd;
        th_arg.function = tcp_server_dispatcher;
        th_arg.type_op = 0;
        th_arg.rank_client_id = 0;
        th_arg.wait4me = FALSE;

        workers_launch( & worker, & th_arg, tcp_server_dispatcher);
    }

    // Wait and finalize for all current workers
    debug_info("[TCP-SERVER] workers_destroy\n");
    workers_destroy( & worker);
    debug_info("[TCP-SERVER] tcp_server_comm_destroy\n");
    tcp_server_comm_destroy( & params);

    // Close semaphores
    sem_destroy( & (params.disk_sem));
    sem_unlink(params.sem_name_server);

    // return OK
    return 0;
}

int tcp_server_down(int argc, char * argv[]) 
{
    int ret, buf;
    int port_number;
    char srv_name[1024];
    char server_name[1024];
    char dns_name[2048];
    int server;
    FILE * file;

    printf("----------------\n");
    printf("Stopping servers\n");
    printf("----------------\n\n");

    //TCP_Init( & argc, & argv);

    // Open host file
    file = fopen(params.host_file, "r");
    if (file == NULL) 
    {
        printf("[MAIN] ERROR: invalid file %s\n", params.host_file);
        return -1;
    }

    while (fscanf(file, "%[^\n] ", srv_name) != EOF) 
    {
        // Lookup port name
        ret = tcp_server_translate(srv_name, server_name, & (port_number));

        //ret = ns_lookup (srv_name, port_name);

        if (ret == -1) 
        {
            printf("[MAIN] ERROR: server %s not found\n", dns_name);
            continue;
        }

        // Connect with servers
        //ret = int_connect(port_number, TCP_INFO_NULL, 0, TCP_COMM_SELF, & server);    TO-DO ¿???? 
        if (ret != 0) 
        {
            printf("[MAIN] ERROR: int_connect fails\n");
            continue;
        }
        buf = TCP_SERVER_FINALIZE;
        TCP_Send( & buf, 1, TCP_INT, 0, 0, server);

        //int_disconnect( &server ); //TODO: fail
    }

    // Close host file
    fclose(file);

    //TCP_Finalize();

    return 0;
}



/*
 * Main
 */

int main(int argc, char * argv[]) 
{
    int ret = -1;
    char * exec_name = NULL;

    // Initializing...
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    // Welcome...
    printf("\n");
    printf(" tcp_server\n");
    printf(" ----------\n");
    printf("\n");
    printf(" Begin.\n");
    printf("\n");

    exec_name = basename(argv[0]);
    printf(" * action=%s\n", exec_name);

    // Get arguments..
    ret = tcp_server_params_get( & params, argc, argv);
    if (ret < 0) 
    {
        tcp_server_params_show_usage();
        return -1;
    }

    // Do associate action...
    if (strcasecmp(exec_name, "xpn_stop_tcp_server") == 0) 
    {
        ret = tcp_server_down(argc, argv);
    } 
    else 
    {
        ret = tcp_server_up();
    }

    return ret;
}

/* ................................................................... */