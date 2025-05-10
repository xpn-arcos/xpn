
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

      #include "sck_server_comm.h"


   /* ... Functions / Funciones ......................................... */

      int sck_server_comm_init (int *new_socket, char *port_name, int ipv )
      {
          int ret ;
          struct timeval t0, t1, tf;
          float  time;

          debug_info("[SCK_SERVER_COMM] [sck_server_comm_init] >> Begin\n");

          // Get timestap
          TIME_MISC_Timer( &t0 );

	  // create server socket...
          ret = socket_server_create(new_socket, 0, ipv) ; // port=0 -> system will assign port
          if (ret < 0)
          {
              debug_info("[SCK_SERVER_COMM] [sck_server_comm_init]: socket create fails\n");
              return -1;
          }

	  // ...get name for server socket...
          ret = socket_getsockname(port_name, *new_socket, ipv) ;
          if (ret < 0)
          {
              debug_info("[SCK_SERVER_COMM] [sck_server_comm_init]: getsockname fails\n");
              return -1;
          }

          // Get timestap
          TIME_MISC_Timer( &t1);
          TIME_MISC_DiffTime( &t0, &t1, &tf );
          time = TIME_MISC_TimevaltoFloat( &tf );

          // Print server init information
          printf(" | * Time to initialize XPN SCK server (port %s): %f s\n", port_name, time);
          debug_info("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_init] server %d available at %s\n", 0, 0, port_name);
          debug_info("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_init] server %d accepting...\n", 0, 0);

          debug_info("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_init] >> End\n", 0);
          return ret ;
      }

      int sck_server_comm_accept(int socket, int **new_socket, int ipv )
      {
          int ret ;

          debug_info("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_accept] >> Begin\n", 0);

          *new_socket = malloc(sizeof(int));
          if ( *new_socket == NULL) {
              printf("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_accept] ERROR: Memory allocation\n", 0);
              return -1;
          }

          ret = socket_server_accept(socket, *new_socket, ipv) ;
          if (ret < 0) {
              perror("accept: ");
	      free(*new_socket) ;
              return -1;
          }

          ret = socket_setopt_data(**new_socket) ;
          if (ret < 0) {
              perror("setsockopt: ");
	      free(*new_socket) ;
              return -1;
          }

          debug_info("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_accept] << End\n", 0);

          return ret ;
      }

      int sck_server_comm_disconnect(int * socket)
      {
          int ret ;

          ret = socket_close(*socket);
          free(socket);

          return ret ;
      }


   /* ................................................................... */

