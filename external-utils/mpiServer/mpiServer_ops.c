
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

	debug_info("[OPS] (ID=%s) mpiServer_create_spacename: %s\n", params->name, path);
	for(i=0; 0 != aux_get_dirs(path, i, dir);i++){
		mkdir(dir, 0777);
	}

	return 0;
}


/*
 * OPERATIONAL FUNCTIONS
 */

/**********************************
Read the operation to realize
***********************************/
int mpiServer_read_operation ( mpiServer_param_st *params, int sd, struct st_mpiServer_msg *head )
{
	int ret;

	debug_info("[OPS] (ID=%s) antes de read_operation: sizeof(struct st_mpiServer_msg) = %d.\n ", params->name, sizeof(struct st_mpiServer_msg));
	ret = mpiServer_comm_readdata(params, sd, (char *)&head->type, sizeof(head->type));
	if(ret == -1){
	        debug_info("[OPS] (ID=%s)  mpiServer_comm_readdata fail\n") ;
		return -1;
	}

	debug_info("[OPS] (ID=%s) read_operation: %d ID=x\n", params->name,head->type);
	switch(head->type){
	case MPISERVER_OPEN_FILE:
		debug_info("[OPS] (ID=%s) OPEN operation from ID=x\n", params->name);
		ret = mpiServer_comm_readdata(params, sd, (char *)&head->u_st_mpiServer_msg.op_open, sizeof(struct st_mpiServer_open));
		if(ret == -1){
			return -1;
		}

		break;
	case MPISERVER_CREAT_FILE:
		debug_info("[OPS] (ID=%s) CREAT operation from ID=x\n", params->name);
		ret = mpiServer_comm_readdata(params, sd, (char *)&head->u_st_mpiServer_msg.op_creat, sizeof(struct st_mpiServer_creat));
		if(ret == -1){
			return -1;
		}

		break;
	case MPISERVER_READ_FILE:
		debug_info("[OPS] (ID=%s) READ operation from ID=x\n", params->name);
		ret = mpiServer_comm_readdata(params, sd, (char *)&head->u_st_mpiServer_msg.op_read, sizeof(struct st_mpiServer_read));
		if(ret == -1){
			return -1;
		}

		break;
	case MPISERVER_WRITE_FILE:
		debug_info("[OPS] (ID=%s) WRITE operation from ID=x\n", params->name);
		ret = mpiServer_comm_readdata(params, sd, (char *)&head->u_st_mpiServer_msg.op_write, sizeof(struct st_mpiServer_write));
		if(ret == -1){
			return -1;
		}

		break;
	case MPISERVER_CLOSE_FILE:
		debug_info("[OPS] (ID=%s) CLOSE operation from ID=x\n", params->name);
		ret = mpiServer_comm_readdata(params, sd, (char *)&head->u_st_mpiServer_msg.op_close, sizeof(struct st_mpiServer_close));
		if(ret == -1){
			return -1;
		}

		break;
	case MPISERVER_RM_FILE:
		debug_info("[OPS] (ID=%s) RM operation from ID=x\n", params->name);
		ret = mpiServer_comm_readdata(params, sd, (char *)&head->u_st_mpiServer_msg.op_rm, sizeof(struct st_mpiServer_rm));
		if(ret == -1){
			return -1;
		}

		break;
	case MPISERVER_GETATTR_FILE:
		debug_info("[OPS] (ID=%s) GETATTR operation from ID=x\n", params->name);
		ret = mpiServer_comm_readdata(params, sd, (char *)&head->u_st_mpiServer_msg.op_getattr, sizeof(struct st_mpiServer_getattr));
		if(ret == -1){
			return -1;
		}

		break;
	case MPISERVER_SETATTR_FILE:
		debug_info("[OPS] (ID=%s) SETATTR operation from ID=x\n", params->name);
		ret = mpiServer_comm_readdata(params, sd, (char *)&head->u_st_mpiServer_msg.op_setattr, sizeof(struct st_mpiServer_setattr));
		if(ret == -1){
			return -1;
		}

		break;
	case MPISERVER_MKDIR_DIR:
		debug_info("[OPS] (ID=%s) MKDIR operation from ID=x\n", params->name);
		ret = mpiServer_comm_readdata(params, sd, (char *)&head->u_st_mpiServer_msg.op_mkdir, sizeof(struct st_mpiServer_mkdir));
		if(ret == -1){
			return -1;
		}

		break;
	case MPISERVER_RMDIR_DIR:
		debug_info("[OPS] (ID=%s) RMDIR operation from ID=x\n", params->name);
		ret = mpiServer_comm_readdata(params, sd, (char *)&head->u_st_mpiServer_msg.op_rmdir, sizeof(struct st_mpiServer_rmdir));
		if(ret == -1){
			return -1;
		}

		break;
	case MPISERVER_FLUSH_FILE:
		debug_info("[OPS] (ID=%s) FLUSH operation from ID=x\n", params->name);
		ret = mpiServer_comm_readdata(params, sd, (char *)&head->u_st_mpiServer_msg.op_flush, sizeof(struct st_mpiServer_flush));
		if(ret == -1){
			return -1;
		}

		break;
	case MPISERVER_PRELOAD_FILE:
		debug_info("[OPS] (ID=%s) PRELOAD operation from ID=x\n", params->name);
		ret = mpiServer_comm_readdata(params, sd, (char *)&head->u_st_mpiServer_msg.op_preload, sizeof(struct st_mpiServer_preload));
		if(ret == -1){
			return -1;
		}

		break;
        case MPISERVER_GETID:
                debug_info("[OPS] (ID=%s) GETID operation from ID=x\n", params->name);
/*
		ret = mpiServer_comm_readdata(params, sd, (char *)&head->id, sizeof(MPISERVER_ID));
		if(ret == -1){
			return -1;
		}
*/
                break;
	case MPISERVER_FINALIZE:
		debug_info("[OPS] (ID=%s) FINALIZE operation from ID=x\n", params->name);
/*
		ret = mpiServer_comm_readdata(params, sd, (char *)&head->u_st_mpiServer_msg.op_end, sizeof(struct st_mpiServer_end));
		if(ret == -1){
			return -1;
		}
*/
		break;
	case MPISERVER_END:
		debug_info("[OPS] (ID=%s) END operation from ID=x\n", params->name);
/*
		ret = mpiServer_comm_readdata(params, sd, (char *)&head->u_st_mpiServer_msg.op_end, sizeof(struct st_mpiServer_end));
		if(ret == -1){
			return -1;
		}
*/
		break;
	}
	return head->type;
}


