
/*
 *  Copyright 2020-2022 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
 *
 *  This file is part of mpiServer.
 *
 *  mpiServer is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  mpiServer is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with mpiServer.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


  /* ... Include / Inclusion ........................................... */

  #include "mpiServer_ops.h"
  #include "mpiServer_comm.h"
  #include "mpiServer_d2xpn.h"


  /* ... Functions / Funciones ......................................... */

  int aux_clear_dirs(char *path)
  {
    int j;
    char ant = '\0', s[255];

    j=0;
    for (unsigned i=0; i < strlen(path); i++)
    {
      switch(path[i])
      {
        case '/':
          if(ant != '/'){
            ant = s[j] = '/';
            j++;
          }
          break;
        default:
          ant = s[j] = path[i];
          j++;
      }

      s[j] = '\0';
    }

    strcpy(path, s);
    return 0;
  }

  int aux_get_dirs(char *path, int n, char *s)
  {
    long unsigned i=0;
    long j=0, ant=-1, pos=-1;
    int cont=-1;
    char new_path[MAXPATHLEN];

    strcpy(new_path, path);

    path = new_path;
    aux_clear_dirs(path);
    s[0] = '\0';
    if (path == NULL) {
      return 0;
    }

    for (i=0; i<strlen(path) && cont<(n+1); i++)
    {
      if (path[i] == '/'){
        if(ant == -1){
          ant = pos = i;
        }
        pos = i;
        cont++;
      }
    }

    if(cont<(n+1)){
      return 0;
    }

    for(j=ant; j<pos; j++){
      s[j] = path[j];
    }

    s[j] = '\0';

    return strlen(s);
  }

  int mpiServer_create_spacename ( mpiServer_param_st *params, char *path )
  {
    int i;
    char dir[MAXPATHLEN];

    // check params...
    if (NULL == params) {
        return -1;
    }

    debug_info("[OPS] (ID=%s) mpiServer_create_spacename: %s\n", params->srv_name, path);
    for(i=0; 0 != aux_get_dirs(path, i, dir);i++){
        mkdir(dir, 0777);
    }

    return 0;
  }

  char * mpiServer_op2string ( int op_code )
  {
    char *ret = "Unknown" ;

    switch (op_code)
    {
        case MPISERVER_OPEN_FILE:   ret = "OPEN" ;
             break ;
        case MPISERVER_CREAT_FILE:  ret = "CREAT" ;
             break ;
        case MPISERVER_READ_FILE:   ret = "READ" ;
             break ;
        case MPISERVER_WRITE_FILE:  ret = "WRITE" ;
             break ;
        case MPISERVER_CLOSE_FILE:  ret = "CLOSE" ;
             break ;
        case MPISERVER_RM_FILE:     ret = "RM" ;
             break ;
        case MPISERVER_GETATTR_FILE:    ret = "GETATTR" ;
             break ;
        case MPISERVER_SETATTR_FILE:    ret = "SETATTR" ;
             break ;
        case MPISERVER_MKDIR_DIR:   ret = "MKDIR" ;
             break ;
        case MPISERVER_RMDIR_DIR:   ret = "RMDIR" ;
             break ;
        case MPISERVER_OPENDIR_DIR: ret = "OPENDIR" ;
             break ;
        case MPISERVER_READDIR_DIR: ret = "READDIR" ;
             break ;
        case MPISERVER_CLOSEDIR_DIR:    ret = "CLOSEDIR" ;
             break ;
        case MPISERVER_FLUSH_FILE:  ret = "FLUSH" ;
             break ;
        case MPISERVER_PRELOAD_FILE:    ret = "PRELOAD" ;
             break ;
        case MPISERVER_STATFS_DIR:  ret = "STATFS" ;
             break ;
        case MPISERVER_FINALIZE:    ret = "FINALIZE" ;
             break ;
        case MPISERVER_GETID:       ret = "GETID" ;
             break ;
        case MPISERVER_END:     ret = "END" ;
             break ;
    }

    return ret ;
  }


/*
 * OPERATIONAL FUNCTIONS
 */

