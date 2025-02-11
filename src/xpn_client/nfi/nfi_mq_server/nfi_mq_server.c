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
//#define DEBUG 1
#include "nfi_mq_server.h"

/* ... Global Variable / Variable Globales ........................... */

#define FILESYSTEM_DLSYM 1


/* ... Functions / Funciones ......................................... */



/*
 * Communication
 */

int mq_server_write_operation(int sd, struct st_mq_server_msg * head) {
    int ret;

    debug_info("[NFI_MQ] (ID=%s) tcpClient_write_data: begin               HEAD_TYPE:%d\n", head->id, sizeof(head->type));
    ret = tcpClient_write_operation(sd, (char * ) & (head->type), 1, head->id);
    debug_info("CLIENT -- %d\n", head->type);
    if (ret < 0) {
        debug_warning("Server[?]: tcpClient_write_data fails :-(");
        return -1;
    }

    debug_info("[NFI_MQ] (ID=%s) write_operation: %d->\n", head->id, head->type);
    switch (head->type) {
        //File API
    case MQ_SERVER_OPEN_FILE_WS:
    case MQ_SERVER_OPEN_FILE_WOS:
        debug_info("[NFI_MQ] (ID=%s) OPEN operation ---------------------  %ld\n", head->id, sizeof(struct st_mq_server_open));
        ret = tcpClient_write_data(sd, (char * ) & head->u_st_mq_server_msg.op_open, sizeof(struct st_mq_server_open), head->id);
        break;
    case MQ_SERVER_CREAT_FILE_WS:
    case MQ_SERVER_CREAT_FILE_WOS:
        debug_info("[NFI_MQ] (ID=%s) CREAT operation\n", head->id);
        ret = tcpClient_write_data(sd, (char * ) & head->u_st_mq_server_msg.op_creat, sizeof(struct st_mq_server_creat), head->id);
        break;
    case MQ_SERVER_READ_FILE_WS:
    case MQ_SERVER_READ_FILE_WOS:
        debug_info("[NFI_MQ] (ID=%s) READ operation\n", head->id);
        ret = tcpClient_write_data(sd, (char * ) & head->u_st_mq_server_msg.op_read, sizeof(struct st_mq_server_read), head->id);
        break;
    case MQ_SERVER_WRITE_FILE_WS:
    case MQ_SERVER_WRITE_FILE_WOS:
        debug_info("[NFI_MQ] (ID=%s) WRITE operation\n", head->id);
        ret = tcpClient_write_data(sd, (char * ) & head->u_st_mq_server_msg.op_write, sizeof(struct st_mq_server_write), head->id);
        break;
    case MQ_SERVER_CLOSE_FILE_WS:
        debug_info("[NFI_MQ] (ID=%s) CLOSE operation\n", head->id);
        ret = tcpClient_write_data(sd, (char * ) & head->u_st_mq_server_msg.op_close, sizeof(struct st_mq_server_close), head->id);
        break;
    case MQ_SERVER_RM_FILE:
        debug_info("[NFI_MQ] (ID=%s) RM operation\n", head->id);
        ret = tcpClient_write_data(sd, (char * ) & head->u_st_mq_server_msg.op_rm, sizeof(struct st_mq_server_rm), head->id);
        break;
    case MQ_SERVER_RENAME_FILE:
        debug_info("[NFI_MQ] (ID=%s) RENAME operation\n", head->id);
        ret = tcpClient_write_data(sd, (char * ) & head->u_st_mq_server_msg.op_rename, sizeof(struct st_mq_server_rename), head->id);
        break;
    case MQ_SERVER_GETATTR_FILE:
        debug_info("[NFI_MQ] (ID=%s) GETATTR operation\n", head->id);
        ret = tcpClient_write_data(sd, (char * ) & head->u_st_mq_server_msg.op_getattr, sizeof(struct st_mq_server_getattr), head->id);
        break;

        //Directory API
    case MQ_SERVER_MKDIR_DIR:
        debug_info("[NFI_MQ] (ID=%s) MKDIR operation\n", head->id);
        ret = tcpClient_write_data(sd, (char * ) & head->u_st_mq_server_msg.op_mkdir, sizeof(struct st_mq_server_mkdir), head->id);
        break;
    case MQ_SERVER_OPENDIR_DIR:
        debug_info("[NFI_MQ] (ID=%s) OPENDIR operation\n", head->id);
        ret = tcpClient_write_data(sd, (char * ) & head->u_st_mq_server_msg.op_opendir, sizeof(struct st_mq_server_opendir), head->id);
        break;
    case MQ_SERVER_READDIR_DIR:
        debug_info("[NFI_MQ] (ID=%s) READDIR operation\n", head->id);
        ret = tcpClient_write_data(sd, (char * ) & head->u_st_mq_server_msg.op_readdir, sizeof(struct st_mq_server_readdir), head->id);
        break;
    case MQ_SERVER_CLOSEDIR_DIR:
        debug_info("[NFI_MQ] (ID=%s) CLOSEDIR operation\n", head->id);
        ret = tcpClient_write_data(sd, (char * ) & head->u_st_mq_server_msg.op_closedir, sizeof(struct st_mq_server_closedir), head->id);
        break;
    case MQ_SERVER_RMDIR_DIR:
        debug_info("[NFI_MQ] (ID=%s) RMDIR operation\n", head->id);
        ret = tcpClient_write_data(sd, (char * ) & head->u_st_mq_server_msg.op_rmdir, sizeof(struct st_mq_server_rmdir), head->id);
        break;

        //Optimization API
/*
    case MQ_SERVER_FLUSH_FILE:
        debug_info("[NFI_MQ] (ID=%s) FLUSH operation\n", head->id);
        ret = tcpClient_write_data(sd, (char * ) & head->u_st_mq_server_msg.op_flush, sizeof(struct st_mq_server_flush), head->id);
        break;
    case MQ_SERVER_PRELOAD_FILE:
        debug_info("[NFI_MQ] (ID=%s) PRELOAD operation\n", head->id);
        ret = tcpClient_write_data(sd, (char * ) & head->u_st_mq_server_msg.op_preload, sizeof(struct st_mq_server_preload), head->id);
        break;
*/

    case MQ_SERVER_READ_MDATA:
      debug_info("[NFI_MQ] [nfi_write_operation] READ_MDATA operation\n");
      ret = nfi_mq_server_comm_write_data(params, (char *)&(head->u_st_mq_server_msg.op_read_mdata), sizeof(head->u_st_mq_server_msg.op_read_mdata));
      break;
    case MQ_SERVER_WRITE_MDATA:
      debug_info("[NFI_MQ] [nfi_write_operation] WRITE_MDATA operation\n");
      ret = nfi_mq_server_comm_write_data(params, (char *)&(head->u_st_mq_server_msg.op_write_mdata), sizeof(head->u_st_mq_server_msg.op_write_mdata));
      break;
    case MQ_SERVER_WRITE_MDATA_FILE_SIZE:
      debug_info("[NFI_MQ] [nfi_write_operation] WRITE_MDATA_FILE_SIZE operation\n");
      ret = nfi_mq_server_comm_write_data(params, (char *)&(head->u_st_mq_server_msg.op_write_mdata_file_size), sizeof(head->u_st_mq_server_msg.op_write_mdata_file_size));
      break;
    }

    return ret;
}

int nfi_mq_server_doRequest(struct nfi_mq_server_server * server_aux, struct st_mq_server_msg * msg, char * req, int req_size) {
    ssize_t ret;

    // send request...
    debug_info("[NFI_MQ] (ID=%s): %s:->...\n", server_aux->id, msg->id);
    ret = mq_server_write_operation(server_aux->params.server, msg);
    if (ret < 0) 
    {
        return -1;
    }

    // read response...
    debug_info("[NFI_MQ] (ID=%s): %s: <- ...\n", server_aux->id, msg->id);
    ret = tcpClient_read_data(server_aux->params.server, req, req_size, msg->id);
    if (ret < 0) 
    {
        return -1;
    }

    // return OK
    return 0;
}

int nfi_mq_server_keepConnected(struct nfi_server * serv) 
{
    if (NULL == serv) 
    {
        return -1;
    }

    #ifdef NFI_DYNAMIC
    if (serv->private_info == NULL) 
    {
        int ret = nfi_mq_server_reconnect(serv);
        if (ret < 0) 
        {
            serv->private_info = NULL;
            return -1;
        }
    }
    #endif

    // return OK
    return (serv->private_info != NULL);
}

/*
 *  PRIVATE FUNCTIONS TO USE mq_server SERVERS
 */

void NFItoMQ_SERVERattr(struct stat * att, struct nfi_attr * nfi_att) 
{
    att->st_dev = nfi_att->st_dev;
    att->st_ino = nfi_att->st_ino;

    if (nfi_att->at_type == NFIFILE) 
    {
        att->st_mode = nfi_att->at_mode | S_IFREG; // protection
    }

    if (nfi_att->at_type == NFIDIR) 
    {
        att->st_mode = nfi_att->at_mode | S_IFDIR; // protection
    }

    att->st_nlink = nfi_att->at_nlink; // number of hard links
    att->st_uid = nfi_att->at_uid; // user ID of owner
    att->st_gid = nfi_att->at_gid; // group ID of owner
    att->st_size = nfi_att->at_size; // size
    att->st_blksize = nfi_att->at_blksize; // blocksize for filesystem I/O
    att->st_blocks = nfi_att->at_blocks; // number of blocks allocated
    att->st_atime = nfi_att->at_atime; // time of last access
    att->st_mtime = nfi_att->at_mtime; // time of last modification
    att->st_ctime = nfi_att->at_ctime; // time of last change
}


void MQ_SERVERtoNFIattr(struct nfi_attr * nfi_att, struct stat * att) 
{
    nfi_att->st_dev = att->st_dev;
    nfi_att->st_ino = att->st_ino;

    if (S_ISREG(att->st_mode)) {
        nfi_att->at_type = NFIFILE;
    }
    if (S_ISDIR(att->st_mode)) {
        nfi_att->at_type = NFIDIR;
    }

    nfi_att->at_mode = att->st_mode & (S_IRWXU | S_IRWXG | S_IRWXO); // protection
    nfi_att->at_nlink = att->st_nlink; // number of hard links
    nfi_att->at_uid = att->st_uid; // user ID of owner
    nfi_att->at_gid = att->st_gid; // group ID of owner
    nfi_att->at_size = att->st_size; // total size, in bytes
    nfi_att->at_blksize = att->st_blksize; // blocksize for filesystem I/O
    nfi_att->at_blocks = att->st_blocks; // number of blocks allocated
    nfi_att->at_atime = att->st_atime; // time of last access
    nfi_att->at_mtime = att->st_mtime; // time of last modification
    nfi_att->at_ctime = att->st_ctime; // time of last change
}

void MQ_SERVERtoNFIInfo(__attribute__((__unused__)) struct nfi_info * nfi_inf, __attribute__((__unused__)) struct nfi_info * mq_server_inf) {
    //TODO
}