void mpiServer_op_open ( mpiServer_param_st *params, int sd, struct st_mpiServer_msg *head )
{
	int fd;
	char s[255];

	//sprintf(s,"%s", head->u_st_mpiServer_msg.op_open.path);
	strcpy(s,head->u_st_mpiServer_msg.op_open.path);

	debug_info("[OPS]%s> begin open(%s) ID=x -> %d\n", params->name,s,fd);

	fd = open(s, O_RDWR);
	//if(fd == -1){
	//	mpiServer_create_spacename(params, s);
	//}
	debug_info("[OPS]%s> end open(%s) ID=x ->%d\n", params->name,s,fd);
	mpiServer_comm_writedata(params, sd, (char *)&fd, sizeof(int));
	debug_info("[OPS] (ID=%s) OPEN operation from ID=x\n", params->name);
}

void mpiServer_op_creat ( mpiServer_param_st *params, int sd, struct st_mpiServer_msg *head )
{
	int fd;
	char s[255];

	// check params...
	if (NULL == params) {
	    return;
	}

	strcpy(s,head->u_st_mpiServer_msg.op_creat.path);

	debug_info("[OPS] (ID=%s) begin creat(%s) ID=x ->\n", params->name,s);
      //fd = open(s, O_CREAT | O_TRUNC | O_RDWR, 0666);
	fd = open(s, O_CREAT | O_RDWR, 0777);
	if (fd == -1)
	{
	    mpiServer_create_spacename(params, s);
            //fd = open(s, O_CREAT | O_TRUNC | O_RDWR, 0666);
            fd = open(s, O_CREAT | O_RDWR, 0666);
	}

	mpiServer_comm_writedata(params, sd,(char *)&fd,sizeof(int)) ;
	debug_info("[OPS] (ID=%s) end CREAT operation from ID=x ->%d\n", params->name,fd);
}

