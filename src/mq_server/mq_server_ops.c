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

#include "mq_server_ops.h"
#include <sys/time.h>

/* GLOBAL VARIABLES */

/* ... Functions / Funciones ......................................... */

double get_time_ops(void)
{
    struct timeval tp;
    struct timezone tzp;

    gettimeofday(&tp,&tzp);
    return((double) tp.tv_sec + .000001 * (double) tp.tv_usec);
}


char * mq_server_op2string(int op_code) {
    char * ret = "Unknown";

    switch (op_code) {
    case MQ_SERVER_OPEN_FILE_WS:
        ret = "OPEN";
        break;
    case MQ_SERVER_OPEN_FILE_WOS:
        ret = "OPEN_WOS";
        break;
    case MQ_SERVER_CREAT_FILE_WS:
        ret = "CREAT";
        break;
    case MQ_SERVER_CREAT_FILE_WOS:
        ret = "CREAT_WOS";
        break;
    case MQ_SERVER_READ_FILE_WS:
        ret = "READ";
        break;
    case MQ_SERVER_READ_FILE_WOS:
        ret = "READ_WOS";
        break;
    case MQ_SERVER_WRITE_FILE_WS:
        ret = "WRITE";
        break;
    case MQ_SERVER_WRITE_FILE_WOS:
        ret = "WRITE_WOS";
        break;
    case MQ_SERVER_CLOSE_FILE_WS:
        ret = "CLOSE";
        break;
    case MQ_SERVER_RM_FILE:
        ret = "RM";
        break;
    case MQ_SERVER_RENAME_FILE:
        ret = "RENAME";
        break;
    case MQ_SERVER_GETATTR_FILE:
        ret = "GETATTR";
        break;
    case MQ_SERVER_SETATTR_FILE:
        ret = "SETATTR";
        break;
    case MQ_SERVER_MKDIR_DIR:
        ret = "MKDIR";
        break;
    case MQ_SERVER_RMDIR_DIR:
        ret = "RMDIR";
        break;
    case MQ_SERVER_OPENDIR_DIR:
        ret = "OPENDIR";
        break;
    case MQ_SERVER_READDIR_DIR:
        ret = "READDIR";
        break;
    case MQ_SERVER_CLOSEDIR_DIR:
        ret = "CLOSEDIR";
        break;
    case MQ_SERVER_FLUSH_FILE:
        ret = "FLUSH";
        break;
    case MQ_SERVER_PRELOAD_FILE:
        ret = "PRELOAD";
        break;
    case MQ_SERVER_STATFS_DIR:
        ret = "STATFS";
        break;
    case MQ_SERVER_FINALIZE:
        ret = "FINALIZE";
        break;
    case MQ_SERVER_GETID:
        ret = "GETID";
        break;
    case MQ_SERVER_DISCONNECT:
        ret = "DISCONNECT";
        break;
    case MQ_SERVER_GETNODENAME:
        ret = "GETNODENAME";
        break;
    case MQ_SERVER_END:
        ret = "END";
        break;
    }

    return ret;
}

/*
 * OPERATIONAL FUNCTIONS
 */

void mq_server_op_open_ws(mq_server_param_st * params, int sd, struct st_mq_server_msg * head, int rank_client_id);
void mq_server_op_open_wos(mq_server_param_st * params, int sd, struct st_mq_server_msg * head, int rank_client_id);
void mq_server_op_creat_ws(mq_server_param_st * params, int sd, struct st_mq_server_msg * head, int rank_client_id);
void mq_server_op_creat_wos(mq_server_param_st * params, int sd, struct st_mq_server_msg * head, int rank_client_id);
void mq_server_op_read_ws(mq_server_param_st * params, int sd, struct st_mq_server_msg * head, int rank_client_id);
void mq_server_op_read_wos(mq_server_param_st * params, int sd, struct st_mq_server_msg * head, int rank_client_id);
void mq_server_op_write_ws(mq_server_param_st * params, int sd, struct st_mq_server_msg * head, int rank_client_id);
void mq_server_op_write_wos(mq_server_param_st * params, int sd, struct st_mq_server_msg * head, int rank_client_id);
void mq_server_op_close_ws(mq_server_param_st * params, int sd, struct st_mq_server_msg * head, int rank_client_id);

void mq_server_op_rm(mq_server_param_st * params, int sd, struct st_mq_server_msg * head, int rank_client_id);
void mq_server_op_rename(mq_server_param_st * params, int sd, struct st_mq_server_msg * head, int rank_client_id);
void mq_server_op_setattr(mq_server_param_st * params, int sd, struct st_mq_server_msg * head, int rank_client_id);
void mq_server_op_getattr(mq_server_param_st * params, int sd, struct st_mq_server_msg * head, int rank_client_id);

void mq_server_op_mkdir(mq_server_param_st * params, int sd, struct st_mq_server_msg * head, int rank_client_id);
void mq_server_op_opendir(mq_server_param_st * params, int sd, struct st_mq_server_msg * head, int rank_client_id);
void mq_server_op_readdir(mq_server_param_st * params, int sd, struct st_mq_server_msg * head, int rank_client_id);
void mq_server_op_closedir(mq_server_param_st * params, int sd, struct st_mq_server_msg * head, int rank_client_id);
void mq_server_op_rmdir(mq_server_param_st * params, int sd, struct st_mq_server_msg * head, int rank_client_id);

//void mq_server_op_flush(mq_server_param_st * params, int sd, struct st_mq_server_msg * head, int rank_client_id);
//void mq_server_op_preload(mq_server_param_st * params, int sd, struct st_mq_server_msg * head, int rank_client_id);

void mq_server_op_getnodename(mq_server_param_st * params, int sd, struct st_mq_server_msg * head, int rank_client_id); //NEW
void mq_server_op_fstat(mq_server_param_st * params, int sd, struct st_mq_server_msg * head, int rank_client_id); //TODO: implement
void mq_server_op_getid(mq_server_param_st * params, int sd, struct st_mq_server_msg * head, int rank_client_id); //TODO: call in switch

// Metadata
void mq_server_op_read_mdata   ( mq_server_param_st *params, void *comm, struct st_mq_server_msg *head, int rank_client_id, int tag_client_id );
void mq_server_op_write_mdata  ( mq_server_param_st *params, void *comm, struct st_mq_server_msg *head, int rank_client_id, int tag_client_id );
void mq_server_op_write_mdata_file_size  ( mq_server_param_st *params, void *comm, struct st_mq_server_msg *head, int rank_client_id, int tag_client_id );


