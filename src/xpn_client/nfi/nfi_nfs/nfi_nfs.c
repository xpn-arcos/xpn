
  /*
   *  Copyright 2020-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos
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

#include "nfi_nfs.h"

/*
 * TODO:
 *  -> nfs_tcp, nfs_udp ??????????????
 *  -> nfs_xxx a nfi_nfs_xxx
 *  -> nfs_misc.[ch]:
 *     -> ParseURL
 *  -> nfs_error.[ch]:
 *     -> nfs_err
 *
 * -> pasar CLIENT * a servidor (es comun a todos...)
 * -> Poner todo a punteros
 * -> struct nfi_fhandle ** Paso de puntero parametro por referencia
 *
 */


/************************************************************
 * PRIVATE FUNCTIONS TO USE NFS SERVERS			    *
 ************************************************************/
void NFItoNFSattr(fattr *nfs_att, struct nfi_attr *nfi_att)
{
	switch(nfi_att->at_type){
		case NFIDIR:
			nfs_att->type = NFDIR;
			break;
                case NFIFILE:
                        nfs_att->type = NFREG;
                        break;
	}

        nfs_att->mode 	= (u_long)nfi_att->at_mode;
        /* u_long nlink; */
        nfs_att->uid 	= (u_long)nfi_att->at_uid;
        nfs_att->gid	= (u_long)nfi_att->at_gid;
        nfs_att->size	= (u_long)nfi_att->at_size;
	nfs_att->blocksize = (u_long)nfi_att->at_blksize;
	/* u_long rdev;  */
	nfs_att->blocks	= (u_long)nfi_att->at_blocks;
	/* u_long fsid;  */
	/* u_long fileid;*/
	nfs_att->atime.seconds 	= (u_long)nfi_att->at_atime;
	nfs_att->atime.useconds	= (u_long)0;
        nfs_att->mtime.seconds	= (u_long)nfi_att->at_atime;
        nfs_att->mtime.useconds	= (u_long)0;
        nfs_att->ctime.seconds	= (u_long)nfi_att->at_ctime;
        nfs_att->ctime.useconds	= (u_long)0;

}

void NFStoNFIattr(struct nfi_attr *nfi_att, fattr *nfs_att){


	switch(nfs_att->type){
		case NFDIR:
			nfi_att->at_type = NFIDIR;
			break;
                case NFREG:
                        nfi_att->at_type = NFIFILE;
                        break;
		default:
			break;
	}

        nfi_att->at_mode = (mode_t)nfs_att->mode;
        /* u_long nlink; */
        nfi_att->at_uid = (uid_t)nfs_att->uid;
        nfi_att->at_gid = (gid_t)nfs_att->gid;
        nfi_att->at_size = (off_t)nfs_att->size;
	nfi_att->at_blksize = (u_long)nfs_att->blocksize;
	/* u_long rdev;  */
	nfi_att->at_blocks = (u_long)nfs_att->blocks;
	/* u_long fsid;  */
	/* u_long fileid;*/
        nfi_att->at_atime = (time_t)nfs_att->atime.seconds
	                + (1000*nfs_att->atime.useconds);
	nfi_att->at_mtime = (time_t)nfs_att->mtime.seconds
			+ (1000*nfs_att->mtime.useconds);
	nfi_att->at_ctime = (time_t)nfs_att->ctime.seconds
	                + (1000*nfs_att->ctime.useconds);
		
	nfi_att->private_info = NULL;
}

void NFStoNFIInfo(__attribute__((__unused__)) struct nfi_info *nfi_inf, __attribute__((__unused__)) struct nfs_info *nfs_inf) {
}






/************************************************************
 * PUBLIC FUNCTIONS TO USE NFS SERVERS			    *
 ************************************************************/

/************************************************************
 * Init NFS server					    *
 ************************************************************/
