/*
 *  Copyright 2000-2025 Felix Garcia Carballeira, Alejandro Calderon Mateos, Diego Camarmas Alonso, Elias del Pozo Puñal
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "xpn_client/xpn.h"
#include "xpn_server/xpn_server_ops.h"


/* ... Functions / Funciones ......................................... */

/*
 * Reads exactly n bytes from a socket.
 * @param sock: Socket file descriptor.
 * @param buffer: Pointer to the buffer where data will be stored.
 * @param n: Number of bytes to read.
 * @return: Number of bytes read on success, -1 on error.
 */
ssize_t read_n_bytes(int sock, void *buffer, size_t n)
{
    if (buffer == NULL || n == 0) {
        fprintf(stderr, "read_n_bytes: invalid buffer or size\n");
        return -1;
    }
    size_t total = 0;
    ssize_t bytes;
    char *ptr = buffer;

    while (total < n)
    {
        bytes = read(sock, ptr + total, n - total);
        if (bytes <= 0)
        {
            if (bytes == 0)
                fprintf(stderr, "read_n_bytes: connection closed unexpectedly\n");
            else
                perror("read_n_bytes: read");
            return -1;
        }
        total += bytes;
    }
    return total;
}



/*
 * Handles a client request.
 * @param arg: Client socket file descriptor (as int).
 * @return: void.
 */
void handle_petition(int arg)
{
    int ret;
    int sd_client;
    struct st_xpn_server_msg pr;
    struct st_xpn_server_status res;
    res.ret = 0;

    sd_client = (int)arg;
    ret = read(sd_client, &pr, sizeof(struct st_xpn_server_msg));
    if (ret < 0) {
        perror("handle_petition: read");
        if (close(sd_client) < 0)
            perror("handle_petition: close");
        return;
    }

    switch (pr.type) {
    case XPN_SERVER_OPEN_FILE: // OPEN
        res.ret = xpn_open(pr.u_st_xpn_server_msg.op_open.path, pr.u_st_xpn_server_msg.op_open.flags, pr.u_st_xpn_server_msg.op_open.mode);
        if (write(sd_client, (char *)&res, sizeof(struct st_xpn_server_status)) < 0)
            perror("handle_petition: write OPEN");
        break;

    case XPN_SERVER_CREAT_FILE: // CREATE
        res.ret = xpn_creat(pr.u_st_xpn_server_msg.op_creat.path, pr.u_st_xpn_server_msg.op_creat.mode);
        if (write(sd_client, (char *)&res, sizeof(struct st_xpn_server_status)) < 0)
            perror("handle_petition: write CREAT");
        break;

    case XPN_SERVER_CLOSE_FILE: // CLOSE
        res.ret = xpn_close(pr.u_st_xpn_server_msg.op_close.fd);
        if (write(sd_client, (char *)&res, sizeof(struct st_xpn_server_status)) < 0)
            perror("handle_petition: write CLOSE");
        break;

    case XPN_SERVER_READ_FILE:  // READ
        char *buf = malloc(pr.u_st_xpn_server_msg.op_read.size);
        if (buf == NULL) {
            perror("handle_petition: malloc READ");
            res.ret = -1;
            ret = write(sd_client, (char *)&res, sizeof(struct st_xpn_server_status));
            break;
        }
        bzero(buf, pr.u_st_xpn_server_msg.op_read.size);

        res.ret = xpn_read(pr.u_st_xpn_server_msg.op_read.fd, buf, pr.u_st_xpn_server_msg.op_read.size);

        if (write(sd_client, (char *)&res, sizeof(struct st_xpn_server_status)) < 0)
            perror("handle_petition: write READ status");

        if (res.ret > 0) {
            if (write(sd_client, buf, res.ret) < 0)
                perror("handle_petition: write READ data");
        }
        free(buf);
        break;
    

    case XPN_SERVER_WRITE_FILE:  // WRITE
        char *buf2 = malloc(pr.u_st_xpn_server_msg.op_write.size);
        if (buf2 == NULL) {
            perror("handle_petition: malloc WRITE");
            res.ret = -1;
            ret = write(sd_client, (char *)&res, sizeof(struct st_xpn_server_status));
            break;
        }
        bzero(buf2, pr.u_st_xpn_server_msg.op_write.size);

        if (write(sd_client, (char *)&res, sizeof(struct st_xpn_server_status)) < 0)
            perror("handle_petition: write WRITE status");

        ssize_t r = read_n_bytes(sd_client, buf2, pr.u_st_xpn_server_msg.op_write.size);
        if (r < 0) {
            perror("handle_petition: read_n_bytes WRITE");
            res.ret = -1;
        } else {
            res.ret = xpn_write(pr.u_st_xpn_server_msg.op_write.fd, buf2, r);
        }

        if (res.ret > 0) {
            if (write(sd_client, (char *)&res, sizeof(struct st_xpn_server_status)) < 0)
                perror("handle_petition: write status");
        }

        free(buf2);
        break;
    

    default:
        fprintf(stderr, "handle_petition: unknown request type %d\n", pr.type);
        break;
    }

    if (close(sd_client) < 0)
        perror("handle_petition: close sd_client");
}


int do_exit = 0;

/*
 * Signal handler for SIGINT.
 * @param signo: Signal number.
 * @return: void.
 */
void sigHandler (int sig_id )
{
    if (SIGINT == sig_id) {
        do_exit = 1;
    }
}


/*
 * Main server entry point.
 * @param argc: Argument count.
 * @param argv: Argument vector.
 * @return: 0 on success, -1 on error.
 */
int main(int argc, char *argv[])
{
    int ret;
    int sd_server, sd_client;
    struct sigaction new_action, old_action;
    struct sockaddr_in address;
    int opt, addrlen;
    extern int do_exit;

    // default initial values
    do_exit = 0;

    // initialize Expand
    ret = xpn_init();
    if (ret < 0) {
        fprintf(stderr, "main: xpn_init failed\n");
        return -1;
    }

    // new server socket
    sd_server = socket(AF_INET, SOCK_STREAM, 0) ;
    if (sd_server < 0) {
        perror("main: socket");
        return -1;
    }

    opt = 1;
    if (setsockopt(sd_server, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("main: setsockopt");
        close(sd_server);
        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(5555);

    if (bind(sd_server, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("main: bind");
        close(sd_server);
        return -1;
    }

    if (listen(sd_server, 3) < 0) {
        perror("main: listen");
        close(sd_server);
        return -1;
    }

    // SIGINT -> end
    new_action.sa_handler = sigHandler;
    sigemptyset(&new_action.sa_mask);
    new_action.sa_flags = 0;
    sigaction(SIGINT, NULL, &old_action);

    if (old_action.sa_handler != SIG_IGN) {
        sigaction(SIGINT, &new_action, NULL);
    }

    while (do_exit == 0)
    {
        addrlen = sizeof(address);
        sd_client = accept(sd_server, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        if (sd_client < 0) {
            perror("main: accept");
            continue;
        }
        handle_petition(sd_client);
    }

    if (close(sd_server) < 0)
        perror("main: close sd_server");

    ret = xpn_destroy();
    if (ret < 0) {
        fprintf(stderr, "main: xpn_destroy failed\n");
        return -1;
    }

    printf("The End.\n");
    return 0;
}

/* ................................................................... */