/*
 * PUBLIC FUNCTIONS TO USE MQ_SERVER
 */

/************************************************************
 * Init mq_server                                               *
 ************************************************************/
int nfi_mq_server_init(char * url, struct nfi_server * serv, __attribute__((__unused__)) struct nfi_attr_server * attr) 
{
    debug_info("Inicio nfi_mq_server_init\n");

    int ret;
    char server[PATH_MAX], dir[PATH_MAX], prt[PATH_MAX];
    struct nfi_mq_server_server * server_aux;

    DEBUG_BEGIN();

    // check params...
    if (serv == NULL) 
    {
        debug_info("ERROR: serv argument is NULL.\n");
        return -1;
    }

    // new nfi_ops with mq_server functions...
    serv->ops = (struct nfi_ops * ) malloc(sizeof(struct nfi_ops));
    NULL_RET_ERR(serv->ops, MQ_SERVERERR_MEMORY);

    bzero(serv->ops, sizeof(struct nfi_ops));
    serv->ops->nfi_reconnect = nfi_mq_server_reconnect;
    serv->ops->nfi_disconnect = nfi_mq_server_disconnect;

    serv->ops->nfi_open = nfi_mq_server_open;
    serv->ops->nfi_create = nfi_mq_server_create;
    serv->ops->nfi_read = nfi_mq_server_read;
    serv->ops->nfi_write = nfi_mq_server_write;
    serv->ops->nfi_close = nfi_mq_server_close;
    serv->ops->nfi_remove = nfi_mq_server_remove;
    serv->ops->nfi_rename = nfi_mq_server_rename;
    serv->ops->nfi_getattr = nfi_mq_server_getattr;
    serv->ops->nfi_setattr = nfi_mq_server_setattr;

    serv->ops->nfi_opendir = nfi_mq_server_opendir;
    serv->ops->nfi_mkdir = nfi_mq_server_mkdir;
    serv->ops->nfi_readdir = nfi_mq_server_readdir;
    serv->ops->nfi_closedir = nfi_mq_server_closedir;
    serv->ops->nfi_rmdir = nfi_mq_server_rmdir;

    serv->ops->nfi_destroy = nfi_mq_server_destroy;
    serv->ops->nfi_statfs = nfi_mq_server_statfs;

    // TODO: add these new functions
    serv->ops->nfi_read_mdata  = nfi_mq_server_read_mdata;
    serv->ops->nfi_write_mdata = nfi_mq_server_write_mdata;

    // TODO: remove these old functions
 // serv->ops->nfi_preload = nfi_mq_server_preload;
 // serv->ops->nfi_flush = nfi_mq_server_flush;


    // parse url...
    ret = ParseURL(url, prt, NULL, NULL, server, NULL, dir);
    if (ret < 0) 
    {
        mq_server_err(MQ_SERVERERR_URL);
        debug_info("nfi_mq_server_init: incorrect url '%s'.\n", url);
        FREE_AND_NULL(serv->ops);
        return -1;
    }

    // new nfi_mpiserver_server...
    server_aux = (struct nfi_mq_server_server * ) malloc(sizeof(struct nfi_mq_server_server));
    if (server_aux == NULL) 
    {
        debug_info("ERROR: out of memory\n");
        FREE_AND_NULL(serv->ops);
        return -1;
    }

    serv->private_info = (void * ) server_aux;

    // Initialize params
    memset( & (server_aux->params), 0, sizeof(tcpClient_param_st));

    // thread checking
    server_aux->params.xpn_thread = TH_NOT;
    serv->xpn_thread = TH_NOT;
    char * env_thread = getenv("XPN_THREAD");
    if (env_thread != NULL) 
    {
        server_aux->params.xpn_thread = atoi(env_thread);
        serv->xpn_thread = atoi(env_thread);
    }

    // Session mode checking
    server_aux->params.xpn_session = 0;
    char * env_session = getenv("XPN_SESSION");
    if (env_session != NULL) 
    {
        server_aux->params.xpn_session = atoi(env_session);
    }

    // Locality mode checking
    server_aux->params.xpn_locality = 1;
    char * env_locality = getenv("XPN_LOCALITY");
    if (env_locality != NULL) 
    {
        server_aux->params.xpn_locality = atoi(env_locality);
    }

    server_aux->params.xpn_keep_connection = 1;
    char * env_connection = getenv("XPN_CONNECTION");
    if (env_connection != NULL) 
    {
        server_aux->params.xpn_keep_connection = atoi(env_connection);
    }
    

    // initialize TCP Client communication side...
    ret = tcpClient_comm_init( & (server_aux->params) );
    if (ret < 0) 
    {
        debug_info("Error nfi_mq_server_init: tcpClient_comm_init\n");
        FREE_AND_NULL(serv->ops);
        FREE_AND_NULL(server_aux);
        return -1;
    }

    // copy 'url' string...
    serv->url = strdup(url);
    NULL_RET_ERR(serv->url, MQ_SERVERERR_MEMORY);

    // new server wrk...
    serv->wrk = (struct nfi_worker * ) malloc(sizeof(struct nfi_worker));
    memset(serv->wrk, 0, sizeof(struct nfi_worker));
    serv->wrk->server = serv;

    debug_info("[NFI_MQ] nfiworker_init()\n");
    ret = nfiworker_init(serv);

    //int n_retries = 0;
    
    /*do
    {
        

        if (ret < 0)
        {
            
            n_retries++;
            printf ("\tConnect retry - %d - %s %s\n", n_retries, url, server_aux->params.server_name);
            //nanosleep((const struct timespec[]){{0, 500000L}}, NULL);
        }
        else
        {
            //printf ("\tConnect OK - %d - %s %s\n", n_retries, url, server_aux->params.server_name);
            break;
        //}

    }while((ret < 0) && (n_retries < 5));*/

    ret = nfi_mq_server_connect(serv, url, prt, server, dir);

    //printf("nfi_mq_server_connect - %d\n", server_aux->params.server );

    if (ret < 0) 
    {
        debug_info("Error nfi_mq_server_connect\n");
        FREE_AND_NULL(serv->ops);
        FREE_AND_NULL(server_aux);
        return -1;
    }

    //printf("[%d]\tURL-%s SERVER_NAME-%s SERVER-%d\n", __LINE__, url, server_aux->params.server_name, server_aux->params.server);

    int data = 0;
    
    ret = tcpClient_read_data( server_aux->params.server, (char *)&data, 1 * sizeof(int), "<unused msg_id>") ;

    if (ret < 0)
    {
        debug_info("Error tcpClient_read_data\n");
        FREE_AND_NULL(serv->ops);
        FREE_AND_NULL(server_aux);
        return -1;
    }

    debug_info("[%s][%d]\tnfi_mq_server_connect - %d\n", __FILE__, __LINE__, ret);

    ret = tcpClient_comm_locality( & (server_aux->params) );
    if (ret < 0) 
    {
        debug_info("Error tcpClient_comm_locality\n");
        FREE_AND_NULL(serv->ops);
        FREE_AND_NULL(server_aux);
        return -1;
    }

    //printf("[%s][%d]\ttcpClient_comm_locality - %d\n", __FILE__, __LINE__, ret);

    DEBUG_END();

    #ifdef HAVE_MOSQUITTO_H
    /*INIT MOSQUITTO CLIENT SIDE */
    int rc = 0;
    server_aux->params.xpn_mosquitto_mode = 0;
    char * env_mosquitto = getenv("XPN_MQTT");
    if (env_mosquitto != NULL) 
    {
        server_aux->params.xpn_mosquitto_mode = atoi(env_mosquitto);

        if (server_aux->params.xpn_mosquitto_mode == 1)                       //MQTT initialization
        {
            server_aux->params.xpn_mosquitto_qos = 0;
            char * env_qos_mqtt = getenv("XPN_MQTT_QOS");
            
            if (env_qos_mqtt != NULL) server_aux->params.xpn_mosquitto_qos = atoi(env_qos_mqtt);

            mosquitto_lib_init();
            server_aux->mqtt = mosquitto_new(NULL, true, NULL);

            if(server_aux->mqtt == NULL)
            {
                fprintf(stderr, "Error: Out of memory.\n");
                return -1;
            }

            mosquitto_int_option(server_aux->mqtt, MOSQ_OPT_TCP_NODELAY, 1);  
            mosquitto_int_option(server_aux->mqtt, MOSQ_OPT_SEND_MAXIMUM, 65535);


            rc = mosquitto_connect(server_aux->mqtt, server_aux->params.server_name, 1883, 0);

            if(rc != MOSQ_ERR_SUCCESS)
            {
                mosquitto_destroy(server_aux->mqtt);
                fprintf(stderr, "Error: %s\n", mosquitto_strerror(rc));
                return 1;
            }

            /* Run the network loop in a background thread, this call returns quickly. */
            rc = mosquitto_loop_start(server_aux->mqtt);

            if(rc != MOSQ_ERR_SUCCESS)
            {
                mosquitto_destroy(server_aux->mqtt);
                fprintf(stderr, "Error: %s\n", mosquitto_strerror(rc));
                return 1;
            }
        }
    }

    #endif



    ret = doDisconnection( & (server_aux->params) );
    if (ret < 0) 
    {
        fprintf(stderr, "ERROR: nfi_mq_server_init: DisConnection failed\n");
        return -1;
    }
    

    debug_info("Fin nfi_mq_server_init\n");

    // return OK
    return 0;
}

/************************************************************
 * Destroy mq_server                                       *
 * **********************************************************/
int nfi_mq_server_destroy(struct nfi_server * serv) 
{
    int ret;
    struct nfi_mq_server_server * server_aux;

    DEBUG_BEGIN();

    // check params...
    if (serv == NULL) 
    {
        return 0;
    }
    server_aux = (struct nfi_mq_server_server * ) serv->private_info;
    if (server_aux == NULL) 
    {
        return 0;
    }

    // Thread destroy...
    debug_info("[NFI_MQ] nfiworker_destroy()\n");
    nfiworker_destroy(serv);

    nfi_mq_server_disconnect( serv );

    // TCP Finalize...
    ret = tcpClient_comm_destroy( & (server_aux->params) );
    if (ret < 0) {
        debug_info("[NFI_MQ]: tcpClient_comm_destroy fails :-(");
    }

    // free private_info, 'url' string and 'server' string...
    FREE_AND_NULL(serv->ops);
    FREE_AND_NULL(serv->private_info);
    FREE_AND_NULL(serv->url);
    FREE_AND_NULL(serv->server);

    //serv->protocol = -1;
    DEBUG_END();

    #ifdef HAVE_MOSQUITTO_H

    if (server_aux->params.xpn_mosquitto_mode == 1)                       //MQTT finalization
    {
        mosquitto_disconnect(server_aux->mqtt);
        mosquitto_destroy(server_aux->mqtt);
        mosquitto_lib_cleanup();
    }   

    #endif

    // return OK
    return 0;
}



/*********************************************************
 * Connect to the server                                 *
 * *******************************************************/
