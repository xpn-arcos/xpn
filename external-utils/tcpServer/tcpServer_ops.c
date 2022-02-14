
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

      #include "tcpServer_ops.h"
      #include "tcpServer_comm.h"
      #include "tcpServer_d2xpn.h"


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


/* 
 * OPERATIONAL FUNCTIONS 
 */

char * tcpServer_op2string ( int op_code )
{
	char *ret = "Unknown" ;

	switch (op_code)
	{
		case TCPSERVER_OPEN_FILE:	ret = "OPEN" ;
			break;
		case TCPSERVER_CREAT_FILE:	ret = "CREAT" ;
			break;
		case TCPSERVER_READ_FILE:	ret = "READ" ;
			break;
		case TCPSERVER_WRITE_FILE:	ret = "WRITE" ;
			break;
		case TCPSERVER_CLOSE_FILE:	ret = "CLOSE" ;
			break;
		case TCPSERVER_RM_FILE:		ret = "RM" ;
			break;
		case TCPSERVER_GETATTR_FILE:	ret = "GETATTR" ;
			break;
		case TCPSERVER_SETATTR_FILE:	ret = "SETATTR" ;
			break;
		case TCPSERVER_MKDIR_DIR:	ret = "MKDIR" ;
			break;
		case TCPSERVER_RMDIR_DIR:	ret = "RMDIR" ;
			break;
		case TCPSERVER_FLUSH_FILE:	ret = "FLUSH" ;
			break;
		case TCPSERVER_PRELOAD_FILE:	ret = "PRELOAD" ;
			break;
		case TCPSERVER_GETID:		ret = "GETID" ;
			break;
		case TCPSERVER_FINALIZE:	ret = "FINALIZE" ;
			break;
		case TCPSERVER_END:		ret = "END" ;
			break;
	}

	return ret ;
}