int nfi_nfs_init(char *url, struct nfi_server *serv, __attribute__((__unused__)) struct nfi_attr_server *attr)
{
	char server[NFSPATHLEN], dir[NFSPATHLEN], prt[NFSPATHLEN];
	int ret;
	struct nfi_nfs_server *server_aux;


	if(serv == NULL){
		nfs_err(NFSERR_PARAM);
		fprintf(stderr,"nfi_nfs_init: serv is null. Don't inicialize this variable\n");
		return -1;
	}
	
	/* functions */
	serv->ops = (struct nfi_ops *)malloc(sizeof(struct nfi_ops));
	if (serv->ops == NULL){
		nfs_err(NFSERR_MEMORY);
		fprintf(stderr,"nfi_nfs_init: serv->ops is null. Can't reserve memory\n");
		return -1;
	}

	serv->ops->nfi_reconnect  = nfi_nfs_reconnect;
	serv->ops->nfi_disconnect = nfi_nfs_disconnect;

	serv->ops->nfi_preload	= NULL;
	serv->ops->nfi_flush	= NULL;

	serv->ops->nfi_getattr	= nfi_nfs_getattr;
	serv->ops->nfi_setattr	= nfi_nfs_setattr;

	serv->ops->nfi_open	= nfi_nfs_open;
	serv->ops->nfi_close	= nfi_nfs_close;

	serv->ops->nfi_read	= nfi_nfs_read;
	serv->ops->nfi_write	= nfi_nfs_write;

	serv->ops->nfi_create	= nfi_nfs_create;
	serv->ops->nfi_remove	= nfi_nfs_remove;
	serv->ops->nfi_rename	= nfi_nfs_rename;

	serv->ops->nfi_mkdir	= nfi_nfs_mkdir;
	serv->ops->nfi_rmdir	= nfi_nfs_rmdir;
	serv->ops->nfi_opendir	= nfi_nfs_opendir;
	serv->ops->nfi_readdir	= nfi_nfs_readdir;
	serv->ops->nfi_closedir	= nfi_nfs_closedir;
	serv->ops->nfi_statfs	= nfi_nfs_statfs;

	ret = ParseURL(url, prt, NULL, NULL, server,  NULL,  dir);
	if(ret <0 ){
		nfs_err(NFSERR_URL);
		free(serv->ops);
		return -1;
	}

	server_aux = (struct nfi_nfs_server *)malloc(sizeof(struct nfi_nfs_server));
	if(server_aux == NULL){
		nfs_err(NFSERR_MEMORY);
		fprintf(stderr,"nfi_nfs_init: url %s incorrect.\n",url);
		return -1;
	}
	server_aux->cl = (CLIENT *)create_connection_mount(server, NFS_UDP);

	if(server_aux->cl == NULL){
		nfs_err(NFSERR_MNTCONNECTION);
		fprintf(stderr,"nfi_nfs_init: server_aux is null. Can't reserve memory\n");
		free(server_aux);
		return -1;
	}
	ret = nfs_mount(dir, server_aux->fh, server_aux->cl );
#if defined(DEBUG_NFI)
	printf("desp. de montar %d\n",ret);
#endif
	if (ret < 0){
		nfs_err(NFSERR_MOUNT);
	    close_connection_mount(server_aux->cl);
		fprintf(stderr,"nfi_nfs_init: Can't mount %s in server %s (err:%d).\n",dir,server,ret);
		free(server_aux);
		return -1;
	}

        close_connection_mount(server_aux->cl);
/*
 	if((strcmp(prt, "nfs2tcp") == 0) ||
	   (strcmp(prt, "nfs2tcphilos") == 0)){
#if defined(DEBUG_NFI)
		printf("nfs2tcp\n");
#endif
*/
		server_aux->cl = create_connection_nfs(server, NFS_TCP);
/*	}
	else{
		server_aux->cl = create_connection_nfs(server, NFS_UDP);
	}
*/
	if(server_aux->cl == NULL){
		nfs_err(NFSERR_NFSCONNECTION);
		fprintf(stderr,"nfi_nfs_init: Can't connect with NFS service of %s\n",server);
		free(server_aux);
		return -1;
	}

	serv->private_info = (void *)server_aux;

	//serv->protocol = NFS;

	serv->server = (char *)malloc(sizeof(char)*(strlen(server)+1));	/* server address */
	if(serv->server == NULL){
	        free(server_aux);
		nfs_err(NFSERR_MEMORY);
		fprintf(stderr,"nfi_nfs_init: serv->server is null. Can't reserve memory\n");
		return -1;
	}
	strcpy(serv->server, server);

	serv->url = (char *)malloc(sizeof(char)*(strlen(url)+1));	/* server address */
	if(serv->url == NULL){
		nfs_err(NFSERR_MEMORY);
		close_connection_nfs(server_aux->cl);
		fprintf(stderr,"nfi_nfs_init: serv->url is null. Can't reserve memory\n");
		free(serv->server);
		free(server_aux);
		return -1;
	}
	strcpy(serv->url, url);
        serv->wrk = (struct nfi_worker *)malloc(sizeof(struct nfi_worker));
	memset(serv->wrk, 0, sizeof(struct nfi_worker));
	if(serv->wrk == NULL){
		close_connection_nfs(server_aux->cl);
		free(serv->server);
			fprintf(stderr,"nfi_nfs_init: serv->wrk is null. Can't reserve memory\n");
		free(serv->url);
		free(server_aux);
		return -1;
	}
	if((strcmp(prt, "nfshilos") == 0)  ||
	   (strcmp(prt, "nfs2hilos") == 0) ||
	   (strcmp(prt, "nfs2udphilos") == 0) ||
	   (strcmp(prt, "nfs2tcphilos") == 0)){
#if defined(DEBUG_NFI)
		printf("nfs2hilos\n");
#endif
		nfi_worker_init(serv->wrk, serv, 1);
	}
	else{
		nfi_worker_init(serv->wrk, serv, 0);
	}
	return 0;
}

/************************************************************
 * Disconnect to the server				    *
 * **********************************************************/
