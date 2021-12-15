#ifndef _MYSERVER_OPS_H_
#define _MYSERVER_OPS_H_

#include "tcpServer.h"

int tcpServer_read_operation(int sd, struct st_tcpServer_msg *head);
void tcpServer_op_open(int sd, struct st_tcpServer_msg *head);
void tcpServer_op_creat(int sd, struct st_tcpServer_msg *head);
void tcpServer_op_close(int sd, struct st_tcpServer_msg *head);
void tcpServer_op_rm(int sd, struct st_tcpServer_msg *head);
void tcpServer_op_read(int sd, struct st_tcpServer_msg *head);
void tcpServer_op_write(int sd, struct st_tcpServer_msg *head);
void tcpServer_op_rmdir(int sd, struct st_tcpServer_msg *head);
void tcpServer_op_mkdir(int sd, struct st_tcpServer_msg *head);
void tcpServer_op_fstat(int sd, struct st_tcpServer_msg *head);
void tcpServer_op_setattr(int sd, struct st_tcpServer_msg *head);
void tcpServer_op_getattr(int sd, struct st_tcpServer_msg *head);
void tcpServer_op_flush(int sd, struct st_tcpServer_msg *head);
void tcpServer_op_preload(int sd, struct st_tcpServer_msg *head);
void tcpServer_op_getid(int sd, struct st_tcpServer_msg *head);

#endif
