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

//#define DEBUG 1

#include "tcp_server/tcp_server_comm.h"


  /* ... Functions / Funciones ......................................... */

// MOSQUITTO FILE
#ifdef HAVE_MOSQUITTO_H

void on_message(struct mosquitto *mqtt, void *obj, const struct mosquitto_message *msg)
{
    //tcp_server_comm_read_data(params, sd, buffer, to_write, rank_client_id);
    //filesystem_lseek(fd, head -> u_st_tcp_server_msg.op_write.offset + cont, SEEK_SET);



    //req.size = filesystem_write(fd, buffer, to_write);
    printf("%s\t%d\n\n", msg->topic, msg->payloadlen);
}

#endif





int tcp_server_comm_init ( tcp_server_param_st * params )
{
    int ret, val ;
    struct sockaddr_in server_addr;
    struct timeval t0, t1, tf;
    float time;

    DEBUG_BEGIN();

    // Get timestap
    TIME_MISC_Timer( & t0 );

    /*
     * Initialize socket
     */

    // create the connections
    params -> global_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (params -> global_sock < 0) {
        perror("error en el socket: ");
        return -1;
    }

    // tcp_nodalay
    val = 1;
    ret = setsockopt(params -> global_sock, IPPROTO_TCP, TCP_NODELAY, & val, sizeof(val)) ;
    if (ret < 0) {
        perror("setsockopt: ");
        return -1;
    }

    // sock_reuseaddr
    val = 1;
    ret = setsockopt(params -> global_sock, SOL_SOCKET, SO_REUSEADDR, (char * ) & val, sizeof(int));
    if (ret < 0) {
        perror("error en el setsockopt:");
        return -1;
    }

    // bind & listen
    bzero((char * )&server_addr, sizeof(server_addr)) ;
    server_addr.sin_family      = AF_INET ;
    server_addr.sin_addr.s_addr = INADDR_ANY ;
    server_addr.sin_port        = htons(atoi(params->port)) ;

    debug_info("[%s][%d]\t%s\n", __FILE__, __LINE__, params->port);

    ret = bind(params -> global_sock, (struct sockaddr * ) & server_addr, sizeof(server_addr));
    if (ret < 0)
    {
        perror("bind: ");
        return -1;
    }
    listen(params -> global_sock, 20);

    /*
     * Initialize mosquitto
     */
    
    #ifdef HAVE_MOSQUITTO_H

    if ( params -> mosquitto_mode  == 1 )
    {
	    printf("[%d]\tBEGIN INIT MOSQUITTO TCP_SERVER\n\n", __LINE__);

	    
	    mosquitto_lib_init();

	    params -> mqtt = mosquitto_new(NULL, true, NULL);

	    if(params -> mqtt == NULL)
	    {
    	    fprintf(stderr, "Error: Out of memory.\n");
    	    return 1;
	    }

	    //mosquitto_connect_callback_set(params -> mqtt, on_connect);
	    //mosquitto_subscribe_callback_set(params -> mqtt, on_subscribe);
	    mosquitto_message_callback_set(params -> mqtt, on_message);

	    #ifndef MOSQ_OPT_TCP_NODELAY
	    #define MOSQ_OPT_TCP_NODELAY 1
	    #endif

	    mosquitto_int_option(params -> mqtt, MOSQ_OPT_TCP_NODELAY, 1);

	    int rc = mosquitto_connect(params -> mqtt, "localhost", 1883, 0);
	    if( rc != MOSQ_ERR_SUCCESS )
	    {
    	    mosquitto_destroy(params -> mqtt);
    	    fprintf(stderr, "[%d]\tERROR INIT MOSQUITTO TCP_SERVER: %s\n", __LINE__, mosquitto_strerror(rc));
    	    return 1;
	    }

        /* Run the network loop in a background thread, this call returns quickly. */
        rc = mosquitto_loop_start(params -> mqtt);

        if(rc != MOSQ_ERR_SUCCESS)
        {
            mosquitto_destroy(params -> mqtt);
            fprintf(stderr, "Error: %s\n", mosquitto_strerror(rc));
            return 1;
        }

        //mosquitto_loop_forever(params -> mqtt, -1, 1);
	    printf("[%d]\tEND INIT MOSQUITTO TCP_SERVER\n\n", __LINE__);
	    
    }

    #endif

    /*
     * Post-initialize
     */

    // Publish socket "host name:port number"
    char * ip = ns_tcp_get_hostname() ;
    ret = ns_tcp_publish(params -> dns_file, params -> name, ip, params -> port) ;
    if (ret < 0) {
        fprintf(stderr, "ns_tcp_publish(dns_file:%s, name:%s, ip:%s, port:%s) -> %d\n", params -> dns_file, params -> name, ip, params -> port, ret) ;
        return -1;
    }

    // Print time to be up-and-running
    TIME_MISC_Timer(&t1);
    TIME_MISC_DiffTime(&t0, &t1, &tf);
    time = TIME_MISC_TimevaltoFloat(&tf);
    printf(" > XPN TCP server started (took %e sec.)\n", time);

    debug_info("[SRV_TCP_COMM] server %d accepting at %s\n", params -> rank, params -> port);

    DEBUG_END();

    // Return OK
    return 1;
}