int nfi_nfs_disconnect(struct nfi_server *serv)
{

        struct nfi_nfs_server *server_aux;
        char dir[NFSPATHLEN];
        int ret;

	if (serv == NULL)
	       return 0;


         server_aux = (struct nfi_nfs_server *)serv->private_info;
         if(server_aux != NULL){
	         if( server_aux->cl != NULL)
		         close_connection_nfs( server_aux->cl);
 	
		 ret = ParseURL(serv->url, NULL, NULL, NULL, NULL,  NULL,  dir);
		 if(ret < 0){
		        nfs_err(NFSERR_URL);
				fprintf(stderr,"nfi_nfs_disconnect: url %s incorrect.\n",serv->url);
                        free(serv->private_info);
		        return -1;
		 }
                 server_aux->cl = create_connection_mount(serv->server, NFS_UDP);
                 if(server_aux->cl != NULL){
                      ret = nfs_umount(dir, server_aux->cl);
                      close_connection_mount(server_aux->cl);
                 }
	
                 free(serv->private_info);
        }

        if(serv->url != NULL){
        	free(serv->url);
	}

	if(serv->server != NULL){
		free(serv->server);
	}

	//serv->protocol = -1;

	nfi_worker_end(serv->wrk);

	return 0;
}



/************************************************************
 * Reconnect to the NFS server				    *
 ************************************************************/
int nfi_nfs_reconnect(struct nfi_server *serv)
{
	/* Don't see the serv result */
	char server[NFSPATHLEN], dir[NFSPATHLEN];
	int ret;
	struct nfi_nfs_server *server_aux;

	ret = ParseURL(serv->url,  NULL, NULL, NULL, server,  NULL,  dir);
	if(ret <0 ){
		nfs_err(NFSERR_URL);
		fprintf(stderr,"nfi_nfs_reconnect: url %s incorrect.\n",serv->url);
		return -1;
	}

	server_aux = (struct nfi_nfs_server *)malloc(sizeof(struct nfi_nfs_server));
	if(server_aux == NULL){
		fprintf(stderr,"nfi_nfs_reconnect: server_aux is null. Can't reserve memory\n");
		nfs_err(NFSERR_MEMORY);
		return -1;
	}

	server_aux->cl = (CLIENT *)create_connection_mount(server, NFS_UDP);
	if(server_aux->cl == NULL){
		nfs_err(NFSERR_MNTCONNECTION);
		fprintf(stderr,"nfi_nfs_reconnect: Can't connect with MNT service of %s\n",server);
		free(server_aux);
		return -1;
	}


	ret = nfs_mount(dir, server_aux->fh, server_aux->cl );
	if (ret < 0){
		nfs_err(NFSERR_MOUNT);
	    close_connection_mount(server_aux->cl);
		fprintf(stderr,"nfi_nfs_init: Can't mount %s in server %s (err:%d).\n",dir,server,ret);
		free(server_aux);
		return -1;
	}

        close_connection_mount(server_aux->cl);
 
	server_aux->cl = create_connection_nfs(server, NFS_UDP);
	if(server_aux->cl == NULL){
		nfs_err(NFSERR_NFSCONNECTION);
		fprintf(stderr,"nfi_nfs_reconnect: Can't connect with NFS service of %s\n",server);
		free(server_aux);
		return -1;
	}


	serv->private_info = (void *)server_aux;
	return 0;
}

/************************************************************
 * Destroy NFS operations				    *
 ************************************************************/

int nfi_nfs_destroy(struct nfi_server *serv)
{

	struct nfi_nfs_server *server_aux;
        char dir[NFSPATHLEN];
	int ret;

	if (serv == NULL)
		return 0;

	if(serv->ops != NULL)
		free(serv->ops);

	server_aux = (struct nfi_nfs_server *)serv->private_info;

	if(server_aux != NULL){
		if( server_aux->cl != NULL)
			close_connection_nfs( server_aux->cl);

	
	
		ret = ParseURL(serv->url, NULL, NULL, NULL, NULL,  NULL,  dir);
		if(ret < 0){
			nfs_err(NFSERR_URL);
                 	free(serv->private_info);
					fprintf(stderr,"nfi_nfs_destroy: url %s incorrect.\n",serv->url);
			return -1;
		}

		server_aux->cl = create_connection_mount(serv->server, NFS_UDP);
		if(server_aux->cl != NULL){
			ret = nfs_umount(dir, server_aux->cl);	
			close_connection_mount(server_aux->cl);
		}
	

		free(serv->private_info);
	}



	if(serv->url != NULL){
		free(serv->url);
	}

	if(serv->server != NULL){
		free(serv->server);
	}

	//serv->protocol = -1;

	nfi_worker_end(serv->wrk);

	return 0;
}




/************************************************************
 * GENERIC FUNCTIONS 										*
 ************************************************************/