/**********************************
Read the operation to realize
***********************************/
int mq_server_do_operation(struct st_th * th, int * the_end)
{
    DEBUG_BEGIN();

    int ret;
    struct st_mq_server_msg head;

    //printf("SERVER DO OPERATION -- %d\n", th -> type_op);

    switch (th -> type_op)
    {
		//File API
	    case MQ_SERVER_OPEN_FILE_WS:
		ret = mq_server_comm_read_data(th -> params, (int) th -> sd, (char * ) & (head.u_st_mq_server_msg.op_open), sizeof(struct st_mq_server_open), 0 /*head.id*/);
		if (ret != -1) {
		    mq_server_op_open_ws(th -> params, (int) th -> sd, & head, 0 /*head.id*/);
		}
		break;
	    case MQ_SERVER_OPEN_FILE_WOS:
		ret = mq_server_comm_read_data(th -> params, (int) th -> sd, (char * ) & (head.u_st_mq_server_msg.op_open), sizeof(struct st_mq_server_open), 0 /*head.id*/);
		if (ret != -1) {
		    mq_server_op_open_wos(th -> params, (int) th -> sd, & head, 0 /*head.id*/);
		}
		break;
	    case MQ_SERVER_CREAT_FILE_WS:
		ret = mq_server_comm_read_data(th -> params, (int) th -> sd, (char * ) & (head.u_st_mq_server_msg.op_creat), sizeof(struct st_mq_server_creat), 0 /*head.id*/);
		if (ret != -1) {
		    mq_server_op_creat_ws(th -> params, (int) th -> sd, & head, 0 /*head.id*/);
		}
		break;
	    case MQ_SERVER_CREAT_FILE_WOS:
		ret = mq_server_comm_read_data(th -> params, (int) th -> sd, (char * ) & (head.u_st_mq_server_msg.op_creat), sizeof(struct st_mq_server_creat), 0 /*head.id*/);
		if (ret != -1) {
		    mq_server_op_creat_wos(th -> params, (int) th -> sd, & head, 0 /*head.id*/);
		}
		break;
	    case MQ_SERVER_READ_FILE_WS:
		ret = mq_server_comm_read_data(th -> params, (int) th -> sd, (char * ) & (head.u_st_mq_server_msg.op_read), sizeof(struct st_mq_server_read), 0 /*head.id*/);
		if (ret != -1) {
		    mq_server_op_read_ws(th -> params, (int) th -> sd, & head, 0 /*head.id*/);
		}
		break;
	    case MQ_SERVER_READ_FILE_WOS:
		ret = mq_server_comm_read_data(th -> params, (int) th -> sd, (char * ) & (head.u_st_mq_server_msg.op_read), sizeof(struct st_mq_server_read), 0 /*head.id*/);
		if (ret != -1) {
		    mq_server_op_read_wos(th -> params, (int) th -> sd, & head, 0 /*head.id*/);
		}
		break;
	    case MQ_SERVER_WRITE_FILE_WS:
		ret = mq_server_comm_read_data(th -> params, (int) th -> sd, (char * ) & (head.u_st_mq_server_msg.op_write), sizeof(struct st_mq_server_write), 0 /*head.id*/);
		if (ret != -1) {
		    mq_server_op_write_ws(th -> params, (int) th -> sd, & head, 0 /*head.id*/);
		}
		break;
	    case MQ_SERVER_WRITE_FILE_WOS:
		ret = mq_server_comm_read_data(th -> params, (int) th -> sd, (char * ) & (head.u_st_mq_server_msg.op_write), sizeof(struct st_mq_server_write), 0 /*head.id*/);
		if (ret != -1) {
		    mq_server_op_write_wos(th -> params, (int) th -> sd, & head, 0 /*head.id*/);
		}
		break;
	    case MQ_SERVER_CLOSE_FILE_WS:
		ret = mq_server_comm_read_data(th -> params, (int) th -> sd, (char * ) & (head.u_st_mq_server_msg.op_close), sizeof(struct st_mq_server_close), 0 /*head.id*/);
		if (ret != -1) {
		    mq_server_op_close_ws(th -> params, (int) th -> sd, & head, 0 /*head.id*/);
		}
		break;

		// Metadata API
	    case MQ_SERVER_RM_FILE:
		ret = mq_server_comm_read_data(th -> params, (int) th -> sd, (char * ) & (head.u_st_mq_server_msg.op_rm), sizeof(struct st_mq_server_rm), 0 /*head.id*/);
		if (ret != -1) {
		    mq_server_op_rm(th -> params, (int) th -> sd, & head, 0 /*head.id*/);
		}
		break;
	    case MQ_SERVER_RENAME_FILE:
		ret = mq_server_comm_read_data(th -> params, (int) th -> sd, (char * ) & (head.u_st_mq_server_msg.op_rename), sizeof(struct st_mq_server_rename), 0 /*head.id*/);
		if (ret != -1) {
		    mq_server_op_rename(th -> params, (int) th -> sd, & head, 0 /*head.id*/);
		}
		break;
	    case MQ_SERVER_GETATTR_FILE:
		ret = mq_server_comm_read_data(th -> params, (int) th -> sd, (char * ) & (head.u_st_mq_server_msg.op_getattr), sizeof(struct st_mq_server_getattr), 0 /*head.id*/);
		if (ret != -1) {
		    mq_server_op_getattr(th -> params, (int) th -> sd, & head, 0 /*head.id*/);
		}
		break;
	    case MQ_SERVER_SETATTR_FILE:
		ret = mq_server_comm_read_data(th -> params, (int) th -> sd, (char * ) & (head.u_st_mq_server_msg.op_setattr), sizeof(struct st_mq_server_setattr), 0 /*head.id*/);
		if (ret != -1) {
		    mq_server_op_setattr(th -> params, (int) th -> sd, & head, 0 /*head.id*/);
		}
		break;

		//Directory API
	    case MQ_SERVER_MKDIR_DIR:
		ret = mq_server_comm_read_data(th -> params, (int) th -> sd, (char * ) & (head.u_st_mq_server_msg.op_mkdir), sizeof(struct st_mq_server_mkdir), 0 /*head.id*/);
		if (ret != -1) {
		    mq_server_op_mkdir(th -> params, (int) th -> sd, & head, 0 /*head.id*/);
		}
		break;
	    case MQ_SERVER_OPENDIR_DIR:
		ret = mq_server_comm_read_data(th -> params, (int) th -> sd, (char * ) & (head.u_st_mq_server_msg.op_opendir), sizeof(struct st_mq_server_opendir), 0 /*head.id*/);
		if (ret != -1) {
		    mq_server_op_opendir(th -> params, (int) th -> sd, & head, 0 /*head.id*/);
		}
		break;
	    case MQ_SERVER_READDIR_DIR:
		ret = mq_server_comm_read_data(th -> params, (int) th -> sd, (char * ) & (head.u_st_mq_server_msg.op_readdir), sizeof(struct st_mq_server_readdir), 0 /*head.id*/);
		if (ret != -1) {
		    mq_server_op_readdir(th -> params, (int) th -> sd, & head, 0 /*head.id*/);
		}
		break;
	    case MQ_SERVER_CLOSEDIR_DIR:
		ret = mq_server_comm_read_data(th -> params, (int) th -> sd, (char * ) & (head.u_st_mq_server_msg.op_closedir), sizeof(struct st_mq_server_closedir), 0 /*head.id*/);
		if (ret != -1) {
		    mq_server_op_closedir(th -> params, (int) th -> sd, & head, 0 /*head.id*/);
		}
		break;
	    case MQ_SERVER_RMDIR_DIR:
		ret = mq_server_comm_read_data(th -> params, (int) th -> sd, (char * ) & (head.u_st_mq_server_msg.op_rmdir), sizeof(struct st_mq_server_rmdir), 0 /*head.id*/);
		if (ret != -1) {
		    mq_server_op_rmdir(th -> params, (int) th -> sd, & head, 0 /*head.id*/);
		}
		break;

/*
		//File system API
	    case MQ_SERVER_PRELOAD_FILE:
		ret = mq_server_comm_read_data(th -> params, (int) th -> sd, (char * ) & (head.u_st_mq_server_msg.op_preload), sizeof(struct st_mq_server_preload), 0);
		if (ret != -1) {
		    mq_server_op_preload(th -> params, (int) th -> sd, & head, 0);
		}
		break;
	    case MQ_SERVER_FLUSH_FILE:
		ret = mq_server_comm_read_data(th -> params, (int) th -> sd, (char * ) & (head.u_st_mq_server_msg.op_flush), sizeof(struct st_mq_server_flush), 0 );
		if (ret != -1) {
		    mq_server_op_flush(th -> params, (int) th -> sd, & head, 0);
		}
		break;
*/

	    // Metadata
	    case MQ_SERVER_READ_MDATA:
	         ret = mq_server_comm_read_data(th->params, th->comm, (char *)&(head.u_st_mq_server_msg.op_read_mdata), sizeof(head.u_st_mq_server_msg.op_read_mdata), th->rank_client_id, th->tag_client_id);
	         if (ret != -1) {
		   mq_server_op_read_mdata(th->params, th->comm, &head, th->rank_client_id, th->tag_client_id);
	         }
	         break;
	    case MQ_SERVER_WRITE_MDATA:
	         ret = mq_server_comm_read_data(th->params, th->comm, (char *)&(head.u_st_mq_server_msg.op_write_mdata), sizeof(head.u_st_mq_server_msg.op_write_mdata), th->rank_client_id, th->tag_client_id);
	         if (ret != -1) {
		   mq_server_op_write_mdata(th->params, th->comm, &head, th->rank_client_id, th->tag_client_id);
	         }
	         break;
	    case MQ_SERVER_WRITE_MDATA_FILE_SIZE:
	         ret = mq_server_comm_read_data(th->params, th->comm, (char *)&(head.u_st_mq_server_msg.op_write_mdata_file_size), sizeof(head.u_st_mq_server_msg.op_write_mdata_file_size), th->rank_client_id, th->tag_client_id);
	         if (ret != -1) {
		   mq_server_op_write_mdata_file_size(th->params, th->comm, &head, th->rank_client_id, th->tag_client_id);
	         }
	         break;

		//Connection API
	    case MQ_SERVER_DISCONNECT:
		break;

	    case MQ_SERVER_FINALIZE:
		*
		the_end = 1;
		break;

		//FS Metadata API
	    case MQ_SERVER_GETNODENAME:
		mq_server_op_getnodename(th -> params, (int) th -> sd, & head, 0 /*head.id*/); //NEW
		break;

    }

    DEBUG_END();

    return 0;
}

