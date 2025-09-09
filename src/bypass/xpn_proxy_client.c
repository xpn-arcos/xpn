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

#include "xpn_proxy_client.h"


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
        errno = EINVAL;
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
 * Sends a message to the server and receives a response.
 * @param pr: Pointer to the request message structure.
 * @param res: Pointer to the response status structure.
 * @return: Socket descriptor on success, -1 on error.
 */
int d_send_receive(struct st_xpn_server_msg *pr, struct st_xpn_server_status *res)
{
    if (!pr || !res) {
        fprintf(stderr, "d_send_receive: null argument(s)\n");
        errno = EINVAL;
        return -1;
    }
    int ret, sd_server, ipv_proxy_server;;
    struct sockaddr_in address;
    char *env = getenv("XPN_PROXY_ADDR");
    char ip[16] = "127.0.0.1";
    int port = 5555;

    ipv_proxy_server  = utils_getenv_int("XPN_PROXY_IPV",  4);

    if (env) {
        char *sep = strchr(env, ':');
        if (sep) {
            size_t ip_len = sep - env;
            if (ip_len < sizeof(ip)) {
                strncpy(ip, env, ip_len);
                ip[ip_len] = '\0';
                port = atoi(sep + 1);
            }
        }
    }

    ret = socket_client_connect(ip, port, &sd_server, ipv_proxy_server);
    if (ret < 0) {
        perror("d_send_receive: socket_client_connect");
        return -1;
    }


/*
    sd_server = socket(AF_INET, SOCK_STREAM, 0);
    if (sd_server < 0) {
        perror("d_send_receive: socket");
        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_port = htons(port);

    ret = inet_pton(AF_INET, ip, &address.sin_addr);
    if (ret <= 0) {
        fprintf(stderr, "d_send_receive: invalid address or address not supported\n");
        close(sd_server);
        return -1;
    }

    ret = connect(sd_server, (struct sockaddr *)&address, sizeof(address));
    if (ret < 0) {
        perror("d_send_receive: connect");
        close(sd_server);
        return -1;
    }
*/
    ret = write(sd_server, (char *)pr, sizeof(struct st_xpn_server_msg));
    if (ret < 0) {
        perror("d_send_receive: write");
        close(sd_server);
        return -1;
    }

    ret = read(sd_server, (char *)res, sizeof(struct st_xpn_server_status));
    if (ret < 0) {
        perror("d_send_receive: read");
        close(sd_server);
        return -1;
    }

    return sd_server;
}

/*
 * Closes the given socket descriptor.
 * @param sd_server: Socket descriptor to close.
 * @return: 0 on success, -1 on error.
 */
int d_close(int sd_server)
{
    if (sd_server < 0) {
        fprintf(stderr, "d_close: invalid socket descriptor\n");
        errno = EBADF;
        return -1;
    }
    int ret = close(sd_server);
    if (ret < 0) {
        perror("d_close: close");
        return -1;
    }
    return 0;
}

/*
 * Opens a file on the server.
 * @param path: Path to the file.
 * @param flags: Flags for opening the file.
 * @param ...: Optional mode_t argument for file permissions.
 * @return: File descriptor on success, -1 on error.
 */
int xpn_open(const char *path, int flags, ...)
{
    if (!path) {
        fprintf(stderr, "xpn_open: path is NULL\n");
        errno = EINVAL;
        return -1;
    }
    struct st_xpn_server_msg pr;
    struct st_xpn_server_status res;

    va_list ap;
    va_start(ap, flags);
    mode_t mode = va_arg(ap, mode_t);
    va_end(ap);

    bzero(&pr, sizeof(pr));
    bzero(&res, sizeof(res));

    pr.type = XPN_SERVER_OPEN_FILE;
    pr.u_st_xpn_server_msg.op_open.flags = flags;
    pr.u_st_xpn_server_msg.op_open.mode = mode;
    strncpy(pr.u_st_xpn_server_msg.op_open.path, path, sizeof(pr.u_st_xpn_server_msg.op_open.path) - 1);

    int fd = d_send_receive(&pr, &res);
    if (fd < 0) {
        fprintf(stderr, "xpn_open: d_send_receive failed\n");
        return -1;
    }

    d_close(fd);
    return res.ret;
}

/*
 * Creates a file on the server.
 * @param path: Path to the file.
 * @param perm: Permissions for the new file.
 * @return: File descriptor on success, -1 on error.
 */
