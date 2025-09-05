
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


  /* ... Globl. variables / Variables globales ......................... */

     char xpnprxyclnt_ip[128] ;
     int  xpnprxyclnt_port      = 0 ;
     int  xpnprxyclnt_sd_server = -1;


  /* ... Aux. functions / Funciones Aux. ............................... */

     /*
      * If not connected then get XPN_PROXY server address and connect to it (keep socket descriptor at xpnprxyclnt_sd_server)
      * @return: 0 if OK
      */
     int xpnprxclnt_keep_connected ( void )
     {
         int    ret;
         struct sockaddr_in address;

         // new client socket
         xpnprxyclnt_sd_server = socket(AF_INET, SOCK_STREAM, 0);
         if (xpnprxyclnt_sd_server < 0) {
             perror("xpnprxclnt_send_recv: socket");
             return -1;
         }

         // connect
         bzero((char * ) &address, sizeof(address));
         address.sin_family      = AF_INET;
         address.sin_addr.s_addr = INADDR_ANY;
         address.sin_port        = htons(xpnprxyclnt_port);

         ret = inet_pton(AF_INET, xpnprxyclnt_ip, &address.sin_addr);
         if (ret <= 0) {
             fprintf(stderr, "xpnprxclnt_send_recv: invalid address or address not supported\n");
             close(xpnprxyclnt_sd_server);
             return -1;
         }

         ret = connect(xpnprxyclnt_sd_server, (struct sockaddr *)&address, sizeof(address));
         if (ret < 0) {
             perror("xpnprxclnt_send_recv: connect");
             close(xpnprxyclnt_sd_server);
             return -1;
         }

	 // return OK
         return 0 ;
     }

     /*
      * Reads exactly n bytes from a socket.
      * @param sock: Socket file descriptor.
      * @param buffer: Pointer to the buffer where data will be stored.
      * @param n: Number of bytes to read.
      * @return: Number of bytes read on success, -1 on error.
      */
     ssize_t read_n_bytes ( int sock, void *buffer, size_t n )
     {
         size_t total = 0;
         ssize_t bytes;
         char *ptr = buffer;

         // check params
         if (NULL == buffer) {
             fprintf(stderr, "read_n_bytes: NULL buffer\n");
             errno = EINVAL;
             return -1;
         }
         if (0 == n) {
             fprintf(stderr, "read_n_bytes: invalid size (0 bytes to read)\n");
             errno = EINVAL;
             return -1;
         }

         // read n bytes
         while (total < n)
         {
             bytes = read(sock, ptr + total, n - total);
             if (bytes <= 0)
             {
                 if (bytes == 0)
                      fprintf(stderr, "read_n_bytes: connection closed unexpectedly\n");
                 else perror("read_n_bytes: read");

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
      * @return: 0 if OK
      */
     int xpnprxclnt_send_recv ( struct st_xpn_server_msg *pr, struct st_xpn_server_status *res )
     {
         int ret;

         if (!pr || !res) {
             fprintf(stderr, "xpnprxclnt_send_recv: null argument(s)\n");
             errno = EINVAL;
             return -1;
         }

         ret = write(xpnprxyclnt_sd_server, (char *)pr, sizeof(struct st_xpn_server_msg));
         if (ret < 0) {
             perror("xpnprxclnt_send_recv: write");
             return -1;
         }

         bzero(res, sizeof(struct st_xpn_server_status));

         ret = read(xpnprxyclnt_sd_server, (char *)res, sizeof(struct st_xpn_server_status));
         if (ret < 0) {
             perror("xpnprxclnt_send_recv: read");
             return -1;
         }

         return 0;
     }


  /* ... Functions / Funciones ......................................... */

     /*
      * Initialize Expand proxy
      */
     int xpn_init ( void )
     {
         int    ret;
         char  *env, *sep ;
         size_t ip_len ;

         // check params
         if (xpnprxyclnt_sd_server != -1) {
             fprintf(stderr, "xpn_init: xpn_destroy must be called first\n");
             return -1;
         }

	 // default values
         strcpy(xpnprxyclnt_ip, "127.0.0.1") ;
         xpnprxyclnt_port = 5555;

	 // check environment variable XPN_PROXY_ADDR for new ip:port
         env = getenv("XPN_PROXY_ADDR");
         if (env)
         {
// <TODO>:
// XPN_PROXY_ADDR=localhost:5555
// XPN_PROXY_ADDR=localhost       <- without port too
// </TODO>
             sep = strchr(env, ':');
             if (sep)
             {
                 ip_len = sep - env;
                 if (ip_len < sizeof(xpnprxyclnt_ip)) {
                     strncpy(xpnprxyclnt_ip, env, ip_len);
                     xpnprxyclnt_ip[ip_len] = '\0';
                 }

                 xpnprxyclnt_port = atoi(sep + 1);
             }
         }

	 // keep connection
         ret = xpnprxclnt_keep_connected() ;
         if (ret < 0) {
             return -1;
         }

	 // return OK
         return 0 ;
     }


     /*
      * Finalize Expand proxy
      */
     int xpn_destroy ( void )
     {
         int ret ;

         // check params
         if (xpnprxyclnt_sd_server < 0) {
             fprintf(stderr, "xpn_destroy: xpn_init must be called first\n");
             return -1;
         }

         // close
         ret = close(xpnprxyclnt_sd_server);
         if (ret < 0) {
             perror("xpn_destroy: close");
             return -1;
         }

	 xpnprxyclnt_sd_server = -1 ;

	 // return OK
         return 0;
     }


     /*
      * Opens a file on the server.
      * @param path: Path to the file.
      * @param flags: Flags for opening the file.
      * @param ...: Optional mode_t argument for file permissions.
      * @return: File descriptor on success, -1 on error.
      */
     int xpn_open ( const char *path, int flags, ... )
     {
         struct st_xpn_server_msg    pr;
         struct st_xpn_server_status res;
	 int    ret ;

         va_list ap;
         va_start(ap, flags);
         mode_t mode = va_arg(ap, mode_t);
         va_end(ap);

	 // check params
         if (!path) {
             fprintf(stderr, "xpn_open: path is NULL\n");
             errno = EINVAL;
             return -1;
         }

	 // do request...
         bzero(&pr,  sizeof(pr));
         pr.type = XPN_SERVER_OPEN_FILE;
         pr.u_st_xpn_server_msg.op_open.flags = flags;
         pr.u_st_xpn_server_msg.op_open.mode  = mode;
         strncpy(pr.u_st_xpn_server_msg.op_open.path, path, sizeof(pr.u_st_xpn_server_msg.op_open.path) - 1);

         ret = xpnprxclnt_send_recv(&pr, &res) ;
         if (ret < 0) {
             return -1;
         }

         return res.ret;
     }

     /*
      * Creates a file on the server.
      * @param path: Path to the file.
      * @param perm: Permissions for the new file.
      * @return: File descriptor on success, -1 on error.
      */
     int xpn_creat ( const char *path, mode_t perm )
     {
         struct st_xpn_server_msg pr;
         struct st_xpn_server_status res;
	 int    ret ;

	 // check params
         if (!path) {
             fprintf(stderr, "xpn_creat: path is NULL\n");
             errno = EINVAL;
             return -1;
         }

	 // do request...
         bzero(&pr,  sizeof(pr));
         pr.type = XPN_SERVER_CREAT_FILE;
         pr.u_st_xpn_server_msg.op_creat.flags = 0777;
         pr.u_st_xpn_server_msg.op_creat.mode  = perm;
         strncpy(pr.u_st_xpn_server_msg.op_creat.path, path, sizeof(pr.u_st_xpn_server_msg.op_creat.path) - 1);

         ret = xpnprxclnt_send_recv(&pr, &res);
         if (ret < 0) {
             return -1;
         }

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
	 int    ret ;

	 // check params
         if (fd < 0)
         {
             fprintf(stderr, "xpn_close: invalid fd\n");
             errno = EBADF;
             return -1;
         }

	 // do request...
         bzero(&pr, sizeof(pr));
         pr.type = XPN_SERVER_CLOSE_FILE;
         pr.u_st_xpn_server_msg.op_close.fd = fd;

         ret = xpnprxclnt_send_recv(&pr, &res);
         if (ret < 0) {
             return -1;
         }

         return res.ret;
     }

     /*
      * Reads data from a file on the server.
      * @param fd: File descriptor to read from.
      * @param buffer: Buffer to store the read data.
      * @param size: Number of bytes to read.
      * @return: Number of bytes read on success, -1 on error.
      */
     ssize_t xpn_read ( int fd, void *buffer, size_t size )
     {
         struct st_xpn_server_msg pr;
         struct st_xpn_server_status res;
	 int    ret ;
         ssize_t r ;

	 // check params
         if (fd < 0 || !buffer || size == 0) {
             fprintf(stderr, "xpn_read: invalid arguments\n");
             errno = EINVAL;
             return -1;
         }

	 // do request...
         bzero(&pr, sizeof(pr));
         pr.type                             = XPN_SERVER_READ_FILE ;
         pr.u_st_xpn_server_msg.op_read.fd   = fd ;
         pr.u_st_xpn_server_msg.op_read.size = size ;

         ret = xpnprxclnt_send_recv(&pr, &res);
         if (ret < 0) {
             return -1;
         }

	 // read data
         r = read_n_bytes(xpnprxyclnt_sd_server, buffer, res.ret);
         if (r < 0) {
             return -1;
         }

         return r;
     }

     /*
      * Writes data to a file on the server.
      * @param fd: File descriptor to write to.
      * @param buffer: Buffer containing data to write.
      * @param size: Number of bytes to write.
      * @return: Number of bytes written on success, -1 on error.
      */
     ssize_t xpn_write ( int fd, const void *buffer, size_t size )
     {
         struct st_xpn_server_msg pr;
         struct st_xpn_server_status res;
	 int    sd, ret ;

	 // check params
         if (fd < 0 || !buffer || size == 0) {
             fprintf(stderr, "xpn_write: invalid arguments\n");
             errno = EINVAL;
             return -1;
         }

	 // do request...
         bzero(&pr, sizeof(pr));
         pr.type                              = XPN_SERVER_WRITE_FILE ;
         pr.u_st_xpn_server_msg.op_write.fd   = fd ;
         pr.u_st_xpn_server_msg.op_write.size = size ;

         sd = xpnprxclnt_send_recv(&pr, &res);
         if (sd < 0) {
             return -1;
         }

	 // write data

// <TODO>
// write in a loop similar to read_n_bytes...
         ret = write(xpnprxyclnt_sd_server, buffer, size); 
         if (ret < 0) {
             perror("xpn_write: write");
             return -1;
         }

         ret = read(xpnprxyclnt_sd_server, (char *)&res, sizeof(struct st_xpn_server_status));
         if (ret < 0) {
             perror("xpn_write: read");
             return -1;
         }
// </TODO>

         return res.ret;
     }


  /* ................................................................... */

