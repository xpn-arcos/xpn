
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

      #include "nfi_mpiServer.h"


   /* ... Functions / Funciones ......................................... */

      /*
       * Communication
       */

      int mpiServer_write_operation ( struct nfi_mpiServer_connector sd, struct st_mpiServer_msg *head )
      {
        int ret;

        debug_info("[NFI] (ID=%s) mpiClient_write_data: begin               HEAD_TYPE:%d\n", head->id, sizeof(head->type)) ;
        ret = mpiClient_write_operation(sd, (char *)&head->type, sizeof(head->type), head->id) ;
        if (ret == -1){
            debug_warning("Server[?]: mpiClient_write_data fails :-(") ;
            return -1;
        }

        int rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank) ;

        debug_info("[NFI] (ID=%s) write_operation: %d -> \n", head->id, head->type) ;
        switch (head->type)
        {
          //Connection API
          case MPISERVER_FINALIZE:
                  debug_info("[NFI] (ID=%s) FINALIZE operation\n", head->id) ;
                  ret = 0 ;
          /*
                  ret = mpiClient_write_data(sd, (char *)&head->u_st_mpiServer_msg.op_end, sizeof(struct st_mpiServer_end), head->id) ;
          */
                  break;
          case MPISERVER_GETID:
                  debug_info("[NFI] (ID=%s) GETID operation\n", head->id) ;
                  ret = 0 ;
          /*
                  ret = mpiClient_write_data(sd, (char *)head->id, MPISERVER_ID, head->id) ;
          */
                  break;
          case MPISERVER_END:
                  debug_info("[NFI] (ID=%s) END operation\n", head->id) ;
                  ret = 0 ;
          /*
                  ret = mpiClient_write_data(sd, (char *)&head->u_st_mpiServer_msg.op_end, sizeof(struct st_mpiServer_end), head->id) ;
          */
                  break;

          //File API
          case MPISERVER_OPEN_FILE:
                  debug_info("[NFI] (ID=%s) OPEN operation ---------------------  %ld\n", head->id, sizeof(struct st_mpiServer_open)) ;
                  ret = mpiClient_write_data(sd, (char *)&head->u_st_mpiServer_msg.op_open, sizeof(struct st_mpiServer_open), head->id) ;
                  break;
          case MPISERVER_CREAT_FILE:
                  debug_info("[NFI] (ID=%s) CREAT operation\n", head->id) ;
                  ret = mpiClient_write_data(sd, (char *)&head->u_st_mpiServer_msg.op_creat, sizeof(struct st_mpiServer_creat), head->id) ;
                  break;
          case MPISERVER_READ_FILE:
                  debug_info("[NFI] (ID=%s) READ operation\n", head->id) ;
                  ret = mpiClient_write_data(sd, (char *)&head->u_st_mpiServer_msg.op_read, sizeof(struct st_mpiServer_read), head->id) ;
                  break;
          case MPISERVER_WRITE_FILE:
                  debug_info("[NFI] (ID=%s) WRITE operation\n", head->id) ;
                  ret = mpiClient_write_data(sd, (char *)&head->u_st_mpiServer_msg.op_write, sizeof(struct st_mpiServer_write), head->id) ;
                  break;
          case MPISERVER_CLOSE_FILE:
                  debug_info("[NFI] (ID=%s) CLOSE operation\n", head->id) ;
                  ret = mpiClient_write_data(sd, (char *)&head->u_st_mpiServer_msg.op_close, sizeof(struct st_mpiServer_close), head->id) ;
                  break;
          case MPISERVER_RM_FILE:
                  debug_info("[NFI] (ID=%s) RM operation\n", head->id) ;
                  ret = mpiClient_write_data(sd, (char *)&head->u_st_mpiServer_msg.op_rm, sizeof(struct st_mpiServer_rm), head->id) ;
                  break;
          case MPISERVER_GETATTR_FILE:
                  debug_info("[NFI] (ID=%s) GETATTR operation\n", head->id) ;
                  ret = mpiClient_write_data(sd, (char *)&head->u_st_mpiServer_msg.op_getattr, sizeof(struct st_mpiServer_getattr), head->id) ;
                  break;

          //Directory API
          case MPISERVER_MKDIR_DIR:
                  debug_info("[NFI] (ID=%s) MDKIR operation\n", head->id) ;
                  ret = mpiClient_write_data(sd, (char *)&head->u_st_mpiServer_msg.op_mkdir, sizeof(struct st_mpiServer_mkdir), head->id) ;
                  break;
          case MPISERVER_RMDIR_DIR:
                  debug_info("[NFI] (ID=%s) RMDIR operation\n", head->id) ;
                  ret = mpiClient_write_data(sd, (char *)&head->u_st_mpiServer_msg.op_rmdir, sizeof(struct st_mpiServer_rmdir), head->id) ;
                  break;

          //Optimization API
          case MPISERVER_FLUSH_FILE:
                  debug_info("[NFI] (ID=%s) FLUSH operation\n", head->id) ;
                  ret = mpiClient_write_data(sd, (char *)&head->u_st_mpiServer_msg.op_flush, sizeof(struct st_mpiServer_flush), head->id) ;
                  break;
          case MPISERVER_PRELOAD_FILE:
                  debug_info("[NFI] (ID=%s) PRELOAD operation\n", head->id) ;
                  ret = mpiClient_write_data(sd, (char *)&head->u_st_mpiServer_msg.op_preload, sizeof(struct st_mpiServer_preload), head->id) ;
                  break;
        }

        return ret;
      }

      int nfi_mpiServer_doRequest ( struct nfi_mpiServer_server *server_aux, struct st_mpiServer_msg *msg, char *req, int req_size )
      {
        ssize_t ret ;

        // send request...
        debug_info("[NFI] (ID=%s): %s: -> ...\n", server_aux->id, msg->id) ;
        ret = mpiServer_write_operation(server_aux->sd, msg) ;
        if (ret < 0) {
          return -1 ;
        }

        // read response...
        debug_info("[NFI] (ID=%s): %s: <- ...\n", server_aux->id, msg->id) ;
        bzero(req, req_size) ;
        ret = mpiClient_read_data(server_aux->sd, req, req_size, msg->id) ;
        if (ret < 0) {
          return -1 ;
        }

        // return OK
        return 0 ;
      }

      int nfi_mpiServer_keepConnected ( struct nfi_server *serv )
      {
#ifdef NFI_DYNAMIC
        if (serv->private_info == NULL)
        {
          int ret = nfi_mpiServer_reconnect(serv) ;
          if (ret < 0)
          {
            /* mpiServer_err() ; not necessary */
            serv->private_info = NULL;
            return -1;
          }
        }
#endif

        // return OK
        return (serv->private_info != NULL) ;
      }


      /*
       *  PRIVATE FUNCTIONS TO USE mpiServer SERVERS
       */

      void NFItoMPISERVERattr ( struct stat *att, struct nfi_attr *nfi_att )
      {
        if (nfi_att->at_type == NFIFILE){
          att->st_mode = nfi_att->at_mode | S_IFREG;     /* protection */
        }

        if (nfi_att->at_type == NFIDIR){
          att->st_mode = nfi_att->at_mode | S_IFDIR;     /* protection */
        }

        att->st_size         = nfi_att->at_size;      /* size */
        att->st_uid          = nfi_att->at_uid;      /* user ID of owner */
        att->st_gid          = nfi_att->at_gid;      /* group ID of owner */
        att->st_blksize      = nfi_att->at_blksize;  /* blocksize for filesystem I/O */
        att->st_blocks       = nfi_att->at_blocks;   /* number of blocks allocated */
        att->st_atime        = nfi_att->at_atime;    /* time of last access */
        att->st_mtime        = nfi_att->at_mtime;    /* time of last modification */
        att->st_ctime        = nfi_att->at_ctime;    /* time of last change */
      }

      void MPISERVERtoNFIattr ( struct nfi_attr *nfi_att, struct stat *att )
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

      void MPISERVERtoNFIInfo(struct nfi_info *nfi_inf, struct nfi_info *mpiServer_inf)
      {
      }


      /*
       * PUBLIC FUNCTIONS TO USE MPISERVER
       */

       /************************************************************
        * Init Local                                                *
        ************************************************************/
      int nfi_mpiServer_init ( char *url, struct nfi_server *serv, struct nfi_attr_server *attr )
      {
        int ret ;
        struct nfi_mpiServer_server *server_aux;
        struct st_mpiServer_msg msg;
        char server[MAXPATHLEN], dir[MAXPATHLEN], prt[MAXPATHLEN];


        debug_info("[NFI] nfi_mpiServer_init: begin\n") ;

        // check arguments...
        //NULL_RET_ERR(serv, MPISERVERERR_PARAM) ;
        //NULL_RET_ERR(attr, MPISERVERERR_PARAM) ;

        // new nfi_ops with mpiServer functions...
        serv->ops = (struct nfi_ops *)malloc(sizeof(struct nfi_ops)) ;
        NULL_RET_ERR(serv->ops, MPISERVERERR_MEMORY) ;

        bzero(serv->ops, sizeof(struct nfi_ops)) ;
        serv->ops->nfi_reconnect      = nfi_mpiServer_reconnect;
        serv->ops->nfi_disconnect     = nfi_mpiServer_disconnect;

        serv->ops->nfi_open           = nfi_mpiServer_open;
        serv->ops->nfi_create         = nfi_mpiServer_create;
        serv->ops->nfi_read           = nfi_mpiServer_read;
        serv->ops->nfi_write          = nfi_mpiServer_write;
        serv->ops->nfi_close          = nfi_mpiServer_close;
        serv->ops->nfi_remove         = nfi_mpiServer_remove;
        serv->ops->nfi_rename         = nfi_mpiServer_rename;
        serv->ops->nfi_getattr        = nfi_mpiServer_getattr;
        serv->ops->nfi_setattr        = nfi_mpiServer_setattr;

        serv->ops->nfi_opendir        = nfi_mpiServer_opendir;
        serv->ops->nfi_mkdir          = nfi_mpiServer_mkdir;
        serv->ops->nfi_readdir        = nfi_mpiServer_readdir;
        serv->ops->nfi_closedir       = nfi_mpiServer_closedir;
        serv->ops->nfi_rmdir          = nfi_mpiServer_rmdir;

        serv->ops->nfi_preload        = nfi_mpiServer_preload;
        serv->ops->nfi_flush          = nfi_mpiServer_flush;

        serv->ops->nfi_statfs         = nfi_mpiServer_statfs;

        // parse url...
        ret = ParseURL(url, prt, NULL, NULL, server, NULL, dir) ;
        if (ret < 0)
        {
          mpiServer_err(MPISERVERERR_URL) ;
          debug_error("nfi_mpiServer_init: incorrect url '%s'.\n", url) ;
          free(serv->ops) ;
          return -1;
        }

        // new nfi_mpiserver_server...
        server_aux = (struct nfi_mpiServer_server *)malloc(sizeof(struct nfi_mpiServer_server)) ;
        if (server_aux == NULL) {
          mpiServer_err(MPISERVERERR_MEMORY) ;
          free(serv->ops) ;
          return -1;
        }
        serv->private_info = (void *)server_aux;

        // initialize MPI Client communication side...
        ret = mpiClient_comm_init(&(server_aux->params)) ;
        if (ret < 0) {
          free(serv->ops) ;
          free(server_aux) ;
          return -1 ;
        }

        ret = nfi_mpiServer_connect(serv, url, prt, server, dir) ;
        if (ret < 0) {
          free(serv->ops) ;
          free(server_aux) ;
          return -1 ;
        }

        ret = mpiClient_comm_locality (&(server_aux->params)) ; //NEW //Ajustar
        if (ret < 0) {
          free(serv->ops) ;
          free(server_aux) ;
          return -1 ;
        }

        //serv->protocol = MPISERVER;

        debug_info("[NFI] nfi_mpiServer_init(ID=%s): end\n",server_aux->id) ;

        // return OK
        return 0;
      }


      int nfi_mpiServer_connect(struct nfi_server *serv, char *url, char* prt, char* server, char* dir)
      {
        int ret ;
        struct nfi_mpiServer_server *server_aux;
        //struct st_mpiServer_msg msg;

        static int init = 0;
        static MPI_Comm comm;
        static int id_server = 0;
        static struct nfi_mpiServer_server server_aux2;

        server_aux = (struct nfi_mpiServer_server *) (serv->private_info) ;

        if (init == 0)
        {
          ret = mpiClient_comm_connect(&(server_aux->params)) ;
          if (ret < 0) {
              return -1 ;
          }

          init = 1;
          comm = server_aux->params.server;
          server_aux2 = *server_aux;
        }
        else{
          server_aux->params = server_aux2.params;
        }

        server_aux->sd.comm = comm ; //Comunidador
        server_aux->sd.rank_id = id_server ; //rank

        id_server++;

        int rank;
        MPI_Comm_rank (MPI_COMM_WORLD, &rank) ;

        //.....................................
        /*strcpy(msg.id, "GETID") ;
        msg.type = MPISERVER_GETID;

        nfi_mpiServer_doRequest(server_aux, &msg, (char *)&(server_aux->id), MPISERVER_ID) ;
        //.....................................
        printf("AQUI 2\n") ;
        // copy 'server address' string...
        serv->server = strdup(server) ;
        NULL_RET_ERR(serv->server, MPISERVERERR_MEMORY) ;*/

        // copy 'url' string...
        serv->url = strdup(url) ;
        NULL_RET_ERR(serv->url, MPISERVERERR_MEMORY) ;

        // new server wrk...
        serv->wrk = (struct nfi_worker *)malloc(sizeof(struct nfi_worker)) ;
        memset(serv->wrk, 0, sizeof(struct nfi_worker)) ;

#ifdef _ALL_THREADS_
//      nfi_worker_init(serv->wrk, serv, 1) ;
//      nfi_worker_init(serv->wrk, serv, 0) ;
#else
        if (strcmp("mpiServer", prt) == 0)
        {
          debug_info("[NFI] mpiServer\n") ;
          debug_info("[NFI] nfi_worker_init(1,ID=%s): \n",server_aux->id) ;
          nfi_worker_init(serv->wrk, serv, 1) ;
        }
        else
        {
          debug_info("[NFI] nfi_worker_init(0,ID=%s): \n",server_aux->id) ;
          nfi_worker_init(serv->wrk, serv, 0) ;
        }
#endif
      }



      /************************************************************
       * Disconnect to the server                                   *
       * **********************************************************/
      int nfi_mpiServer_disconnect ( struct nfi_server *serv )
      {
        int ret ;
        struct nfi_mpiServer_server *server_aux;
        //struct st_mpiServer_msg msg;

        debug_info("[NFI] nfi_mpiServer_disconnect(): begin\n") ;

        // check params...
        if (serv == NULL) {
          return 0;
        }
        server_aux = (struct nfi_mpiServer_server *)serv->private_info;
        if (server_aux == NULL) {
          return 0;
        }

        // MPI Disconnect...
        ret = mpiClient_comm_disconnect(&(server_aux->params)) ;
        if (ret < 0) {
          debug_error("[NFI]: mpiClient_comm_disconnect fails :-(") ;
        }

        // free private_info, 'url' string and 'server' string...
        FREE_AND_NULL(serv->private_info) ;
        FREE_AND_NULL(serv->url) ;
        FREE_AND_NULL(serv->server) ;

        //serv->protocol = -1;
        debug_info("[NFI] nfi_mpiServer_disconnect(): end\n") ;

        // return OK
        return 0;
      }



      /************************************************************
       * Reconnect to the MPI server                              *
       ************************************************************/
      int nfi_mpiServer_reconnect(struct nfi_server *serv)
      {
        /* Don't see the serv result */
        char   server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
        int    ret;
        struct nfi_mpiServer_server *server_aux;

        // ParseURL...
        ret = ParseURL(serv->url,  NULL, NULL, NULL, server,  NULL,  dir) ;
        if (ret < 0) {
          mpiServer_err(MPISERVERERR_URL) ;
          debug_error("nfi_mpiServer_reconnect: url %s incorrect.\n", serv->url) ;
          return -1;
        }

        // get private_info...
        server_aux = (struct nfi_mpiServer_server *)malloc(sizeof(struct nfi_mpiServer_server)) ;
        NULL_RET_ERR(server_aux, MPISERVERERR_MEMORY) ;

        strcpy(server_aux->path, dir) ;
        serv->private_info = (void *)server_aux;

        // reconnect...
        ret = mpiClient_comm_connect(&(server_aux->params)) ;
        if (ret < 0) {
          debug_error("nfi_mpiServer_reconnect: mpiClient_comm_connect fails.\n") ;
          return -1;
        }

        // return OK
        return 0;
      }



      /************************************************************
       * Destroy MPI operations                             *
       ************************************************************/
      int nfi_mpiServer_destroy(struct nfi_server *serv)
      {
        int ret ;
        struct nfi_mpiServer_server *server_aux;
        struct st_mpiServer_msg msg;

        // check arguments...
        if (serv == NULL) {
          return 0;
        }
        server_aux = (struct nfi_mpiServer_server *)serv->private_info;
        if (server_aux == NULL) {
          return 0;
        }

        // Remote disconnect...
        msg.type = -1;
        mpiServer_write_operation(server_aux->sd, &msg) ;

        // Finalize MPI communication...
        ret = mpiClient_comm_destroy(&(server_aux->params)) ;
        if (ret < 0) {
          debug_error("[NFI]: mpiClient_comm_destroy fails :-(") ;
        }

        // end workers...
        nfi_worker_end(serv->wrk) ;

        // free private_info, ops, 'url' string and 'server' string...
        FREE_AND_NULL(serv->url) ;
        FREE_AND_NULL(serv->server) ;
        FREE_AND_NULL(serv->private_info) ;
        FREE_AND_NULL(serv->ops) ;

        //serv->protocol = -1;

        // return OK
        return 0;
      }


      /*
       *  GENERIC FUNCTIONS
       */

      int nfi_mpiServer_open ( struct nfi_server *serv,  char *url, struct nfi_fhandle *fho )
      {
        char   dir[NFIMAXPATHLEN], server[NFIMAXPATHLEN];
        int    ret ;
        struct nfi_mpiServer_server *server_aux;
        struct nfi_mpiServer_fhandle *fh_aux;
        struct st_mpiServer_msg msg;

        // Check arguments...
        NULL_RET_ERR(serv, MPISERVERERR_PARAM) ;
        NULL_RET_ERR(fho,  MPISERVERERR_PARAM) ;
        nfi_mpiServer_keepConnected(serv) ;
        NULL_RET_ERR(serv->private_info, MPISERVERERR_PARAM) ;

        // private_info...
        server_aux = (struct nfi_mpiServer_server *) serv->private_info;
        debug_info("[NFI] nfi_mpiServer_open(ID=%s): begin %s\n",server_aux->id,url) ;

        if (url[strlen(url)-1] == '/'){
          return nfi_mpiServer_opendir(serv, url, fho) ;
        }

        ret = ParseURL(url, NULL, NULL, NULL, server,  NULL,  dir) ;
        if (ret < 0) {
          fprintf(stderr,"nfi_mpiServer_open: url %s incorrect.\n",url) ;
          mpiServer_err(MPISERVERERR_URL) ;
          return -1;
        }
        fho->url = strdup(url) ;
        NULL_RET_ERR(fho->url, MPISERVERERR_MEMORY) ;

        fh_aux = (struct nfi_mpiServer_fhandle *)malloc(sizeof(struct nfi_mpiServer_fhandle)) ;
        if (fh_aux == NULL) {
          mpiServer_err(MPISERVERERR_MEMORY) ;
          free(fho->url) ;
          return -1;
        }

        server_aux = (struct nfi_mpiServer_server *) serv->private_info;

        /************** LOCAL *****************/
        if(server_aux->params.locality[server_aux->sd.rank_id])
        {
          fh_aux->fd = filesystem_open(dir, O_RDWR) ;
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
          msg.type = MPISERVER_OPEN_FILE;
          strcpy(msg.id, server_aux->id) ;
          strcpy(msg.u_st_mpiServer_msg.op_open.path,dir) ;

          nfi_mpiServer_doRequest(server_aux, &msg, (char *)&(fh_aux->fd), sizeof(int)) ;
          strcpy(fh_aux->path, dir) ;
        }
        /*****************************************/

        fho->type = NFIFILE;
        fho->server = NULL;
        fho->priv_fh = NULL;
        fho->server = serv;
        fho->priv_fh = (void *) fh_aux;

        debug_info("[NFI] nfi_mpiServer_open(ID=%s): end\n",server_aux->id) ;

        return 0;
      }

      int nfi_mpiServer_create(struct nfi_server *serv,  char *url, struct nfi_attr *attr, struct nfi_fhandle  *fh)
      {
        char   server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
        int    ret ;
        struct nfi_mpiServer_server *server_aux;
        struct nfi_mpiServer_fhandle *fh_aux;
        struct st_mpiServer_msg msg;

        // Check arguments...
        NULL_RET_ERR(serv, MPISERVERERR_PARAM) ;
        NULL_RET_ERR(attr, MPISERVERERR_PARAM) ;
        nfi_mpiServer_keepConnected(serv) ;
        NULL_RET_ERR(serv->private_info, MPISERVERERR_PARAM) ;

        // private_info...
        server_aux = (struct nfi_mpiServer_server *) serv->private_info;
        debug_info("[NFI] nfi_mpiServer_create(ID=%s): begin %s\n",server_aux->id,url) ;

        ret = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir) ;
        if (ret < 0) {
          fprintf(stderr,"ERROR: nfi_mpiServer_create: url %s incorrect.\n",url) ;
          mpiServer_err(MPISERVERERR_URL) ;
          return -1;
        }

        /* private_info file handle */
        fh_aux = (struct nfi_mpiServer_fhandle *)malloc(sizeof(struct nfi_mpiServer_fhandle)) ;
        NULL_RET_ERR(fh_aux, MPISERVERERR_MEMORY) ;

        bzero(fh_aux, sizeof(struct nfi_mpiServer_fhandle)) ;

        server_aux = (struct nfi_mpiServer_server *) serv->private_info;
        /* create the file into the directory */

        /************** LOCAL *****************/
        if (server_aux->params.locality[server_aux->sd.rank_id])
        {
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
          msg.type = MPISERVER_CREAT_FILE;
          strcpy(msg.id, server_aux->id) ;
          strcpy(msg.u_st_mpiServer_msg.op_creat.path,dir) ;
          nfi_mpiServer_doRequest(server_aux, &msg, (char *)&(fh_aux->fd), sizeof(int)) ;
          strcpy(fh_aux->path, dir) ;
        }
        /*****************************************/

        fh->type = NFIFILE;
        fh->server = serv;
        fh->priv_fh = (void *)fh_aux;

        fh->url = strdup(url) ;
        if (fh->url == NULL) {
          mpiServer_err(MPISERVERERR_MEMORY) ;
          free(fh_aux) ;
          return -1;
        }

        debug_info("[NFI] nfi_mpiServer_create(ID=%s): end\n",server_aux->id) ;

        return 0;
      }

      ssize_t nfi_mpiServer_read ( struct nfi_server *serv,
                                 struct nfi_fhandle *fh,
                                 void *buffer,
                                 off_t offset,
                                 size_t size )
                                 //off_t offset)
      {

        int ret, cont, diff;
        struct nfi_mpiServer_server *server_aux;
        struct nfi_mpiServer_fhandle *fh_aux;
        struct st_mpiServer_msg msg;
        struct st_mpiServer_read_req req;

        // Check arguments...
        NULL_RET_ERR(serv, MPISERVERERR_PARAM) ;
        NULL_RET_ERR(fh,   MPISERVERERR_PARAM) ;
        nfi_mpiServer_keepConnected(serv) ;
        NULL_RET_ERR(serv->private_info, MPISERVERERR_PARAM) ;

        // private_info
        server_aux = (struct nfi_mpiServer_server *) serv->private_info;
        debug_info("[NFI] nfi_mpiServer_read(%s): begin off %d size %d\n",server_aux->id,(int)offset, (int)size) ;

        fh_aux = (struct nfi_mpiServer_fhandle *) fh->priv_fh;

        /************** LOCAL *****************/
        if (server_aux->params.locality[server_aux->sd.rank_id])
        {
          filesystem_lseek(fh_aux->fd, offset, SEEK_SET) ;
          ret = filesystem_read(fh_aux->fd, buffer, size) ;
          debug_info("read %s(%d) off %ld size %zu (ret:%zd)", fh->url, fh_aux->fd, (long int)offset, size, ret)
          if (ret < 0) {
              debug_error("filesystem_read reads zero bytes from url:%s offset:%ld size:%zu (ret:%zd) errno=%d\n", fh->url, (long int)offset, size, ret, errno) ;
              return -1;
          }
        }
        /************** SERVER ****************/
        else {

          msg.type = MPISERVER_READ_FILE;
          strcpy(msg.id, server_aux->id) ;
          msg.u_st_mpiServer_msg.op_read.fd       = fh_aux->fd;
          msg.u_st_mpiServer_msg.op_read.offset   = offset;
          msg.u_st_mpiServer_msg.op_read.size     = size;

          #ifdef  DBG_IO
            printf("[NFI]read: -> fd %d \n",msg.u_st_mpiServer_msg.op_read.fd) ;
            printf("[NFI]read: -> offset %d \n",(int)msg.u_st_mpiServer_msg.op_read.offset) ;
            printf("[NFI]read: -> size %d \n",msg.u_st_mpiServer_msg.op_read.size) ;
          #endif

          ret = mpiServer_write_operation(server_aux->sd, &msg) ;
          if(ret == -1){
            perror("ERROR: (1)nfi_mpiServer_read: Error on write operation") ;
            fprintf(stderr,"ERROR: (1)nfi_mpiServer_read: Error on write operation\n") ;
            return -1;
          }

          // read n times: number of bytes + read data (n bytes)
          cont = 0 ;

          do {
            ret = mpiClient_read_data(server_aux->sd, (char *)&req, sizeof(struct st_mpiServer_read_req), msg.id) ;
            debug_info("[NFI] nfi_mpiServer_read(ID=%s): (1)mpiClient_read_data = %d.\n",server_aux->id, ret) ;
            if(ret == -1){
              perror("ERROR: (2)nfi_mpiServer_read: Error on write operation") ;
              fprintf(stderr,"ERROR: (2)nfi_mpiServer_read: Error on write operation\n") ;
              return -1;
            }

            if(req.size > 0){
              debug_info("[NFI] nfi_mpiServer_read(ID=%s): (2)mpiClient_read_data = %d. size = %d\n",server_aux->id, ret, req.size) ;
              ret = mpiClient_read_data(server_aux->sd, (char *)buffer+cont, req.size, msg.id) ;
              debug_info("[NFI] nfi_mpiServer_read(ID=%s): (2)mpiClient_read_data = %d.\n",server_aux->id, ret) ;
              if(ret == -1){
                perror("ERROR: (3)nfi_mpiServer_read: Error on write operation") ;
                fprintf(stderr,"ERROR: (3)nfi_mpiServer_read: Error on read operation\n") ;
              }
            }
            cont = cont + req.size ;
            diff = msg.u_st_mpiServer_msg.op_read.size - cont;

          } while ((diff > 0) && (req.size != 0)) ;

          if (req.size < 0)
          {
            fprintf(stderr,"ERROR: nfi_mpiServer_read: Fail read %s off %d size %d (err:%d).\n",fh->url,(int)offset,(int)size,(int)req.size) ;
            mpiServer_err(MPISERVERERR_READ) ;
            return -1;
          }

          ret = cont;
        }

        debug_info("[NFI] nfi_mpiServer_read(ID=%s): end\n",server_aux->id) ;

        return ret;
      }

      ssize_t nfi_mpiServer_write( struct nfi_server *serv,
                                   struct nfi_fhandle *fh,
                                   void *buffer,
                                   off_t offset,
                                   size_t size )
                                   //off_t offset)
      {
        struct nfi_mpiServer_server *server_aux;
        struct nfi_mpiServer_fhandle *fh_aux;
        struct st_mpiServer_msg msg;
        struct st_mpiServer_write_req req;
        int ret, diff, cont;

        // Check arguments...
        if (size == 0){
          return 0;
        }

        NULL_RET_ERR(serv, MPISERVERERR_PARAM) ;
        NULL_RET_ERR(fh,   MPISERVERERR_PARAM) ;
        nfi_mpiServer_keepConnected(serv) ;
        NULL_RET_ERR(serv->private_info, MPISERVERERR_PARAM) ;

        // private_info...
        server_aux = (struct nfi_mpiServer_server *) serv->private_info;
        debug_info("[NFI] nfi_mpiServer_write(ID=%s): begin off %d size %d\n",server_aux->id,(int)offset, (int)size) ;

        fh_aux     = (struct nfi_mpiServer_fhandle *) fh->priv_fh;
        server_aux = (struct nfi_mpiServer_server  *) serv->private_info;

        /************** LOCAL *****************/
        if (server_aux->params.locality[server_aux->sd.rank_id])
        {
          filesystem_lseek(fh_aux->fd, offset, SEEK_SET) ;
          ret = filesystem_read(fh_aux->fd, buffer, size) ;
          debug_info("read %s(%d) off %ld size %zu (ret:%zd)", fh->url, fh_aux->fd, (long int)offset, size, ret)
          if (ret < 0) {
              debug_error("filesystem_read reads zero bytes from url:%s offset:%ld size:%zu (ret:%zd) errno=%d\n", fh->url, (long int)offset, size, ret, errno) ;
              return -1;
          }
        }
        /************** SERVER ****************/
        else {
          msg.type = MPISERVER_WRITE_FILE;
          strcpy(msg.id, server_aux->id) ;
          msg.u_st_mpiServer_msg.op_write.fd     = fh_aux->fd;
          msg.u_st_mpiServer_msg.op_write.offset = offset;
          msg.u_st_mpiServer_msg.op_write.size   = size;

          #ifdef  DBG_IO
            printf("[NFI]write: -> fd %d \n",msg.u_st_mpiServer_msg.op_write.fd) ;
            printf("[NFI]write: -> offset %d \n",(int)msg.u_st_mpiServer_msg.op_write.offset) ;
            printf("[NFI]write: -> size %d \n",msg.u_st_mpiServer_msg.op_write.size) ;
          #endif

          ret = mpiServer_write_operation(server_aux->sd, &msg) ;
          if(ret == -1){
            fprintf(stderr,"(1)ERROR: nfi_mpiServer_write(ID=%s): Error on write operation\n",server_aux->id) ;
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
              ret = mpiClient_write_data(server_aux->sd, (char *)buffer + cont, buffer_size, msg.id) ;
              if(ret == -1){
                fprintf(stderr,"(2)ERROR: nfi_mpiServer_read(ID=%s): Error on write operation\n",server_aux->id) ;
              }
            }
            else{
              ret = mpiClient_write_data(server_aux->sd, (char *)buffer + cont, diff, msg.id) ;
              if(ret == -1){
                fprintf(stderr,"(2)ERROR: nfi_mpiServer_read(ID=%s): Error on write operation\n",server_aux->id) ;
              }
            }

            cont = cont + ret; //Send bytes
            diff = size - cont;

          } while ((diff > 0) && (ret != 0)) ;

          ret = mpiClient_read_data(server_aux->sd, (char *)&req, sizeof(struct st_mpiServer_write_req), msg.id) ;
          if(ret == -1){
            fprintf(stderr,"(3)ERROR: nfi_mpiServer_write(ID=%s): Error on write operation\n",server_aux->id) ;
            return -1;
          }

          debug_info("[NFI] nfi_mpiServer_write(ID=%s): write %s off %d size %d (err:%d).\n",server_aux->id,fh->url,(int)offset,(int)size,(int)req.size) ;
          if (req.size < 0)
          {
            fprintf(stderr,"ERROR: nfi_mpiServer_write(ID=%s): Fail write %s off %d size %d (err:%d).\n",server_aux->id,fh->url,(int)offset,(int)size,(int)req.size) ;
            mpiServer_err(MPISERVERERR_WRITE) ;
            return -1;
          }

          ret = cont;
        }

        debug_info("[NFI] nfi_mpiServer_write(ID=%s): end\n",server_aux->id) ;

        return ret;
      }

      int nfi_mpiServer_close ( struct nfi_server *serv,  struct nfi_fhandle *fh )
      {
        int ret = -1;
        struct nfi_mpiServer_fhandle *fh_aux;
        struct nfi_mpiServer_server *server_aux;
        struct st_mpiServer_msg msg;

        // Check arguments...
        NULL_RET_ERR(serv, MPISERVERERR_PARAM) ;
        NULL_RET_ERR(fh,   MPISERVERERR_PARAM) ;
        nfi_mpiServer_keepConnected(serv) ;
        NULL_RET_ERR(serv->private_info, MPISERVERERR_PARAM) ;

        // private_info...
        server_aux = (struct nfi_mpiServer_server *) serv->private_info;
        debug_info("[NFI] nfi_mpiServer_close(ID=%s): begin\n",server_aux->id) ;

        if (fh->priv_fh != NULL)
        {
          fh_aux = (struct nfi_mpiServer_fhandle *) fh->priv_fh;
          server_aux = (struct nfi_mpiServer_server *) serv->private_info;

          /************** LOCAL *****************/
          if (server_aux->params.locality[server_aux->sd.rank_id])
          {
            if (fh_aux != NULL) {
              filesystem_close(fh_aux->fd) ;
            }
          }
          /************** SERVER ****************/
          else {
            msg.type = MPISERVER_CLOSE_FILE;
            strcpy(msg.id, server_aux->id) ;
            msg.u_st_mpiServer_msg.op_close.fd = fh_aux->fd;
            nfi_mpiServer_doRequest(server_aux, &msg, (char *)&(ret), sizeof(int)) ;
            debug_info("[NFI] nfi_mpiServer_close(ID=%s): close -> %d \n",server_aux->id,msg.u_st_mpiServer_msg.op_close.fd) ;
          }

          /* free memory */
          free(fh->priv_fh) ;
          fh->priv_fh = NULL;
        }

        fh->type = NFINULL;
        fh->server = NULL;

        debug_info("[NFI] nfi_mpiServer_close(ID=%s): end\n",server_aux->id) ;

        // Return OK
        return ret;
      }







      int nfi_mpiServer_remove(struct nfi_server *serv,  char *url)
      {
        char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
        int ret;
        struct nfi_mpiServer_server *server_aux;
        struct st_mpiServer_msg msg;

        // Check arguments...
        NULL_RET_ERR(serv, MPISERVERERR_PARAM) ;
        NULL_RET_ERR(url,  MPISERVERERR_PARAM) ;
        nfi_mpiServer_keepConnected(serv) ;
        NULL_RET_ERR(serv->private_info, MPISERVERERR_PARAM) ;

        // private_info...
        server_aux = (struct nfi_mpiServer_server *) serv->private_info;
        debug_info("[NFI] nfi_mpiServer_remove(%s): begin %s\n",server_aux->id, url) ;
        if (serv == NULL){
          mpiServer_err(MPISERVERERR_PARAM) ;
          return -1;
        }

        server_aux = (struct nfi_mpiServer_server *)serv->private_info;

        ret = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir) ;
        if(ret < 0){
          fprintf(stderr,"nfi_mpiServer_remove: url %s incorrect.\n",url) ;
          mpiServer_err(MPISERVERERR_URL) ;
          return -1;
        }

        /* remove the file into the directory */
        //ret = unlink(dir) ;
        /*****************************************/
        msg.type = MPISERVER_RM_FILE;
        strcpy(msg.id, server_aux->id) ;
        strcpy(msg.u_st_mpiServer_msg.op_rm.path,dir) ;

        //mpiServer_write_operation(server_aux->sd, &msg) ;
        nfi_mpiServer_doRequest(server_aux, &msg, (char *)&(ret), sizeof(int)) ;
        debug_info("[NFI] nfi_mpiServer_remove(ID=%s): end \n",server_aux->id) ;
        /*****************************************/

        return ret;
      }

      int nfi_mpiServer_rename(struct nfi_server *server,  char *old_url, char *new_url)
      {
        /*
          struct nfi_mpiServer_server *server_aux;
          struct nfi_mpiServer_fhandle *fh_aux;

          // Check arguments...
          NULL_RET_ERR(serv,     MPISERVERERR_PARAM) ;
          NULL_RET_ERR(old_url,  MPISERVERERR_PARAM) ;
          NULL_RET_ERR(new_url,  MPISERVERERR_PARAM) ;
          nfi_mpiServer_keepConnected(serv) ;
          NULL_RET_ERR(serv->private_info, MPISERVERERR_PARAM) ;

          // private_info...
          server_aux = (strcut nfi_mpiServer_server *)serv->private_info;

          // TODO: rename
        */

          return 0;
      }

      int nfi_mpiServer_getattr ( struct nfi_server *serv,  struct nfi_fhandle *fh, struct nfi_attr *attr )
      {
          struct nfi_mpiServer_server *server_aux;
          struct nfi_mpiServer_fhandle *fh_aux;
          struct st_mpiServer_msg msg;
          struct st_mpiServer_attr_req req;

          debug_info("[NFI] nfi_mpiServer_getattr (...): begin\n") ;

          // check arguments...
          NULL_RET_ERR(serv,            MPISERVERERR_PARAM) ;
          NULL_RET_ERR(fh,              MPISERVERERR_PARAM) ;
          NULL_RET_ERR(attr,            MPISERVERERR_PARAM) ;
          NULL_RET_ERR(fh->priv_fh,     MPISERVERERR_PARAM) ;
          nfi_mpiServer_keepConnected(serv) ;
          NULL_RET_ERR(serv->private_info, MPISERVERERR_PARAM) ;

          // copy private information...
          server_aux = (struct nfi_mpiServer_server *) serv->private_info;
          fh_aux = (struct nfi_mpiServer_fhandle *) fh->priv_fh;
          server_aux = (struct nfi_mpiServer_server *) serv->private_info;

          /*****************************************/
          msg.type = MPISERVER_GETATTR_FILE;
          strcpy(msg.id, server_aux->id) ;
          strcpy(msg.u_st_mpiServer_msg.op_getattr.path, fh_aux->path) ;

          nfi_mpiServer_doRequest(server_aux, &msg, (char *)&req, sizeof(struct st_mpiServer_attr_req)) ;
          /*****************************************/

          MPISERVERtoNFIattr(attr, &req.attr) ;

          debug_info("[NFI] nfi_mpiServer_getattr (...): end\n") ;

          // return status
          return req.status;
      }

      int nfi_mpiServer_setattr ( struct nfi_server *serv,  struct nfi_fhandle *fh, struct nfi_attr *attr )
      {
          struct nfi_mpiServer_server *server_aux;
          struct nfi_mpiServer_fhandle *fh_aux;

          // Check arguments...
          NULL_RET_ERR(serv, MPISERVERERR_PARAM) ;
          NULL_RET_ERR(fh,   MPISERVERERR_PARAM) ;
          NULL_RET_ERR(attr, MPISERVERERR_PARAM) ;
          NULL_RET_ERR(fh->priv_fh, MPISERVERERR_PARAM) ;
          nfi_mpiServer_keepConnected(serv) ;
          NULL_RET_ERR(serv->private_info, MPISERVERERR_PARAM) ;

          fh_aux = (struct nfi_mpiServer_fhandle *) fh->priv_fh;
          server_aux = (struct nfi_mpiServer_server *) serv->private_info;

          // TODO: setattr...

          // Return OK
          return 0;
      }

      int nfi_mpiServer_opendir(struct nfi_server *serv,  char *url, struct nfi_fhandle *fho)
      {
          char dir[NFIMAXPATHLEN], server[NFIMAXPATHLEN];
          int ret;
          struct nfi_mpiServer_server *server_aux;
          struct nfi_mpiServer_fhandle *fh_aux;

          // Check arguments...
          NULL_RET_ERR(serv, MPISERVERERR_PARAM) ;
          NULL_RET_ERR(url,  MPISERVERERR_PARAM) ;
          NULL_RET_ERR(fho,  MPISERVERERR_PARAM) ;
          nfi_mpiServer_keepConnected(serv) ;
          NULL_RET_ERR(serv->private_info, MPISERVERERR_PARAM) ;

          // private_info...
          ret = ParseURL(url, NULL, NULL, NULL, server,  NULL,  dir) ;
          if (ret < 0) {
                fprintf(stderr,"nfi_mpiServer_opendir: url %s incorrect.\n",url) ;
                mpiServer_err(MPISERVERERR_URL) ;
                return -1;
          }

          fho->url = strdup(url) ;
          NULL_RET_ERR(fho->url, MPISERVERERR_MEMORY) ;

          fh_aux = (struct nfi_mpiServer_fhandle *)malloc(sizeof(struct nfi_mpiServer_fhandle)) ;
          if (fh_aux == NULL){
                mpiServer_err(MPISERVERERR_MEMORY) ;
                free(fho->url) ;
                return -1;
          }

          server_aux = (struct nfi_mpiServer_server *) serv->private_info;

          fh_aux->dir = opendir(dir) ;
          if (ret < 0 )
          {
                fprintf(stderr,"nfi_mpiServer_opendir: Fail opendir %s in server %s.\n",dir,serv->server) ;
                mpiServer_err(MPISERVERERR_MEMORY) ;
                free(fh_aux) ;
                free(fho->url) ;
                return -1;
          }

          fh_aux->fd = ret;
          strcpy(fh_aux->path, dir) ;
          fho->type = NFIDIR;

          fho->server = NULL;
          fho->priv_fh = NULL;
          fho->server = serv;
          fho->priv_fh = (void *) fh_aux;

          return 0;
      }

      int nfi_mpiServer_mkdir(struct nfi_server *serv,  char *url, struct nfi_attr *attr, struct nfi_fhandle *fh)
      {
          char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
          int ret;
          struct nfi_mpiServer_server *server_aux;
          struct nfi_mpiServer_fhandle *fh_aux;
          struct st_mpiServer_msg msg;

          // Check arguments...
          NULL_RET_ERR(serv, MPISERVERERR_PARAM) ;
          NULL_RET_ERR(attr, MPISERVERERR_PARAM) ;
          nfi_mpiServer_keepConnected(serv) ;
          NULL_RET_ERR(serv->private_info, MPISERVERERR_PARAM) ;

          // private_info...
          server_aux = (struct nfi_mpiServer_server *)serv->private_info;

          ret = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir) ;
          if (ret < 0) {
                fprintf(stderr,"nfi_mpiServer_mkdir: url %s incorrect.\n",url) ;
                mpiServer_err(MPISERVERERR_URL) ;
                return -1;
          }

          /* private_info file handle */
          fh_aux = (struct nfi_mpiServer_fhandle *)malloc(sizeof(struct nfi_mpiServer_fhandle)) ;
          NULL_RET_ERR(fh_aux, MPISERVERERR_MEMORY) ;

          bzero(fh_aux, sizeof(struct nfi_mpiServer_fhandle)) ;
          /* create the dir into the directory */

          /******************************************************/
          msg.type = MPISERVER_MKDIR_DIR;
          strcpy(msg.u_st_mpiServer_msg.op_mkdir.path, dir) ;

          nfi_mpiServer_doRequest(server_aux, &msg, (char *)&(fh_aux->fd), sizeof(int)) ;
          strcpy(fh_aux->path, dir) ;
          /******************************************************/

        if ((ret < 0)&&(errno != EEXIST)) {
                mpiServer_err(MPISERVERERR_MKDIR) ;
                fprintf(stderr,"nfi_mpiServer_mkdir: Fail mkdir %s in server %s.\n",dir,serv->server) ;
                free(fh_aux) ;
                return -1;
        }

        fh->type = NFIDIR;
        fh->priv_fh = (void *)fh_aux;

        fh->url = strdup(url) ;
        if(fh->url == NULL){
               mpiServer_err(MPISERVERERR_MEMORY) ;
               free(fh_aux) ;
               return -1;
        }

        //TODO:
        //MPISERVERtoNFIattr(attr, &st) ;

        return 0;
      }

      int nfi_mpiServer_readdir(struct nfi_server *serv,  struct nfi_fhandle *fh, char *entry, unsigned char *type)
      {
          struct dirent *ent;
          struct nfi_mpiServer_server *server_aux;
          struct nfi_mpiServer_fhandle *fh_aux;

          // Check arguments...
          NULL_RET_ERR(serv,        MPISERVERERR_PARAM) ;
          NULL_RET_ERR(fh,          MPISERVERERR_PARAM) ;
          NULL_RET_ERR(fh->priv_fh, MPISERVERERR_PARAM) ;
          if (fh->type != NFIDIR) {
                mpiServer_err(MPISERVERERR_NOTDIR) ;
                return -1;
          }
          nfi_mpiServer_keepConnected(serv) ;
          NULL_RET_ERR(serv->private_info, MPISERVERERR_PARAM) ;

          // private_info...
          server_aux = (struct nfi_mpiServer_server *)serv->private_info;
          fh_aux = (struct nfi_mpiServer_fhandle *)fh->priv_fh;

          entry[0] = '\0';
          ent = readdir(fh_aux->dir) ;

          if(ent == NULL){
                return 1;
          }
          if(type==NULL){
                return 0;
          }

          strcpy(entry, ent->d_name) ;
          //printf("[NFI]ent->d_name = %s S_ISDIR(%o) = %o\n", ent->d_name, ent->d_type,S_ISDIR(ent->d_type)) ;
//        *type = ent->d_type;

          return 0;
      }

      int nfi_mpiServer_closedir ( struct nfi_server *serv,  struct nfi_fhandle *fh )
      {
          struct nfi_mpiServer_fhandle *fh_aux;

          // Check arguments...
          NULL_RET_ERR(serv, MPISERVERERR_PARAM) ;
          NULL_RET_ERR(fh,   MPISERVERERR_PARAM) ;
          nfi_mpiServer_keepConnected(serv) ;
          NULL_RET_ERR(serv->private_info, MPISERVERERR_PARAM) ;

          // do closedir...
          if (fh->priv_fh != NULL){
                fh_aux = (struct nfi_mpiServer_fhandle *) fh->priv_fh;
                closedir(fh_aux->dir) ;
                /* free memory */
                free(fh->priv_fh) ;
                fh->priv_fh = NULL;

          }

          return 0;
      }

      int nfi_mpiServer_rmdir(struct nfi_server *serv,  char *url)
      {
          int ret;
          struct nfi_mpiServer_server *server_aux;
          struct st_mpiServer_msg msg;
          char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];

          // Check arguments...
          NULL_RET_ERR(serv, MPISERVERERR_PARAM) ;
          NULL_RET_ERR(url,  MPISERVERERR_PARAM) ;
          nfi_mpiServer_keepConnected(serv) ;
          NULL_RET_ERR(serv->private_info, MPISERVERERR_PARAM) ;

          // private_info...
          server_aux = (struct nfi_mpiServer_server *)serv->private_info;

          ret = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir) ;
          if (ret < 0) {
                fprintf(stderr,"nfi_mpiServer_rmdir: url %s incorrect.\n",url) ;
                mpiServer_err(MPISERVERERR_URL) ;
                return -1;
          }

        /******************************************************/
        msg.type = MPISERVER_RMDIR_DIR;
        strcpy(msg.u_st_mpiServer_msg.op_rmdir.path, url) ;

        nfi_mpiServer_doRequest(server_aux, &msg, (char *)&(ret), sizeof(int)) ;
        /******************************************************/


        //ret = rmdir(dir) ;
        if(ret < 0){
                fprintf(stderr,"nfi_mpiServer_rmdir: Fail rmdir %s in server %s.\n",dir,serv->server) ;
                mpiServer_err(MPISERVERERR_REMOVE) ;
                return -1;
        }

        return 0;
      }

      int nfi_mpiServer_preload(struct nfi_server *serv, char *url, char *virtual_path, char *storage_path, int opt)
      {
          //char dir[NFIMAXPATHLEN];
          int ret;
          struct nfi_mpiServer_server *server_aux;
          struct st_mpiServer_msg msg;

          // Check arguments...
          NULL_RET_ERR(serv, MPISERVERERR_PARAM) ;
          NULL_RET_ERR(url,  MPISERVERERR_PARAM) ;
          NULL_RET_ERR(virtual_path, MPISERVERERR_PARAM) ;
          NULL_RET_ERR(storage_path, MPISERVERERR_PARAM) ;
          nfi_mpiServer_keepConnected(serv) ;
          NULL_RET_ERR(serv->private_info, MPISERVERERR_PARAM) ;

          // get private_info...
          server_aux = (struct nfi_mpiServer_server *) serv->private_info;
          debug_info("[NFI] nfi_mpiServer_preload(ID=%s): begin %s - %s \n", server_aux->id, virtual_path, storage_path) ;

          if (url[strlen(url)-1] == '/'){
                mpiServer_err(MPISERVERERR_PARAM) ;
                return -1;
          }

          server_aux = (struct nfi_mpiServer_server *) serv->private_info;
          debug_info("[NFI] nfi_mpiServer_preload(ID=%s): preload %s in server %s.\n",server_aux->id,virtual_path,serv->server) ;

          /*****************************************/
          msg.type = MPISERVER_PRELOAD_FILE;
          strcpy(msg.id, server_aux->id) ;
          //strcpy(msg.u_st_mpiServer_msg.op_preload.path,dir) ;
          strcpy(msg.u_st_mpiServer_msg.op_preload.virtual_path,virtual_path) ;
          strcpy(msg.u_st_mpiServer_msg.op_preload.storage_path,storage_path) ;
          msg.u_st_mpiServer_msg.op_preload.block_size = serv->block_size;
          msg.u_st_mpiServer_msg.op_preload.opt        = opt;

          nfi_mpiServer_doRequest(server_aux, &msg, (char *)&ret, sizeof(int)) ;
          /*****************************************/

          debug_info("[NFI] nfi_mpiServer_preload(ID=%s): end %s - %s = %d\n", server_aux->id,virtual_path, storage_path, ret) ;
          if (ret == -1) {
              printf("[NFI]Error en el preload\n") ;
          }

          return ret;
      }

      int nfi_mpiServer_flush ( struct nfi_server *serv,  char *url, char *virtual_path, char *storage_path, int opt )
      {
          //char dir[NFIMAXPATHLEN];
          int ret;
          struct nfi_mpiServer_server *server_aux;
          struct st_mpiServer_msg msg;

          // Check arguments...
          NULL_RET_ERR(serv, MPISERVERERR_PARAM) ;
          NULL_RET_ERR(url,  MPISERVERERR_PARAM) ;
          NULL_RET_ERR(virtual_path, MPISERVERERR_PARAM) ;
          NULL_RET_ERR(storage_path, MPISERVERERR_PARAM) ;
          nfi_mpiServer_keepConnected(serv) ;
          NULL_RET_ERR(serv->private_info, MPISERVERERR_PARAM) ;

          // private_info...
          server_aux = (struct nfi_mpiServer_server *) serv->private_info;
          debug_info("[NFI] nfi_mpiServer_flush(ID=%s): begin %s - %s \n", server_aux->id,virtual_path, storage_path) ;

          if (url[strlen(url)-1] == '/') {
              mpiServer_err(MPISERVERERR_PARAM) ;
              return -1;
          }

          server_aux = (struct nfi_mpiServer_server *) serv->private_info;
          debug_info("[NFI] nfi_mpiServer_flush(ID=%s): open %s in server %s.\n",server_aux->id,virtual_path,serv->server) ;

          /*****************************************/
          msg.type = MPISERVER_FLUSH_FILE;
          strcpy(msg.id, server_aux->id) ;
          //strcpy(msg.u_st_mpiServer_msg.op_flush.path,dir) ;
          strcpy(msg.u_st_mpiServer_msg.op_flush.virtual_path,virtual_path) ;
          strcpy(msg.u_st_mpiServer_msg.op_flush.storage_path,storage_path) ;
          msg.u_st_mpiServer_msg.op_flush.block_size = serv->block_size;
          msg.u_st_mpiServer_msg.op_flush.opt = opt;

          nfi_mpiServer_doRequest(server_aux, &msg, (char *)&ret, sizeof(int)) ;
          /*****************************************/

          debug_info("[NFI] nfi_mpiServer_flush(ID=%s): end %s - %s = %d\n", server_aux->id,virtual_path, storage_path, ret) ;

          return 0;
      }














      int nfi_mpiServer_statfs(struct nfi_server *serv,  struct nfi_info *inf)
      {
/*
          struct mpiServer_info mpiServerinf;
          int ret;
          struct nfi_mpiServer_server *server_aux;

          // Check arguments...
          NULL_RET_ERR(serv, MPISERVERERR_PARAM) ;
          NULL_RET_ERR(inf,  MPISERVERERR_PARAM) ;
          nfi_mpiServer_keepConnected(serv) ;
          NULL_RET_ERR(serv->private_info, MPISERVERERR_PARAM) ;

          // private_info...
          server_aux = (struct nfi_mpiServer_server *)serv->private_info;

          ret = mpiServer_statfs(server_aux->fh, &mpiServerinf, server_aux->cl) ;
          if (ret <0).{
                mpiServer_err(MPISERVERERR_STATFS) ;
                return -1;
          }

          MPItoNFIInfo(inf, &mpiServerinf) ;
*/

          return 0;
      }


   /* ................................................................... */