int xpn_creat(const char *path, mode_t perm)
{
    if (!path) {
        fprintf(stderr, "xpn_creat: path is NULL\n");
        errno = EINVAL;
        return -1;
    }
    struct st_xpn_server_msg pr;
    struct st_xpn_server_status res;

    bzero(&pr, sizeof(pr));
    bzero(&res, sizeof(res));

    pr.type = XPN_SERVER_CREAT_FILE;
    pr.u_st_xpn_server_msg.op_creat.flags = 0777;
    pr.u_st_xpn_server_msg.op_creat.mode = perm;
    strncpy(pr.u_st_xpn_server_msg.op_creat.path, path, sizeof(pr.u_st_xpn_server_msg.op_creat.path) - 1);

    int fd = d_send_receive(&pr, &res);
    if (fd < 0) {
        fprintf(stderr, "xpn_creat: d_send_receive failed\n");
        return -1;
    }

    d_close(fd);
    return res.ret;
}

/*
 * Closes a file on the server.
 * @param fd: File descriptor to close.
 * @return: 0 on success, -1 on error.
 */
int xpn_close (int fd)
{
    if (fd < 0)
    {
        fprintf(stderr, "xpn_close: invalid fd\n");
        errno = EBADF;
        return -1;
    }
    struct st_xpn_server_msg pr;
    struct st_xpn_server_status res;

    bzero(&pr, sizeof(pr));
    bzero(&res, sizeof(res));

    pr.type = XPN_SERVER_CLOSE_FILE;
    pr.u_st_xpn_server_msg.op_close.fd = fd;

    int sd = d_send_receive(&pr, &res);
    if (sd < 0)
    {
        fprintf(stderr, "xpn_close: d_send_receive failed\n");
        return -1;
    }

    d_close(sd);
    return res.ret;
}

/*
 * Reads data from a file on the server.
 * @param fd: File descriptor to read from.
 * @param buffer: Buffer to store the read data.
 * @param size: Number of bytes to read.
 * @return: Number of bytes read on success, -1 on error.
 */
ssize_t xpn_read(int fd, void *buffer, size_t size)
{
    if (fd < 0 || !buffer || size == 0) {
        fprintf(stderr, "xpn_read: invalid arguments\n");
        errno = EINVAL;
        return -1;
    }
    struct st_xpn_server_msg pr;
    struct st_xpn_server_status res;

    bzero(&pr, sizeof(pr));
    bzero(&res, sizeof(res));

    pr.type = XPN_SERVER_READ_FILE;
    pr.u_st_xpn_server_msg.op_read.fd = fd;
    pr.u_st_xpn_server_msg.op_read.size = size;

    int sd = d_send_receive(&pr, &res);
    if (sd < 0) {
        fprintf(stderr, "xpn_read: d_send_receive failed\n");
        return -1;
    }

    ssize_t r = read_n_bytes(sd, buffer, res.ret);
    if (r < 0) {
        fprintf(stderr, "xpn_read: read_n_bytes failed\n");
        d_close(sd);
        return -1;
    }

    d_close(sd);
    return r;
}

/*
 * Writes data to a file on the server.
 * @param fd: File descriptor to write to.
 * @param buffer: Buffer containing data to write.
 * @param size: Number of bytes to write.
 * @return: Number of bytes written on success, -1 on error.
 */
ssize_t xpn_write(int fd, const void *buffer, size_t size)
{
    if (fd < 0 || !buffer || size == 0) {
        fprintf(stderr, "xpn_write: invalid arguments\n");
        errno = EINVAL;
        return -1;
    }
    struct st_xpn_server_msg pr;
    struct st_xpn_server_status res;

    bzero(&pr, sizeof(pr));
    bzero(&res, sizeof(res));

    pr.type = XPN_SERVER_WRITE_FILE;
    pr.u_st_xpn_server_msg.op_write.fd = fd;
    pr.u_st_xpn_server_msg.op_write.size = size;

    int sd = d_send_receive(&pr, &res);
    if (sd < 0) {
        fprintf(stderr, "xpn_write: d_send_receive failed\n");
        return -1;
    }

    int ret = write(sd, buffer, size);
    if (ret < 0) {
        perror("xpn_write: write");
        d_close(sd);
        return -1;
    }

    ret = read(sd, (char *)&res, sizeof(struct st_xpn_server_status));
    if (ret < 0) {
        perror("xpn_write: read");
        d_close(sd);
        return -1;
    }

    d_close(sd);
    return res.ret;
}