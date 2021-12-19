
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
       * Debug
       */

#ifdef DBG_NFI
  #define dbgnfi_error(...)    fprintf(stderr, __VA_ARGS__)
  #define dbgnfi_warning(...)  fprintf(stderr, __VA_ARGS__)
  #define dbgnfi_info(...)     fprintf(stdout, __VA_ARGS__)
#else
  #define dbgnfi_error(...)
  #define dbgnfi_warning(...)
  #define dbgnfi_info(...)
#endif


      /*
       * Communication
       */

      int mpiServer_comm_init ( mpiServer_param_st *params )  // TODO: connect to nfi
      {
              int ret, provided ;
      	      char srv_name[1024] ;
              MPI_Info info ;
      
              debug_info("[COMM] begin mpiServer_comm_init(...)\n") ;
      
              // MPI_Init
              ret = MPI_Init_thread(&(params->argc), &(params->argv), MPI_THREAD_MULTIPLE, &provided) ;
              if (MPI_SUCCESS != ret) {
                  debug_error("Server[%d]: MPI_Init fails :-(", -1) ;
                  return -1 ;
              }
      
              // params->rank = comm_rank()
              ret = MPI_Comm_rank(MPI_COMM_WORLD, &(params->rank)) ;
              if (MPI_SUCCESS != ret) {
                  debug_error("Server[%d]: MPI_Comm_rank fails :-(", params->rank) ;
                  return -1 ;
              }
      
              // params->size = comm_size()
              ret = MPI_Comm_size(MPI_COMM_WORLD, &(params->size)) ;
              if (MPI_SUCCESS != ret) {
                  debug_error("Server[%d]: MPI_Comm_size fails :-(", params->rank) ;
                  return -1 ;
              }
      
              // Open server port...
              ret = MPI_Open_port(MPI_INFO_NULL, params->port_name) ;
              if (MPI_SUCCESS != ret) {
                  debug_error("Server[%d]: MPI_Open_port fails :-(", params->rank) ;
                  return -1 ;
              }
      
              // Publish port name
              MPI_Info_create(&info) ;
              MPI_Info_set(info, "ompi_global_scope", "true") ;
              sprintf(srv_name, "mpiServer.%d", params->rank) ;
      
              ret = MPI_Publish_name(srv_name, info, params->port_name) ;
              if (MPI_SUCCESS != ret) {
                  debug_error("Server[%d]: MPI_Publish_name fails :-(", params->rank) ;
                  return -1 ;
              }
      
              debug_info("[COMM] server %d available at %s\n", params->rank, params->port_name) ;
              debug_info("[COMM] server %d accepting...\n",    params->rank) ;
              debug_info("[COMM] end mpiServer_comm_init(...)\n") ;
      
      	      // Return OK
      	      return 1 ;
      }

      int mpiServer_comm_destroy ( mpiServer_param_st *params ) // TODO: connect to nfi
      {
              int ret ;
      
              debug_info("[COMM] begin mpiServer_comm_destroy(...)\n") ;
      
              // Close port
              MPI_Close_port(params->port_name) ;
      
              // Unpublish port name
              ret = MPI_Unpublish_name(params->srv_name, MPI_INFO_NULL, params->port_name) ;
              if (MPI_SUCCESS != ret) {
                  debug_error("Server[%d]: MPI_Unpublish_name fails :-(", params->rank) ;
                  return -1 ;
              }
      
              // Finalize
              ret = MPI_Finalize() ;
              if (MPI_SUCCESS != ret) {
                  debug_error("Server[%d]: MPI_Finalize fails :-(", params->rank) ;
                  return -1 ;
              }
      
              debug_info("[COMM] end mpiServer_comm_destroy(...)\n") ;
      
              // Return OK
              return 1 ;
      }

      int mpiServer_comm_accept ( mpiServer_param_st *params ) // TODO: connect to NFI
      {
      	      int ret ;
      
              debug_info("[COMM] begin mpiServer_accept_comm(...)\n") ;
      
              // Accept
              ret = MPI_Comm_accept(params->port_name, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &(params->client)) ;
              if (MPI_SUCCESS != ret) {
                  debug_error("Server[%d]: MPI_Comm_accept fails :-(", params->rank) ;
                  return -1 ;
              }
      
              debug_info("[COMM] end mpiServer_accept_comm(...)\n") ;
      
              // Return client MPI_Comm
      	      return (int)(params->client) ;
      }
      
      int mpiServer_comm_close ( mpiServer_param_st *params ) // TODO: connect to NFI
      {
              int ret ;
      
              // Disconnect
              ret = MPI_Comm_disconnect(&(params->client)) ;
              if (MPI_SUCCESS != ret) {
                  debug_error("Server[%d]: MPI_Comm_disconnect fails :-(", params->rank) ;
                  return -1 ;
              }
      
              // Return OK
              return 1 ;
      }

      ssize_t mpiServer_write_data ( int fd, char *data, ssize_t size, char *msg_id )
      {
      	      int ret ;
      
      	      debug_info("[COMM] server: begin comm_write_data(...)\n") ;
      
      	      // Check params
      	      if (size == 0){
      	          return 0;
      	      }
      	      if (size < 0){
                  debug_warning("Server[?]: size < 0") ;
      	          return -1;
      	      }
      
              // Send message
              ret = MPI_Send(data, size, MPI_CHAR, 0, 0, (MPI_Comm)fd) ;
              if (MPI_SUCCESS != ret) {
                  debug_warning("Server[?]: MPI_Recv fails :-(") ;
              }
      
      	      debug_info("[COMM] server: end comm_write_data(...)\n") ;
      
              // Return bytes written
      	      return size;
      }

      ssize_t mpiServer_read_data ( int fd, char *data, ssize_t size, char *msg_id )
      {
      	      int ret ;
      	      MPI_Status status ;
      
      	      debug_info("[COMM] server: begin comm_read_data(...)\n") ;
      
      	      // Check params
      	      if (size == 0) {
      	          return  0;
      	      }
      	      if (size < 0){
                  debug_warning("Server[?]: size < 0") ;
      	          return  -1;
      	      }
      
              // Get message
      	      ret = MPI_Recv(data, size, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, (MPI_Comm)fd, &status);
              if (MPI_SUCCESS != ret) {
                  debug_warning("Server[?]: MPI_Recv fails :-(") ;
              }

	      // TODO: status holds the mpi_rank and tag to be used to answer in case of a high-level function that reads a request... how we can return (size, rank, tag, ...) with this 'descriptor'-based interface?
      
      	      debug_info("[COMM] server: end comm_read_data(...)\n") ;
      
              // Return bytes read
      	      return size;
      }

      /**********************************
       Write the operation to realize
      ***********************************/
      int mpiServer_write_operation ( int sd, struct st_mpiServer_msg *head )
      {
        int ret;

	dbgnfi_info("[NFI] (ID=%s) mpiServer_write_data: begin\n", head->id);

        ret = mpiServer_write_data(sd, (char *)&head->type, sizeof(head->type), head->id);
        if(ret == -1){
                return -1;
        }

        dbgnfi_info("[NFI] (ID=%s) write_operation: %d -> \n", head->id, head->type);
        switch (head->type)
	{
		case MPISERVER_OPEN_FILE:
			dbgnfi_info("[NFI] (ID=%s) OPEN operation\n",head->id);
			ret = mpiServer_write_data(sd, (char *)&head->u_st_mpiServer_msg.op_open, sizeof(struct st_mpiServer_open), head->id);
			if(ret == -1){
				return -1;
			}
			break;
		case MPISERVER_CREAT_FILE:
			dbgnfi_info("[NFI] (ID=%s) CREAT operation\n",head->id);
			ret = mpiServer_write_data(sd, (char *)&head->u_st_mpiServer_msg.op_creat, sizeof(struct st_mpiServer_creat), head->id);
			if(ret == -1){
				return -1;
			}
			break;
		case MPISERVER_READ_FILE:
			dbgnfi_info("[NFI] (ID=%s) READ operation\n",head->id);
			ret = mpiServer_write_data(sd, (char *)&head->u_st_mpiServer_msg.op_read, sizeof(struct st_mpiServer_read), head->id);
			if(ret == -1){
				return -1;
			}
			break;
		case MPISERVER_WRITE_FILE:
			dbgnfi_info("[NFI] (ID=%s) WRITE operation\n", head->id);
			ret = mpiServer_write_data(sd, (char *)&head->u_st_mpiServer_msg.op_write, sizeof(struct st_mpiServer_write), head->id);
			if(ret == -1){
				return -1;
			}
			break;
		case MPISERVER_CLOSE_FILE:
			dbgnfi_info("[NFI] (ID=%s) CLOSE operation\n",head->id);
			ret = mpiServer_write_data(sd, (char *)&head->u_st_mpiServer_msg.op_close, sizeof(struct st_mpiServer_close), head->id);
			if(ret == -1){
				return -1;
			}
			break;
		case MPISERVER_RM_FILE:
			dbgnfi_info("[NFI] (ID=%s) RM operation\n",head->id);
			ret = mpiServer_write_data(sd, (char *)&head->u_st_mpiServer_msg.op_rm, sizeof(struct st_mpiServer_rm), head->id);
			if(ret == -1){
				return -1;
			}
			break;
		case MPISERVER_GETATTR_FILE:
			dbgnfi_info("[NFI] (ID=%s) GETATTR operation\n",head->id);
			ret = mpiServer_write_data(sd, (char *)&head->u_st_mpiServer_msg.op_getattr, sizeof(struct st_mpiServer_getattr), head->id);
			if(ret == -1){
				return -1;
			}
			break;
		case MPISERVER_MKDIR_DIR:
			dbgnfi_info("[NFI] (ID=%s) MDKIR operation\n",head->id);
			ret = mpiServer_write_data(sd, (char *)&head->u_st_mpiServer_msg.op_mkdir, sizeof(struct st_mpiServer_mkdir), head->id);
			if(ret == -1){
				return -1;
			}
			break;
		case MPISERVER_RMDIR_DIR:
			dbgnfi_info("[NFI] (ID=%s) RMDIR operation\n",head->id);
			ret = mpiServer_write_data(sd, (char *)&head->u_st_mpiServer_msg.op_rmdir, sizeof(struct st_mpiServer_rmdir), head->id);
			if(ret == -1){
				return -1;
			}
			break;
		case MPISERVER_FLUSH_FILE:
			dbgnfi_info("[NFI] (ID=%s) FLUSH operation\n",head->id);
			ret = mpiServer_write_data(sd, (char *)&head->u_st_mpiServer_msg.op_flush, sizeof(struct st_mpiServer_flush), head->id);
			if(ret == -1){
				return -1;
			}
			break;
		case MPISERVER_PRELOAD_FILE:
			dbgnfi_info("[NFI] (ID=%s) PRELOAD operation\n",head->id);
			ret = mpiServer_write_data(sd, (char *)&head->u_st_mpiServer_msg.op_preload, sizeof(struct st_mpiServer_preload), head->id);
			if(ret == -1){
				return -1;
			}
			break;
		case MPISERVER_FINALIZE:
			dbgnfi_info("[NFI] (ID=%s) FINALIZE operation\n",head->id);
		/*
			ret = mpiServer_write_data(sd, (char *)&head->u_st_mpiServer_msg.op_end, sizeof(struct st_mpiServer_end), head->id);
			if(ret == -1){
				return -1;
			}
		*/
			break;
		case MPISERVER_GETID:
			dbgnfi_info("[NFI] (ID=%s) GETID operation\n",head->id);
		/*
			ret = mpiServer_write_data(sd, (char *)head->id, MPISERVER_ID, head->id);
			if(ret == -1){
				return -1;
			}
		*/
			break;
		case MPISERVER_END:
			dbgnfi_info("[NFI] (ID=%s) END operation\n",head->id);
		/*
			ret = mpiServer_write_data(sd, (char *)&head->u_st_mpiServer_msg.op_end, sizeof(struct st_mpiServer_end), head->id);
			if(ret == -1){
				return -1;
			}
		*/
			break;
        }

        return 0;
        //return head->type;
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

	att->st_size	= nfi_att->at_size;      /* size */
	att->st_uid	= nfi_att->at_uid;      /* user ID of owner */
	att->st_gid	= nfi_att->at_gid;      /* group ID of owner */
	att->st_blksize	= nfi_att->at_blksize;  /* blocksize for filesystem I/O */
	att->st_blocks	= nfi_att->at_blocks;   /* number of blocks allocated */
	att->st_atime	= nfi_att->at_atime;    /* time of last access */
	att->st_mtime	= nfi_att->at_mtime;    /* time of last modification */
	att->st_ctime	= nfi_att->at_ctime;    /* time of last change */
      }

      void MPISERVERtoNFIattr ( struct nfi_attr *nfi_att, struct stat *att )
      {
	if(S_ISREG(att->st_mode)){
		nfi_att->at_type = NFIFILE;
	}
        if(S_ISDIR(att->st_mode)){
		nfi_att->at_type = NFIDIR;
        }

	nfi_att->at_size = att->st_size;      /* size */
        nfi_att->at_mode = att->st_mode&(S_IRWXU|S_IRWXG|S_IRWXO); /* protection */
        nfi_att->at_uid = att->st_uid;      /* user ID of owner */
        nfi_att->at_gid = att->st_gid;      /* group ID of owner */
        nfi_att->at_blksize = att->st_blksize;  /* blocksize for filesystem I/O*/
        nfi_att->at_blocks = att->st_blocks;   /* number of blocks allocated */
	nfi_att->at_atime = att->st_atime;    /* time of last access */
        nfi_att->at_mtime = att->st_mtime;    /* time of last modification */
        nfi_att->at_ctime = att->st_ctime;    /* time of last change */
      }

      void MPISERVERtoNFIInfo(struct nfi_info *nfi_inf, struct nfi_info *mpiServer_inf)
      {
      }


      /*
       * PUBLIC FUNCTIONS TO USE MPISERVER
       */

       /************************************************************
        * Init Local 					    	    *
        ************************************************************/
      int nfi_mpiServer_init ( char *url, struct nfi_server *serv, struct nfi_attr_server *attr )
      {
	int ret;
	struct nfi_mpiServer_server *server_aux;
	struct st_mpiServer_msg msg;
	char server[MAXPATHLEN], dir[MAXPATHLEN], prt[MAXPATHLEN];


	dbgnfi_info("[NFI] nfi_mpiServer_init: begin\n");

	// check arguments...
	if (serv == NULL) {
	    mpiServer_err(MPISERVERERR_PARAM);
	    return -1;
	}

	// new nfi_ops with mpiServer functions...
	serv->ops = (struct nfi_ops *)malloc(sizeof(struct nfi_ops));
	if (serv->ops == NULL){
		mpiServer_err(MPISERVERERR_MEMORY);
		return -1;
	}
	bzero(serv->ops, sizeof(struct nfi_ops));

	serv->ops->nfi_preload	  = nfi_mpiServer_preload;
	serv->ops->nfi_flush	  = nfi_mpiServer_flush;
	serv->ops->nfi_reconnect  = nfi_mpiServer_reconnect;
	serv->ops->nfi_disconnect = nfi_mpiServer_disconnect;
	serv->ops->nfi_getattr	  = nfi_mpiServer_getattr;
	serv->ops->nfi_setattr	  = nfi_mpiServer_setattr;
	serv->ops->nfi_open	  = nfi_mpiServer_open;
	serv->ops->nfi_close	  = nfi_mpiServer_close;
	serv->ops->nfi_read	  = nfi_mpiServer_read;
	serv->ops->nfi_write	  = nfi_mpiServer_write;
	serv->ops->nfi_create	  = nfi_mpiServer_create;
	serv->ops->nfi_remove	  = nfi_mpiServer_remove;
	serv->ops->nfi_rename	  = nfi_mpiServer_rename;
	serv->ops->nfi_mkdir	  = nfi_mpiServer_mkdir;
	serv->ops->nfi_rmdir	  = nfi_mpiServer_rmdir;
	serv->ops->nfi_opendir	  = nfi_mpiServer_opendir;
	serv->ops->nfi_readdir	  = nfi_mpiServer_readdir;
	serv->ops->nfi_closedir	  = nfi_mpiServer_closedir;
	serv->ops->nfi_statfs	  = nfi_mpiServer_statfs;

	// parse url...
	ret = ParseURL(url, prt, NULL, NULL, server, NULL, dir);
	if (ret < 0)
	{
		mpiServer_err(MPISERVERERR_URL);
		fprintf(stderr,"nfi_mpiServer_init: incorrect url '%s'.\n", url);
		free(serv->ops);
		return -1;
	}

	// new nfi_mpiserver_server...
	server_aux = (struct nfi_mpiServer_server *)malloc(sizeof(struct nfi_mpiServer_server));
	if(server_aux == NULL){
		mpiServer_err(MPISERVERERR_MEMORY);
		return -1;
	}

	server_aux->sd = mpiServer_connect(server);
	strcpy(msg.id, "GETID");
        msg.type = MPISERVER_GETID;
        mpiServer_write_operation(server_aux->sd, &msg);
	dbgnfi_info("[NFI] nfi_mpiServer_init: mpiServer_write_data\n");
	mpiServer_read_data(server_aux->sd, (char *)server_aux->id, MPISERVER_ID, msg.id);
	dbgnfi_info("[NFI] nfi_mpiServer_init: mpiServer_read_data id = %s\n",server_aux->id);

	serv->private_info = (void *)server_aux;
	//serv->protocol = MPISERVER;

	// copy 'server address' string...
	serv->server = (char *)malloc(sizeof(char)*(strlen(server)+1));	/* server address */
	if(serv->server == NULL){
		mpiServer_err(MPISERVERERR_MEMORY);
		return -1;
	}
	strcpy(serv->server, server);

	// copy 'url' string...
	serv->url = (char *)malloc(sizeof(char)*(strlen(url)+1));	/* server address */
	if(serv->url == NULL){
		mpiServer_err(MPISERVERERR_MEMORY);
		return -1;
	}
	strcpy(serv->url, url);

	// new server wrk...
        serv->wrk = (struct nfi_worker *)malloc(sizeof(struct nfi_worker));
	memset(serv->wrk, 0, sizeof(struct nfi_worker));

#ifdef _ALL_THREADS_
//	nfi_worker_init(serv->wrk, serv, 1);
//	nfi_worker_init(serv->wrk, serv, 0);
#else
	if(strcmp("mpiServer", prt) == 0){
		dbgnfi_info("[NFI] mpiServer\n");
	        dbgnfi_info("[NFI] nfi_worker_init(1,ID=%s): \n",server_aux->id);
		nfi_worker_init(serv->wrk, serv, 1);
	}
	else{
	        dbgnfi_info("[NFI] nfi_worker_init(0,ID=%s): \n",server_aux->id);
		nfi_worker_init(serv->wrk, serv, 0);
	}
#endif

	dbgnfi_info("[NFI] nfi_mpiServer_init(ID=%s): end\n",server_aux->id);

	// return OK
	return 0;
      }

      /************************************************************
       * Disconnect to the server				    *
       * **********************************************************/
      int nfi_mpiServer_disconnect ( struct nfi_server *serv )
      {
          struct nfi_mpiServer_server *server_aux;

	  dbgnfi_info("[NFI] nfi_mpiServer_disconnect(): begin\n");

	  // check params...
	  if (serv == NULL) {
	       return 0;
	  }

	  // close connections...
          server_aux = (struct nfi_mpiServer_server *)serv->private_info;
          if (server_aux != NULL) {
        	close(server_aux->sd);
                free(serv->private_info);
                server_aux = NULL ;
	  }

	  // free 'url' string
          if (serv->url != NULL) {
        	free(serv->url);
        	serv->url = NULL;
	  }

	  // free 'server' string
	  if (serv->server != NULL) {
		free(serv->server);
		serv->server = NULL;
	  }

	  //serv->protocol = -1;
	  dbgnfi_info("[NFI] nfi_mpiServer_disconnect(): end\n");

	  // return OK
	  return 0;
      }

      /************************************************************
       * Reconnect to the NFS server				    *
       ************************************************************/
      int nfi_mpiServer_reconnect(struct nfi_server *serv)
      {
	/* Don't see the serv result */
	char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
	int ret;
	struct nfi_mpiServer_server *server_aux;

	ret = ParseURL(serv->url,  NULL, NULL, NULL, server,  NULL,  dir);
	if(ret <0 ){
		mpiServer_err(MPISERVERERR_URL);
		fprintf(stderr,"nfi_mpiServer_reconnect: url %s incorrect.\n",serv->url);
		return -1;
	}

	server_aux = (struct nfi_mpiServer_server *)malloc(sizeof(struct nfi_mpiServer_server));
	if(server_aux == NULL){
		mpiServer_err(MPISERVERERR_MEMORY);
		return -1;
	}

	strcpy(server_aux->path, dir);

	serv->private_info = (void *)server_aux;
	return 0;
      }

      /************************************************************
       * Destroy NFS operations				    *
       ************************************************************/
      int nfi_mpiServer_destroy(struct nfi_server *serv)
      {
	  struct nfi_mpiServer_server *server_aux;
	  struct st_mpiServer_msg msg;

	  // check arguments...
	  if (serv == NULL) {
		return 0;
	  }

	  // free private info
	  server_aux = (struct nfi_mpiServer_server *)serv->private_info;

	  if (server_aux != NULL) {
		msg.type = -1;
        	mpiServer_write_operation(server_aux->sd, &msg);
        	close(server_aux->sd);
		free(serv->private_info);
	        server_aux = NULL ;
	  }

	  // end workers...
	  nfi_worker_end(serv->wrk);

	  // free 'url' string
	  if (serv->url != NULL) {
		free(serv->url);
	        serv->url = NULL ;
	  }

	  // free 'server' string
	  if (serv->server != NULL) {
		free(serv->server);
	        serv->server = NULL ;
	  }

	  // free nfi_ops...
	  if (serv->ops != NULL) {
		free(serv->ops);
	  }

	  //serv->protocol = -1;

          // return OK
	  return 0;
      }


      /*
       *  GENERIC FUNCTIONS
       */

      int nfi_mpiServer_getattr ( struct nfi_server *serv,  struct nfi_fhandle *fh, struct nfi_attr *attr )
      {
	  struct nfi_mpiServer_server *server_aux;
	  struct nfi_mpiServer_fhandle *fh_aux;
	  struct st_mpiServer_msg msg;
	  struct st_mpiServer_attr_req req;

	  dbgnfi_info("[NFI] nfi_mpiServer_getattr (...): begin\n");

	  // check arguments...
	  if (attr == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	  }

	  if (serv == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	  }

	  if (fh == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	  }

	  if (fh->priv_fh == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	  }

	  // copy private information...
	  server_aux = (struct nfi_mpiServer_server *) serv->private_info;
	  strcpy(msg.id, server_aux->id);

#ifdef NFI_DYNAMIC
	  if (serv->private_info == NULL){
		ret = nfi_mpiServer_reconnect(serv);
		if(ret <0){
			/* mpiServer_err(); not necessary */
			return -1;
		}
	  }
#else
	  if (serv->private_info == NULL){
               mpiServer_err(MPISERVERERR_PARAM);
               return -1;
          }
#endif
	  fh_aux = (struct nfi_mpiServer_fhandle *) fh->priv_fh;
	  server_aux = (struct nfi_mpiServer_server *) serv->private_info;

	  /*****************************************/
	  msg.type = MPISERVER_GETATTR_FILE;
	  strcpy(msg.u_st_mpiServer_msg.op_getattr.path, fh_aux->path);

          mpiServer_write_operation(server_aux->sd, &msg);
	  dbgnfi_info("[NFI] nfi_mpiServer_getattr(ID=%s): getattr: -> %s \n", server_aux->id, msg.u_st_mpiServer_msg.op_getattr.path);
	  bzero(&req, sizeof(struct st_mpiServer_attr_req));
	  mpiServer_read_data(server_aux->sd, (char *)&req, sizeof(struct st_mpiServer_attr_req), msg.id);
	  dbgnfi_info("[NFI] nfi_mpiServer_getattr(ID=%s): getattr: <- %d\n", server_aux->id, req.status);
	  /*****************************************/

	/*
	ret = stat(fh_aux->path, &st);
	if(ret < 0){
		fprintf(stderr,"nfi_mpiServer_getattr: Fail stat %s in server %s.\n",fh_aux->path,serv->server);
		mpiServer_err(MPISERVERERR_GETATTR);
		return -1;
	}
	*/

	  MPISERVERtoNFIattr(attr, &req.attr);

	  dbgnfi_info("[NFI] nfi_mpiServer_getattr (...): end\n");

	  // return status
	  return req.status;
      }

      int nfi_mpiServer_setattr ( struct nfi_server *serv,  struct nfi_fhandle *fh, struct nfi_attr *attr )
      {
	struct nfi_mpiServer_server *server_aux;
	struct nfi_mpiServer_fhandle *fh_aux;

	if (attr == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}

	if (serv == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}

	if (fh->priv_fh == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_mpiServer_reconnect(serv);
		if(ret <0){
			/* mpiServer_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               mpiServer_err(MPISERVERERR_PARAM);
               return -1;
        }
#endif


	fh_aux = (struct nfi_mpiServer_fhandle *) fh->priv_fh;
	server_aux = (struct nfi_mpiServer_server *) serv->private_info;

 //	NFItoMPISERVERattr(&st, attr);
	/* no se comp hacer el setattr */
	/*
	ret = stat(fh_aux->path, &st);
	*/
/*
	ret = truncate(fh_aux->path, st.st_mpiServer_size);
	if(ret < 0){
		fprintf(stderr,"nfi_mpiServer_setattr: Fail stat %s in server %s.\n",fh_aux->path,serv->server);
		mpiServer_err(MPISERVERERR_GETATTR);
		return -1;
	}
*/
	return 0;
      }

      int nfi_mpiServer_preload(struct nfi_server *serv, char *url, char *virtual_path, char *storage_path, int opt)
      {
	//char dir[NFIMAXPATHLEN];
	int ret;
	struct nfi_mpiServer_server *server_aux;
	struct st_mpiServer_msg msg;

	server_aux = (struct nfi_mpiServer_server *) serv->private_info;
	strcpy(msg.id,server_aux->id);
	dbgnfi_info("[NFI] nfi_mpiServer_preload(ID=%s): begin %s - %s \n", server_aux->id, virtual_path, storage_path);
	if(url == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}

	if(url[strlen(url)-1] == '/'){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}

	if (serv == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}

	if (virtual_path == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}

	if (storage_path == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}
#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_mpiServer_reconnect(serv);
		if(ret <0){
			/* mpiServer_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               mpiServer_err(MPISERVERERR_PARAM);
               return -1;
        }
#endif

	server_aux = (struct nfi_mpiServer_server *) serv->private_info;
	dbgnfi_info("[NFI] nfi_mpiServer_preload(ID=%s): preload %s in server %s.\n",server_aux->id,virtual_path,serv->server);

	/*****************************************/
	msg.type = MPISERVER_PRELOAD_FILE;
	//strcpy(msg.u_st_mpiServer_msg.op_preload.path,dir);
	strcpy(msg.u_st_mpiServer_msg.op_preload.virtual_path,virtual_path);
	strcpy(msg.u_st_mpiServer_msg.op_preload.storage_path,storage_path);
	msg.u_st_mpiServer_msg.op_preload.opt = opt;

        mpiServer_write_operation(server_aux->sd, &msg);
	dbgnfi_info("[NFI] nfi_mpiServer_preload(ID=%s): preload: -> %s \n",server_aux->id,msg.u_st_mpiServer_msg.op_preload.virtual_path);
	mpiServer_read_data(server_aux->sd, (char *)&ret, sizeof(int), msg.id);
	dbgnfi_info("[NFI] nfi_mpiServer_preload(ID=%s): preload: <- %d \n",server_aux->id,ret);
	/*****************************************/

	dbgnfi_info("[NFI] nfi_mpiServer_preload(ID=%s): end %s - %s = %d\n", server_aux->id,virtual_path, storage_path, ret);
	if(ret == -1){
		printf("[NFI]Error en el preload\n");
	}

	return ret;
      }


      int nfi_mpiServer_flush(struct nfi_server *serv,  char *url, char *virtual_path, char *storage_path, int opt)
      {
	//char dir[NFIMAXPATHLEN];
	int ret;
	struct nfi_mpiServer_server *server_aux;

	struct st_mpiServer_msg msg;

	server_aux = (struct nfi_mpiServer_server *) serv->private_info;
	strcpy(msg.id,server_aux->id);
	dbgnfi_info("[NFI] nfi_mpiServer_flush(ID=%s): begin %s - %s \n", server_aux->id,virtual_path, storage_path);
	if (url == NULL) {
	    mpiServer_err(MPISERVERERR_PARAM);
	    return -1;
	}

	if (url[strlen(url)-1] == '/') {
	    mpiServer_err(MPISERVERERR_PARAM);
	    return -1;
	}

	if (serv == NULL){
	    mpiServer_err(MPISERVERERR_PARAM);
	    return -1;
	}

	if (virtual_path == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}

	if (storage_path == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}
#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_mpiServer_reconnect(serv);
		if(ret <0){
			/* mpiServer_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               mpiServer_err(MPISERVERERR_PARAM);
               return -1;
        }
#endif

	server_aux = (struct nfi_mpiServer_server *) serv->private_info;
	dbgnfi_info("[NFI] nfi_mpiServer_flush(ID=%s): open %s in server %s.\n",server_aux->id,virtual_path,serv->server);

	/*****************************************/
	msg.type = MPISERVER_FLUSH_FILE;
	//strcpy(msg.u_st_mpiServer_msg.op_flush.path,dir);
	strcpy(msg.u_st_mpiServer_msg.op_flush.virtual_path,virtual_path);
	strcpy(msg.u_st_mpiServer_msg.op_flush.storage_path,storage_path);
	msg.u_st_mpiServer_msg.op_flush.opt = opt;

        mpiServer_write_operation(server_aux->sd, &msg);
	dbgnfi_info("[NFI] nfi_mpiServer_flush(ID=%s): flush: -> %s \n",server_aux->id,msg.u_st_mpiServer_msg.op_flush.virtual_path);
	mpiServer_read_data(server_aux->sd, (char *)&ret, sizeof(int), msg.id);
	dbgnfi_info("[NFI] nfi_mpiServer_flush(ID=%s): flush: <- %d \n",server_aux->id,ret);
	/*****************************************/

	dbgnfi_info("[NFI] nfi_mpiServer_flush(ID=%s): end %s - %s = %d\n", server_aux->id,virtual_path, storage_path, ret);
	return 0;
      }

      int nfi_mpiServer_open ( struct nfi_server *serv,  char *url, struct nfi_fhandle *fho )
      {
	char dir[NFIMAXPATHLEN], server[NFIMAXPATHLEN];
	int ret;
	struct nfi_mpiServer_server *server_aux;
	struct nfi_mpiServer_fhandle *fh_aux;

	struct st_mpiServer_msg msg;

	server_aux = (struct nfi_mpiServer_server *) serv->private_info;
	strcpy(msg.id,server_aux->id);
	dbgnfi_info("[NFI] nfi_mpiServer_open(ID=%s): begin %s\n",server_aux->id,url);
	if(url[strlen(url)-1] == '/'){
		return nfi_mpiServer_opendir(serv, url, fho);
	}

	if (serv == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}

	if (fho == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}
#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_mpiServer_reconnect(serv);
		if(ret <0){
			/* mpiServer_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               mpiServer_err(MPISERVERERR_PARAM);
               return -1;
        }
#endif

	ret = ParseURL(url, NULL, NULL, NULL, server,  NULL,  dir);
	if(ret < 0){
		fprintf(stderr,"nfi_mpiServer_open: url %s incorrect.\n",url);
		mpiServer_err(MPISERVERERR_URL);
		return -1;
	}
	fho->url = (char *)malloc(strlen(url)+1);
	if(fho->url == NULL){
		mpiServer_err(MPISERVERERR_MEMORY);
		return -1;
	}

	strcpy(fho->url, url);

	fh_aux = (struct nfi_mpiServer_fhandle *)malloc(sizeof(struct nfi_mpiServer_fhandle));
	if (fh_aux == NULL){
		mpiServer_err(MPISERVERERR_MEMORY);
		free(fho->url);
		return -1;
	}

	server_aux = (struct nfi_mpiServer_server *) serv->private_info;

	/*****************************************/
	msg.type = MPISERVER_OPEN_FILE;
	strcpy(msg.u_st_mpiServer_msg.op_open.path,dir);

        mpiServer_write_operation(server_aux->sd, &msg);
	dbgnfi_info("[NFI] nfi_mpiServer_open(ID=%s): open -> %s \n",server_aux->id,msg.u_st_mpiServer_msg.op_open.path);
	mpiServer_read_data(server_aux->sd, (char *)&fh_aux->fd, sizeof(int), msg.id);
	dbgnfi_info("[NFI] nfi_mpiServer_open(ID=%s): open <- %d \n",server_aux->id,fh_aux->fd);
	strcpy(fh_aux->path, dir);
	/*****************************************/

	fho->type = NFIFILE;
	fho->server = NULL;
	fho->priv_fh = NULL;
	fho->server = serv;
	fho->priv_fh = (void *) fh_aux;

	dbgnfi_info("[NFI] nfi_mpiServer_open(ID=%s): end\n",server_aux->id);
	return 0;

      }

      int nfi_mpiServer_close(struct nfi_server *server,  struct nfi_fhandle *fh)
      {
	struct nfi_mpiServer_fhandle *fh_aux;
	struct nfi_mpiServer_server *server_aux;
	struct st_mpiServer_msg msg;

	server_aux = (struct nfi_mpiServer_server *) server->private_info;
	strcpy(msg.id,server_aux->id);
	dbgnfi_info("[NFI] nfi_mpiServer_close(ID=%s): begin\n",server_aux->id);
	if (server == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}
	if (fh == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}

	if (fh->priv_fh != NULL){
		fh_aux = (struct nfi_mpiServer_fhandle *) fh->priv_fh;
		server_aux = (struct nfi_mpiServer_server *) server->private_info;
		/*****************************************/
		msg.type = MPISERVER_CLOSE_FILE;
		msg.u_st_mpiServer_msg.op_close.fd = fh_aux->fd;

        	mpiServer_write_operation(server_aux->sd, &msg);

		dbgnfi_info("[NFI] nfi_mpiServer_close(ID=%s): close -> %d \n",server_aux->id,msg.u_st_mpiServer_msg.op_close.fd);
		/*****************************************/
		//close(fh_aux->fd);
		/* free memory */
		free(fh->priv_fh);
		fh->priv_fh = NULL;
	}

	fh->type = NFINULL;
	fh->server = NULL;
	dbgnfi_info("[NFI] nfi_mpiServer_close(ID=%s): end\n",server_aux->id);

	return 0;
      }

      ssize_t nfi_mpiServer_read( struct nfi_server *serv,
			          struct nfi_fhandle *fh,
			          void *buffer,
			          off_t offset,
			          size_t size )
			          //off_t offset)
      {
	int ret, cont;
	struct nfi_mpiServer_server *server_aux;
	struct nfi_mpiServer_fhandle *fh_aux;
	struct st_mpiServer_msg msg;
	struct st_mpiServer_read_req req;

	server_aux = (struct nfi_mpiServer_server *) serv->private_info;
	strcpy(msg.id,server_aux->id);
	dbgnfi_info("[NFI] nfi_mpiServer_read(%s): begin off %d size %d\n",server_aux->id,(int)offset, (int)size);
	if (serv == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}

	if (fh == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_mpiServer_reconnect(serv);
		if(ret <0){
			/* mpiServer_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               mpiServer_err(MPISERVERERR_PARAM);
               return -1;
        }
#endif


	fh_aux = (struct nfi_mpiServer_fhandle *) fh->priv_fh;
	server_aux = (struct nfi_mpiServer_server *) serv->private_info;

	/*****************************************/

	msg.type = MPISERVER_READ_FILE;
	msg.u_st_mpiServer_msg.op_read.fd 	= fh_aux->fd;
	msg.u_st_mpiServer_msg.op_read.offset 	= offset;
	msg.u_st_mpiServer_msg.op_read.size 	= size;

	#ifdef  DBG_IO
		printf("[NFI]read: -> fd %d \n",msg.u_st_mpiServer_msg.op_read.fd);
		printf("[NFI]read: -> offset %d \n",(int)msg.u_st_mpiServer_msg.op_read.offset);
		printf("[NFI]read: -> size %d \n",msg.u_st_mpiServer_msg.op_read.size);
	#endif

	ret = mpiServer_write_operation(server_aux->sd, &msg);
	if(ret == -1){
		perror("ERROR: (1)nfi_mpiServer_read: Error on write operation");
		fprintf(stderr,"ERROR: (1)nfi_mpiServer_read: Error on write operation\n");
		return -1;
	}

	cont = 0;
	do{
		ret = mpiServer_read_data(server_aux->sd, (char *)&req, sizeof(struct st_mpiServer_read_req), msg.id);
		dbgnfi_info("[NFI] nfi_mpiServer_read(ID=%s): (1)mpiServer_read_data = %d.\n",server_aux->id, ret);
		if(ret == -1){
			perror("ERROR: (2)nfi_mpiServer_read: Error on write operation");
			fprintf(stderr,"ERROR: (2)nfi_mpiServer_read: Error on write operation\n");
			return -1;
		}


		if(req.size > 0){
		dbgnfi_info("[NFI] nfi_mpiServer_read(ID=%s): (2)mpiServer_read_data = %d. size = %d cont = %d\n",server_aux->id, ret, req.size, cont);
			ret = mpiServer_read_data(server_aux->sd, (char *)buffer+cont, req.size, msg.id);
			dbgnfi_info("[NFI] nfi_mpiServer_read(ID=%s): (2)mpiServer_read_data = %d.\n",server_aux->id, ret);
			if(ret == -1){
				perror("ERROR: (3)nfi_mpiServer_read: Error on write operation");
				fprintf(stderr,"ERROR: (3)nfi_mpiServer_read: Error on read operation\n");
				return -1;
			}
			//cont += req.size;
		}
		cont += req.size;
		dbgnfi_info("[NFI] nfi_mpiServer_read(ID=%s): read(%d,%d) cont = %d\n",server_aux->id,req.size,(int)req.last,cont);
	}while((req.size>0)&&(!req.last));

	/*****************************************/
	dbgnfi_info("[NFI] nfi_mpiServer_read(ID=%s): read %s off %d size %d (err:%d).\n",server_aux->id,fh->url,(int)offset,(int)size,(int)req.size);
	if(req.size < 0){
		fprintf(stderr,"ERROR: nfi_mpiServer_read: Fail read %s off %d size %d (err:%d).\n",fh->url,(int)offset,(int)size,(int)req.size);
		mpiServer_err(MPISERVERERR_READ);
		return -1;
	}

	dbgnfi_info("[NFI] nfi_mpiServer_read(ID=%s): end\n",server_aux->id);
	return req.size;
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

	int ret;

	server_aux = (struct nfi_mpiServer_server *) serv->private_info;
	strcpy(msg.id,server_aux->id);
	dbgnfi_info("[NFI] nfi_mpiServer_write(ID=%s): begin off %d size %d\n",server_aux->id,(int)offset, (int)size);

	if(size == 0){
		return 0;
	}
	if (serv == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}
	if (fh == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_mpiServer_reconnect(serv);
		if(ret <0){
			/* mpiServer_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               mpiServer_err(MPISERVERERR_PARAM);
               return -1;
        }
#endif

    	fh_aux = (struct nfi_mpiServer_fhandle *) fh->priv_fh;
	server_aux = (struct nfi_mpiServer_server *) serv->private_info;

	/*****************************************/
	msg.type = MPISERVER_WRITE_FILE;
	msg.u_st_mpiServer_msg.op_write.fd 	= fh_aux->fd;
	msg.u_st_mpiServer_msg.op_write.offset 	= offset;
	msg.u_st_mpiServer_msg.op_write.size 	= size;

	#ifdef  DBG_IO
		printf("[NFI]write: -> fd %d \n",msg.u_st_mpiServer_msg.op_write.fd);
		printf("[NFI]write: -> offset %d \n",(int)msg.u_st_mpiServer_msg.op_write.offset);
		printf("[NFI]write: -> size %d \n",msg.u_st_mpiServer_msg.op_write.size);
	#endif

	ret = mpiServer_write_operation(server_aux->sd, &msg);
	if(ret == -1){
		fprintf(stderr,"(1)ERROR: nfi_mpiServer_write(ID=%s): Error on write operation\n",server_aux->id);
		return -1;
	}

	ret = mpiServer_write_data(server_aux->sd, (char *)buffer, size, msg.id);
	if(ret == -1){
		fprintf(stderr,"(2)ERROR: nfi_mpiServer_read(ID=%s): Error on write operation\n",server_aux->id);
		return -1;
	}

	ret = mpiServer_read_data(server_aux->sd, (char *)&req, sizeof(struct st_mpiServer_write_req), msg.id);
	if(ret == -1){
		fprintf(stderr,"(3)ERROR: nfi_mpiServer_write(ID=%s): Error on write operation\n",server_aux->id);
		return -1;
	}


	/*****************************************/
	dbgnfi_info("[NFI] nfi_mpiServer_write(ID=%s): write %s off %d size %d (err:%d).\n",server_aux->id,fh->url,(int)offset,(int)size,(int)req.size);
	if (req.size < 0)
	{
		fprintf(stderr,"ERROR: nfi_mpiServer_write(ID=%s): Fail write %s off %d size %d (err:%d).\n",server_aux->id,fh->url,(int)offset,(int)size,(int)req.size);
		mpiServer_err(MPISERVERERR_WRITE);
		return -1;
	}

	dbgnfi_info("[NFI] nfi_mpiServer_write(ID=%s): end\n",server_aux->id);
	return req.size;
      }

      int nfi_mpiServer_create(struct nfi_server *serv,  char *url, struct nfi_attr *attr, struct nfi_fhandle  *fh)
      {
	char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
	int ret;
	struct nfi_mpiServer_server *server_aux;
        struct nfi_mpiServer_fhandle *fh_aux;
	struct st_mpiServer_msg msg;

	server_aux = (struct nfi_mpiServer_server *) serv->private_info;
	strcpy(msg.id,server_aux->id);
	dbgnfi_info("[NFI] nfi_mpiServer_create(ID=%s): begin %s\n",server_aux->id,url);
	if (serv == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}

	if (attr == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_mpiServer_reconnect(serv);
		if(ret <0){
			/* mpiServer_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               mpiServer_err(MPISERVERERR_PARAM);
               return -1;
        }
#endif
	ret = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir);
	if(ret < 0){
		fprintf(stderr,"ERROR: nfi_mpiServer_create: url %s incorrect.\n",url);
		mpiServer_err(MPISERVERERR_URL);
		return -1;
	}
	/* private_info file handle */
	fh_aux = (struct nfi_mpiServer_fhandle *)malloc(sizeof(struct nfi_mpiServer_fhandle));
	if (fh_aux == NULL){
		mpiServer_err(MPISERVERERR_MEMORY);
		return -1;
	}

	bzero(fh_aux, sizeof(struct nfi_mpiServer_fhandle));


    	server_aux = (struct nfi_mpiServer_server *) serv->private_info;
	/* create the file into the directory */

	/*****************************************/
	msg.type = MPISERVER_CREAT_FILE;
	strcpy(msg.u_st_mpiServer_msg.op_creat.path,dir);

    	mpiServer_write_operation(server_aux->sd, &msg);
	mpiServer_read_data(server_aux->sd, (char *)&(fh_aux->fd), sizeof(int), msg.id);

	strcpy(fh_aux->path, dir);
	/*****************************************/
	fh->type = NFIFILE;
	fh->server = serv;
        fh->priv_fh = (void *)fh_aux;

        fh->url = (char *)malloc(strlen(url)+1);
        if(fh->url == NULL){
               	mpiServer_err(MPISERVERERR_MEMORY);
		free(fh_aux);
               	return -1;
        }

        strcpy(fh->url, url);

	dbgnfi_info("[NFI] nfi_mpiServer_create(ID=%s): end\n",server_aux->id);
	return 0;
      }

      int nfi_mpiServer_remove(struct nfi_server *serv,  char *url)
      {
	char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
	int ret;
	struct nfi_mpiServer_server *server_aux;
	struct st_mpiServer_msg msg;

	server_aux = (struct nfi_mpiServer_server *) serv->private_info;
	strcpy(msg.id,server_aux->id);
	dbgnfi_info("[NFI] nfi_mpiServer_remove(%s): begin %s\n",server_aux->id, url);
	if (serv == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_mpiServer_reconnect(serv);
		if(ret <0){
			/* mpiServer_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               mpiServer_err(MPISERVERERR_PARAM);
               return -1;
        }
#endif


	server_aux = (struct nfi_mpiServer_server *)serv->private_info;

	ret = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir);
	if(ret < 0){
		fprintf(stderr,"nfi_mpiServer_remove: url %s incorrect.\n",url);
		mpiServer_err(MPISERVERERR_URL);
		return -1;
	}
	/* remove the file into the directory */
	//ret = unlink(dir);
	/*****************************************/
	msg.type = MPISERVER_RM_FILE;
	strcpy(msg.u_st_mpiServer_msg.op_rm.path,dir);

        mpiServer_write_operation(server_aux->sd, &msg);

	/*****************************************/
	dbgnfi_info("[NFI] nfi_mpiServer_remove(ID=%s): end \n",server_aux->id);
	return 0;
      }

      int nfi_mpiServer_rename(struct nfi_server *server,  char *old_url, char *new_url)
      {
	/*
        struct nfi_mpiServer_server *server_aux;
        struct nfi_mpiServer_fhandle *fh_aux;
	if (server == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_mpiServer_reconnect(serv);
		if(ret <0){
			mpiServer_err(); not necessary
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               mpiServer_err(MPISERVERERR_PARAM);
               return -1;
        }
#endif
	server_aux = (strcut nfi_mpiServer_server *)serv->private_info;
	*/

	/* ????? */
	return 0;
      }

      int nfi_mpiServer_mkdir(struct nfi_server *serv,  char *url, struct nfi_attr *attr, struct nfi_fhandle *fh)
      {
	char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
	int ret;
        struct nfi_mpiServer_server *server_aux;
        struct nfi_mpiServer_fhandle *fh_aux;
	struct st_mpiServer_msg msg;

	if (serv == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}

	if (attr == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_mpiServer_reconnect(serv);
		if(ret <0){
			/* mpiServer_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               mpiServer_err(MPISERVERERR_PARAM);
               return -1;
        }
#endif

	server_aux = (struct nfi_mpiServer_server *)serv->private_info;

	ret = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir);
	if(ret < 0){
		fprintf(stderr,"nfi_mpiServer_mkdir: url %s incorrect.\n",url);
		mpiServer_err(MPISERVERERR_URL);
		return -1;
	}

	/* private_info file handle */
	fh_aux = (struct nfi_mpiServer_fhandle *)malloc(sizeof(struct nfi_mpiServer_fhandle));
	if (fh_aux == NULL){
		mpiServer_err(MPISERVERERR_MEMORY);
		return -1;
	}


	bzero(fh_aux, sizeof(struct nfi_mpiServer_fhandle));
	/* create the dir into the directory */

	/******************************************************/
	msg.type = MPISERVER_MKDIR_DIR;
	strcpy(msg.u_st_mpiServer_msg.op_mkdir.path, dir);

    	mpiServer_write_operation(server_aux->sd, &msg);
	mpiServer_read_data(server_aux->sd, (char *)&(fh_aux->fd), sizeof(int), msg.id);

	strcpy(fh_aux->path, dir);
	/******************************************************/

	if((ret < 0)&&(errno != EEXIST)){
		mpiServer_err(MPISERVERERR_MKDIR);
		fprintf(stderr,"nfi_mpiServer_mkdir: Fail mkdir %s in server %s.\n",dir,serv->server);
		free(fh_aux);
		return -1;
	}

	fh->type = NFIDIR;
        fh->priv_fh = (void *)fh_aux;

        fh->url = (char *)malloc(strlen(url)+1);
        if(fh->url == NULL){
               mpiServer_err(MPISERVERERR_MEMORY);
	       free(fh_aux);
               return -1;
        }
        strcpy(fh->url, url);

	//TODO:
	//MPISERVERtoNFIattr(attr, &st);

	return 0;
      }

      int nfi_mpiServer_rmdir(struct nfi_server *serv,  char *url)
      {
	char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
	int ret;

        struct nfi_mpiServer_server *server_aux;
	struct st_mpiServer_msg msg;

	if (serv == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}


#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_mpiServer_reconnect(serv);
		if(ret <0){
			/* mpiServer_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               mpiServer_err(MPISERVERERR_PARAM);
               return -1;
        }
#endif


	server_aux = (struct nfi_mpiServer_server *)serv->private_info;

	ret = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir);
	if(ret < 0){
		fprintf(stderr,"nfi_mpiServer_rmdir: url %s incorrect.\n",url);
		mpiServer_err(MPISERVERERR_URL);
		return -1;
	}


	/******************************************************/
	msg.type = MPISERVER_RMDIR_DIR;
	strcpy(msg.u_st_mpiServer_msg.op_rmdir.path, url);

    	mpiServer_write_operation(server_aux->sd, &msg);
	mpiServer_read_data(server_aux->sd, (char *)&ret, sizeof(int), msg.id);

	//strcpy(fh_aux->path, dir);
	/******************************************************/


	//ret = rmdir(dir);
	if(ret < 0){
		fprintf(stderr,"nfi_mpiServer_rmdir: Fail rmdir %s in server %s.\n",dir,serv->server);
		mpiServer_err(MPISERVERERR_REMOVE);
		return -1;
	}

	return 0;
      }

      int nfi_mpiServer_opendir(struct nfi_server *serv,  char *url, struct nfi_fhandle *fho)
      {
	char dir[NFIMAXPATHLEN], server[NFIMAXPATHLEN];
	int ret;
	struct nfi_mpiServer_server *server_aux;
	struct nfi_mpiServer_fhandle *fh_aux;


	if (serv == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}

	if (fho == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_mpiServer_reconnect(serv);
		if(ret <0){
			/* mpiServer_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               mpiServer_err(MPISERVERERR_PARAM);
               return -1;
        }
#endif

	ret = ParseURL(url, NULL, NULL, NULL, server,  NULL,  dir);
	if(ret < 0){
		fprintf(stderr,"nfi_mpiServer_opendir: url %s incorrect.\n",url);
		mpiServer_err(MPISERVERERR_URL);
		return -1;
	}

	fho->url = (char *)malloc(strlen(url)+1);
	if(fho->url == NULL){
		mpiServer_err(MPISERVERERR_MEMORY);
		return -1;
	}

	strcpy(fho->url, url);

	fh_aux = (struct nfi_mpiServer_fhandle *)malloc(sizeof(struct nfi_mpiServer_fhandle));
	if (fh_aux == NULL){
		mpiServer_err(MPISERVERERR_MEMORY);
		free(fho->url);
		return -1;
	}

	server_aux = (struct nfi_mpiServer_server *) serv->private_info;


	fh_aux->dir = opendir(dir);
    if (ret < 0 ){
		fprintf(stderr,"nfi_mpiServer_opendir: Fail opendir %s in server %s.\n",dir,serv->server);
	    mpiServer_err(MPISERVERERR_MEMORY);
		free(fh_aux);
		free(fho->url);
		return -1;
	}

	fh_aux->fd = ret;
	strcpy(fh_aux->path, dir);
	fho->type = NFIDIR;

	fho->server = NULL;
	fho->priv_fh = NULL;
	fho->server = serv;
	fho->priv_fh = (void *) fh_aux;

	return 0;
      }

      int nfi_mpiServer_readdir(struct nfi_server *serv,  struct nfi_fhandle *fh, char *entry, unsigned char *type)
      {
	struct dirent *ent;

        struct nfi_mpiServer_server *server_aux;
        struct nfi_mpiServer_fhandle *fh_aux;


	if (serv == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}


	if (fh == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}

	if (fh->priv_fh == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}

	if(fh->type != NFIDIR){
		mpiServer_err(MPISERVERERR_NOTDIR);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_mpiServer_reconnect(serv);
		if(ret <0){
			/* mpiServer_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               mpiServer_err(MPISERVERERR_PARAM);
               return -1;
        }
#endif

	server_aux = (struct nfi_mpiServer_server *)serv->private_info;
	fh_aux = (struct nfi_mpiServer_fhandle *)fh->priv_fh;

	entry[0] = '\0';
	ent = readdir(fh_aux->dir);

	if(ent == NULL){
		return 1;
	}
	if(type==NULL){
		return 0;
	}
	strcpy(entry, ent->d_name);
	//printf("[NFI]ent->d_name = %s S_ISDIR(%o) = %o\n", ent->d_name, ent->d_type,S_ISDIR(ent->d_type));
//	*type = ent->d_type;

	return 0;
      }

      int nfi_mpiServer_closedir(struct nfi_server *serv,  struct nfi_fhandle *fh)
      {
	struct nfi_mpiServer_fhandle *fh_aux;

	if (serv == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}
	if (fh == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}

	if (fh->priv_fh != NULL){
		fh_aux = (struct nfi_mpiServer_fhandle *) fh->priv_fh;
		closedir(fh_aux->dir);
		/* free memory */
		free(fh->priv_fh);
		fh->priv_fh = NULL;

	}

	return 0;
      }

      int nfi_mpiServer_statfs(struct nfi_server *serv,  struct nfi_info *inf)
      {/*

	struct mpiServer_info mpiServerinf;
	int ret;
        struct nfi_mpiServer_server *server_aux;


	if (serv == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}

	if (inf == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}


#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_mpiServer_reconnect(serv);
		if(ret <0){
		 mpiServer_err(); not necessary
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               mpiServer_err(MPISERVERERR_PARAM);
               return -1;
        }
#endif

	server_aux = (struct nfi_mpiServer_server *)serv->private_info;
	ret = mpiServer_statfs(server_aux->fh, &mpiServerinf, server_aux->cl);
	if(ret <0){
		mpiServer_err(MPISERVERERR_STATFS);
		return -1;
	}

	NFStoNFIInfo(inf, &mpiServerinf);
*/
	return 0;
      }


   /* ................................................................... */

