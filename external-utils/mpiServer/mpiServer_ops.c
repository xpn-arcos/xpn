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
    #include "mpiServer_file.h"
    #include "mpiServer_comm.h"
    #include "mpiServer_d2xpn.h"


  /* ... Functions / Funciones ......................................... */

  char * mpiServer_op2string ( int op_code )
  {
    char *ret = "Unknown" ;

    switch (op_code)
    {
        case MPISERVER_OPEN_FILE:       ret = "OPEN" ;
             break ;
        case MPISERVER_CREAT_FILE:      ret = "CREAT" ;
             break ;
        case MPISERVER_READ_FILE:       ret = "READ" ;
             break ;
        case MPISERVER_WRITE_FILE:      ret = "WRITE" ;
             break ;
        case MPISERVER_CLOSE_FILE:      ret = "CLOSE" ;
             break ;
        case MPISERVER_RM_FILE:         ret = "RM" ;
             break ;
        case MPISERVER_GETATTR_FILE:    ret = "GETATTR" ;
             break ;
        case MPISERVER_SETATTR_FILE:    ret = "SETATTR" ;
             break ;
        case MPISERVER_MKDIR_DIR:       ret = "MKDIR" ;
             break ;
        case MPISERVER_RMDIR_DIR:       ret = "RMDIR" ;
             break ;
        case MPISERVER_OPENDIR_DIR:     ret = "OPENDIR" ;
             break ;
        case MPISERVER_READDIR_DIR:     ret = "READDIR" ;
             break ;
        case MPISERVER_CLOSEDIR_DIR:    ret = "CLOSEDIR" ;
             break ;
        case MPISERVER_FLUSH_FILE:      ret = "FLUSH" ;
             break ;
        case MPISERVER_PRELOAD_FILE:    ret = "PRELOAD" ;
             break ;
        case MPISERVER_STATFS_DIR:      ret = "STATFS" ;
             break ;
        case MPISERVER_FINALIZE:        ret = "FINALIZE" ;
             break ;
        case MPISERVER_GETID:           ret = "GETID" ;
             break ;
        case MPISERVER_DISCONNECT:      ret = "DISCONNECT" ;
             break ;
        case MPISERVER_GETNAME:         ret = "GETNAME" ;
             break ;
        case MPISERVER_END:             ret = "END" ;
             break ;
    }

    return ret ;
  }


  /*
   * OPERATIONAL FUNCTIONS
   */

  void  mpiServer_op_getname     ( mpiServer_param_st *params, MPI_Comm sd, struct st_mpiServer_msg *head, int rank_client_id ) ; //NEW

  void  mpiServer_op_open        ( mpiServer_param_st *params, MPI_Comm sd, struct st_mpiServer_msg *head, int rank_client_id ) ;
  void  mpiServer_op_creat       ( mpiServer_param_st *params, MPI_Comm sd, struct st_mpiServer_msg *head, int rank_client_id ) ;
  void  mpiServer_op_read        ( mpiServer_param_st *params, MPI_Comm sd, struct st_mpiServer_msg *head, int rank_client_id ) ;
  void  mpiServer_op_write       ( mpiServer_param_st *params, MPI_Comm sd, struct st_mpiServer_msg *head, int rank_client_id ) ;
  void  mpiServer_op_close       ( mpiServer_param_st *params, MPI_Comm sd, struct st_mpiServer_msg *head, int rank_client_id ) ;
  void  mpiServer_op_rm          ( mpiServer_param_st *params, MPI_Comm sd, struct st_mpiServer_msg *head, int rank_client_id ) ;
  void  mpiServer_op_setattr     ( mpiServer_param_st *params, MPI_Comm sd, struct st_mpiServer_msg *head, int rank_client_id ) ;
  void  mpiServer_op_getattr     ( mpiServer_param_st *params, MPI_Comm sd, struct st_mpiServer_msg *head, int rank_client_id ) ;

  void  mpiServer_op_mkdir       ( mpiServer_param_st *params, MPI_Comm sd, struct st_mpiServer_msg *head, int rank_client_id ) ;
  void  mpiServer_op_rmdir       ( mpiServer_param_st *params, MPI_Comm sd, struct st_mpiServer_msg *head, int rank_client_id ) ;

  void  mpiServer_op_flush       ( mpiServer_param_st *params, MPI_Comm sd, struct st_mpiServer_msg *head, int rank_client_id ) ;
  int   mpiServer_op_preload     ( mpiServer_param_st *params, MPI_Comm sd, struct st_mpiServer_msg *head, int rank_client_id ) ;
  
  void  mpiServer_op_fstat       ( mpiServer_param_st *params, MPI_Comm sd, struct st_mpiServer_msg *head, int rank_client_id ) ;
  void  mpiServer_op_getid       ( mpiServer_param_st *params, MPI_Comm sd, struct st_mpiServer_msg *head, int rank_client_id ) ;

  /**********************************
  Read the operation to realize
  ***********************************/
  int mpiServer_do_operation ( struct st_th *th, int * the_end )
  {

    DEBUG_BEGIN() ;

    int ret;
    struct st_mpiServer_msg head;

    switch(th->type_op)
    {
      //Connection API
      case MPISERVER_DISCONNECT:
        break;

      case MPISERVER_FINALIZE:
        *the_end = 1;
        break;

      case MPISERVER_GETNAME:
        mpiServer_op_getname(th->params, th->sd, &head, th->rank_client_id); //NEW
        break;

      //File API
      case MPISERVER_OPEN_FILE:
        ret = mpiServer_comm_read_data(th->params, th->sd, (char *)&(head.u_st_mpiServer_msg.op_open), sizeof(struct st_mpiServer_open), th->rank_client_id);
        if (ret != -1) {
            mpiServer_op_open(th->params, th->sd, &head, th->rank_client_id);
        }
        break;
      case MPISERVER_CREAT_FILE:
        ret = mpiServer_comm_read_data(th->params, th->sd, (char *)&(head.u_st_mpiServer_msg.op_creat), sizeof(struct st_mpiServer_creat), th->rank_client_id);
        if (ret != -1) {
            mpiServer_op_creat(th->params, th->sd, &head, th->rank_client_id);
        }
        break;
      case MPISERVER_READ_FILE:
        ret = mpiServer_comm_read_data(th->params, th->sd, (char *)&(head.u_st_mpiServer_msg.op_read), sizeof(struct st_mpiServer_read), th->rank_client_id);
        if (ret != -1) {
            mpiServer_op_read(th->params, th->sd, &head, th->rank_client_id);
        }
        break;
      case MPISERVER_WRITE_FILE:
        ret = mpiServer_comm_read_data(th->params, th->sd, (char *)&(head.u_st_mpiServer_msg.op_write), sizeof(struct st_mpiServer_write), th->rank_client_id);
        if (ret != -1) {
            mpiServer_op_write(th->params, th->sd, &head, th->rank_client_id);
        }
        break;
      case MPISERVER_CLOSE_FILE:
        ret = mpiServer_comm_read_data(th->params, th->sd, (char *)&(head.u_st_mpiServer_msg.op_close), sizeof(struct st_mpiServer_close), th->rank_client_id);
        if (ret != -1) {
            mpiServer_op_close(th->params, th->sd, &head, th->rank_client_id);
        }
        break;
      case MPISERVER_RM_FILE:
        ret = mpiServer_comm_read_data(th->params, th->sd, (char *)&(head.u_st_mpiServer_msg.op_rm), sizeof(struct st_mpiServer_rm), th->rank_client_id);
        if (ret != -1) {
            mpiServer_op_rm(th->params, th->sd, &head, th->rank_client_id);
        }
        break;
      case MPISERVER_GETATTR_FILE:
        ret = mpiServer_comm_read_data(th->params, th->sd, (char *)&(head.u_st_mpiServer_msg.op_getattr), sizeof(struct st_mpiServer_getattr), th->rank_client_id);
        if (ret != -1) {
            mpiServer_op_getattr(th->params, th->sd, &head, th->rank_client_id);
        }
        break;
      case MPISERVER_SETATTR_FILE:
        ret = mpiServer_comm_read_data(th->params, th->sd, (char *)&(head.u_st_mpiServer_msg.op_setattr), sizeof(struct st_mpiServer_setattr), th->rank_client_id);
        if (ret != -1) {
            mpiServer_op_setattr(th->params, th->sd, &head, th->rank_client_id);
        }
        break;

      //Directory API
      case MPISERVER_MKDIR_DIR:
        ret = mpiServer_comm_read_data(th->params, th->sd, (char *)&(head.u_st_mpiServer_msg.op_mkdir), sizeof(struct st_mpiServer_mkdir), th->rank_client_id);
        if (ret != -1) {
            mpiServer_op_mkdir(th->params, th->sd, &head, th->rank_client_id);
        }
        break;
      case MPISERVER_RMDIR_DIR:
        ret = mpiServer_comm_read_data(th->params, th->sd, (char *)&(head.u_st_mpiServer_msg.op_rmdir), sizeof(struct st_mpiServer_rmdir), th->rank_client_id);
        if (ret != -1) {
            mpiServer_op_rmdir(th->params, th->sd, &head, th->rank_client_id);
        }
        break;

      //Optimization API
      case MPISERVER_PRELOAD_FILE:
        ret = mpiServer_comm_read_data(th->params, th->sd, (char *)&(head.u_st_mpiServer_msg.op_preload), sizeof(struct st_mpiServer_preload), th->rank_client_id);
        if (ret != -1) {
            ret = mpiServer_op_preload(th->params, th->sd, &head, th->rank_client_id);
        }
        break;
      case MPISERVER_FLUSH_FILE:
        ret = mpiServer_comm_read_data(th->params, th->sd, (char *)&(head.u_st_mpiServer_msg.op_flush), sizeof(struct st_mpiServer_flush), th->rank_client_id);
        if (ret != -1) {
            mpiServer_op_flush(th->params, th->sd, &head, th->rank_client_id);
        }
        break;
    }

    DEBUG_END() ;

    return 0;
  }



  //Connection API

  void  mpiServer_op_getname ( mpiServer_param_st *params, MPI_Comm sd, struct st_mpiServer_msg *head, int rank_client_id ) //NEW
  {
    char serv_name [1024]; //Other value??

    // Get server host name
    gethostname(serv_name, sizeof(serv_name));

    mpiServer_comm_write_data(params, sd, (char *)&serv_name, sizeof(serv_name), rank_client_id);

    // show debug info
    debug_info("[OPS] (ID=%s) GETNAME=%s\n", params->srv_name, serv_name);
  }



  //File API

  void mpiServer_op_open ( mpiServer_param_st *params, MPI_Comm sd, struct st_mpiServer_msg *head, int rank_client_id )
  {
    int fd;
    char *s;

    // do open
    s = head->u_st_mpiServer_msg.op_open.path;
    fd = mpiServer_file_open(s, O_RDWR) ;

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
    fd = mpiServer_file_creat(params, s, 0777) ;

    mpiServer_comm_write_data(params, sd, (char *)&fd, sizeof(int), rank_client_id) ;

    // show debug info
    debug_info("[OPS] (ID=%s) CREAT(%s)=%d\n", params->srv_name, s, fd);
  }

  void mpiServer_op_read ( mpiServer_param_st *params, MPI_Comm sd, struct st_mpiServer_msg *head, int rank_client_id )
  {
    struct st_mpiServer_read_req req;
    char *buffer;
    long  size, diff, to_read, cont ;

    debug_info("[OPS] (ID=%s) begin read: fd %d offset %d size %d ID=x\n", params->srv_name,
                                                                           head->u_st_mpiServer_msg.op_read.fd,
                                                                           (int)head->u_st_mpiServer_msg.op_read.offset,
                                                                           head->u_st_mpiServer_msg.op_read.size);
    // initialize counters
    cont = 0 ;
    size = head->u_st_mpiServer_msg.op_read.size;
    if (size > MAX_BUFFER_SIZE) {
        size = MAX_BUFFER_SIZE;
    }
    diff = head->u_st_mpiServer_msg.op_read.size - cont;

    // malloc a buffer of size...
    buffer = (char *)malloc(size) ;
    if (NULL == buffer)
    {
      req.size = -1;  // TODO: check in client that -1 is treated properly... :-9
      mpiServer_comm_write_data(params, sd,(char *)&req,sizeof(struct st_mpiServer_write_req), rank_client_id);
      return ;
    }
    // loop...
    do
    {
      if (diff > size)
        to_read = size ;
      else to_read = diff ;

      // lseek and read data...
      LSEEK(head->u_st_mpiServer_msg.op_read.fd, head->u_st_mpiServer_msg.op_read.offset + cont, SEEK_SET);
      req.size = mpiServer_file_read_buffer(params, head->u_st_mpiServer_msg.op_read.fd, buffer, to_read);
      // if error then send as "how many bytes" -1
      if (req.size < 0)
      {
        req.size = -1;  // TODO: check in client that -1 is treated properly... :-)
        mpiServer_comm_write_data(params, sd,(char *)&req,sizeof(struct st_mpiServer_write_req), rank_client_id);

        FREE_AND_NULL(buffer) ;
        return ;
      }
      // send (how many + data) to client...
      mpiServer_comm_write_data(params, sd, (char *)&req, sizeof(struct st_mpiServer_read_req), rank_client_id);
      debug_info("[OPS] (ID=%s) op_read: send size %d\n", params->srv_name, req.size);

      // send data to client...
      if (req.size > 0) {
        mpiServer_comm_write_data(params, sd, buffer, req.size, rank_client_id);
        debug_info("[OPS] (ID=%s) op_read: send data\n", params->srv_name);
      }
      cont = cont + req.size; //Send bytes
      diff = head->u_st_mpiServer_msg.op_read.size - cont;

    } while ((diff > 0) && (req.size != 0)) ;

    // free buffer
    FREE_AND_NULL(buffer) ;

    // debugging information
    debug_info("[OPS] (ID=%s) end READ: fd %d offset %d size %d ID=x\n",  params->srv_name,
                                                                          head->u_st_mpiServer_msg.op_read.fd,
                                                                          (int)head->u_st_mpiServer_msg.op_read.offset,
                                                                          size) ;
  }

  void mpiServer_op_write ( mpiServer_param_st *params, MPI_Comm sd, struct st_mpiServer_msg *head, int rank_client_id )
  {
    struct st_mpiServer_write_req req;
    char *buffer;
    int   size, diff, cont, to_write ;

    debug_info("[OPS] (ID=%s) begin write: fd %d ID=xn", params->srv_name, head->u_st_mpiServer_msg.op_write.fd);

    // initialize counters
    cont = 0 ;
    size = (head->u_st_mpiServer_msg.op_write.size);
    if (size > MAX_BUFFER_SIZE) {
        size = MAX_BUFFER_SIZE;
    }
    diff = head->u_st_mpiServer_msg.op_read.size - cont;

    // malloc a buffer of size...
    buffer = (char *)malloc(size) ;
    if (NULL == buffer)
    {
      req.size = -1;  // TODO: check in client that -1 is treated properly... :-)
      mpiServer_comm_write_data(params, sd,(char *)&req,sizeof(struct st_mpiServer_write_req), rank_client_id);
      return ;
    }

    // loop...
    do
    {
      if (diff > size)
     to_write = size ;
      else to_write = diff ;

      // read data from MPI and write into the file
      mpiServer_comm_read_data(params, sd, buffer, to_write, rank_client_id);
      LSEEK(head->u_st_mpiServer_msg.op_write.fd, head->u_st_mpiServer_msg.op_write.offset + cont, SEEK_SET);
      req.size = mpiServer_file_write_buffer(params, head->u_st_mpiServer_msg.op_write.fd, buffer, to_write, to_write) ;

      // update counters
      cont = cont + req.size ; // Received bytes
      diff = head->u_st_mpiServer_msg.op_read.size - cont ;

    } while ((diff > 0) && (req.size != 0)) ;

    // write to the client the status of the write operation
    req.size = cont;
    mpiServer_comm_write_data(params, sd,(char *)&req,sizeof(struct st_mpiServer_write_req), rank_client_id);

    // free buffer
    FREE_AND_NULL(buffer) ;

    // for debugging purpouses
    debug_info("[OPS] (ID=%s) end write: fd %d ID=xn", params->srv_name, head->u_st_mpiServer_msg.op_write.fd);
  }

  void mpiServer_op_close ( mpiServer_param_st *params, MPI_Comm sd, struct st_mpiServer_msg *head, int rank_client_id )
  {
    // check params...
    if (NULL == params) {
      return ;
    }

    // do close
    int ret = mpiServer_file_close(head->u_st_mpiServer_msg.op_close.fd) ;

    mpiServer_comm_write_data(params, sd, (char *)&ret, sizeof(int), rank_client_id);

    // show debug info
    debug_info("[OPS] (ID=%s) CLOSE(fd=%d)\n", params->srv_name, head->u_st_mpiServer_msg.op_close.fd) ;
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
    int ret = unlink(s);

    mpiServer_comm_write_data(params, sd, (char *)&ret, sizeof(int), rank_client_id);

    // show debug info
    debug_info("[OPS] (ID=%s) RM(path=%s)\n", params->srv_name, head->u_st_mpiServer_msg.op_rm.path);
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

  

  //Directory API
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



  //Optimization API

  int mpiServer_op_preload ( mpiServer_param_st *params, MPI_Comm sd, struct st_mpiServer_msg *head, int rank_client_id )
  {
    int ret;
    int  fd_dest, fd_orig;

    int BLOCKSIZE = head->u_st_mpiServer_msg.op_preload.block_size;
    char buffer [BLOCKSIZE];
    char path [PATH_MAX];

    // do preload
    /*ret = mpiServer_d2xpn(params,
                          head->u_st_mpiServer_msg.op_preload.virtual_path,
                          head->u_st_mpiServer_msg.op_preload.storage_path) ;*/

    //Open origin file
    fd_orig = open(head->u_st_mpiServer_msg.op_preload.storage_path, O_RDONLY);
    if (fd_orig == -1)
    {
        perror("Error on open operation\n");
        return -1;
    }

    char *protocol;
    char *user;
    char *machine;
    int  port;
    char *file;
    char *relative;
    char *params1;

    ret = URLSTR_ParseURL(head->u_st_mpiServer_msg.op_preload.virtual_path, &protocol, &user, &machine, &port, &file, &relative, &params1) ;   

    //Create new file
    fd_dest = creat(file, 0777);
    if (fd_dest == -1)
    {
        perror("Error on creat operation\n");
        close(fd_orig);
        return -1;
    }


    int cont = BLOCKSIZE * params->rank;
    int read_bytes, write_bytes;

    do{

        ret = lseek (fd_orig, cont, SEEK_SET);
        if (ret == -1)
        {
            perror("Error on lseek operation\n");
            close(fd_orig);
            close(fd_dest);
            return -1;
        }

        read_bytes = read(fd_orig, &buffer, BLOCKSIZE);
        if(read_bytes == -1){
            perror("Error on read operation\n");
            close(fd_orig);
            close(fd_dest);
            return -1;
        }

        if (read_bytes > 0)
        {
            write_bytes = write(fd_dest, &buffer, read_bytes);
            if(write_bytes==-1){
                perror("Error on read operation\n");
                close(fd_orig);
                close(fd_dest);
                return -1;
            }
        }

        cont = cont + (BLOCKSIZE * params->size);

    }while(read_bytes == BLOCKSIZE);

    close(fd_orig);
    close(fd_dest);



    mpiServer_comm_write_data(params, sd, (char *)&ret, sizeof(int), rank_client_id);

    // show debug info
    debug_info("[OPS] (ID=%s) PRELOAD(%s,%s) -> %d\n",  params->srv_name,
                                                        head->u_st_mpiServer_msg.op_preload.virtual_path,
                                                        head->u_st_mpiServer_msg.op_preload.storage_path,
                                                        ret);
    return 0;
  }
  
  void mpiServer_op_flush ( mpiServer_param_st *params, MPI_Comm sd, struct st_mpiServer_msg *head, int rank_client_id)
  {
    int ret;
    int  fd_dest, fd_orig;

    int BLOCKSIZE = head->u_st_mpiServer_msg.op_flush.block_size;
    char buffer [BLOCKSIZE];
    char path [PATH_MAX];

    char *protocol;
    char *user;
    char *machine;
    int  port;
    char *file;
    char *relative;
    char *params1;

    ret = URLSTR_ParseURL(head->u_st_mpiServer_msg.op_flush.virtual_path, &protocol, &user, &machine, &port, &file, &relative, &params1) ;  


    //Open origin file
    fd_orig = open(file, O_RDONLY);
    if (fd_orig == -1)
    {
        perror("open: ");
        printf("Error on open operation on '%s'\n", file);
        return -1;
    }


    //Create new file
    fd_dest = open(head->u_st_mpiServer_msg.op_flush.storage_path, O_WRONLY | O_CREAT, 0755);
    if (fd_dest == -1)
    {
        perror("open: ");
        printf("Error on open operation on '%s'\n", head->u_st_mpiServer_msg.op_flush.storage_path);
        return -1;
    }

    MPI_Barrier(MPI_COMM_WORLD);

    int cont = BLOCKSIZE * params->rank;
    int read_bytes, write_bytes;

    do{
        read_bytes = read(fd_orig, &buffer, BLOCKSIZE);
        if(read_bytes == -1){
            perror("Error on read operation\n");
            return -1;
        }

        if (read_bytes > 0)
        {
            lseek (fd_dest, cont, SEEK_SET);

            write_bytes = write(fd_dest, &buffer, read_bytes);
            if(write_bytes==-1){
                perror("Error on read operation\n");
                return -1;
            }
        }

        cont = cont + (BLOCKSIZE * params->size);

    }while(read_bytes == BLOCKSIZE);

    close(fd_orig);
    close(fd_dest);

    mpiServer_comm_write_data(params, sd, (char *)&ret, sizeof(int), rank_client_id) ;

    // show debug info
    debug_info("[OPS] (ID=%s) FLUSH(%s)\n", params->srv_name, head->u_st_mpiServer_msg.op_flush.virtual_path);
  }



  void mpiServer_op_getid ( mpiServer_param_st *params, MPI_Comm sd, struct st_mpiServer_msg *head, int rank_client_id)
  {
    // do getid
    mpiServer_comm_write_data(params, sd,(char *)head->id, MPISERVER_ID, rank_client_id);

    // show debug info
    debug_info("[OPS] (ID=%s) GETID(...)\n", params->srv_name);
  }