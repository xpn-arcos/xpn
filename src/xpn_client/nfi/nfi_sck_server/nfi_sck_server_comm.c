
/*
 *  Copyright 2020-2025 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Dario Muñoz Muñoz, Elias del Pozo Puñal
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

     #include "socket.h"
     #include "base/ns.h"
     #include "nfi_sck_server_comm.h"


  /* ... Functions / Funciones ......................................... */

     int nfi_sck_server_comm_connect ( char *srv_name, char *port_name, int *out_socket )
     {
         int ret ;
         int ipv ;

         debug_info("[NFI_SCK_SERVER_COMM] [nfi_sck_server_comm_connect] >> Begin\n");

         ipv = utils_getenv_int("XPN_SCK_IPV", DEFAULT_XPN_SCK_IPV);
	 ret = socket_client_connect_with_retries(srv_name, port_name, out_socket, 2, ipv) ;

         debug_info("[NFI_SCK_SERVER_COMM] [nfi_sck_server_comm_connect] << End\n");

         return ret ;
     }

     int nfi_sck_server_comm_disconnect(int socket, int keep_connected)
     {
         int ret;
         int code = XPN_SERVER_DISCONNECT;

         debug_info("[NFI_SCK_SERVER_COMM] [nfi_sck_server_comm_disconnect] >> Begin\n");

         // If it has been previously disconnected, just return OK
         if (socket == -1)
         {
             debug_info("[NFI_SCK_SERVER_COMM] [nfi_sck_server_comm_disconnect] Previously disconnected\n");
             debug_info("[NFI_SCK_SERVER_COMM] [nfi_sck_server_comm_disconnect] << End\n");
             return 0;
         }

         if (keep_connected == 1)
         {

             debug_info("[NFI_SCK_SERVER_COMM] [nfi_sck_server_comm_disconnect] Send disconnect message\n");
             ret = socket_send(socket, &code, sizeof(code));
             if (ret < 0)
             {
                 printf("[NFI_SCK_SERVER_COMM] [nfi_sck_server_comm_disconnect] ERROR: nfi_sck_server_comm_write_operation fails\n");
                 return ret;
             }

             // Disconnect
             debug_info("[NFI_SCK_SERVER_COMM] [nfi_sck_server_comm_disconnect] Disconnect\n");

         }

         ret = socket_close(socket);
         if (ret < 0)
         {
             printf("[NFI_SCK_SERVER_COMM] [nfi_sck_server_comm_disconnect] ERROR: MPI_Comm_disconnect fails\n");
             return ret;
         }

         debug_info("[NFI_SCK_SERVER_COMM] [nfi_sck_server_comm_disconnect] << End\n");

         // Return OK
         return ret;
     }


  /* ................................................................... */

