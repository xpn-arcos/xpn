
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
    #include <pthread.h>
    #include <sys/queue.h>

    #include "base/socket.h"
    #include "base/service_socket.h"
    #include "xpn_client/xpn.h"
    #include "xpn_server/xpn_server_ops.h"

    #include "xpn_proxy/xpn_proxy_macros.h"


 /* ... Global items / Elementos globales ............................. */

    #define THREAD_POOL_SIZE 32

    int do_exit = 0;

    // Mutex and condition variable for queue
    pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t  queue_cond  = PTHREAD_COND_INITIALIZER;

    // Node for client socket queue
    typedef struct client_node {
        int sd_client;
        TAILQ_ENTRY(client_node) nodes;
    } client_node_t;

    // Queue head for client sockets
    TAILQ_HEAD(client_queue, client_node);
    struct client_queue client_q = TAILQ_HEAD_INITIALIZER(client_q);


 /* ... Functions / Funciones ......................................... */

    /*
     * Reads exactly n bytes from a socket.
     * @param sock: Socket file descriptor.
     * @param buffer: Pointer to the buffer where data will be stored.
     * @param n: Number of bytes to read.
     * @return: Number of bytes read on success, -1 on error.
     */
    ssize_t read_n_bytes ( int sock, void *buffer, size_t n )
    {
        size_t  total = 0;
        ssize_t bytes ;
        char *ptr ;

        if ( (buffer == NULL) || (n == 0) )
	{
            fprintf(stderr, "read_n_bytes: invalid buffer or size\n");
            printf("[XPN_PROXY_SERVER]\t[read_n_bytes]\t%d\n", __LINE__);
            return -1;
        }

        ptr = (char *)buffer ;
        while (total < n)
        {
            bytes = read(sock, ptr + total, n - total);
            if (bytes <= 0)
            {
                printf("[XPN_PROXY_SERVER]\t[read_n_bytes]\t%d\n", __LINE__);
                if (bytes == 0)
                     fprintf(stderr, "connection closed unexpectedly\n");
		else perror("read: ");

                return -1;
            }

            total += bytes;
        }

        return total;
    }

    ssize_t write_n_bytes ( int sock, void *buffer, size_t n )
    {
        ssize_t bytes ;

	bytes = filesystem_write(sock, (char *)buffer, n) ;
	if (bytes < 0) {
	    printf("[XPN_PROXY_SERVER]\t[write_n_bytes]\t%d\n", __LINE__);
	    perror("write: ") ;
	}

	return bytes ;
    }

    int xpn_proxy_server_read_fullpath ( int sd_client, char *full_path, int path_len, char *path_src )
    {
	ssize_t r ;

	if (path_len <= XPN_PATH_MAX)
	{
	    memcpy(full_path, path_src, path_len) ;
	    full_path[path_len] = '\0' ;
            return 0 ;
	}

	// path_len > XPN_PATH_MAX
	memcpy(full_path, path_src, XPN_PATH_MAX) ;

	r = read_n_bytes(sd_client, full_path + XPN_PATH_MAX, path_len - XPN_PATH_MAX) ;
	if (r < 0) {
	    full_path[0] = '\0' ;
            return -1 ;
	}

	full_path[path_len] = '\0' ;
        return 0 ;
    }

    /*
     * Worker thread function for the thread pool.
     * Waits for client sockets in the queue and processes them.
     */
    void *worker_thread ( void *arg )
    {
        (void)arg ;
        int handle_petition(int arg) ;
	int ret ;

        while (1)
        {
            pthread_mutex_lock(&queue_mutex);
            while (TAILQ_EMPTY(&client_q) && !do_exit) {
                   pthread_cond_wait(&queue_cond, &queue_mutex);
            }
            if (do_exit) {
                pthread_mutex_unlock(&queue_mutex);
                break;
            }

            client_node_t *node = TAILQ_FIRST(&client_q);
            if (node) {
                TAILQ_REMOVE(&client_q, node, nodes);
            }

            pthread_mutex_unlock(&queue_mutex);

            if (node)
	    {
                ret = handle_petition(node->sd_client);
		if (ret < 0) {
		    printf("[XPN_PROXY_SERVER]\t[handle_petition]\t%d\n", __LINE__) ;
		    printf("error detected in handle_petition(...).\n") ;
		}

		ret = close(node->sd_client) ;
		if (ret < 0) {
		    printf("[XPN_PROXY_SERVER]\t[handle_petition]\t%d\n", __LINE__) ;
		    perror("close: ") ;
		}

                free(node) ;
            }
        }

        return NULL;
    }


    /*
     * Handles a client request.
     * @param arg: Client socket file descriptor (as int).
     * @return: integer 0 if OK and -1 if error found.
     */
    int handle_petition ( int arg )
    {
        int    ret ;
        int    sd_client;
        struct st_xpn_server_msg    pr;
        struct st_xpn_server_status res;
        char   full_path[PATH_MAX], full_path_old[PATH_MAX], full_path_new[PATH_MAX];
	int    path_len;
	char  *path_src;
        char  *buf, *buf2 ;
        struct st_xpn_server_opendir_req req_opendir;
        struct st_xpn_server_readdir_req ret_entry;
        struct st_xpn_server_attr_req    req_attr;
        DIR   *ret_od ;
        struct dirent * ret_readdir;

        res.ret = 0;
        sd_client = (int)arg;

        ret = read_n_bytes(sd_client, &pr, sizeof(struct st_xpn_server_msg)) ;
        if (ret < 0) {
            printf("[XPN_PROXY_SERVER]\t[handle_petition]\t%d\n", __LINE__);
	    perror("read: ") ;
            return -1 ;
        }

        switch (pr.type)
        {
           case XPN_SERVER_OPEN_FILE: // OPEN

		// read full path
	        path_len = pr.u_st_xpn_server_msg.op_open.path_len;
	        path_src = pr.u_st_xpn_server_msg.op_open.path ;
	        ret = xpn_proxy_server_read_fullpath(sd_client, full_path, path_len, path_src) ;

		// OPEN
                res.ret = PROXY_XPN_OPEN(full_path, pr.u_st_xpn_server_msg.op_open.flags, pr.u_st_xpn_server_msg.op_open.mode);
                res.server_errno = errno ;

		// send status
                ret = write_n_bytes(sd_client, (char *)&res, sizeof(struct st_xpn_server_status)) ;
                break;

           case XPN_SERVER_CREAT_FILE: // CREATE

		// read full path
	        path_len = pr.u_st_xpn_server_msg.op_creat.path_len;
	        path_src = pr.u_st_xpn_server_msg.op_creat.path ;
	        ret = xpn_proxy_server_read_fullpath(sd_client, full_path, path_len, path_src) ;

		// CREAT
                res.ret = PROXY_XPN_CREAT(full_path, pr.u_st_xpn_server_msg.op_creat.mode);
                res.server_errno = errno ;

		// send status
                ret = write_n_bytes(sd_client, (char *)&res, sizeof(struct st_xpn_server_status)) ;
                break;

           case XPN_SERVER_CLOSE_FILE: // CLOSE

		// CLOSE
                res.ret = PROXY_XPN_CLOSE(pr.u_st_xpn_server_msg.op_close.fd);
                res.server_errno = errno ;

		// send status
                ret = write_n_bytes(sd_client, (char *)&res, sizeof(struct st_xpn_server_status)) ;
                break;

           case XPN_SERVER_READ_FILE:  // READ

                buf = malloc(pr.u_st_xpn_server_msg.op_read.size);
                if (buf == NULL)
		{
                    printf("[XPN_PROXY_SERVER]\t[handle_petition]\t%d\n", __LINE__);
		    perror("malloc: ") ;

		    // send status
                    res.ret = -1;
                    ret = write_n_bytes(sd_client, (char *)&res, sizeof(struct st_xpn_server_status)) ;
                    break;
                }

		// read data from file
                bzero(buf, pr.u_st_xpn_server_msg.op_read.size);
                res.ret = PROXY_XPN_READ(pr.u_st_xpn_server_msg.op_read.fd, buf, pr.u_st_xpn_server_msg.op_read.size);
                res.server_errno = errno ;

		// send status
                ret = write_n_bytes(sd_client, (char *)&res, sizeof(struct st_xpn_server_status)) ;

		// send data
                if (res.ret > 0) {
                    ret = write_n_bytes(sd_client, buf, res.ret) ;
                }

                free(buf);
                break;

           case XPN_SERVER_WRITE_FILE:  // WRITE

                buf2 = malloc(pr.u_st_xpn_server_msg.op_write.size) ;
                if (buf2 == NULL)
		{
                    printf("[XPN_PROXY_SERVER]\t[handle_petition]\t%d\n", __LINE__) ;
		    perror("malloc: ") ;

		    // send status
                    res.ret = -1 ;
                    ret = write_n_bytes(sd_client, (char *)&res, sizeof(struct st_xpn_server_status)) ;
                    break ;
                }

		// send status (1/2)
                res.ret = 0 ;
                ret = write_n_bytes(sd_client, (char *)&res, sizeof(struct st_xpn_server_status)) ;

		// read from socket
                bzero(buf2, pr.u_st_xpn_server_msg.op_write.size) ;
                res.ret = read_n_bytes(sd_client, buf2, pr.u_st_xpn_server_msg.op_write.size) ;
                res.server_errno = errno ;

		// write data to file
                if (res.ret > 0)
                {
                    res.ret = PROXY_XPN_WRITE(pr.u_st_xpn_server_msg.op_write.fd, buf2, res.ret) ;
                    res.server_errno = errno ;
                }

		// send status (2/2)
                ret = write_n_bytes(sd_client, (char *)&res, sizeof(struct st_xpn_server_status)) ;

                free(buf2);
                break;

           case XPN_SERVER_RM_FILE: // REMOVE

		// read full path
	        path_len = pr.u_st_xpn_server_msg.op_rm.path_len;
	        path_src = pr.u_st_xpn_server_msg.op_rm.path ;
	        ret = xpn_proxy_server_read_fullpath(sd_client, full_path, path_len, path_src) ;

		// UNLINK
                res.ret = PROXY_XPN_UNLINK(full_path) ;
                res.server_errno = errno ;

		// send status
                ret = write_n_bytes(sd_client, (char *)&res, sizeof(struct st_xpn_server_status)) ;
                break;

           case XPN_SERVER_RENAME_FILE: // RENAME

		// read full path
	        path_len = pr.u_st_xpn_server_msg.op_rename.old_url_len ;
	        path_src = pr.u_st_xpn_server_msg.op_rename.old_url ;
	        ret = xpn_proxy_server_read_fullpath(sd_client, full_path_old, path_len, path_src) ;

		// read full path
	        path_len = pr.u_st_xpn_server_msg.op_rename.new_url_len ;
	        path_src = pr.u_st_xpn_server_msg.op_rename.new_url ;
	        ret = xpn_proxy_server_read_fullpath(sd_client, full_path_new, path_len, path_src) ;

		// RENAME
                res.ret = PROXY_XPN_RENAME(full_path_old, full_path_new);
                res.server_errno = errno;

		// send status
                ret = write_n_bytes(sd_client, (char *)&res, sizeof(struct st_xpn_server_status)) ;
                break;

           case XPN_SERVER_GETATTR_FILE: // GETATTR

		// read full path
	        path_len = pr.u_st_xpn_server_msg.op_getattr.path_len ;
	        path_src = pr.u_st_xpn_server_msg.op_getattr.path ;
	        ret = xpn_proxy_server_read_fullpath(sd_client, full_path, path_len, path_src) ;

		// STAT
                bzero(&req_attr, sizeof(struct st_xpn_server_attr_req));
                res.ret = PROXY_XPN_STAT(full_path, &req_attr.attr);
                res.server_errno = errno;

		// send status
                ret = write_n_bytes(sd_client, (char *)&res, sizeof(struct st_xpn_server_status)) ;

		// send attr struct
                if (res.ret == 0)
                {
                    req_attr.status_req.ret = req_attr.status;
                    req_attr.status_req.server_errno = res.server_errno;
                    req_attr.status = res.ret ;

                    write_n_bytes(sd_client, (char *)&req_attr, sizeof(struct st_xpn_server_attr_req)) ;
                }
                break;

           case XPN_SERVER_MKDIR_DIR: // MKDIR

		// read full path
	        path_len = pr.u_st_xpn_server_msg.op_mkdir.path_len ;
	        path_src = pr.u_st_xpn_server_msg.op_mkdir.path ;
	        ret = xpn_proxy_server_read_fullpath(sd_client, full_path, path_len, path_src) ;

		// MKDIR
                res.ret = PROXY_XPN_MKDIR(full_path, pr.u_st_xpn_server_msg.op_mkdir.mode);
                res.server_errno = errno;

		// send status
                ret = write_n_bytes(sd_client, (char *)&res, sizeof(struct st_xpn_server_status)) ;
                break;

           case XPN_SERVER_OPENDIR_DIR: // OPENDIR

		// read full path
	        path_len = pr.u_st_xpn_server_msg.op_opendir.path_len ;
	        path_src = pr.u_st_xpn_server_msg.op_opendir.path ;
	        ret = xpn_proxy_server_read_fullpath(sd_client, full_path, path_len, path_src) ;

		// OPENDIR
                ret_od = PROXY_XPN_OPENDIR(full_path);
                res.server_errno = errno;

		// send status
                ret = write_n_bytes(sd_client, (char *)&res, sizeof(struct st_xpn_server_status)) ;

		// send struct opendir
                if (res.ret == 0)
		{
                    req_opendir.status.ret = (ret_od == NULL) ? -1 : 0;
                    req_opendir.dir        = ret_od;
                    req_opendir.status.server_errno = res.server_errno ;

                    write_n_bytes(sd_client, (char *)&req_opendir, sizeof(struct st_xpn_server_opendir_req)) ;
                }
                break;

           case XPN_SERVER_CLOSEDIR_DIR: // CLOSEDIR

		// CLOSEDIR
                res.ret = PROXY_XPN_CLOSEDIR(pr.u_st_xpn_server_msg.op_closedir.dir);
                res.server_errno = errno;

		// send status
                ret = write_n_bytes(sd_client, (char *)&res, sizeof(struct st_xpn_server_status)) ;
                break;

           case XPN_SERVER_READDIR_DIR: // READDIR

		// READDIR
                ret_readdir = PROXY_XPN_READDIR(pr.u_st_xpn_server_msg.op_readdir.dir);
                if (ret_readdir != NULL)
		{
                    ret_entry.end = 1 ;
                    res.ret = 0 ;
                }
                else
		{
                    ret_entry.end = 0 ;
                    res.ret = -1 ;
                }

		res.server_errno = errno ;

		// send status
                ret = write_n_bytes(sd_client, (char *)&res, sizeof(struct st_xpn_server_status)) ;

		// send readdir struct
                if (res.ret == 0)
                {
                    ret_entry.status.ret          = res.ret ;
                    ret_entry.status.server_errno = res.server_errno ;
                    ret_entry.ret                 = *ret_readdir ;

                    write_n_bytes(sd_client, (char *)&ret_entry, sizeof(struct st_xpn_server_readdir_req)) ;
                }
                break;

           case XPN_SERVER_RMDIR_DIR: // RMDIR

		// read full path
	        path_len = pr.u_st_xpn_server_msg.op_rmdir.path_len ;
	        path_src = pr.u_st_xpn_server_msg.op_rmdir.path ;
	        ret = xpn_proxy_server_read_fullpath(sd_client, full_path, path_len, path_src) ;

		// RMDIR
                res.ret = PROXY_XPN_RMDIR(full_path);
                res.server_errno = errno;

		// send status
                ret = write_n_bytes(sd_client, (char *)&res, sizeof(struct st_xpn_server_status)) ;
                break;

           default:
                fprintf(stderr, "handle_petition: unknown request type %d\n", pr.type);
                break;
        }

        return 0 ;
    }

    /*
     * Signal handler for SIGINT.
     * @param signo: Signal number.
     * @return: void.
     */
    void sigHandler ( int signo )
    {
        do_exit = 1;
        pthread_cond_broadcast(&queue_cond);

        printf("[XPN_PROXY_SERVER]\t[sigHandler]\t%d signal id:%d\n", __LINE__, signo);
    }


    /*
     * Main server entry point.
     * @param argc: Argument count.
     * @param argv: Argument vector.
     * @return: 0 on success, -1 on error.
     */
    int main ( int argc, char *argv[] )
    {
        int ret;
        pthread_t threads[THREAD_POOL_SIZE];
        int sd_server, sd_client, ipv, port_proxy;
        struct sigaction new_action, old_action;
        extern int do_exit;

        do_exit = 0;

#ifdef USE_XPN_FUNCTIONS
        ret = xpn_init();
        if (ret < 0) {
            printf("[XPN_PROXY_SERVER]\t[main]\t%d\n", __LINE__);
            return -1;
        }
#endif

        port_proxy = utils_getenv_int("XPN_PROXY_PORT", DEFAULT_XPN_PROXY_PORT);
        ipv        = utils_getenv_int("XPN_PROXY_IPV",  DEFAULT_XPN_SCK_IPV);

        ret = socket_server_create(&sd_server, port_proxy, ipv) ;
        if (ret < 0)
        {
            printf("[XPN_PROXY_SERVER]\t[main]\t%d\n", __LINE__);
#ifdef USE_XPN_FUNCTIONS
            xpn_destroy();
#endif
            return -1;
        }

        new_action.sa_handler = sigHandler;
        sigemptyset(&new_action.sa_mask);
        new_action.sa_flags = 0;
        sigaction(SIGINT, NULL, &old_action);

        if (old_action.sa_handler != SIG_IGN) {
            sigaction(SIGINT, &new_action, NULL);
        }

        // Start thread pool
        for (int i = 0; i < THREAD_POOL_SIZE; ++i) {
            pthread_create(&threads[i], NULL, worker_thread, NULL);
        }

        while (do_exit == 0)
        {
            ret = socket_server_accept(sd_server, &sd_client, ipv);
            if (sd_client < 0 || ret < 0)
	    {
                if (do_exit) {
                    break;
		}
                printf("[XPN_PROXY_SERVER]\t[main]\t%d\n", __LINE__);
                continue;
            }

            client_node_t *node = malloc(sizeof(client_node_t));
            if (!node) {
                printf("[XPN_PROXY_SERVER]\t[main]\t%d\n", __LINE__);
                close(sd_client);
                continue;
            }

            node->sd_client = sd_client;

            pthread_mutex_lock(&queue_mutex);
            TAILQ_INSERT_TAIL(&client_q, node, nodes);
            pthread_cond_signal(&queue_cond);
            pthread_mutex_unlock(&queue_mutex);
        }

        // Notify all threads to exit and join them
        pthread_mutex_lock(&queue_mutex);
        pthread_cond_broadcast(&queue_cond);
        pthread_mutex_unlock(&queue_mutex);

        for (int i = 0; i < THREAD_POOL_SIZE; ++i) {
            pthread_join(threads[i], NULL);
        }

        if (socket_close(sd_server) < 0) {
            printf("[XPN_PROXY_SERVER]\t[main]\t%d\n", __LINE__);
        }

        pthread_mutex_destroy(&queue_mutex);
        pthread_cond_destroy(&queue_cond);

#ifdef USE_XPN_FUNCTIONS
        ret = xpn_destroy();
        if (ret < 0) {
            printf("[XPN_PROXY_SERVER]\t[main]\t%d\n", __LINE__);
            return -1;
        }
#endif

        printf("The End.\n");
        return 0;
    }


 /* ................................................................... */