/**********************************
Read the operation to realize
***********************************/
/*int mpiServer_read_operation ( mpiServer_param_st *params, MPI_Comm sd, struct st_mpiServer_msg *head, int *rank_client_id )
{
    int ret;


        DEBUG_BEGIN() ;

    ret = mpiServer_comm_readoperation(params, sd, (char *)&head->type, sizeof(head->type), &rank_client_id);
    if (ret == -1) {
        debug_info("[OPS] (ID=%s)  mpiServer_comm_read_data fail\n") ;
        return -1;
    }

    debug_info("[OPS] (ID=%s) begin to read operation (%s) arguments\n", params->srv_name, mpiServer_op2string(head->type));
    switch(head->type)
    {
        case MPISERVER_OPEN_FILE:
            ret = mpiServer_comm_read_data(params, sd, (char *)&head->u_st_mpiServer_msg.op_open, sizeof(struct st_mpiServer_open), rank_client_id);
            break;
        case MPISERVER_CREAT_FILE:
            ret = mpiServer_comm_read_data(params, sd, (char *)&head->u_st_mpiServer_msg.op_creat, sizeof(struct st_mpiServer_creat), rank_client_id);
            break;
        case MPISERVER_READ_FILE:
            ret = mpiServer_comm_read_data(params, sd, (char *)&head->u_st_mpiServer_msg.op_read, sizeof(struct st_mpiServer_read), rank_client_id);
            break;
        case MPISERVER_WRITE_FILE:
            ret = mpiServer_comm_read_data(params, sd, (char *)&head->u_st_mpiServer_msg.op_write, sizeof(struct st_mpiServer_write), rank_client_id);
            break;
        case MPISERVER_CLOSE_FILE:
            ret = mpiServer_comm_read_data(params, sd, (char *)&head->u_st_mpiServer_msg.op_close, sizeof(struct st_mpiServer_close), rank_client_id);
            break;
        case MPISERVER_RM_FILE:
            ret = mpiServer_comm_read_data(params, sd, (char *)&head->u_st_mpiServer_msg.op_rm, sizeof(struct st_mpiServer_rm), rank_client_id);
            break;
        case MPISERVER_GETATTR_FILE:
            ret = mpiServer_comm_read_data(params, sd, (char *)&head->u_st_mpiServer_msg.op_getattr, sizeof(struct st_mpiServer_getattr), rank_client_id);
            break;
        case MPISERVER_SETATTR_FILE:
            ret = mpiServer_comm_read_data(params, sd, (char *)&head->u_st_mpiServer_msg.op_setattr, sizeof(struct st_mpiServer_setattr), rank_client_id);
            break;
        case MPISERVER_MKDIR_DIR:
            ret = mpiServer_comm_read_data(params, sd, (char *)&head->u_st_mpiServer_msg.op_mkdir, sizeof(struct st_mpiServer_mkdir), rank_client_id);
            break;
        case MPISERVER_RMDIR_DIR:
            ret = mpiServer_comm_read_data(params, sd, (char *)&head->u_st_mpiServer_msg.op_rmdir, sizeof(struct st_mpiServer_rmdir), rank_client_id);
            break;
        case MPISERVER_FLUSH_FILE:
            ret = mpiServer_comm_read_data(params, sd, (char *)&head->u_st_mpiServer_msg.op_flush, sizeof(struct st_mpiServer_flush), rank_client_id);
            break;
        case MPISERVER_PRELOAD_FILE:
            ret = mpiServer_comm_read_data(params, sd, (char *)&head->u_st_mpiServer_msg.op_preload, sizeof(struct st_mpiServer_preload), rank_client_id);
            break;
        case MPISERVER_GETID:
            // ret = mpiServer_comm_read_data(params, sd, (char *)&head->id, sizeof(MPISERVER_ID), rank_client_id);
            break;
        case MPISERVER_FINALIZE:
            // ret = mpiServer_comm_read_data(params, sd, (char *)&head->u_st_mpiServer_msg.op_end, sizeof(struct st_mpiServer_end), rank_client_id);
            break;
        case MPISERVER_END:
            // ret = mpiServer_comm_read_data(params, sd, (char *)&head->u_st_mpiServer_msg.op_end, sizeof(struct st_mpiServer_end), rank_client_id);
            break;
    }

        DEBUG_END() ;

    // Return
    if (ret == -1) {
        return -1;
    }
    return head->type;
}*/

