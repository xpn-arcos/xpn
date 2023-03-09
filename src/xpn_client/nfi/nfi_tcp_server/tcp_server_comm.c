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

int tcpClient_comm_init(tcpClient_param_st * params) 
{
    int ret, provided, claimed;
    int flag = 0;

    debug_info("[CLI-COMM] begin tcpClient_comm_init(...)\n");


    debug_info("[CLI-COMM] end tcpClient_comm_init(...)\n");

    // Return OK
    return 0;
}



int tcpClient_comm_destroy(tcpClient_param_st * params) 
{

    debug_info("[CLI-COMM] begin tcpClient_comm_destroy(...)\n");

    
    debug_info("[CLI-COMM] end tcpClient_comm_destroy(...)\n");

    // Return OK
    return 0;
}



int tcpClient_comm_connect(tcpClient_param_st * params) 
{
    
    struct hostent * hp;
    struct sockaddr_in server_addr;
    int ret, sd, flag = 1;
    int lookup_retries = 0;


    debug_info("[CLI-COMM] begin tcpClient_comm_connect(...)\n");

    // Lookup port name
    do 
    {
        ret = tcp_server_translate(params -> srv_name, params -> server_name, & (params -> port_number));

        if (ret == -1) 
        {
            if (lookup_retries == 0) 
            {
                char cli_name[HOST_NAME_MAX];
                gethostname(cli_name, HOST_NAME_MAX);
                printf("----------------------------------------------------------------\n");
                printf("XPN Client %s : Waiting for servers being up and running...\n", cli_name);
                printf("----------------------------------------------------------------\n\n");
            }
            lookup_retries++;
            sleep(2);
        }
    } while (ret == -1 && lookup_retries < 150);

    if (ret == -1) 
    {
        debug_error("ERROR: DNS Lookup %s Port %s\n", params -> server_name, params -> port_name);
        return -1;
    }


    // Connect...
    
    printf("[NFI_COMM]----SERVER = %s NEWSERVER = %s PORT = %d\n", params -> srv_name, params -> server_name, port);

    sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sd < 0) 
    {
        perror("socket:");
        return -1;
    }
    printf("[NFI_COMM]----SERVER = %s NEWSERVER = %s PORT = %d ==> %d\n", params -> srv_name, params -> server_name, port, sd);


    if (setsockopt(sd, IPPROTO_TCP, TCP_NODELAY, & flag, sizeof(flag)) == -1) 
    {
        perror("setsockopt: ");
        return -1;
    }

    //NEW

    int val = 1024 * 1024; //1 MB

    if (setsockopt(sd, SOL_SOCKET, SO_SNDBUF, (char * ) & val, sizeof(int)) == -1) 
    {
        perror("setsockopt: ");
        return -1;
    }

    val = 1024 * 1024; //1 MB
    if (setsockopt(sd, SOL_SOCKET, SO_RCVBUF, (char * ) & val, sizeof(int)) == -1) 
    {
        perror("setsockopt: ");
        return -1;
    }

    /**************************************************/

    hp = gethostbyname(params -> server_name);
    if (hp == NULL) 
    {
        //tcp_server_err(TCP_SERVERERR_MEMORY);

        fprintf(stderr, "nfi_tcp_server_init: error gethostbyname %s (%s,%d)\n", params -> srv_name, params -> server_name, port);
        return -1;
    }

    //printf("[NFI_COMM]server = %s-%d-%p\n",server,TCP_SERVER_PORT,hp);
    printf("[NFI_COMM]server = %s-%d\n", params -> server_name, port);

    bzero((char * ) & server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    memcpy( & (server_addr.sin_addr), hp -> h_addr, hp -> h_length);
    //server_addr.sin_port = htons(TCP_SERVER_PORT);
    server_addr.sin_port = htons(port);
    printf("[NFI_COMM]Antes de connect to %s\n", params -> server_name);

    //se establece la conexion
    ret = connect(sd, (struct sockaddr * ) & server_addr, sizeof(server_addr));
    printf("[NFI_COMM]%s)connect(%s,%d) = %d\n", params -> srv_name, params -> server_name, port, ret);

    if (ret == -1) 
    {
        //tcp_server_err(TCP_SERVERERR_MEMORY);
        fprintf(stderr, "nfi_tcp_server_init: error in connect %s (%s,%d)\n", params -> srv_name, params -> server_name, port);
        perror("nfi_tcp_server_init:");
        return -1;
    }

    return sd;
}



int tcpClient_comm_disconnect(tcpClient_param_st * params) 
{

    debug_info("[CLI-COMM] begin tcpClient_comm_disconnect nservers\n");

    //TO DO: Revise

    // Return OK
    return 0;
}