int nfi_mq_server_connect(struct nfi_server * serv, __attribute__((__unused__)) char * url, __attribute__((__unused__)) char * prt, char * server, __attribute__((__unused__)) char * dir) {
    int ret;
    struct nfi_mq_server_server * server_aux;

    DEBUG_BEGIN();

    // check params...
    if (serv == NULL) 
    {
        return -1;
    }
    server_aux = (struct nfi_mq_server_server * )(serv->private_info);
    if (server_aux == NULL) 
    {
        return -1;
    }
    strcpy(server_aux->params.srv_name, server);
    debug_info("[%s][%d]\t%s\n", __FILE__, __LINE__, server);

    ret = tcpClient_comm_connect( & (server_aux->params));
    if (ret < 0) 
    {
        return -1;
    }

    DEBUG_END();

    return 0;
}



/************************************************************
 * Disconnect to the server                                 *
 * **********************************************************/
int nfi_mq_server_disconnect(struct nfi_server * serv) {
    int ret;
    struct nfi_mq_server_server * server_aux;

    DEBUG_BEGIN();

    // check params...
    if (serv == NULL) 
    {
        return 0;
    }
    server_aux = (struct nfi_mq_server_server * ) serv->private_info;
    if (server_aux == NULL) 
    {
        return 0;
    }

    if ( server_aux->params.xpn_keep_connection == 1 )
    {
        // TCP Disconnect... - ALWAYS RETURNS 0
        ret = tcpClient_comm_disconnect( & (server_aux->params));
        if (ret < 0) 
        {
            debug_info("[NFI_MQ]: tcpClient_comm_disconnect fails :-(");
        }
    }

    FREE_AND_NULL(serv->private_info);

    DEBUG_END();

    // return OK
    return 0;
}



/************************************************************
 * Reconnect to the MPI server                              *
 ************************************************************/
int nfi_mq_server_reconnect(struct nfi_server * serv) 
{
    // Don't see the serv result
    int ret;
    char server[PATH_MAX], dir[PATH_MAX];
    struct nfi_mq_server_server * server_aux;

    DEBUG_BEGIN();

    // Check arguments
    if (serv == NULL) 
    {
        return -1;
    }

    // ParseURL...
    ret = ParseURL(serv->url, NULL, NULL, NULL, server, NULL, dir);
    if (ret < 0) 
    {
        mq_server_err(MQ_SERVERERR_URL);
        debug_info("nfi_mq_server_reconnect: url %s incorrect.\n", serv->url);
        return -1;
    }

    // get private_info...
    server_aux = (struct nfi_mq_server_server * ) malloc(sizeof(struct nfi_mq_server_server));
    NULL_RET_ERR(server_aux, MQ_SERVERERR_MEMORY);

    strcpy(server_aux->path, dir);
    serv->private_info = (void * ) server_aux;

    // reconnect...
    ret = tcpClient_comm_connect( & (server_aux->params));
    if (ret < 0) 
    {
        debug_info("nfi_mq_server_reconnect: tcpClient_comm_connect fails.\n");
        return -1;
    }

    DEBUG_END();

    // return OK
    return 0;
}

/*
 *  GENERIC FUNCTIONS
 */

int nfi_mq_server_open ( struct nfi_server *serv,  char *url, int flags, mode_t mode, struct nfi_fhandle *fho )
{
    int ret;
    char dir[PATH_MAX], server[PATH_MAX];
    struct nfi_mq_server_server * server_aux;
    struct nfi_mq_server_fhandle * fh_aux;
    struct st_mq_server_msg msg;

    DEBUG_BEGIN();

    // Check arguments...
    NULL_RET_ERR(serv, MQ_SERVERERR_PARAM);
    NULL_RET_ERR(fho, MQ_SERVERERR_PARAM);
    nfi_mq_server_keepConnected(serv);
    NULL_RET_ERR(serv->private_info, MQ_SERVERERR_PARAM);

    // private_info...
    server_aux = (struct nfi_mq_server_server * ) serv->private_info;
    debug_info("[NFI_MQ] nfi_mq_server_open(ID=%s): begin %s\n", server_aux->id, url);

    ret = doConnection( & (server_aux->params) );
    if (ret < 0) 
    {
        fprintf(stderr, "ERROR: nfi_mq_server_open: Connection failed\n");
        return -1;
    }

    // from url->server + dir
    ret = ParseURL(url, NULL, NULL, NULL, server, NULL, dir);
    if (ret < 0) {
        fprintf(stderr, "nfi_mq_server_open: url %s incorrect.\n", url);
        mq_server_err(MQ_SERVERERR_URL);
        return -1;
    }

    fho->url = strdup(url);
    NULL_RET_ERR(fho->url, MQ_SERVERERR_MEMORY);

    fh_aux = (struct nfi_mq_server_fhandle * ) malloc(sizeof(struct nfi_mq_server_fhandle));
    if (fh_aux == NULL) {
        mq_server_err(MQ_SERVERERR_MEMORY);
        FREE_AND_NULL(fho->url);
        return -1;
    }

    /************** LOCAL *****************/
    if (server_aux->params.locality) 
    {
        fh_aux->fd = filesystem_open2(dir, O_RDWR, S_IRWXU);
        if (fh_aux->fd < 0) 
        {
            debug_info("filesystem_open fails to open '%s' in server %s.\n", dir, serv->server);
            FREE_AND_NULL(fh_aux);
            FREE_AND_NULL(fho->url);
            return -1;
        }
        if (server_aux->params.xpn_session == 0) 
        {
            filesystem_close(fh_aux->fd);
        }
        strcpy(fh_aux->path, dir);
    }
    /************** REMOTE ****************/
    else 
    {
        if (server_aux->params.xpn_session) 
        {
            msg.type = MQ_SERVER_OPEN_FILE_WS;
        } else 
        {
            msg.type = MQ_SERVER_OPEN_FILE_WOS;
        }
        memccpy(msg.id, server_aux->id, 0, MQ_SERVER_ID - 1);
	memccpy(msg.u_st_mq_server_msg.op_open.path, dir, 0, PATH_MAX - 1);
  	msg.u_st_mq_server_msg.op_open.flags = flags;
  	msg.u_st_mq_server_msg.op_open.mode = mode;
  	msg.u_st_mq_server_msg.op_open.xpn_session = serv->xpn_session_file;

        nfi_mq_server_doRequest(server_aux, & msg, (char * ) & (fh_aux->fd), sizeof(int));

        if (fh_aux->fd < 0) 
        {
            debug_info("filesystem_open fails to open '%s' in server %s.\n", dir, serv->server);
            FREE_AND_NULL(fh_aux);
            FREE_AND_NULL(fho->url);
            return -1;
        }

        memccpy(fh_aux->path, dir, 0, PATH_MAX - 1);

    }
    /*****************************************/

    fho->type = NFIFILE;
    fho->server = serv;
    fho->priv_fh = (void * ) fh_aux;


    ret = doDisconnection( & (server_aux->params) );
    if (ret < 0) 
    {
        fprintf(stderr, "ERROR: nfi_mq_server_open: DisConnection failed\n");
        return -1;
    }


    DEBUG_END();

    return 0;
}


int nfi_mq_server_create(struct nfi_server * serv, char * url, mode_t mode, __attribute__((__unused__)) struct nfi_attr * attr, struct nfi_fhandle * fh)
{
    // NOTE: actually creat is not in use, it use like POSIX open(path, O_WRONLY|O_CREAT|O_TRUNC, mode);
    return nfi_mq_server_open(serv, url, O_WRONLY|O_CREAT|O_TRUNC, mode, fh);
}


/*
int nfi_mq_server_create(struct nfi_server * serv, char * url, mode_t mode, struct nfi_attr * attr, struct nfi_fhandle * fh)
{
    int ret;
    char server[PATH_MAX], dir[PATH_MAX];
    struct nfi_mq_server_server * server_aux;
    struct nfi_mq_server_fhandle * fh_aux;
    struct st_mq_server_msg msg;
    struct st_mq_server_attr_req req;

    DEBUG_BEGIN();

    // Check arguments...
    NULL_RET_ERR(serv, MQ_SERVERERR_PARAM);
    NULL_RET_ERR(attr, MQ_SERVERERR_PARAM);
    nfi_mq_server_keepConnected(serv);
    NULL_RET_ERR(serv->private_info, MQ_SERVERERR_PARAM);

    // private_info...
    server_aux = (struct nfi_mq_server_server * ) serv->private_info;
    debug_info("[NFI_MQ] nfi_mq_server_create(ID=%s): begin %s\n", server_aux->id, url);

    ret = doConnection( & (server_aux->params) );
    if (ret < 0) 
    {
        fprintf(stderr, "ERROR: nfi_mq_server_create: Connection failed\n");
        return -1;
    }

    //printf("[NFI_MQ_SERVER] CREATE WOS - %s\n", url);

    // url->server + dir
    ret = ParseURL(url, NULL, NULL, NULL, server, NULL, dir);
    if (ret < 0) {
        fprintf(stderr, "ERROR: nfi_mq_server_create: url %s incorrect.\n", url);
        mq_server_err(MQ_SERVERERR_URL);
        return -1;
    }

    // private_info file handle
    fh_aux = (struct nfi_mq_server_fhandle * ) malloc(sizeof(struct nfi_mq_server_fhandle));
    NULL_RET_ERR(fh_aux, MQ_SERVERERR_MEMORY);
    bzero(fh_aux, sizeof(struct nfi_mq_server_fhandle));

    // create the file into the directory

    // ************** LOCAL *****************
    if (server_aux->params.locality) 
    {
        fh_aux->fd = filesystem_open2(dir, O_CREAT | O_RDWR | O_TRUNC, attr->at_mode);
        if (fh_aux->fd < 0) 
        {
            debug_info("files_posix_open fails to creat '%s' in server '%s'.\n", dir, serv->server);
            FREE_AND_NULL(fh_aux);
            DEBUG_END();
            return -1;
        }

        //Get stat
        ret = filesystem_stat(dir, & (req.attr));
        if (ret < 0) 
        {
            debug_info("nfi_mq_server_create: Fail stat %s.\n", dir);
            FREE_AND_NULL(fh_aux);
            DEBUG_END();
            return ret;
        }

        if (server_aux->params.xpn_session == 0) 
        {
            filesystem_close(fh_aux->fd);
        }
        strcpy(fh_aux->path, dir);
    }
    // ************** REMOTE ****************
    else 
    {        
        if (server_aux->params.xpn_session) 
        {
            msg.type = MQ_SERVER_CREAT_FILE_WS;
        } else 
        {
            msg.type = MQ_SERVER_CREAT_FILE_WOS;
        }
        memccpy(msg.id, server_aux->id, 0, MQ_SERVER_ID - 1);
        memccpy(msg.u_st_mq_server_msg.op_creat.path, dir, 0, PATH_MAX - 1);

        //printf("CreateClienteWos\n\n");
        //printf("[NFI_MQ_SERVER] CREATE WOS - %s\n", msg.u_st_mq_server_msg.op_creat.path);

        nfi_mq_server_doRequest(server_aux, & msg, (char * ) & (fh_aux->fd), sizeof(int));


        if (fh_aux->fd < 0) 
        {
            //printf("doRequest(creat) fails to open '%s' in server %s.\n", dir, serv->server);
            FREE_AND_NULL(fh_aux);
            DEBUG_END();
            return -1;
        }

        memccpy(fh_aux->path, dir, 0, PATH_MAX - 1);

        // Get stat
        //bzero(&msg, sizeof(struct st_mq_server_msg));
        msg.type = MQ_SERVER_GETATTR_FILE;
        memccpy(msg.id, server_aux->id, 0, MQ_SERVER_ID - 1);
        memccpy(msg.u_st_mq_server_msg.op_getattr.path, dir, 0, PATH_MAX - 1);

        nfi_mq_server_doRequest(server_aux, & msg, (char * ) & req, sizeof(struct st_mq_server_attr_req));

        //printf("nfi_mq_server_doRequest after gettattr\n");
    }
    // *****************************************

    fh->type = NFIFILE;
    fh->server = serv;
    fh->priv_fh = (void * ) fh_aux;

    fh->url = strdup(url);
    if (fh->url == NULL) 
    {
        mq_server_err(MQ_SERVERERR_MEMORY);
        FREE_AND_NULL(fh_aux);
        DEBUG_END();
        return -1;
    }

    MQ_SERVERtoNFIattr(attr, & req.attr);

    ret = doDisconnection( & (server_aux->params) );
    if (ret < 0) 
    {
        fprintf(stderr, "ERROR: nfi_mq_server_create: DisConnection failed\n");
        return -1;
    }
    

    DEBUG_END();

    return 0;
}
*/



