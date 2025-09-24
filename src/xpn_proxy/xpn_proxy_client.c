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
        printf("[XPN_PROXY_CLIENT]\t[read_n_bytes]\t%d\n", __LINE__);
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
            {
                printf("[XPN_PROXY_CLIENT]\t[read_n_bytes]\t%d\n", __LINE__);
            }
            else
            {
                printf("[XPN_PROXY_CLIENT]\t[read_n_bytes]\t%d\n", __LINE__);
            }
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
int d_send_receive(struct st_xpn_server_msg *pr, struct st_xpn_server_status *res, const char *path, int path_len)
{
    if (!pr || !res) {
        printf("[XPN_PROXY_CLIENT]\t[d_send_receive]\t%d\n", __LINE__);
        errno = EINVAL;
        return -1;
    }
    int ret, sd_server, ipv_proxy_server;;
    //struct sockaddr_in address;
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
        printf("[XPN_PROXY_CLIENT]\t[d_send_receive]\t%d\n", __LINE__);
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
        printf("[XPN_PROXY_CLIENT]\t[d_send_receive]\t%d\n", __LINE__);
        close(sd_server);
        return -1;
    }

    if(path_len > XPN_PATH_MAX) 
    {
        ret = write(sd_server, path + XPN_PATH_MAX, path_len - XPN_PATH_MAX);
        if (ret < 0) {
            printf("[XPN_PROXY_CLIENT]\t[d_send_receive]\t%d\n", __LINE__);
            close(sd_server);
            return -1;
        }
    }

    ret = read(sd_server, (char *)res, sizeof(struct st_xpn_server_status));
    if (ret < 0) {
        printf("[XPN_PROXY_CLIENT]\t[d_send_receive]\t%d\n", __LINE__);
        close(sd_server);
        return -1;
    }

    return sd_server;
}

/*
 * Sends a message to the server and receives a response.
 * @param pr: Pointer to the request message structure.
 * @param res: Pointer to the response status structure.
 * @return: Socket descriptor on success, -1 on error.
 */