void mpiServer_op_open ( mpiServer_param_st *params, MPI_Comm sd, struct st_mpiServer_msg *head, int rank_client_id )
{
    int fd;
    char *s;

    // do open
    s = head->u_st_mpiServer_msg.op_open.path;
    fd = open(s, O_RDWR);
    mpiServer_comm_write_data(params, sd, (char *)&fd, sizeof(int), rank_client_id);

    // show debug info
    debug_info("[OPS] (ID=%s) OPEN(%s)=%d\n", params->srv_name, s, fd);
}

void mpiServer_op_creat ( mpiServer_param_st *params, MPI_Comm sd, struct st_mpiServer_msg *head, int rank_client_id )
{
    int fd;
    char *s;

    // do creat
    s = head->u_st_mpiServer_msg.op_creat.path;
    fd = open(s, O_CREAT | O_RDWR, 0777);
    if (fd == -1)
    {
        mpiServer_create_spacename(params, s);
            fd = open(s, O_CREAT | O_RDWR, 0660);
    }
    mpiServer_comm_write_data(params, sd, (char *)&fd, sizeof(int), rank_client_id) ;

    // show debug info
    debug_info("[OPS] (ID=%s) CREAT(%s)=%d\n", params->srv_name, s, fd);
}

void mpiServer_op_flush ( mpiServer_param_st *params, MPI_Comm sd, struct st_mpiServer_msg *head, int rank_client_id)
{
    int ret;

    // check arguments
        if (NULL == head) {
        return ;
    }

    // do flush
    ret = 0 ; // TODO: do flush...
    debug_warning("[OPS] (ID=%s) TODO: flush\n", params->srv_name) ;

    mpiServer_comm_write_data(params, sd, (char *)&ret, sizeof(int), rank_client_id) ;

    // show debug info
    debug_info("[OPS] (ID=%s) FLUSH(%s)\n", params->srv_name, head->u_st_mpiServer_msg.op_flush.virtual_path);
}

void mpiServer_op_preload ( mpiServer_param_st *params, MPI_Comm sd, struct st_mpiServer_msg *head, int rank_client_id )
{
    int ret;

    // do preload
    ret = mpiServer_d2xpn(params,
                  head->u_st_mpiServer_msg.op_preload.virtual_path,
                              head->u_st_mpiServer_msg.op_preload.storage_path) ;
    mpiServer_comm_write_data(params, sd, (char *)&ret, sizeof(int), rank_client_id);

    // show debug info
    debug_info("[OPS] (ID=%s) PRELOAD(%s,%s) -> %d\n", params->srv_name,
                                        head->u_st_mpiServer_msg.op_preload.virtual_path,
                                        head->u_st_mpiServer_msg.op_preload.storage_path,
                                        ret);
}

void mpiServer_op_close ( mpiServer_param_st *params, MPI_Comm sd, struct st_mpiServer_msg *head, int rank_client_id )
{
    // check params...
    if (NULL == params) {
        return ;
    }

    // do close
    close(head->u_st_mpiServer_msg.op_close.fd);

    //TODO return client

    // show debug info
    debug_info("[OPS] (ID=%s) CLOSE(fd=%d)\n", params->srv_name, head->u_st_mpiServer_msg.op_close.fd);
}

void mpiServer_op_rm ( mpiServer_param_st *params, MPI_Comm sd, struct st_mpiServer_msg *head, int rank_client_id )
{
    char *s;

    // check params...
    if (NULL == params) {
        return ;
    }

    // do rm
    s = head->u_st_mpiServer_msg.op_rm.path;
    unlink(s);

    //TODO return client

    // show debug info
    debug_info("[OPS] (ID=%s) RM(path=%s)\n", params->srv_name, head->u_st_mpiServer_msg.op_rm.path);
}


long op_read_buffer ( mpiServer_param_st *params, int read_fd2, void *buffer, int buffer_size )
{
     ssize_t read_num_bytes       = -1 ;
     ssize_t read_remaining_bytes = buffer_size ;
     void   *read_buffer          = buffer ;

     // check arguments...
     if (NULL == params) {
     debug_warning("WARNING[%s]:\t read with NULL mpiServer_param_st *.\n", params->srv_name) ;
     }

     while (read_remaining_bytes > 0)
     {
     /* Read from local file... */
         read_num_bytes = read(read_fd2, read_buffer, read_remaining_bytes) ;

     /* Check errors */
         if (read_num_bytes == -1) {
         debug_error("ERROR[%s]:\t read fails to read data.\n", params->srv_name) ;
         return -1 ;
         }

     /* Check end of file */
         if (read_num_bytes == 0)
     {
         debug_error("INFO[%s]:\t end of file, readed %ld.\n", 
             params->srv_name, 
             (buffer_size - read_remaining_bytes)) ;
         return (buffer_size - read_remaining_bytes) ;
         }

         read_remaining_bytes -= read_num_bytes ;
         read_buffer          += read_num_bytes ;
     }

     return buffer_size ;
}

