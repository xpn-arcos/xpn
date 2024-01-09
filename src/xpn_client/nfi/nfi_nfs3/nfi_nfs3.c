
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

#include "nfi_nfs3.h"


/************************************************************
 * PRIVATE FUNCTIONS TO USE NFS SERVERS			    *
 ************************************************************/
void NFItoNFS3attr(fattr3 *nfs3_att, struct nfi_attr *nfi_att)
{
        nfs3_att->st_dev = nfi_att->st_dev ;
        nfs3_att->st_ino = nfi_att->st_ino ;

	switch(nfi_att->at_type)
	{
		case NFIDIR:
			nfs3_att->type = NF3DIR;
			break;
                case NFIFILE:
                        nfs3_att->type = NF3REG;
                        break;
	}

        nfs3_att->mode 	= (u_long)nfi_att->at_mode;
        /* u_long nlink; */
        nfs3_att->uid 	= (u_long)nfi_att->at_uid;
        nfs3_att->gid	= (u_long)nfi_att->at_gid;
        nfs3_att->size	= (u_long)nfi_att->at_size;
	//nfs3_att->blocksize = (u_long)nfi_att->at_blksize;
	/* u_long rdev;  */
	//nfs3_att->blocks	= (u_long)nfi_att->at_blocks;
	/* u_long fsid;  */
	/* u_long fileid;*/
	nfs3_att->atime.seconds 	= (u_long)nfi_att->at_atime;
	//nfs3_att->atime.useconds	= (u_long)0;
        nfs3_att->mtime.seconds	= (u_long)nfi_att->at_atime;
        //nfs3_att->mtime.useconds	= (u_long)0;
        nfs3_att->ctime.seconds	= (u_long)nfi_att->at_ctime;
        //nfs3_att->ctime.useconds	= (u_long)0;
}

void NFS3toNFIattr(struct nfi_attr *nfi_att, fattr3 *nfs3_att)
{
        nfi_att->st_dev = nfs3_att->st_dev;
        nfi_att->st_ino = nfs3_att->st_ino;

	switch (nfs3_att->type)
	{
		case NF3DIR:
			nfi_att->at_type = NFIDIR;
			break;
                case NF3REG:
                        nfi_att->at_type = NFIFILE;
                        break;
		default:
			break;
	}

        nfi_att->at_mode = (mode_t)nfs3_att->mode;
        /* u_long nlink; */
        nfi_att->at_uid = (uid_t)nfs3_att->uid;
        nfi_att->at_gid = (gid_t)nfs3_att->gid;
        nfi_att->at_size = (off_t)nfs3_att->size;
	//nfi_att->at_blksize = (u_long)nfs3_att->blocksize;
	/* u_long rdev;  */
	//nfi_att->at_blocks = (u_long)nfs3_att->blocks;
	/* u_long fsid;  */
	/* u_long fileid;*/
        nfi_att->at_atime = (time_t)nfs3_att->atime.seconds;
	                //+ (1000*nfs3_att->atime.useconds);
	nfi_att->at_mtime = (time_t)nfs3_att->mtime.seconds;
			//+ (1000*nfs3_att->mtime.useconds);
	nfi_att->at_ctime = (time_t)nfs3_att->ctime.seconds;
	                //+ (1000*nfs3_att->ctime.useconds);

	nfi_att->private_info = NULL;
}

void NFS3toNFIInfo(__attribute__((__unused__)) struct nfi_info *nfi_inf, __attribute__((__unused__)) fsinfo3resok *nfs3_inf)
{
    // TODO
}



/************************************************************
 * PUBLIC FUNCTIONS TO USE NFS SERVERS			    *
 ************************************************************/

/************************************************************
 * Init NFS server					    *
 ************************************************************/