//
// File API
//





void mq_server_op_open_ws(mq_server_param_st * params, int sd, struct st_mq_server_msg * head, int rank_client_id) // WS - With Session
{
    int fd;
    char *s;
    char *extra = "/#";
    char *sm = malloc(strlen(head -> u_st_mq_server_msg.op_open.path) + strlen(extra) + 1);
    strcpy(sm, head -> u_st_mq_server_msg.op_open.path);
    strcat(sm, extra);

    /*
     *      MOSQUITTO OPEN FILE
     */
    
    if ( params -> mosquitto_mode == 1 )
    {
        #ifdef HAVE_MOSQUITTO_H
        debug_info("[%d]\tBEGIN OPEN MOSQUITTO MQ_SERVER WS - %s\n", __LINE__, sm);

        int rc = mosquitto_subscribe(params -> mqtt, NULL, sm, params -> mosquitto_qos);
        if(rc != MOSQ_ERR_SUCCESS)
        {
            fprintf(stderr, "Error subscribing open: %s\n", mosquitto_strerror(rc));
            mosquitto_disconnect(params -> mqtt);
        }

        debug_info("[%d]\tEND OPEN MOSQUITTO MQ_SERVER WS - %s\n\n", __LINE__, sm);
        #endif
    }

    s = head -> u_st_mq_server_msg.op_open.path;

    // do open
    fd = filesystem_open(s, O_RDWR);

    mq_server_comm_write_data(params, sd, (char * ) & fd, sizeof(int), rank_client_id);

    // show debug info
    debug_info("[%d][MQ-SERVER-OPS] (ID=%s) OPEN(%s)=%d\n", __LINE__, params -> srv_name, s, fd);
}



void mq_server_op_open_wos(mq_server_param_st * params, int sd, struct st_mq_server_msg * head, int rank_client_id) //WOS - Without Session
{
    int fd;
    char *s;
    char *extra = "/#";
    char *sm = malloc(strlen(head -> u_st_mq_server_msg.op_open.path) + strlen(extra) + 1);
    strcpy(sm, head -> u_st_mq_server_msg.op_open.path);
    strcat(sm, extra);

    s = head -> u_st_mq_server_msg.op_open.path;

    /*
     *      MOSQUITTO OPEN FILE
     */
    
    if ( params -> mosquitto_mode == 1 )
    {
        #ifdef HAVE_MOSQUITTO_H
        debug_info("[%d]\tBEGIN OPEN MOSQUITTO MQ_SERVER WOS - %s\n", __LINE__, sm);

        int rc = mosquitto_subscribe(params -> mqtt, NULL, sm, params -> mosquitto_qos);
        if(rc != MOSQ_ERR_SUCCESS)
        {
            fprintf(stderr, "Error subscribing open: %s\n", mosquitto_strerror(rc));
            mosquitto_disconnect(params -> mqtt);
        }

        debug_info("[%d]\tEND OPEN MOSQUITTO MQ_SERVER WOS - %s\n\n", __LINE__, sm);
/**
        printf("[%d]\tBEGIN CLOSE OPEN MOSQUITTO MQ_SERVER - WOS \n\n", __LINE__);

        mosquitto_unsubscribe(params -> mqtt, NULL, sm);
        mosquitto_unsubscribe(params -> mqtt, NULL, s);

        printf("[%d]\tEND CLOSE OPEN MOSQUITTO MQ_SERVER - WOS %s\n\n", __LINE__, s);
        **/
        #endif
    }
    
    // do open
    fd = filesystem_open(s, O_RDWR);

    mq_server_comm_write_data(params, sd, (char * ) & fd, sizeof(int), rank_client_id);

    filesystem_close(fd);
    

    // show debug info
    debug_info("[%d][MQ-SERVER-OPS] (ID=%s) OPEN(%s)=%d\n", __LINE__, params -> srv_name, s, fd);

}