ssize_t nfi_mq_server_read(struct nfi_server * serv, struct nfi_fhandle * fh, void * buffer, off_t offset, size_t size) 
{
    int ret, cont, diff;
    struct nfi_mq_server_server * server_aux;
    struct nfi_mq_server_fhandle * fh_aux;
    struct st_mq_server_msg msg;
    struct st_mq_server_read_req req;

    DEBUG_BEGIN();

    // Check arguments...
    NULL_RET_ERR(serv, MQ_SERVERERR_PARAM);
    NULL_RET_ERR(fh, MQ_SERVERERR_PARAM);
    nfi_mq_server_keepConnected(serv);
    NULL_RET_ERR(serv->private_info, MQ_SERVERERR_PARAM);

    // private_info
    server_aux = (struct nfi_mq_server_server * ) serv->private_info;
    debug_info("[NFI_MQ] nfi_mq_server_read(%s): begin off %d size %d\n", server_aux->id, (int) offset, (int) size);
    fh_aux = (struct nfi_mq_server_fhandle * ) fh->priv_fh;

    ret = doConnection( & (server_aux->params) );
    if (ret < 0) 
    {
        fprintf(stderr, "ERROR: nfi_mq_server_read: Connection failed\n");
        return -1;
    }

    /************** LOCAL *****************/
    if (server_aux->params.locality) 
    {
        if (server_aux->params.xpn_session) 
        {
            filesystem_lseek(fh_aux->fd, offset, SEEK_SET);
            //if(server_aux->params.sem_server != 0) sem_wait(server_aux->params.sem_server);
            ret = filesystem_read(fh_aux->fd, buffer, size);
            //if(server_aux->params.sem_server != 0) sem_post(server_aux->params.sem_server);

            debug_info("[NFI_MQ] read %s(%d) off %ld size %zu (ret:%zd)", fh->url, fh_aux->fd, (long int) offset, size, ret);
        } else 
        {
            int fd;

            fd = filesystem_open(fh_aux->path, O_RDONLY);
            if (fd < 0) 
            {
                debug_info("filesystem_read reads zero bytes from url:%s offset:%ld size:%zu (ret:%zd) errno=%d\n", fh->url, (long int) offset, size, ret, errno);
                return -1;
            }

            filesystem_lseek(fd, offset, SEEK_SET);
            //if(server_aux->params.sem_server != 0) sem_wait(server_aux->params.sem_server);
            ret = filesystem_read(fd, buffer, size);
            //if(server_aux->params.sem_server != 0) sem_post(server_aux->params.sem_server);

            debug_info("[NFI_MQ] read %s(%d) off %ld size %zu (ret:%zd)", fh->url, fd, (long int) offset, size, ret);

            filesystem_close(fd);
        }

        if (ret < 0) {
            debug_info("filesystem_read reads zero bytes from url:%s offset:%ld size:%zu (ret:%zd) errno=%d\n", fh->url, (long int) offset, size, ret, errno);
            return -1;
        }
    }
    /************** REMOTE ****************/
    else {
        // send read request
        if (server_aux->params.xpn_session) {
            msg.type = MQ_SERVER_READ_FILE_WS;
            msg.u_st_mq_server_msg.op_read.fd = fh_aux->fd;
            debug_info("[NFI_MQ] read:->fd     %d \n", msg.u_st_mq_server_msg.op_read.fd);
        } else {
            msg.type = MQ_SERVER_READ_FILE_WOS;
            memccpy(msg.u_st_mq_server_msg.op_read.path, fh_aux->path, 0, PATH_MAX);
            debug_info("[NFI_MQ] read:->path   %s \n", msg.u_st_mq_server_msg.op_read.path);
        }
        memccpy(msg.id, server_aux->id, 0, MQ_SERVER_ID - 1);
        msg.u_st_mq_server_msg.op_read.offset = offset;
        msg.u_st_mq_server_msg.op_read.size = size;

        debug_info("[NFI_MQ] read:->offset %d \n", (int) msg.u_st_mq_server_msg.op_read.offset);
        debug_info("[NFI_MQ] read:->size   %d \n", msg.u_st_mq_server_msg.op_read.size);

        ret = mq_server_write_operation(server_aux->params.server, & msg);
        if (ret < 0) {
            fprintf(stderr, "ERROR: (1)nfi_mq_server_read: Error on write operation\n");
            return -1;
        }

        // read n times: number of bytes + read data (n bytes)
        cont = 0;
        do {
            ret = tcpClient_read_data(server_aux->params.server, (char * ) & req, sizeof(struct st_mq_server_read_req), msg.id);
            debug_info("[NFI_MQ] nfi_mq_server_read(ID=%s): (1)tcpClient_read_data = %d.\n", server_aux->id, ret);

            if (ret < 0) {
                fprintf(stderr, "ERROR: (2)nfi_mq_server_read: Error on write operation\n");
                return -1;
            }

            // TODO: mq_server_ops.c:465 ->if (req.size < 0)->error on server side so... something must be done on client side

            if (req.size > 0) {
                debug_info("[NFI_MQ] nfi_mq_server_read(ID=%s): (2)tcpClient_read_data = %d. size = %d\n", server_aux->id, ret, req.size);
                ret = tcpClient_read_data(server_aux->params.server, (char * ) buffer + cont, req.size, msg.id);
                debug_info("[NFI_MQ] nfi_mq_server_read(ID=%s): (2)tcpClient_read_data = %d.\n", server_aux->id, ret);

                if (ret < 0) {
                    fprintf(stderr, "ERROR: (3)nfi_mq_server_read: Error on read operation\n");
                }
            }

            cont = cont + req.size;
            diff = msg.u_st_mq_server_msg.op_read.size - cont;

        } while ((diff > 0) && (req.size != 0));

        if (req.size < 0) {
            fprintf(stderr, "ERROR: nfi_mq_server_read: Fail read %s off %d size %d (err:%d).\n", fh->url, (int) offset, (int) size, (int) req.size);
            mq_server_err(MQ_SERVERERR_READ);
            return -1;
        }

        ret = cont;
    }

    int ret2 = doDisconnection( & (server_aux->params) );
    if (ret2 < 0) 
    {
        fprintf(stderr, "ERROR: nfi_mq_server_read: DisConnection failed\n");
        return -1;
    }
    

    DEBUG_END();

    return ret;
}