long op_write_buffer ( mpiServer_param_st *params, int write_fd2, void *buffer, int buffer_size, int num_readed_bytes )
{
     ssize_t write_num_bytes       = -1 ;
     ssize_t write_remaining_bytes = num_readed_bytes ;
     void   *write_buffer          = buffer ;

     // check arguments...
     if (NULL == params) {
     debug_warning("WARNING[%s]:\t read with NULL mpiServer_param_st *.\n", params->srv_name) ;
     }
     if (num_readed_bytes > buffer_size) {
     debug_error("ERROR[%s]:\t write for %d bytes from a buffer with only %d bytes.\n", params->srv_name, num_readed_bytes, buffer_size) ;
     return -1 ;
     }

     while (write_remaining_bytes > 0)
     {
     /* Write into local file (write_fd2)... */
         write_num_bytes = write(write_fd2, write_buffer, write_remaining_bytes) ;

     /* Check errors */
         if (write_num_bytes == -1) {
         debug_error("ERROR[%s]:\t write fails to write data.\n", params->srv_name) ;
         return -1 ;
         }

         write_remaining_bytes -= write_num_bytes ;
         write_buffer          += write_num_bytes ;
     }

     return num_readed_bytes ;
}












void mpiServer_op_read ( mpiServer_param_st *params, MPI_Comm sd, struct st_mpiServer_msg *head, int rank_client_id )
{
    long size ;
    int cont = 0;
    struct st_mpiServer_read_req req;
    char *buffer;
    //char buffer[128*1024];
   
    debug_info("[OPS] (ID=%s) begin read: fd %d offset %d size %d ID=x\n",
           params->srv_name,
           head->u_st_mpiServer_msg.op_read.fd,
           (int)head->u_st_mpiServer_msg.op_read.offset,
           head->u_st_mpiServer_msg.op_read.size);

    // malloc a buffer of size...
    size = head->u_st_mpiServer_msg.op_read.size;

    if (size > (1 * 1024 * 1024)) // > 1MB
    {
        size = 1 * 1024 * 1024; // 1MB
    }

    buffer = (char *)malloc(size) ;
    if (NULL == buffer)
    {
        req.size = -1;  // TODO: check in client that -1 is treated properly... :-9
        mpiServer_comm_write_data(params, sd,(char *)&req,sizeof(struct st_mpiServer_write_req), rank_client_id);
        return ;
    }

    // read data...
    LSEEK(head->u_st_mpiServer_msg.op_read.fd, head->u_st_mpiServer_msg.op_read.offset, 0);
    req.size = op_read_buffer(params, head->u_st_mpiServer_msg.op_read.fd, buffer, size);
    if (req.size < 0) {
        perror("read:");

        req.size = -1;  // TODO: check in client that -1 is treated properly... :-9
        mpiServer_comm_write_data(params, sd,(char *)&req,sizeof(struct st_mpiServer_write_req), rank_client_id);

        FREE_AND_NULL(buffer) ;
        return ;
    }

    // send (how many + data) to client...
    mpiServer_comm_write_data(params, sd, (char *)&req, sizeof(struct st_mpiServer_read_req), rank_client_id);
    debug_info("[OPS] (ID=%s) op_read: send size %d\n", params->srv_name, req.size);

    mpiServer_comm_write_data(params, sd, buffer, req.size, rank_client_id);
    debug_info("[OPS] (ID=%s) op_read: send data\n", params->srv_name);

    // free buffer
    FREE_AND_NULL(buffer) ;

    // debugging information
    debug_info("[OPS] (ID=%s) end READ: fd %d offset %d size %d ID=x\n",
                params->srv_name,
                head->u_st_mpiServer_msg.op_read.fd,
                (int)head->u_st_mpiServer_msg.op_read.offset,
                size) ;
}