void mpiServer_op_flush ( mpiServer_param_st *params, int sd, struct st_mpiServer_msg *head )
{
	int ret = 0;

	// check params...
	if (NULL == params) {
	    return ;
	}
	if (sd < 0) {
	    return ;
	}
	if (NULL == head) {
	    return ;
	}

	debug_info("[OPS] (ID=%s) begin FLUSH(%s) ID=x ->\n", params->name, head->u_st_mpiServer_msg.op_flush.virtual_path);

	mpiServer_comm_writedata(params, sd, (char *)&ret, sizeof(int));

	debug_info("[OPS] (ID=%s) end FLUSH operation from ID=x\n", params->name);
}

void mpiServer_op_preload ( mpiServer_param_st *params, int sd, struct st_mpiServer_msg *head )
{
	int ret;

	debug_info("[OPS] (ID=%s) begin preload(%s,%s) ID=x\n", params->name,
							  head->u_st_mpiServer_msg.op_preload.virtual_path,
							  head->u_st_mpiServer_msg.op_preload.storage_path);
	ret = 0;

	ret = mpiServer_d2xpn(params,
			      head->u_st_mpiServer_msg.op_preload.virtual_path,
                              head->u_st_mpiServer_msg.op_preload.storage_path) ;


	debug_info("[OPS] (ID=%s) end preload(%s,%s) ID=x -> %d\n", params->name,
							      head->u_st_mpiServer_msg.op_preload.virtual_path,
							      head->u_st_mpiServer_msg.op_preload.storage_path,
							      ret);

	mpiServer_comm_writedata(params, sd, (char *)&ret, sizeof(int));

	debug_info("[OPS] (ID=%s) end PRELOAD operation from ID=x\n", params->name);
}

void mpiServer_op_close ( mpiServer_param_st *params, int sd, struct st_mpiServer_msg *head )
{
	// check params...
	if (NULL == params) {
	    return ;
	}
	if (sd < 0) {
	    return ;
	}

	debug_info("[OPS] (ID=%s) begin close: fd %d ID=x\n", params->name, head->u_st_mpiServer_msg.op_close.fd);

	close(head->u_st_mpiServer_msg.op_close.fd);

	debug_info("[OPS] (ID=%s) end close: fd %d ID=x\n", params->name, head->u_st_mpiServer_msg.op_close.fd);
}

void mpiServer_op_rm ( mpiServer_param_st *params, int sd, struct st_mpiServer_msg *head )
{
	char s[255];

	// check params...
	if (NULL == params) {
	    return ;
	}
	if (sd < 0) {
	    return ;
	}

	debug_info("[OPS] (ID=%s) begin RM: path %s ID=x\n", params->name, head->u_st_mpiServer_msg.op_rm.path);

	strcpy(s, head->u_st_mpiServer_msg.op_rm.path);
	unlink(s);

	debug_info("[OPS] (ID=%s) end RM: path %s ID=x\n", params->name, head->u_st_mpiServer_msg.op_rm.path);
}

