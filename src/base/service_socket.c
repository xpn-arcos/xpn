
/*
 *  Copyright 2020-2025 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos
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

   #include "base/service_socket.h"


/* ... Functions / Funciones ......................................... */

int sersoc_lookup_port_name ( char * srv_name, char * port_name, int socket_accept_code )
{
    int ret = -1 ;

    // Lookup port name
    debug_info("[SERSOC] [sersoc_lookup_port_name] >> Begin\n");

    int port = utils_getenv_int("XPN_SCK_PORT", DEFAULT_XPN_SCK_PORT) ;
    ret = sersoc_do_send_recv(srv_name, port, socket_accept_code, port_name) ;

    debug_info("[SERSOC] [sersoc_lookup_port_name] << End\n");
    return ret;
}

int sersoc_do_send_recv ( char * srv_name, int port, int req_id, char *res_val )
{
    int ret = -1 ;
    int connection_socket ;

    debug_info("[SERSOC] [sersoc_do_send_recv] >> Begin\n");

    // Lookup port name
    ret  = socket_client_connect(srv_name, port, &connection_socket);
    if (ret < 0)
    {
        debug_error("[SERSOC] [sersoc_do_send_recv] ERROR: socket connect\n");
        return -1;
    }

    ret = socket_send(connection_socket, &req_id, sizeof(int));
    if (ret < 0)
    {
        debug_error("[SERSOC] [sersoc_do_send_recv] ERROR: socket send\n");
        socket_close(connection_socket);
        return -1;
    }

    ret = socket_recv(connection_socket, res_val, MAX_PORT_NAME_LENGTH);
    if (ret < 0)
    {
        debug_error("[SERSOC] [sersoc_do_send_recv] ERROR: socket read\n");
        socket_close(connection_socket);
        return -1;
    }

    ret = socket_close(connection_socket);
    if (ret < 0)
    {
        debug_error("[SERSOC] [sersoc_do_send_recv] ERROR: socket close\n");
        return -1;
    }

    debug_error("[SERSOC] [sersoc_do_send_recv] request to '%s' command '%d' -> response: %s\n", srv_name, req_id, res_val);
    debug_info("[SERSOC] [sersoc_do_send_recv] << End\n");

    return ret;
}

int sersoc_do_send ( char * srv_name, int port, int req_id )
{
    int ret = -1 ;
    int connection_socket ;

    debug_info("[SERSOC] [sersoc_do_send] >> Begin\n");

    // Lookup port name
    ret  = socket_client_connect(srv_name, port, &connection_socket);
    if (ret < 0)
    {
        debug_error("[SERSOC] [sersoc_do_send] ERROR: socket connect\n");
        return -1;
    }

    ret = socket_send(connection_socket, &req_id, sizeof(int));
    if (ret < 0)
    {
        debug_error("[SERSOC] [sersoc_do_send] ERROR: socket send\n");
        socket_close(connection_socket);
        return -1;
    }

    ret = socket_close(connection_socket);
    if (ret < 0)
    {
        debug_error("[SERSOC] [sersoc_do_send] ERROR: socket close\n");
        return -1;
    }

    debug_error("[SERSOC] [sersoc_do_send] request to '%s' command '%d' -> response: %s\n", srv_name, req_id, res_val);
    debug_info("[SERSOC] [sersoc_do_send] << End\n");

    return ret;
}


/* ................................................................... */