int nfi_nfs_getattr(struct nfi_server *serv,  struct nfi_fhandle *fh, struct nfi_attr *attr){

	int ret;
	fattr fatt;

	struct nfi_nfs_server *server_aux;
	struct nfi_nfs_fhandle *fh_aux;


	if (attr == NULL){	
		nfs_err(NFSERR_PARAM);
		fprintf(stderr,"nfi_nfs_getattr: attr parameter incorrect\n");
		return -1;
	}


	if (serv == NULL){	
		nfs_err(NFSERR_PARAM);
		fprintf(stderr,"nfi_nfs_getattr: serv parameter incorrect\n");
		return -1;
	}


	if (fh == NULL){	
		nfs_err(NFSERR_PARAM);
		fprintf(stderr,"nfi_nfs_getattr: fh parameter incorrect\n");
		return -1;
	}

	if (fh->priv_fh == NULL){
		nfs_err(NFSERR_PARAM);
		fprintf(stderr,"nfi_nfs_getattr: fh->priv_fh parameter incorrect\n");
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_nfs_reconnect(serv);
		if(ret <0){
			/* nfs_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               nfs_err(NFSERR_PARAM);
               return -1;
        }
#endif
	fh_aux = (struct nfi_nfs_fhandle *) fh->priv_fh;
	server_aux = (struct nfi_nfs_server *) serv->private_info;

	ret = nfs_getattr(fh_aux->fh, &fatt, server_aux->cl);
	if(ret < 0){
		nfs_err(NFSERR_GETATTR);
		fprintf(stderr,"nfi_nfs_getattr: Fail getattr %s in server %s (err:%d).\n",fh->url,serv->server,ret);
		return -1;
	}

	NFStoNFIattr(attr, &fatt);
	return 0;
}


int nfi_nfs_setattr(struct nfi_server *serv,  struct nfi_fhandle *fh, struct nfi_attr *attr){

	int ret;
	fattr fatt;

	struct nfi_nfs_server *server_aux;
	struct nfi_nfs_fhandle *fh_aux;


	if (attr == NULL){	
		nfs_err(NFSERR_PARAM);
		nfs_err(NFSERR_PARAM);
		return -1;
	}

	if (serv == NULL){	
		nfs_err(NFSERR_PARAM);
		return -1;
	}

	if (fh->priv_fh == NULL){	
		nfs_err(NFSERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_nfs_reconnect(serv);
		if(ret <0){
			/* nfs_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               nfs_err(NFSERR_PARAM);		
               return -1;
        }
#endif


	NFItoNFSattr(&fatt,attr);

	fh_aux = (struct nfi_nfs_fhandle *) fh->priv_fh;
	server_aux = (struct nfi_nfs_server *) serv->private_info;

	ret = nfs_setattr(fh_aux->fh, &fatt, server_aux->cl);
	if(ret < 0){
		nfs_err(NFSERR_GETATTR);
		fprintf(stderr,"nfi_nfs_setattr: Fail setattr %s in server %s (err:%d).\n",fh->url,serv->server,ret);
		return -1;
	}

	return 0;
}


int nfs_open(struct nfi_server *serv,  char *url, struct nfi_fhandle *fho )
{
	char dir[NFSPATHLEN], server[NFSPATHLEN];
	int ret;
	struct nfi_nfs_server *server_aux;
	struct nfi_nfs_fhandle *fh_aux;

	
	if (serv == NULL){	
		nfs_err(NFSERR_PARAM);
		return -1;
	}

	if (fho == NULL){	
		nfs_err(NFSERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_nfs_reconnect(serv);
		if(ret <0){
			/* nfs_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               nfs_err(NFSERR_PARAM);
               return -1;
        }
#endif

	ret = ParseURL(url, NULL, NULL, NULL, server,  NULL,  dir);
	if(ret < 0){
		nfs_err(NFSERR_URL);
		fprintf(stderr,"nfs_open: url %s incorrect.\n",url);
		return -1;
	}

	fho->url = (char *)malloc(strlen(url)+1);
	if(fho->url == NULL){
		nfs_err(NFSERR_MEMORY);
		return -1;
	}

	strcpy(fho->url, url);

	fh_aux = (struct nfi_nfs_fhandle *)malloc(sizeof(struct nfi_nfs_fhandle));
	if (fh_aux == NULL){
		nfs_err(NFSERR_MEMORY);
		fprintf(stderr,"nfs_open: fho->url is null. Can't reserve memory\n");
		free(fho->url);
		return -1;
	}
	
	server_aux = (struct nfi_nfs_server *) serv->private_info;

	getDirWithURL(serv->url, dir);

	/* obtaine the NFS file handle */
	ret = nfs_lookup(server_aux->fh,
			dir,
			fh_aux->fh,
			NULL,
			server_aux->cl);

#define DBG_NFI 0
#ifdef  DBG_NFI
	//fprintf(stderr,"nfs_open: lookup %s in server %s (err:%d).\n",dir,serv->server,ret);
#endif

	if (ret < 0) {
		//fprintf(stderr,"nfs_open: Fail lookup %s in server %s (err:%d).\n",dir,serv->server,ret);
		//nfs_err(NFSERR_LOOKUP);
		//free(fho->url);
		free(fh_aux) ;
		return -1;
	}

	switch (ret)
	{
		case NFREG:
			fho->type = NFIFILE;
			break;
		case NFDIR:
			fho->type = NFIDIR;
			break;
	}
	fho->server = NULL;
	fho->priv_fh = NULL;
	fho->server = serv;
	fho->priv_fh = (void *) fh_aux;

	return 0;
}


int nfi_nfs_open(struct nfi_server *serv,  char *url, struct nfi_fhandle *fho )
{
	if (nfs_open(serv,  url, fho ) <0){
		//fprintf(stderr,"nfi_nfs_open: Fail %s\n",url);
		return -1;
	}

	if (fho->type != NFIFILE){
		nfi_nfs_close(serv, fho);
		fprintf(stderr,"nfi_nfs_open: Fail %s isn't a FILE.\n",url);
		return -1;
	}
	
	return 0;
}




int nfi_nfs_close(struct nfi_server *server,  struct nfi_fhandle *fh){
	if (server == NULL){	
		nfs_err(NFSERR_PARAM);
		fprintf(stderr,"nfi_nfs_close: server is null.\n");
		return -1;
	}
	if (fh == NULL){	
		nfs_err(NFSERR_PARAM);
		fprintf(stderr,"nfi_nfs_close: fh is null.\n");
		return -1;
	}

	if (fh->priv_fh != NULL){
		/* free memory */
		//fprintf(stderr,"nfi_nfs_close: fh->priv_fh is null.\n");
		free(fh->priv_fh);
		fh->priv_fh = NULL;
	
	}
	
	if (fh->url != NULL){
		free(fh->url);
	}

	fh->type = NFINULL;
	fh->server = NULL;

	return 0;
}

ssize_t nfi_nfs_read(	struct nfi_server *serv,
			struct nfi_fhandle *fh,
			void *buffer,
			off_t offset,
			size_t size)
{

	ssize_t new_size;

	struct nfi_nfs_server *server_aux;
	struct nfi_nfs_fhandle *fh_aux;

	if (serv == NULL){	
		nfs_err(NFSERR_PARAM);
		return -1;
	}


	if (fh == NULL){	
		nfs_err(NFSERR_PARAM);
		return -1;
	}


#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_nfs_reconnect(serv);
		if(ret <0){
			/* nfs_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               nfs_err(NFSERR_PARAM);
               return -1;
        }
#endif


	fh_aux = (struct nfi_nfs_fhandle *) fh->priv_fh;
	server_aux = (struct nfi_nfs_server *) serv->private_info;



//fprintf(stderr, "nfs_read(offset=%d, size=%d)", offset, size);
	new_size = nfs_read(fh_aux->fh,
			    (char *)buffer,
			    offset,
			    size,
			    server_aux->cl);
//fprintf(stderr, "=%d\n", new_size);
#ifdef  DBG_NFI
	//fprintf(stderr,"nfi_nfs_read: read %s off %d size %d (err:%d).\n",fh->url,(int)offset,(int)size,(int)new_size);
#endif

	if(new_size < 0){
		nfs_err(NFSERR_READ);
		fprintf(stderr,"nfi_nfs_read: Fail read %s off %d size %d (err:%d).\n",fh->url,(int)offset,(int)size,(int)new_size);
		return -1;
	}


	return new_size;
}

ssize_t nfi_nfs_write(	struct nfi_server *serv,
			struct nfi_fhandle *fh,
			void *buffer,
			off_t offset,
			size_t size)
{

	ssize_t new_size;
        struct nfi_nfs_server *server_aux;
        struct nfi_nfs_fhandle *fh_aux;

	if (serv == NULL){	
		nfs_err(NFSERR_PARAM);
		return -1;
	}


	if (fh == NULL){	
		nfs_err(NFSERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_nfs_reconnect(serv);
		if(ret <0){
			/* nfs_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               nfs_err(NFSERR_PARAM);
               return -1;
        }
#endif


        fh_aux = (struct nfi_nfs_fhandle *) fh->priv_fh;
        server_aux = (struct nfi_nfs_server *) serv->private_info;


	new_size = nfs_write(fh_aux->fh,
			(char *)buffer,
			offset,
			size,
			server_aux->cl);
#define DBG_NFI 0
#ifdef  DBG_NFI
	//fprintf(stderr,"nfi_nfs_write: write %s off %d size %d (err:%d).\n",fh->url,(int)offset,(int)size,(int)new_size);
#endif

	if(new_size < 0){
		fprintf(stderr,"nfi_nfs_write: Fail write %s off %d size %d (err:%d).\n",fh->url,(int)offset,(int)size,(int)new_size);
		nfs_err(NFSERR_WRITE);
		return -1;
	}


	return new_size;
}

int nfi_nfs_create(struct nfi_server *serv,  char *url, struct nfi_attr *attr, struct nfi_fhandle  *fh){

	char server[NFSPATHLEN], dir[NFSPATHLEN], file[NFSPATHLEN];
	int ret;
	fhandle fhAux;
	fattr fatt;		/* NFS attributes */

        struct nfi_nfs_server *server_aux;
        struct nfi_nfs_fhandle *fh_aux;


	/*************************/
	/* problemas con el test */
	/*************************/
	ret = nfs_open(serv,  url, fh);
	if (ret != -1){
		return ret;
	}

	if (serv == NULL){	
		nfs_err(NFSERR_PARAM);
		return -1;
	}

	if (attr == NULL){	
		nfs_err(NFSERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_nfs_reconnect(serv);
		if(ret <0){
			/* nfs_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               nfs_err(NFSERR_PARAM);
               return -1;
        }
#endif


	ret = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir);
	if(ret < 0){
		nfs_err(NFSERR_URL);
		fprintf(stderr,"nfi_nfs_create: url %s incorrect.\n",url);
		return -1;
	}


	/* elimina del dir la parte del fichero */
	ret = getNameFile(file, dir);
	if(ret < 0){
		nfs_err(NFSERR_URL);
	
		fprintf(stderr,"nfi_nfs_create: url %s incorrect.\n",url);
		return -1;
	}

	/* private_info file handle */
	fh_aux = (struct nfi_nfs_fhandle *)malloc(sizeof(struct nfi_nfs_fhandle));
	if (fh_aux == NULL){
		nfs_err(NFSERR_MEMORY);
		fprintf(stderr,"nfi_nfs_create: fh_aux is null. Can't reserve memory\n");
		return -1;
	}

	bzero(fh_aux, sizeof(struct nfi_nfs_fhandle));


    	server_aux = (struct nfi_nfs_server *) serv->private_info;
	getDirWithURL(serv->url,dir);
	/* obtain the directory file handle */

	ret = nfs_lookup(server_aux->fh,
			dir,
			fhAux,
			NULL,
			server_aux->cl);
	if(ret < 0){
		nfs_err(NFSERR_LOOKUP);
		fprintf(stderr,"nfi_nfs_create: Fail lookup %s in server %s (ret:%d).\n",dir,serv->server,ret);
		free(fh_aux);
		return -1;
	}

	/* create the file into the directory */
	ret = nfs_create(fhAux,
			  file,
			  attr->at_mode,
			  fh_aux->fh,
			  &fatt,
			  server_aux->cl);

	if(ret < 0){
		nfs_err(NFSERR_CREATE);
		fprintf(stderr,"nfi_nfs_create: Fail create %s in server %s (ret:%d).\n",url,serv->server,ret);
		free(fh_aux);
		return -1;
	}
	fh->server = serv;
        fh->priv_fh = (void *)fh_aux;

        fh->url = (char *)malloc(strlen(url)+1);
        if(fh->url == NULL){
               nfs_err(NFSERR_MEMORY);
			   fprintf(stderr,"nfi_nfs_init: fh->url is null. Can't reserve memory\n");
               return -1;
        }
        strcpy(fh->url, url);

	NFStoNFIattr(attr, &fatt);

	return 0;
}

int nfi_nfs_remove(struct nfi_server *serv,  char *url)
{

	char server[NFSPATHLEN], dir[NFSPATHLEN], file[NFSPATHLEN];
	int ret;
	fhandle fhAux;


        struct nfi_nfs_server *server_aux;


	if (serv == NULL){	
		nfs_err(NFSERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_nfs_reconnect(serv);
		if(ret <0){
			/* nfs_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               nfs_err(NFSERR_PARAM);
               return -1;
        }
#endif


	server_aux = (struct nfi_nfs_server *)serv->private_info;

	ret = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir);
	if(ret < 0){
		nfs_err(NFSERR_URL);
		fprintf(stderr,"nfi_nfs_remove: url %s incorrect.\n",url);
		return -1;
	}
	/* elimina del dir la parte del fichero */
	ret = getNameFile(file, dir);
	if(ret < 0){
		nfs_err(NFSERR_URL);
		fprintf(stderr,"nfi_nfs_remove: url %s incorrect.\n",url);
		return -1;
	}

	getDirWithURL(serv->url,dir);
        /* obtain the directory file handle */
	ret = nfs_lookup(server_aux->fh,
			dir,
			fhAux,
			NULL,
			server_aux->cl);

	if(ret < 0){
		fprintf(stderr,"nfi_nfs_lookup: Fail lookup %s in server %s (ret:%d).\n",dir,serv->server,ret);
		nfs_err(NFSERR_LOOKUP);
		return -1;
	}

	/* remove the file into the directory */
	ret = nfs_remove(fhAux,
			  file, 				
			  server_aux->cl);

	if(ret < 0){
		//fprintf(stderr,"nfi_nfs_remove: Fail remove %s in server %s (ret:%d).\n",dir,serv->server,ret);
		nfs_err(NFSERR_REMOVE);
		return -1;
	}



	return 0;
}

int nfi_nfs_rename(struct nfi_server *serv,  char *old_url, char *new_url)
{

        struct nfi_nfs_server *server_aux;
        fhandle fhAux_src, fhAux_dest;
        char server[NFSPATHLEN], dir_src[NFSPATHLEN], file_src[NFSPATHLEN], dir_dest[NFSPATHLEN], file_dest[NFSPATHLEN];
        int ret;


	if (serv == NULL){	
		nfs_err(NFSERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_nfs_reconnect(serv);
		if(ret <0){
			nfs_err(); not necessary
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               nfs_err(NFSERR_PARAM);
               return -1;
        }
#endif
	server_aux = (struct nfi_nfs_server *)serv->private_info;
        bzero(fhAux_src, sizeof(fhandle));
        bzero(fhAux_dest, sizeof(fhandle));

        /* obtain the directory file handle */
        ret = ParseURL(old_url,  NULL, NULL, NULL, server,  NULL,  dir_src);
        if(ret < 0){
                nfs_err(NFSERR_URL);
                fprintf(stderr,"nfi_nfs_rename: url %s incorrect.\n",old_url);
                return -1;
        }

        /* elimina del dir la parte del fichero */
        ret = getNameFile(file_src, dir_src);
        if(ret < 0){
                nfs_err(NFSERR_URL);
                fprintf(stderr,"nfi_nfs_rename: url %s incorrect.\n",old_url);
                return -1;
        }

        /* obtain the directory file handle */
        ret = ParseURL(new_url,  NULL, NULL, NULL, server,  NULL,  dir_dest);
        if(ret < 0){
                nfs_err(NFSERR_URL);
                fprintf(stderr,"nfi_nfs_rename: url %s incorrect.\n",new_url);
                return -1;
        }

        /* elimina del dir la parte del fichero */
        ret = getNameFile(file_dest, dir_dest);
        if(ret < 0){
                nfs_err(NFSERR_URL);
                fprintf(stderr,"nfi_nfs_rename: url %s incorrect.\n",new_url);
                return -1;
        }

	getDirWithURL(serv->url,dir_src);
	getDirWithURL(serv->url,dir_dest);

	/* obtain the directory file handle */
        ret = nfs_lookup(server_aux->fh,
                        dir_src,
                        fhAux_src,
                        NULL,
                        server_aux->cl);
        if(ret < 0){
                nfs_err(NFSERR_LOOKUP);
                fprintf(stderr,"nfi_nfs_rename: Fail lookup %s in server %s (ret:%d).\n",dir_src,serv->server,ret);
                //free(fh_aux);
                return -1;
        }


        /* obtain the directory file handle */
        ret = nfs_lookup(server_aux->fh,
                        dir_dest,
                        fhAux_dest,
                        NULL,
                        server_aux->cl);
        if(ret < 0){
                nfs_err(NFSERR_LOOKUP);
                fprintf(stderr,"nfi_nfs_rename: Fail lookup %s in server %s (ret:%d).\n",dir_dest,serv->server,ret);
                //free(fh_aux);
                return -1;
        }


	ret = nfs_rename(fhAux_src, file_src, fhAux_dest, file_dest, server_aux->cl);
	if(ret < 0){
		//fprintf(stderr,"nfi_nfs_remove: Fail remove %s in server %s (ret:%d).\n",dir,serv->server,ret);
		nfs_err(NFSERR_REMOVE);
		return -1;
	}


	return 0;
}

int nfi_nfs_mkdir(struct nfi_server *serv,  char *url, struct nfi_attr *attr, struct nfi_fhandle *fh)
{
	char server[NFSPATHLEN], dir[NFSPATHLEN], file[NFSPATHLEN];
	int ret;
	fhandle fhAux;
	fattr fatt;		/* NFS attributes */


        struct nfi_nfs_server *server_aux;
        struct nfi_nfs_fhandle *fh_aux;


	if (serv == NULL){	
		nfs_err(NFSERR_PARAM);
		return -1;
	}

	if (attr == NULL){	
		nfs_err(NFSERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_nfs_reconnect(serv);
		if(ret <0){
			/* nfs_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               nfs_err(NFSERR_PARAM);
               return -1;
        }
#endif

	server_aux = (struct nfi_nfs_server *)serv->private_info;

	ret = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir);
	if(ret < 0){
		nfs_err(NFSERR_URL);
		return -1;
	}

	/* elimina del dir la parte del fichero */
	ret = getNameFile(file, dir);
	if(ret < 0){
		nfs_err(NFSERR_URL);
		fprintf(stderr,"nfi_nfs_mkdir: url %s incorrect.\n",url);
		return -1;
	}

	/* private_info file handle */
	fh_aux = (struct nfi_nfs_fhandle *)malloc(sizeof(struct nfi_nfs_fhandle));
	if (fh_aux == NULL){
		fprintf(stderr,"nfi_nfs_mkdir: fh_aux is null. Can't reserve memory\n");
		nfs_err(NFSERR_MEMORY);
		return -1;
	}


	bzero(fh_aux, sizeof(struct nfi_nfs_fhandle));

	getDirWithURL(serv->url,dir);
	/* obtain the directory file handle */
	ret = nfs_lookup(server_aux->fh,
			dir,
			fhAux,
			NULL,
			server_aux->cl);

	if(ret < 0){
		nfs_err(NFSERR_LOOKUP);
		fprintf(stderr,"nfi_nfs_mkdir: Fail lookup %s in server %s (ret:%d).\n",dir,serv->server,ret);
		free(fh_aux);
		return -1;
	}

	/* create the dir into the directory */
	ret = nfs_mkdir(fhAux,
			  file,
			  attr->at_mode,
			  fh_aux->fh,
			  &fatt,
			  server_aux->cl);

	if(ret < 0){
		nfs_err(NFSERR_MKDIR);
		fprintf(stderr,"nfi_nfs_mkdir: Fail mkdir %s in server %s (ret:%d).\n",serv->url,serv->server,ret);
		free(fh_aux);
		return -1;
	}

        fh->priv_fh = (void *)fh_aux;

        fh->url = (char *)malloc(strlen(url)+1);
        if(fh->url == NULL){
               nfs_err(NFSERR_MEMORY);
	       free(fh_aux);
               return -1;
        }
        strcpy(fh->url, url);

	NFStoNFIattr(attr, &fatt);


	return 0;
}

int nfi_nfs_rmdir(struct nfi_server *serv,  char *url)
{
	char server[NFSPATHLEN], dir[NFSPATHLEN], file[NFSPATHLEN];
	int ret;
	fhandle fhAux;


        struct nfi_nfs_server *server_aux;

	if (serv == NULL){	
		nfs_err(NFSERR_PARAM);
		return -1;
	}


#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_nfs_reconnect(serv);
		if(ret <0){
			/* nfs_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               nfs_err(NFSERR_PARAM);
               return -1;
        }
#endif


	server_aux = (struct nfi_nfs_server *)serv->private_info;

	ret = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir);
	if(ret < 0){
		nfs_err(NFSERR_URL);
		fprintf(stderr,"nfi_nfs_mkdir: url %s incorrect.\n",url);
		return -1;
	}
	/* elimina del dir la parte del fichero */
	ret = getNameFile(file, dir);
	if(ret < 0){
		nfs_err(NFSERR_URL);
		fprintf(stderr,"nfi_nfs_mkdir: url %s incorrect.\n",url);
		return -1;
	}

	getDirWithURL(serv->url,dir);
	/* obtain the directory file handle */
	ret = nfs_lookup(server_aux->fh,
			dir,
			fhAux,
			NULL,
			server_aux->cl);

	if(ret < 0){
		nfs_err(NFSERR_LOOKUP);
		fprintf(stderr,"nfi_nfs_rmdir: Fail lookup %s in server %s (ret:%d).\n",dir,serv->server,ret);
		return -1;
	}

	/* remove the dir into the directory */
	ret = nfs_rmdir(fhAux,
			  file, 				
			  server_aux->cl);
	if(ret < 0){
		nfs_err(NFSERR_REMOVE);
		fprintf(stderr,"nfi_nfs_rmdir: Fail rmdir %s in server %s (ret:%d).\n",serv->url,serv->server,ret);
		return -1;
	}

	return 0;
}

int nfi_nfs_opendir(struct nfi_server *server,  char *url, struct nfi_fhandle *fho)
{
        if (nfs_open(server,  url, fho ) <0){
                return -1;
        }

	if (fho->type != NFIDIR){
	        nfi_nfs_close(server, fho);
	        return -1;
	}
        return 0;

}

int nfi_nfs_readdir(struct nfi_server *serv, struct nfi_fhandle *fh, struct dirent *entry )
{
	int ret;


        struct nfi_nfs_server *server_aux;
        struct nfi_nfs_fhandle *fh_aux;


	if (serv == NULL){	
		nfs_err(NFSERR_PARAM);
		return -1;
	}


	if (fh == NULL){	
		nfs_err(NFSERR_PARAM);
		return -1;
	}

	if (fh->priv_fh == NULL){	
		nfs_err(NFSERR_PARAM);
		return -1;
	}

	if(fh->type != NFIDIR){
		nfs_err(NFSERR_NOTDIR);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_nfs_reconnect(serv);
		if(ret <0){
			/* nfs_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               nfs_err(NFSERR_PARAM);
               return -1;
        }
#endif

	server_aux = (struct nfi_nfs_server *)serv->private_info;
	fh_aux = (struct nfi_nfs_fhandle *)fh->priv_fh;

	if(fh_aux->eofdir){
		entry->d_name[0] = '\0';
		return 0;
	}

	// TODO: update nfs_readdir to new header
	ret = nfs_readdir(fh_aux->fh, fh_aux->cookie, entry, server_aux->cl);
	if((ret < 0)&&(ret != NFSERR_EOFDIR)){
		nfs_err(NFSERR_READDIR);
		return -1;	
	}

	if(ret == NFSERR_EOFDIR){
		fh_aux->eofdir = 1;
	}

	return 0;
}

int nfi_nfs_closedir(struct nfi_server *serv,  struct nfi_fhandle *fh)
{
	return nfi_nfs_close(serv, fh);
}


int nfi_nfs_statfs(struct nfi_server *serv,  struct nfi_info *inf)
{
	struct nfs_info nfsinf;
	int ret;
        struct nfi_nfs_server *server_aux;


	if (serv == NULL){	
		nfs_err(NFSERR_PARAM);
		return -1;
	}

	if (inf == NULL){	
		nfs_err(NFSERR_PARAM);
		return -1;
	}


#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_nfs_reconnect(serv);
		if(ret <0){
		/* nfs_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               nfs_err(NFSERR_PARAM);
               return -1;
        }
#endif

	server_aux = (struct nfi_nfs_server *)serv->private_info;
	ret = nfs_statfs(server_aux->fh, &nfsinf, server_aux->cl);
	if(ret <0){
		nfs_err(NFSERR_STATFS);
		return -1;
	}

	NFStoNFIInfo(inf, &nfsinf);
	return 0;
}