void mq_server_op_creat_ws(mq_server_param_st * params, int sd, struct st_mq_server_msg * head, int rank_client_id)
{
    int fd, rc;
    char *s;
    char *extra = "/#";
    char *sm = malloc(strlen(head -> u_st_mq_server_msg.op_creat.path) + strlen(extra) + 1);
    strcpy(sm, head -> u_st_mq_server_msg.op_creat.path);
    strcat(sm, extra);

    if ( params -> mosquitto_mode == 1 )
    {
        #ifdef HAVE_MOSQUITTO_H
        debug_info("[%d]\tBEGIN CREAT MOSQUITTO MQ_SERVER WS - %s\n", __LINE__, sm);

        rc = mosquitto_subscribe(params -> mqtt, NULL, sm, params -> mosquitto_qos);

        if(rc != MOSQ_ERR_SUCCESS)
        {
            fprintf(stderr, "Error subscribing open: %s\n", mosquitto_strerror(rc));
            mosquitto_disconnect(params -> mqtt);
        }

        debug_info("[%d]\tEND CREAT MOSQUITTO MQ_SERVER WS - %s\n\n", __LINE__, sm);
        #endif
    }

    s = head -> u_st_mq_server_msg.op_creat.path;

    // do creat
    fd = filesystem_creat(s, 0770); // TODO: mq_server_op_creat don't use 'mode' from client ?
    if (fd < 0) {
        filesystem_mkpath(s);
        fd = filesystem_creat(s, 0770);
    }

    mq_server_comm_write_data(params, sd, (char * ) & fd, sizeof(int), rank_client_id);

    // show debug info
    debug_info("[%d][MQ-SERVER-OPS] (ID=%s) CREAT(%s)=%d\n", __LINE__, params -> srv_name, s, fd);
}



void mq_server_op_creat_wos(mq_server_param_st * params, int sd, struct st_mq_server_msg * head, int rank_client_id)
{
    int fd;
    char *extra = "/#";
    char *s;
    char *sm = malloc(strlen(head -> u_st_mq_server_msg.op_creat.path) + strlen(extra) + 1);
    strcpy(sm, head -> u_st_mq_server_msg.op_creat.path);
    strcat(sm, extra);

    s = head -> u_st_mq_server_msg.op_creat.path;
    //printf("MQ_SERVER_OPS PREOK - %s\n", s);

    if ( params -> mosquitto_mode == 1 )
    {
        #ifdef HAVE_MOSQUITTO_H
        debug_info("[%d]\tBEGIN CREATE MOSQUITTO MQ_SERVER WOS - %s\n", __LINE__, sm);

        int rc = mosquitto_subscribe(params -> mqtt, NULL, sm, params -> mosquitto_qos);

        if(rc != MOSQ_ERR_SUCCESS)
        {
            fprintf(stderr, "Error subscribing creat: %s\n", mosquitto_strerror(rc));
            mosquitto_disconnect(params -> mqtt);
        }

        debug_info("[%d]\tEND CREATE MOSQUITTO MQ_SERVER WOS - %s\n\n", __LINE__, sm);
  /**      
        printf("[%d]\tBEGIN CLOSE CREAT MOSQUITTO MQ_SERVER - WOS \n\n", __LINE__);

        mosquitto_unsubscribe(params -> mqtt, NULL, sm);
        mosquitto_unsubscribe(params -> mqtt, NULL, s);

        printf("[%d]\tEND CLOSE CREAT MOSQUITTO MQ_SERVER - WOS %s \n\n", __LINE__, s);
        **/
        #endif
    }
    int retries = 0;

    do
    {

        // do creat
	fd = filesystem_creat(s, 0770); // TODO: mq_server_op_creat don't use 'mode' from client ?
	//fd = filesystem_open(s, O_CREAT|O_APPEND);
        if (fd < 0) 
        {
            filesystem_mkpath(s);
            fd = filesystem_creat(s, 0770);
		//fd = filesystem_open(s, O_CREAT|O_APPEND);
            //printf("MQ_SERVER_OPS CREATE NOTOK - %s - %d\n", s, fd);
            retries++;
        }
        else
        {
            //printf("MQ_SERVER_OPS CREATEOK - %s - %d\n", s, fd);
            break;
        }

    }while((fd < 0) && (retries < 5));

    
    
    
    //printf("[%d][MQ-SERVER-OPS] (ID=%s) CREAT(%s)=%d\n", __LINE__, params -> srv_name, s, fd);
    mq_server_comm_write_data(params, sd, (char * ) & fd, sizeof(int), rank_client_id);


    filesystem_close(fd);

    // show debug info
    debug_info("[%d][MQ-SERVER-OPS] (ID=%s) CREAT(%s)=%d\n", __LINE__, params -> srv_name, s, fd);
}



void mq_server_op_read_ws(mq_server_param_st * params, int sd, struct st_mq_server_msg * head, int rank_client_id)
{
    struct st_mq_server_read_req req;
    char * buffer;
    long size, diff, to_read, cont;

    debug_info("[MQ-SERVER-OPS] (ID=%s) begin read: fd %d offset %d size %d ID=x\n",
        params -> srv_name, head -> u_st_mq_server_msg.op_read.fd, (int) head -> u_st_mq_server_msg.op_read.offset, head -> u_st_mq_server_msg.op_read.size);

    // initialize counters
    cont = 0;
    size = head -> u_st_mq_server_msg.op_read.size;

    if (size > MAX_BUFFER_SIZE) {
        size = MAX_BUFFER_SIZE;
    }

    diff = head -> u_st_mq_server_msg.op_read.size - cont;

    // malloc a buffer of size...
    buffer = (char * ) malloc(size);
    if (NULL == buffer) {
        req.size = -1; // TODO: check in client that -1 is treated properly... :-9
        mq_server_comm_write_data(params, sd, (char * ) & req, sizeof(struct st_mq_server_write_req), rank_client_id);
        return;
    }

    // loop...
    do {
        if (diff > size)
            to_read = size;
        else to_read = diff;

        // lseek and read data...
        filesystem_lseek(head -> u_st_mq_server_msg.op_read.fd, head -> u_st_mq_server_msg.op_read.offset + cont, SEEK_SET);
        req.size = filesystem_read(head -> u_st_mq_server_msg.op_read.fd, buffer, to_read);
        // if error then send as "how many bytes" -1
        if (req.size < 0) {
            req.size = -1; // TODO: check in client that -1 is treated properly... :-)
            mq_server_comm_write_data(params, sd, (char * ) & req, sizeof(struct st_mq_server_write_req), rank_client_id);

            FREE_AND_NULL(buffer);
            return;
        }
        // send (how many + data) to client...
        mq_server_comm_write_data(params, sd, (char * ) & req, sizeof(struct st_mq_server_read_req), rank_client_id);
        debug_info("[MQ-SERVER-OPS] (ID=%s) op_read: send size %d\n", params -> srv_name, req.size);

        // send data to client...
        if (req.size > 0) {
            mq_server_comm_write_data(params, sd, buffer, req.size, rank_client_id);
            debug_info("[MQ-SERVER-OPS] (ID=%s) op_read: send data\n", params -> srv_name);
        }
        cont = cont + req.size; //Send bytes
        diff = head -> u_st_mq_server_msg.op_read.size - cont;

    } while ((diff > 0) && (req.size != 0));

    // free buffer
    FREE_AND_NULL(buffer);

    // debugging information
    debug_info("[MQ-SERVER-OPS] (ID=%s) end READ: fd %d offset %d size %d ID=x\n",
        params -> srv_name, head -> u_st_mq_server_msg.op_read.fd, (int) head -> u_st_mq_server_msg.op_read.offset, size);
}