void mpiServer_op_write ( mpiServer_param_st *params, MPI_Comm sd, struct st_mpiServer_msg *head, int rank_client_id )
{
    struct st_mpiServer_write_req req;
    int size ;
    char *buffer;
    //char buffer[128*1024];
    
    debug_info("[OPS] (ID=%s) begin write: fd %d ID=xn", params->srv_name, head->u_st_mpiServer_msg.op_write.fd);

    // malloc a buffer of size...
    size = (head->u_st_mpiServer_msg.op_write.size);

    if (size > (1 * 1024 * 1024)) // > 1MB
    {
        size = 1 * 1024 * 1024; // 1MB
    }

    buffer = (char *)malloc(size) ;
    
    if (NULL == buffer)
    {
        req.size = -1;  // TODO: check in client that -1 is treated properly... :-9
        mpiServer_comm_write_data(params, sd,(char *)&req,sizeof(struct st_mpiServer_write_req), rank_client_id);
        return ;
    }

    // read data from MPI
    mpiServer_comm_read_data(params, sd, buffer, size, rank_client_id);
    // write into the file
    LSEEK(head->u_st_mpiServer_msg.op_write.fd, head->u_st_mpiServer_msg.op_write.offset, 0);
    req.size = op_write_buffer(params, head->u_st_mpiServer_msg.op_write.fd, buffer, size, size) ;

    // write to the client the status of the write operation
    mpiServer_comm_write_data(params, sd,(char *)&req,sizeof(struct st_mpiServer_write_req), rank_client_id);

    // free buffer
    FREE_AND_NULL(buffer) ;

    // for debugging purpouses 
    debug_info("[OPS] (ID=%s) end write: fd %d ID=xn", params->srv_name, head->u_st_mpiServer_msg.op_write.fd);
}















void mpiServer_op_mkdir ( mpiServer_param_st *params, MPI_Comm sd, struct st_mpiServer_msg *head, int rank_client_id )
{
    int ret;
    char *s;

    // do mkdir
    s = head->u_st_mpiServer_msg.op_mkdir.path;
    ret = mkdir(s, 0777);
    mpiServer_comm_write_data(params, sd,(char *)&ret,sizeof(int), rank_client_id);

    // show debug info
    debug_info("[OPS] (ID=%s) MKDIR(%s)\n", params->srv_name, s);
}

void mpiServer_op_rmdir ( mpiServer_param_st *params, MPI_Comm sd, struct st_mpiServer_msg *head, int rank_client_id )
{
    int ret;
    char *s;

    // do rmdir
    s = head->u_st_mpiServer_msg.op_rmdir.path; 
    ret = rmdir(s);
    mpiServer_comm_write_data(params, sd, (char *)&ret, sizeof(int), rank_client_id);

    // show debug info
    debug_info("[OPS] (ID=%s) RMDIR(%s) \n", params->srv_name, s);
}

void mpiServer_op_setattr ( mpiServer_param_st *params, MPI_Comm sd, struct st_mpiServer_msg *head, int rank_client_id )
{
    // check params...
    if (sd < 0) {
        return ;
    }
    if (NULL == params) {
        return ;
    }
    if (NULL == head) {
        return ;
    }

    //TODO

    // do setattr
    debug_info("[OPS] SETATTR operation to be implemented !!\n");

    // show debug info
    debug_info("[OPS] (ID=%s) SETATTR(...)\n", params->srv_name);
}

void mpiServer_op_getattr ( mpiServer_param_st *params, MPI_Comm sd, struct st_mpiServer_msg *head, int rank_client_id )
{
    struct st_mpiServer_attr_req req;
    char *s;

    // do getattr
    s = head->u_st_mpiServer_msg.op_getattr.path;
    req.status = stat(s, &req.attr);
    mpiServer_comm_write_data(params, sd,(char *)&req,sizeof(struct st_mpiServer_attr_req), rank_client_id);

    // show debug info
    debug_info("[OPS] (ID=%s) GETATTR(%s)\n", params->srv_name, head->u_st_mpiServer_msg.op_getattr.path);
}

void mpiServer_op_getid ( mpiServer_param_st *params, MPI_Comm sd, struct st_mpiServer_msg *head, int rank_client_id)
{
    // do getid
        mpiServer_comm_write_data(params, sd,(char *)head->id, MPISERVER_ID, rank_client_id);

    // show debug info
        debug_info("[OPS] (ID=%s) GETID(...)\n", params->srv_name);
}

   /* ................................................................... */

