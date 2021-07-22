#ifndef _MYSERVER_OPS_H_
#define _MYSERVER_OPS_H_

#include "myServer.h"

int myServer_read_operation(int sd, struct st_myServer_msg *head);
void myServer_op_open(int sd, struct st_myServer_msg *head);
void myServer_op_creat(int sd, struct st_myServer_msg *head);
void myServer_op_close(int sd, struct st_myServer_msg *head);
void myServer_op_rm(int sd, struct st_myServer_msg *head);
void myServer_op_read(int sd, struct st_myServer_msg *head);
void myServer_op_write(int sd, struct st_myServer_msg *head);
void myServer_op_rmdir(int sd, struct st_myServer_msg *head);
void myServer_op_mkdir(int sd, struct st_myServer_msg *head);
void myServer_op_fstat(int sd, struct st_myServer_msg *head);
void myServer_op_setattr(int sd, struct st_myServer_msg *head);
void myServer_op_getattr(int sd, struct st_myServer_msg *head);
void myServer_op_flush(int sd, struct st_myServer_msg *head);
void myServer_op_preload(int sd, struct st_myServer_msg *head);
void myServer_op_getid(int sd, struct st_myServer_msg *head);

#endif