// Read the operation to realize
int tcpServer_read_operation(int sd, struct st_tcpServer_msg *head)
{
	int ret;

	debug_info("[OPS] (%s) read_operation code of %d bytes.\n ", TCPSERVER_ALIAS_NAME_STRING, sizeof(head->type));
	ret = tcpServer_comm_readdata(sd, (char *)&head->type, sizeof(head->type), head->id);
	if (ret == -1) {
	    return -1;
	}

	debug_info("[OPS] (%s) read operation '%s' from ID=%s\n",TCPSERVER_ALIAS_NAME_STRING, tcpServer_op2string(head->type), head->id);
	switch (head->type)
	{
		case TCPSERVER_OPEN_FILE:
			ret = tcpServer_comm_readdata(sd, (char *)&(head->u_st_tcpServer_msg.op_open), sizeof(struct st_tcpServer_open), head->id);
			break;
		case TCPSERVER_CREAT_FILE:
			ret = tcpServer_comm_readdata(sd, (char *)&(head->u_st_tcpServer_msg.op_creat), sizeof(struct st_tcpServer_creat), head->id);
			break;
		case TCPSERVER_READ_FILE:
			ret = tcpServer_comm_readdata(sd, (char *)&(head->u_st_tcpServer_msg.op_read), sizeof(struct st_tcpServer_read), head->id);
			break;
		case TCPSERVER_WRITE_FILE:
			ret = tcpServer_comm_readdata(sd, (char *)&(head->u_st_tcpServer_msg.op_write), sizeof(struct st_tcpServer_write), head->id);
			break;
		case TCPSERVER_CLOSE_FILE:
			ret = tcpServer_comm_readdata(sd, (char *)&(head->u_st_tcpServer_msg.op_close), sizeof(struct st_tcpServer_close), head->id);
			break;
		case TCPSERVER_RM_FILE:
			ret = tcpServer_comm_readdata(sd, (char *)&(head->u_st_tcpServer_msg.op_rm), sizeof(struct st_tcpServer_rm), head->id);
			break;
		case TCPSERVER_GETATTR_FILE:
			ret = tcpServer_comm_readdata(sd, (char *)&(head->u_st_tcpServer_msg.op_getattr), sizeof(struct st_tcpServer_getattr), head->id);
			break;
		case TCPSERVER_SETATTR_FILE:
			ret = tcpServer_comm_readdata(sd, (char *)&(head->u_st_tcpServer_msg.op_setattr), sizeof(struct st_tcpServer_setattr), head->id);
			break;
		case TCPSERVER_MKDIR_DIR:
			ret = tcpServer_comm_readdata(sd, (char *)&(head->u_st_tcpServer_msg.op_mkdir), sizeof(struct st_tcpServer_mkdir), head->id);
			break;
		case TCPSERVER_RMDIR_DIR:
			ret = tcpServer_comm_readdata(sd, (char *)&(head->u_st_tcpServer_msg.op_rmdir), sizeof(struct st_tcpServer_rmdir), head->id);
			break;
		case TCPSERVER_FLUSH_FILE:
			ret = tcpServer_comm_readdata(sd, (char *)&(head->u_st_tcpServer_msg.op_flush), sizeof(struct st_tcpServer_flush), head->id);
			break;
		case TCPSERVER_PRELOAD_FILE:
			ret = tcpServer_comm_readdata(sd, (char *)&(head->u_st_tcpServer_msg.op_preload), sizeof(struct st_tcpServer_preload), head->id);
			break;
		case TCPSERVER_GETID:
			/* ret = tcpServer_comm_readdata(sd, (char *)&(head->id), sizeof(TCPSERVER_ID), head->id); */
			break;
		case TCPSERVER_FINALIZE:
			/* ret = tcpServer_comm_readdata(sd, (char *)&(head->u_st_tcpServer_msg.op_end), sizeof(struct st_tcpServer_end), head->id); */
			break;
		case TCPSERVER_END:
			/* ret = tcpServer_comm_readdata(sd, (char *)&(head->u_st_tcpServer_msg.op_end), sizeof(struct st_tcpServer_end), head->id); */
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

	debug_info("[OPS]%s> begin open(%s) ID=%s -> %d\n", TCPSERVER_ALIAS_NAME_STRING, head->u_st_tcpServer_msg.op_open.path, head->id, fd);

	strcpy(s,head->u_st_tcpServer_msg.op_open.path);
	fd = open(s, O_RDWR);
	//if(fd == -1){
	//	tcpServer_create_spacename(s);
	//}

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



long op_read_buffer (int read_fd2, void *buffer, int buffer_size )
{
     ssize_t read_num_bytes       = -1 ;
     ssize_t read_remaining_bytes = buffer_size ;
     void   *read_buffer          = buffer ;

     while (read_remaining_bytes > 0)
     {
     /* Read from local file... */
         read_num_bytes = read(read_fd2, read_buffer, read_remaining_bytes) ;

     /* Check errors */
         if (read_num_bytes == -1) {
         debug_error("ERROR:\t read fails to read data.\n") ;
         return -1 ;
         }

     /* Check end of file */
         if (read_num_bytes == 0)
     	{
	        debug_error("INFO:\t end of file, readed %ld.\n", (buffer_size - read_remaining_bytes)) ;
	        return (buffer_size - read_remaining_bytes) ;
        }

         read_remaining_bytes -= read_num_bytes ;
         read_buffer          += read_num_bytes ;
     }

     return buffer_size ;
}



long op_write_buffer ( int write_fd2, void *buffer, int buffer_size, int num_readed_bytes )
{
     ssize_t write_num_bytes       = -1 ;
     ssize_t write_remaining_bytes = num_readed_bytes ;
     void   *write_buffer          = buffer ;

     if (num_readed_bytes > buffer_size) {
     debug_error("ERROR:\t write for %d bytes from a buffer with only %d bytes.\n", num_readed_bytes, buffer_size) ;
     return -1 ;
     }

     while (write_remaining_bytes > 0)
     {
     /* Write into local file (write_fd2)... */
         write_num_bytes = write(write_fd2, write_buffer, write_remaining_bytes) ;

     /* Check errors */
         if (write_num_bytes == -1) {
         debug_error("ERROR:\t write fails to write data.\n") ;
         return -1 ;
         }

         write_remaining_bytes -= write_num_bytes ;
         write_buffer          += write_num_bytes ;
     }

     return num_readed_bytes ;
}



void tcpServer_op_read ( int sd, struct st_tcpServer_msg *head )
{
	unsigned long cont = 0;
	int size_req, size = 0;
	//char *buffer;
	struct st_tcpServer_read_req req;
	int SIZE;

#ifndef _MALLOC_
	char buffer[128*1024];
	size = head->u_st_tcpServer_msg.op_read.size;
	debug_info("[OPS] (%s) op_read: static buffer (%d) ID=%s\n",TCPSERVER_ALIAS_NAME_STRING,MAX_BUFFER_SIZE,head->id);
#else
	char *buffer;
	size = head->u_st_tcpServer_msg.op_read.size;
	debug_info("[OPS] (%s) op_read: variable buffer (%d) ID=%s\n",TCPSERVER_ALIAS_NAME_STRING,MAX_BUFFER_SIZE,head->id);
#endif

	//
	debug_info("[OPS] (%s) begin read: fd %d offset %d size %d ID=%s\n",TCPSERVER_ALIAS_NAME_STRING,
						head->u_st_tcpServer_msg.op_read.fd,
						(int)head->u_st_tcpServer_msg.op_read.offset,
						head->u_st_tcpServer_msg.op_read.size,
						head->id);

#ifdef _MALLOC_
	SIZE = head->u_st_tcpServer_msg.op_read.size;
	buffer = (char *)malloc(SIZE);
	//buffer = (char *)malloc(MAX_BUFFER_SIZE);
	//debug_info("[OPS] (%s) op_read: malloc(%d) ID=%s\n",TCPSERVER_ALIAS_NAME_STRING,MAX_BUFFER_SIZE,head->id);
	debug_info("[OPS] (%s) op_read: malloc(%d) ID=%s\n",TCPSERVER_ALIAS_NAME_STRING,SIZE,head->id);
#endif
	//t1 = MPI_Wtime();

#ifdef _LARGEFILE64_
	lseek64(head->u_st_tcpServer_msg.op_read.fd, head->u_st_tcpServer_msg.op_read.offset, 0);
#else
	debug_info("[OPS] (%s) lseek: fd %d offset %d size %d ID=%s\n",TCPSERVER_ALIAS_NAME_STRING,
						head->u_st_tcpServer_msg.op_read.fd,
						(int)head->u_st_tcpServer_msg.op_read.offset,
						head->u_st_tcpServer_msg.op_read.size,
						head->id);

	lseek(head->u_st_tcpServer_msg.op_read.fd, head->u_st_tcpServer_msg.op_read.offset, 0);
#endif

	req.size = op_read_buffer( head->u_st_tcpServer_msg.op_read.fd, buffer, size);

	if(req.size < 0){
		perror("read:");
		req.size = -1;  // TODO: check in client that -1 is treated properly... :-9
		tcpServer_comm_writedata(sd, (char *)&req, sizeof(struct st_tcpServer_write_req), head->id);
		return;
	}

	// send (how many + data) to client...
	tcpServer_comm_writedata(sd, (char *)&req, sizeof(struct st_tcpServer_read_req), head->id);
	debug_info("[OPS] (%s) op_read: send size %d\n",TCPSERVER_ALIAS_NAME_STRING, req.size);

	if(req.size > 0){
		tcpServer_comm_writedata(sd, (char *)buffer, req.size, head->id);
		debug_info("[OPS] (%s) op_read: send data\n",TCPSERVER_ALIAS_NAME_STRING);
	}


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
	char buffer[128*1024];
	size = head->u_st_tcpServer_msg.op_write.size
#else
	char *buffer;
	size = head->u_st_tcpServer_msg.op_write.size;
#endif

	debug_info("[OPS] (%s) begin write: fd %d ID=%sn",TCPSERVER_ALIAS_NAME_STRING,
						head->u_st_tcpServer_msg.op_write.fd,
						head->id);


#ifdef _MALLOC_
	buffer = (char *)malloc(size);
	//buffer = (char *)malloc(head->u_st_tcpServer_msg.op_read.size);
	//buffer = (char *)malloc(MAX_BUFFER_SIZE);
#endif
	tcpServer_comm_readdata(sd,(char *)buffer, size, head->id);

#ifdef _LARGEFILE64_
	lseek64(head->u_st_tcpServer_msg.op_write.fd, head->u_st_tcpServer_msg.op_write.offset, 0);
#else
	lseek(head->u_st_tcpServer_msg.op_write.fd, head->u_st_tcpServer_msg.op_write.offset, 0);
#endif

	req.size = op_write_buffer (head->u_st_tcpServer_msg.op_write.fd, buffer, size, size);

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
	// TODO: setattr
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
        tcpServer_comm_writedata(sd, (char *)(head->id), TCPSERVER_ID, head->id);

        debug_info("[OPS] (%s) end GETID operation from ID=%s\n",TCPSERVER_ALIAS_NAME_STRING,head->id);
}

