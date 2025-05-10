
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

     #include "base/socket_ip4.h"


  /* ... Functions / Funciones ......................................... */

     int socket_ip4_server_create ( int * out_socket, int port )
     {
         int ret = 0;
         struct sockaddr_in server_addr;
         int server_socket, val ;

         // check arguments...
         if (NULL == out_socket)
         {
             printf("[SOCKET] [socket_ip4_server_create] ERROR: NULL out_socket\n");
             return -1;
         }

         // Socket init
         debug_info("[SOCKET] [socket_server_create] Scoket init\n");

         server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
         if (server_socket < 0)
         {
             printf("[SOCKET] [socket_server_create] ERROR: socket fails\n");
             return -1;
         }

         // tcp_nodelay
         debug_info(" [SOCKET] [socket_server_create] TCP nodelay\n");

         val = 1;
         ret = setsockopt(server_socket, IPPROTO_TCP, TCP_NODELAY, & val, sizeof(val));
         if (ret < 0)
         {
             printf("[SOCKET] [socket_server_create] ERROR: setsockopt for TCP_NODELAY fails\n");
             return -1;
         }

         // sock_reuseaddr
         debug_info(" [SOCKET] [socket_server_create] Socket reuseaddr nodelay\n");

         val = 1;
         ret = setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (char * ) & val, sizeof(int));
         if (ret < 0)
         {
             printf("[SOCKET] [socket_server_create] ERROR: setsockopt for SO_REUSEADDR fails\n");
             return -1;
         }

         // bind
         debug_info("[SOCKET] [socket_server_create] Socket bind\n");

         bzero((char * ) & server_addr, sizeof(server_addr));
         server_addr.sin_family = AF_INET;
         server_addr.sin_addr.s_addr = INADDR_ANY;
         server_addr.sin_port = htons(port);

         ret = bind(server_socket, (struct sockaddr * ) & server_addr, sizeof(server_addr));
         if (ret < 0)
         {
             printf("[SOCKET] [socket_server_create] ERROR: bind fails\n");
             return -1;
         }


         // listen
         debug_info("[SOCKET] [socket_server_create] Socket listen\n");

         ret = listen(server_socket, SOMAXCONN);
         if (ret < 0)
         {
             printf("[SOCKET] [socket_server_create] ERROR: listen fails\n");
             return -1;
         }

         // return new server_socket as *out_socket
         *out_socket = server_socket;
         return 0;
     }

     int socket_ip4_server_accept ( int socket, int * out_conection_socket )
     {
         struct sockaddr_in client_addr ;
         socklen_t sock_size ;

         // check arguments...
         if (NULL == out_conection_socket)
         {
             printf("[SOCKET] [socket_ip4_server_accept] ERROR: NULL out_conection_socket\n");
             return -1;
         }

         // Accept
         debug_info("[SOCKET] [socket_ip4_server_accept] Accept\n");

         sock_size = sizeof(struct sockaddr_in);
         *out_conection_socket = accept(socket, (struct sockaddr * ) & client_addr, & sock_size);
         if (*out_conection_socket < 0) {
             printf("[SOCKET] [socket_ip4_server_accept] ERROR: socket accept\n");
             return -1;
         }

         debug_info("[SOCKET] [socket_ip4_server_accept] accepted for %d\n", *out_conection_socket);

         return 0;
     }

     int socket_ip4_client_connect ( char * srv_name, int port, int * out_socket )
     {
         int    ret, client_fd;
         struct sockaddr_in serv_addr;
         struct hostent * hp;

         // check arguments...
         if (NULL == out_socket)
         {
             printf("[SOCKET] [socket_ip4_client_connect] ERROR: NULL out_socket\n");
             return -1;
         }

	 // socket + gethostbyname + connect
         client_fd = socket(AF_INET, SOCK_STREAM, 0);
         if (client_fd < 0)
         {
             printf("[SOCKET] [socket_read] ERROR: socket creation error\n");
             return -1;
         }

         hp = gethostbyname(srv_name);
         if (hp == NULL)
         {
             printf("[SOCKET] [socket_read] ERROR: gethostbyname srv_name: %s\n", srv_name);
             close(client_fd);
             return -1;
         }

         bzero((char * ) & serv_addr, sizeof(serv_addr));
         serv_addr.sin_family = AF_INET;
         serv_addr.sin_port   = htons(port);
         memcpy( & (serv_addr.sin_addr), hp->h_addr, hp->h_length);

         ret = connect(client_fd, (struct sockaddr * ) &serv_addr, sizeof(serv_addr));
         if (ret < 0)
         {
             printf("[SOCKET] [socket_read] ERROR: socket connection failed to %s in port %d %s\n", srv_name, port, strerror(errno));
             close(client_fd);
             return -1;
         }

         *out_socket = client_fd;
         return 0;
     }


     //
     //  address management
     //

     int socket_ip4_gethostbyname ( char * ip, size_t ip_size, char * srv_name )
     {
         char   *ip_local;
         struct hostent *srv_entry;

         // check arguments...
         if (NULL == ip)
         {
             printf("[SOCKET] [socket_ip4_gethostbyname] ERROR: NULL ip argument\n");
             return -1;
         }

	 ip_size = ip_size ; // to avoid warning unused argument

         // gethostBYname
         srv_entry = gethostbyname(srv_name);                                    // find host information
         ip_local = inet_ntoa(*((struct in_addr *)srv_entry->h_addr_list[0]));   // Convert into IP string
         strcpy(ip, ip_local);

         return 1;
     }

     int socket_ip4_getsockname ( char * port_name, int new_socket )
     {
         struct sockaddr_in server_addr;

         // get sockname
         socklen_t len = sizeof(server_addr);
         getsockname( new_socket, (struct sockaddr * ) &server_addr, &len);
         sprintf(port_name, "%d", ntohs(server_addr.sin_port));

         return 1;
     }


  /* ................................................................... */

