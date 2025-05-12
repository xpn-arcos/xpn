
/*
 *  Copyright 2000-2025 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Elias Del Pozo Puñal
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

     #include "base/socket_ip6.h"


  /* ... Functions / Funciones ......................................... */

     int socket_ip6_server_create ( int * out_socket, int port )
     {
         int ret = 0;
         struct sockaddr_in6 server_addr;
         int server_socket, val ;

         // check arguments...
         if (NULL == out_socket)
         {
             printf("[SOCKET_IP6] [socket_ip6_server_create] ERROR: NULL out_socket\n");
             return -1;
         }

	 // socket
         server_socket = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
         if (server_socket < 0)
         {
             printf("[SOCKET_IP6] [socket_server_create] ERROR: socket fails\n");
             return -1;
         }

         val = 1;
         ret = setsockopt(server_socket, IPPROTO_TCP, TCP_NODELAY, &val, sizeof(val));
         if (ret < 0)
         {
             printf("[SOCKET_IP6] [socket_server_create] ERROR: setsockopt fails\n");
             return -1;
         }

         debug_info("[SOCKET_IP6] [socket_server_create] Socket reuseaddr\n");

         val = 1;
         ret = setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (char * ) &val, sizeof(int));
         if (ret < 0)
         {
             printf("[SOCKET_IP6] [socket_server_create] ERROR: setsockopt fails\n");
             return -1;
         }

         // bind
         debug_info("[SOCKET_IP6] [socket_server_create] Socket bind\n");

         bzero((char * ) &server_addr, sizeof(server_addr));
         server_addr.sin6_family = AF_INET6;
         server_addr.sin6_addr = in6addr_any;
         server_addr.sin6_port = htons(port);

         ret = bind(server_socket, (struct sockaddr * ) &server_addr, sizeof(server_addr));
         if (ret < 0)
         {
             printf("[SOCKET_IP6] [socket_server_create] ERROR: bind fails\n");
             return -1;
         }

         // listen
         debug_info("[SOCKET_IP6] [socket_server_create] Socket listen\n");

         ret = listen(server_socket, SOMAXCONN);
         if (ret < 0)
         {
             printf("[SOCKET_IP6] [socket_server_create] ERROR: listen fails\n");
             return -1;
         }

         *out_socket = server_socket;
         return 0;
     }

     int socket_ip6_server_accept ( int socket, int * out_conection_socket )
     {
         struct sockaddr_in6 client_addr;
         socklen_t sock_size ;

         // check arguments...
         if (NULL == out_conection_socket)
         {
             printf("[SOCKET_IP6] [socket_ip6_server_accept] ERROR: NULL out_conection_socket\n");
             return -1;
         }

         // Accept
         debug_info("[SOCKET_IP6] [socket_ip6_server_accept] Accept\n");

         sock_size = sizeof(struct sockaddr_in);
         *out_conection_socket = accept(socket, (struct sockaddr * ) &client_addr, &sock_size);
         if (*out_conection_socket < 0) {
             printf("[SOCKET_IP6] [socket_accept_send] ERROR: socket accept\n");
             return -1;
         }

         debug_info("[SOCKET_IP6] [socket_ip6_server_accept] accepted for %d\n", *out_conection_socket);

         return 0;
     }


     int socket_ip6_client_connect ( char *srv_name, int port, int *out_socket )
     {
         int client_fd;
         struct addrinfo hints, *res, *p;
         char port_str[6];  // max port is 65535

         // check arguments...
         if (NULL == out_socket)
         {
             printf("[SOCKET_IP6] [socket_ip6_client_connect] ERROR: NULL out_socket\n");
             return -1;
         }

	 // connecting...
         snprintf(port_str, sizeof(port_str), "%d", port);

         memset(&hints, 0, sizeof(hints));
         hints.ai_family = AF_INET6;       // IPv6 only
         hints.ai_socktype = SOCK_STREAM;  // TCP

         int status = getaddrinfo(srv_name, port_str, &hints, &res);
         if (status != 0)
         {
             fprintf(stderr, "[SOCKET_IP6] getaddrinfo error: %s\n", gai_strerror(status));
             return -1;
         }

         for (p = res; p != NULL; p = p->ai_next)
         {
             client_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
             if (client_fd == -1) {
                 continue;
	     }

             if (connect(client_fd, p->ai_addr, p->ai_addrlen) == 0)
             {
                 // Success
                 *out_socket = client_fd;
                 freeaddrinfo(res);
                 return 0;
             }

             close(client_fd);
         }

         fprintf(stderr, "[SOCKET_IP6] Connection failed to %s on port %d\n", srv_name, port);
         freeaddrinfo(res);
         return -1;
     }

     int socket_ip6_client_connect_with_retries ( char *srv_name, char *port_name, int *out_socket, int n_retries )
     {
         int ret;
         struct addrinfo hints, *res = NULL;
	 int socket_setopt_data ( int socket ) ;
         int socket_client_connect_retries ( int sd, int n_retries, struct sockaddr *ai_addr, socklen_t ai_addrlen ) ;

         debug_info("[SOCKET] [socket_ip6_client_connect_with_retries] srv_name:%s port_name:%s\n", srv_name, port_name);

	 // socket
         *out_socket = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
         if (*out_socket < 0) {
             perror("socket: ") ;
             return -1;
         }

         ret = socket_setopt_data(*out_socket) ;
         if (ret < 0) {
             close(*out_socket);
             return -1;
         }

	 // getaddrinfo
         memset(&hints, 0, sizeof(hints));
         hints.ai_family   = AF_INET6;
         hints.ai_socktype = SOCK_STREAM;

         ret = getaddrinfo(srv_name, port_name, &hints, &res);
         if ( (ret != 0) || (res == NULL) ) {
             fprintf(stderr, "getaddrinfo failed for %s:%s - %s\n", srv_name, port_name, gai_strerror(ret));
             close(*out_socket);
             return -1;
         }

         // Connect with retries
         ret = socket_client_connect_retries(*out_socket, n_retries, res->ai_addr, res->ai_addrlen) ;

         freeaddrinfo(res);

         if (ret < 0) {
             printf("[SOCKET] [socket_ip6_client_connect_with_retries] ERROR: connect fails\n");
             close(*out_socket);
             return -1;
         }

         return 0;
     }


     //
     //  address management
     //

     int socket_ip6_gethostname ( char * srv_name )
     {
         int ret ;
         struct ifaddrs *ifaddr, *ifa ;
         char ipstr[INET6_ADDRSTRLEN] ;
         struct sockaddr_in6 *sa6 ;

         debug_info("[SOCKET_IP6] [socket_ip6_gethostname] >> Begin IPv6\n");

         if (getifaddrs(&ifaddr) == -1)
         {
             perror("getifaddrs");
             return -1 ;
         }

         debug_info("[NS] [ns_get_hostname] >> Successfully retrieved network interfaces");

         for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
         {
		 // Skip empty and non-IPv6
                 if (!ifa->ifa_addr) {
                     continue;
                 }
                 if (ifa->ifa_addr->sa_family != AF_INET6) {
                     continue;
                 }

                 sa6 = (struct sockaddr_in6 *)ifa->ifa_addr;

                 // Skip link-local and loopback
                 if (IN6_IS_ADDR_LINKLOCAL(&sa6->sin6_addr)) {
                     debug_info("[NS] [ns_get_hostname] >> Skipping link-local IPv6 address");
                     continue;
                 }
                 if (IN6_IS_ADDR_LOOPBACK(&sa6->sin6_addr)) {
                     debug_info("[NS] [ns_get_hostname] >> Skipping loopback IPv6 address");
                     continue;
                 }

		 // inet_ntop...
                 inet_ntop(AF_INET6, &sa6->sin6_addr, ipstr, sizeof(ipstr));
                 debug_info("[NS] [ns_get_hostname] >> Found global IPv6 address: %s", ipstr);

                 ret = getnameinfo((struct sockaddr *)sa6, sizeof(*sa6), srv_name, sizeof(srv_name), NULL, 0, NI_NAMEREQD);
		 if (ret == 0) {
                     debug_info("Resolved hostname for IPv6 %s: %s", ipstr, hostname);
                 }
                 /*
		 if (ret == 0) {
                     debug_info("Resolved hostname for IPv6 %s: %s", ipstr, hostname);
                     break;  // Stop after first valid result
                 }
                 else {
                     debug_info("Could not resolve name for %s: %s", ipstr, gai_strerror(ret));
                 }
		 */
         }

         freeifaddrs(ifaddr);

         debug_info("[SOCKET_IP6] [socket_ip6_gethostname] >> End IPv6\n");

	 return 0 ;
     }

     int socket_ip6_gethostbyname ( char *ip, size_t ip_size, char *srv_name )
     {
         struct addrinfo hints, *res, *p;
         int status;
         struct sockaddr_in6 *ipv6 ;

         memset(&hints, 0, sizeof(hints));
         hints.ai_family = AF_INET6;      // IPv6 only
         hints.ai_socktype = SOCK_STREAM; // TCP

     	 // printf("%s %ld %s\n\n", ip, (long)ip_size, srv_name);

         status = getaddrinfo(srv_name, NULL, &hints, &res);
         if (status != 0)
	 {
     	     printf("[SOCKET_IP6] socket_ip6_gethostname");
             fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
             return -1;
         }

         for (p = res; p != NULL; p = p->ai_next)
	 {
             if (p->ai_family == AF_INET6)
	     {
                 ipv6 = (struct sockaddr_in6 *)p->ai_addr;
                 if (inet_ntop(AF_INET6, &ipv6->sin6_addr, ip, ip_size))
		 {
                     freeaddrinfo(res);
                     return 0;  // Success
                 }
		 else
		 {
                     perror("inet_ntop");
                     break;
                 }
             }
         }

         freeaddrinfo(res);
         return -1;
     }

     int socket_ip6_getsockname ( char * port_name, int new_socket )
     {
         struct sockaddr_in6 server_addr;

         socklen_t len = sizeof(server_addr);
         getsockname(new_socket, (struct sockaddr *)&server_addr, &len);
         sprintf(port_name, "%d", ntohs(server_addr.sin6_port));

         return 1;
     }


  /* ................................................................... */

