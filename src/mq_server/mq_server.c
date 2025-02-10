/*
 *  Copyright 2020-2023 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos
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

   #include "all_system.h"
   #include "base/utils.h"
   #include "base/workers.h"
   #include "base/ns.h"
   #include "mq_server/mq_server_params.h"
   #include "mq_server/mq_server_ops.h"
   #include "mq_server/mq_server_comm.h"
   #include "mq_server/mq_server_d2xpn.h"
   #include <signal.h>
   #include <sys/time.h>
   #include <pthread.h>


/* ... Global variables / Variables globales ......................... */

mq_server_param_st params;
worker_t worker;
int the_end = 0;
char serv_name[HOST_NAME_MAX];

int file;

#define MAX_LINE_LENGTH 256
#define QUEUE_SIZE 2000

typedef struct {
    int socket;
    // Otros datos del cliente que se deseen procesar
}
ClientData;

typedef struct 
{
    ClientData * queue[QUEUE_SIZE];
    int front;
    int rear;
    int count;
    pthread_mutex_t mutex;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
    //pthread_cond_t idle;
}
CircularQueue;

/* ... Auxiliar Functions / Funciones Auxiliares ..................... */

CircularQueue queue;

void queue_init() {
    queue.front = 0;
    queue.rear = -1;
    queue.count = 0;
    pthread_mutex_init( & queue.mutex, NULL);
    pthread_cond_init( & queue.not_empty, NULL);
    pthread_cond_init( & queue.not_full, NULL);
    //pthread_cond_init(&queue.idle, NULL);
}

void enqueue(ClientData * client) {
    pthread_mutex_lock( & queue.mutex);
    while (queue.count >= QUEUE_SIZE) {
        pthread_cond_wait( & queue.not_full, & queue.mutex);
    }

    queue.rear = (queue.rear + 1) % QUEUE_SIZE;
    queue.queue[queue.rear] = client;
    queue.count++;

    pthread_cond_signal( & queue.not_empty);
    pthread_mutex_unlock( & queue.mutex);
}

ClientData * dequeue() {
    pthread_mutex_lock( & queue.mutex);

    while (queue.count <= 0) {
        // Dormir hasta que haya elementos en la cola
        pthread_cond_wait( & queue.not_empty, & queue.mutex);
    }

    ClientData * client = queue.queue[queue.front];
    queue.front = (queue.front + 1) % QUEUE_SIZE;
    queue.count--;

    // Despertar a un hilo dormido si la cola aún tiene elementos
    /*if (queue.count > 0) 
    {
        pthread_cond_signal(&queue.not_empty);
    } 
    else 
    {
        // No hay más elementos en la cola, señalizar que todos los hilos están inactivos
        pthread_cond_broadcast(&queue.idle);
    }*/
    pthread_cond_signal( & queue.not_full);
    pthread_mutex_unlock( & queue.mutex);

    return client;
}

void signal_callback_handler(int signum) {

    printf("Caught signal SIGPIPE server %d\n", signum);
}

void mq_server_run(struct st_th th) {
    debug_info("[TCP-SERVER] (ID=%d): begin to do operation '%s' OP_ID %d\n", th.id, mq_server_op2string(th.type_op), th.type_op);

    mq_server_do_operation( & th, & the_end);

    debug_info("[TCP-SERVER] (ID=%d) end to do operation '%s'\n", th.id, mq_server_op2string(th.type_op));
}

