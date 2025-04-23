
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

   #include "xpn_server_comm.h"


/* ... Functions / Funciones ......................................... */

int xpn_server_comm_init ( int server_type, xpn_server_param_st * params )
{
    int ret = -1;

    switch (server_type)
    {
#ifdef ENABLE_MPI_SERVER
       case XPN_SERVER_TYPE_MPI:
            ret = mpi_server_comm_init(params->argc, params->argv, params->thread_mode_connections, params->port_name);
            break;
#endif

#ifdef ENABLE_SCK_SERVER
       case XPN_SERVER_TYPE_SCK:
            // Initialize socket subsystem
            ret = sck_server_comm_init( & params->server_socket,         params->port_name_conn);

            // Initialize socket without connection between ops.
            ret = sck_server_comm_init( & params->server_socket_no_conn, params->port_name_no_conn);

            // Initialize mosquitto if it is enabled
    	    if (1 == params->mosquitto_mode) {
                ret = mq_server_mqtt_init(params);
            }
            break;
#endif

       default:
            printf("[XPN_SERVER] [xpn_server_comm_init] server_type '%d' not recognized, please check your compiler options just in case.\n", server_type);
            break;
    }

    return ret;
}

int xpn_server_comm_destroy ( int server_type, xpn_server_param_st * params )
{
    int ret = -1;

    switch (server_type)
    {
#ifdef ENABLE_MPI_SERVER
       case XPN_SERVER_TYPE_MPI:
            ret = mpi_server_comm_destroy(params->port_name);
            break;
#endif

#ifdef ENABLE_SCK_SERVER
       case XPN_SERVER_TYPE_SCK:
            // Finalize mosquitto if it is enabled
	    if (1 == params->mosquitto_mode) {
                ret = mq_server_mqtt_destroy(params);
            }

            // Close socket
	    ret =     shutdown(params->server_socket, SHUT_RDWR) ;
            ret = socket_close(params->server_socket);

	    ret =     shutdown(params->server_socket_no_conn, SHUT_RDWR) ;
            ret = socket_close(params->server_socket_no_conn);
            break;
#endif

       default:
            printf("[XPN_SERVER] [xpn_server_comm_destroy] server_type '%d' not recognized, please check your compiler options just in case.\n", server_type);
            break;
    }

    return ret;
}

int xpn_server_comm_accept ( int server_type, xpn_server_param_st * params, int connection_type, void ** new_sd )
{
    int ret = -1;

    switch (server_type)
    {
#ifdef ENABLE_MPI_SERVER
       case XPN_SERVER_TYPE_MPI:
            ret = mpi_server_comm_accept(params->port_name, (MPI_Comm ** ) new_sd);
            break;
#endif

#ifdef ENABLE_SCK_SERVER
       case XPN_SERVER_TYPE_SCK:
            if (connection_type != XPN_SERVER_CONNECTIONLESS)
                 ret = sck_server_comm_accept(params->server_socket,         (int ** ) new_sd);
            else ret = sck_server_comm_accept(params->server_socket_no_conn, (int ** ) new_sd);
            break;
#endif

       default:
            printf("[XPN_SERVER] [xpn_server_comm_accept] server_type '%d' not recognized, please check your compiler options just in case.\n", server_type);
            break;
    }

    return ret;
}

int xpn_server_comm_disconnect ( int server_type, void * sd )
{
    int ret = -1;

    switch (server_type)
    {
#ifdef ENABLE_MPI_SERVER
       case XPN_SERVER_TYPE_MPI:
            ret = mpi_server_comm_disconnect((MPI_Comm * ) sd);
            break;
#endif

#ifdef ENABLE_SCK_SERVER
       case XPN_SERVER_TYPE_SCK:
            ret = sck_server_comm_disconnect((int * ) sd);
            break;
#endif

       default:
            printf("[XPN_SERVER] [xpn_server_comm_disconnect] server_type '%d' not recognized, please check your compiler options just in case.\n", server_type);
            break;
    }

    return ret;
}

ssize_t xpn_server_comm_read_operation ( int server_type, void * sd, int * op, __attribute__((__unused__)) int * rank_client_id, __attribute__((__unused__)) int * tag_client_id )
{
    ssize_t ret = -1;

    switch (server_type)
    {
#ifdef ENABLE_MPI_SERVER
       case XPN_SERVER_TYPE_MPI:
            ret = mpi_server_comm_read_operation((MPI_Comm * ) sd, op, rank_client_id, tag_client_id);
            break;
#endif

#ifdef ENABLE_SCK_SERVER
       case XPN_SERVER_TYPE_SCK:
            ret = socket_recv( * (int * ) sd, op, sizeof( * op));
            break;
#endif

       default:
            printf("[XPN_SERVER] [xpn_server_comm_read_operation] server_type '%d' not recognized, please check your compiler options just in case.\n", server_type);
            break;
    }

    return ret;
}

ssize_t xpn_server_comm_write_data ( int server_type, void * sd, char * data, ssize_t size, __attribute__((__unused__)) int rank_client_id, __attribute__((__unused__)) int tag_client_id )
{
    ssize_t ret = -1;

    switch (server_type)
    {
#ifdef ENABLE_MPI_SERVER
       case XPN_SERVER_TYPE_MPI:
            ret = mpi_server_comm_write_data((MPI_Comm * ) sd, data, size, rank_client_id, tag_client_id);
            break;
#endif

#ifdef ENABLE_SCK_SERVER
       case XPN_SERVER_TYPE_SCK:
            ret = socket_send( * (int * ) sd, data, size);
            break;
#endif

       default:
            printf("[XPN_SERVER] [xpn_server_comm_write_data] server_type '%d' not recognized, please check your compiler options just in case.\n", server_type);
            break;
    }

    return ret;
}

ssize_t xpn_server_comm_read_data ( int server_type, void * sd, char * data, ssize_t size, __attribute__((__unused__)) int rank_client_id, __attribute__((__unused__)) int tag_client_id )
{
    ssize_t ret = -1;

    switch (server_type)
    {
#ifdef ENABLE_MPI_SERVER
       case XPN_SERVER_TYPE_MPI:
            ret = mpi_server_comm_read_data((MPI_Comm * ) sd, data, size, rank_client_id, tag_client_id);
            break;
#endif

#ifdef ENABLE_SCK_SERVER
       case XPN_SERVER_TYPE_SCK:
            ret = socket_recv( * (int *)sd, data, size );
            break;
#endif

       default:
            printf("[XPN_SERVER] [xpn_server_comm_read_data] server_type '%d' not recognized, please check your compiler options just in case.\n", server_type);
            break;
    }

    return ret;
}


/* ................................................................... */

