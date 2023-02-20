#ifndef _TCP_SERVER_OPS_H_
#define _TCP_SERVER_OPS_H_

#include "tcp_server.h"


   /*
    *  API
    */

#ifdef _LARGEFILE64_
    #define LSEEK lseek64
#else
    #define LSEEK lseek
#endif

    int tcp_server_read_operation(int sd, struct st_tcp_server_msg *head);

    void tcp_server_op_open(int sd, struct st_tcp_server_msg *head);
    void tcp_server_op_open_wos(int sd, struct st_tcp_server_msg *head);
    void tcp_server_op_creat(int sd, struct st_tcp_server_msg *head);
    void tcp_server_op_creat_wos(int sd, struct st_tcp_server_msg *head);
    void tcp_server_op_read(int sd, struct st_tcp_server_msg *head);
    void tcp_server_op_read_wos(int sd, struct st_tcp_server_msg *head);
    void tcp_server_op_write(int sd, struct st_tcp_server_msg *head);
    void tcp_server_op_write_wos(int sd, struct st_tcp_server_msg *head);
    void tcp_server_op_close(int sd, struct st_tcp_server_msg *head);

    void tcp_server_op_rm(int sd, struct st_tcp_server_msg *head);
    void tcp_server_op_rmdir(int sd, struct st_tcp_server_msg *head);
    void tcp_server_op_mkdir(int sd, struct st_tcp_server_msg *head);
    void tcp_server_op_fstat(int sd, struct st_tcp_server_msg *head);
    void tcp_server_op_setattr(int sd, struct st_tcp_server_msg *head);
    void tcp_server_op_getattr(int sd, struct st_tcp_server_msg *head);
    void tcp_server_op_flush(int sd, struct st_tcp_server_msg *head);
    void tcp_server_op_preload(int sd, struct st_tcp_server_msg *head);
    void tcp_server_op_getid(int sd, struct st_tcp_server_msg *head);

#endif