void mq_server_dispatcher(struct st_th th) {
    int ret;
    int disconnect;
    struct st_th th_arg;

    //printf("Arrived mq_server dispatcher\n");

    // check params...
    if (NULL == th.params) {
        printf("[WORKERS ID=%d] ERROR: NULL arguments", th.id);
        return;
    }

    int data = 0;

    ret = mq_server_comm_write_data(th.params, (int) th.sd, (char * ) & data, sizeof(int), 0);
    if (ret < 0) {
        printf("[TCP-SERVER] ERROR: sync write fails\n");
        return -1;
    }

    //printf("[TCP-SERVER] OK: sync write\n")

    disconnect = 0;
    while (!disconnect) {
        ret = mq_server_comm_read_operation(th.params, (int) th.sd, (char * ) & (th.type_op), 1, & (th.rank_client_id));
/*
        if (th.type_op == MQ_SERVER_OPEN_FILE_WOS || th.type_op == MQ_SERVER_OPEN_FILE_WS) 
        {
            struct timeval current_time;
            gettimeofday( & current_time, NULL);
            time_t now = current_time.tv_sec;
            struct tm * timeinfo;
            timeinfo = localtime( & now);

            char time_str[20];
            strftime(time_str, sizeof(time_str), "%H:%M:%S", timeinfo);
            printf("OPEN - %s\n", time_str);
            //int retw = write(file, time_str, strlen(time_str));
            //if (retw < 0) printf("ERROR Write Dispatcher\n");
        }
        else if (th.type_op == MQ_SERVER_CLOSE_FILE_WS)
        {
            struct timeval current_time;
            gettimeofday( & current_time, NULL);
            time_t now = current_time.tv_sec;
            struct tm * timeinfo;
            timeinfo = localtime( & now);

            char time_str[20];
            strftime(time_str, sizeof(time_str), "%H:%M:%S", timeinfo);
            printf("CLOSE - %s\n", time_str);
        }
        else if (th.type_op == MQ_SERVER_WRITE_FILE_WS || th.type_op == MQ_SERVER_WRITE_FILE_WOS)
        {
            struct timeval current_time;
            gettimeofday( & current_time, NULL);
            time_t now = current_time.tv_sec;
            struct tm * timeinfo;
            timeinfo = localtime(& now);

            char time_str[20];
            strftime(time_str, sizeof(time_str), "%H:%M:%S", timeinfo);
            printf("WRITET - %s\n", time_str);
        }
*/
        //fprintf(file, "%s\n", time_str);
        if (ret < 0) {
            //printf("[TCP-SERVER] ERROR: mq_server_comm_readdata fail\n");
            return;
        }

        if (ret == 0) {
            //printf("[TCP-SERVER] WARNING: mq_server_comm_readdata broken pipe\n");
            return;
        }

        if (th.type_op == MQ_SERVER_DISCONNECT || th.type_op == MQ_SERVER_FINALIZE) 
        {
            printf("[TCP-SERVER] INFO: DISCONNECT received\n");
            disconnect = 1;
            continue;
        }

        // Launch worker per operation
        th_arg.params = & params;
        th_arg.sd = (int) th.sd;
        th_arg.function = mq_server_run;
        th_arg.type_op = th.type_op;
        th_arg.rank_client_id = th.rank_client_id;
        th_arg.wait4me = FALSE;

        mq_server_run(th_arg);
    }

    debug_info("[TCP-SERVER] mq_server_worker_run (ID=%d) close\n", th.rank_client_id);

    mq_server_comm_close((int) th.sd);
}

void * process_client(void * arg) {
    struct st_mq_server_msg head;
    int rank_client_id, ret;
    struct st_th th_arg;

    while (1) {
        char bufferSock[MAX_LINE_LENGTH];
        ssize_t bytes_received;

        ClientData * client = dequeue();

        ////////////////////////////////////////////

        ret = mq_server_comm_read_operation( & params, client -> socket, (char * ) & (head.type), 1, & (rank_client_id));
        //printf("SERVER 1 -- %d\n", head.type);

        if (ret < 0) {
            printf("[TCP-SERVER] ERROR: mq_server_comm_readdata fail\n");
            return -1;
        }

        if (head.type == MQ_SERVER_FINALIZE) {
            the_end = 1;
            printf("[TCP-SERVER]: mq_server finalized\n");
            continue;
        }

        //Launch dispatcher per application
        th_arg.params = & params;
        th_arg.sd = client -> socket;
        th_arg.function = mq_server_dispatcher;
        th_arg.type_op = 0;
        th_arg.rank_client_id = 0;
        th_arg.wait4me = FALSE;

        mq_server_dispatcher(th_arg);

        //base_workers_launch( & worker, & th_arg, mq_server_dispatcher);

        // Finalizar la conexión con el cliente y liberar memoria
        close(client -> socket);
        free(client);
    }

    pthread_exit(NULL);
}

/* ... Functions / Funciones ......................................... */

int mq_server_up(void) 
{
    int ret;
    //struct st_mq_server_msg head;
    //int rank_client_id;
    //struct st_th th_arg;
    //sem_t * sem_server;
    int sd;
    //int nthreads = sysconf(_SC_NPROCESSORS_ONLN);
    int nthreads = 128;
    printf("NUMERO DE HILOS ------------------ %d\n", nthreads);
    pthread_t threads[nthreads];
    queue_init();

    // Feedback
    printf("\n");
    printf(" -------------------\n");
    printf(" > Starting servers... (%s)\n", serv_name);
    printf(" -------------------\n");
    printf("\n");

    signal(SIGPIPE, SIG_IGN);

    // Initialize
    debug_msg_init();
    ret = mq_server_comm_init( & params);
    if (ret < 0) 
    {
        printf("[TCP-SERVER] ERROR: tcp_comm initialization fails\n");
        return -1;
    }
    /*ret = base_workers_init(&worker, params.thread_mode);
    if (ret < 0) {
        printf("[TCP-SERVER] ERROR: workers initialization fails\n");
        return -1;
    }*/

    // Initialize semaphore for server disks
    /*
    ret = sem_init( & (params.disk_sem), 0, 1);
    if (ret < 0) 
    {
        printf("[TCP-SERVER] ERROR: semaphore initialization fails\n");
        return -1;
    }

    // Initialize semaphore for clients
    sprintf(params.sem_name_server, "%s%d", serv_name, getpid());
    sem_server = sem_open(params.sem_name_server, O_CREAT, 0777, 1);
    if (sem_server == 0) {
        printf("[TCP-SERVER] ERROR: semaphore open fails\n");
        return -1;
    }
    */

    // Crear el grupo de hilos (thread pool)
    for (int i = 0; i < nthreads; i++) {
        pthread_create( & threads[i], NULL, process_client, & i);
    }
/*
    char * xpn_time = getenv("XPN_TIME");

    if (xpn_time == NULL) {
        printf("[TCP-SERVER] Error: process_client\n");
    }

    file = open(xpn_time, O_CREAT | O_WRONLY, 0777);
    if (file == NULL) {
        printf("[TCP-SERVER] ERROR: process_client\n");
    }
*/
    // Loop: receiving + processing
    the_end = 0;

    static int pet = 0;

    while (!the_end) {
        debug_info("[TCP-SERVER] mq_server_accept_comm()\n");

        params.client = 0;
        sd = mq_server_comm_accept( & params);
        if (sd < 0) {
            continue;
        }
        // Crear la estructura de datos del cliente
        ClientData * client = (ClientData * ) malloc(sizeof(ClientData));
        client -> socket = sd;

        // Encolar el socket
        enqueue(client);

    }

    // Wait and finalize for all current workers
    //debug_info("[TCP-SERVER] base_workers_destroy\n");
    //base_workers_destroy( & worker);

    // Esperar a que todos los hilos finalicen
    for (int i = 0; i < nthreads; i++) {
        pthread_join(threads[i], NULL);
    }

    debug_info("[TCP-SERVER] mq_server_comm_destroy\n");
    mq_server_comm_destroy( & params);

    /*
    // Close semaphores
    sem_destroy( & (params.disk_sem));
    sem_unlink(params.sem_name_server);
    */

    // return OK
    return 0;
}