int nfi_nfs3_init(char *url, struct nfi_server *serv, __attribute__((__unused__)) struct nfi_attr_server *attr)
{
	char server[MNTNAMLEN], dir[MNTNAMLEN], prt[MNTNAMLEN];
	int ret;
	struct nfi_nfs3_server *server_aux;


	if(serv == NULL){
		nfs3_err(NFS3ERR_PARAM);
		fprintf(stderr,"nfi_nfs3_init: serv is null. Don't inicialize this variable\n");
		return -1;
	}
	/* functions */
	serv->ops = (struct nfi_ops *)malloc(sizeof(struct nfi_ops));
	if (serv->ops == NULL){
		nfs3_err(NFS3ERR_MEMORY);
		fprintf(stderr,"nfi_nfs3_init: serv->ops is null. Can't reserve memory\n");
		return -1;
	}

	serv->ops->nfi_reconnect  = nfi_nfs3_reconnect;
	serv->ops->nfi_disconnect = nfi_nfs3_disconnect;

	serv->ops->nfi_getattr	= nfi_nfs3_getattr;
	serv->ops->nfi_setattr	= nfi_nfs3_setattr;

	serv->ops->nfi_open	= nfi_nfs3_open;
	serv->ops->nfi_close	= nfi_nfs3_close;

	serv->ops->nfi_read	= nfi_nfs3_read;
	serv->ops->nfi_write	= nfi_nfs3_write;

	serv->ops->nfi_create	= nfi_nfs3_create;
	serv->ops->nfi_remove	= nfi_nfs3_remove;
	serv->ops->nfi_rename	= nfi_nfs3_rename;

	serv->ops->nfi_mkdir	= nfi_nfs3_mkdir;
	serv->ops->nfi_rmdir	= nfi_nfs3_rmdir;
	serv->ops->nfi_opendir	= nfi_nfs3_opendir;
	serv->ops->nfi_readdir	= nfi_nfs3_readdir;
	serv->ops->nfi_closedir	= nfi_nfs3_closedir;
	serv->ops->nfi_statfs	= nfi_nfs3_statfs;

	//printf("url %s\n",url);
	ret = ParseURL(url, prt, NULL, NULL, server,  NULL,  dir);
	if(ret <0 ){
		nfs3_err(NFS3ERR_URL);
		fprintf(stderr,"nfi_nfs3_init: url %s incorrect.\n",url);
		free(serv->ops);
		return -1;
	}

	//printf("prt %s server %s dir %s\n",prt,server,dir);
	server_aux = (struct nfi_nfs3_server *)malloc(sizeof(struct nfi_nfs3_server));
	if(server_aux == NULL){
		nfs3_err(NFS3ERR_MEMORY);
		fprintf(stderr,"nfi_nfs3_init: server_aux is null. Can't reserve memory\n");
		return -1;
	}
	server_aux->cl = (CLIENT *)create_connection_mount3(server, NFS3_UDP);

	if(server_aux->cl == NULL){
		nfs3_err(NFS3ERR_MNTCONNECTION);
		fprintf(stderr,"nfi_nfs3_init: Can't connect with MNT service of %s\n",server);
		free(server_aux);
		return -1;
	}
	//printf("antes de mount(%s)\n",dir);
	ret = nfs3_mount(dir, &server_aux->fh, server_aux->cl );
	//printf("desp. de mount(%s) %d\n",dir,ret);
#if defined(DEBUG_NFI)
	printf("desp. de montar %d\n",ret);
#endif
	if (ret < 0){
		nfs3_err(-ret);
	        close_connection_mount3(server_aux->cl);
		fprintf(stderr,"nfi_nfs3_init: Can't mount %s in server %s (err:%d).\n",dir,server,ret);
		free(server_aux);
		return -1;
	}
	nfs3_err(ret);

        close_connection_mount3(server_aux->cl);
 	if((strcmp(prt, "nfs3") == 0) ||
		(strcmp(prt, "nfs3tcp") == 0) ||
	   	(strcmp(prt, "nfs3tcphilos") == 0)){
#if defined(DEBUG_NFI)
		printf("nfs3\n");
#endif

		//printf("nfs3tcp\n");
		server_aux->cl = create_connection_nfs3(server, NFS3_TCP);
	}
	else{
		//printf("nfs3\n");
		server_aux->cl = create_connection_nfs3(server, NFS3_UDP);
	}
	if(server_aux->cl == NULL){
		nfs3_err(NFS3ERR_NFSCONNECTION);
		fprintf(stderr,"nfi_nfs3_init: Can't connect with NFS service of %s\n",server);
		free(server_aux);
		return -1;
	}

	serv->private_info = (void *)server_aux;

	//serv->protocol = NFS3;

	serv->server = (char *)malloc(sizeof(char)*(strlen(server)+1));	/* server address */
	if(serv->server == NULL){
		fprintf(stderr,"nfi_nfs3_init: serv->server is null. Can't reserve memory\n");
		nfs3_err(NFS3ERR_MEMORY);
		return -1;
	}
	strcpy(serv->server, server);

	serv->url = (char *)malloc(sizeof(char)*(strlen(url)+1));	/* server address */
	if(serv->url == NULL){
		fprintf(stderr,"nfi_nfs3_init: serv->url is null. Can't reserve memory\n");
		nfs3_err(NFS3ERR_MEMORY);
		close_connection_nfs3(server_aux->cl);
		free(serv->server);
		free(server_aux);
		return -1;
	}
	strcpy(serv->url, url);
        serv->wrk = (struct nfi_worker *)malloc(sizeof(struct nfi_worker));
	memset(serv->wrk, 0, sizeof(struct nfi_worker));
	if(serv->wrk == NULL){
		fprintf(stderr,"nfi_nfs3_init: serv->wrk is null. Can't reserve memory\n");
		close_connection_nfs3(server_aux->cl);
		free(serv->server);
		free(serv->url);
		free(server_aux);
		return -1;
	}
	if((strcmp(prt, "nfs3hilos") == 0)  ||
	   (strcmp(prt, "nfs3udphilos") == 0) ||
	   (strcmp(prt, "nfs3tcphilos") == 0)){
#if defined(DEBUG_NFI)
		printf("nfs3hilos\n");
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
int nfi_nfs3_disconnect(struct nfi_server *serv)
{

        struct nfi_nfs3_server *server_aux;
        char dir[MNTNAMLEN];
        int ret;

	if (serv == NULL)
	       return 0;


         server_aux = (struct nfi_nfs3_server *)serv->private_info;
         if(server_aux != NULL){
	         if( server_aux->cl != NULL)
		         close_connection_nfs3( server_aux->cl);
 
		 ret = ParseURL(serv->url, NULL, NULL, NULL, NULL,  NULL,  dir);
		 if(ret < 0){
			fprintf(stderr,"nfi_nfs3_disconnect: url %s incorrect.\n",serv->url);
		        nfs3_err(NFS3ERR_URL);
                        free(serv->private_info);
		        return -1;
		 }
                 server_aux->cl = create_connection_mount3(serv->server, NFS3_UDP);
                 if(server_aux->cl != NULL){
                      ret = nfs3_umount(dir, server_aux->cl);
                      close_connection_mount3(server_aux->cl);
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
int nfi_nfs3_reconnect(struct nfi_server *serv)
{
	/* Don't see the serv result */
	char server[MNTNAMLEN], dir[MNTNAMLEN];
	int ret;
	struct nfi_nfs3_server *server_aux;

	ret = ParseURL(serv->url,  NULL, NULL, NULL, server,  NULL,  dir);
	if(ret <0 ){
		fprintf(stderr,"nfi_nfs3_reconnect: url %s incorrect.\n",serv->url);
		nfs3_err(NFS3ERR_URL);
		return -1;
	}

	server_aux = (struct nfi_nfs3_server *)malloc(sizeof(struct nfi_nfs3_server));
	if(server_aux == NULL){
		fprintf(stderr,"nfi_nfs3_reconnect: server_aux is null. Can't reserve memory\n");
		nfs3_err(NFS3ERR_MEMORY);
		return -1;
	}

	server_aux->cl = (CLIENT *)create_connection_mount3(server, NFS3_UDP);
	if(server_aux->cl == NULL){
		nfs3_err(NFS3ERR_MNTCONNECTION);
		fprintf(stderr,"nfi_nfs3_reconnect: Can't connect with MNT service of %s\n",server);
		free(server_aux);
		return -1;
	}


	ret = nfs3_mount(dir, &server_aux->fh, server_aux->cl );
	if (ret < 0){
		nfs3_err(-ret);
		fprintf(stderr,"nfi_nfs3_init: Can't mount %s in server %s (err:%d).\n",dir,server,ret);
	        close_connection_mount3(server_aux->cl);
		free(server_aux);
		return -1;
	}
		nfs3_err(-ret);

        close_connection_mount3(server_aux->cl);

	server_aux->cl = create_connection_nfs3(server, NFS3_UDP);
	if(server_aux->cl == NULL){
		nfs3_err(NFS3ERR_NFSCONNECTION);
		fprintf(stderr,"nfi_nfs3_reconnect: Can't connect with NFS service of %s\n",server);
		free(server_aux);
		return -1;
	}


	serv->private_info = (void *)server_aux;
	return 0;
}

/************************************************************
 * Destroy NFS operations				    *
 ************************************************************/

int nfi_nfs3_destroy(struct nfi_server *serv)
{

	struct nfi_nfs3_server *server_aux;
        char dir[MNTNAMLEN];
	int ret;

	if (serv == NULL)
		return 0;

	if(serv->ops != NULL)
		free(serv->ops);

	server_aux = (struct nfi_nfs3_server *)serv->private_info;

	if(server_aux != NULL){
		if( server_aux->cl != NULL)
			close_connection_nfs3( server_aux->cl);



		ret = ParseURL(serv->url, NULL, NULL, NULL, NULL,  NULL,  dir);
		if(ret < 0){
			nfs3_err(NFS3ERR_URL);
			fprintf(stderr,"nfi_nfs3_destroy: url %s incorrect.\n",serv->url);
                 	free(serv->private_info);
			return -1;
		}

		server_aux->cl = create_connection_mount3(serv->server, NFS3_UDP);
		if(server_aux->cl != NULL){
			ret = nfs3_umount(dir, server_aux->cl);
			close_connection_mount3(server_aux->cl);
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

int nfi_nfs3_getattr(struct nfi_server *serv,  struct nfi_fhandle *fh, struct nfi_attr *attr){

	int ret;
	fattr3 fatt;

	struct nfi_nfs3_server *server_aux;
	struct nfi_nfs3_fhandle *fh_aux;


	if (attr == NULL){
		nfs3_err(NFS3ERR_PARAM);
		fprintf(stderr,"nfi_nfs3_getattr: attr parameter incorrect\n");
		return -1;
	}


	if (serv == NULL){
		nfs3_err(NFS3ERR_PARAM);
		fprintf(stderr,"nfi_nfs3_getattr: serv parameter incorrect\n");
		return -1;
	}


	if (fh == NULL){
		nfs3_err(NFS3ERR_PARAM);
		fprintf(stderr,"nfi_nfs3_getattr: fh parameter incorrect\n");
		return -1;
	}

	if (fh->priv_fh == NULL){
		nfs3_err(NFS3ERR_PARAM);
		fprintf(stderr,"nfi_nfs3_getattr: fh->priv_fh parameter incorrect\n");
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_nfs3_reconnect(serv);
		if(ret <0){
			/* nfs3_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               nfs3_err(NFS3ERR_PARAM);
               return -1;
        }
#endif
	fh_aux = (struct nfi_nfs3_fhandle *) fh->priv_fh;
	server_aux = (struct nfi_nfs3_server *) serv->private_info;

	ret = nfs3_getattr(&fh_aux->fh, &fatt, server_aux->cl);
	if(ret < 0){
		nfs3_err(-ret);
		fprintf(stderr,"nfi_nfs3_getattr: Fail getattr %s in server %s (err:%d).\n",fh->url,serv->server,ret);
		return -1;
	}

	nfs3_err(0);
	NFS3toNFIattr(attr, &fatt);
	return 0;
}


int nfi_nfs3_setattr(struct nfi_server *serv,  struct nfi_fhandle *fh, struct nfi_attr *attr){

	int ret;
	fattr3 fatt;

	struct nfi_nfs3_server *server_aux;
	struct nfi_nfs3_fhandle *fh_aux;


	if (attr == NULL){
		nfs3_err(NFS3ERR_PARAM);
		return -1;
	}

	if (serv == NULL){
		nfs3_err(NFS3ERR_PARAM);
		return -1;
	}

	if (fh->priv_fh == NULL){
		nfs3_err(NFS3ERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_nfs3_reconnect(serv);
		if(ret <0){
			/* nfs3_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               nfs3_err(NFS3ERR_PARAM);
               return -1;
        }
#endif


	NFItoNFS3attr(&fatt,attr);

	fh_aux = (struct nfi_nfs3_fhandle *) fh->priv_fh;
	server_aux = (struct nfi_nfs3_server *) serv->private_info;

	ret = nfs3_setattr(&fh_aux->fh, &fatt, server_aux->cl);
	if(ret < 0){
		nfs3_err(-ret);
		fprintf(stderr,"nfi_nfs3_setattr: Fail setattr %s in server %s (err:%d).\n",fh->url,serv->server,ret);
		return -1;
	}
	nfs3_err(0);

	return 0;
}

int nfs3_open(struct nfi_server *serv,  char *url, struct nfi_fhandle *fho )
{
	char dir[MNTNAMLEN], server[MNTNAMLEN];
	int ret;
	struct nfi_nfs3_server *server_aux;
	struct nfi_nfs3_fhandle *fh_aux;


	if (serv == NULL){
		nfs3_err(NFS3ERR_PARAM);
		return -1;
	}

	if (fho == NULL){
		nfs3_err(NFS3ERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_nfs3_reconnect(serv);
		if(ret <0){
			/* nfs3_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               nfs3_err(NFS3ERR_PARAM);
               return -1;
        }
#endif

	ret = ParseURL(url, NULL, NULL, NULL, server,  NULL,  dir);
	if(ret < 0){
		nfs3_err(NFS3ERR_URL);
		fprintf(stderr,"nfs3_open: url %s incorrect.\n",url);
		return -1;
	}

	fho->url = (char *)malloc(strlen(url)+1);
	if(fho->url == NULL){
		fprintf(stderr,"nfs3_open: fho->url is null. Can't reserve memory\n");
		nfs3_err(NFS3ERR_MEMORY);
		return -1;
	}

	strcpy(fho->url, url);

	fh_aux = (struct nfi_nfs3_fhandle *)malloc(sizeof(struct nfi_nfs3_fhandle));
	if (fh_aux == NULL){
		nfs3_err(NFS3ERR_MEMORY);
		fprintf(stderr,"nfs3_open: fh_aux is null. Can't reserve memory\n");
		free(fho->url);
		return -1;
	}

	server_aux = (struct nfi_nfs3_server *) serv->private_info;

	getDirWithURL(serv->url, dir);

	/* obtaine the NFS file handle */
	ret = nfs3_lookup(&server_aux->fh,
			dir,
			&fh_aux->fh,
			NULL,
			server_aux->cl);


	if(ret < 0){
		nfs3_err(-ret);
		//fprintf(stderr,"nfs3_open: Fail lookup %s in server %s (err:%d).\n",dir,serv->server,ret);
		//free(fho->url);
		//free(fh_aux);
		//free(server_aux);
		return -1;
	}
	nfs3_err(-ret);
	switch(ret){
		case NF3REG:
			fho->type = NFIFILE;
			break;
		case NF3DIR:
			fho->type = NFIDIR;
			break;
	}
	fho->server = NULL;
	fho->priv_fh = NULL;
	fho->server = serv;
	fho->priv_fh = (void *) fh_aux;

	return 0;

}



int nfi_nfs3_open(struct nfi_server *serv,  char *url, struct nfi_fhandle *fho )
{
	if (nfs3_open(serv,  url, fho ) <0){
		//fprintf(stderr,"nfi_nfs3_open: Fail %s\n",url);
		return -1;
	}

	if (fho->type != NFIFILE){
		nfi_nfs3_close(serv, fho);
		fprintf(stderr,"nfi_nfs3_open: Fail %s isn't a FILE.\n",url);
		return -1;
	}

	return 0;
}




int nfi_nfs3_close(struct nfi_server *server,  struct nfi_fhandle *fh){
	if (server == NULL){
		nfs3_err(NFS3ERR_PARAM);
		return -1;
	}
	if (fh == NULL){
		nfs3_err(NFS3ERR_PARAM);
		return -1;
	}

	if (fh->priv_fh != NULL){
		/* free memory */
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

ssize_t nfi_nfs3_read(	struct nfi_server *serv,
			struct nfi_fhandle *fh,
			void *buffer,
			off_t offset,
			size_t size)
{

	ssize_t new_size;

	struct nfi_nfs3_server *server_aux;
	struct nfi_nfs3_fhandle *fh_aux;

	if (serv == NULL){
		nfs3_err(NFS3ERR_PARAM);
		return -1;
	}


	if (fh == NULL){
		nfs3_err(NFS3ERR_PARAM);
		return -1;
	}


#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_nfs3_reconnect(serv);
		if(ret <0){
			/* nfs3_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               nfs3_err(NFS3ERR_PARAM);
               return -1;
        }
#endif


	fh_aux = (struct nfi_nfs3_fhandle *) fh->priv_fh;
	server_aux = (struct nfi_nfs3_server *) serv->private_info;


//fprintf(stderr, "nfs3_read(offset=%d, size=%d)", offset, size);
	new_size = nfs3_read(&fh_aux->fh,
			    (char *)buffer,
			    offset,
			    size,
			    server_aux->cl);
//fprintf(stderr, "=%d\n", new_size);

	if(new_size < 0){
		nfs3_err(-new_size);
		fprintf(stderr,"nfi_nfs3_read: Fail read %s off %d size %d (err:%d).\n",fh->url,(int)offset,(int)size,(int)new_size);
		return -1;
	}
	nfs3_err(0);


	return new_size;
}

ssize_t nfi_nfs3_write(	struct nfi_server *serv,
			struct nfi_fhandle *fh,
			void *buffer,
			off_t offset,
			size_t size)
{

	ssize_t new_size;
        struct nfi_nfs3_server *server_aux;
        struct nfi_nfs3_fhandle *fh_aux;

	if (serv == NULL){
		nfs3_err(NFS3ERR_PARAM);
		return -1;
	}


	if (fh == NULL){
		nfs3_err(NFS3ERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_nfs3_reconnect(serv);
		if(ret <0){
			/* nfs3_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               nfs3_err(NFS3ERR_PARAM);
               return -1;
        }
#endif


        fh_aux = (struct nfi_nfs3_fhandle *) fh->priv_fh;
        server_aux = (struct nfi_nfs3_server *) serv->private_info;


	new_size = nfs3_write(&fh_aux->fh,
			(char *)buffer,
			offset,
			size,
			server_aux->cl);
	if(new_size < 0){
		nfs3_err(new_size);
		fprintf(stderr,"nfi_nfs3_write: Fail write %s off %d size %d (err:%d).\n",fh->url,(int)offset,(int)size,(int)new_size);
		nfs3_err(NFS3ERR_WRITE);
		return -1;
	}
	nfs3_err(0);


	return new_size;
}

int nfi_nfs3_create(struct nfi_server *serv,  char *url, struct nfi_attr *attr, struct nfi_fhandle  *fh){

	char server[MNTNAMLEN], dir[MNTNAMLEN], file[MNTNAMLEN];
	int ret;
	fhandle3 fhAux;
	fattr3 fatt;		/* NFS attributes */

        struct nfi_nfs3_server *server_aux;
        struct nfi_nfs3_fhandle *fh_aux;

	if (serv == NULL){
		nfs3_err(NFS3ERR_PARAM);
		return -1;
	}

	if (attr == NULL){
		nfs3_err(NFS3ERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_nfs3_reconnect(serv);
		if(ret <0){
			/* nfs3_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               nfs3_err(NFS3ERR_PARAM);
               return -1;
        }
#endif


	ret = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir);
	if(ret < 0){
		nfs3_err(NFS3ERR_URL);
		fprintf(stderr,"nfi_nfs3_create: url %s incorrect.\n",url);
		return -1;
	}


	/* elimina del dir la parte del fichero */
	ret = getNameFile(file, dir);
	if(ret < 0){
		nfs3_err(NFS3ERR_URL);
		fprintf(stderr,"nfi_nfs3_create: url %s incorrect.\n",url);
		return -1;
	}

	/* private_info file handle */
	fh_aux = (struct nfi_nfs3_fhandle *)malloc(sizeof(struct nfi_nfs3_fhandle));
	if (fh_aux == NULL){
		nfs3_err(NFS3ERR_MEMORY);
		fprintf(stderr,"nfi_nfs3_create: fh_aux is null. Can't reserve memory\n");
		return -1;
	}

	bzero(fh_aux, sizeof(struct nfi_nfs3_fhandle));


        server_aux = (struct nfi_nfs3_server *) serv->private_info;
	getDirWithURL(serv->url,dir);
	/* obtain the directory file handle */


	ret = nfs3_lookup(&server_aux->fh,
			dir,
			&fhAux,
			NULL,
			server_aux->cl);
	if(ret < 0){
		nfs3_err(-ret);
		//nfs3_err(NFS3ERR_LOOKUP);
		fprintf(stderr,"nfi_nfs3_create: Fail lookup %s in server %s (ret:%d).\n",dir,serv->server,ret);
		free(fh_aux);
		return -1;
	}
	nfs3_err(0);
	//printf("fhAux = %d - %p \n",fhAux.fhandle3_len,
	//		fhAux.fhandle3_val);
	/* create the file into the directory */
	ret = nfs3_create(&fhAux,
			  file,
			  attr->at_mode,
			  &fh_aux->fh,
			  &fatt,
			  server_aux->cl);

	if(ret < 0){
		nfs3_err(NFS3ERR_CREATE);
		fprintf(stderr,"nfi_nfs3_create: Fail create %s in server %s (ret:%d).\n",url,serv->server,ret);
		free(fh_aux);
		return -1;
	}
	fh->server = serv;
        fh->priv_fh = (void *)fh_aux;

        fh->url = (char *)malloc(strlen(url)+1);
        if(fh->url == NULL){
               nfs3_err(NFS3ERR_MEMORY);
		fprintf(stderr,"nfi_nfs3_init: fh->url is null. Can't reserve memory\n");
               return -1;
        }
        strcpy(fh->url, url);

	NFS3toNFIattr(attr, &fatt);

	return 0;
}

int nfi_nfs3_remove(struct nfi_server *serv,  char *url)
{

	char server[MNTNAMLEN], dir[MNTNAMLEN], file[MNTNAMLEN];
	int ret;
	fhandle3 fhAux;


        struct nfi_nfs3_server *server_aux;


	if (serv == NULL){
		nfs3_err(NFS3ERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_nfs3_reconnect(serv);
		if(ret <0){
			/* nfs3_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               nfs3_err(NFS3ERR_PARAM);
               return -1;
        }
#endif


	server_aux = (struct nfi_nfs3_server *)serv->private_info;

	ret = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir);
	if(ret < 0){
		nfs3_err(NFS3ERR_URL);
		fprintf(stderr,"nfi_nfs3_remove: url %s incorrect.\n",url);
		return -1;
	}
	/* elimina del dir la parte del fichero */
	ret = getNameFile(file, dir);
	if(ret < 0){
		nfs3_err(NFS3ERR_URL);
		fprintf(stderr,"nfi_nfs3_remove: url %s incorrect.\n",url);
		return -1;
	}

	getDirWithURL(serv->url,dir);
        /* obtain the directory file handle */
	ret = nfs3_lookup(&server_aux->fh,
			dir,
			&fhAux,
			NULL,
			server_aux->cl);

	if(ret < 0){
		fprintf(stderr,"nfi_nfs3_lookup: Fail lookup %s in server %s (ret:%d).\n",dir,serv->server,ret);
		nfs3_err(NFS3ERR_LOOKUP);
		return -1;
	}

	/* remove the file into the directory */
	ret = nfs3_remove(&fhAux,
			  file, 
			  server_aux->cl);

	if(ret < 0){
		nfs3_err(-ret);
		//fprintf(stderr,"nfi_nfs3_remove: Fail remove %s in server %s (ret:%d).\n",dir,serv->server,ret);
		nfs3_err(NFS3ERR_REMOVE);
		return -1;
	}
	nfs3_err(0);



	return 0;
}

int nfi_nfs3_rename(__attribute__((__unused__)) struct nfi_server *server, __attribute__((__unused__)) char *old_url, __attribute__((__unused__)) char *new_url)
{
	/*
        struct nfi_nfs3_server *server_aux;
        struct nfi_nfs3_fhandle *fh_aux;
	if (server == NULL){
		nfs3_err(NFS3ERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_nfs3_reconnect(serv);
		if(ret <0){
			nfs3_err(); not necessary
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               nfs3_err(NFS3ERR_PARAM);
               return -1;
        }
#endif
	server_aux = (strcut nfi_nfs3_server *)serv->private_info;
	*/

	/* ????? */
	return 0;
}

int nfi_nfs3_mkdir(struct nfi_server *serv,  char *url, struct nfi_attr *attr, struct nfi_fhandle *fh)
{
	char server[MNTNAMLEN], dir[MNTNAMLEN], file[MNTNAMLEN];
	int ret;
	fhandle3 fhAux;
	fattr3 fatt;		/* NFS attributes */


        struct nfi_nfs3_server *server_aux;
        struct nfi_nfs3_fhandle *fh_aux;


	if (serv == NULL){
		nfs3_err(NFS3ERR_PARAM);
		return -1;
	}

	if (attr == NULL){
		nfs3_err(NFS3ERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_nfs3_reconnect(serv);
		if(ret <0){
			/* nfs3_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               nfs3_err(NFS3ERR_PARAM);
               return -1;
        }
#endif

	server_aux = (struct nfi_nfs3_server *)serv->private_info;

	ret = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir);
	if(ret < 0){
		nfs3_err(NFS3ERR_URL);
		fprintf(stderr,"nfi_nfs3_mkdir: url %s incorrect.\n",url);
		return -1;
	}

	/* elimina del dir la parte del fichero */
	ret = getNameFile(file, dir);
	if(ret < 0){
		nfs3_err(NFS3ERR_URL);
		fprintf(stderr,"nfi_nfs3_mkdir: url %s incorrect.\n",url);
		return -1;
	}

	/* private_info file handle */
	fh_aux = (struct nfi_nfs3_fhandle *)malloc(sizeof(struct nfi_nfs3_fhandle));
	if (fh_aux == NULL){
		nfs3_err(NFS3ERR_MEMORY);
		fprintf(stderr,"nfi_nfs3_mkdir: fh_aux is null. Can't reserve memory\n");
		return -1;
	}


	bzero(fh_aux, sizeof(struct nfi_nfs3_fhandle));

	getDirWithURL(serv->url,dir);
	/* obtain the directory file handle */
	ret = nfs3_lookup(&server_aux->fh,
			dir,
			&fhAux,
			NULL,
			server_aux->cl);

	if(ret < 0){
		//nfs3_err(NFS3ERR_LOOKUP);
		nfs3_err(-ret);
		fprintf(stderr,"nfi_nfs3_mkdir: Fail lookup %s in server %s (ret:%d).\n",dir,serv->server,ret);
		free(fh_aux);
		return -1;
	}
	nfs3_err(0);

	/* create the dir into the directory */
	ret = nfs3_mkdir(&fhAux,
			  file,
			  attr->at_mode,
			  &fh_aux->fh,
			  &fatt,
			  server_aux->cl);

	if(ret < 0){
		nfs3_err(NFS3ERR_MKDIR);
		free(fh_aux);
		return -1;
	}

        fh->priv_fh = (void *)fh_aux;

        fh->url = (char *)malloc(strlen(url)+1);
        if(fh->url == NULL){
               nfs3_err(NFS3ERR_MEMORY);
	       fprintf(stderr,"nfi_nfs3_mkdir: fh->url is null. Can't reserve memory\n");
	       free(fh_aux);
               return -1;
        }
        strcpy(fh->url, url);

	NFS3toNFIattr(attr, &fatt);


	return 0;
}

int nfi_nfs3_rmdir(struct nfi_server *serv,  char *url)
{
	char server[MNTNAMLEN], dir[MNTNAMLEN], file[MNTNAMLEN];
	int ret;
	fhandle3 fhAux;


        struct nfi_nfs3_server *server_aux;

	if (serv == NULL){
		nfs3_err(NFS3ERR_PARAM);
		return -1;
	}


#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_nfs3_reconnect(serv);
		if(ret <0){
			/* nfs3_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               nfs3_err(NFS3ERR_PARAM);
               return -1;
        }
#endif


	server_aux = (struct nfi_nfs3_server *)serv->private_info;

	ret = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir);
	if(ret < 0){
		nfs3_err(NFS3ERR_URL);
		fprintf(stderr,"nfi_nfs3_rmdir: url %s incorrect.\n",url);
		return -1;
	}
	/* elimina del dir la parte del fichero */
	ret = getNameFile(file, dir);
	if(ret < 0){
		nfs3_err(NFS3ERR_URL);
		fprintf(stderr,"nfi_nfs3_rmdir: url %s incorrect.\n",url);
		return -1;
	}

	getDirWithURL(serv->url,dir);
	/* obtain the directory file handle */
	ret = nfs3_lookup(&server_aux->fh,
			dir,
			&fhAux,
			NULL,
			server_aux->cl);

	if(ret < 0){
		nfs3_err(NFS3ERR_LOOKUP);
		fprintf(stderr,"nfi_nfs3_rmdir: Fail lookup %s in server %s (ret:%d).\n",dir,serv->server,ret);
		return -1;
	}

	/* remove the dir into the directory */
	ret = nfs3_rmdir(&fhAux,
			  file, 
			  server_aux->cl);
	if(ret < 0){
		nfs3_err(-ret);
		fprintf(stderr,"nfi_nfs3_rmdir: Fail rmdir %s in server %s (ret:%d).\n",url,serv->server,ret);
		nfs3_err(NFS3ERR_REMOVE);
		return -1;
	}
	nfs3_err(0);

	return 0;
}

int nfi_nfs3_opendir(struct nfi_server *server,  char *url, struct nfi_fhandle *fho)
{
        int ret = nfs3_open(server,  url, fho );
        if (ret <0){
		fprintf(stderr,"nfi_nfs3_opendir: Fail open %s in server %s.\n",url,server->server);
		nfs3_err(-ret);
                return -1;
        }

	if (fho->type != NFIDIR){
		fprintf(stderr,"nfi_nfs3_opendir: Fail %s isn't a DIR.\n",url);
	        nfi_nfs3_close(server, fho);
	        return -1;
	}
        return 0;

}

int nfi_nfs3_readdir(struct nfi_server *serv, struct nfi_fhandle *fh, struct dirent *entry)
{
	int ret;


        struct nfi_nfs3_server *server_aux;
        struct nfi_nfs3_fhandle *fh_aux;


	if (serv == NULL){
		nfs3_err(NFS3ERR_PARAM);
		return -1;
	}


	if (fh == NULL){
		nfs3_err(NFS3ERR_PARAM);
		return -1;
	}

	if (fh->priv_fh == NULL){
		nfs3_err(NFS3ERR_PARAM);
		return -1;
	}

	if(fh->type != NFIDIR){
		nfs3_err(-1);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_nfs3_reconnect(serv);
		if(ret <0){
			/* nfs3_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               nfs3_err(NFS3ERR_PARAM);
               return -1;
        }
#endif

	server_aux = (struct nfi_nfs3_server *)serv->private_info;
	fh_aux = (struct nfi_nfs3_fhandle *)fh->priv_fh;

	if(fh_aux->eofdir){
		entry->d_name[0] = '\0';
		return 0;
	}

	// TODO: update nfs3_readdir with the new header
	ret = nfs3_readdir(&fh_aux->fh, fh_aux->cookie, entry, server_aux->cl);
	if((ret < 0)&&(ret != NFS3ERR_EOFDIR)){
		fprintf(stderr,"nfi_nfs3_readdir: Fail readdir %s in server %s (ret:%d).\n",fh->url,serv->server,ret);
		nfs3_err(NFS3ERR_READDIR);
		return -1;
	}

	if(ret == NFS3ERR_EOFDIR){
		fh_aux->eofdir = 1;
	}

	return 0;
}

int nfi_nfs3_closedir(struct nfi_server *serv,  struct nfi_fhandle *fh)
{
	return nfi_nfs3_close(serv, fh);
}


int nfi_nfs3_statfs(struct nfi_server *serv,  struct nfi_info *inf)
{
	fsinfo3resok nfsinf;
	int ret;
        struct nfi_nfs3_server *server_aux;


	if (serv == NULL){
		nfs3_err(NFS3ERR_PARAM);
		return -1;
	}

	if (inf == NULL){
		nfs3_err(NFS3ERR_PARAM);
		return -1;
	}


#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_nfs3_reconnect(serv);
		if(ret <0){
		/* nfs3_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               nfs3_err(NFS3ERR_PARAM);
               return -1;
        }
#endif

	server_aux = (struct nfi_nfs3_server *)serv->private_info;
	ret = nfs3_statfs(&server_aux->fh, &nfsinf, server_aux->cl);
	if(ret <0){
		nfs3_err(-ret);
		fprintf(stderr,"nfi_nfs3_fstat: Fail fstat in server %s (ret:%d).\n",serv->server,ret);
		nfs3_err(NFS3ERR_STATFS);
		return -1;
	}
	nfs3_err(0);

	NFS3toNFIInfo(inf, &nfsinf);
	return 0;
}
