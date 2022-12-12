
/*
 *  Copyright 2020-2022 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
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

      #include "nfi_mpi_server.h"


  /* ... Global Variable / Variable Globales ........................... */

      int xpn_session = 0;


  /* ... Functions / Funciones ......................................... */

      /*
       * Communication
       */

      int mpi_server_write_operation ( MPI_Comm sd, struct st_mpi_server_msg *head )
      {
        int ret;

        debug_info("[NFI] (ID=%s) mpiClient_write_data: begin               HEAD_TYPE:%d\n", head->id, sizeof(head->type)) ;
        ret = mpiClient_write_operation(sd, (char *)&(head->type), 1, head->id) ;
        if (ret == -1){
            debug_warning("Server[?]: mpiClient_write_data fails :-(") ;
            return -1;
        }

        int rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank) ;

        debug_info("[NFI] (ID=%s) write_operation: %d -> \n", head->id, head->type) ;
        switch (head->type)
        {
          //File API
          case MPI_SERVER_OPEN_FILE_WS:
          case MPI_SERVER_OPEN_FILE_WOS:
                  debug_info("[NFI] (ID=%s) OPEN operation ---------------------  %ld\n", head->id, sizeof(struct st_mpi_server_open)) ;
                  ret = mpiClient_write_data(sd, (char *)&head->u_st_mpi_server_msg.op_open, sizeof(struct st_mpi_server_open), head->id) ;
                  break;
          case MPI_SERVER_CREAT_FILE_WS:
          case MPI_SERVER_CREAT_FILE_WOS:
                  debug_info("[NFI] (ID=%s) CREAT operation\n", head->id) ;
                  ret = mpiClient_write_data(sd, (char *)&head->u_st_mpi_server_msg.op_creat, sizeof(struct st_mpi_server_creat), head->id) ;
                  break;
          case MPI_SERVER_READ_FILE_WS:
          case MPI_SERVER_READ_FILE_WOS:
                  debug_info("[NFI] (ID=%s) READ operation\n", head->id) ;
                  ret = mpiClient_write_data(sd, (char *)&head->u_st_mpi_server_msg.op_read, sizeof(struct st_mpi_server_read), head->id) ;
                  break;
          case MPI_SERVER_WRITE_FILE_WS:
          case MPI_SERVER_WRITE_FILE_WOS:
                  debug_info("[NFI] (ID=%s) WRITE operation\n", head->id) ;
                  ret = mpiClient_write_data(sd, (char *)&head->u_st_mpi_server_msg.op_write, sizeof(struct st_mpi_server_write), head->id) ;
                  break;
          case MPI_SERVER_CLOSE_FILE_WS:
                  debug_info("[NFI] (ID=%s) CLOSE operation\n", head->id) ;
                  ret = mpiClient_write_data(sd, (char *)&head->u_st_mpi_server_msg.op_close, sizeof(struct st_mpi_server_close), head->id) ;
                  break;
          case MPI_SERVER_RM_FILE:
                  debug_info("[NFI] (ID=%s) RM operation\n", head->id) ;
                  ret = mpiClient_write_data(sd, (char *)&head->u_st_mpi_server_msg.op_rm, sizeof(struct st_mpi_server_rm), head->id) ;
                  break;
          case MPI_SERVER_RENAME_FILE:
                  debug_info("[NFI] (ID=%s) RENAME operation\n", head->id) ;
                  ret = mpiClient_write_data(sd, (char *)&head->u_st_mpi_server_msg.op_rename, sizeof(struct st_mpi_server_rename), head->id) ;
                  break;
          case MPI_SERVER_GETATTR_FILE:
                  debug_info("[NFI] (ID=%s) GETATTR operation\n", head->id) ;
                  ret = mpiClient_write_data(sd, (char *)&head->u_st_mpi_server_msg.op_getattr, sizeof(struct st_mpi_server_getattr), head->id) ;
                  break;

          //Directory API
          case MPI_SERVER_MKDIR_DIR:
                  debug_info("[NFI] (ID=%s) MDKIR operation\n", head->id) ;
                  ret = mpiClient_write_data(sd, (char *)&head->u_st_mpi_server_msg.op_mkdir, sizeof(struct st_mpi_server_mkdir), head->id) ;
                  break;
          case MPI_SERVER_OPENDIR_DIR:
                  debug_info("[NFI] (ID=%s) OPENDIR operation\n", head->id) ;
                  ret = mpiClient_write_data(sd, (char *)&head->u_st_mpi_server_msg.op_opendir, sizeof(struct st_mpi_server_opendir), head->id) ;
                  break;
          case MPI_SERVER_READDIR_DIR:
                  debug_info("[NFI] (ID=%s) READDIR operation\n", head->id) ;
                  ret = mpiClient_write_data(sd, (char *)&head->u_st_mpi_server_msg.op_readdir, sizeof(struct st_mpi_server_readdir), head->id) ;
                  break;
          case MPI_SERVER_CLOSEDIR_DIR:
                  debug_info("[NFI] (ID=%s) CLOSEDIR operation\n", head->id) ;
                  ret = mpiClient_write_data(sd, (char *)&head->u_st_mpi_server_msg.op_closedir, sizeof(struct st_mpi_server_closedir), head->id) ;
                  break;
          case MPI_SERVER_RMDIR_DIR:
                  debug_info("[NFI] (ID=%s) RMDIR operation\n", head->id) ;
                  ret = mpiClient_write_data(sd, (char *)&head->u_st_mpi_server_msg.op_rmdir, sizeof(struct st_mpi_server_rmdir), head->id) ;
                  break;

          //Optimization API
          case MPI_SERVER_FLUSH_FILE:
                  debug_info("[NFI] (ID=%s) FLUSH operation\n", head->id) ;
                  ret = mpiClient_write_data(sd, (char *)&head->u_st_mpi_server_msg.op_flush, sizeof(struct st_mpi_server_flush), head->id) ;
                  break;
          case MPI_SERVER_PRELOAD_FILE:
                  debug_info("[NFI] (ID=%s) PRELOAD operation\n", head->id) ;
                  ret = mpiClient_write_data(sd, (char *)&head->u_st_mpi_server_msg.op_preload, sizeof(struct st_mpi_server_preload), head->id) ;
                  break;
        }

        return ret;
      }

      int nfi_mpi_server_doRequest ( struct nfi_mpi_server_server *server_aux, struct st_mpi_server_msg *msg, char *req, int req_size )
      {
        ssize_t ret ;

        // send request...
        debug_info("[NFI] (ID=%s): %s: -> ...\n", server_aux->id, msg->id) ;
        ret = mpi_server_write_operation(server_aux->params.server, msg) ;
        if (ret < 0) {
          return -1 ;
        }

        // read response...
        debug_info("[NFI] (ID=%s): %s: <- ...\n", server_aux->id, msg->id) ;
        ret = mpiClient_read_data(server_aux->params.server, req, req_size, msg->id) ;
        if (ret < 0) {
          return -1 ;
        }

        // return OK
        return 0 ;
      }

      int nfi_mpi_server_keepConnected ( struct nfi_server *serv )
      {
        if (NULL == serv) 
	    return -1;

#ifdef NFI_DYNAMIC
        if (serv->private_info == NULL)
        {
          int ret = nfi_mpi_server_reconnect(serv) ;
          if (ret < 0)
          {
            /* mpi_server_err() ; not necessary */
            serv->private_info = NULL;
            return -1;
          }
        }
#endif

        // return OK
        return (serv->private_info != NULL) ;
      }


      /*
       *  PRIVATE FUNCTIONS TO USE mpi_server SERVERS
       */

      void NFItoMPI_SERVERattr ( struct stat *att, struct nfi_attr *nfi_att )
      {
        if (nfi_att->at_type == NFIFILE){
          att->st_mode = nfi_att->at_mode | S_IFREG;     /* protection */
        }

        if (nfi_att->at_type == NFIDIR){
          att->st_mode = nfi_att->at_mode | S_IFDIR;     /* protection */
        }

        att->st_size         = nfi_att->at_size;      /* size */
        att->st_uid          = nfi_att->at_uid;       /* user ID of owner */
        att->st_gid          = nfi_att->at_gid;       /* group ID of owner */
        att->st_blksize      = nfi_att->at_blksize;   /* blocksize for filesystem I/O */
        att->st_blocks       = nfi_att->at_blocks;    /* number of blocks allocated */
        att->st_atime        = nfi_att->at_atime;     /* time of last access */
        att->st_mtime        = nfi_att->at_mtime;     /* time of last modification */
        att->st_ctime        = nfi_att->at_ctime;     /* time of last change */
      }

      void MPI_SERVERtoNFIattr ( struct nfi_attr *nfi_att, struct stat *att )
      {
        if (S_ISREG(att->st_mode)) {
          nfi_att->at_type = NFIFILE;
        }
        if (S_ISDIR(att->st_mode)) {
          nfi_att->at_type = NFIDIR;
        }

        nfi_att->at_size    = att->st_size;      /* size */
        nfi_att->at_mode    = att->st_mode&(S_IRWXU|S_IRWXG|S_IRWXO) ; /* protection */
        nfi_att->at_uid     = att->st_uid;      /* user ID of owner */
        nfi_att->at_gid     = att->st_gid;      /* group ID of owner */
        nfi_att->at_blksize = att->st_blksize;  /* blocksize for filesystem I/O*/
        nfi_att->at_blocks  = att->st_blocks;   /* number of blocks allocated */
        nfi_att->at_atime   = att->st_atime;    /* time of last access */
        nfi_att->at_mtime   = att->st_mtime;    /* time of last modification */
        nfi_att->at_ctime   = att->st_ctime;    /* time of last change */
      }

      void MPI_SERVERtoNFIInfo(struct nfi_info *nfi_inf, struct nfi_info *mpi_server_inf)
      {
        //TODO
      }


      /*
       * PUBLIC FUNCTIONS TO USE MPI_SERVER
       */

       /************************************************************
        * Init Local                                                *
        ************************************************************/
      int nfi_mpi_server_init ( char *url, struct nfi_server *serv, struct nfi_attr_server *attr )
      {
        int ret ;
        struct nfi_mpi_server_server *server_aux;
        char server[MAXPATHLEN], dir[MAXPATHLEN], prt[MAXPATHLEN];

        debug_info("[NFI] nfi_mpi_server_init: begin\n") ;

        // new nfi_ops with mpi_server functions...
        serv->ops = (struct nfi_ops *)malloc(sizeof(struct nfi_ops)) ;
        NULL_RET_ERR(serv->ops, MPI_SERVERERR_MEMORY) ;

        bzero(serv->ops, sizeof(struct nfi_ops)) ;
        serv->ops->nfi_reconnect      = nfi_mpi_server_reconnect;
        serv->ops->nfi_disconnect     = nfi_mpi_server_disconnect;

        serv->ops->nfi_open           = nfi_mpi_server_open;
        serv->ops->nfi_create         = nfi_mpi_server_create;
        serv->ops->nfi_read           = nfi_mpi_server_read;
        serv->ops->nfi_write          = nfi_mpi_server_write;
        serv->ops->nfi_close          = nfi_mpi_server_close;
        serv->ops->nfi_remove         = nfi_mpi_server_remove;
        serv->ops->nfi_rename         = nfi_mpi_server_rename;
        serv->ops->nfi_getattr        = nfi_mpi_server_getattr;
        serv->ops->nfi_setattr        = nfi_mpi_server_setattr;

        serv->ops->nfi_opendir        = nfi_mpi_server_opendir;
        serv->ops->nfi_mkdir          = nfi_mpi_server_mkdir;
        serv->ops->nfi_readdir        = nfi_mpi_server_readdir;
        serv->ops->nfi_closedir       = nfi_mpi_server_closedir;
        serv->ops->nfi_rmdir          = nfi_mpi_server_rmdir;

        serv->ops->nfi_preload        = nfi_mpi_server_preload;
        serv->ops->nfi_flush          = nfi_mpi_server_flush;

        serv->ops->nfi_statfs         = nfi_mpi_server_statfs;

        // parse url...
        ret = ParseURL(url, prt, NULL, NULL, server, NULL, dir) ;
        if (ret < 0)
        {
          mpi_server_err(MPI_SERVERERR_URL) ;
          debug_error("nfi_mpi_server_init: incorrect url '%s'.\n", url) ;
          free(serv->ops) ;
          return -1;
        }

        // new nfi_mpiserver_server...
        server_aux = (struct nfi_mpi_server_server *)malloc(sizeof(struct nfi_mpi_server_server)) ;
        if (server_aux == NULL) {
          mpi_server_err(MPI_SERVERERR_MEMORY) ;
          free(serv->ops) ;
          return -1;
        }
        serv->private_info = (void *)server_aux;

        // Session mode checking
        char * env_session = getenv("XPN_SESSION");
        if (env_session != NULL)
        {
          xpn_session = atoi(env_session);
        }

        // initialize MPI Client communication side...
        ret = mpiClient_comm_init(&(server_aux->params)) ;
        if (ret < 0) {
          free(serv->ops) ;
          free(server_aux) ;
          return -1 ;
        }

        ret = nfi_mpi_server_connect(serv, url, prt, server, dir) ;
        if (ret < 0) {
          free(serv->ops) ;
          free(server_aux) ;
          return -1 ;
        }

        ret = mpiClient_comm_locality (&(server_aux->params)) ;
        if (ret < 0) {
          free(serv->ops) ;
          free(server_aux) ;
          return -1 ;
        }

        debug_info("[NFI] nfi_mpi_server_init(ID=%s): end\n",server_aux->id) ;

        // return OK
        return 0;
      }


      /*********************************************************
       * Connect to the server                                 *
       * *******************************************************/
      int nfi_mpi_server_connect(struct nfi_server *serv, char *url, char* prt, char* server, char* dir)
      {
        int ret ;
        struct nfi_mpi_server_server *server_aux;

        server_aux = (struct nfi_mpi_server_server *) (serv->private_info) ;
        strcpy(server_aux->params.srv_name, server) ;

        ret = mpiClient_comm_connect(&(server_aux->params)) ;
        if (ret < 0) {
            return -1 ;
        }

        // copy 'url' string...
        serv->url = strdup(url) ;
        NULL_RET_ERR(serv->url, MPI_SERVERERR_MEMORY) ;

        // new server wrk...
        serv->wrk = (struct nfi_worker *)malloc(sizeof(struct nfi_worker)) ;
        memset(serv->wrk, 0, sizeof(struct nfi_worker)) ;

        if (strcmp("mpi_server", prt) == 0)
        {
          debug_info("[NFI] mpi_server\n") ;
          debug_info("[NFI] nfi_worker_init(1,ID=%s): \n",server_aux->id) ;
          nfi_worker_init(serv->wrk, serv, 1) ;
        }
        else
        {
          debug_info("[NFI] nfi_worker_init(0,ID=%s): \n",server_aux->id) ;
          nfi_worker_init(serv->wrk, serv, 0) ;
        }

        return 0;
      }


      /************************************************************
       * Disconnect to the server                                 *
       * **********************************************************/
      int nfi_mpi_server_disconnect ( struct nfi_server *serv )
      {
        int ret ;
        struct nfi_mpi_server_server *server_aux;

        debug_info("[NFI] nfi_mpi_server_disconnect(): begin\n") ;

        // check params...
        if (serv == NULL) {
          return 0;
        }
        server_aux = (struct nfi_mpi_server_server *)serv->private_info;
        if (server_aux == NULL) {
          return 0;
        }

        // MPI Disconnect...
        ret = mpiClient_comm_disconnect(&(server_aux->params)) ;
        if (ret < 0) {
          debug_error("[NFI]: mpiClient_comm_disconnect fails :-(") ;
        }

        // MPI Finalize...
        ret = mpiClient_comm_destroy ( &(server_aux->params) );
        if (ret < 0) {
          debug_error("[NFI]: mpiClient_comm_destroy fails :-(") ;
        }

        // free private_info, 'url' string and 'server' string...
        FREE_AND_NULL(serv->private_info) ;
        FREE_AND_NULL(serv->url) ;
        FREE_AND_NULL(serv->server) ;

        //serv->protocol = -1;
        debug_info("[NFI] nfi_mpi_server_disconnect(): end\n") ;

        // return OK
        return 0;
      }


      /************************************************************
       * Reconnect to the MPI server                              *
       ************************************************************/
      int nfi_mpi_server_reconnect(struct nfi_server *serv)
      {
        /* Don't see the serv result */
        char   server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
        int    ret;
        struct nfi_mpi_server_server *server_aux;

        // ParseURL...
        ret = ParseURL(serv->url,  NULL, NULL, NULL, server,  NULL,  dir) ;
        if (ret < 0) {
          mpi_server_err(MPI_SERVERERR_URL) ;
          debug_error("nfi_mpi_server_reconnect: url %s incorrect.\n", serv->url) ;
          return -1;
        }

        // get private_info...
        server_aux = (struct nfi_mpi_server_server *)malloc(sizeof(struct nfi_mpi_server_server)) ;
        NULL_RET_ERR(server_aux, MPI_SERVERERR_MEMORY) ;

        strcpy(server_aux->path, dir) ;
        serv->private_info = (void *)server_aux;

        // reconnect...
        ret = mpiClient_comm_connect(&(server_aux->params)) ;
        if (ret < 0) {
          debug_error("nfi_mpi_server_reconnect: mpiClient_comm_connect fails.\n") ;
          return -1;
        }

        // return OK
        return 0;
      }




      /*
       *  GENERIC FUNCTIONS
       */

      int nfi_mpi_server_open ( struct nfi_server *serv,  char *url, struct nfi_fhandle *fho )
      {
        int nfi_mpi_server_open_ws  ( struct nfi_server *serv,  char *url, struct nfi_fhandle *fho );
        int nfi_mpi_server_open_wos ( struct nfi_server *serv,  char *url, struct nfi_fhandle *fho );

        if (xpn_session)
        {
          return nfi_mpi_server_open_ws ( serv, url, fho );
        }
        else
        {
          return nfi_mpi_server_open_wos ( serv, url, fho );
        }
      }

      int nfi_mpi_server_open_ws ( struct nfi_server *serv,  char *url, struct nfi_fhandle *fho )
      {
        char   dir[NFIMAXPATHLEN], server[NFIMAXPATHLEN];
        int    ret ;
        struct nfi_mpi_server_server *server_aux;
        struct nfi_mpi_server_fhandle *fh_aux;
        struct st_mpi_server_msg msg;

        // Check arguments...
        NULL_RET_ERR(serv, MPI_SERVERERR_PARAM) ;
        NULL_RET_ERR(fho,  MPI_SERVERERR_PARAM) ;
        nfi_mpi_server_keepConnected(serv) ;
        NULL_RET_ERR(serv->private_info, MPI_SERVERERR_PARAM) ;

        // private_info...
        server_aux = (struct nfi_mpi_server_server *) serv->private_info;
        debug_info("[NFI] nfi_mpi_server_open(ID=%s): begin %s\n",server_aux->id,url) ;

        /*if (url[strlen(url)-1] == '/'){
          return nfi_mpi_server_opendir(serv, url, fho) ;
        }*/

        ret = ParseURL(url, NULL, NULL, NULL, server,  NULL,  dir) ;
        if (ret < 0) {
          fprintf(stderr,"nfi_mpi_server_open: url %s incorrect.\n",url) ;
          mpi_server_err(MPI_SERVERERR_URL) ;
          return -1;
        }
        fho->url = strdup(url) ;
        NULL_RET_ERR(fho->url, MPI_SERVERERR_MEMORY) ;

        fh_aux = (struct nfi_mpi_server_fhandle *)malloc(sizeof(struct nfi_mpi_server_fhandle)) ;
        if (fh_aux == NULL) {
          mpi_server_err(MPI_SERVERERR_MEMORY) ;
          free(fho->url) ;
          return -1;
        }

        server_aux = (struct nfi_mpi_server_server *) serv->private_info;

        /************** LOCAL *****************/
        if(server_aux->params.locality)
        {
          fh_aux->fd = filesystem_open2(dir, O_RDWR, S_IRWXU) ;
          if (fh_aux->fd < 0)
          {
            debug_error("filesystem_open fails to open '%s' in server %s.\n", dir, serv->server) ;
            free(fh_aux) ;
            free(fho->url) ;
            return -1;
          }
          strcpy(fh_aux->path, dir) ;
        }
        /************** SERVER ****************/
        else {
          msg.type = MPI_SERVER_OPEN_FILE_WS;
          strcpy(msg.id, server_aux->id) ;
          strcpy(msg.u_st_mpi_server_msg.op_open.path,dir) ;

          nfi_mpi_server_doRequest(server_aux, &msg, (char *)&(fh_aux->fd), sizeof(int)) ;

          if (fh_aux->fd < 0)
          {
            debug_error("filesystem_open fails to open '%s' in server %s.\n", dir, serv->server) ;
            free(fh_aux) ;
            free(fho->url) ;
            return -1;
          }
          strcpy(fh_aux->path, dir) ;
        }
        /*****************************************/

        fho->type = NFIFILE;
        fho->server = NULL;
        fho->priv_fh = NULL;
        fho->server = serv;
        fho->priv_fh = (void *) fh_aux;

        debug_info("[NFI] nfi_mpi_server_open(ID=%s): end\n",server_aux->id) ;

        return 0;
      }

      int nfi_mpi_server_open_wos ( struct nfi_server *serv,  char *url, struct nfi_fhandle *fho )
      {
        char   dir[NFIMAXPATHLEN], server[NFIMAXPATHLEN];
        int    ret ;
        struct nfi_mpi_server_server *server_aux;
        struct nfi_mpi_server_fhandle *fh_aux;
        struct st_mpi_server_msg msg;

        // Check arguments...
        NULL_RET_ERR(serv, MPI_SERVERERR_PARAM) ;
        NULL_RET_ERR(fho,  MPI_SERVERERR_PARAM) ;
        nfi_mpi_server_keepConnected(serv) ;
        NULL_RET_ERR(serv->private_info, MPI_SERVERERR_PARAM) ;

        // private_info...
        server_aux = (struct nfi_mpi_server_server *) serv->private_info;
        debug_info("[NFI] nfi_mpi_server_open(ID=%s): begin %s\n",server_aux->id,url) ;

        /*if (url[strlen(url)-1] == '/'){
          return nfi_mpi_server_opendir(serv, url, fho) ;
        }*/

        ret = ParseURL(url, NULL, NULL, NULL, server,  NULL,  dir) ;
        if (ret < 0) {
          fprintf(stderr,"nfi_mpi_server_open: url %s incorrect.\n",url) ;
          mpi_server_err(MPI_SERVERERR_URL) ;
          return -1;
        }
        fho->url = strdup(url) ;
        NULL_RET_ERR(fho->url, MPI_SERVERERR_MEMORY) ;

        fh_aux = (struct nfi_mpi_server_fhandle *)malloc(sizeof(struct nfi_mpi_server_fhandle)) ;
        if (fh_aux == NULL) {
          mpi_server_err(MPI_SERVERERR_MEMORY) ;
          free(fho->url) ;
          return -1;
        }

        server_aux = (struct nfi_mpi_server_server *) serv->private_info;

        /************** LOCAL *****************/
        if(server_aux->params.locality)
        {
          fh_aux->fd = filesystem_open2(dir, O_RDWR, S_IRWXU) ;
          if (fh_aux->fd < 0)
          {
            debug_error("filesystem_open fails to open '%s' in server %s.\n", dir, serv->server) ;
            free(fh_aux) ;
            free(fho->url) ;
            return -1;
          }
          filesystem_close(fh_aux->fd);
          strcpy(fh_aux->path, dir) ;
        }
        /************** SERVER ****************/
        else {
          msg.type = MPI_SERVER_OPEN_FILE_WOS;
          strcpy(msg.id, server_aux->id) ;
          strcpy(msg.u_st_mpi_server_msg.op_open.path,dir) ;

          nfi_mpi_server_doRequest(server_aux, &msg, (char *)&(fh_aux->fd), sizeof(int)) ;

          if (fh_aux->fd < 0)
          {
            debug_error("filesystem_open fails to open '%s' in server %s.\n", dir, serv->server) ;
            free(fh_aux) ;
            free(fho->url) ;
            return -1;
          }
          strcpy(fh_aux->path, dir) ;
        }
        /*****************************************/

        fho->type = NFIFILE;
        fho->server = NULL;
        fho->priv_fh = NULL;
        fho->server = serv;
        fho->priv_fh = (void *) fh_aux;

        debug_info("[NFI] nfi_mpi_server_open(ID=%s): end\n",server_aux->id) ;

        return 0;
      }

      int nfi_mpi_server_create(struct nfi_server *serv,  char *url, struct nfi_attr *attr, struct nfi_fhandle  *fh)
      {
        int nfi_mpi_server_create_ws  (struct nfi_server *serv,  char *url, struct nfi_attr *attr, struct nfi_fhandle  *fh);
        int nfi_mpi_server_create_wos (struct nfi_server *serv,  char *url, struct nfi_attr *attr, struct nfi_fhandle  *fh);

        if (xpn_session)
        {
          return nfi_mpi_server_create_ws( serv, url, attr, fh );
        }
        else
        {
          return nfi_mpi_server_create_wos( serv, url, attr, fh );
        }
      }

      int nfi_mpi_server_create_ws (struct nfi_server *serv,  char *url, struct nfi_attr *attr, struct nfi_fhandle  *fh)
      {
        char   server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
        int    ret ;
        struct nfi_mpi_server_server *server_aux;
        struct nfi_mpi_server_fhandle *fh_aux;
        struct st_mpi_server_msg msg;

        // Check arguments...
        NULL_RET_ERR(serv, MPI_SERVERERR_PARAM) ;
        NULL_RET_ERR(attr, MPI_SERVERERR_PARAM) ;
        nfi_mpi_server_keepConnected(serv) ;
        NULL_RET_ERR(serv->private_info, MPI_SERVERERR_PARAM) ;

        // private_info...
        server_aux = (struct nfi_mpi_server_server *) serv->private_info;
        debug_info("[NFI] nfi_mpi_server_create(ID=%s): begin %s\n",server_aux->id,url) ;

        ret = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir) ;
        if (ret < 0) {
          fprintf(stderr,"ERROR: nfi_mpi_server_create: url %s incorrect.\n",url) ;
          mpi_server_err(MPI_SERVERERR_URL) ;
          return -1;
        }

        /* private_info file handle */
        fh_aux = (struct nfi_mpi_server_fhandle *)malloc(sizeof(struct nfi_mpi_server_fhandle)) ;
        NULL_RET_ERR(fh_aux, MPI_SERVERERR_MEMORY) ;

        bzero(fh_aux, sizeof(struct nfi_mpi_server_fhandle)) ;

        server_aux = (struct nfi_mpi_server_server *) serv->private_info;
        /* create the file into the directory */

        /************** LOCAL *****************/
        if(server_aux->params.locality)        {
          fh_aux->fd = filesystem_open2(dir, O_CREAT|O_RDWR|O_TRUNC, attr->at_mode) ;
          if (fh_aux->fd < 0) {
            debug_error("files_posix_open fails to creat '%s' in server '%s'.\n", dir, serv->server) ;
            free(fh_aux) ;
            return -1;
          }
          strcpy(fh_aux->path, dir) ;
        }
        /************** SERVER ****************/
        else {
          msg.type = MPI_SERVER_CREAT_FILE_WS;
          strcpy(msg.id, server_aux->id) ;
          strcpy(msg.u_st_mpi_server_msg.op_creat.path,dir) ;
          nfi_mpi_server_doRequest(server_aux, &msg, (char *)&(fh_aux->fd), sizeof(int)) ;
          strcpy(fh_aux->path, dir) ;
        }
        /*****************************************/

        fh->type = NFIFILE;
        fh->server = serv;
        fh->priv_fh = (void *)fh_aux;

        fh->url = strdup(url) ;
        if (fh->url == NULL) {
          mpi_server_err(MPI_SERVERERR_MEMORY) ;
          free(fh_aux) ;
          return -1;
        }

        debug_info("[NFI] nfi_mpi_server_create(ID=%s): end\n",server_aux->id) ;

        return 0;
      }

      int nfi_mpi_server_create_wos (struct nfi_server *serv,  char *url, struct nfi_attr *attr, struct nfi_fhandle  *fh)
      {
        char   server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
        int    ret ;
        struct nfi_mpi_server_server *server_aux;
        struct nfi_mpi_server_fhandle *fh_aux;
        struct st_mpi_server_msg msg;

        // Check arguments...
        NULL_RET_ERR(serv, MPI_SERVERERR_PARAM) ;
        NULL_RET_ERR(attr, MPI_SERVERERR_PARAM) ;
        nfi_mpi_server_keepConnected(serv) ;
        NULL_RET_ERR(serv->private_info, MPI_SERVERERR_PARAM) ;

        // private_info...
        server_aux = (struct nfi_mpi_server_server *) serv->private_info;
        debug_info("[NFI] nfi_mpi_server_create(ID=%s): begin %s\n",server_aux->id,url) ;

        ret = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir) ;
        if (ret < 0) {
          fprintf(stderr,"ERROR: nfi_mpi_server_create: url %s incorrect.\n",url) ;
          mpi_server_err(MPI_SERVERERR_URL) ;
          return -1;
        }

        /* private_info file handle */
        fh_aux = (struct nfi_mpi_server_fhandle *)malloc(sizeof(struct nfi_mpi_server_fhandle)) ;
        NULL_RET_ERR(fh_aux, MPI_SERVERERR_MEMORY) ;

        bzero(fh_aux, sizeof(struct nfi_mpi_server_fhandle)) ;

        server_aux = (struct nfi_mpi_server_server *) serv->private_info;
        /* create the file into the directory */

        /************** LOCAL *****************/
        if(server_aux->params.locality)        {
          fh_aux->fd = filesystem_open2(dir, O_CREAT|O_RDWR|O_TRUNC, attr->at_mode) ;
          if (fh_aux->fd < 0) {
            debug_error("files_posix_open fails to creat '%s' in server '%s'.\n", dir, serv->server) ;
            free(fh_aux) ;
            return -1;
          }
          filesystem_close(fh_aux->fd);
          strcpy(fh_aux->path, dir) ;
        }
        /************** SERVER ****************/
        else {
          msg.type = MPI_SERVER_CREAT_FILE_WOS;
          strcpy(msg.id, server_aux->id) ;
          strcpy(msg.u_st_mpi_server_msg.op_creat.path,dir) ;
          nfi_mpi_server_doRequest(server_aux, &msg, (char *)&(fh_aux->fd), sizeof(int)) ;
          strcpy(fh_aux->path, dir) ;
        }
        /*****************************************/

        fh->type = NFIFILE;
        fh->server = serv;
        fh->priv_fh = (void *)fh_aux;

        fh->url = strdup(url) ;
        if (fh->url == NULL) {
          mpi_server_err(MPI_SERVERERR_MEMORY) ;
          free(fh_aux) ;
          return -1;
        }

        debug_info("[NFI] nfi_mpi_server_create(ID=%s): end\n",server_aux->id) ;

        return 0;
      }

      ssize_t nfi_mpi_server_read ( struct nfi_server *serv,
                                 struct nfi_fhandle *fh,
                                 void *buffer,
                                 off_t offset,
                                 size_t size )
      {
        ssize_t nfi_mpi_server_read_ws  ( struct nfi_server *serv, struct nfi_fhandle *fh, void *buffer, off_t offset, size_t size );
        ssize_t nfi_mpi_server_read_wos ( struct nfi_server *serv, struct nfi_fhandle *fh, void *buffer, off_t offset, size_t size );

        if (xpn_session)
        {
          return nfi_mpi_server_read_ws ( serv, fh, buffer, offset, size );
        }
        else
        {
          return nfi_mpi_server_read_wos ( serv, fh, buffer, offset, size );
        }
      }

      ssize_t nfi_mpi_server_read_ws ( struct nfi_server *serv,
                                      struct nfi_fhandle *fh,
                                      void *buffer,
                                      off_t offset,
                                      size_t size )
      {

        int ret, cont, diff;
        struct nfi_mpi_server_server *server_aux;
        struct nfi_mpi_server_fhandle *fh_aux;
        struct st_mpi_server_msg msg;
        struct st_mpi_server_read_req req;

        // Check arguments...
        NULL_RET_ERR(serv, MPI_SERVERERR_PARAM) ;
        NULL_RET_ERR(fh,   MPI_SERVERERR_PARAM) ;
        nfi_mpi_server_keepConnected(serv) ;
        NULL_RET_ERR(serv->private_info, MPI_SERVERERR_PARAM) ;

        // private_info
        server_aux = (struct nfi_mpi_server_server *) serv->private_info;
        debug_info("[NFI] nfi_mpi_server_read(%s): begin off %d size %d\n",server_aux->id,(int)offset, (int)size) ;

        fh_aux = (struct nfi_mpi_server_fhandle *) fh->priv_fh;

        /************** LOCAL *****************/
        if(server_aux->params.locality)
        {
          filesystem_lseek(fh_aux->fd, offset, SEEK_SET) ;

          if(server_aux->params.sem_server != 0) sem_wait(server_aux->params.sem_server);
          ret = filesystem_read(fh_aux->fd, buffer, size) ;
          if(server_aux->params.sem_server != 0) sem_post(server_aux->params.sem_server);

          debug_info("read %s(%d) off %ld size %zu (ret:%zd)", fh->url, fh_aux->fd, (long int)offset, size, ret)
          if (ret < 0) {
            debug_error("filesystem_read reads zero bytes from url:%s offset:%ld size:%zu (ret:%zd) errno=%d\n", fh->url, (long int)offset, size, ret, errno) ;
            return -1;
          }
        }
        /************** SERVER ****************/
        else {

          msg.type = MPI_SERVER_READ_FILE_WS;
          strcpy(msg.id, server_aux->id) ;
          msg.u_st_mpi_server_msg.op_read.fd       = fh_aux->fd;
          msg.u_st_mpi_server_msg.op_read.offset   = offset;
          msg.u_st_mpi_server_msg.op_read.size     = size;

          #ifdef  DBG_IO
            printf("[NFI]read: -> fd %d \n",msg.u_st_mpi_server_msg.op_read.fd) ;
            printf("[NFI]read: -> offset %d \n",(int)msg.u_st_mpi_server_msg.op_read.offset) ;
            printf("[NFI]read: -> size %d \n",msg.u_st_mpi_server_msg.op_read.size) ;
          #endif

          //ret = mpi_server_write_operation(server_aux->sd, &msg) ;
          ret = mpi_server_write_operation(server_aux->params.server, &msg) ;
          if(ret == -1){
            perror("ERROR: (1)nfi_mpi_server_read: Error on write operation") ;
            fprintf(stderr,"ERROR: (1)nfi_mpi_server_read: Error on write operation\n") ;
            return -1;
          }

          // read n times: number of bytes + read data (n bytes)
          cont = 0 ;

          do {
            ret = mpiClient_read_data(server_aux->params.server, (char *)&req, sizeof(struct st_mpi_server_read_req), msg.id) ;
            debug_info("[NFI] nfi_mpi_server_read(ID=%s): (1)mpiClient_read_data = %d.\n",server_aux->id, ret) ;
            if(ret == -1){
              perror("ERROR: (2)nfi_mpi_server_read: Error on write operation") ;
              fprintf(stderr,"ERROR: (2)nfi_mpi_server_read: Error on write operation\n") ;
              return -1;
            }

            if(req.size > 0){
              debug_info("[NFI] nfi_mpi_server_read(ID=%s): (2)mpiClient_read_data = %d. size = %d\n",server_aux->id, ret, req.size) ;
              ret = mpiClient_read_data(server_aux->params.server, (char *)buffer+cont, req.size, msg.id) ;
              debug_info("[NFI] nfi_mpi_server_read(ID=%s): (2)mpiClient_read_data = %d.\n",server_aux->id, ret) ;
              if(ret == -1){
                perror("ERROR: (3)nfi_mpi_server_read: Error on write operation") ;
                fprintf(stderr,"ERROR: (3)nfi_mpi_server_read: Error on read operation\n") ;
              }
            }
            cont = cont + req.size ;
            diff = msg.u_st_mpi_server_msg.op_read.size - cont;

          } while ((diff > 0) && (req.size != 0)) ;

          if (req.size < 0)
          {
            fprintf(stderr,"ERROR: nfi_mpi_server_read: Fail read %s off %d size %d (err:%d).\n",fh->url,(int)offset,(int)size,(int)req.size) ;
            mpi_server_err(MPI_SERVERERR_READ) ;
            return -1;
          }

          ret = cont;
        }

        debug_info("[NFI] nfi_mpi_server_read(ID=%s): end\n",server_aux->id) ;

        return ret;
      }

      ssize_t nfi_mpi_server_read_wos ( struct nfi_server *serv,
                                       struct nfi_fhandle *fh,
                                       void *buffer,
                                       off_t offset,
                                       size_t size )
                                       //off_t offset)
      {

        int ret, cont, diff;
        struct nfi_mpi_server_server *server_aux;
        struct nfi_mpi_server_fhandle *fh_aux;
        struct st_mpi_server_msg msg;
        struct st_mpi_server_read_req req;

        // Check arguments...
        NULL_RET_ERR(serv, MPI_SERVERERR_PARAM) ;
        NULL_RET_ERR(fh,   MPI_SERVERERR_PARAM) ;
        nfi_mpi_server_keepConnected(serv) ;
        NULL_RET_ERR(serv->private_info, MPI_SERVERERR_PARAM) ;

        // private_info
        server_aux = (struct nfi_mpi_server_server *) serv->private_info;
        debug_info("[NFI] nfi_mpi_server_read(%s): begin off %d size %d\n",server_aux->id,(int)offset, (int)size) ;

        fh_aux = (struct nfi_mpi_server_fhandle *) fh->priv_fh;

        /************** LOCAL *****************/
        if(server_aux->params.locality)
        {
          int fd;
          fd = filesystem_open(fh_aux->path, O_RDONLY);
          if (fd < 0)
          {
            debug_error("filesystem_read reads zero bytes from url:%s offset:%ld size:%zu (ret:%zd) errno=%d\n", fh->url, (long int)offset, size, ret, errno) ;
            return -1;
          }
          filesystem_lseek(fd, offset, SEEK_SET) ;

          if(server_aux->params.sem_server != 0) sem_wait(server_aux->params.sem_server);
          ret = filesystem_read(fd, buffer, size) ;
          if(server_aux->params.sem_server != 0) sem_post(server_aux->params.sem_server);

          debug_info("read %s(%d) off %ld size %zu (ret:%zd)", fh->url, fd, (long int)offset, size, ret)
          if (ret < 0) {
            filesystem_close(fd);
            debug_error("filesystem_read reads zero bytes from url:%s offset:%ld size:%zu (ret:%zd) errno=%d\n", fh->url, (long int)offset, size, ret, errno) ;
            return -1;
          }
          filesystem_close(fd);
        }
        /************** SERVER ****************/
        else {

          msg.type = MPI_SERVER_READ_FILE_WOS;
          strcpy(msg.id, server_aux->id) ;
          //msg.u_st_mpi_server_msg.op_read.fd       = fh_aux->fd;
          strcpy(msg.u_st_mpi_server_msg.op_read.path, fh_aux->path);
          msg.u_st_mpi_server_msg.op_read.offset   = offset;
          msg.u_st_mpi_server_msg.op_read.size     = size;

          #ifdef  DBG_IO
            printf("[NFI]read: -> path %s \n",msg.u_st_mpi_server_msg.op_read.path) ;
            printf("[NFI]read: -> offset %d \n",(int)msg.u_st_mpi_server_msg.op_read.offset) ;
            printf("[NFI]read: -> size %d \n",msg.u_st_mpi_server_msg.op_read.size) ;
          #endif

          //ret = mpi_server_write_operation(server_aux->sd, &msg) ;
          ret = mpi_server_write_operation(server_aux->params.server, &msg) ;
          if(ret == -1){
            perror("ERROR: (1)nfi_mpi_server_read: Error on write operation") ;
            fprintf(stderr,"ERROR: (1)nfi_mpi_server_read: Error on write operation\n") ;
            return -1;
          }

          // read n times: number of bytes + read data (n bytes)
          cont = 0 ;

          do {
            ret = mpiClient_read_data(server_aux->params.server, (char *)&req, sizeof(struct st_mpi_server_read_req), msg.id) ;
            debug_info("[NFI] nfi_mpi_server_read(ID=%s): (1)mpiClient_read_data = %d.\n",server_aux->id, ret) ;
            if(ret == -1){
              perror("ERROR: (2)nfi_mpi_server_read: Error on write operation") ;
              fprintf(stderr,"ERROR: (2)nfi_mpi_server_read: Error on write operation\n") ;
              return -1;
            }

            if(req.size > 0){
              debug_info("[NFI] nfi_mpi_server_read(ID=%s): (2)mpiClient_read_data = %d. size = %d\n",server_aux->id, ret, req.size) ;
              ret = mpiClient_read_data(server_aux->params.server, (char *)buffer+cont, req.size, msg.id) ;
              debug_info("[NFI] nfi_mpi_server_read(ID=%s): (2)mpiClient_read_data = %d.\n",server_aux->id, ret) ;
              if(ret == -1){
                perror("ERROR: (3)nfi_mpi_server_read: Error on write operation") ;
                fprintf(stderr,"ERROR: (3)nfi_mpi_server_read: Error on read operation\n") ;
              }
            }
            cont = cont + req.size ;
            diff = msg.u_st_mpi_server_msg.op_read.size - cont;

          } while ((diff > 0) && (req.size != 0)) ;

          if (req.size < 0)
          {
            fprintf(stderr,"ERROR: nfi_mpi_server_read: Fail read %s off %d size %d (err:%d).\n",fh->url,(int)offset,(int)size,(int)req.size) ;
            mpi_server_err(MPI_SERVERERR_READ) ;
            return -1;
          }

          ret = cont;
        }

        debug_info("[NFI] nfi_mpi_server_read(ID=%s): end\n",server_aux->id) ;

        return ret;
      }

      ssize_t nfi_mpi_server_write( struct nfi_server *serv,
                                   struct nfi_fhandle *fh,
                                   void *buffer,
                                   off_t offset,
                                   size_t size )
      {
        ssize_t nfi_mpi_server_write_ws  ( struct nfi_server *serv, struct nfi_fhandle *fh, void *buffer, off_t offset, size_t size );
        ssize_t nfi_mpi_server_write_wos ( struct nfi_server *serv, struct nfi_fhandle *fh, void *buffer, off_t offset, size_t size );

        if (xpn_session)
        {
          return nfi_mpi_server_write_ws ( serv, fh, buffer, offset, size );
        }
        else
        {
          return nfi_mpi_server_write_wos ( serv, fh, buffer, offset, size );
        }
      }

      ssize_t nfi_mpi_server_write_ws ( struct nfi_server *serv,
                                       struct nfi_fhandle *fh,
                                       void *buffer,
                                       off_t offset,
                                       size_t size )
      {
        struct nfi_mpi_server_server *server_aux;
        struct nfi_mpi_server_fhandle *fh_aux;
        struct st_mpi_server_msg msg;
        struct st_mpi_server_write_req req;
        int ret, diff, cont;

        // Check arguments...
        if (size == 0){
          return 0;
        }

        NULL_RET_ERR(serv, MPI_SERVERERR_PARAM) ;
        NULL_RET_ERR(fh,   MPI_SERVERERR_PARAM) ;
        nfi_mpi_server_keepConnected(serv) ;
        NULL_RET_ERR(serv->private_info, MPI_SERVERERR_PARAM) ;

        // private_info...
        server_aux = (struct nfi_mpi_server_server *) serv->private_info;
        debug_info("[NFI] nfi_mpi_server_write(ID=%s): begin off %d size %d\n",server_aux->id,(int)offset, (int)size) ;

        fh_aux     = (struct nfi_mpi_server_fhandle *) fh->priv_fh;
        server_aux = (struct nfi_mpi_server_server  *) serv->private_info;

        /************** LOCAL *****************/
        if(server_aux->params.locality)
        {
          filesystem_lseek(fh_aux->fd, offset, SEEK_SET) ;

          if(server_aux->params.sem_server != 0) sem_wait(server_aux->params.sem_server);
          ret = filesystem_write(fh_aux->fd, buffer, size) ;
          debug_info("write %s(%d) off %ld size %zu (ret:%zd)", fh->url, fh_aux->fd, (long int)offset, size, ret);
          if(server_aux->params.sem_server != 0) sem_post(server_aux->params.sem_server);

          if (ret < 0) {
              debug_error("filesystem_read reads zero bytes from url:%s offset:%ld size:%zu (ret:%zd) errno=%d\n", fh->url, (long int)offset, size, ret, errno) ;
              return -1;
          }
        }
        /************** SERVER ****************/
        else {
          msg.type = MPI_SERVER_WRITE_FILE_WS;
          strcpy(msg.id, server_aux->id) ;
          msg.u_st_mpi_server_msg.op_write.fd     = fh_aux->fd;
          msg.u_st_mpi_server_msg.op_write.offset = offset;
          msg.u_st_mpi_server_msg.op_write.size   = size;

          #ifdef  DBG_IO
            printf("[NFI]write: -> fd %d \n",msg.u_st_mpi_server_msg.op_write.fd) ;
            printf("[NFI]write: -> offset %d \n",(int)msg.u_st_mpi_server_msg.op_write.offset) ;
            printf("[NFI]write: -> size %d \n",msg.u_st_mpi_server_msg.op_write.size) ;
          #endif

          ret = mpi_server_write_operation(server_aux->params.server, &msg) ;
          if(ret == -1){
            fprintf(stderr,"(1)ERROR: nfi_mpi_server_write(ID=%s): Error on write operation\n",server_aux->id) ;
            return -1;
          }

          diff = size;
          cont = 0;

          int buffer_size = size;

          // Max buffer size
          if (buffer_size > MAX_BUFFER_SIZE)
          {
              buffer_size = MAX_BUFFER_SIZE;
          }

          do{
            if (diff > buffer_size)
            {
              ret = mpiClient_write_data(server_aux->params.server, (char *)buffer + cont, buffer_size, msg.id) ;
              if(ret == -1){
                fprintf(stderr,"(2)ERROR: nfi_mpi_server_read(ID=%s): Error on write operation\n",server_aux->id) ;
              }
            }
            else{
              ret = mpiClient_write_data(server_aux->params.server, (char *)buffer + cont, diff, msg.id) ;
              if(ret == -1){
                fprintf(stderr,"(2)ERROR: nfi_mpi_server_read(ID=%s): Error on write operation\n",server_aux->id) ;
              }
            }

            cont = cont + ret; //Send bytes
            diff = size - cont;

          } while ((diff > 0) && (ret != 0)) ;

          ret = mpiClient_read_data(server_aux->params.server, (char *)&req, sizeof(struct st_mpi_server_write_req), msg.id) ;
          if(ret == -1){
            fprintf(stderr,"(3)ERROR: nfi_mpi_server_write(ID=%s): Error on write operation\n",server_aux->id) ;
            return -1;
          }

          debug_info("[NFI] nfi_mpi_server_write(ID=%s): write %s off %d size %d (err:%d).\n",server_aux->id,fh->url,(int)offset,(int)size,(int)req.size) ;
          if (req.size < 0)
          {
            fprintf(stderr,"ERROR: nfi_mpi_server_write(ID=%s): Fail write %s off %d size %d (err:%d).\n",server_aux->id,fh->url,(int)offset,(int)size,(int)req.size) ;
            mpi_server_err(MPI_SERVERERR_WRITE) ;
            return -1;
          }

          ret = cont;
        }

        debug_info("[NFI] nfi_mpi_server_write(ID=%s): end\n",server_aux->id) ;

        return ret;
      }

      ssize_t nfi_mpi_server_write_wos ( struct nfi_server *serv,
                                        struct nfi_fhandle *fh,
                                        void *buffer,
                                        off_t offset,
                                        size_t size )
      {
        struct nfi_mpi_server_server *server_aux;
        struct nfi_mpi_server_fhandle *fh_aux;
        struct st_mpi_server_msg msg;
        struct st_mpi_server_write_req req;
        int ret, diff, cont;

        // Check arguments...
        if (size == 0){
          return 0;
        }

        NULL_RET_ERR(serv, MPI_SERVERERR_PARAM) ;
        NULL_RET_ERR(fh,   MPI_SERVERERR_PARAM) ;
        nfi_mpi_server_keepConnected(serv) ;
        NULL_RET_ERR(serv->private_info, MPI_SERVERERR_PARAM) ;

        // private_info...
        server_aux = (struct nfi_mpi_server_server *) serv->private_info;
        debug_info("[NFI] nfi_mpi_server_write(ID=%s): begin off %d size %d\n",server_aux->id,(int)offset, (int)size) ;

        fh_aux     = (struct nfi_mpi_server_fhandle *) fh->priv_fh;
        server_aux = (struct nfi_mpi_server_server  *) serv->private_info;

        /************** LOCAL *****************/
        if(server_aux->params.locality)
        {
          int fd;
          fd = filesystem_open(fh_aux->path, O_WRONLY);
          if (fd < 0)
          {
            debug_error("filesystem_read reads zero bytes from url:%s offset:%ld size:%zu (ret:%zd) errno=%d\n", fh->url, (long int)offset, size, ret, errno) ;
            return -1;
          }
          filesystem_lseek(fd, offset, SEEK_SET) ;

          if(server_aux->params.sem_server != 0) sem_wait(server_aux->params.sem_server);
          ret = filesystem_write(fd, buffer, size) ;
          debug_info("write %s(%d) off %ld size %zu (ret:%zd)", fh->url, fd, (long int)offset, size, ret);

          if (ret < 0) {
            filesystem_close(fd);
            debug_error("filesystem_read reads zero bytes from url:%s offset:%ld size:%zu (ret:%zd) errno=%d\n", fh->url, (long int)offset, size, ret, errno) ;
            if(server_aux->params.sem_server != 0) sem_post(server_aux->params.sem_server);

            return -1;
          }
          filesystem_close(fd);
          if(server_aux->params.sem_server != 0) sem_post(server_aux->params.sem_server);

        }
        /************** SERVER ****************/
        else {
          msg.type = MPI_SERVER_WRITE_FILE_WOS;
          strcpy(msg.id, server_aux->id) ;
          //msg.u_st_mpi_server_msg.op_write.fd     = fh_aux->fd;
          strcpy(msg.u_st_mpi_server_msg.op_write.path, fh_aux->path);
          msg.u_st_mpi_server_msg.op_write.offset = offset;
          msg.u_st_mpi_server_msg.op_write.size   = size;

          #ifdef  DBG_IO
            printf("[NFI]write: -> path %s \n",msg.u_st_mpi_server_msg.op_write.path) ;
            printf("[NFI]write: -> offset %d \n",(int)msg.u_st_mpi_server_msg.op_write.offset) ;
            printf("[NFI]write: -> size %d \n",msg.u_st_mpi_server_msg.op_write.size) ;
          #endif

          ret = mpi_server_write_operation(server_aux->params.server, &msg) ;
          if(ret == -1){
            fprintf(stderr,"(1)ERROR: nfi_mpi_server_write(ID=%s): Error on write operation\n",server_aux->id) ;
            return -1;
          }

          diff = size;
          cont = 0;

          int buffer_size = size;

          // Max buffer size
          if (buffer_size > MAX_BUFFER_SIZE)
          {
              buffer_size = MAX_BUFFER_SIZE;
          }

          do{
            if (diff > buffer_size)
            {
              ret = mpiClient_write_data(server_aux->params.server, (char *)buffer + cont, buffer_size, msg.id) ;
              if(ret == -1){
                fprintf(stderr,"(2)ERROR: nfi_mpi_server_read(ID=%s): Error on write operation\n",server_aux->id) ;
              }
            }
            else{
              ret = mpiClient_write_data(server_aux->params.server, (char *)buffer + cont, diff, msg.id) ;
              if(ret == -1){
                fprintf(stderr,"(2)ERROR: nfi_mpi_server_read(ID=%s): Error on write operation\n",server_aux->id) ;
              }
            }

            cont = cont + ret; //Send bytes
            diff = size - cont;

          } while ((diff > 0) && (ret != 0)) ;

          ret = mpiClient_read_data(server_aux->params.server, (char *)&req, sizeof(struct st_mpi_server_write_req), msg.id) ;
          if(ret == -1){
            fprintf(stderr,"(3)ERROR: nfi_mpi_server_write(ID=%s): Error on write operation\n",server_aux->id) ;
            return -1;
          }

          debug_info("[NFI] nfi_mpi_server_write(ID=%s): write %s off %d size %d (err:%d).\n",server_aux->id,fh->url,(int)offset,(int)size,(int)req.size) ;
          if (req.size < 0)
          {
            fprintf(stderr,"ERROR: nfi_mpi_server_write(ID=%s): Fail write %s off %d size %d (err:%d).\n",server_aux->id,fh->url,(int)offset,(int)size,(int)req.size) ;
            mpi_server_err(MPI_SERVERERR_WRITE) ;
            return -1;
          }

          ret = cont;
        }

        debug_info("[NFI] nfi_mpi_server_write(ID=%s): end\n",server_aux->id) ;

        return ret;
      }

      int nfi_mpi_server_close ( struct nfi_server *serv,  struct nfi_fhandle *fh )
      {
        int nfi_mpi_server_close_ws  ( struct nfi_server *serv,  struct nfi_fhandle *fh );
        int nfi_mpi_server_close_wos ( struct nfi_server *serv,  struct nfi_fhandle *fh );

        if (xpn_session)
        {
          return nfi_mpi_server_close_ws ( serv, fh );
        }
        else
        {
          return 1;
        }
      }

      int nfi_mpi_server_close_ws ( struct nfi_server *serv,  struct nfi_fhandle *fh )
      {
        int ret = -1;
        struct nfi_mpi_server_fhandle *fh_aux;
        struct nfi_mpi_server_server *server_aux;
        struct st_mpi_server_msg msg;

        // Check arguments...
        NULL_RET_ERR(serv, MPI_SERVERERR_PARAM) ;
        NULL_RET_ERR(fh,   MPI_SERVERERR_PARAM) ;
        nfi_mpi_server_keepConnected(serv) ;
        NULL_RET_ERR(serv->private_info, MPI_SERVERERR_PARAM) ;

        // private_info...
        server_aux = (struct nfi_mpi_server_server *) serv->private_info;
        debug_info("[NFI] nfi_mpi_server_close(ID=%s): begin\n",server_aux->id) ;

        if (fh->priv_fh != NULL)
        {
          fh_aux = (struct nfi_mpi_server_fhandle *) fh->priv_fh;
          server_aux = (struct nfi_mpi_server_server *) serv->private_info;

          /************** LOCAL *****************/
          if(server_aux->params.locality)
          {
            if (fh_aux != NULL) {
              if(server_aux->params.sem_server != 0) sem_wait(server_aux->params.sem_server);
              ret = filesystem_close(fh_aux->fd) ;
              if(server_aux->params.sem_server != 0) sem_post(server_aux->params.sem_server);
            }
          }
          /************** SERVER ****************/
          else {
            msg.type = MPI_SERVER_CLOSE_FILE_WS;
            strcpy(msg.id, server_aux->id) ;
            msg.u_st_mpi_server_msg.op_close.fd = fh_aux->fd;
            nfi_mpi_server_doRequest(server_aux, &msg, (char *)&(ret), sizeof(int)) ;
            debug_info("[NFI] nfi_mpi_server_close(ID=%s): close -> %d \n",server_aux->id,msg.u_st_mpi_server_msg.op_close.fd) ;
          }

          /* free memory */
          free(fh->priv_fh) ;
          fh->priv_fh = NULL;
        }

        fh->type = NFINULL;
        fh->server = NULL;

        debug_info("[NFI] nfi_mpi_server_close(ID=%s): end\n",server_aux->id) ;

        // Return OK
        return ret;
      }

      int nfi_mpi_server_remove(struct nfi_server *serv,  char *url)
      {
        char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
        int ret;
        struct nfi_mpi_server_server *server_aux;
        struct st_mpi_server_msg msg;

        // Check arguments...
        NULL_RET_ERR(serv, MPI_SERVERERR_PARAM) ;
        NULL_RET_ERR(url,  MPI_SERVERERR_PARAM) ;
        nfi_mpi_server_keepConnected(serv) ;
        NULL_RET_ERR(serv->private_info, MPI_SERVERERR_PARAM) ;

        // private_info...
        server_aux = (struct nfi_mpi_server_server *) serv->private_info;
        debug_info("[NFI] nfi_mpi_server_remove(%s): begin %s\n",server_aux->id, url) ;
        if (serv == NULL){
          mpi_server_err(MPI_SERVERERR_PARAM) ;
          return -1;
        }

        server_aux = (struct nfi_mpi_server_server *)serv->private_info;

        ret = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir) ;
        if(ret < 0){
          fprintf(stderr,"nfi_mpi_server_remove: url %s incorrect.\n",url) ;
          mpi_server_err(MPI_SERVERERR_URL) ;
          return -1;
        }

        /************** LOCAL *****************/
        if(server_aux->params.locality)
        {
          ret = filesystem_unlink(dir) ;
          if (ret < 0)
          {
            debug_error("filesystem_open fails to open '%s' in server %s.\n", dir, serv->server) ;
            return -1;
          }
        }
        /************** SERVER ****************/
        else {
          msg.type = MPI_SERVER_RM_FILE;
          strcpy(msg.id, server_aux->id) ;
          strcpy(msg.u_st_mpi_server_msg.op_rm.path,dir) ;

          nfi_mpi_server_doRequest(server_aux, &msg, (char *)&(ret), sizeof(int)) ;
        }

        debug_info("[NFI] nfi_mpi_server_remove(ID=%s): end \n",server_aux->id) ;

        return ret;
      }

      int nfi_mpi_server_rename(struct nfi_server *serv,  char *old_url, char *new_url)
      {
        int ret;
        char server[NFIMAXPATHLEN], old_path[NFIMAXPATHLEN], new_path[NFIMAXPATHLEN];
        struct nfi_mpi_server_server *server_aux;
        struct st_mpi_server_msg msg;

        // Check arguments...
        NULL_RET_ERR(serv,     MPI_SERVERERR_PARAM) ;
        NULL_RET_ERR(old_url,  MPI_SERVERERR_PARAM) ;
        NULL_RET_ERR(new_url,  MPI_SERVERERR_PARAM) ;
        nfi_mpi_server_keepConnected(serv) ;
        NULL_RET_ERR(serv->private_info, MPI_SERVERERR_PARAM) ;

        // private_info...
        server_aux = (struct nfi_mpi_server_server *) serv->private_info;
        /*debug_info("[NFI] nfi_mpi_server_remove(%s): begin %s\n",server_aux->id, url) ;
        if (serv == NULL){
          mpi_server_err(MPI_SERVERERR_PARAM) ;
          return -1;
        }*/

        ret = ParseURL(old_url, NULL, NULL, NULL, server,  NULL,  old_path) ;
        if (ret < 0) {
          fprintf(stderr,"nfi_mpi_server_open: url %s incorrect.\n",old_url) ;
          mpi_server_err(MPI_SERVERERR_URL) ;
          return -1;
        }

        ret = ParseURL(new_url, NULL, NULL, NULL, server,  NULL,  new_path) ;
        if (ret < 0) {
          fprintf(stderr,"nfi_mpi_server_open: url %s incorrect.\n",new_url) ;
          mpi_server_err(MPI_SERVERERR_URL) ;
          return -1;
        }

        /************** LOCAL *****************/
        if(server_aux->params.locality)
        {
          ret = filesystem_rename(old_path, new_path) ;
          if (ret < 0)
          {
            debug_error("filesystem_rename fails to rename '%s' in server %s.\n", old_path, serv->server) ;
            return -1;
          }
        }
        /************** SERVER ****************/
        else {
          msg.type = MPI_SERVER_RENAME_FILE;

          strcpy(msg.id, server_aux->id) ;
          strcpy(msg.u_st_mpi_server_msg.op_rename.old_url, old_path) ;
          strcpy(msg.u_st_mpi_server_msg.op_rename.new_url, new_path) ;

          nfi_mpi_server_doRequest(server_aux, &msg, (char *)&(ret), sizeof(int)) ;
        }

        debug_info("[NFI] nfi_mpi_server_remove(ID=%s): end \n",server_aux->id) ;

        return ret;
      }

      int nfi_mpi_server_getattr ( struct nfi_server *serv,  struct nfi_fhandle *fh, struct nfi_attr *attr )
      {
        struct nfi_mpi_server_server *server_aux;
        struct nfi_mpi_server_fhandle *fh_aux;
        struct st_mpi_server_msg msg;
        struct st_mpi_server_attr_req req;

        debug_info("[NFI] nfi_mpi_server_getattr (...): begin\n") ;

        // check arguments...
        NULL_RET_ERR(serv,            MPI_SERVERERR_PARAM) ;
        NULL_RET_ERR(fh,              MPI_SERVERERR_PARAM) ;
        NULL_RET_ERR(attr,            MPI_SERVERERR_PARAM) ;
        NULL_RET_ERR(fh->priv_fh,     MPI_SERVERERR_PARAM) ;
        nfi_mpi_server_keepConnected(serv) ;
        NULL_RET_ERR(serv->private_info, MPI_SERVERERR_PARAM) ;

        // copy private information...
        server_aux = (struct nfi_mpi_server_server  *) serv->private_info;
        fh_aux     = (struct nfi_mpi_server_fhandle *) fh->priv_fh;
        server_aux = (struct nfi_mpi_server_server  *) serv->private_info;

        /************** LOCAL *****************/
        if(server_aux->params.locality)
        {
          req.status = filesystem_stat(fh_aux->path, &(req.attr)) ;
          if (((int) req.status) < 0) {
            debug_error("nfi_local_getattr: Fail stat %s.\n", fh_aux->path) ;
            return req.status;
          }
        }
        /************** SERVER ****************/
        else {
          msg.type = MPI_SERVER_GETATTR_FILE;
          strcpy(msg.id, server_aux->id) ;
          strcpy(msg.u_st_mpi_server_msg.op_getattr.path, fh_aux->path) ;

          nfi_mpi_server_doRequest(server_aux, &msg, (char *)&req, sizeof(struct st_mpi_server_attr_req)) ;
        }

        MPI_SERVERtoNFIattr(attr, &req.attr) ;

        debug_info("[NFI] nfi_mpi_server_getattr (...): end\n") ;

        // return status
        return req.status;
      }

      int nfi_mpi_server_setattr ( struct nfi_server *serv,  struct nfi_fhandle *fh, struct nfi_attr *attr )
      {
        struct nfi_mpi_server_server *server_aux;
        struct nfi_mpi_server_fhandle *fh_aux;

        // Check arguments...
        NULL_RET_ERR(serv, MPI_SERVERERR_PARAM) ;
        NULL_RET_ERR(fh,   MPI_SERVERERR_PARAM) ;
        NULL_RET_ERR(attr, MPI_SERVERERR_PARAM) ;
        NULL_RET_ERR(fh->priv_fh, MPI_SERVERERR_PARAM) ;
        nfi_mpi_server_keepConnected(serv) ;
        NULL_RET_ERR(serv->private_info, MPI_SERVERERR_PARAM) ;

        fh_aux = (struct nfi_mpi_server_fhandle *) fh->priv_fh;
        server_aux = (struct nfi_mpi_server_server *) serv->private_info;

        // TODO: setattr...

        return 0;
      }

      int nfi_mpi_server_mkdir(struct nfi_server *serv,  char *url, struct nfi_attr *attr, struct nfi_fhandle *fh)
      {
        char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
        int ret;
        struct nfi_mpi_server_server *server_aux;
        struct nfi_mpi_server_fhandle *fh_aux;
        struct st_mpi_server_msg msg;

        // Check arguments...
        NULL_RET_ERR(serv, MPI_SERVERERR_PARAM) ;
        NULL_RET_ERR(attr, MPI_SERVERERR_PARAM) ;
        nfi_mpi_server_keepConnected(serv) ;
        NULL_RET_ERR(serv->private_info, MPI_SERVERERR_PARAM) ;

        // private_info...
        server_aux = (struct nfi_mpi_server_server *)serv->private_info;

        ret = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir) ;
        if (ret < 0) {
          fprintf(stderr,"nfi_mpi_server_mkdir: url %s incorrect.\n",url) ;
          mpi_server_err(MPI_SERVERERR_URL) ;
          return -1;
        }

        /* private_info file handle */
        fh_aux = (struct nfi_mpi_server_fhandle *)malloc(sizeof(struct nfi_mpi_server_fhandle)) ;
        NULL_RET_ERR(fh_aux, MPI_SERVERERR_MEMORY) ;

        bzero(fh_aux, sizeof(struct nfi_mpi_server_fhandle)) ;

        /************** LOCAL *****************/
        if(server_aux->params.locality)
        {
          ret = filesystem_mkdir(dir, /*attr->at_mode*/ 0777) ;
          if ((ret < 0) && (errno != EEXIST))
          {
            debug_error("nfi_local_mkdir: Fail mkdir %s.\n", dir) ;
            free(fh_aux) ;
            return -1;
          }
          fh_aux->fd = ret; //Cuidado
        }
        /************** SERVER ****************/
        else {
          msg.type = MPI_SERVER_MKDIR_DIR;
          strcpy(msg.u_st_mpi_server_msg.op_mkdir.path, dir) ;

          nfi_mpi_server_doRequest(server_aux, &msg, (char *)&(fh_aux->fd), sizeof(int)) ;
          strcpy(fh_aux->path, dir) ;

          if ((fh_aux->fd < 0)&&(errno != EEXIST)) {
            mpi_server_err(MPI_SERVERERR_MKDIR) ;
            fprintf(stderr,"nfi_mpi_server_mkdir: Fail mkdir %s in server %s.\n",dir,serv->server) ;
            free(fh_aux) ;
            return -1;
          }
        }

        fh->type = NFIDIR;
        fh->priv_fh = (void *)fh_aux;

        fh->url = STRING_MISC_StrDup(url) ;
        if(fh->url == NULL){
          mpi_server_err(MPI_SERVERERR_MEMORY) ;
          free(fh_aux) ;
          return -1;
        }

        //LOCALtoNFIattr(attr, &st) ; //TODO

        return ret;
      }

      int nfi_mpi_server_opendir(struct nfi_server *serv,  char *url, struct nfi_fhandle *fho)
      {
        char dir[NFIMAXPATHLEN], server[NFIMAXPATHLEN];
        int ret;
        struct st_mpi_server_msg msg;
        struct nfi_mpi_server_server *server_aux;
        struct nfi_mpi_server_fhandle *fh_aux;

        // Check arguments...
        NULL_RET_ERR(serv, MPI_SERVERERR_PARAM) ;
        NULL_RET_ERR(url,  MPI_SERVERERR_PARAM) ;
        NULL_RET_ERR(fho,  MPI_SERVERERR_PARAM) ;
        nfi_mpi_server_keepConnected(serv) ;
        NULL_RET_ERR(serv->private_info, MPI_SERVERERR_PARAM) ;

        // private_info...
        ret = ParseURL(url, NULL, NULL, NULL, server,  NULL,  dir) ;
        if (ret < 0) {
          fprintf(stderr,"nfi_mpi_server_opendir: url %s incorrect.\n",url) ;
          mpi_server_err(MPI_SERVERERR_URL) ;
          return -1;
        }

        fho->url = strdup(url) ;
        NULL_RET_ERR(fho->url, MPI_SERVERERR_MEMORY) ;

        fh_aux = (struct nfi_mpi_server_fhandle *)malloc(sizeof(struct nfi_mpi_server_fhandle)) ;
        if (fh_aux == NULL){
          mpi_server_err(MPI_SERVERERR_MEMORY) ;
          free(fho->url) ;
          return -1;
        }

        server_aux = (struct nfi_mpi_server_server *) serv->private_info;

        /************** LOCAL *****************/
        if(server_aux->params.locality)
        {
          fh_aux->dir = filesystem_opendir(dir) ;
          if (fh_aux->dir == NULL) {
            free(fh_aux) ;
            free(fho->url) ;
            debug_error("real_posix_opendir fails to open directory '%s' in server '%s'.\n", dir, serv->server) ;
            return -1;
          }
        }
        /************** SERVER ****************/
        else {
          msg.type = MPI_SERVER_OPENDIR_DIR;
          strcpy(msg.id, server_aux->id) ;
          strcpy(msg.u_st_mpi_server_msg.op_opendir.path, dir) ;

          unsigned long long aux;

          //nfi_mpi_server_doRequest(server_aux, &msg, (char *)&(fh_aux->dir), sizeof(DIR*)) ; //NEW
          nfi_mpi_server_doRequest(server_aux, &msg, (char *)&(aux), sizeof(DIR*)) ; //NEW
          fh_aux->dir = (DIR *)aux;
        }

        //fh_aux->fd = ret;
        strcpy(fh_aux->path, dir) ;
        fho->type = NFIDIR;

        fho->server = NULL;
        fho->priv_fh = NULL;
        fho->server = serv;
        fho->priv_fh = (void *) fh_aux;

        return 0; //CUIDADO
      }

      int nfi_mpi_server_readdir(struct nfi_server *serv,  struct nfi_fhandle *fh, char *entry, unsigned char *type)
      {
        struct dirent *ent;
        struct st_mpi_server_direntry ret_entry;
        struct st_mpi_server_msg msg;
        struct nfi_mpi_server_server *server_aux;
        struct nfi_mpi_server_fhandle *fh_aux;

        // Check arguments...
        NULL_RET_ERR(serv,        MPI_SERVERERR_PARAM) ;
        NULL_RET_ERR(fh,          MPI_SERVERERR_PARAM) ;
        NULL_RET_ERR(fh->priv_fh, MPI_SERVERERR_PARAM) ;
        if (fh->type != NFIDIR) {
          mpi_server_err(MPI_SERVERERR_NOTDIR) ;
          return -1;
        }
        nfi_mpi_server_keepConnected(serv) ;
        NULL_RET_ERR(serv->private_info, MPI_SERVERERR_PARAM) ;

        // private_info...
        server_aux = (struct nfi_mpi_server_server *)serv->private_info;
        fh_aux = (struct nfi_mpi_server_fhandle *)fh->priv_fh;

        entry[0] = '\0';

        /************** LOCAL *****************/
        if(server_aux->params.locality)
        {
          ent = filesystem_readdir(fh_aux->dir) ;

          if(ent == NULL){
              return 1;
          }
          if(type==NULL){
                return 0;
          }

          strcpy(entry, ent->d_name) ;
          *type = ent->d_type;
        }
        /************** SERVER ****************/
        else {
          msg.type = MPI_SERVER_READDIR_DIR;

          strcpy(msg.id, server_aux->id) ;
          msg.u_st_mpi_server_msg.op_readdir.dir = fh_aux->dir ;

          nfi_mpi_server_doRequest(server_aux, &msg, (char *)&(ret_entry), sizeof(struct st_mpi_server_direntry)) ; //NEW

          if(ret_entry.end == 0){
              return 1;
          }
          if(type==NULL){
                return 0;
          }
          
          strcpy(entry, ret_entry.ret.d_name) ;
          *type = ret_entry.ret.d_type;
        }

        return 0;
      }

      int nfi_mpi_server_closedir ( struct nfi_server *serv,  struct nfi_fhandle *fh )
      {
        int ret;
        struct st_mpi_server_msg msg;
        struct nfi_mpi_server_server *server_aux;
        struct nfi_mpi_server_fhandle *fh_aux;

        // Check arguments...
        NULL_RET_ERR(serv, MPI_SERVERERR_PARAM) ;
        NULL_RET_ERR(fh,   MPI_SERVERERR_PARAM) ;
        nfi_mpi_server_keepConnected(serv) ;
        NULL_RET_ERR(serv->private_info, MPI_SERVERERR_PARAM) ;

        // do closedir...
        if (fh->priv_fh != NULL){
          // private_info...
          server_aux = (struct nfi_mpi_server_server *)serv->private_info;
          fh_aux = (struct nfi_mpi_server_fhandle *) fh->priv_fh;

          /************** LOCAL *****************/
          if(server_aux->params.locality)
          {
            filesystem_closedir(fh_aux->dir);
          }
          /************** SERVER ****************/
          else {
            msg.type = MPI_SERVER_CLOSEDIR_DIR;

            strcpy(msg.id, server_aux->id) ;
            msg.u_st_mpi_server_msg.op_closedir.dir = fh_aux->dir ;

            nfi_mpi_server_doRequest(server_aux, &msg, (char *)&(ret), sizeof(int)) ; //NEW

          }

          /* free memory */
          FREE_AND_NULL(fh->priv_fh) ;

        }

        return 0;
      }

      int nfi_mpi_server_rmdir(struct nfi_server *serv,  char *url)
      {
        int ret;
        struct nfi_mpi_server_server *server_aux;
        struct st_mpi_server_msg msg;
        char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];

        // Check arguments...
        NULL_RET_ERR(serv, MPI_SERVERERR_PARAM) ;
        NULL_RET_ERR(url,  MPI_SERVERERR_PARAM) ;
        nfi_mpi_server_keepConnected(serv) ;
        NULL_RET_ERR(serv->private_info, MPI_SERVERERR_PARAM) ;

        // private_info...
        server_aux = (struct nfi_mpi_server_server *)serv->private_info;

        ret = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir) ;
        if (ret < 0) {
          fprintf(stderr,"nfi_mpi_server_rmdir: url %s incorrect.\n",url) ;
          mpi_server_err(MPI_SERVERERR_URL) ;
          return -1;
        }

        /************** LOCAL *****************/
        if(server_aux->params.locality)
        {
          ret = filesystem_rmdir(dir) ;
          if (ret < 0)
          {
            debug_error(stderr,"nfi_local_rmdir: Fail rmdir %s.\n", dir) ;
            return -1;
          }
        }
        /************** SERVER ****************/
        else {
          msg.type = MPI_SERVER_RMDIR_DIR;
          strcpy(msg.u_st_mpi_server_msg.op_rmdir.path, dir) ;
          nfi_mpi_server_doRequest(server_aux, &msg, (char *)&(ret), sizeof(int)) ;

          if(ret < 0){
            fprintf(stderr,"nfi_mpi_server_rmdir: Fail rmdir %s in server %s.\n",dir,serv->server) ;
            mpi_server_err(MPI_SERVERERR_REMOVE) ;
            return -1;
          }
        }

        return 0;
      }

      int nfi_mpi_server_preload(struct nfi_server *serv, char *url, char *virtual_path, char *storage_path, int opt)
      {
        //char dir[NFIMAXPATHLEN];
        int ret;
        struct nfi_mpi_server_server *server_aux;
        struct st_mpi_server_msg msg;

        // Check arguments...
        NULL_RET_ERR(serv, MPI_SERVERERR_PARAM) ;
        NULL_RET_ERR(url,  MPI_SERVERERR_PARAM) ;
        NULL_RET_ERR(virtual_path, MPI_SERVERERR_PARAM) ;
        NULL_RET_ERR(storage_path, MPI_SERVERERR_PARAM) ;
        nfi_mpi_server_keepConnected(serv) ;
        NULL_RET_ERR(serv->private_info, MPI_SERVERERR_PARAM) ;

        // get private_info...
        server_aux = (struct nfi_mpi_server_server *) serv->private_info;
        debug_info("[NFI] nfi_mpi_server_preload(ID=%s): begin %s - %s \n", server_aux->id, virtual_path, storage_path) ;

        if (url[strlen(url)-1] == '/'){
            mpi_server_err(MPI_SERVERERR_PARAM) ;
            return -1;
        }

        server_aux = (struct nfi_mpi_server_server *) serv->private_info;
        debug_info("[NFI] nfi_mpi_server_preload(ID=%s): preload %s in server %s.\n",server_aux->id,virtual_path,serv->server) ;

        /*****************************************/
        msg.type = MPI_SERVER_PRELOAD_FILE;
        strcpy(msg.id, server_aux->id) ;
        //strcpy(msg.u_st_mpi_server_msg.op_preload.path,dir) ;
        strcpy(msg.u_st_mpi_server_msg.op_preload.virtual_path,virtual_path) ;
        strcpy(msg.u_st_mpi_server_msg.op_preload.storage_path,storage_path) ;
        msg.u_st_mpi_server_msg.op_preload.block_size = serv->block_size;
        msg.u_st_mpi_server_msg.op_preload.opt        = opt;

        nfi_mpi_server_doRequest(server_aux, &msg, (char *)&ret, sizeof(int)) ;
        /*****************************************/

        debug_info("[NFI] nfi_mpi_server_preload(ID=%s): end %s - %s = %d\n", server_aux->id,virtual_path, storage_path, ret) ;
        if (ret == -1) {
          printf("[NFI]Error en el preload\n") ;
        }

        return ret;
      }

      int nfi_mpi_server_flush ( struct nfi_server *serv,  char *url, char *virtual_path, char *storage_path, int opt )
      {
        //char dir[NFIMAXPATHLEN];
        int ret;
        struct nfi_mpi_server_server *server_aux;
        struct st_mpi_server_msg msg;

        // Check arguments...
        NULL_RET_ERR(serv, MPI_SERVERERR_PARAM) ;
        NULL_RET_ERR(url,  MPI_SERVERERR_PARAM) ;
        NULL_RET_ERR(virtual_path, MPI_SERVERERR_PARAM) ;
        NULL_RET_ERR(storage_path, MPI_SERVERERR_PARAM) ;
        nfi_mpi_server_keepConnected(serv) ;
        NULL_RET_ERR(serv->private_info, MPI_SERVERERR_PARAM) ;

        // private_info...
        server_aux = (struct nfi_mpi_server_server *) serv->private_info;
        debug_info("[NFI] nfi_mpi_server_flush(ID=%s): begin %s - %s \n", server_aux->id,virtual_path, storage_path) ;

        if (url[strlen(url)-1] == '/') {
          mpi_server_err(MPI_SERVERERR_PARAM) ;
          return -1;
        }

        server_aux = (struct nfi_mpi_server_server *) serv->private_info;
        debug_info("[NFI] nfi_mpi_server_flush(ID=%s): open %s in server %s.\n",server_aux->id,virtual_path,serv->server) ;

        /*****************************************/
        msg.type = MPI_SERVER_FLUSH_FILE;
        strcpy(msg.id, server_aux->id) ;
        //strcpy(msg.u_st_mpi_server_msg.op_flush.path,dir) ;
        strcpy(msg.u_st_mpi_server_msg.op_flush.virtual_path,virtual_path) ;
        strcpy(msg.u_st_mpi_server_msg.op_flush.storage_path,storage_path) ;
        msg.u_st_mpi_server_msg.op_flush.block_size = serv->block_size;
        msg.u_st_mpi_server_msg.op_flush.opt = opt;

        nfi_mpi_server_doRequest(server_aux, &msg, (char *)&ret, sizeof(int)) ;
        /*****************************************/

        debug_info("[NFI] nfi_mpi_server_flush(ID=%s): end %s - %s = %d\n", server_aux->id,virtual_path, storage_path, ret) ;

        return 0;
      }

      //TODO
      int nfi_mpi_server_statfs(struct nfi_server *serv,  struct nfi_info *inf)
      {
        /*
          struct mpi_server_info mpi_serverinf;
          int ret;
          struct nfi_mpi_server_server *server_aux;

          // Check arguments...
          NULL_RET_ERR(serv, MPI_SERVERERR_PARAM) ;
          NULL_RET_ERR(inf,  MPI_SERVERERR_PARAM) ;
          nfi_mpi_server_keepConnected(serv) ;
          NULL_RET_ERR(serv->private_info, MPI_SERVERERR_PARAM) ;

          // private_info...
          server_aux = (struct nfi_mpi_server_server *)serv->private_info;

          ret = mpi_server_statfs(server_aux->fh, &mpi_serverinf, server_aux->cl) ;
          if (ret <0).{
                mpi_server_err(MPI_SERVERERR_STATFS) ;
                return -1;
          }

          MPItoNFIInfo(inf, &mpi_serverinf) ;
        */

        return 0;
      }


   /* ................................................................... */