void mq_server_op_read_wos(mq_server_param_st * params, int sd, struct st_mq_server_msg * head, int rank_client_id) {
    struct st_mq_server_read_req req;
    char * buffer;
    long size, diff, to_read, cont;

    debug_info("[MQ-SERVER-OPS] (ID=%s) begin read: path %s offset %d size %d ID=x\n",
        params -> srv_name, head -> u_st_mq_server_msg.op_read.path, (int) head -> u_st_mq_server_msg.op_read.offset, head -> u_st_mq_server_msg.op_read.size);

    // initialize counters
    cont = 0;
    size = head -> u_st_mq_server_msg.op_read.size;
    if (size > MAX_BUFFER_SIZE) {
        size = MAX_BUFFER_SIZE;
    }

    diff = head -> u_st_mq_server_msg.op_read.size - cont;

    //Open file
    int fd = filesystem_open(head -> u_st_mq_server_msg.op_read.path, O_RDONLY);
    if (fd < 0) {
        req.size = -1; // TODO: check in client that -1 is treated properly... :-9
        mq_server_comm_write_data(params, sd, (char * ) & req, sizeof(struct st_mq_server_write_req), rank_client_id);
        return;
    }

    // malloc a buffer of size...
    buffer = (char * ) malloc(size);
    if (NULL == buffer) {
        req.size = -1; // TODO: check in client that -1 is treated properly... :-9
        mq_server_comm_write_data(params, sd, (char * ) & req, sizeof(struct st_mq_server_write_req), rank_client_id);
        filesystem_close(fd);
        return;
    }

    // loop...
    do {
        if (diff > size)
            to_read = size;
        else to_read = diff;

        // lseek and read data...
        filesystem_lseek(fd, head -> u_st_mq_server_msg.op_read.offset + cont, SEEK_SET);
        req.size = filesystem_read(fd, buffer, to_read);
        // if error then send as "how many bytes" -1

        if (req.size < 0) {
            mq_server_comm_write_data(params, sd, (char * ) & req, sizeof(struct st_mq_server_write_req), rank_client_id);
            filesystem_close(fd);
            FREE_AND_NULL(buffer);
            return;
        }
        // send (how many + data) to client...
        mq_server_comm_write_data(params, sd, (char * ) & req, sizeof(struct st_mq_server_read_req), rank_client_id);
        debug_info("[MQ-SERVER-OPS] (ID=%s) op_read: send size %d\n", params -> srv_name, req.size);

        // send data to client...
        if (req.size > 0) {
            mq_server_comm_write_data(params, sd, buffer, req.size, rank_client_id);
            debug_info("[MQ-SERVER-OPS] (ID=%s) op_read: send data\n", params -> srv_name);
        }

        cont = cont + req.size; //Send bytes
        diff = head -> u_st_mq_server_msg.op_read.size - cont;

    } while ((diff > 0) && (req.size != 0));

    filesystem_close(fd);

    // free buffer
    FREE_AND_NULL(buffer);

    // debugging information
    debug_info("[MQ-SERVER-OPS] (ID=%s) end READ: path %s offset %d size %d ID=x\n",
        params -> srv_name, head -> u_st_mq_server_msg.op_read.path, (int) head -> u_st_mq_server_msg.op_read.offset, size);
}



void mq_server_op_write_ws(mq_server_param_st * params, int sd, struct st_mq_server_msg * head, int rank_client_id)
{
    if( params -> mosquitto_mode == 0 )
    {
        debug_info("[MQ-SERVER-OPS] (ID=%s) begin write: fd %d ID=xn", params -> srv_name, head -> u_st_mq_server_msg.op_write.fd);
    
        struct st_mq_server_write_req req;
        char * buffer;
        int size, diff, cont, to_write;

        debug_info("[MQ-SERVER-OPS] (ID=%s) begin write: fd %d ID=xn", params -> srv_name, head -> u_st_mq_server_msg.op_write.fd);

        // initialize counters
        cont = 0;
        size = (head -> u_st_mq_server_msg.op_write.size);
        if (size > MAX_BUFFER_SIZE) {
            size = MAX_BUFFER_SIZE;
        }
        diff = head -> u_st_mq_server_msg.op_read.size - cont;

        // malloc a buffer of size...
        buffer = (char * ) malloc(size);
        if (NULL == buffer) {
            req.size = -1; // TODO: check in client that -1 is treated properly... :-)
            mq_server_comm_write_data(params, sd, (char * ) & req, sizeof(struct st_mq_server_write_req), rank_client_id);
            return;
        }

        // loop...
        do {
            if (diff > size) to_write = size;
            else to_write = diff;

            // read data from TCP and write into the file
            mq_server_comm_read_data(params, sd, buffer, to_write, rank_client_id);
            filesystem_lseek(head -> u_st_mq_server_msg.op_write.fd, head -> u_st_mq_server_msg.op_write.offset + cont, SEEK_SET);
            //sem_wait(&disk_sem);
            req.size = filesystem_write(head -> u_st_mq_server_msg.op_write.fd, buffer, to_write);
            //sem_post(&disk_sem);

            // update counters
            cont = cont + req.size; // Received bytes
            diff = head -> u_st_mq_server_msg.op_read.size - cont;

        } while ((diff > 0) && (req.size != 0));

        // write to the client the status of the write operation
        req.size = cont;
        mq_server_comm_write_data(params, sd, (char * ) & req, sizeof(struct st_mq_server_write_req), rank_client_id);

        // free buffer
        FREE_AND_NULL(buffer);
    }

    // for debugging purpouses
    debug_info("[MQ-SERVER-OPS] (ID=%s) end write: fd %d ID=xn", params -> srv_name, head -> u_st_mq_server_msg.op_write.fd);
}