ssize_t nfi_mq_server_write(struct nfi_server * serv, struct nfi_fhandle * fh, void * buffer, off_t offset, size_t size) 
{
    int ret, diff, cont;

    struct nfi_mq_server_server * server_aux;
    struct nfi_mq_server_fhandle * fh_aux;
    struct st_mq_server_msg msg;
    struct st_mq_server_write_req req;

    DEBUG_BEGIN();

    // Check arguments...
    if (size == 0) {
        return 0;
    }

    NULL_RET_ERR(serv, MQ_SERVERERR_PARAM);
    NULL_RET_ERR(fh, MQ_SERVERERR_PARAM);
    nfi_mq_server_keepConnected(serv);
    NULL_RET_ERR(serv->private_info, MQ_SERVERERR_PARAM);

    // private_info...
    server_aux = (struct nfi_mq_server_server * ) serv->private_info;
    debug_info("[NFI_MQ] nfi_mq_server_write(ID=%s): begin off %d size %d\n", server_aux->id, (int) offset, (int) size);
    fh_aux = (struct nfi_mq_server_fhandle * ) fh->priv_fh;

    debug_info("ANTES ------------------- %s\n", server_aux->params.server_name);

    ret = doConnection( & (server_aux->params) );
    if (ret < 0) 
    {
        fprintf(stderr, "ERROR: nfi_mq_server_write: Connection failed\n");
        return -1;
    }


    if (server_aux->params.xpn_mosquitto_mode == 0)
    {
        /************** LOCAL *****************/
        if (server_aux->params.locality) 
        {
            if (server_aux->params.xpn_session) 
            {
                filesystem_lseek(fh_aux->fd, offset, SEEK_SET);
                //if(server_aux->params.sem_server != 0) sem_wait(server_aux->params.sem_server);
                ret = filesystem_write(fh_aux->fd, buffer, size);
                //if(server_aux->params.sem_server != 0) sem_post(server_aux->params.sem_server);

                debug_info("[NFI_MQ] write %s(%d) off %ld size %zu (ret:%zd)", fh->url, fh_aux->fd, (long int) offset, size, ret);
            } else 
            {
                int fd;

                fd = filesystem_open(fh_aux->path, O_WRONLY); // WOS
                if (fd < 0) {
                    debug_info("filesystem_write writes zero bytes from url:%s offset:%ld size:%zu (ret:%zd) errno=%d\n", fh->url, (long int) offset, size, ret, errno);
                    return -1;
                }

                filesystem_lseek(fd, offset, SEEK_SET);
                //if(server_aux->params.sem_server != 0) sem_wait(server_aux->params.sem_server);
                ret = filesystem_write(fd, buffer, size);
                //if(server_aux->params.sem_server != 0) sem_post(server_aux->params.sem_server);

                debug_info("[NFI_MQ] write %s(%d) off %ld size %zu (ret:%zd)", fh->url, fd, (long int) offset, size, ret);

                filesystem_close(fd); // WOS
            }

            if (ret < 0) {
                debug_info("filesystem_write writes zero bytes from url:%s offset:%ld size:%zu (ret:%zd) errno=%d\n", fh->url, (long int) offset, size, ret, errno);
                return -1;
            }
        }
        /************** REMOTE ****************/
        else 
        {
            // send write request
            if (server_aux->params.xpn_session) 
            {
                msg.type = MQ_SERVER_WRITE_FILE_WS;
                msg.u_st_mq_server_msg.op_write.fd = fh_aux->fd;
                debug_info("[NFI_MQ] write:->fd     %d \n", msg.u_st_mq_server_msg.op_write.fd);
            } else 
            {
                msg.type = MQ_SERVER_WRITE_FILE_WOS;
                memccpy(msg.u_st_mq_server_msg.op_write.path, fh_aux->path, 0, PATH_MAX - 1);
                debug_info("[NFI_MQ] write:->path   %s \n", msg.u_st_mq_server_msg.op_write.path);
            }

            memccpy(msg.id, server_aux->id, 0, MQ_SERVER_ID - 1);
            msg.u_st_mq_server_msg.op_write.offset = offset;
            msg.u_st_mq_server_msg.op_write.size = size;

            debug_info("[NFI_MQ] write:->offset %d \n", (int) msg.u_st_mq_server_msg.op_write.offset);
            debug_info("[NFI_MQ] write:->size   %d \n", msg.u_st_mq_server_msg.op_write.size);

            //printf("EscrituraClienteWos\n\n");

            ret = mq_server_write_operation(server_aux->params.server, & msg);
            if (ret < 0) 
            {
                fprintf(stderr, "(1)ERROR: nfi_mq_server_write(ID=%s): Error on write operation\n", server_aux->id);
                return -1;
            }

            // write n times: ...
            diff = size;
            cont = 0;

            // Max buffer size
            int buffer_size = size;
            if (buffer_size > MAX_BUFFER_SIZE) 
            {
                buffer_size = MAX_BUFFER_SIZE;
            }

            do 
            {
                int bytes_to_write = 0;

                if( diff > buffer_size )        bytes_to_write = buffer_size;
                else                            bytes_to_write = diff;

                
                ret = tcpClient_write_data(server_aux->params.server, (char * ) buffer + cont, bytes_to_write, msg.id); 
                //printf("REMOTE - %s - %d - %d - %d\n\n", server_aux->params.server_name, bytes_to_write, offset, ret);
                if (ret < 0) 
                {
                    fprintf(stderr, "(2)ERROR: nfi_mq_server_write(ID=%s): Error on write operation\n", server_aux->id);
                    return ret;
                }

                cont = cont + bytes_to_write; //Send bytes
                diff = size - cont;

            } while ((diff > 0) && (ret != 0));


            ret = tcpClient_read_data(server_aux->params.server, (char * ) & req, sizeof(struct st_mq_server_write_req), msg.id);

            if (ret < 0) 
            {
                fprintf(stderr, "(3)ERROR: nfi_mq_server_write(ID=%s): Error on write operation\n", server_aux->id);
                return -1;
            }
            debug_info("[NFI_MQ] nfi_mq_server_write(ID=%s): write %s off %d size %d (err:%d).\n", server_aux->id, fh->url, (int) offset, (int) size, (int) req.size);
            if (req.size < 0) 
            {
                fprintf(stderr, "ERROR: nfi_mq_server_write(ID=%s): Fail write %s off %d size %d (err:%d).\n", server_aux->id, fh->url, (int) offset, (int) size, (int) req.size);
                mq_server_err(MQ_SERVERERR_WRITE);
                return -1;
            }

            ret = cont;
        }
    }
    else
    {

        // write n times: ...
        diff = size;
        cont = 0;

        // Max buffer size
        int buffer_size = size;
        if (buffer_size > MAX_BUFFER_SIZE) 
        {
            buffer_size = MAX_BUFFER_SIZE;
        }

        do 
        {
            int bytes_to_write = 0;
            char *topic = malloc(strlen(fh_aux->path) + sizeof(bytes_to_write) + sizeof(offset) + 3);
           
            if( diff > buffer_size )        bytes_to_write = buffer_size;
            else                            bytes_to_write = diff;

            #ifdef HAVE_MOSQUITTO_H

            sprintf(topic, "%s/%d/%ld", fh_aux->path, bytes_to_write, offset);
            debug_info("\nCLIENTE ESCRITURA - %s - topic=%s\n", fh_aux->path, topic);
            debug_info("\nREMOTE MQTT- %s - %d - %d\n\n", server_aux->params.server_name, cont, bytes_to_write);

            ret = mosquitto_publish(server_aux->mqtt, NULL, topic, bytes_to_write, (char * ) buffer + cont, server_aux->params.xpn_mosquitto_qos, false);

            if(ret != MOSQ_ERR_SUCCESS)
            {
                fprintf(stderr, "Error publishing write: %s\n", mosquitto_strerror(ret));
                free(topic);
                return -1;
            }
            
            #endif
            if (ret < 0) 
            {
                fprintf(stderr, "(2)ERROR: nfi_mq_server_write(ID=%s): Error on write operation\n", server_aux->id);
            }

            free(topic);
            cont = cont + bytes_to_write; //Send bytes
            diff = size - cont;
            
        } while ((diff > 0) && (ret != 0));        

        ret = cont;
    }

    int ret2 = doDisconnection( & (server_aux->params) );
    if (ret2 < 0) 
    {
        fprintf(stderr, "ERROR: nfi_mq_server_write: DisConnection failed\n");
        return -1;
    }

    DEBUG_END();

    return ret;
}



int nfi_mq_server_close(struct nfi_server * serv, struct nfi_fhandle * fh) {
    int ret = -1;
    struct nfi_mq_server_fhandle * fh_aux;
    struct nfi_mq_server_server * server_aux;
    struct st_mq_server_msg msg;

    DEBUG_BEGIN();

    // Check arguments...
    NULL_RET_ERR(serv, MQ_SERVERERR_PARAM);
    NULL_RET_ERR(fh, MQ_SERVERERR_PARAM);
    nfi_mq_server_keepConnected(serv);
    NULL_RET_ERR(serv->private_info, MQ_SERVERERR_PARAM);

    // private_info...
    server_aux = (struct nfi_mq_server_server * ) serv->private_info;
    debug_info("[NFI_MQ] nfi_mq_server_close(ID=%s): begin\n", server_aux->id);

    ret = doConnection( & (server_aux->params) );
    if (ret < 0) 
    {
        fprintf(stderr, "ERROR: nfi_mq_server_close: Connection failed\n");
        return -1;
    }

    // without session->just return ok
    if (server_aux->params.xpn_session == 0) 
    {
        debug_info("[NFI_MQ] nfi_mq_server_close(ID=%s): end\n", server_aux->id);
        return 1;
    }

    if (NULL == fh->priv_fh) {
        debug_info("[NFI_MQ] nfi_mq_server_close(ID=%s): end\n", server_aux->id);
        return -1;
    }

    fh_aux = (struct nfi_mq_server_fhandle * ) fh->priv_fh;
    server_aux = (struct nfi_mq_server_server * ) serv->private_info;

    /************** LOCAL *****************/
    if (server_aux->params.locality) {
        if (fh_aux != NULL) {
            //if(server_aux->params.sem_server != 0) sem_wait(server_aux->params.sem_server);
            ret = filesystem_close(fh_aux->fd);
            //if(server_aux->params.sem_server != 0) sem_post(server_aux->params.sem_server);
        }
    }
    /************** REMOTE ****************/
    else {
        //bzero(&msg, sizeof(struct st_mq_server_msg));

        msg.type = MQ_SERVER_CLOSE_FILE_WS;
        memccpy(msg.id, server_aux->id, 0, MQ_SERVER_ID - 1);
        msg.u_st_mq_server_msg.op_close.fd = fh_aux->fd;
        memccpy(msg.u_st_mq_server_msg.op_close.path, fh_aux->path, 0, PATH_MAX - 1);

        nfi_mq_server_doRequest(server_aux, & msg, (char * ) & (ret), sizeof(int));
        debug_info("[NFI_MQ] nfi_mq_server_close(ID=%s): close->%d\n", server_aux->id, msg.u_st_mq_server_msg.op_close.fd);
    }

    // free memory
    FREE_AND_NULL(fh->priv_fh);
    fh->type = NFINULL;
    fh->server = NULL;

    int ret2 = doDisconnection( & (server_aux->params) );
    if (ret2 < 0) 
    {
        fprintf(stderr, "ERROR: nfi_mq_server_close: DisConnection failed\n");
        return -1;
    }
    

    DEBUG_END();

    // Return OK
    return ret;
}



int nfi_mq_server_remove(struct nfi_server * serv, char * url) 
{
    int ret;
    char server[PATH_MAX], dir[PATH_MAX];
    struct nfi_mq_server_server * server_aux;
    struct st_mq_server_msg msg;    

    // Check arguments...
    NULL_RET_ERR(serv, MQ_SERVERERR_PARAM);
    NULL_RET_ERR(url, MQ_SERVERERR_PARAM);
    nfi_mq_server_keepConnected(serv);
    NULL_RET_ERR(serv->private_info, MQ_SERVERERR_PARAM);

    // private_info...
    server_aux = (struct nfi_mq_server_server * ) serv->private_info;
    debug_info("[NFI_MQ] nfi_mq_server_remove(%s): begin %s\n", server_aux->id, url);
    if (server_aux == NULL) 
    {
        mq_server_err(MQ_SERVERERR_PARAM);
        return -1;
    }

    ret = doConnection( & (server_aux->params) );
    if (ret < 0) 
    {
        fprintf(stderr, "ERROR: nfi_mq_server_remove: Connection failed\n");
        return -1;
    }

    // from url->server + dir
    ret = ParseURL(url, NULL, NULL, NULL, server, NULL, dir);
    if (ret < 0) 
    {
        fprintf(stderr, "nfi_mq_server_remove: url %s incorrect.\n", url);
        mq_server_err(MQ_SERVERERR_URL);
        return -1;
    }

    /************** LOCAL *****************/
    if (server_aux->params.locality) 
    {
        ret = filesystem_unlink(dir);
        if (ret < 0) {
            debug_info("filesystem_open fails to open '%s' in server %s.\n", dir, serv->server);
            return -1;
        }
    }
    /************** REMOTE ****************/
    else 
    {
        msg.type = MQ_SERVER_RM_FILE;
        memccpy(msg.id, server_aux->id, 0, MQ_SERVER_ID - 1);
        memccpy(msg.u_st_mq_server_msg.op_rm.path, dir, 0, PATH_MAX - 1);

        nfi_mq_server_doRequest(server_aux, & msg, (char * ) & (ret), sizeof(int));
    }

    int ret2 = doDisconnection( & (server_aux->params) );
    if (ret2 < 0) 
    {
        fprintf(stderr, "ERROR: nfi_mq_server_remove: DisConnection failed\n");
        return -1;
    }
    

    DEBUG_END();

    return ret;
}