int tcpClient_comm_locality(tcpClient_param_st * params) 
{
    int ret;
    int data;
    char cli_name[HOST_NAME_MAX];
    char serv_name[HOST_NAME_MAX];

    debug_info("[CLI-COMM] begin tcpClient_comm_locality\n");

    // Locality disable
    if (!params -> xpn_locality) 
    {
        debug_info("[CLI-COMM] tcpClient_comm_locality disable\n");
        params -> locality = 0;
        return 1;
    }

    // Get client host name
    gethostname(cli_name, HOST_NAME_MAX);

    data = TCP_SERVER_GETNODENAME;

    ret = tcpClient_write_data(params -> server, &data, 1 * sizeof(int), "<unused msg_id>") ; 
    if (ret != 0) 
    {
        debug_warning("Server[?]: TCP_Send fails :-(");
        return -1;
    }

    ret = tcpClient_read_data( params -> server, serv_name, HOST_NAME_MAX * sizeof(char), "<unused msg_id>") ;

    if (ret != 0) 
    {
        debug_warning("Server[?]: tcpClient_read_data fails :-(");
        return -1;
    }

    ret = tcpClient_read_data( params -> server, params -> sem_name_server, PATH_MAX * sizeof(char), "<unused msg_id>") ;

    if (ret != 0) 
    {
        debug_warning("Server[?]: tcpClient_read_data fails :-(");
        return -1;
    }

    if (strcmp(cli_name, serv_name) == 0) 
    {
        params -> locality = 1;
        params -> sem_server = sem_open(params -> sem_name_server, 0);
    } else 
    {
        params -> locality = 0;
    }

    debug_info("[CLI-COMM] end tcpClient_comm_locality\n");

    // Return OK
    return 1;
}



ssize_t tcpClient_write_operation(int fd, char * data, ssize_t size, __attribute__((__unused__)) char * msg_id) 
{
    int ret;

    debug_info("[CLI-COMM] begin tcpClient_write_operation(...)\n");

    // Check params
    if (size == 0) 
    {
        return 0;
    }
    if (size < 0) 
    {
        debug_warning("Server[?]: size < 0");
        return -1;
    }

    ret = tcpClient_write_data(fd, data, size * sizeof(int), msg_id) ;

    debug_info("[CLI-COMM] end tcpClient_write_operation(...)\n");

    // Return bytes written
    return size;
}



ssize_t tcpClient_write_data(int fd, char * data, ssize_t size, char * msg_id) 
{
    
    int ret;
    static ssize_t( * real_write)(int,const void * , size_t) = NULL;

    debug_info("[CLI-COMM] begin tcpClient_write_data(...)\n");

    // Check params
    if (size == 0) 
    {
        return 0;
    }
    if (size < 0) 
    {
        debug_warning("Server[?]: size < 0");
        return -1;
    }

    if (NULL == real_write)
    {
        real_write = (ssize_t( * )(int,const void * , size_t)) dlsym(RTLD_NEXT, "write");
    }

    do 
    {
        ret = real_write(fd, data + cont, size - cont);

        debug_info("[NFI_COMM]client: write_data(%d): %lu = %d ID=%s --th:%d--\n", fd, (unsigned long) size, ret, msg_id, (int) pthread_self());

        if (ret <= 0) perror("client: Error write_comm:");
        
        cont += ret;

    } while ((ret > 0) && (cont != size));

    if (ret == -1) 
    {
        debug_info("[NFI_COMM]client: write_data(%d): err %d  ID=%s --th:%d--\n", fd, ret, msg_id, (int) pthread_self());

        perror("client: write_data");
        return ret;
    }

    debug_info("[NFI_COMM]client: write_data(%d): %d de %lu ID=%s --th:%d--\n", fd, cont, (unsigned long) size, msg_id, (int) pthread_self());



    debug_info("[CLI-COMM] end tcpClient_write_data(...)\n");

    // Return bytes written
    return size;
}



ssize_t tcpClient_read_data(int fd, char * data, ssize_t size, char * msg_id) 
{
    int ret;
    TCP_Status status;
    static ssize_t( * real_read)(int, void * , size_t) = NULL;

    debug_info("[CLI-COMM] begin tcpClient_read_data(...)\n");

    // Check params
    if (size == 0) 
    {
        return 0;
    }
    if (size < 0) 
    {
        debug_warning("Server[?]: size < 0");
        return -1;
    }

    if (NULL == real_read) 
    {
        real_read = (ssize_t( * )(int, void * , size_t)) dlsym(RTLD_NEXT, "read");
    }

    do 
    {
        ret = real_read(fd, data + cont, size - cont);

        debug_info("[NFI_COMM]client: read_data(%d): %lu = %d ID=%s --th:%d--\n", fd, (unsigned long) size, ret, msg_id, (int) pthread_self());

        if (ret <= 0) 
        {
            perror("client: Error read_comm:");
        }
        cont += ret;

    } while ((ret > 0) && (cont != size));

    if (ret == -1) 
    {
        debug_info("[NFI_COMM]client: read_data(%d): err %d  ID=%s --th:%d--\n", fd, ret, msg_id, (int) pthread_self());

        perror("client: read_data");
        return ret;
    }
    debug_info("[NFI_COMM]client: read_data(%d): %d de %lu ID=%s --th:%d--\n", fd, cont, (unsigned long) size, msg_id, (int) pthread_self());

    debug_info("[CLI-COMM] end tcpClient_read_data(...)\n");

    // Return bytes read
    return size;
}