void mq_server_op_write_wos(mq_server_param_st * params, int sd, struct st_mq_server_msg * head, int rank_client_id)
{
    if( params -> mosquitto_mode == 0 )
    {
        struct st_mq_server_write_req req;
        char * buffer;
        int size, diff, cont, to_write;

        debug_info("[MQ-SERVER-OPS] (ID=%s) begin write: path %s ID=xn", params -> srv_name, head -> u_st_mq_server_msg.op_write.path);

        // initialize counters
        cont = 0;
        size = (head -> u_st_mq_server_msg.op_write.size);
        if (size > MAX_BUFFER_SIZE) {
            size = MAX_BUFFER_SIZE;
        }
        diff = head -> u_st_mq_server_msg.op_read.size - cont;

        double start_time = 0.0, total_time = 0.0;
        start_time = get_time_ops();

        //Open file
        int fd = filesystem_open(head -> u_st_mq_server_msg.op_write.path, O_WRONLY);
        //printf("MQ_SERVER_OPS WRITE - %s - %d\n", head -> u_st_mq_server_msg.op_write.path, fd);
        if (fd < 0)
        {
        	printf("MQ_SERVER_OPS WRITE - %s - %d\n", head -> u_st_mq_server_msg.op_write.path, fd);
            req.size = -1; // TODO: check in client that -1 is treated properly... :-)
            mq_server_comm_write_data(params, sd, (char * ) & req, sizeof(struct st_mq_server_write_req), rank_client_id);
            return;
        }

        // malloc a buffer of size...
        buffer = (char * ) malloc(size);
        if (NULL == buffer) {
            req.size = -1; // TODO: check in client that -1 is treated properly... :-)
            mq_server_comm_write_data(params, sd, (char * ) & req, sizeof(struct st_mq_server_write_req), rank_client_id);
            filesystem_close(fd);
            return;
        }

        //debug_info("\npath = %s | to_write = %d | offset = %ld\n", head -> u_st_mq_server_msg.op_write.path, size, head -> u_st_mq_server_msg.op_write.offset);

        // loop...
        do {
            if (diff > size) to_write = size;
            else to_write = diff;

            // read data from TCP and write into the file
            mq_server_comm_read_data(params, sd, buffer, to_write, rank_client_id);
            filesystem_lseek(fd, head -> u_st_mq_server_msg.op_write.offset + cont, SEEK_SET);
            //sem_wait(&disk_sem);

            /********WRITES TIMES*********/
            /*char copy_header[20];
		    strncpy(copy_header, buffer, 20);

		    if ((strstr(copy_header, "FIN") != NULL))
		    {
		        struct timeval current_time;
		        gettimeofday(&current_time, NULL);
		        time_t now = current_time.tv_sec;
		        struct tm *timeinfo;
		        timeinfo = localtime(&now);

		        char time_str[20];
		        strftime(time_str, sizeof(time_str), "%H:%M:%S", timeinfo);
		        //int retw = write(file2, end_time, strlen(end_time));
		        printf("ENDW - %s\n", time_str);
		        //if (retw < 0) printf("ERROR Write Dispatcher\n");

		//    close(file2);
		    }
		    else if ((strstr(copy_header, "INI") != NULL))
		    {
		        struct timeval current_time;
		        gettimeofday(&current_time, NULL);
		        time_t now = current_time.tv_sec;
		        struct tm *timeinfo;
		        timeinfo = localtime(&now);

		        char time_str[20];
		        strftime(time_str, sizeof(time_str), "%H:%M:%S", timeinfo);
		        //int retw = write(file2, end_time, strlen(end_time));
		        printf("STARTW - %s\n", time_str);
		    }*/
            req.size = filesystem_write(fd, buffer, to_write);

            //sem_post(&disk_sem);

            // update counters
            cont = cont + req.size; // Received bytes
            diff = head -> u_st_mq_server_msg.op_read.size - cont;

        } while ((diff > 0) && (req.size != 0));

        // write to the client the status of the write operation
        req.size = cont;
        mq_server_comm_write_data(params, sd, (char * ) & req, sizeof(struct st_mq_server_write_req), rank_client_id);

        filesystem_close(fd);

        total_time = (get_time_ops() - start_time);
        printf("%s;%.8f\n", head -> u_st_mq_server_msg.op_write.path, total_time);
        FREE_AND_NULL(buffer);
    }

    // for debugging purpouses
    debug_info("[MQ-SERVER-OPS] (ID=%s) end write: fd %d ID=xn", params -> srv_name, head -> u_st_mq_server_msg.op_write.fd);
}



void mq_server_op_close_ws(mq_server_param_st * params, int sd, struct st_mq_server_msg * head, int rank_client_id)
{
    int ret = -1;

    // check params...
    if (NULL == params)
    {
        return;
    }

    char *extra = "/#";
    char *s;
    char *sm = malloc(strlen(head -> u_st_mq_server_msg.op_close.path) + strlen(extra) + 1);
    strcpy(sm, head -> u_st_mq_server_msg.op_close.path);
    strcat(sm, extra);

    s = head -> u_st_mq_server_msg.op_close.path;

    if( params -> mosquitto_mode == 1 )
    {
        #ifdef HAVE_MOSQUITTO_H
        debug_info("[%d]\tBEGIN CLOSE MOSQUITTO MQ_SERVER - WS \n\n", __LINE__);

        mosquitto_unsubscribe(params -> mqtt, NULL, sm);
        mosquitto_unsubscribe(params -> mqtt, NULL, s);

        debug_info("[%d]\tEND CLOSE MOSQUITTO MQ_SERVER - WS %s\n\n", __LINE__, sm);
        #endif
    }
    
    // do close
    if (head -> u_st_mq_server_msg.op_close.fd != -1 &&  params -> mosquitto_mode == 0 )
    {
        ret = filesystem_close(head -> u_st_mq_server_msg.op_close.fd);
    }

    mq_server_comm_write_data(params, sd, (char * ) & ret, sizeof(int), rank_client_id);

    // show debug info
    debug_info("[MQ-SERVER-OPS] (ID=%s) CLOSE(fd=%d, path=%s)\n", params -> srv_name, head -> u_st_mq_server_msg.op_close.fd, head -> u_st_mq_server_msg.op_close.path);
}



void mq_server_op_rm(mq_server_param_st * params, int sd, struct st_mq_server_msg * head, int rank_client_id)
{
    char * s;

    // check params...
    if (NULL == params)
    {
        return;
    }

    // do rm
    s = head -> u_st_mq_server_msg.op_rm.path;
    int ret = filesystem_unlink(s);

    mq_server_comm_write_data(params, sd, (char * ) & ret, sizeof(int), rank_client_id);

    // show debug info
    debug_info("[MQ-SERVER-OPS] (ID=%s) RM(path=%s)\n", params -> srv_name, head -> u_st_mq_server_msg.op_rm.path);
}