void mpiServer_op_read ( mpiServer_param_st *params, int sd, struct st_mpiServer_msg *head )
{
	long size_req, size = 0, cont = 0;
	//char *buffer;
	struct st_mpiServer_read_req req;
	int SIZE;

#ifndef _MALLOC_
	char buffer[MAX_BUFFER_SIZE];
	SIZE = MAX_BUFFER_SIZE;
	debug_info("[OPS] (ID=%s) op_read: static buffer (%d) ID=x\n", params->name,MAX_BUFFER_SIZE);
#else
	char *buffer;
	SIZE = 0;
	debug_info("[OPS] (ID=%s) op_read: variable buffer (%d) ID=x\n", params->name,MAX_BUFFER_SIZE);
#endif

	//
	debug_info("[OPS] (ID=%s) begin read: fd %d offset %d size %d ID=x\n", params->name,
						head->u_st_mpiServer_msg.op_read.fd,
						(int)head->u_st_mpiServer_msg.op_read.offset,
						head->u_st_mpiServer_msg.op_read.size);

#ifdef _MALLOC_
	SIZE = head->u_st_mpiServer_msg.op_read.size;
	buffer = (char *)malloc(SIZE);
	//buffer = (char *)malloc(MAX_BUFFER_SIZE);
	//debug_info("[OPS] (ID=%s) op_read: malloc(%d) ID=x\n", params->name,MAX_BUFFER_SIZE);
	debug_info("[OPS] (ID=%s) op_read: malloc(%d) ID=x\n", params->name,SIZE);
#endif
	//t1 = MPI_Wtime();
	do{
#ifdef _LARGEFILE64_
		lseek64(head->u_st_mpiServer_msg.op_read.fd,
				head->u_st_mpiServer_msg.op_read.offset+cont,
				0);
#else
	        debug_info("[OPS] (ID=%s) lseek: fd %d offset %d size %d ID=x\n", params->name,
						head->u_st_mpiServer_msg.op_read.fd,
						(int)head->u_st_mpiServer_msg.op_read.offset+cont,
						head->u_st_mpiServer_msg.op_read.size);

		lseek(head->u_st_mpiServer_msg.op_read.fd,
			head->u_st_mpiServer_msg.op_read.offset+cont,
			0);
#endif

		size_req = (head->u_st_mpiServer_msg.op_read.size - cont);
		if(size_req>SIZE){
			size_req = SIZE;
		}

		req.size = read(head->u_st_mpiServer_msg.op_read.fd,
		 			buffer,
       		   			size_req);

		if(req.size < 0){
			perror("read:");
		}
		cont += req.size;
		if (((unsigned)cont == head->u_st_mpiServer_msg.op_read.size) || (req.size < size_req)) {
			req.last = 1;
		} else {
			req.last = 0;
		}
		mpiServer_comm_writedata(params, sd, (char *)&req, sizeof(struct st_mpiServer_read_req));
		debug_info("[OPS] (ID=%s) op_read: send size %d\n", params->name, req.size);

		if(req.size > 0){
			mpiServer_comm_writedata(params, sd, (char *)buffer, req.size);
			debug_info("[OPS] (ID=%s) op_read: send data\n", params->name);
		}else{
			break;
		}
	}while((size >0)&&(!req.last));

        debug_info("[OPS] (ID=%s) end read: fd %d offset %d size %d ID=x\n", params->name,
                                                                       head->u_st_mpiServer_msg.op_read.fd,
                                                                       (int)head->u_st_mpiServer_msg.op_read.offset,
                                                                       size);

	debug_info("[OPS] (ID=%s) op_read: new_size %d\n", params->name,size);
#ifdef _MALLOC_
	debug_info("[OPS] (ID=%s) op_read: free ID=x\n", params->name);
	free(buffer);
#endif
        debug_info("[OPS] (ID=%s) end READ: fd %d offset %d size %d ID=x\n", params->name,
                                                head->u_st_mpiServer_msg.op_read.fd,
                                                (int)head->u_st_mpiServer_msg.op_read.offset,
                                                size);
}