int nfi_mq_server_rename(struct nfi_server * serv, char * old_url, char * new_url) 
{
    int ret;
    char server[PATH_MAX], old_path[PATH_MAX], new_path[PATH_MAX];
    struct nfi_mq_server_server * server_aux;
    struct st_mq_server_msg msg;

    DEBUG_BEGIN();    

    // Check arguments...
    NULL_RET_ERR(serv, MQ_SERVERERR_PARAM);
    NULL_RET_ERR(old_url, MQ_SERVERERR_PARAM);
    NULL_RET_ERR(new_url, MQ_SERVERERR_PARAM);
    nfi_mq_server_keepConnected(serv);
    NULL_RET_ERR(serv->private_info, MQ_SERVERERR_PARAM);

    // private_info...
    server_aux = (struct nfi_mq_server_server * ) serv->private_info;
    debug_info("[NFI_MQ] nfi_mq_server_remove(%s): begin %s\n", server_aux->id, new_url);
    if (server_aux == NULL) 
    {
        mq_server_err(MQ_SERVERERR_PARAM);
        return -1;
    }

    ret = doConnection( & (server_aux->params) );
    if (ret < 0) 
    {
        fprintf(stderr, "ERROR: nfi_mq_server_rename: Connection failed\n");
        return -1;
    }

    ret = ParseURL(old_url, NULL, NULL, NULL, server, NULL, old_path);
    if (ret < 0) 
    {
        fprintf(stderr, "nfi_mq_server_open: url %s incorrect.\n", old_url);
        mq_server_err(MQ_SERVERERR_URL);
        return -1;
    }

    ret = ParseURL(new_url, NULL, NULL, NULL, server, NULL, new_path);
    if (ret < 0) 
    {
        fprintf(stderr, "nfi_mq_server_open: url %s incorrect.\n", new_url);
        mq_server_err(MQ_SERVERERR_URL);
        return -1;
    }

    /************** LOCAL *****************/
    if (server_aux->params.locality) 
    {
        ret = filesystem_rename(old_path, new_path);
        if (ret < 0) 
        {
            debug_info("filesystem_rename fails to rename '%s' in server %s.\n", old_path, serv->server);
            return -1;
        }
    }
    /************** REMOTE ****************/
    else 
    {
        msg.type = MQ_SERVER_RENAME_FILE;

        memccpy(msg.id, server_aux->id, 0, MQ_SERVER_ID - 1);
        memccpy(msg.u_st_mq_server_msg.op_rename.old_url, old_path, 0, PATH_MAX - 1);
        memccpy(msg.u_st_mq_server_msg.op_rename.new_url, new_path, 0, PATH_MAX - 1);

        nfi_mq_server_doRequest(server_aux, & msg, (char * ) & (ret), sizeof(int));
    }

    int ret2 = doDisconnection( & (server_aux->params) );
    if (ret2 < 0) 
    {
        fprintf(stderr, "ERROR: nfi_mq_server_rename: DisConnection failed\n");
        return -1;
    }
    

    DEBUG_END();

    return ret;
}



int nfi_mq_server_getattr(struct nfi_server * serv, struct nfi_fhandle * fh, struct nfi_attr * attr) 
{
    int ret;
    char server[PATH_MAX], dir[PATH_MAX];
    struct nfi_mq_server_server * server_aux;
    //struct nfi_mq_server_fhandle *fh_aux;
    struct st_mq_server_msg msg;
    struct st_mq_server_attr_req req;

    DEBUG_BEGIN();

    // check arguments...
    NULL_RET_ERR(serv, MQ_SERVERERR_PARAM);
    NULL_RET_ERR(fh, MQ_SERVERERR_PARAM);
    NULL_RET_ERR(attr, MQ_SERVERERR_PARAM);
    nfi_mq_server_keepConnected(serv);
    NULL_RET_ERR(serv->private_info, MQ_SERVERERR_PARAM);

    // copy private information...
    //fh_aux     = (struct nfi_mq_server_fhandle *) fh->priv_fh; //TODO: fstat
    //server_aux = (struct nfi_mq_server_server * ) serv->private_info;

    // private_info...
    server_aux = (struct nfi_mq_server_server * ) serv->private_info;
    debug_info("[NFI_MQ] nfi_mq_server_write(ID=%s): begin off %d size %d\n", server_aux->id, (int) offset, (int) size);
    //fh_aux = (struct nfi_mq_server_fhandle * ) fh->priv_fh;

    ret = doConnection( & (server_aux->params) );
    if (ret < 0) 
    {
        fprintf(stderr, "ERROR: nfi_mq_server_getattr: Connection failed\n");
        return -1;
    }

    ret = ParseURL(fh->url, NULL, NULL, NULL, server, NULL, dir);
    if (ret < 0) {
        fprintf(stderr, "nfi_mq_server_getattr: url %s incorrect.\n", dir);
        mq_server_err(MQ_SERVERERR_URL);
        return -1;
    }

    /************** LOCAL *****************/
    if (server_aux->params.locality) 
    {
        req.status = filesystem_stat(dir, & (req.attr));
        if (((int) req.status) < 0) {
            debug_info("nfi_mq_server_getattr: Fail stat %s.\n", dir);
            return req.status;
        }
    }
    /************** REMOTE ****************/
    else 
    {
        msg.type = MQ_SERVER_GETATTR_FILE;
        memccpy(msg.id, server_aux->id, 0, MQ_SERVER_ID - 1);
        memccpy(msg.u_st_mq_server_msg.op_getattr.path, dir, 0, PATH_MAX - 1);

        nfi_mq_server_doRequest(server_aux, & msg, (char * ) & req, sizeof(struct st_mq_server_attr_req));
    }

    MQ_SERVERtoNFIattr(attr, & req.attr);

    int ret2 = doDisconnection( & (server_aux->params) );
    if (ret2 < 0) 
    {
        fprintf(stderr, "ERROR: nfi_mq_server_getattr: DisConnection failed\n");
        return -1;
    }
    

    DEBUG_END();

    // return status
    return req.status;
}



int nfi_mq_server_setattr(struct nfi_server * serv, struct nfi_fhandle * fh, struct nfi_attr * attr) 
{
    struct nfi_mq_server_server * server_aux;
    struct nfi_mq_server_fhandle * fh_aux;

    DEBUG_BEGIN();

    // Check arguments...
    NULL_RET_ERR(serv, MQ_SERVERERR_PARAM);
    NULL_RET_ERR(fh, MQ_SERVERERR_PARAM);
    NULL_RET_ERR(attr, MQ_SERVERERR_PARAM);
    NULL_RET_ERR(fh->priv_fh, MQ_SERVERERR_PARAM);
    nfi_mq_server_keepConnected(serv);
    NULL_RET_ERR(serv->private_info, MQ_SERVERERR_PARAM);

    fh_aux = (struct nfi_mq_server_fhandle * ) fh->priv_fh;
    server_aux = (struct nfi_mq_server_server * ) serv->private_info;

    // TODO: setattr
    server_aux = server_aux;
    fh_aux = fh_aux;

    DEBUG_END();

    return 0;
}



int nfi_mq_server_mkdir(struct nfi_server *serv,  char *url, mode_t mode, __attribute__((__unused__)) struct nfi_attr *attr, __attribute__((__unused__)) struct nfi_fhandle *fh)
{
    int ret;
    char server[PATH_MAX], dir[PATH_MAX];
    struct nfi_mq_server_server * server_aux;
    struct nfi_mq_server_fhandle * fh_aux;
    struct st_mq_server_msg msg;
    struct st_mq_server_attr_req req;

    DEBUG_BEGIN();

    // Check arguments...
    NULL_RET_ERR(serv, MQ_SERVERERR_PARAM);
    NULL_RET_ERR(attr, MQ_SERVERERR_PARAM);
    nfi_mq_server_keepConnected(serv);
    NULL_RET_ERR(serv->private_info, MQ_SERVERERR_PARAM);

    // private_info...
    server_aux = (struct nfi_mq_server_server * ) serv->private_info;

    ret = doConnection( & (server_aux->params) );
    if (ret < 0) 
    {
        fprintf(stderr, "ERROR: nfi_mq_server_mkdir: Connection failed\n");
        return -1;
    }

    ret = ParseURL(url, NULL, NULL, NULL, server, NULL, dir);
    if (ret < 0) 
    {
        fprintf(stderr, "nfi_mq_server_mkdir: url %s incorrect.\n", url);
        mq_server_err(MQ_SERVERERR_URL);
        return -1;
    }

    // private_info file handle
    fh_aux = (struct nfi_mq_server_fhandle * ) malloc(sizeof(struct nfi_mq_server_fhandle));
    NULL_RET_ERR(fh_aux, MQ_SERVERERR_MEMORY);
    bzero(fh_aux, sizeof(struct nfi_mq_server_fhandle));

    /************** LOCAL *****************/
    if (server_aux->params.locality) 
    {
        ret = filesystem_mkdir(dir, /*attr->at_mode*/ 0777);
        if ((ret < 0) && (errno != EEXIST)) 
        {
            debug_info("nfi_mq_server_mkdir: Fail mkdir %s.\n", dir);
            FREE_AND_NULL(fh_aux);
            return -1;
        }
        fh_aux->fd = ret; //Cuidado

        //Get stat
        ret = filesystem_stat(dir, & (req.attr));
        if (ret < 0) {
            debug_info("nfi_mq_server_create: Fail stat %s.\n", dir);
            return ret;
        }
    }
    /************** SERVER ****************/
    else 
    {
        msg.type = MQ_SERVER_MKDIR_DIR;
        msg.u_st_mq_server_msg.op_mkdir.mode = mode;
        memccpy(msg.u_st_mq_server_msg.op_mkdir.path, dir, 0, PATH_MAX - 1);

        nfi_mq_server_doRequest(server_aux, & msg, (char * ) & (fh_aux->fd), sizeof(int));
        memccpy(fh_aux->path, dir, 0, PATH_MAX - 1);

        if ((fh_aux->fd < 0) && (errno != EEXIST)) 
        {
            mq_server_err(MQ_SERVERERR_MKDIR);
            fprintf(stderr, "nfi_mq_server_mkdir: Fail mkdir %s in server %s.\n", dir, serv->server);
            FREE_AND_NULL(fh_aux);
            return -1;
        }

        // Get stat
        msg.type = MQ_SERVER_GETATTR_FILE;
        msg.u_st_mq_server_msg.op_mkdir.mode = mode;
        memccpy(msg.id, server_aux->id, 0, MQ_SERVER_ID - 1);
        memccpy(msg.u_st_mq_server_msg.op_getattr.path, dir, 0, PATH_MAX - 1);

        nfi_mq_server_doRequest(server_aux, & msg, (char * ) & req, sizeof(struct st_mq_server_attr_req));
    }

    fh->type = NFIDIR;
    fh->priv_fh = (void * ) fh_aux;

    fh->url = STRING_MISC_StrDup(url);
    if (fh->url == NULL) {
        mq_server_err(MQ_SERVERERR_MEMORY);
        FREE_AND_NULL(fh_aux);
        return -1;
    }

    MQ_SERVERtoNFIattr(attr, & req.attr);

    int ret2 = doDisconnection( & (server_aux->params) );
    if (ret2 < 0) 
    {
        fprintf(stderr, "ERROR: nfi_mq_server_mkdir: DisConnection failed\n");
        return -1;
    }


    DEBUG_END();

    return ret;
}