void mq_server_op_rename(mq_server_param_st * params, int sd, struct st_mq_server_msg * head, int rank_client_id)
{
    char * old_url;
    char * new_url;

    // check params...
    if (NULL == params)
    {
        return;
    }

    // do rename
    old_url = head -> u_st_mq_server_msg.op_rename.old_url;
    new_url = head -> u_st_mq_server_msg.op_rename.new_url;

    int ret = filesystem_rename(old_url, new_url);

    mq_server_comm_write_data(params, sd, (char * ) & ret, sizeof(int), rank_client_id);

    // show debug info
    debug_info("[MQ-SERVER-OPS] (ID=%s) RM(path=%s)\n", params -> srv_name, head -> u_st_mq_server_msg.op_rm.path);
}



void mq_server_op_getattr(mq_server_param_st * params, int sd, struct st_mq_server_msg * head, int rank_client_id)
{
    struct st_mq_server_attr_req req;
    char * s;

    // do getattr
    s = head -> u_st_mq_server_msg.op_getattr.path;
    req.status = filesystem_stat(s, & req.attr);
    mq_server_comm_write_data(params, sd, (char * ) & req, sizeof(struct st_mq_server_attr_req), rank_client_id);

    // show debug info
    debug_info("[MQ-SERVER-OPS] (ID=%s) GETATTR(%s)\n", params -> srv_name, head -> u_st_mq_server_msg.op_getattr.path);
}



void mq_server_op_setattr(mq_server_param_st * params, int sd, struct st_mq_server_msg * head, __attribute__((__unused__)) int rank_client_id)
{
    // check params...
    if (sd < 0)
    {
        return;
    }
    if (NULL == params)
    {
        return;
    }
    if (NULL == head)
    {
        return;
    }

    //TODO
    // rank_client_id = rank_client_id;
    //TODO

    // do setattr
    debug_info("[MQ-SERVER-OPS] SETATTR operation to be implemented !!\n");

    // show debug info
    debug_info("[MQ-SERVER-OPS] (ID=%s) SETATTR(...)\n", params -> srv_name);
}




//Directory API
void mq_server_op_mkdir(mq_server_param_st * params, int sd, struct st_mq_server_msg * head, int rank_client_id)
{
    int ret;
    char * s;

    debug_info("MQ_SERVER_MKDIR -- %s\n", head -> u_st_mq_server_msg.op_mkdir.path);

    // do mkdir
    s = head -> u_st_mq_server_msg.op_mkdir.path;
    ret = filesystem_mkdir(s, 0777); //TO-DO: 0777 received from the client

    mq_server_comm_write_data(params, sd, (char * ) & ret, sizeof(int), rank_client_id);

    // show debug info
    debug_info("[MQ-SERVER-OPS] (ID=%s) MKDIR(%s)\n", params -> srv_name, s);
}



void mq_server_op_opendir(mq_server_param_st * params, int sd, struct st_mq_server_msg * head, int rank_client_id)
{
    DIR * ret;
    char * s;

    // do mkdir
    s = head -> u_st_mq_server_msg.op_opendir.path;
    ret = filesystem_opendir(s);

    mq_server_comm_write_data(params, sd, (char * )(unsigned long long * ) & (ret), (unsigned int) sizeof(DIR * ), rank_client_id);

    // show debug info
    debug_info("[MQ-SERVER-OPS] (ID=%s) OPENDIR(%s)\n", params -> srv_name, s);
}



void mq_server_op_readdir(mq_server_param_st * params, int sd, struct st_mq_server_msg * head, int rank_client_id)
{
    struct dirent * ret;
    struct st_mq_server_direntry ret_entry;
    DIR * s;

    // do mkdir
    s = head -> u_st_mq_server_msg.op_readdir.dir;
    ret = filesystem_readdir(s);

    if (ret != NULL) {
        ret_entry.end = 1;
        ret_entry.ret = * ret;
    } else {
        ret_entry.end = 0;
    }

    mq_server_comm_write_data(params, sd, (char * ) & ret_entry, sizeof(struct st_mq_server_direntry), rank_client_id);

    // show debug info
    debug_info("[MQ-SERVER-OPS] (ID=%s) READDIR(%s)\n", params -> srv_name, s);
}



void mq_server_op_closedir(mq_server_param_st * params, int sd, struct st_mq_server_msg * head, int rank_client_id)
{
    int ret;
    DIR * s;

    // do mkdir
    s = head -> u_st_mq_server_msg.op_closedir.dir;
    ret = filesystem_closedir(s);

    mq_server_comm_write_data(params, sd, (char * ) & ret, sizeof(int), rank_client_id);

    // show debug info
    debug_info("[MQ-SERVER-OPS] (ID=%s) READDIR(%s)\n", params -> srv_name, s);
}



void mq_server_op_rmdir(mq_server_param_st * params, int sd, struct st_mq_server_msg * head, int rank_client_id)
{
    int ret;
    char * s;

    // do rmdir
    s = head -> u_st_mq_server_msg.op_rmdir.path;

    ret = filesystem_rmdir(s);
    mq_server_comm_write_data(params, sd, (char * ) & ret, sizeof(int), rank_client_id);

    // show debug info
    debug_info("[MQ-SERVER-OPS] (ID=%s) RMDIR(%s) \n", params -> srv_name, s);
}



//Optimization API