int tcp_server_comm_destroy ( tcp_server_param_st * params )
{
    int ret;

    DEBUG_BEGIN();

    // Unpublish port name
    for (int i = 0; i < params -> size; ++i)
    {
        if (params -> rank == i)
	{
            ret = ns_tcp_unpublish(params -> dns_file, params -> srv_name);
            if (ret < 0) 
            {
                debug_info("[SRV_TCP_COMM] server%d: ns_unpublish fails :-(", params -> rank);
                return -1;
            }
        }
    }

    /*
     * Destroy mosquitto
     */
#ifdef HAVE_MOSQUITTO_H
    if (params -> mosquitto_mode)
    {
	    
        debug_info("[%d]\tBEGIN DESTROY MOSQUITTO TCP_SERVER\n\n", __LINE__);
        mosquitto_lib_cleanup();
        mosquitto_loop_stop(params -> mqtt, true);
        debug_info("[%d]\tEND DESTROY MOSQUITTO TCP_SERVER\n\n", __LINE__);
	
    }
#endif
    
    // Print server info
    char serv_name  [HOST_NAME_MAX];
    gethostname(serv_name, HOST_NAME_MAX);
    debug_info("--------------------------------\n");
    debug_info("XPN TCP server %s stopped\n", serv_name);
    debug_info("--------------------------------\n\n");

    DEBUG_END();

    // Return OK
    return 1;
}


int tcp_server_comm_accept(tcp_server_param_st * params)
{
    int    ret, sc, flag;
    struct sockaddr_in client_addr;
    socklen_t size = sizeof(struct sockaddr_in);

    DEBUG_BEGIN();

    sc = accept(params -> global_sock, (struct sockaddr * ) & client_addr, & size);
    if (sc < 0)
    {
        perror("accept: ");
    }
    debug_info("[SRV_TCP_COMM] desp. accept conection .... %d\n", sc);

    // tcp_nodelay
    flag = 1;
    ret = setsockopt(sc, IPPROTO_TCP, TCP_NODELAY, & flag, sizeof(flag)) ;
    if (ret < 0)
    {
        perror("setsockopt: ");
        return -1;
    }

    //NEW
    int val = 1024 * 1024; //1 MB

    ret = setsockopt(sc, SOL_SOCKET, SO_SNDBUF, (char * ) & val, sizeof(int)) ;
    if (ret < 0)
    {
        perror("setsockopt: ");
        return -1;
    }

    val = 1024 * 1024; //1 MB
    ret = setsockopt(sc, SOL_SOCKET, SO_RCVBUF, (char * ) & val, sizeof(int)) ;
    if (ret < 0)
    {
        perror("setsockopt: ");
        return -1;
    }

    DEBUG_END();

    // Return client int
    params -> client = sc;
    return params -> client;
}


int tcp_server_comm_connect ( tcp_server_param_st * params, char *server_name, int port_number )
{
    struct hostent * hp;
    struct sockaddr_in server_addr;
    int ret, sd, flag, val ;


    DEBUG_BEGIN();

    debug_info("[SRV_TCP_COMM] begin tcpClient_comm_connect(...)\n");

    // Socket
    debug_info("[SRV_TCP_COMM]----SERVER = %s NEWSERVER = %s PORT = %d\n", params -> srv_name, server_name, port_number);
    sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sd < 0) {
        perror("socket:");
        return -1;
    }
    debug_info("[SRV_TCP_COMM]----SERVER = %s NEWSERVER = %s PORT = %d ==> %d\n", params -> srv_name, server_name, port_number, sd);

    // sock-options
    flag = 1 ;
    ret = setsockopt(sd, IPPROTO_TCP, TCP_NODELAY, & flag, sizeof(flag)) ;
    if (ret < 0) {
        perror("setsockopt: ");
        return -1;
    }

    val = params->IOsize ; //1 MB
    ret = setsockopt(sd, SOL_SOCKET, SO_SNDBUF, (char * ) & val, sizeof(int)) ;
    if (ret < 0) {
        perror("setsockopt: ");
        return -1;
    }

    val = params->IOsize ; //1 MB
    ret = setsockopt(sd, SOL_SOCKET, SO_RCVBUF, (char * ) & val, sizeof(int)) ;
    if (ret < 0) {
        perror("setsockopt: ");
        return -1;
    }

    // server_name to ip address...
    hp = gethostbyname(server_name);
    if (NULL == hp) {
        //tcp_server_err(TCP_SERVERERR_MEMORY);
        fprintf(stderr, "tcp_server_comm_connect: error gethostbyname (%s,%d)\n", server_name, port_number);
        return -1;
    }

    // Connect...
    debug_info("[SRV_TCP_COMM] server = %s-%d\n", server_name, port_number);

    bzero((char * ) & server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port   = htons(port_number);
    memcpy( & (server_addr.sin_addr), hp -> h_addr, hp -> h_length);

    //se establece la conexion
    ret = connect(sd, (struct sockaddr * ) & server_addr, sizeof(server_addr));
    if (ret < 0) {
        //tcp_server_err(TCP_SERVERERR_MEMORY);
        fprintf(stderr, "nfi_tcp_server_init: error in connect (%s,%d)\n", server_name, port_number);
        perror("nfi_tcp_server_init:");
        return -1;
    }

    DEBUG_END();

    return sd;
}


