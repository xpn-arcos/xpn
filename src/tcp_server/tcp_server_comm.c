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

#include "tcp_server_comm.h"


/* ... Functions / Funciones ......................................... */

int tcp_server_comm_init(tcp_server_param_st * params) {
    int ret, provided;
    char serv_name[HOST_NAME_MAX];

    // Print server info
    gethostname(serv_name, HOST_NAME_MAX);
    printf("--------------------------------\n");
    printf("Starting XPN TCP server %s\n", serv_name);
    printf("--------------------------------\n\n");

    //Get timestap
    struct timeval t0;
    TIME_MISC_Timer( & t0);

    DEBUG_BEGIN();

    /*
     * Initialize socket
     */

    /* create the connections */
    params -> global_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (params -> global_sock < 0) {
        perror("error en el socket:");
        return -1;
    }

    // tcp_nodalay
    val = 1;
    if (setsockopt(params -> global_sock, IPPROTO_TCP, TCP_NODELAY, & val, sizeof(val)) == -1) {
        perror("setsockopt: ");
        return -1;
    }

    //NEW
    val = 1024 * 1024; //1 MB

    if (setsockopt(params -> global_sock, SOL_SOCKET, SO_SNDBUF, (char * ) & val, sizeof(int)) == -1) {
        perror("setsockopt: ");
        return -1;
    }

    val = 1024 * 1024; //1 MB
    if (setsockopt(params -> global_sock, SOL_SOCKET, SO_RCVBUF, (char * ) & val, sizeof(int)) == -1) {
        perror("setsockopt: ");
        return -1;
    }

    // sock_reuseaddr
    val = 1;
    ret = setsockopt(params -> global_sock, SOL_SOCKET, SO_REUSEADDR, (char * ) & val, sizeof(int));
    if (ret == -1) {
        perror("error en el setsockopt:");
        return -1;
    }
    bzero((char * ) & server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    // bind & listen
    ret = bind(params -> global_sock, (struct sockaddr * ) & server_addr, sizeof(server_addr));
    if (ret == -1) {
        perror("Error en el bind:");
        return -1;
    }
    listen(params -> global_sock, 20);

    /*
     * Initialize mosquitto
     */

    /*
    printf("[%d]\tBEGIN INIT MOSQUITTO TCP_SERVER\n\n", __LINE__);

    mosquitto_lib_init();

    mosqtcpserver = mosquitto_new(NULL, true, NULL);
    if(mosqtcpserver == NULL)
    {
    fprintf(stderr, "Error: Out of memory.\n");
    return 1;
    }

    mosquitto_connect_callback_set(mosqtcpserver, on_connect);
    mosquitto_subscribe_callback_set(mosqtcpserver, on_subscribe);
    mosquitto_message_callback_set(mosqtcpserver, on_message);

    #ifndef MOSQ_OPT_TCP_NODELAY
    #define MOSQ_OPT_TCP_NODELAY 1
    #endif

    mosquitto_int_option(mosqtcpserver, MOSQ_OPT_TCP_NODELAY, 1);

    int rc = mosquitto_connect(mosqtcpserver, "localhost", 1883, 60);

    if(rc != MOSQ_ERR_SUCCESS)
    {
    mosquitto_destroy(mosqtcpserver);
    fprintf(stderr, "[%d]\tERROR INIT MOSQUITTO TCP_SERVER: %s\n", __LINE__, mosquitto_strerror(rc));
    return 1;
    }

    printf("[%d]\tEND INIT MOSQUITTO TCP_SERVER\n\n", __LINE__);
    */

    /*
     * Publish socket "name"
     */

    ret = tcp_server_updateFile(params -> srv_name, params -> dns_file, params -> port_name);

    if (ret == -1) {
        perror("[%d]\tError en tcp_server_updateFile:", __LINE__);
        return -1;
    }

    /*
     * Print time to be up-and-running
     */

    struct timeval t1;
    struct timeval tf;
    float time;
    TIME_MISC_Timer( & t1);
    TIME_MISC_DiffTime( & t0, & t1, & tf);
    time = TIME_MISC_TimevaltoFloat( & tf);

    debug_info("[SERV-COMM] server %d available at %s\n", params -> rank, params -> port_name);
    debug_info("[SERV-COMM] server %d accepting...\n", params -> rank);

    DEBUG_END();

    // Return OK
    return 1;
}

int tcp_server_comm_destroy(tcp_server_param_st * params) {
    int ret;

    DEBUG_BEGIN();

    for (int i = 0; i < params -> size; ++i) {
        if (params -> rank == i) {
            // Unpublish port name
            ret = ns_unpublish(params -> dns_file); //TO-DO
            if (ret < 0) {
                debug_error("Server[%d]: ns_unpublish fails :-(", params -> rank);
                return -1;
            }
        }
    }

    /*
     * Destroy mosquitto
     */
    /*
        printf("[%d]\tBEGIN DESTROY MOSQUITTO TCP_SERVER\n\n", __LINE__);
        mosquitto_loop_forever(mosqtcpserver, -1, 1);
        mosquitto_lib_cleanup();
        printf("[%d]\tEND DESTROY MOSQUITTO TCP_SERVER\n\n", __LINE__);



        // Print server info
        char serv_name  [HOST_NAME_MAX];
        gethostname(serv_name, HOST_NAME_MAX);
        printf("--------------------------------\n");
        printf("XPN MPI server %s stopped\n", serv_name);
        printf("--------------------------------\n\n");
    */
    DEBUG_END();

    // Return OK
    return 1;
}

int tcp_server_comm_accept(tcp_server_param_st * params) {
    struct sockaddr_in client_addr;
    int ret, sc, flag;
    socklen_t size = sizeof(struct sockaddr_in);

    DEBUG_BEGIN();

    sc = accept(params -> global_sock, (struct sockaddr * ) & client_addr, & size);
    if (sc == -1) {
        perror("accept: ");
    }
    debug_info("[COMM] desp. accept conection .... %d\n", sc);

    // tcp_nodelay
    flag = 1;
    if (setsockopt(sc, IPPROTO_TCP, TCP_NODELAY, & flag, sizeof(flag)) == -1) {
        perror("setsockopt: ");
        return -1;
    }

    //NEW
    int val = 1024 * 1024; //1 MB

    if (setsockopt(sc, SOL_SOCKET, SO_SNDBUF, (char * ) & val, sizeof(int)) == -1) {
        perror("setsockopt: ");
        return -1;
    }

    val = 1024 * 1024; //1 MB
    if (setsockopt(sc, SOL_SOCKET, SO_RCVBUF, (char * ) & val, sizeof(int)) == -1) {
        perror("setsockopt: ");
        return -1;
    }

    DEBUG_END();

    // Return client int
    params -> client = sc;
    return params -> client;
}

int tcpClient_comm_close(int fd) {

    close(fd); //NUEVO

    // Return OK
    return 1;
}

ssize_t tcp_server_comm_read_operation(tcp_server_param_st * params, int fd, char * data, ssize_t size, int * rank_client_id) {
    int ret;
    //TCP_Status status ;

    DEBUG_BEGIN();

    // Check params
    if (NULL == params) {
        debug_warning("Server[%d]: ERROR: NULL arguments", -1);
        return -1;
    }
    if (size == 0) {
        return 0;
    }
    if (size < 0) {
        debug_warning("Server[%d]: ERROR: size < 0", params -> rank);
        return -1;
    }

    ret = tcp_server_comm_read_data(params, fd, data, size * sizeof(int), rank_client_id); //      Nuevo

    if (ret != 0) {
        debug_warning("Server: tcp_server_comm_read_op fails : %d\n", ret);
    }

    DEBUG_END();

    // Return bytes read
    return size; //TO-DO number of ints
}

ssize_t tcp_server_comm_write_data(tcp_server_param_st * params, int fd, char * data, ssize_t size, int rank_client_id) //TO-DO rank client
{
    int ret, cont = 0;

    DEBUG_BEGIN();

    // Check params
    if (NULL == params) {
        debug_warning("Server[%d]: ERROR: NULL params", -1);
        return -1;
    }
    if (size == 0) {
        return 0;
    }
    if (size < 0) {
        debug_warning("Server[%d]: ERROR: size < 0", params -> rank);
        return -1;
    }

    do {
        debug_info("[COMM] server:write_comm(%d) antes: %d = %d data %p ID=%s:%p --th:%d--\n", fd, size, ret, data, id, id, (int) pthread_self());
        ret = write(fd, data + cont, size - cont);
        if (ret < 0) {
            perror("server: Error write_comm:");
        }
        debug_info("[COMM] server:write_comm(%d) desp: %d = %d data %p ID=%s:%p --th:%d--\n", fd, size, ret, data, id, id, (int) pthread_self());
        cont += ret;
    } while ((ret > 0) && (cont != size));

    if (ret == -1) {
        debug_info("[COMM] server: Error write_comm(%d): -1 ID=%s:%p\n", fd, id, id);
        return ret;
    }

    DEBUG_END();

    // Return bytes written
    return cont;
}

ssize_t tcp_server_comm_read_data(tcp_server_param_st * params, int fd, char * data, ssize_t size, int rank_client_id) //TO-DO rank client
{
    int ret, cont = 0;
    //TCP_Status status ;

    DEBUG_BEGIN();

    // Check params
    if (NULL == params) {
        debug_warning("Server[-1]: ERROR: NULL params", -1);
        return -1;
    }
    if (size == 0) {
        return 0;
    }
    if (size < 0) {
        debug_warning("Server[%d]: ERROR: size < 0", params -> rank);
        return -1;
    }

    do {
        debug_info("[COMM] server:read_comm(%d) antes: %d = %d data %p ID=%s:%p --th:%d--\n", fd, size, ret, data, id, id, (int) pthread_self());
        ret = read(fd, data + cont, size - cont);
        if (ret < 0) {
            perror("server: Error read_comm:");
        }
        debug_info("[COMM] server:read_comm(%d) desp: %d = %d data %p ID=%s:%p --th:%d--\n", fd, size, ret, data, id, id, (int) pthread_self());
        cont += ret;
    } while ((ret > 0) && (cont != size));

    if (ret == -1) {
        debug_info("[COMM] server: Error read_comm(%d): -1 ID=%s:%p\n", fd, id, id);
        return ret;
    }

    DEBUG_END();

    // Return bytes read
    return cont;
}

/* ................................................................... */