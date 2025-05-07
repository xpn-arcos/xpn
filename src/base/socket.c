
/*
 *  Copyright 2020-2025 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra, Dario Muñoz Muñoz
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

   #include "base/socket.h"


/* ... Functions / Funciones ......................................... */

int socket_send ( int socket, void * buffer, int size )
{
    int r;
    int l = size;

    do
    {
        r = dlsym_write(socket, buffer, l);
        if (r < 0)
        {
            if (EPIPE == errno)
                 printf("[SOCKET] [socket_send] ERROR: client closed the connection.\n") ;
	    else printf("[SOCKET] [socket_send] ERROR: socket send buffer size %d Failed\n", size) ;

            return -1;
        }

        l = l - r;
        buffer = (void *) ((char *)buffer + r) ;

    } while ((l > 0) && (r >= 0));

    return size;
}

int socket_recv ( int socket, void * buffer, int size )
{
    int r;
    int l = size;

    do
    {
        r = dlsym_read(socket, buffer, l);
        if (r < 0)
        {
            if (EPIPE == errno)
                 printf("[SOCKET] [socket_send] ERROR: client closed the connection abruptly\n") ;
            else printf("[SOCKET] [socket_recv] ERROR: socket read buffer size %d Failed\n", size) ;

            return -1;
        }
        if (0 == r)
        {
            printf("[SOCKET] [socket_recv] WARN: end of file receive for socket '%d'\n", socket) ;
            return 0;
        }

        l = l - r;
        buffer = (void *) ((char *)buffer + r) ;

    } while ((l > 0) && (r >= 0));

    return size;
}

int socket_server_create ( int * out_socket, int port, int socket_mode )
{
    int ret = 0;

    debug_info("[SOCKET] [base_socket_init] >> Begin\n");

    // check arguments...
    if (NULL == out_socket)
    {
        debug_error("[SOCKET] [base_socket_init] ERROR: NULL out_socket\n");
        return -1;
    }

    // initialize...
    
    switch (socket_mode)
    {
    case SCK_IP4:
        debug_info("[SOCKET] [base_socket_init] socket_ip4_server_create\n");
        ret = socket_ip4_server_create(out_socket, port);
        break;

    case SCK_IP6:
        debug_info("[SOCKET] [base_socket_init] socket_ip6_server_create\n");
        ret = socket_ip6_server_create(out_socket, port);
        break;

    default:
        debug_info("[SOCKET] [base_socket_init] ERROR: on socket_server_create(%d).\n", socket_mode);
        return -1;
        break;
    }

    debug_info("[SOCKET] [base_socket_init] >> End\n");

    return ret;
}

int socket_server_accept ( int socket, int * out_conection_socket, int socket_mode )
{
    int ret = 0;

    debug_info("[SOCKET] [socket_server_accept] >> Begin\n");

    // check arguments...
    if (NULL == out_conection_socket)
    {
        debug_error("[SOCKET] [socket_server_accept] ERROR: NULL out_socket\n");
        return -1;
    }

    
    switch (socket_mode)
    {
    case SCK_IP4:
        debug_info("[SOCKET] [socket_server_accept] socket_ip4_server_accept\n");
        ret = socket_ip4_server_accept(socket, out_conection_socket);
        break;

    case SCK_IP6:
        debug_info("[SOCKET] [socket_server_accept] socket_ip6_server_accept\n");
        ret = socket_ip6_server_accept(socket, out_conection_socket);
        break;

    default:
        debug_info("[SOCKET] [socket_server_accept] ERROR: on socket_server_accept(%d).\n", socket_mode);
        return -1;
        break;
    }

    debug_info("[SOCKET] [socket_server_accept] >> End\n");

    return ret;
}

int socket_client_connect ( char * srv_name, int port, int * out_socket, int socket_mode )
{

    int ret = 0;

    debug_info("[SOCKET] [socket_client_connect] >> Begin\n");

    // check arguments...
    if (NULL == srv_name)
    {
        debug_error("[SOCKET] [socket_client_connect] ERROR: NULL srv_name\n");
        return -1;
    }

    
    switch (socket_mode)
    {
    case SCK_IP4:
        debug_info("[SOCKET] [socket_client_connect] socket_ip4_server_connect\n");
        ret = socket_ip4_client_connect(srv_name, port, out_socket);
        break;

    case SCK_IP6:
        debug_info("[SOCKET] [socket_client_connect] socket_ip6_server_connect\n");
        ret = socket_ip6_client_connect(srv_name, port, out_socket);
        break;

    default:
        debug_info("[SOCKET] [socket_client_connect] ERROR: on socket_client_connect(%d).\n", socket_mode);
        return -1;
        break;
    }

    debug_info("[SOCKET] [socket_client_connect] >> End\n");

    return ret;
}

int socket_close ( int socket )
{
    int ret;

    ret = close(socket);
    if (ret < 0)
    {
        printf("[SOCKET] [socket_close] ERROR: socket close Failed\n");
        return -1;
    }

    return ret;
}


int socket_gethostbyname ( char * ip, size_t ip_size, char * srv_name, int socket_mode )
{
    int ret = 0;
    debug_info("[SOCKET] [socket_gethostbyname] >> Begin\n");

    // check arguments...
    if (NULL == srv_name)
    {
        debug_error("[SOCKET] [socket_gethostbyname] ERROR: NULL srv_name\n");
        return NULL;
    }

    
    switch (socket_mode)
    {
    case SCK_IP4:
        debug_info("[SOCKET] [socket_gethostbyname] socket_ip4_server_connect\n");
        ret = socket_ip4_gethostbyname(ip, ip_size, srv_name);
        break;

    case SCK_IP6:
        debug_info("[SOCKET] [socket_gethostbyname] socket_ip6_server_connect\n");
        ret = socket_ip6_gethostbyname(ip, ip_size, srv_name);
        break;

    default:
        debug_info("[SOCKET] [socket_gethostbyname] ERROR: on socket_gethostbyname(%d).\n", socket_mode);
        return -1;
        break;
    }

    return ret;

}

/* ................................................................... */