int tcp_server_comm_close(int fd)
{
    DEBUG_BEGIN();

    close(fd);

    DEBUG_END();

    // Return OK
    return 1;
}


ssize_t tcp_server_comm_read_operation(tcp_server_param_st * params, int fd, char * data, ssize_t size, int * rank_client_id)
{
    int ret;

    DEBUG_BEGIN();

    // Check arguments
    if (NULL == params) {
        debug_warning("[SRV_TCP_COMM] server ERROR: NULL params arguments");
        return -1;
    }
    if (size < 0) {
        debug_warning("[SRV_TCP_COMM] server %d ERROR: size < 0", params -> rank);
        return -1;
    }
    if (size == 0) {
        return 0;
    }

    ret = tcp_server_comm_read_data(params, fd, data, size * sizeof(int), *rank_client_id); //      Nuevo
    if (ret < 0) {
        debug_warning("[SRV_TCP_COMM] server: tcp_server_comm_read_op fails : %d\n", ret);
        return ret ;
    }

    DEBUG_END();

    // Return int readed
    return ret / sizeof(int) ;
}


ssize_t tcp_server_comm_write_data(tcp_server_param_st * params, int fd, char * data, ssize_t size, __attribute__((__unused__)) int rank_client_id) //TODO rank client
{
    int ret, cont;

    DEBUG_BEGIN();

    // Check params
    if (NULL == params) {
        fprintf(stderr, "[SRV_TCP_COMM] server %d: ERROR: NULL params", -1);
        return -1;
    }
    if (size < 0) {
        fprintf(stderr, "[SRV_TCP_COMM] server %d: ERROR: size < 0", params -> rank);
        return -1;
    }
    if (size == 0) {
        return 0;
    }

    cont = 0;
    do
    {
        ret = 0;
        debug_info("[SRV_TCP_COMM] server:write_comm(%d) antes: %d = %d data %p --th:%d--\n", fd, size, ret, data, (int) pthread_self());
        //debug_info("Antes Escritura - %d\n", ret);
        ret = write(fd, data + cont, size - cont);
        //debug_info("Despues Escritura - %d\n", ret);
        if (ret < 0) {
            perror("server: Error write_comm: ");
	       return -1;
        }

        debug_info("[SRV_TCP_COMM] server:write_comm(%d) desp: %d = %d data %p --th:%d--\n", fd, size, ret, data, (int) pthread_self());
        cont += ret;

    } while ((ret > 0) && (cont != size));

    if (ret < 0) {
        debug_info("[SRV_TCP_COMM] server: Error write_comm(%d): -1 \n", fd);
        return ret;
    }

    DEBUG_END();
    // Return bytes written
    return cont;
}


ssize_t tcp_server_comm_read_data(tcp_server_param_st * params, int fd, char * data, ssize_t size, __attribute__((__unused__)) int rank_client_id) //TODO rank client
{
    int ret, cont ;

    DEBUG_BEGIN();

    // Check arguments
    if (NULL == params) {
        fprintf(stderr, "[SRV_TCP_COMM]: ERROR: NULL params");
        return -1;
    }
    if (size < 0) {
        fprintf(stderr, "[SRV_TCP_COMM] server %d: ERROR: size < 0", params -> rank);
        return -1;
    }
    if (size == 0) {
        return 0;
    }

    cont = 0;
    do
    {
        ret = 0;
        debug_info("[SRV_TCP_COMM] server:read_comm(%d) antes: %d = %d data %p --th:%d--\n", fd, size, ret, data, (int) pthread_self());
        //debug_info("Antes Lectura - %d\n", ret);
        ret = read(fd, data + cont, size - cont);
        //debug_info("Despues Lectura - %d\n", ret);
        if (ret < 0) {
            perror("[SRV_TCP_COMM] server: Error read_comm: ");
            return -1;
        }

        debug_info("[SRV_TCP_COMM] server:read_comm(%d) desp: %d = %d data %p --th:%d--\n", fd, size, ret, data, (int) pthread_self());
        cont += ret;

    } while ((ret > 0) && (cont != size));

    if (ret < 0) {
        debug_info("[SRV_TCP_COMM] server: Error read_comm(%d): -1\n", fd);
        return ret;
    }

    DEBUG_END();

    // Return bytes read
    return cont;
}


  /* ................................................................... */

