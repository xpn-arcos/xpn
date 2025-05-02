
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


int socket_ip6_server_create ( int * out_socket, int port )
{
    int ret = 0;
    struct sockaddr_in6 server_addr;

    int server_socket = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
    if (server_socket < 0)
    {
        printf("[SOCKET] [socket_server_create] ERROR: socket fails\n");
        return -1;
    }

    int val = 1;
    ret = setsockopt(server_socket, IPPROTO_TCP, TCP_NODELAY, & val, sizeof(val));
    if (ret < 0)
    {
        printf("[SOCKET] [socket_server_create] ERROR: setsockopt fails\n");
        return -1;
    }

    debug_info("[SOCKET] [socket_server_create] Socket reuseaddr\n");

    val = 1;
    ret = setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (char * ) & val, sizeof(int));
    if (ret < 0)
    {
        printf("[SOCKET] [socket_server_create] ERROR: setsockopt fails\n");
        return -1;
    }

    // bind
    debug_info("[SOCKET] [socket_server_create] Socket bind\n");

    bzero((char * ) & server_addr, sizeof(server_addr));
    server_addr.sin6_family = AF_INET6;
    server_addr.sin6_addr = in6addr_any;
    server_addr.sin6_port = htons(port);

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

    * out_socket = server_socket;
    return 0;
}

int socket_ip6_server_accept ( int socket, int * out_conection_socket )
{
    struct sockaddr_in6 client_addr;
    socklen_t sock_size = sizeof(struct sockaddr_in);

    int new_socket = accept(socket, (struct sockaddr * ) & client_addr, & sock_size);
    if (new_socket < 0) {
        printf("[SOCKET] [socket_accept_send] ERROR: socket accept\n");
        return -1;
    }

    * out_conection_socket = new_socket;
    return 0;
}

/*int socket_ip6_client_connect ( char * srv_name, int port, int * out_socket )
{
    int client_fd;
    struct sockaddr_in6 serv_addr;

    client_fd = socket(AF_INET6, SOCK_STREAM, 0);
    if (client_fd < 0)
    {
        printf("[SOCKET] [socket_read] ERROR: socket creation error\n");
        return -1;
    }

    struct hostent * hp;
    hp = gethostbyname(srv_name);
    if (hp == NULL)
    {
        printf("[SOCKET] [socket_read] ERROR: gethostbyname srv_name: %s\n", srv_name);
        close(client_fd);
        return -1;
    }

    bzero((char * ) & serv_addr, sizeof(serv_addr));
    serv_addr.sin6_family = AF_INET6;
    serv_addr.sin6_port   = htons(port);
    memcpy( & (serv_addr.sin6_addr), hp -> h_addr, hp -> h_length);
    inet_pton(AF_INET6, "::1", &serv_addr.sin6_addr);

    int status = connect(client_fd, (struct sockaddr * ) & serv_addr, sizeof(serv_addr));
    if (status < 0)
    {
        printf("[SOCKET] [socket_read] ERROR: socket connection failed to %s in port %d %s\n", srv_name, port, strerror(errno));
        close(client_fd);
        return -1;
    }

    * out_socket = client_fd;
    return 0;
}*/

int socket_ip6_client_connect(char *srv_name, int port, int *out_socket)
{
    int client_fd;
    struct addrinfo hints, *res, *p;
    char port_str[6];  // max port is 65535

    snprintf(port_str, sizeof(port_str), "%d", port);

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET6;       // IPv6 only
    hints.ai_socktype = SOCK_STREAM;  // TCP

    int status = getaddrinfo(srv_name, port_str, &hints, &res);
    if (status != 0)
    {
        fprintf(stderr, "[SOCKET] getaddrinfo error: %s\n", gai_strerror(status));
        return -1;
    }

    for (p = res; p != NULL; p = p->ai_next)
    {
        client_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (client_fd == -1)
            continue;

        if (connect(client_fd, p->ai_addr, p->ai_addrlen) == 0)
        {
            // Success
            *out_socket = client_fd;
            freeaddrinfo(res);
            return 0;
        }

        close(client_fd);
    }

    fprintf(stderr, "[SOCKET] Connection failed to %s on port %d\n", srv_name, port);
    freeaddrinfo(res);
    return -1;
}

/*
int socket_ip6_gethostbyname ( char * ip, size_t ip_size, char * srv_name )
{
    struct hostent *srv_entry;
    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET6; // IPv6
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(srv_name, NULL, &hints, &res) != 0) 
    {
        perror("getaddrinfo");
        return -1;
    }

    struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)res->ai_addr;
    inet_ntop(AF_INET6, &ipv6->sin6_addr, ip, ip_size);  
    //printf("IPv6 address: %s\n", ip);

    freeaddrinfo(res); 
    return 0;
}*/


int socket_ip6_gethostbyname(char *ip, size_t ip_size, char *srv_name)
{
    struct addrinfo hints, *res, *p;
    int status;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET6;      // IPv6 only
    hints.ai_socktype = SOCK_STREAM; // TCP

    status = getaddrinfo(srv_name, NULL, &hints, &res);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return -1;
    }

    for (p = res; p != NULL; p = p->ai_next) {
        if (p->ai_family == AF_INET6) {
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            if (inet_ntop(AF_INET6, &ipv6->sin6_addr, ip, ip_size)) {
                freeaddrinfo(res);
                return 0;  // Success
            } else {
                perror("inet_ntop");
                break;
            }
        }
    }

    freeaddrinfo(res);
    return -1;
}

/* ................................................................... */