int nfi_mq_server_opendir(struct nfi_server * serv, char * url, struct nfi_fhandle * fho) 
{
    int ret;
    char dir[PATH_MAX], server[PATH_MAX];
    struct nfi_mq_server_server * server_aux;
    struct nfi_mq_server_fhandle * fh_aux;
    struct st_mq_server_msg msg;

    DEBUG_BEGIN();    

    // Check arguments...
    NULL_RET_ERR(serv, MQ_SERVERERR_PARAM);
    NULL_RET_ERR(url, MQ_SERVERERR_PARAM);
    NULL_RET_ERR(fho, MQ_SERVERERR_PARAM);
    nfi_mq_server_keepConnected(serv);
    NULL_RET_ERR(serv->private_info, MQ_SERVERERR_PARAM);

    // private_info...
    server_aux = (struct nfi_mq_server_server * ) serv->private_info;

    ret = doConnection( & (server_aux->params) );
    if (ret < 0) 
    {
        fprintf(stderr, "ERROR: nfi_mq_server_opendir: Connection failed\n");
        return -1;
    }

    // private_info...
    ret = ParseURL(url, NULL, NULL, NULL, server, NULL, dir);
    if (ret < 0) {
        fprintf(stderr, "nfi_mq_server_opendir: url %s incorrect.\n", url);
        mq_server_err(MQ_SERVERERR_URL);
        return -1;
    }

    fho->url = strdup(url);
    NULL_RET_ERR(fho->url, MQ_SERVERERR_MEMORY);

    fh_aux = (struct nfi_mq_server_fhandle * ) malloc(sizeof(struct nfi_mq_server_fhandle));
    if (fh_aux == NULL) {
        mq_server_err(MQ_SERVERERR_MEMORY);
        FREE_AND_NULL(fho->url);
        return -1;
    }

    server_aux = (struct nfi_mq_server_server * ) serv->private_info;

    /************** LOCAL *****************/
    if (server_aux->params.locality) 
    {
        fh_aux->dir = filesystem_opendir(dir);
        if (fh_aux->dir == NULL) 
        {
            FREE_AND_NULL(fh_aux);
            FREE_AND_NULL(fho->url);
            debug_info("filesystem_opendir fails to open directory '%s' in server '%s'.\n", dir, serv->server);
            return -1;
        }
    }
    /************** SERVER ****************/
    else 
    {
        msg.type = MQ_SERVER_OPENDIR_DIR;
        memccpy(msg.id, server_aux->id, 0, MQ_SERVER_ID - 1);
        memccpy(msg.u_st_mq_server_msg.op_opendir.path, dir, 0, PATH_MAX - 1);

        unsigned long long aux;
        nfi_mq_server_doRequest(server_aux, & msg, (char * ) & (aux), sizeof(DIR * ));

        fh_aux->dir = (DIR * ) aux;
    }

    strcpy(fh_aux->path, dir);
    fho->type = NFIDIR;
    fho->server = serv;
    fho->priv_fh = (void * ) fh_aux;

    int ret2 = doDisconnection( & (server_aux->params) );
    if (ret2 < 0) 
    {
        fprintf(stderr, "ERROR: nfi_mq_server_opendir: DisConnection failed\n");
        return -1;
    }
    

    DEBUG_END();

    return 0;
}



int nfi_mq_server_readdir(struct nfi_server * serv, struct nfi_fhandle * fh, struct dirent * entry) 
{
    int ret = 0;
    struct nfi_mq_server_server * server_aux;
    struct nfi_mq_server_fhandle * fh_aux;
    struct st_mq_server_msg msg;
    struct st_mq_server_direntry ret_entry;
    struct dirent * ent;

    DEBUG_BEGIN();

    // Check arguments...
    NULL_RET_ERR(serv, MQ_SERVERERR_PARAM);
    NULL_RET_ERR(fh, MQ_SERVERERR_PARAM);
    NULL_RET_ERR(fh->priv_fh, MQ_SERVERERR_PARAM);

    if (fh->type != NFIDIR) 
    {
        mq_server_err(MQ_SERVERERR_NOTDIR);
        return -1;
    }

    nfi_mq_server_keepConnected(serv);
    NULL_RET_ERR(serv->private_info, MQ_SERVERERR_PARAM);

    // private_info...
    server_aux = (struct nfi_mq_server_server * ) serv->private_info;
    fh_aux = (struct nfi_mq_server_fhandle * ) fh->priv_fh;

    ret = doConnection( & (server_aux->params) );
    if (ret < 0) 
    {
        fprintf(stderr, "ERROR: nfi_mq_server_readdir: Connection failed\n");
        return -1;
    }
    

    // clean all entry content
    memset(entry, 0, sizeof(struct dirent));

    /************** LOCAL *****************/
    if (server_aux->params.locality) 
    {
        ent = filesystem_readdir(fh_aux->dir);
        if (ent == NULL) {
            debug_info("nfi_mq_server_readdir: readdir");
            return -1;
        }

        memcpy(entry, ent, sizeof(struct dirent));
    }
    /************** SERVER ****************/
    else 
    {
        msg.type = MQ_SERVER_READDIR_DIR;
        memccpy(msg.id, server_aux->id, 0, MQ_SERVER_ID - 1);
        msg.u_st_mq_server_msg.op_readdir.dir = fh_aux->dir;

        nfi_mq_server_doRequest(server_aux, & msg, (char * ) & (ret_entry), sizeof(struct st_mq_server_direntry)); //NEW

        if (ret_entry.end == 0) 
        {
            return -1;
        }

        memcpy(entry, & (ret_entry.ret), sizeof(struct dirent));
    }

    int ret2 = doDisconnection( & (server_aux->params) );
    if (ret2 < 0) 
    {
        fprintf(stderr, "ERROR: nfi_mq_server_readdir: DisConnection failed\n");
        return -1;
    }
    

    DEBUG_END();

    return 0;
}



int nfi_mq_server_closedir(struct nfi_server * serv, struct nfi_fhandle * fh) 
{
    int ret;
    struct st_mq_server_msg msg;
    struct nfi_mq_server_server * server_aux;
    struct nfi_mq_server_fhandle * fh_aux;

    DEBUG_BEGIN();
    

    // Check arguments...
    NULL_RET_ERR(serv, MQ_SERVERERR_PARAM);
    NULL_RET_ERR(fh, MQ_SERVERERR_PARAM);
    nfi_mq_server_keepConnected(serv);
    NULL_RET_ERR(serv->private_info, MQ_SERVERERR_PARAM);

    // do closedir...
    if (fh->priv_fh != NULL) 
    {
        // private_info...
        server_aux = (struct nfi_mq_server_server * ) serv->private_info;
        fh_aux = (struct nfi_mq_server_fhandle * ) fh->priv_fh;

        ret = doConnection( & (server_aux->params) );
        if (ret < 0) 
        {
            fprintf(stderr, "ERROR: nfi_mq_server_closedir: Connection failed\n");
            return -1;
        }

        /************** LOCAL *****************/
        if (server_aux->params.locality) 
        {
            filesystem_closedir(fh_aux->dir);
        }
        /************** SERVER ****************/
        else 
        {
            msg.type = MQ_SERVER_CLOSEDIR_DIR;

            memccpy(msg.id, server_aux->id, 0, MQ_SERVER_ID - 1);
            msg.u_st_mq_server_msg.op_closedir.dir = fh_aux->dir;

            nfi_mq_server_doRequest(server_aux, & msg, (char * ) & (ret), sizeof(int)); //NEW

        }

        int ret2 = doDisconnection( & (server_aux->params) );
        if (ret2 < 0) 
        {
            fprintf(stderr, "ERROR: nfi_mq_server_closedir: DisConnection failed\n");
            return -1;
        }

        // free memory
        FREE_AND_NULL(fh->priv_fh);
    }



    DEBUG_END();

    return 0;
}



int nfi_mq_server_rmdir(struct nfi_server * serv, char * url) 
{
    int ret;
    char server[PATH_MAX], dir[PATH_MAX];
    struct nfi_mq_server_server * server_aux;
    struct st_mq_server_msg msg;

    DEBUG_BEGIN();


    // Check arguments...
    NULL_RET_ERR(serv, MQ_SERVERERR_PARAM);
    NULL_RET_ERR(url, MQ_SERVERERR_PARAM);
    nfi_mq_server_keepConnected(serv);
    NULL_RET_ERR(serv->private_info, MQ_SERVERERR_PARAM);

    // private_info...
    server_aux = (struct nfi_mq_server_server * ) serv->private_info;

    ret = doConnection( & (server_aux->params) );
    if (ret < 0) 
    {
        fprintf(stderr, "ERROR: nfi_mq_server_rmdir: Connection failed\n");
        return -1;
    }

    ret = ParseURL(url, NULL, NULL, NULL, server, NULL, dir);
    if (ret < 0) 
    {
        fprintf(stderr, "nfi_mq_server_rmdir: url %s incorrect.\n", url);
        mq_server_err(MQ_SERVERERR_URL);
        return -1;
    }

    /************** LOCAL *****************/
    if (server_aux->params.locality) 
    {
        ret = filesystem_rmdir(dir);
        if (ret < 0) 
        {
            debug_info(stderr, "nfi_mq_server_rmdir: Fail rmdir %s.\n", dir);
            return -1;
        }
    }
    /************** SERVER ****************/
    else 
    {
        msg.type = MQ_SERVER_RMDIR_DIR;
        debug_info("RMDIR - %d\n", msg.type);
        memccpy(msg.u_st_mq_server_msg.op_rmdir.path, dir, 0, PATH_MAX - 1);
        nfi_mq_server_doRequest(server_aux, & msg, (char * ) & (ret), sizeof(int));

        if (ret < 0) 
        {
            fprintf(stderr, "nfi_mq_server_rmdir: Fail rmdir %s in server %s.\n", dir, serv->server);
            mq_server_err(MQ_SERVERERR_REMOVE);
            return -1;
        }
    }

    int ret2 = doDisconnection( & (server_aux->params) );
    if (ret2 < 0) 
    {
        fprintf(stderr, "ERROR: nfi_mq_server_rmdir: DisConnection failed\n");
        return -1;
    }
    

    DEBUG_END();

    return 0;
}



