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

   #include "xpn_client/xpn.h"
   #include "xpn_server/xpn_server_ops.h"
   #include "base/socket.h"
   #include "base/service_socket.h"

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
 * Worker thread function for the thread pool.
 * Waits for client sockets in the queue and processes them.
 */
void *worker_thread(void *arg)
{
    (void)arg;
    void handle_petition(int arg);

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

        if (node) {
            handle_petition(node->sd_client);
            free(node);
        }
    }

    return NULL;
}

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
        printf("[XPN_PROXY_SERVER]\t[read_n_bytes]\t%d\n", __LINE__);
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
                fprintf(stderr, "read_n_bytes: connection closed unexpectedly\n");
                printf("[XPN_PROXY_SERVER]\t[read_n_bytes]\t%d\n", __LINE__);
            }else
            {
                printf("read_n_bytes: read");
                printf("[XPN_PROXY_SERVER]\t[read_n_bytes]\t%d\n", __LINE__);
            }
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
void handle_petition ( int arg )
{
    int ret, ret2;
    int sd_client;
    struct st_xpn_server_msg pr;
    struct st_xpn_server_status res;
    res.ret = 0;

    sd_client = (int)arg;
    ret = read(sd_client, &pr, sizeof(struct st_xpn_server_msg));
    if (ret < 0)
    {
        printf("[XPN_PROXY_SERVER]\t[handle_petition]\t%d\n", __LINE__);
        if (close(sd_client) < 0) {
            printf("[XPN_PROXY_SERVER]\t[handle_petition]\t%d\n", __LINE__);
	}

        return;
    }

    switch (pr.type)
    {
    case XPN_SERVER_OPEN_FILE: // OPEN
        res.ret = PROXY_XPN_OPEN(pr.u_st_xpn_server_msg.op_open.path, pr.u_st_xpn_server_msg.op_open.flags, pr.u_st_xpn_server_msg.op_open.mode);
        res.server_errno = errno = errno;

        if (write(sd_client, (char *)&res, sizeof(struct st_xpn_server_status)) < 0)
            printf("[XPN_PROXY_SERVER]\t[handle_petition]\t%d\n", __LINE__);
        break;

    case XPN_SERVER_CREAT_FILE: // CREATE
        res.ret = PROXY_XPN_CREAT(pr.u_st_xpn_server_msg.op_creat.path, pr.u_st_xpn_server_msg.op_creat.mode);
        res.server_errno = errno = errno;

        if (write(sd_client, (char *)&res, sizeof(struct st_xpn_server_status)) < 0)
            printf("[XPN_PROXY_SERVER]\t[handle_petition]\t%d\n", __LINE__);
        break;

    case XPN_SERVER_CLOSE_FILE: // CLOSE
        res.ret = PROXY_XPN_CLOSE(pr.u_st_xpn_server_msg.op_close.fd);
        res.server_errno = errno = errno;

        if (write(sd_client, (char *)&res, sizeof(struct st_xpn_server_status)) < 0)
            printf("[XPN_PROXY_SERVER]\t[handle_petition]\t%d\n", __LINE__);
        break;

    case XPN_SERVER_READ_FILE:  // READ
        char *buf = malloc(pr.u_st_xpn_server_msg.op_read.size);
        if (buf == NULL) {
            printf("[XPN_PROXY_SERVER]\t[handle_petition]\t%d\n", __LINE__);
            res.ret = -1;
            ret = write(sd_client, (char *)&res, sizeof(struct st_xpn_server_status));
            break;
        }
        bzero(buf, pr.u_st_xpn_server_msg.op_read.size);

        res.ret = PROXY_XPN_READ(pr.u_st_xpn_server_msg.op_read.fd, buf, pr.u_st_xpn_server_msg.op_read.size);
        res.server_errno = errno = errno;

        if (write(sd_client, (char *)&res, sizeof(struct st_xpn_server_status)) < 0)
            printf("[XPN_PROXY_SERVER]\t[handle_petition]\t%d\n", __LINE__);

        if (res.ret > 0) {
            if (write(sd_client, buf, res.ret) < 0)
                printf("[XPN_PROXY_SERVER]\t[handle_petition]\t%d\n", __LINE__);
        }
        free(buf);
        break;

    case XPN_SERVER_WRITE_FILE:  // WRITE
        char *buf2 = malloc(pr.u_st_xpn_server_msg.op_write.size);
        if (buf2 == NULL) {
            printf("[XPN_PROXY_SERVER]\t[handle_petition]\t%d\n", __LINE__);
            res.ret = -1;
            ret = write(sd_client, (char *)&res, sizeof(struct st_xpn_server_status));
            break;
        }
        bzero(buf2, pr.u_st_xpn_server_msg.op_write.size);

        if (write(sd_client, (char *)&res, sizeof(struct st_xpn_server_status)) < 0)
            printf("[XPN_PROXY_SERVER]\t[handle_petition]\t%d\n", __LINE__);

        ssize_t r = read_n_bytes(sd_client, buf2, pr.u_st_xpn_server_msg.op_write.size);
        if (r < 0) {
            printf("[XPN_PROXY_SERVER]\t[handle_petition]\t%d\n", __LINE__);
            res.ret = -1;
            res.server_errno = errno = errno;
        } else {
            res.ret = PROXY_XPN_WRITE(pr.u_st_xpn_server_msg.op_write.fd, buf2, r);
            res.server_errno = errno = errno;
        }

        if (res.ret > 0) {
            if (write(sd_client, (char *)&res, sizeof(struct st_xpn_server_status)) < 0)
                printf("[XPN_PROXY_SERVER]\t[handle_petition]\t%d\n", __LINE__);
        }

        free(buf2);
        break;

    case XPN_SERVER_RM_FILE: // REMOVE
        res.ret = PROXY_XPN_UNLINK(pr.u_st_xpn_server_msg.op_rm.path);
        res.server_errno = errno = errno;

        if (write(sd_client, (char *)&res, sizeof(struct st_xpn_server_status)) < 0)
            printf("[XPN_PROXY_SERVER]\t[handle_petition]\t%d\n", __LINE__);
        break;

    case XPN_SERVER_RENAME_FILE: // RENAME
        res.ret = PROXY_XPN_RENAME(pr.u_st_xpn_server_msg.op_rename.old_url, pr.u_st_xpn_server_msg.op_rename.new_url);
        res.server_errno = errno;

        if (write(sd_client, (char *)&res, sizeof(struct st_xpn_server_status)) < 0)
            printf("[XPN_PROXY_SERVER]\t[handle_petition]\t%d\n", __LINE__);
        break;

    case XPN_SERVER_GETATTR_FILE: // GETATTR
        struct st_xpn_server_attr_req req;

        req.status = PROXY_XPN_STAT(pr.u_st_xpn_server_msg.op_getattr.path, &req.attr);
        req.status_req.ret = req.status;

        res.ret = req.status;
        res.server_errno = errno;

        req.status_req.server_errno = errno;

        if (write(sd_client, (char *)&res, sizeof(struct st_xpn_server_status)) < 0) {
            printf("[XPN_PROXY_SERVER]\t[handle_petition]\t%d\n", __LINE__);
	}

        if (res.ret == 0)
        {
            if (write(sd_client, (char *)&req, sizeof(struct st_xpn_server_attr_req)) < 0) {
                printf("[XPN_PROXY_SERVER]\t[handle_petition]\t%d\n", __LINE__);
	    }
        }

        break;

    case XPN_SERVER_MKDIR_DIR: // MKDIR
        res.ret = PROXY_XPN_MKDIR(pr.u_st_xpn_server_msg.op_mkdir.path, pr.u_st_xpn_server_msg.op_mkdir.mode);
        res.server_errno = errno;

        if (write(sd_client, (char *)&res, sizeof(struct st_xpn_server_status)) < 0)
            printf("[XPN_PROXY_SERVER]\t[handle_petition]\t%d\n", __LINE__);
        break;

    case XPN_SERVER_OPENDIR_DIR: // OPENDIR
        DIR* ret;
        struct st_xpn_server_opendir_req req_opendir;

        ret = PROXY_XPN_OPENDIR(pr.u_st_xpn_server_msg.op_opendir.path);

        req_opendir.status.ret = ret == NULL ? -1 : 0;
        req_opendir.dir = ret == NULL ? NULL : ret;
        req_opendir.status.server_errno = errno;
        req.status_req.server_errno = errno;

        ret2 = write(sd_client, (char *)&res, sizeof(struct st_xpn_server_status)) ;
        if (ret2 < 0) {
            printf("[XPN_PROXY_SERVER]\t[handle_petition]\t%d\n", __LINE__);
	}

        if (res.ret == 0)
        {
            if (write(sd_client, (char *)&req_opendir, sizeof(struct st_xpn_server_opendir_req)) < 0) {
                printf("[XPN_PROXY_SERVER]\t[handle_petition]\t%d\n", __LINE__);
	    }
        }
        break;

    case XPN_SERVER_CLOSEDIR_DIR: // CLOSEDIR

        res.ret = PROXY_XPN_CLOSEDIR(pr.u_st_xpn_server_msg.op_closedir.dir);
        res.server_errno = errno;

        ret2 = write(sd_client, (char *)&res, sizeof(struct st_xpn_server_status)) ;
        if (ret2 < 0) {
            printf("[XPN_PROXY_SERVER]\t[handle_petition]\t%d\n", __LINE__);
	}
        break;

    case XPN_SERVER_READDIR_DIR: // READDIR

        struct dirent * ret_readdir;
        struct st_xpn_server_readdir_req ret_entry;

        ret_readdir = PROXY_XPN_READDIR(pr.u_st_xpn_server_msg.op_readdir.dir);
        if (ret_readdir != NULL)
        {
            ret_entry.end = 1;
            ret_entry.ret = *ret_readdir;
        }
        else
        {
            ret_entry.end = 0;
        }

        ret_entry.status.ret = ret_readdir == NULL ? -1 : 0;
        ret_entry.status.server_errno = errno;

        res.ret = ret_entry.status.ret;
        res.server_errno = ret_entry.status.server_errno;

        ret2 = write(sd_client, (char *)&res, sizeof(struct st_xpn_server_status)) ;
        if (ret2 < 0) {
            printf("[XPN_PROXY_SERVER]\t[handle_petition]\t%d\n", __LINE__);
	}

        if (res.ret == 0)
        {
            if (write(sd_client, (char *)&ret_entry, sizeof(struct st_xpn_server_readdir_req)) < 0) {
                printf("[XPN_PROXY_SERVER]\t[handle_petition]\t%d\n", __LINE__);
	    }
        }

        break;

    case XPN_SERVER_RMDIR_DIR: // CLOSEDIR

        res.ret = PROXY_XPN_RMDIR(pr.u_st_xpn_server_msg.op_rmdir.path);
        res.server_errno = errno;

        ret2 = write(sd_client, (char *)&res, sizeof(struct st_xpn_server_status)) ;
        if (ret2 < 0) {
            printf("[XPN_PROXY_SERVER]\t[handle_petition]\t%d\n", __LINE__);
	}
        break;


    default:
        fprintf(stderr, "handle_petition: unknown request type %d\n", pr.type);
        break;
    }

    if (close(sd_client) < 0) {
        printf("[XPN_PROXY_SERVER]\t[handle_petition]\t%d\n", __LINE__);
    }
}

/*
 * Signal handler for SIGINT.
 * @param signo: Signal number.
 * @return: void.
 */
void sigHandler(int signo)
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
int main(int argc, char *argv[])
{
    int ret;
    int sd_server, sd_client, ipv, port_proxy;
    struct sigaction new_action, old_action;
    extern int do_exit;

    do_exit = 0;
    pthread_t threads[THREAD_POOL_SIZE];

#ifdef USE_XPN_FUNCTIONS
    ret = xpn_init();
    if (ret < 0) {
        printf("[XPN_PROXY_SERVER]\t[main]\t%d\n", __LINE__);
        return -1;
    }
#endif

    port_proxy = utils_getenv_int("XPN_PROXY_PORT", DEFAULT_XPN_PROXY_PORT);
    ipv  = utils_getenv_int("XPN_PROXY_IPV",  DEFAULT_XPN_SCK_IPV);

    if (socket_server_create(&sd_server, port_proxy, ipv) < 0) {
        printf("[XPN_PROXY_SERVER]\t[main]\t%d\n", __LINE__);
        xpn_destroy();
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
        if (sd_client < 0) {
            if (do_exit)
                break;
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

