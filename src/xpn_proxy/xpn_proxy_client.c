
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
    #include "xpn_proxy_opendir.h"


 /* ... Global Variables / Variables globales ......................... */

    int ipv_proxy_server;
    char ip[256];
    int port;


 /* ... Internal Functions / Funciones internas ....................... */

    /*
     * Reads exactly n bytes from a socket.
     * @param sock: Socket file descriptor.
     * @param buffer: Pointer to the buffer where data will be stored.
     * @param n: Number of bytes to read.
     * @return: Number of bytes read on success, -1 on error.
     */
    ssize_t read_n_bytes(int sock, void * buffer, size_t n)
    {
        size_t total = 0;
        ssize_t bytes;
        char * ptr;

        if (buffer == NULL || n == 0) {
            printf("[XPN_PROXY_CLIENT]\t[read_n_bytes]\t%d\n", __LINE__);
            errno = EINVAL;
            return -1;
        }

        ptr = buffer;
        while (total < n)
	{
            bytes = read(sock, ptr + total, n - total);
            if (bytes <= 0)
	    {
                if (bytes == 0)
                     printf("[XPN_PROXY_CLIENT]\t[read_n_bytes]\t%d\n", __LINE__);
                else perror("read: ");

                return -1;
            }
            total += bytes;
        }
        return total;
    }

    ssize_t write_n_bytes(int sock, const void * buffer, size_t n)
    {
        ssize_t bytes;

        bytes = filesystem_write(sock, (char * ) buffer, n);
        if (bytes < 0) {
            printf("[XPN_PROXY_SERVER]\t[write_n_bytes]\t%d\n", __LINE__);
            perror("write: ");
        }

        return bytes;
    }

    /*
     * Sends a message to the server and receives a response.
     * @param pr: Pointer to the request message structure.
     * @param res: Pointer to the response status structure.
     * @return: Socket descriptor on success, -1 on error.
     */
    int d_send_receive(struct st_xpn_server_msg * pr, struct st_xpn_server_status * res, const char * path, int path_len)
    {
        int ret, sd_server;

        if (!pr || !res) {
            printf("[XPN_PROXY_CLIENT]\t[d_send_receive]\t%d\n", __LINE__);
            errno = EINVAL;
            return -1;
        }

        ret = socket_client_connect(ip, port, & sd_server, ipv_proxy_server);
        if (ret < 0) {
            perror("d_send_receive: socket_client_connect");
            printf("[XPN_PROXY_CLIENT]\t[d_send_receive]\t%d\n", __LINE__);
            return -1;
        }

        ret = write_n_bytes(sd_server, (char * ) pr, sizeof(struct st_xpn_server_msg));
        if (ret < 0) {
            printf("[XPN_PROXY_CLIENT]\t[d_send_receive]\t%d\n", __LINE__);
            close(sd_server);
            return -1;
        }

        if (path_len > XPN_PATH_MAX)
	{
            ret = write_n_bytes(sd_server, path + XPN_PATH_MAX, path_len - XPN_PATH_MAX);
            if (ret < 0) {
                printf("[XPN_PROXY_CLIENT]\t[d_send_receive]\t%d\n", __LINE__);
                close(sd_server);
                return -1;
            }
        }

        ret = read_n_bytes(sd_server, (char * ) res, sizeof(struct st_xpn_server_status));
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
    int d_send_receive_rn(struct st_xpn_server_msg * pr, struct st_xpn_server_status * res, const char * old_path, const char * new_path, int old_path_len, int new_path_len)
    {
        int ret, sd_server;

        if (!pr || !res) {
            printf("[XPN_PROXY_CLIENT]\t[d_send_receive]\t%d\n", __LINE__);
            errno = EINVAL;
            return -1;
        }

        ret = socket_client_connect(ip, port, & sd_server, ipv_proxy_server);
        if (ret < 0) {
            perror("d_send_receive: socket_client_connect");
            printf("[XPN_PROXY_CLIENT]\t[d_send_receive]\t%d\n", __LINE__);
            return -1;
        }

        ret = write_n_bytes(sd_server, (char * ) pr, sizeof(struct st_xpn_server_msg));
        if (ret < 0) {
            printf("[XPN_PROXY_CLIENT]\t[d_send_receive]\t%d\n", __LINE__);
            close(sd_server);
            return -1;
        }

        if (old_path_len > XPN_PATH_MAX)
	{
            ret = write_n_bytes(sd_server, old_path + XPN_PATH_MAX, old_path_len - XPN_PATH_MAX);
            if (ret < 0) {
                printf("[XPN_PROXY_CLIENT]\t[d_send_receive]\t%d\n", __LINE__);
                close(sd_server);
                return -1;
            }
        }

        if (new_path_len > XPN_PATH_MAX)
	{
            ret = write_n_bytes(sd_server, new_path + XPN_PATH_MAX, new_path_len - XPN_PATH_MAX);
            if (ret < 0) {
                printf("[XPN_PROXY_CLIENT]\t[d_send_receive]\t%d\n", __LINE__);
                close(sd_server);
                return -1;
            }
        }

        ret = read_n_bytes(sd_server, (char * ) res, sizeof(struct st_xpn_server_status));
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
        int ret ;

        if (sd_server < 0) {
            printf("[XPN_PROXY_CLIENT]\t[d_send_receive]\t%d\n", __LINE__);
            errno = EBADF;
            return -1;
        }

        ret = close(sd_server);
        if (ret < 0) {
            printf("[XPN_PROXY_CLIENT]\t[d_send_receive]\t%d\n", __LINE__);
            return -1;
        }

        return 0;
    }


  /* ... Functions / Funciones ......................................... */

    /*
     * Initialize internal connection
     * @return: 0 on success, -1 on error.
     */
    int xpn_init ( void )
    {
        char   * env, * sep;
        size_t ip_len, ip_size;

        // default values
        ipv_proxy_server = 4;
        strcpy(ip, "127.0.0.1");
        port = 5555;
	ip_size = 256 ; // ip defined as "char ip[256];"

        // try to get IPversion
        ipv_proxy_server = utils_getenv_int("XPN_PROXY_IPV", 4);

        // try to get IP:port
        env = getenv("XPN_PROXY_ADDR");
        if (env)
	{
            sep = strchr(env, ':');
            if (sep) // XPN_PROXY_ADDR=<ip>:<port>
	    {
                ip_len = sep - env;
                if (ip_len < ip_size) {
                    strncpy(ip, env, ip_len);
                    ip[ip_len] = '\0';
                    port = atoi(sep + 1);
                }
            }
	    else // XPN_PROXY_ADDR=<ip>
	    {
	        strncpy(ip, env, ip_size-1) ;
	    }
        }

        return 0;
    }

    /*
     * Destroy internal connection
     * @return: 0 on success, -1 on error.
     */
    int xpn_destroy(void)
    {
        return 0;
    }

    /*
     * Opens a file on the server.
     * @param path: Path to the file.
     * @param flags: Flags for opening the file.
     * @param ...: Optional mode_t argument for file permissions.
     * @return: File descriptor on success, -1 on error.
     */
    int xpn_open(const char * path, int flags, ...)
    {
        va_list ap;
        va_start(ap, flags);
        mode_t mode = va_arg(ap, mode_t);
        va_end(ap);

        struct st_xpn_server_msg pr;
        struct st_xpn_server_status res;
        int path_len, path_len_max;

        /* check params */
        if (!path) {
            printf("[XPN_PROXY_CLIENT]\t[xpn_open]\t%d\n", __LINE__);
            errno = EINVAL;
            return -1;
        }

        /* fill request */
        bzero(&pr,  sizeof(pr));
        bzero(&res, sizeof(res));

        path_len = strlen(path);
        if (path_len > XPN_PATH_MAX)
            path_len_max = XPN_PATH_MAX;
        else path_len_max = path_len;

        pr.type = XPN_SERVER_OPEN_FILE;
        pr.u_st_xpn_server_msg.op_open.flags = flags;
        pr.u_st_xpn_server_msg.op_open.mode = mode;
        memcpy(pr.u_st_xpn_server_msg.op_open.path, path, path_len_max);
        pr.u_st_xpn_server_msg.op_open.path_len = path_len;

        /* send request, receive response */
        int fd = d_send_receive( & pr, & res, path, pr.u_st_xpn_server_msg.op_open.path_len);
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
    int xpn_creat(const char * path, mode_t perm)
    {
        struct st_xpn_server_msg pr;
        struct st_xpn_server_status res;
        int path_len, path_len_max;

        /* check params */
        if (!path) {
            printf("[XPN_PROXY_CLIENT]\t[xpn_creat]\t%d\n", __LINE__);
            errno = EINVAL;
            return -1;
        }

        /* fill request */
        bzero(&pr,  sizeof(pr));
        bzero(&res, sizeof(res));

        path_len = strlen(path);
        if (path_len > XPN_PATH_MAX)
            path_len_max = XPN_PATH_MAX;
        else path_len_max = path_len;

        pr.type = XPN_SERVER_CREAT_FILE;
        pr.u_st_xpn_server_msg.op_creat.flags = 0777;
        pr.u_st_xpn_server_msg.op_creat.mode = perm;
        memcpy(pr.u_st_xpn_server_msg.op_creat.path, path, path_len_max);
        pr.u_st_xpn_server_msg.op_creat.path_len = path_len;

        /* send request, receive response */
        int fd = d_send_receive( & pr, & res, path, pr.u_st_xpn_server_msg.op_creat.path_len);
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
    int xpn_close ( int fd )
    {
        struct st_xpn_server_msg pr;
        struct st_xpn_server_status res;

        /* check params */
        if (fd < 0) {
            printf("[XPN_PROXY_CLIENT]\t[xpn_close]\t%d\n", __LINE__);
            errno = EBADF;
            return -1;
        }

        /* fill request */
        bzero(&pr,  sizeof(pr));
        bzero(&res, sizeof(res));

        pr.type = XPN_SERVER_CLOSE_FILE;
        pr.u_st_xpn_server_msg.op_close.fd = fd;

        /* send request, receive response */
        int sd = d_send_receive( & pr, & res, NULL, 0);
        if (sd < 0) {
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
    ssize_t xpn_read(int fd, void * buffer, size_t size)
    {
        struct st_xpn_server_msg pr;
        struct st_xpn_server_status res;

        /* check params */
        if (fd < 0 || !buffer || size == 0) {
            printf("[XPN_PROXY_CLIENT]\t[xpn_read]\t%d\n", __LINE__);
            errno = EINVAL;
            return -1;
        }

        /* fill request */
        bzero(&pr,  sizeof(pr));
        bzero(&res, sizeof(res));

        pr.type = XPN_SERVER_READ_FILE;
        pr.u_st_xpn_server_msg.op_read.fd = fd;
        pr.u_st_xpn_server_msg.op_read.size = size;

        /* send request, receive response */
        int sd = d_send_receive( & pr, & res, NULL, 0);
        if (sd < 0) {
            printf("[XPN_PROXY_CLIENT]\t[xpn_read]\t%d\n", __LINE__);
            return -1;
        }

        ssize_t r = read_n_bytes(sd, buffer, res.ret);
        if (r < 0) {
            printf("[XPN_PROXY_CLIENT]\t[xpn_read]\t%d\n", __LINE__);
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
    ssize_t xpn_write(int fd, const void * buffer, size_t size)
    {
        struct st_xpn_server_msg pr;
        struct st_xpn_server_status res;

        /* check params */
        if (fd < 0 || !buffer || size == 0) {
            printf("[XPN_PROXY_CLIENT]\t[xpn_write]\t%d\n", __LINE__);
            errno = EINVAL;
            return -1;
        }

        /* fill request */
        bzero(&pr,  sizeof(pr));
        bzero(&res, sizeof(res));

        pr.type = XPN_SERVER_WRITE_FILE;
        pr.u_st_xpn_server_msg.op_write.fd = fd;
        pr.u_st_xpn_server_msg.op_write.size = size;

        /* send request, receive response */
        int sd = d_send_receive( & pr, & res, NULL, 0);
        if (sd < 0) {
            printf("[XPN_PROXY_CLIENT]\t[xpn_write]\t%d\n", __LINE__);
            return -1;
        }

        int ret = write_n_bytes(sd, buffer, size);
        if (ret < 0) {
            printf("[XPN_PROXY_CLIENT]\t[xpn_write]\t%d\n", __LINE__);
            d_close(sd);
            return -1;
        }

        ret = read_n_bytes(sd, (char * ) & res, sizeof(struct st_xpn_server_status));
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
    int xpn_unlink ( const char * path )
    {
        struct st_xpn_server_msg pr;
        struct st_xpn_server_status res;
        int path_len, path_len_max;

        /* check params */
        if (!path) {
            printf("[XPN_PROXY_CLIENT]\t[xpn_unlink]\t%d\n", __LINE__);
            errno = EINVAL;
            return -1;
        }

        path_len = strlen(path);
        if (path_len > XPN_PATH_MAX)
            path_len_max = XPN_PATH_MAX;
        else path_len_max = path_len;

        /* fill request */
        bzero(&pr,  sizeof(pr));
        bzero(&res, sizeof(res));

        pr.type = XPN_SERVER_RM_FILE;

        memcpy(pr.u_st_xpn_server_msg.op_rm.path, path, path_len_max);
        pr.u_st_xpn_server_msg.op_rm.path_len = path_len;

        /* send request, receive response */
        int sd = d_send_receive( & pr, & res, path, pr.u_st_xpn_server_msg.op_rm.path_len);
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
    int xpn_rename(const char * path, const char * newpath)
    {
        struct st_xpn_server_msg pr;
        struct st_xpn_server_status res;
        int path_len, path_len_max;

        /* check params */
        if (!path || !newpath) {
            printf("[XPN_PROXY_CLIENT]\t[xpn_rename]\t%d\n", __LINE__);
            errno = EINVAL;
            return -1;
        }

        /* fill request */
        bzero(&pr,  sizeof(pr));
        bzero(&res, sizeof(res));

        path_len = strlen(path);
        if (path_len > XPN_PATH_MAX)
             path_len_max = XPN_PATH_MAX;
        else path_len_max = path_len;

        pr.type = XPN_SERVER_RENAME_FILE;
        memcpy(pr.u_st_xpn_server_msg.op_rename.old_url, path, path_len_max);
        pr.u_st_xpn_server_msg.op_rename.old_url_len = path_len;

        path_len = strlen(newpath);
        if (path_len > XPN_PATH_MAX)
             path_len_max = XPN_PATH_MAX;
        else path_len_max = path_len;

        memcpy(pr.u_st_xpn_server_msg.op_rename.new_url, newpath, path_len_max);
        pr.u_st_xpn_server_msg.op_rename.new_url_len = path_len;

        /* send request, receive response */
        int sd = d_send_receive_rn( & pr, & res, path, newpath, pr.u_st_xpn_server_msg.op_rename.old_url_len, pr.u_st_xpn_server_msg.op_rename.new_url_len);
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
    int xpn_stat(const char * path, struct stat * sb)
    {
        int ret = 0, sd = 0;
        struct st_xpn_server_msg pr;
        struct st_xpn_server_status res;
        struct st_xpn_server_attr_req st_req;
        int path_len, path_len_max;

        /* check params */
        if (!path || !sb) {
            printf("[XPN_PROXY_CLIENT]\t[xpn_stat]\t%d\n", __LINE__);
            errno = EINVAL;
            return -1;
        }

        /* fill request */
        bzero(&pr,     sizeof(pr));
        bzero(&res,    sizeof(res));
        bzero(&st_req, sizeof(st_req));

        path_len = strlen(path);
        if (path_len > XPN_PATH_MAX)
             path_len_max = XPN_PATH_MAX;
        else path_len_max = path_len;

        pr.type = XPN_SERVER_GETATTR_FILE;
        memcpy(pr.u_st_xpn_server_msg.op_getattr.path, path, path_len_max);
        pr.u_st_xpn_server_msg.op_getattr.path_len = path_len;

        /* send request, receive response */
        sd = d_send_receive( & pr, & res, path, pr.u_st_xpn_server_msg.op_getattr.path_len);
        if (sd < 0) {
            printf("[XPN_PROXY_CLIENT]\t[xpn_stat]\t%d\n", __LINE__);
            return -1;
        }
        if (res.ret != 0) {
            d_close(sd);
            return res.ret;
        }

        /* try to read 'struct stat' */
        ret = read_n_bytes(sd, (char * ) & st_req, sizeof(struct st_xpn_server_attr_req));
        if (ret < 0) {
            printf("[XPN_PROXY_CLIENT]\t[xpn_stat]\t%d\n", __LINE__);
            d_close(sd);
            return -1;
        }
        if (st_req.status != 0) {
            errno = EIO;
            printf("[XPN_PROXY_CLIENT]\t[xpn_stat]\t%d\n", __LINE__);
            d_close(sd);
            return -1;
        }

        /* copy 'struct stat' */
        memcpy(sb, & st_req.attr, sizeof(struct stat));

        d_close(sd);
        return res.ret;
    }

    /*
     * Creates a directory on the server.
     * @param path: Path to the directory to create.
     * @param perm: Permissions for the new directory.
     * @return: 0 on success, -1 on error.
     */
    int xpn_mkdir ( const char * path, mode_t perm )
    {
        struct st_xpn_server_msg pr;
        struct st_xpn_server_status res;
        int path_len, path_len_max;

        /* check params */
        if (!path) {
            printf("[XPN_PROXY_CLIENT]\t[xpn_mkdir]\t%d\n", __LINE__);
            errno = EINVAL;
            return -1;
        }

        /* fill request */
        bzero(&pr,  sizeof(pr));
        bzero(&res, sizeof(res));

        path_len = strlen(path);
        if (path_len > XPN_PATH_MAX)
             path_len_max = XPN_PATH_MAX;
        else path_len_max = path_len;

        pr.type = XPN_SERVER_MKDIR_DIR;
        pr.u_st_xpn_server_msg.op_mkdir.mode = perm;
        memcpy(pr.u_st_xpn_server_msg.op_mkdir.path, path, path_len_max);
        pr.u_st_xpn_server_msg.op_mkdir.path_len = path_len;

        /* send request, receive response */
        int sd = d_send_receive( & pr, & res, path, path_len);
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
    int xpn_rmdir(const char * path)
    {
        struct st_xpn_server_msg pr;
        struct st_xpn_server_status res;
        int path_len, path_len_max;

        /* check params */
        if (!path) {
            printf("[XPN_PROXY_CLIENT]\t[xpn_rmdir]\t%d\n", __LINE__);
            errno = EINVAL;
            return -1;
        }

        /* fill request */
        bzero(&pr,  sizeof(pr));
        bzero(&res, sizeof(res));

        path_len = strlen(path);
        if (path_len > XPN_PATH_MAX)
             path_len_max = XPN_PATH_MAX;
        else path_len_max = path_len;

        pr.type = XPN_SERVER_RMDIR_DIR;
        memcpy(pr.u_st_xpn_server_msg.op_rmdir.path, path, path_len_max);
        pr.u_st_xpn_server_msg.op_rmdir.path_len = path_len;

        /* send request, receive response */
        int sd = d_send_receive( & pr, & res, path, path_len);
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

DIR * xpn_opendir ( const char *path )
{
   int ret = 0, sd = 0;
   struct st_xpn_server_msg pr;
   struct st_xpn_server_status res;
   struct st_xpn_server_opendir_req st_req;
   int path_len, path_len_max ;
   char path_aux[PATH_MAX];
   DIR *dirp = NULL;

   if ( !path ) {
       printf("[XPN_PROXY_CLIENT]\t[xpn_opendir]\t%d\n", __LINE__);
       errno = EINVAL;
       return NULL;
   }
   bzero(&pr,     sizeof(pr));
   bzero(&res,    sizeof(res));
   bzero(&st_req, sizeof(st_req));

   path_len = strlen(path) ;
   if  (path_len > XPN_PATH_MAX)
        path_len_max = XPN_PATH_MAX;
   else path_len_max = path_len;

   pr.type = XPN_SERVER_OPENDIR_DIR;
   memcpy(pr.u_st_xpn_server_msg.op_opendir.path, path, path_len_max) ;
   pr.u_st_xpn_server_msg.op_opendir.path_len = path_len ;

   sd = d_send_receive(&pr, &res, path, path_len);
   if (sd < 0)
   {
       printf("[XPN_PROXY_CLIENT]\t[xpn_opendir]\t%d\n", __LINE__);
       return NULL;
   }
   if (res.ret != 0)
   {
       d_close(sd);
       errno = res.server_errno;
       return NULL;
   }

   ret = read_n_bytes(sd, (char *)&st_req, sizeof(struct st_xpn_server_opendir_req));
   if (ret < 0)
   {
       errno = EIO;
       printf("[XPN_PROXY_CLIENT]\t[xpn_opendir]\t%d\n", __LINE__);
       d_close(sd);
       return NULL;
   }

   if (st_req.status.ret != 0)
   {
       errno = EIO;
       printf("[XPN_PROXY_CLIENT]\t[xpn_opendir]\t%d\n", __LINE__);
       d_close(sd);
       return NULL;
   }
   if (st_req.dir == NULL) {
       errno = EIO;
       d_close(sd);
       return NULL;
   }

    strcpy(path_aux,path);
    if (path_aux[strlen(path)-1] != '/')
    {
        path_aux[strlen(path)] = '/';
        path_aux[strlen(path)+1] = '\0';
    }

    dirp = (DIR *)malloc(sizeof(DIR));
    if (dirp == NULL)
    {
        return NULL;
    }

    dirp->fd   = st_req.fd;
    dirp->path = strdup(path_aux) ;
    dirp->remote_handler = st_req.dir ;

    d_close(sd);
    return dirp;
}

    /*
     * Reads a directory entry from the server.
     * @param dirp: Pointer to DIR structure.
     * @return: Pointer to dirent structure on success, NULL on error or end of directory.
     */
    struct dirent * xpn_readdir(DIR * dirp)
    {
        static struct dirent local_dirent;

        int ret2 = 0, sd = 0;
        struct st_xpn_server_msg pr;
        struct st_xpn_server_status res;
        struct st_xpn_server_readdir_req st_req;

        /* check params */
        if (!dirp) {
            printf("[XPN_PROXY_CLIENT]\t[xpn_readdir]\t%d\n", __LINE__);
            errno = EBADF;
            return NULL;
        }

        /* fill request */
        bzero(&pr,     sizeof(pr));
        bzero(&res,    sizeof(res));
        bzero(&st_req, sizeof(st_req));

        pr.type = XPN_SERVER_READDIR_DIR;
        pr.u_st_xpn_server_msg.op_readdir.dir = dirp->remote_handler;

        /* send request, receive response */
        sd = d_send_receive( & pr, & res, NULL, 0);
        if (sd < 0) {
            printf("[XPN_PROXY_CLIENT]\t[xpn_readdir]\t%d\n", __LINE__);
            return NULL;
        }
        if (res.ret != 0) {
            d_close(sd);
            errno = res.server_errno;
            return NULL;
        }

        /* try to read 'dirent' */
        ret2 = read_n_bytes(sd, (char * ) & st_req, sizeof(struct st_xpn_server_readdir_req));
        if (ret2 < 0) {
            printf("[XPN_PROXY_CLIENT]\t[xpn_readdir]\t%d\n", __LINE__);
            d_close(sd);
            return NULL;
        }
        if (st_req.status.ret != 0) {
            errno = EIO;
            printf("[XPN_PROXY_CLIENT]\t[xpn_readdir]\t%d\n", __LINE__);
            d_close(sd);
            return NULL;
        }

        /* return 'dirent' */
        memcpy( & local_dirent, & st_req.ret, sizeof(struct dirent));
        d_close(sd);

        return &local_dirent;
    }

    /*
     * Closes a directory on the server.
     * @param dirp: Pointer to DIR structure.
     * @return: 0 on success, -1 on error.
     */
    int xpn_closedir(DIR * dirp)
    {
        int sd = 0;
        struct st_xpn_server_msg pr;
        struct st_xpn_server_status res;
        struct st_xpn_server_readdir_req st_req;

        /* check params */
        if (!dirp) {
            printf("[XPN_PROXY_CLIENT]\t[xpn_closedir]\t%d\n", __LINE__);
            errno = EBADF;
            return -1;
        }

        /* fill request */
        bzero(&pr,     sizeof(pr));
        bzero(&res,    sizeof(res));
        bzero(&st_req, sizeof(st_req));

        pr.type = XPN_SERVER_CLOSEDIR_DIR;
        pr.u_st_xpn_server_msg.op_closedir.dir = dirp->remote_handler;

        /* send request, receive response */
        sd = d_send_receive( & pr, & res, NULL, 0);
        if (sd < 0) {
            printf("[XPN_PROXY_CLIENT]\t[xpn_closedir]\t%d\n", __LINE__);
            return -1;
        }

        free (dirp->path);
        dirp->path = NULL;
        free (dirp);
        dirp = NULL;

        d_close(sd);
        return res.ret;
    }


  /* ................................................................... */