void mq_server_op_preload(mq_server_param_st * params, int sd, struct st_mq_server_msg * head, int rank_client_id)
{
    int ret  = 0;
    int fd_dest = 0, fd_orig = 0;
    char * protocol = NULL;
    char * user = NULL;
    char * machine = NULL;
    char * port = NULL;
    char * file = NULL;
    char * params1 = NULL;

    int BLOCKSIZE = head -> u_st_mq_server_msg.op_preload.block_size;
    char buffer[BLOCKSIZE];

    // Open origin file
    fd_orig = filesystem_open(head -> u_st_mq_server_msg.op_preload.storage_path, O_RDONLY);
    if (fd_orig < 0)
    {
        mq_server_comm_write_data(params, sd, (char * ) & fd_orig, sizeof(int), rank_client_id); // TO-DO: Check error treatment client-side
        return;
    }

    ret = ParseURL(head -> u_st_mq_server_msg.op_preload.virtual_path, protocol, user, machine, port, file, params1);

    // Create new file
    fd_dest = filesystem_creat(file, 0777);
    if (fd_dest < 0)
    {
        filesystem_close(fd_orig);
        mq_server_comm_write_data(params, sd, (char * ) & fd_dest, sizeof(int), rank_client_id); // TO-DO: Check error treatment client-side
        free(protocol); free(user); free(machine); free(file); free(params1) ;  
        return;
    }

    int cont = BLOCKSIZE * params -> rank;
    int read_bytes, write_bytes;

    do
    {
        off_t ret_2 = filesystem_lseek(fd_orig, cont, SEEK_SET);
        if (ret_2 < (off_t) -1)
        {
            filesystem_close(fd_orig);
            filesystem_close(fd_dest);
            mq_server_comm_write_data(params, sd, (char * ) & ret, sizeof(int), rank_client_id); // TO-DO: Check error treatment client-side
            free(protocol); free(user); free(machine); free(file); free(params1) ;  
            return;
        }

        read_bytes = filesystem_read(fd_orig, & buffer, BLOCKSIZE);
        if (read_bytes < 0)
        {
            filesystem_close(fd_orig);
            filesystem_close(fd_dest);
            mq_server_comm_write_data(params, sd, (char * ) & read_bytes, sizeof(int), rank_client_id); // TO-DO: Check error treatment client-side
            free(protocol); free(user); free(machine); free(file); free(params1) ;  
            return;
        }

        if (read_bytes > 0)
        {
            write_bytes = filesystem_write(fd_dest, & buffer, read_bytes);
            if (write_bytes < 0)
            {
                filesystem_close(fd_orig);
                filesystem_close(fd_dest);
                mq_server_comm_write_data(params, sd, (char * ) & write_bytes, sizeof(int), rank_client_id); // TO-DO: Check error treatment client-side
                free(protocol); free(user); free(machine); free(file); free(params1) ;  
                return;
            }
        }

        cont = cont + (BLOCKSIZE * params -> size);

    } while (read_bytes == BLOCKSIZE);

    filesystem_close(fd_orig);
    filesystem_close(fd_dest);

    mq_server_comm_write_data(params, sd, (char * ) & cont, sizeof(int), rank_client_id);

    // show debug info
    debug_info("[MQ-SERVER-OPS] (ID=%s) PRELOAD(%s,%s) -> %d\n", params -> srv_name, head -> u_st_mq_server_msg.op_preload.virtual_path, head -> u_st_mq_server_msg.op_preload.storage_path, ret);

    free(protocol); free(user); free(machine); free(file); free(params1) ;  
    return;
}



void mq_server_op_flush(mq_server_param_st * params, int sd, struct st_mq_server_msg * head, int rank_client_id)
{
    int ret  = 0;
    int fd_dest = 0, fd_orig = 0;
    char * protocol = NULL;
    char * user = NULL;
    char * machine = NULL;
    char * port = NULL;
    char * file = NULL;
    char * params1 = NULL;

    int BLOCKSIZE = head -> u_st_mq_server_msg.op_flush.block_size;
    char buffer[BLOCKSIZE];

    ret = ParseURL(head -> u_st_mq_server_msg.op_flush.virtual_path, protocol, user, machine, port, file, params1);

    // Open origin file
    fd_orig = filesystem_open(file, O_RDONLY);
    if (fd_orig < 0)
    {
        debug_info("Error on open operation on '%s'\n", file);
        mq_server_comm_write_data(params, sd, (char * ) & ret, sizeof(int), rank_client_id);
        free(protocol); free(user); free(machine); free(file); free(params1) ;  
        return;
    }

    // Create new file
    fd_dest = filesystem_open(head -> u_st_mq_server_msg.op_flush.storage_path, O_WRONLY | O_CREAT);
    if (fd_dest < 0)
    {
        debug_info("Error on open operation on '%s'\n", head -> u_st_mq_server_msg.op_flush.storage_path);
        filesystem_close(fd_orig);
        mq_server_comm_write_data(params, sd, (char * ) & fd_dest, sizeof(int), rank_client_id);
        free(protocol); free(user); free(machine); free(file); free(params1) ;  
        return;
    }

    //TCP_Barrier(TCP_COMM_WORLD);

    int cont = BLOCKSIZE * params -> rank;
    int read_bytes, write_bytes;

    do
    {
        read_bytes = filesystem_read(fd_orig, & buffer, BLOCKSIZE);
        if (read_bytes < 0)
        {
            filesystem_close(fd_orig);
            filesystem_close(fd_dest);
            mq_server_comm_write_data(params, sd, (char * ) & read_bytes, sizeof(int), rank_client_id); // TO-DO: Check error treatment client-side
            free(protocol); free(user); free(machine); free(file); free(params1) ;  
            return;
        }

        if (read_bytes > 0)
        {
            filesystem_lseek(fd_dest, cont, SEEK_SET);
            write_bytes = filesystem_write(fd_dest, & buffer, read_bytes);
            if (write_bytes < 0)
            {
                filesystem_close(fd_orig);
                filesystem_close(fd_dest);
                mq_server_comm_write_data(params, sd, (char * ) & write_bytes, sizeof(int), rank_client_id); // TO-DO: Check error treatment client-side
                free(protocol); free(user); free(machine); free(file); free(params1) ;  
                return;
            }
        }

        cont = cont + (BLOCKSIZE * params -> size);

    } while (read_bytes == BLOCKSIZE);

    filesystem_close(fd_orig);
    filesystem_close(fd_dest);
    mq_server_comm_write_data(params, sd, (char * ) & cont, sizeof(int), rank_client_id);

    // show debug info
    debug_info("[MQ-SERVER-OPS] (ID=%s) FLUSH(%s)\n", params -> srv_name, head -> u_st_mq_server_msg.op_flush.virtual_path);

    free(protocol); free(user); free(machine); free(file); free(params1) ;  
    return;
}



//FS Metadata API

void mq_server_op_getnodename(mq_server_param_st * params, int sd, __attribute__((__unused__)) struct st_mq_server_msg * head, int rank_client_id)
{
    char serv_name[HOST_NAME_MAX];

    DEBUG_BEGIN();

    // Get server host name
    gethostname(serv_name, HOST_NAME_MAX);

    // <TODO>
    // head = head; // Avoid unused parameter
    // </TODO>

    // show debug info
    debug_info("[MQ-SERVER-OPS] (ID=%s) GETNAME=%s\n", params -> srv_name, serv_name);

    mq_server_comm_write_data(params, sd, (char * ) serv_name, HOST_NAME_MAX, rank_client_id); // Send one single message
  //mq_server_comm_write_data(params, sd, (char * ) params -> sem_name_server, PATH_MAX, rank_client_id); // Send one single message

    DEBUG_END();
}

void mq_server_op_getid(mq_server_param_st * params, int sd, struct st_mq_server_msg * head, int rank_client_id)
{
    // do getid
    mq_server_comm_write_data(params, sd, (char * ) head -> id, MQ_SERVER_ID, rank_client_id); //TO-DO: Check function

    // show debug info
    debug_info("[MQ-SERVER-OPS] (ID=%s) GETID(...)\n", params -> srv_name);
}


/* ................................................................... */