int nfi_mq_server_statfs(__attribute__((__unused__)) struct nfi_server * serv, __attribute__((__unused__)) struct nfi_info * inf) 
{
    DEBUG_BEGIN();

    /*
    struct mq_server_info mq_serverinf;
    int ret;
    struct nfi_mq_server_server *server_aux;

    // Check arguments...
    NULL_RET_ERR(serv, MQ_SERVERERR_PARAM) ;
    NULL_RET_ERR(inf,  MQ_SERVERERR_PARAM) ;
    nfi_mq_server_keepConnected(serv) ;
    NULL_RET_ERR(serv->private_info, MQ_SERVERERR_PARAM) ;

    // private_info...
    server_aux = (struct nfi_mq_server_server *)serv->private_info;

    ret = mq_server_statfs(server_aux->fh, &mq_serverinf, server_aux->cl) ;
    if (ret <0).{
      mq_server_err(MQ_SERVERERR_STATFS) ;
      return -1;
    }

    MPItoNFIInfo(inf, &mq_serverinf) ;
    */

    //TODO

    DEBUG_END();

    return 0;
}



int nfi_mq_server_preload(struct nfi_server * serv, char * url, char * virtual_path, char * storage_path, int opt) 
{
    //char dir[PATH_MAX];
    int ret;
    struct nfi_mq_server_server * server_aux;
    struct st_mq_server_msg msg;

    DEBUG_BEGIN();

    // Check arguments...
    NULL_RET_ERR(serv, MQ_SERVERERR_PARAM);
    NULL_RET_ERR(url, MQ_SERVERERR_PARAM);
    NULL_RET_ERR(virtual_path, MQ_SERVERERR_PARAM);
    NULL_RET_ERR(storage_path, MQ_SERVERERR_PARAM);
    nfi_mq_server_keepConnected(serv);
    NULL_RET_ERR(serv->private_info, MQ_SERVERERR_PARAM);

    // get private_info...
    server_aux = (struct nfi_mq_server_server * ) serv->private_info;
    debug_info("[NFI_MQ] nfi_mq_server_preload(ID=%s): begin %s - %s \n", server_aux->id, virtual_path, storage_path);

    if (url[strlen(url) - 1] == '/') {
        mq_server_err(MQ_SERVERERR_PARAM);
        return -1;
    }

    server_aux = (struct nfi_mq_server_server * ) serv->private_info;

    ret = doConnection( & (server_aux->params) );
    if (ret < 0) 
    {
        fprintf(stderr, "ERROR: nfi_mq_server_rmdir: Connection failed\n");
        return -1;
    }

    debug_info("[NFI_MQ] nfi_mq_server_preload(ID=%s): preload %s in server %s.\n", server_aux->id, virtual_path, serv->server);

    /*****************************************/
    msg.type = MQ_SERVER_PRELOAD_FILE;
    memccpy(msg.id, server_aux->id, 0, MQ_SERVER_ID - 1);
    memccpy(msg.u_st_mq_server_msg.op_preload.virtual_path, virtual_path, 0, PATH_MAX - 1);
    memccpy(msg.u_st_mq_server_msg.op_preload.storage_path, storage_path, 0, PATH_MAX - 1);
    msg.u_st_mq_server_msg.op_preload.block_size = serv->block_size;
    msg.u_st_mq_server_msg.op_preload.opt = opt;

    nfi_mq_server_doRequest(server_aux, & msg, (char * ) & ret, sizeof(int));
    /*****************************************/

    debug_info("[NFI_MQ] nfi_mq_server_preload(ID=%s): end %s - %s = %d\n", server_aux->id, virtual_path, storage_path, ret);

    int ret2 = doDisconnection( & (server_aux->params) );
    if (ret2 < 0) 
    {
        fprintf(stderr, "ERROR: nfi_mq_server_rmdir: DisConnection failed\n");
        return -1;
    }

    DEBUG_END();

    return ret2;
}



int nfi_mq_server_flush(struct nfi_server * serv, char * url, char * virtual_path, char * storage_path, int opt) 
{
    //char dir[PATH_MAX];
    int ret;
    struct nfi_mq_server_server * server_aux;
    struct st_mq_server_msg msg;

    DEBUG_BEGIN();

    // Check arguments...
    NULL_RET_ERR(serv, MQ_SERVERERR_PARAM);
    NULL_RET_ERR(url, MQ_SERVERERR_PARAM);
    NULL_RET_ERR(virtual_path, MQ_SERVERERR_PARAM);
    NULL_RET_ERR(storage_path, MQ_SERVERERR_PARAM);
    nfi_mq_server_keepConnected(serv);
    NULL_RET_ERR(serv->private_info, MQ_SERVERERR_PARAM);

    // private_info...
    server_aux = (struct nfi_mq_server_server * ) serv->private_info;
    debug_info("[NFI_MQ] nfi_mq_server_flush(ID=%s): begin %s - %s \n", server_aux->id, virtual_path, storage_path);

    if (url[strlen(url) - 1] == '/') {
        mq_server_err(MQ_SERVERERR_PARAM);
        return -1;
    }

    server_aux = (struct nfi_mq_server_server * ) serv->private_info;
    debug_info("[NFI_MQ] nfi_mq_server_flush(ID=%s): open %s in server %s.\n", server_aux->id, virtual_path, serv->server);

    /*****************************************/
    //bzero(&msg, sizeof(struct st_mq_server_msg));

    msg.type = MQ_SERVER_FLUSH_FILE;
    memccpy(msg.id, server_aux->id, 0, MQ_SERVER_ID - 1);
    memccpy(msg.u_st_mq_server_msg.op_flush.virtual_path, virtual_path, 0, PATH_MAX - 1);
    memccpy(msg.u_st_mq_server_msg.op_flush.storage_path, storage_path, 0, PATH_MAX - 1);
    msg.u_st_mq_server_msg.op_flush.block_size = serv->block_size;
    msg.u_st_mq_server_msg.op_flush.opt = opt;

    nfi_mq_server_doRequest(server_aux, & msg, (char * ) & ret, sizeof(int));
    /*****************************************/

    int ret2 = doDisconnection( & (server_aux->params) );
    if (ret2 < 0) 
    {
        fprintf(stderr, "ERROR: nfi_mq_server_rmdir: DisConnection failed\n");
        return -1;
    }


    DEBUG_END();

    return 0;
}


int nfi_mq_server_read_mdata ( struct nfi_server *serv, char *url, struct xpn_metadata *mdata )
{
  int ret;
  char dir[PATH_MAX];
  struct nfi_mq_server *server_aux;
  struct st_mq_server_msg msg;
  struct st_mq_server_read_mdata_req req;

  debug_info("[SERV_ID=%d] [NFI_MQ] [nfi_mq_server_read_mdata] >> Begin\n", serv->id);

  // check arguments...
  NULL_RET_ERR(serv,               EINVAL);
  nfi_mq_server_keep_connected(serv);
  NULL_RET_ERR(serv->private_info, EINVAL);

  // private_info...
  debug_info("[SERV_ID=%d] [NFI_MQ] [nfi_mq_server_read_mdata] Get server private info\n", serv->id);

  server_aux = (struct nfi_mq_server  *) serv->private_info;
  if (server_aux == NULL)
  {
    errno = EINVAL;
    return -1;
  }

  // from url -> server + dir
  ret = ParseURL(url, NULL, NULL, NULL, NULL,  NULL,  dir);
  if (ret < 0)
  {
    printf("[SERV_ID=%d] [NFI_MQ] [nfi_mq_server_read_mdata] ERROR: incorrect url '%s'.\n", serv->id, url);
    errno = EINVAL;
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_MQ] [nfi_mq_server_read_mdata] ParseURL(%s)= %s\n", serv->id, url, dir);
  
  debug_info("[SERV_ID=%d] [NFI_MQ] [nfi_mq_server_read_mdata] nfi_mq_server_read_mdata(%s)\n", serv->id, dir);


  msg.type = XPN_SERVER_READ_MDATA;
  memccpy(msg.u_st_mq_server_msg.op_read_mdata.path, dir,            0, PATH_MAX-1);

  nfi_mq_server_do_request(server_aux, &msg, (char *)&req, sizeof(struct st_mq_server_read_mdata_req));

  if (req.status.ret < 0)
      errno = req.status.server_errno;
  debug_info("[SERV_ID=%d] [NFI_MQ] [nfi_mq_server_read_mdata] nfi_mq_server_read_mdata(%s)=%d\n", serv->id, dir, req.status);

  memcpy(mdata, &req.mdata, sizeof(struct xpn_metadata));

  debug_info("[NFI_MQ] [nfi_mq_server_read_mdata] >> End\n");

  return req.status.ret;
}

int nfi_mq_server_write_mdata ( struct nfi_server *serv, char *url, struct xpn_metadata *mdata, int only_file_size )
{
  int ret;
  char dir[PATH_MAX];
  struct nfi_mq_server *server_aux;
  struct st_mq_server_msg msg;
  struct st_mq_server_status req;

  debug_info("[SERV_ID=%d] [NFI_MQ] [nfi_mq_server_write_mdata] >> Begin\n", serv->id);

  // check arguments...
  NULL_RET_ERR(serv,               EINVAL);
  nfi_mq_server_keep_connected(serv);
  NULL_RET_ERR(serv->private_info, EINVAL);

  // private_info...
  debug_info("[SERV_ID=%d] [NFI_MQ] [nfi_mq_server_write_mdata] Get server private info\n", serv->id);

  server_aux = (struct nfi_mq_server  *) serv->private_info;
  if (server_aux == NULL)
  {
    errno = EINVAL;
    return -1;
  }

  // from url -> server + dir
  ret = ParseURL(url, NULL, NULL, NULL, NULL,  NULL,  dir);
  if (ret < 0)
  {
    printf("[SERV_ID=%d] [NFI_MQ] [nfi_mq_server_write_mdata] ERROR: incorrect url '%s'.\n", serv->id, url);
    errno = EINVAL;
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_MQ] [nfi_mq_server_write_mdata] ParseURL(%s)= %s\n", serv->id, url, dir);
  
  debug_info("[SERV_ID=%d] [NFI_MQ] [nfi_mq_server_write_mdata] nfi_mq_server_write_mdata(%s)\n", serv->id, dir);

  if (only_file_size){
    msg.type = XPN_SERVER_WRITE_MDATA_FILE_SIZE;
    memccpy(msg.u_st_mq_server_msg.op_write_mdata_file_size.path, dir,            0, PATH_MAX-1);
    msg.u_st_mq_server_msg.op_write_mdata_file_size.size = mdata->file_size;
  }else{
    msg.type = XPN_SERVER_WRITE_MDATA;
    memccpy(msg.u_st_mq_server_msg.op_write_mdata.path, dir,            0, PATH_MAX-1);
    memcpy(&msg.u_st_mq_server_msg.op_write_mdata.mdata, mdata, sizeof(struct xpn_metadata));
  }

  nfi_mq_server_do_request(server_aux, &msg, (char *)&req, sizeof(struct st_mq_server_status));

  if (req.ret < 0)
      errno = req.server_errno;
  debug_info("[SERV_ID=%d] [NFI_MQ] [nfi_mq_server_write_mdata] nfi_mq_server_write_mdata(%s)=%d\n", serv->id, dir, req.ret);

  debug_info("[NFI_MQ] [nfi_mq_server_write_mdata] >> End\n");

  return req.ret;
}


/* ................................................................... */