int d_send_receive_rn(struct st_xpn_server_msg *pr, struct st_xpn_server_status *res, const char *old_path, const char *new_path, int old_path_len, int new_path_len)
{
    if (!pr || !res) {
        printf("[XPN_PROXY_CLIENT]\t[d_send_receive]\t%d\n", __LINE__);
        errno = EINVAL;
        return -1;
    }
    int ret, sd_server, ipv_proxy_server;;
    //struct sockaddr_in address;
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
        printf("[XPN_PROXY_CLIENT]\t[d_send_receive]\t%d\n", __LINE__);
        return -1;
    }

    ret = write(sd_server, (char *)pr, sizeof(struct st_xpn_server_msg));
    if (ret < 0) {
        printf("[XPN_PROXY_CLIENT]\t[d_send_receive]\t%d\n", __LINE__);
        close(sd_server);
        return -1;
    }

    if (old_path_len > XPN_PATH_MAX) 
    {
        ret = write(sd_server, old_path + XPN_PATH_MAX, old_path_len - XPN_PATH_MAX);
        if (ret < 0) {
            printf("[XPN_PROXY_CLIENT]\t[d_send_receive]\t%d\n", __LINE__);
            close(sd_server);
            return -1;
        }
    }

    if (new_path_len > XPN_PATH_MAX) 
    {
        ret = write(sd_server, new_path + XPN_PATH_MAX, new_path_len - XPN_PATH_MAX);
        if (ret < 0) {
            printf("[XPN_PROXY_CLIENT]\t[d_send_receive]\t%d\n", __LINE__);
            close(sd_server);
            return -1;
        }
    }

    ret = read(sd_server, (char *)res, sizeof(struct st_xpn_server_status));
    if (ret < 0) {
        printf("[XPN_PROXY_CLIENT]\t[d_send_receive]\t%d\n", __LINE__);
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
        printf("[XPN_PROXY_CLIENT]\t[d_send_receive]\t%d\n", __LINE__);
        errno = EBADF;
        return -1;
    }
    int ret = close(sd_server);
    if (ret < 0) {
        printf("[XPN_PROXY_CLIENT]\t[d_send_receive]\t%d\n", __LINE__);
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
        printf("[XPN_PROXY_CLIENT]\t[xpn_open]\t%d\n", __LINE__);
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

    if (strlen(path) > XPN_PATH_MAX) 
    {
        strncpy(pr.u_st_xpn_server_msg.op_open.path, path, XPN_PATH_MAX);
        //pr.u_st_xpn_server_msg.op_open.path[127] = '\0';
    }else
    {
        strncpy(pr.u_st_xpn_server_msg.op_open.path, path, sizeof(pr.u_st_xpn_server_msg.op_open.path) - 1);
    }

    pr.u_st_xpn_server_msg.op_open.path_len = strlen(path);
    int fd = d_send_receive(&pr, &res, path, pr.u_st_xpn_server_msg.op_open.path_len);
    if (fd < 0) {
        printf("[XPN_PROXY_CLIENT]\t[xpn_open]\t%d\n", __LINE__);
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
    if ( !path ) 
    {
        printf("[XPN_PROXY_CLIENT]\t[xpn_creat]\t%d\n", __LINE__);
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
    bzero(pr.u_st_xpn_server_msg.op_creat.path, sizeof(pr.u_st_xpn_server_msg.op_creat.path));
    //strncpy(pr.u_st_xpn_server_msg.op_creat.path, path, sizeof(pr.u_st_xpn_server_msg.op_creat.path) - 1);

    if (strlen(path) > XPN_PATH_MAX) 
    {
        strncpy(pr.u_st_xpn_server_msg.op_creat.path, path, XPN_PATH_MAX);
        //pr.u_st_xpn_server_msg.op_creat.path[127] = '\0';
    }else
    {
        strncpy(pr.u_st_xpn_server_msg.op_creat.path, path, strlen(path));
    }

    pr.u_st_xpn_server_msg.op_creat.path_len = strlen(path);

    int fd = d_send_receive(&pr, &res, path, pr.u_st_xpn_server_msg.op_creat.path_len);
    if (fd < 0) {
        printf("[XPN_PROXY_CLIENT]\t[xpn_creat]\t%d\n", __LINE__);
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
        printf("[XPN_PROXY_CLIENT]\t[xpn_close]\t%d\n", __LINE__);
        errno = EBADF;
        return -1;
    }
    struct st_xpn_server_msg pr;
    struct st_xpn_server_status res;

    bzero(&pr, sizeof(pr));
    bzero(&res, sizeof(res));

    pr.type = XPN_SERVER_CLOSE_FILE;
    pr.u_st_xpn_server_msg.op_close.fd = fd;

    int sd = d_send_receive(&pr, &res, NULL, 0);
    if (sd < 0)
    {
        printf("[XPN_PROXY_CLIENT]\t[xpn_close]\t%d\n", __LINE__);
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
        printf("[XPN_PROXY_CLIENT]\t[xpn_read]\t%d\n", __LINE__);
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

    int sd = d_send_receive(&pr, &res, NULL, 0);
    if (sd < 0) {
        printf("[XPN_PROXY_CLIENT]\t[xpn_read]\t%d\n", __LINE__);
        return -1;
    }

    ssize_t r = read_n_bytes(sd, buffer, res.ret);
    if (r < 0) {
        printf("[XPN_PROXY_CLIENT]\t[xpn_read]\t%d\n", __LINE__);
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
        printf("[XPN_PROXY_CLIENT]\t[xpn_write]\t%d\n", __LINE__);
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

    int sd = d_send_receive(&pr, &res, NULL, 0);
    if (sd < 0) {
        printf("[XPN_PROXY_CLIENT]\t[xpn_write]\t%d\n", __LINE__);
        return -1;
    }

    int ret = write(sd, buffer, size);
    if (ret < 0) {
        printf("[XPN_PROXY_CLIENT]\t[xpn_write]\t%d\n", __LINE__);
        d_close(sd);
        return -1;
    }

    ret = read(sd, (char *)&res, sizeof(struct st_xpn_server_status));
    if (ret < 0) {
        printf("[XPN_PROXY_CLIENT]\t[xpn_write]\t%d\n", __LINE__);
        d_close(sd);
        return -1;
    }

    d_close(sd);
    return res.ret;
}

/*
 * Removes a file on the server.
 * @param path: Path to the file to remove.
 * @return: 0 on success, -1 on error.
 */
int xpn_unlink (const char *path)
{
    if ( !path ) {
        printf("[XPN_PROXY_CLIENT]\t[xpn_unlink]\t%d\n", __LINE__);
        errno = EINVAL;
        return -1;
    }
    struct st_xpn_server_msg pr;
    struct st_xpn_server_status res;

    bzero(&pr, sizeof(pr));
    bzero(&res, sizeof(res));

    pr.type = XPN_SERVER_RM_FILE;
    //strncpy(pr.u_st_xpn_server_msg.op_rm.path, path, sizeof(pr.u_st_xpn_server_msg.op_rm.path) - 1);

    if (strlen(path) > XPN_PATH_MAX) 
    {
        strncpy(pr.u_st_xpn_server_msg.op_rm.path, path, XPN_PATH_MAX);
        //pr.u_st_xpn_server_msg.op_rm.path[127] = '\0';
    }else
    {
        strncpy(pr.u_st_xpn_server_msg.op_rm.path, path, sizeof(pr.u_st_xpn_server_msg.op_rm.path) - 1);
    }

    pr.u_st_xpn_server_msg.op_rm.path_len = strlen(path);

    int sd = d_send_receive(&pr, &res, path, pr.u_st_xpn_server_msg.op_rm.path_len);
    if (sd < 0) {
        printf("[XPN_PROXY_CLIENT]\t[xpn_unlink]\t%d\n", __LINE__);
        return -1;
    }

    d_close(sd);
    return res.ret;
}

/*
 * Renames a file or directory on the server.
 * @param path: Current path of the file or directory.
 * @param newpath: New path for the file or directory.
 * @return: 0 on success, -1 on error.
 */
int xpn_rename (const char *path, const char *newpath)
{
    if ( !path || !newpath ) {
        printf("[XPN_PROXY_CLIENT]\t[xpn_rename]\t%d\n", __LINE__);
        errno = EINVAL;
        return -1;
    }
    struct st_xpn_server_msg pr;
    struct st_xpn_server_status res;

    bzero(&pr, sizeof(pr));
    bzero(&res, sizeof(res));

    pr.type = XPN_SERVER_RENAME_FILE;
    //strncpy(pr.u_st_xpn_server_msg.op_rename.old_url, path, sizeof(pr.u_st_xpn_server_msg.op_rename.old_url) - 1);
    
    if (strlen(path) > XPN_PATH_MAX) 
    {
        strncpy(pr.u_st_xpn_server_msg.op_rename.old_url, path, XPN_PATH_MAX);
        //pr.u_st_xpn_server_msg.op_rename.old_url[127] = '\0';
    }else
    {
        strncpy(pr.u_st_xpn_server_msg.op_rename.old_url, path, sizeof(pr.u_st_xpn_server_msg.op_rename.old_url) - 1);
    }
    
    //strncpy(pr.u_st_xpn_server_msg.op_rename.new_url, newpath, sizeof(pr.u_st_xpn_server_msg.op_rename.new_url) - 1);

    if (strlen(newpath) > XPN_PATH_MAX) 
    {
        strncpy(pr.u_st_xpn_server_msg.op_rename.new_url, newpath, XPN_PATH_MAX);
        //pr.u_st_xpn_server_msg.op_rename.new_url[127] = '\0';
    }else
    {
        strncpy(pr.u_st_xpn_server_msg.op_rename.new_url, newpath, sizeof(pr.u_st_xpn_server_msg.op_rename.new_url) - 1);
    }

    pr.u_st_xpn_server_msg.op_rename.old_url_len = strlen(path);
    pr.u_st_xpn_server_msg.op_rename.new_url_len = strlen(newpath);

    int sd = d_send_receive_rn(&pr, &res, path, newpath, pr.u_st_xpn_server_msg.op_rename.old_url_len, pr.u_st_xpn_server_msg.op_rename.new_url_len);
    if (sd < 0) {
        printf("[XPN_PROXY_CLIENT]\t[xpn_rename]\t%d\n", __LINE__);
        return -1;
    }

    d_close(sd);
    return res.ret;
}

/*
 * Retrieves file or directory attributes from the server.
 * @param path: Path to the file or directory.
 * @param sb: Pointer to a stat structure to store the attributes.
 * @return: 0 on success, -1 on error.
 */
int xpn_stat (const char *path, struct stat *sb)
{
    if ( !path || !sb ) {
        printf("[XPN_PROXY_CLIENT]\t[xpn_stat]\t%d\n", __LINE__);
        errno = EINVAL;
        return -1;
    }

    int ret = 0, sd = 0;
    struct st_xpn_server_msg pr;
    struct st_xpn_server_status res;
    struct st_xpn_server_attr_req st_req;

    bzero(&pr, sizeof(pr));
    bzero(&res, sizeof(res));
    bzero(&st_req, sizeof(st_req));

    pr.type = XPN_SERVER_GETATTR_FILE;

    if (strlen(path) > XPN_PATH_MAX) 
    {
        strncpy(pr.u_st_xpn_server_msg.op_getattr.path, path, XPN_PATH_MAX);
        //pr.u_st_xpn_server_msg.op_getattr.path[127] = '\0';
    }else
    {
        strncpy(pr.u_st_xpn_server_msg.op_getattr.path, path, sizeof(pr.u_st_xpn_server_msg.op_getattr.path) - 1);
    }

    pr.u_st_xpn_server_msg.op_getattr.path_len = strlen(path);

    sd = d_send_receive(&pr, &res, path, pr.u_st_xpn_server_msg.op_getattr.path_len);
    if (sd < 0) 
    {
        printf("[XPN_PROXY_CLIENT]\t[xpn_stat]\t%d\n", __LINE__);
        return -1;
    }

    if (res.ret == 0)
    {
        ret = read(sd, (char *)&st_req, sizeof(struct st_xpn_server_attr_req));
        if (ret < 0) 
        {
            printf("[XPN_PROXY_CLIENT]\t[xpn_stat]\t%d\n", __LINE__);
            d_close(sd);
            return -1;
        }

        if (st_req.status == 0) 
        {
            memcpy(sb, &st_req.attr, sizeof(struct stat));
        } else 
        {
            printf("[XPN_PROXY_CLIENT]\t[xpn_stat]\t%d\n", __LINE__);
            errno = EIO; 
            d_close(sd);
            return -1;
        }
    }

    d_close(sd);
    return res.ret;
}

/*
 * Creates a directory on the server.
 * @param path: Path to the directory to create.
 * @param perm: Permissions for the new directory.
 * @return: 0 on success, -1 on error.
 */
int xpn_mkdir (const char *path, mode_t perm)
{
    if ( !path ) {
        printf("[XPN_PROXY_CLIENT]\t[xpn_mkdir]\t%d\n", __LINE__);
        errno = EINVAL;
        return -1;
    }
    struct st_xpn_server_msg pr;
    struct st_xpn_server_status res;

    bzero(&pr, sizeof(pr));
    bzero(&res, sizeof(res));

    pr.type = XPN_SERVER_MKDIR_DIR;
    int len_path = strlen(path);

    if (len_path > XPN_PATH_MAX) 
    {
        strncpy(pr.u_st_xpn_server_msg.op_mkdir.path, path, XPN_PATH_MAX);
    }else
    {
        strncpy(pr.u_st_xpn_server_msg.op_mkdir.path, path, len_path);
    }
    pr.u_st_xpn_server_msg.op_mkdir.mode = perm;
    pr.u_st_xpn_server_msg.op_mkdir.path_len = len_path;

    int sd = d_send_receive(&pr, &res, path, pr.u_st_xpn_server_msg.op_mkdir.path_len);
    if (sd < 0) {
        printf("[XPN_PROXY_CLIENT]\t[xpn_mkdir]\t%d\n", __LINE__);
        return -1;
    }

    d_close(sd);
    return res.ret;
}

/*
 * Removes a directory on the server.
 * @param path: Path to the directory to remove.
 * @return: 0 on success, -1 on error.
 */
int xpn_rmdir (const char *path )
{
    if ( !path ) {
        printf("[XPN_PROXY_CLIENT]\t[xpn_rmdir]\t%d\n", __LINE__);
        errno = EINVAL;
        return -1;
    }
    struct st_xpn_server_msg pr;
    struct st_xpn_server_status res;

    bzero(&pr, sizeof(pr));
    bzero(&res, sizeof(res));

    pr.type = XPN_SERVER_RMDIR_DIR;

    if (strlen(path) > XPN_PATH_MAX) 
    {
        strncpy(pr.u_st_xpn_server_msg.op_rmdir.path, path, XPN_PATH_MAX);
        //pr.u_st_xpn_server_msg.op_rmdir.path[127] = '\0';
    }else
    {
        strncpy(pr.u_st_xpn_server_msg.op_rmdir.path, path, sizeof(pr.u_st_xpn_server_msg.op_rmdir.path) - 1);
    }

    pr.u_st_xpn_server_msg.op_rmdir.path_len = strlen(path);

    int sd = d_send_receive(&pr, &res, path, strlen(path));
    if (sd < 0) {
        printf("[XPN_PROXY_CLIENT]\t[xpn_rmdir]\t%d\n", __LINE__);
        return -1;
    }

    d_close(sd);
    return res.ret;
}

/*
 * Opens a directory on the server.
 * @param path: Path to the directory.
 * @return: Pointer to DIR structure on success, NULL on error.
 */
DIR * xpn_opendir (const char *path)
{
    if ( !path ) {
        printf("[XPN_PROXY_CLIENT]\t[xpn_opendir]\t%d\n", __LINE__);
        errno = EINVAL;
        return NULL;
    }

    int ret = 0, sd = 0;
    struct st_xpn_server_msg pr;
    struct st_xpn_server_status res;
    struct st_xpn_server_opendir_req st_req;

    bzero(&pr, sizeof(pr));
    bzero(&res, sizeof(res));
    bzero(&st_req, sizeof(st_req));

    pr.type = XPN_SERVER_OPENDIR_DIR;

    if (strlen(path) > XPN_PATH_MAX) 
    {
        strncpy(pr.u_st_xpn_server_msg.op_opendir.path, path, XPN_PATH_MAX);
    }else
    {
        strncpy(pr.u_st_xpn_server_msg.op_opendir.path, path, sizeof(pr.u_st_xpn_server_msg.op_opendir.path) - 1);
    }

    pr.u_st_xpn_server_msg.op_opendir.path_len = strlen(path);
    //strncpy(pr.u_st_xpn_server_msg.op_opendir.path, path, sizeof(pr.u_st_xpn_server_msg.op_opendir.path) - 1);

    sd = d_send_receive(&pr, &res, path, strlen(path));
    if (sd < 0) 
    {
        printf("[XPN_PROXY_CLIENT]\t[xpn_opendir]\t%d\n", __LINE__);
        return NULL;
    }

    if (res.ret == 0)
    {
        ret = read(sd, (char *)&st_req, sizeof(struct st_xpn_server_opendir_req));
        if (ret < 0) 
        {
            printf("[XPN_PROXY_CLIENT]\t[xpn_opendir]\t%d\n", __LINE__);
            d_close(sd);
            errno = EIO;
            return NULL;
        }

        if (st_req.status.ret != 0) 
        {
            printf("[XPN_PROXY_CLIENT]\t[xpn_opendir]\t%d\n", __LINE__);
            errno = EIO; 
            d_close(sd);
            return NULL;
        }
        else
        {
            d_close(sd);
            if (st_req.dir == NULL) {
                errno = EIO;
                return NULL;
            }
            return st_req.dir;
        }
    }

    d_close(sd);
    errno = res.server_errno;
    return NULL;
}

/*
 * Reads a directory entry from the server.
 * @param dirp: Pointer to DIR structure.
 * @return: Pointer to dirent structure on success, NULL on error or end of directory.
 */
struct dirent* xpn_readdir (DIR *dirp)
{
    static struct dirent local_dirent;;

    if ( !dirp ) 
    {
        printf("[XPN_PROXY_CLIENT]\t[xpn_readdir]\t%d\n", __LINE__);
        errno = EBADF;
        return NULL;
    }

    int ret2 = 0, sd = 0;
    struct st_xpn_server_msg pr;
    struct st_xpn_server_status res;
    struct st_xpn_server_readdir_req st_req;

    bzero(&pr, sizeof(pr));
    bzero(&res, sizeof(res));
    bzero(&st_req, sizeof(st_req));

    pr.type = XPN_SERVER_READDIR_DIR;
    pr.u_st_xpn_server_msg.op_readdir.dir = dirp;

    sd = d_send_receive(&pr, &res, NULL, 0);
    if (sd < 0) 
    {
        printf("[XPN_PROXY_CLIENT]\t[xpn_readdir]\t%d\n", __LINE__);
        return NULL;
    }

    if (res.ret == 0)
    {
        ret2 = read(sd, (char *)&st_req, sizeof(struct st_xpn_server_readdir_req));
        if (ret2 < 0) 
        {
            printf("[XPN_PROXY_CLIENT]\t[xpn_readdir]\t%d\n", __LINE__);
            d_close(sd);
            return NULL;
        }

        if (st_req.status.ret != 0) 
        {
            printf("[XPN_PROXY_CLIENT]\t[xpn_readdir]\t%d\n", __LINE__);
            errno = EIO; 
            d_close(sd);
            return NULL;
        }
        else
        {
            memcpy(&local_dirent, &st_req.ret, sizeof(struct dirent));

            d_close(sd);
            return &local_dirent; 
        }
    }

    d_close(sd);
    errno = res.server_errno;
    return NULL;
}


/*
 * Closes a directory on the server.
 * @param dirp: Pointer to DIR structure.
 * @return: 0 on success, -1 on error.
 */
int xpn_closedir (DIR *dirp)
{
    if ( !dirp ) 
    {
        printf("[XPN_PROXY_CLIENT]\t[xpn_readdir]\t%d\n", __LINE__);
        errno = EBADF;
        return -1;
    }

    int sd = 0;
    struct st_xpn_server_msg pr;
    struct st_xpn_server_status res;
    struct st_xpn_server_readdir_req st_req;

    bzero(&pr, sizeof(pr));
    bzero(&res, sizeof(res));
    bzero(&st_req, sizeof(st_req));

    pr.type = XPN_SERVER_CLOSEDIR_DIR;
    pr.u_st_xpn_server_msg.op_closedir.dir = dirp;

        sd = d_send_receive(&pr, &res, NULL, 0);
    if (sd < 0) 
    {
        printf("[XPN_PROXY_CLIENT]\t[xpn_readdir]\t%d\n", __LINE__);
        return -1;
    }

    d_close(sd);
    return res.ret;
}


/* ................................................................... */