void mpiServer_op_write ( mpiServer_param_st *params, int sd, struct st_mpiServer_msg *head )
{
	//char *buffer;
	int cont =0 ,size =0;
	struct st_mpiServer_write_req req;
#ifndef _MALLOC_
	char buffer[MAX_BUFFER_SIZE];
	int SIZE = MAX_BUFFER_SIZE;
#else
	char *buffer;
	int SIZE = 0;
#endif

	debug_info("[OPS] (ID=%s) begin write: fd %d ID=xn", params->name, head->u_st_mpiServer_msg.op_write.fd);

#ifdef _MALLOC_
	SIZE = head->u_st_mpiServer_msg.op_write.size;
#endif
	do{
	size = (head->u_st_mpiServer_msg.op_write.size - cont);
	if(size>SIZE){
		size= SIZE;
	}

	if(size == 0)
		break;

#ifdef _MALLOC_
	buffer = (char *)malloc(SIZE);
	//buffer = (char *)malloc(head->u_st_mpiServer_msg.op_read.size);
	//buffer = (char *)malloc(MAX_BUFFER_SIZE);
#endif
	mpiServer_comm_readdata(params, sd,(char *)buffer, size);

#ifdef _LARGEFILE64_
	lseek64(head->u_st_mpiServer_msg.op_write.fd,
			head->u_st_mpiServer_msg.op_write.offset+cont,
			0);
#else
	lseek(head->u_st_mpiServer_msg.op_write.fd,
			head->u_st_mpiServer_msg.op_write.offset+cont,
			0);
#endif

	req.size = write(head->u_st_mpiServer_msg.op_write.fd,
			  buffer,
		          size);

	cont += size;
	}while(req.size>0);

	if(req.size>=0){
		req.size = head->u_st_mpiServer_msg.op_write.size;
	}
	mpiServer_comm_writedata(params, sd,(char *)&req,sizeof(struct st_mpiServer_write_req));

	#ifdef _MALLOC_
		free(buffer);
	#endif

	debug_info("[OPS] (ID=%s) op_write on fd %d with %d bytes", params->name, head->u_st_mpiServer_msg.op_write.fd, req.size);

	debug_info("[OPS] (ID=%s) end write: fd %d ID=xn", params->name, head->u_st_mpiServer_msg.op_write.fd);
}

void mpiServer_op_mkdir ( mpiServer_param_st *params, int sd, struct st_mpiServer_msg *head )
{
	char s[255];
	int ret;

	debug_info("[OPS] (ID=%s) begin MKDIR operation from ID=x\n", params->name);

	strcpy(s, head->u_st_mpiServer_msg.op_mkdir.path);
	ret = mkdir(s, 0777);
	mpiServer_comm_writedata(params, sd,(char *)&ret,sizeof(int));

	debug_info("[OPS] (ID=%s) end MKDIR operation from ID=x\n", params->name);
}

void mpiServer_op_rmdir ( mpiServer_param_st *params, int sd, struct st_mpiServer_msg *head )
{
	char s[255];
	int ret;

	debug_info("[OPS] (ID=%s) begin RMDIR operation from ID=x\n", params->name);

	strcpy(s, head->u_st_mpiServer_msg.op_rmdir.path);
	ret = rmdir(s);
	mpiServer_comm_writedata(params, sd, (char *)&ret, sizeof(int));

	debug_info("[OPS] (ID=%s) end RMDIR operation from ID=x\n", params->name);
}

void mpiServer_op_setattr ( mpiServer_param_st *params, int sd, struct st_mpiServer_msg *head )
{
	debug_info("[OPS] (ID=%s) begin SETATTR operation from ID=x\n", params->name);

	// check params...
	if (NULL == params) {
	    return ;
	}
	if (sd < 0) {
	    return ;
	}
	if (NULL == head) {
	    return ;
	}

	// do request...
	debug_info("[OPS] SETATTR operation to be implemented !!\n");

	debug_info("[OPS] (ID=%s) end   SETATTR operation from ID=x\n", params->name);
}

void mpiServer_op_getattr ( mpiServer_param_st *params, int sd, struct st_mpiServer_msg *head )
{
	char s[255];
	struct st_mpiServer_attr_req req;

	debug_info("[OPS] (ID=%s) begin GETATTR(%s) ID=xn", params->name, head->u_st_mpiServer_msg.op_getattr.path);

	strcpy(s, head->u_st_mpiServer_msg.op_getattr.path);
	req.status = stat(s, &req.attr);
	mpiServer_comm_writedata(params, sd,(char *)&req,sizeof(struct st_mpiServer_attr_req));

	debug_info("[OPS] (ID=%s) end GETATTR(%s) ID=xn", params->name, head->u_st_mpiServer_msg.op_getattr.path);
}

void mpiServer_op_getid ( mpiServer_param_st *params, int sd, struct st_mpiServer_msg *head )
{
        debug_info("[OPS] (ID=%s) begin GETID ID=x\n", params->name);

        mpiServer_comm_writedata(params, sd,(char *)head->id, MPISERVER_ID);

        debug_info("[OPS] (ID=%s) end GETID operation from ID=x\n", params->name);
}

   /* ................................................................... */