int mq_server_down(void) 
{
    int ret, sd, data;
    char srv_name[1024];
    char server_name[1024];
    char port_number[1024];
    FILE * file;
    int ns_lookup(char * param_srv_name, char *protocol, char * host_name, char * port_name);

    // Feedback
    printf("\n");
    printf(" -------------------\n");
    printf(" > Stopping servers... (%s)\n", serv_name);
    printf(" -------------------\n");
    printf("\n");

    // Initialize
    debug_msg_init();
    ret = mq_server_comm_init( & params);
    if (ret < 0) {
        printf("[TCP-SERVER] ERROR: tcp_comm initialization fails\n");
        return -1;
    }
    ret = base_workers_init( & worker, params.thread_mode);
    if (ret < 0) {
        printf("[TCP-SERVER] ERROR: workers initialization fails\n");
        return -1;
    }

    // Open host file
    file = fopen(params.dns_file, "r");
    if (file == NULL) 
    {
        printf("[TCP-SERVER] ERROR: invalid file %s\n", params.dns_file);
        return -1;
    }

    while (fscanf(file, "%s %s %s", srv_name, server_name, port_number) != EOF) 
    {
        // Lookup port name
        ret = ns_lookup("mq_server", srv_name, server_name, port_number);
        if (ret < 0) {
            printf("[TCP-SERVER] ERROR: server %s %s %s not found\n", srv_name, server_name, port_number);
            continue;
        }

        // Connect with server
        sd = mq_server_comm_connect( & params, server_name, atoi(port_number));
        if (sd < 0) 
        {
            printf("[TCP-SERVER] ERROR: connect to %s failed\n", server_name);
            continue;
        }

        // Send shutdown request
        data = MQ_SERVER_FINALIZE;
        ret = mq_server_comm_write_data( & params, sd, (char * ) & data, sizeof(int), 0); // 0: rank_client_id
        if (ret < 0) 
        {
            printf("[TCP-SERVER] ERROR: write SERVER_FINALIZE to %s failed\n", srv_name);
            return -1;
        }

        // Close
        mq_server_comm_close(sd);
    }

    // Close host file
    fclose(file);

    // Wait and finalize for all current workers
    debug_info("[TCP-SERVER] base_workers_destroy\n");
    base_workers_destroy( & worker);
    debug_info("[TCP-SERVER] mq_server_comm_destroy\n");
    mq_server_comm_destroy( & params);

    return 0;
}

/*
 * Main
 */

int main(int argc, char * argv[]) 
{
    signal(SIGPIPE, signal_callback_handler);
    int ret = -1;
    char * exec_name = NULL;

    // Initializing...
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    // Welcome...
    printf("\n");
    printf(" mq_server\n");
    printf(" ----------\n");
    printf("\n");
    printf(" > Begin.\n");
    printf("\n");

    // Get arguments..
    ret = mq_server_params_get( & params, argc, argv);
    if (ret < 0) {
        mq_server_params_show_usage();
        return -1;
    }

    // Show configuration...
    exec_name = basename(argv[0]);
    printf("   * action=%s\n", exec_name);
    gethostname(serv_name, HOST_NAME_MAX);
    printf("   * host=%s\n", serv_name);
    mq_server_params_show( & params);

    // Do associate action...
    if (strcasecmp(exec_name, "xpn_stop_mq_server") == 0) {
        ret = mq_server_down();
    } else {
        ret = mq_server_up();
    }

    return ret;
}

/* ................................................................... */
