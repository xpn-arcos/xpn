
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

//#include "tcpServer.h"
#include "tcpServer_ops.h"
#include "tcpServer_comm.h"
#include "tcpServer_d2xpn.h"


   /* ... Global variables / Variables globales ......................... */

//char global_buffer[MAX_BUFFER_SIZE];


   /* ... Functions / Funciones ......................................... */

int aux_clear_dirs(char *path)
{
	unsigned long i=0;
	int j;
	char ant = '\0', s[255];

	j=0;
	for (i=0;i < strlen(path); i++)
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
	unsigned long i=0;
	int j=0, ant=-1, pos=-1;
	int cont=-1;
	char new_path[MAXPATHLEN];

	strcpy(new_path, path);

	path = new_path;
	aux_clear_dirs(path);
	s[0] = '\0';
	if (path == NULL) {
		return 0;
	}

	for(i=0; i<strlen(path) && cont<(n+1); i++){
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

int tcpServer_create_spacename(char *path)
{
	char dir[MAXPATHLEN];
	int i;

	debug_info("[OPS] (%s) tcpServer_create_spacename: %s\n", TCPSERVER_ALIAS_NAME_STRING, path);

	for(i=0; 0 != aux_get_dirs(path, i, dir);i++){
		mkdir(dir, 0777);
	}
	return 0;
}

/* OPERATIONAL FUNCTIONS */

/**********************************
Read the operation to realize
***********************************/
int tcpServer_read_operation(int sd, struct st_tcpServer_msg *head)
{
	int ret;

	debug_info("[OPS] (%s) antes de read_operation: sizeof(struct st_tcpServer_msg) = %d.\n ", TCPSERVER_ALIAS_NAME_STRING, sizeof(struct st_tcpServer_msg));

	ret = tcpServer_comm_readdata(sd, (char *)&head->type, sizeof(head->type), head->id);
	if (ret == -1) {
	    return -1;
	}

	debug_info("[OPS] (%s) read_operation: %d ID=%s\n", TCPSERVER_ALIAS_NAME_STRING,head->type,head->id);

	switch(head->type)
	{
		case TCPSERVER_OPEN_FILE:
			debug_info("[OPS] (%s) OPEN operation from ID=%s\n",TCPSERVER_ALIAS_NAME_STRING,head->id);
			ret = tcpServer_comm_readdata(sd, (char *)&head->u_st_tcpServer_msg.op_open, sizeof(struct st_tcpServer_open), head->id);
			break;
		case TCPSERVER_CREAT_FILE:
			debug_info("[OPS] (%s) CREAT operation from ID=%s\n",TCPSERVER_ALIAS_NAME_STRING,head->id);
			ret = tcpServer_comm_readdata(sd, (char *)&head->u_st_tcpServer_msg.op_creat, sizeof(struct st_tcpServer_creat), head->id);
			break;
		case TCPSERVER_READ_FILE:
			debug_info("[OPS] (%s) READ operation from ID=%s\n",TCPSERVER_ALIAS_NAME_STRING,head->id);
			ret = tcpServer_comm_readdata(sd, (char *)&head->u_st_tcpServer_msg.op_read, sizeof(struct st_tcpServer_read), head->id);
			break;
		case TCPSERVER_WRITE_FILE:
			debug_info("[OPS] (%s) WRITE operation from ID=%s\n",TCPSERVER_ALIAS_NAME_STRING,head->id);
			ret = tcpServer_comm_readdata(sd, (char *)&head->u_st_tcpServer_msg.op_write, sizeof(struct st_tcpServer_write), head->id);
			break;
		case TCPSERVER_CLOSE_FILE:
			debug_info("[OPS] (%s) CLOSE operation from ID=%s\n",TCPSERVER_ALIAS_NAME_STRING,head->id);
			ret = tcpServer_comm_readdata(sd, (char *)&head->u_st_tcpServer_msg.op_close, sizeof(struct st_tcpServer_close), head->id);
			break;
		case TCPSERVER_RM_FILE:
			debug_info("[OPS] (%s) RM operation from ID=%s\n",TCPSERVER_ALIAS_NAME_STRING,head->id);
			ret = tcpServer_comm_readdata(sd, (char *)&head->u_st_tcpServer_msg.op_rm, sizeof(struct st_tcpServer_rm), head->id);
			break;
		case TCPSERVER_GETATTR_FILE:
			debug_info("[OPS] (%s) GETATTR operation from ID=%s\n",TCPSERVER_ALIAS_NAME_STRING,head->id);
			ret = tcpServer_comm_readdata(sd, (char *)&head->u_st_tcpServer_msg.op_getattr, sizeof(struct st_tcpServer_getattr), head->id);
			break;
		case TCPSERVER_SETATTR_FILE:
			debug_info("[OPS] (%s) SETATTR operation from ID=%s\n",TCPSERVER_ALIAS_NAME_STRING,head->id);
			ret = tcpServer_comm_readdata(sd, (char *)&head->u_st_tcpServer_msg.op_setattr, sizeof(struct st_tcpServer_setattr), head->id);
			break;
		case TCPSERVER_MKDIR_DIR:
			debug_info("[OPS] (%s) MKDIR operation from ID=%s\n",TCPSERVER_ALIAS_NAME_STRING,head->id);
			ret = tcpServer_comm_readdata(sd, (char *)&head->u_st_tcpServer_msg.op_mkdir, sizeof(struct st_tcpServer_mkdir), head->id);
			break;
		case TCPSERVER_RMDIR_DIR:
			debug_info("[OPS] (%s) RMDIR operation from ID=%s\n",TCPSERVER_ALIAS_NAME_STRING,head->id);
			ret = tcpServer_comm_readdata(sd, (char *)&head->u_st_tcpServer_msg.op_rmdir, sizeof(struct st_tcpServer_rmdir), head->id);
			break;
		case TCPSERVER_FLUSH_FILE:
			debug_info("[OPS] (%s) FLUSH operation from ID=%s\n",TCPSERVER_ALIAS_NAME_STRING,head->id);
			ret = tcpServer_comm_readdata(sd, (char *)&head->u_st_tcpServer_msg.op_flush, sizeof(struct st_tcpServer_flush), head->id);
			break;
		case TCPSERVER_PRELOAD_FILE:
			debug_info("[OPS] (%s) PRELOAD operation from ID=%s\n",TCPSERVER_ALIAS_NAME_STRING,head->id);
			ret = tcpServer_comm_readdata(sd, (char *)&head->u_st_tcpServer_msg.op_preload, sizeof(struct st_tcpServer_preload), head->id);
			break;
		case TCPSERVER_GETID:
			debug_info("[OPS] (%s) GETID operation from ID=%s\n",TCPSERVER_ALIAS_NAME_STRING,head->id);
			/* ret = tcpServer_comm_readdata(sd, (char *)&head->id, sizeof(TCPSERVER_ID), head->id); */
			break;
		case TCPSERVER_FINALIZE:
			debug_info("[OPS] (%s) FINALIZE operation from ID=%s\n",TCPSERVER_ALIAS_NAME_STRING,head->id);
			/* ret = tcpServer_comm_readdata(sd, (char *)&head->u_st_tcpServer_msg.op_end, sizeof(struct st_tcpServer_end), head->id); */
			break;
		case TCPSERVER_END:
			debug_info("[OPS] (%s) END operation from ID=%s\n",TCPSERVER_ALIAS_NAME_STRING,head->id);
			/* ret = tcpServer_comm_readdata(sd, (char *)&head->u_st_tcpServer_msg.op_end, sizeof(struct st_tcpServer_end), head->id); */
			break;
	}

	// return...
	if (ret == -1) {
	    return -1;
	}
	return head->type;
}

void tcpServer_op_open(int sd, struct st_tcpServer_msg *head)
{
	int fd;
	char s[255];

	//sprintf(s,"%s", head->u_st_tcpServer_msg.op_open.path);
	strcpy(s,head->u_st_tcpServer_msg.op_open.path);

	debug_info("[OPS]%s> begin open(%s) ID=%s -> %d\n",TCPSERVER_ALIAS_NAME_STRING,s,head->id,fd);

	fd = open(s, O_RDWR);
	//if(fd == -1){
	//	tcpServer_create_spacename(s);
	//}
	debug_info("[OPS]%s> end open(%s) ID=%s ->%d\n",TCPSERVER_ALIAS_NAME_STRING,s, head->id, fd);
	tcpServer_comm_writedata(sd, (char *)&fd, sizeof(int), head->id);
	debug_info("[OPS] (%s) OPEN operation from ID=%s\n",TCPSERVER_ALIAS_NAME_STRING,head->id);
}

void tcpServer_op_creat(int sd, struct st_tcpServer_msg *head)
{
	int fd;
	char s[255];

	debug_info("[OPS] (%s) begin creat(%s) ID=%s ->\n", TCPSERVER_ALIAS_NAME_STRING, head->u_st_tcpServer_msg.op_creat.path, head->id);

	strcpy(s,head->u_st_tcpServer_msg.op_creat.path);
	//fd = open(s, O_CREAT | O_TRUNC | O_RDWR, 0666);
	fd = open(s, O_CREAT | O_RDWR, 0777);
	if (fd == -1){
		tcpServer_create_spacename(s);
	        //fd = open(s, O_CREAT | O_TRUNC | O_RDWR, 0666);
	        fd = open(s, O_CREAT | O_RDWR, 0666);
	}

	tcpServer_comm_writedata(sd,(char *)&fd,sizeof(int), head->id);

	debug_info("[OPS] (%s) end CREAT operation from ID=%s ->%d\n",TCPSERVER_ALIAS_NAME_STRING,head->id,fd);
}

void tcpServer_op_flush(int sd, struct st_tcpServer_msg *head)
{
	int ret = 0;
	//char s_exe[1024];

	debug_info("[OPS] (%s) begin flush(%s) ID=%s ->\n",TCPSERVER_ALIAS_NAME_STRING, head->u_st_tcpServer_msg.op_flush.virtual_path, head->id);

	//sprintf(s,"%s", head->u_st_tcpServer_msg.op_creat.path);
	//strcpy(s,head->u_st_tcpServer_msg.op_flush.path);
/*
	sprintf(s_exe,"%s %s %s\n", TCPSERVER_FLUSH_EXE,
				head->u_st_tcpServer_msg.op_flush.virtual_path,
				head->u_st_tcpServer_msg.op_flush.storage_path);
	debug_info("[OPS]begin system->%s\n",s_exe);
	ret = system(s_exe);
	debug_info("[OPS]end system->%s\n",s_exe);

	debug_info("[OPS] (%s) open flush(%s) ID=%s -> %d\n",TCPSERVER_ALIAS_NAME_STRING,
							head->u_st_tcpServer_msg.op_flush.virtual_path,
							sd, head->id,ret);
*/

	tcpServer_comm_writedata(sd, (char *)&ret, sizeof(int), head->id);

	debug_info("[OPS] (%s) end FLUSH operation from ID=%s\n",TCPSERVER_ALIAS_NAME_STRING,head->id);
}

void tcpServer_op_preload(int sd, struct st_tcpServer_msg *head)
{
	int ret;

	debug_info("[OPS] (%s) begin preload(%s,%s) ID=%s\n",TCPSERVER_ALIAS_NAME_STRING,
							head->u_st_tcpServer_msg.op_preload.virtual_path,
							head->u_st_tcpServer_msg.op_preload.storage_path,
							 head->id);

	ret = tcpServer_d2xpn(head->u_st_tcpServer_msg.op_preload.virtual_path,
                             head->u_st_tcpServer_msg.op_preload.storage_path,
			     head->u_st_tcpServer_msg.op_preload.opt);

	debug_info("[OPS] (%s) end preload(%s,%s) ID=%s -> %d\n",TCPSERVER_ALIAS_NAME_STRING,
							head->u_st_tcpServer_msg.op_preload.virtual_path,
							head->u_st_tcpServer_msg.op_preload.storage_path,
							head->id, ret);

	tcpServer_comm_writedata(sd, (char *)&ret, sizeof(int),  head->id);

	debug_info("[OPS] (%s) end PRELOAD operation from ID=%s\n", TCPSERVER_ALIAS_NAME_STRING,head->id);
}

void tcpServer_op_close(int sd, struct st_tcpServer_msg *head)
{
	debug_info("[OPS] (%s) begin close: fd %d ID=%s\n",TCPSERVER_ALIAS_NAME_STRING,
							head->u_st_tcpServer_msg.op_close.fd,
							head->id);

	// check params...
	if (sd < 0) {
	    printf("[OPS] (%s)  CLOSE operation with sd < 0\n",TCPSERVER_ALIAS_NAME_STRING);
	}

	close(head->u_st_tcpServer_msg.op_close.fd);

	debug_info("[OPS] (%s) end CLOSE operation from ID=%s\n",TCPSERVER_ALIAS_NAME_STRING,head->id);
}

void tcpServer_op_rm ( int sd, struct st_tcpServer_msg *head )
{
	char s[255];

	debug_info("[OPS] (%s) begin unlink: path %s ID=%s\n",TCPSERVER_ALIAS_NAME_STRING,
						head->u_st_tcpServer_msg.op_rm.path,
						head->id);

	// check params...
	if (sd < 0) {
	    printf("[OPS] (%s)  RM operation with sd < 0\n",TCPSERVER_ALIAS_NAME_STRING);
	}

	strcpy(s, head->u_st_tcpServer_msg.op_rm.path);
	unlink(s);

	debug_info("[OPS] (%s) end RM: path %s ID=%s\n",TCPSERVER_ALIAS_NAME_STRING,
						head->u_st_tcpServer_msg.op_rm.path,
						head->id);
}

void tcpServer_op_read(int sd, struct st_tcpServer_msg *head)
{
	unsigned long cont = 0;
	int size_req, size = 0;
	//char *buffer;
	struct st_tcpServer_read_req req;
	int SIZE;

#ifndef _MALLOC_
	char buffer[MAX_BUFFER_SIZE];
	SIZE = MAX_BUFFER_SIZE;
	debug_info("[OPS] (%s) op_read: static buffer (%d) ID=%s\n",TCPSERVER_ALIAS_NAME_STRING,MAX_BUFFER_SIZE,head->id);
#else
	char *buffer;
	SIZE = 0;
	debug_info("[OPS] (%s) op_read: variable buffer (%d) ID=%s\n",TCPSERVER_ALIAS_NAME_STRING,MAX_BUFFER_SIZE,head->id);
#endif

	//
	debug_info("[OPS] (%s) begin read: fd %d offset %d size %d ID=%s\n",TCPSERVER_ALIAS_NAME_STRING,
						head->u_st_tcpServer_msg.op_read.fd,
						(int)head->u_st_tcpServer_msg.op_read.offset,
						head->u_st_tcpServer_msg.op_read.size,
						head->id);

/*
#ifdef _LARGEFILE64_
	printf("[OPS] (%s) op_read: offset %lld\n",TCPSERVER_ALIAS_NAME_STRING,head->u_st_tcpServer_msg.op_read.offset);
#else
	printf("[OPS] (%s) op_read: offset %d\n",TCPSERVER_ALIAS_NAME_STRING,(int)head->u_st_tcpServer_msg.op_read.offset);
#endif
	printf("[OPS] (%s) op_read: size %d\n",TCPSERVER_ALIAS_NAME_STRING,head->u_st_tcpServer_msg.op_read.size);
#endif
*/
#ifdef _MALLOC_
	SIZE = head->u_st_tcpServer_msg.op_read.size;
	buffer = (char *)malloc(SIZE);
	//buffer = (char *)malloc(MAX_BUFFER_SIZE);
	//debug_info("[OPS] (%s) op_read: malloc(%d) ID=%s\n",TCPSERVER_ALIAS_NAME_STRING,MAX_BUFFER_SIZE,head->id);
	debug_info("[OPS] (%s) op_read: malloc(%d) ID=%s\n",TCPSERVER_ALIAS_NAME_STRING,SIZE,head->id);
#endif
	//t1 = MPI_Wtime();
	do{
#ifdef _LARGEFILE64_
		lseek64(head->u_st_tcpServer_msg.op_read.fd,
				head->u_st_tcpServer_msg.op_read.offset+cont,
				0);
#else
	        debug_info("[OPS] (%s) lseek: fd %d offset %d size %d ID=%s\n",TCPSERVER_ALIAS_NAME_STRING,
						head->u_st_tcpServer_msg.op_read.fd,
						(int)head->u_st_tcpServer_msg.op_read.offset+cont,
						head->u_st_tcpServer_msg.op_read.size,
						head->id);

		lseek(head->u_st_tcpServer_msg.op_read.fd,
				head->u_st_tcpServer_msg.op_read.offset+cont,
				0);
#endif

		size_req = (head->u_st_tcpServer_msg.op_read.size - cont);
		if(size_req>SIZE){
			size_req = SIZE;
		}

		req.size = read(head->u_st_tcpServer_msg.op_read.fd,
		 			buffer,
       		   			size_req);

		//usleep(rand()%1000);
		//req.size = size_req;

		if(req.size < 0){
			perror("read:");
		}
		cont += req.size;
		if((cont == head->u_st_tcpServer_msg.op_read.size) ||
			(req.size < size_req)){
			req.last = 1;
		}else{
			req.last = 0;
		}
		tcpServer_comm_writedata(sd, (char *)&req, sizeof(struct st_tcpServer_read_req), head->id);
		debug_info("[OPS] (%s) op_read: send size %d\n",TCPSERVER_ALIAS_NAME_STRING, req.size);

		if(req.size > 0){
			tcpServer_comm_writedata(sd, (char *)buffer, req.size, head->id);
			debug_info("[OPS] (%s) op_read: send data\n",TCPSERVER_ALIAS_NAME_STRING);
		}else{
			break;
		}
	} while ((size >0)&&(!req.last)) ;

        debug_info("[OPS] (%s) end read: fd %d offset %d size %d ID=%s\n",TCPSERVER_ALIAS_NAME_STRING,
                                                head->u_st_tcpServer_msg.op_read.fd,
                                                (int)head->u_st_tcpServer_msg.op_read.offset,
                                                size,
                                                head->id);

#ifdef _MALLOC_
	debug_info("[OPS] (%s) op_read: free ID=%s\n",TCPSERVER_ALIAS_NAME_STRING,head->id);
	free(buffer);
#endif
        debug_info("[OPS] (%s) end READ: fd %d offset %d size %d ID=%s\n",TCPSERVER_ALIAS_NAME_STRING,
                                                head->u_st_tcpServer_msg.op_read.fd,
                                                (int)head->u_st_tcpServer_msg.op_read.offset,
                                                size,
                                                head->id);
}

void tcpServer_op_write(int sd, struct st_tcpServer_msg *head)
{
	//char *buffer;
	int cont =0 ,size =0;
	struct st_tcpServer_write_req req;
#ifndef _MALLOC_
	char buffer[MAX_BUFFER_SIZE];
	int SIZE = MAX_BUFFER_SIZE;
#else
	char *buffer;
	int SIZE = 0;
#endif

	debug_info("[OPS] (%s) begin write: fd %d ID=%sn",TCPSERVER_ALIAS_NAME_STRING,
						head->u_st_tcpServer_msg.op_write.fd,
						head->id);
/*
#ifdef DBG_OPS
	debug_info("[OPS] (%s) op_write: fd %d\n",TCPSERVER_ALIAS_NAME_STRING,head->u_st_tcpServer_msg.op_write.fd);
#ifdef _LARGEFILE64_
	debug_info("[OPS] (%s) op_write: offset %lld\n",TCPSERVER_ALIAS_NAME_STRING,head->u_st_tcpServer_msg.op_write.offset);
#else
	debug_info("[OPS] (%s) op_write: offset %d\n",TCPSERVER_ALIAS_NAME_STRING,(int)head->u_st_tcpServer_msg.op_write.offset);
#endif
	debug_info("[OPS] (%s) op_write: size %d\n",TCPSERVER_ALIAS_NAME_STRING,head->u_st_tcpServer_msg.op_write.size);
#endif
*/
	//t1 = MPI_Wtime();

#ifdef _MALLOC_
	SIZE = head->u_st_tcpServer_msg.op_write.size;
#endif
	do
	{
		size = (head->u_st_tcpServer_msg.op_write.size - cont);
		if(size>SIZE){
			size= SIZE;
		}

		if(size == 0)
			break;

#ifdef _MALLOC_
		buffer = (char *)malloc(SIZE);
		//buffer = (char *)malloc(head->u_st_tcpServer_msg.op_read.size);
		//buffer = (char *)malloc(MAX_BUFFER_SIZE);
#endif
		tcpServer_comm_readdata(sd,(char *)buffer, size, head->id);

#ifdef _LARGEFILE64_
		lseek64(head->u_st_tcpServer_msg.op_write.fd,
				head->u_st_tcpServer_msg.op_write.offset+cont,
				0);
#else
		lseek(head->u_st_tcpServer_msg.op_write.fd,
				head->u_st_tcpServer_msg.op_write.offset+cont,
				0);
#endif

		req.size = write(head->u_st_tcpServer_msg.op_write.fd,
				  buffer,
				  size);

		cont += size;
	}while(req.size>0);

	if(req.size>=0){
		req.size = head->u_st_tcpServer_msg.op_write.size;
	}
	tcpServer_comm_writedata(sd,(char *)&req,sizeof(struct st_tcpServer_write_req), head->id);

	#ifdef _MALLOC_
		free(buffer);
	#endif

	debug_info("[OPS] (%s) end write: fd %d ID=%sn",TCPSERVER_ALIAS_NAME_STRING,
						head->u_st_tcpServer_msg.op_write.fd,
						head->id);
}

void tcpServer_op_mkdir(int sd, struct st_tcpServer_msg *head)
{
	char s[255];
	int ret;

	strcpy(s, head->u_st_tcpServer_msg.op_mkdir.path);
	ret = mkdir(s, 0777);
	tcpServer_comm_writedata(sd,(char *)&ret,sizeof(int), head->id);

	debug_info("[OPS] (%s) end MKDIR operation from ID=%s\n",TCPSERVER_ALIAS_NAME_STRING,head->id);
}

void tcpServer_op_rmdir(int sd, struct st_tcpServer_msg *head)
{
	char s[255];
	int ret;

	strcpy(s, head->u_st_tcpServer_msg.op_rmdir.path);
	ret = rmdir(s);
	tcpServer_comm_writedata(sd,(char *)&ret,sizeof(int), head->id);

	debug_info("[OPS] (%s) end RMDIR operation from ID=%s\n",TCPSERVER_ALIAS_NAME_STRING,head->id);
}

void tcpServer_op_setattr(int sd, struct st_tcpServer_msg *head)
{
	// check params...
	if (sd < 0) {
	    printf("[OPS] (%s)  RM operation with sd < 0\n",TCPSERVER_ALIAS_NAME_STRING);
	}
	if (NULL == head) {
	    printf("[OPS] (%s)  RM operation with head == NULL\n",TCPSERVER_ALIAS_NAME_STRING);
	}
}

void tcpServer_op_getattr(int sd, struct st_tcpServer_msg *head)
{
	char s[255];
	struct st_tcpServer_attr_req req;

	debug_info("[OPS] (%s) begin getattr(%s) ID=%sn",TCPSERVER_ALIAS_NAME_STRING,
						head->u_st_tcpServer_msg.op_getattr.path,
						head->id);

	strcpy(s, head->u_st_tcpServer_msg.op_getattr.path);
	req.status = stat(s, &req.attr);
	tcpServer_comm_writedata(sd,(char *)&req,sizeof(struct st_tcpServer_attr_req), head->id);

	debug_info("[OPS] (%s) end GETATTR(%s) ID=%sn",TCPSERVER_ALIAS_NAME_STRING,
						head->u_st_tcpServer_msg.op_getattr.path,
						head->id);
}

void tcpServer_op_getid(int sd, struct st_tcpServer_msg *head)
{
        debug_info("[OPS] (%s) begin GETID ID=%s\n",TCPSERVER_ALIAS_NAME_STRING, head->id);

        //sprintf(s,"%s/", head->u_st_tcpServer_msg.op_rmdir.path);
        tcpServer_comm_writedata(sd,(char *)head->id, TCPSERVER_ID, head->id);

        debug_info("[OPS] (%s) end GETID operation from ID=%s\n",TCPSERVER_ALIAS_NAME_STRING,head->id);
}

