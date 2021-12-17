//#include "mpiServer.h"
#include "mpiServer_ops.h"
#include "mpiServer_comm.h"
#include "mpiServer_d2xpn.h"

//char global_buffer[MAX_BUFFER_SIZE];

int aux_clear_dirs(char *path){
	int i, j;
	char ant = '\0', s[255];
	
	j=0;
	for(i=0;i < strlen(path); i++){
		switch(path[i]){
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

int aux_get_dirs(char *path, int n, char *s){
	int i=0, j=0, ant=-1, pos=-1;
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


int mpiServer_create_spacename(char *path)
{
	char dir[MAXPATHLEN];
	int i;

	debug_info("[OPS]%s)mpiServer_create_spacename: %s\n", MPISERVER_ALIAS_NAME_STRING, path);
	for(i=0; 0 != aux_get_dirs(path, i, dir);i++){
		mkdir(dir, 0777);
	}

	return 0;
}

/* OPERATIONAL FUNCTIONS */

/**********************************
Read the operation to realize
***********************************/
int mpiServer_read_operation(int sd, struct st_mpiServer_msg *head){
	int ret;

	debug_info("[OPS]%s)antes de read_operation: sizeof(struct st_mpiServer_msg) = %d.\n ", MPISERVER_ALIAS_NAME_STRING, sizeof(struct st_mpiServer_msg));
	ret = mpiServer_comm_readdata(sd, (char *)&head->type, sizeof(head->type), head->id);
	if(ret == -1){
	        debug_info("[OPS]%s) mpiServer_comm_readdata fail\n") ;
		return -1;
	}
	
	debug_info("[OPS]%s)read_operation: %d ID=%s\n", MPISERVER_ALIAS_NAME_STRING,head->type,head->id);
	switch(head->type){
	case MPISERVER_OPEN_FILE:
		debug_info("[OPS]%s)OPEN operation from ID=%s\n",MPISERVER_ALIAS_NAME_STRING,head->id);
		ret = mpiServer_comm_readdata(sd, (char *)&head->u_st_mpiServer_msg.op_open, sizeof(struct st_mpiServer_open), head->id);
		if(ret == -1){
			return -1;
		}
	
		break;
	case MPISERVER_CREAT_FILE:
		debug_info("[OPS]%s)CREAT operation from ID=%s\n",MPISERVER_ALIAS_NAME_STRING,head->id);
		ret = mpiServer_comm_readdata(sd, (char *)&head->u_st_mpiServer_msg.op_creat, sizeof(struct st_mpiServer_creat), head->id);
		if(ret == -1){
			return -1;
		}
	
		break;
	case MPISERVER_READ_FILE:
		debug_info("[OPS]%s)READ operation from ID=%s\n",MPISERVER_ALIAS_NAME_STRING,head->id);
		ret = mpiServer_comm_readdata(sd, (char *)&head->u_st_mpiServer_msg.op_read, sizeof(struct st_mpiServer_read), head->id);
		if(ret == -1){
			return -1;
		}
	
		break;
	case MPISERVER_WRITE_FILE:
		debug_info("[OPS]%s)WRITE operation from ID=%s\n",MPISERVER_ALIAS_NAME_STRING,head->id);
		ret = mpiServer_comm_readdata(sd, (char *)&head->u_st_mpiServer_msg.op_write, sizeof(struct st_mpiServer_write), head->id);
		if(ret == -1){
			return -1;
		}
	
		break;
	case MPISERVER_CLOSE_FILE:
		debug_info("[OPS]%s)CLOSE operation from ID=%s\n",MPISERVER_ALIAS_NAME_STRING,head->id);
		ret = mpiServer_comm_readdata(sd, (char *)&head->u_st_mpiServer_msg.op_close, sizeof(struct st_mpiServer_close), head->id);
		if(ret == -1){
			return -1;
		}
	
		break;
	case MPISERVER_RM_FILE:
		debug_info("[OPS]%s)RM operation from ID=%s\n",MPISERVER_ALIAS_NAME_STRING,head->id);
		ret = mpiServer_comm_readdata(sd, (char *)&head->u_st_mpiServer_msg.op_rm, sizeof(struct st_mpiServer_rm), head->id);
		if(ret == -1){
			return -1;
		}
	
		break;
	case MPISERVER_GETATTR_FILE:
		debug_info("[OPS]%s)GETATTR operation from ID=%s\n",MPISERVER_ALIAS_NAME_STRING,head->id);
		ret = mpiServer_comm_readdata(sd, (char *)&head->u_st_mpiServer_msg.op_getattr, sizeof(struct st_mpiServer_getattr), head->id);
		if(ret == -1){
			return -1;
		}
	
		break;
	case MPISERVER_SETATTR_FILE:
		debug_info("[OPS]%s)SETATTR operation from ID=%s\n",MPISERVER_ALIAS_NAME_STRING,head->id);
		ret = mpiServer_comm_readdata(sd, (char *)&head->u_st_mpiServer_msg.op_setattr, sizeof(struct st_mpiServer_setattr), head->id);
		if(ret == -1){
			return -1;
		}
	
		break;
	case MPISERVER_MKDIR_DIR:
		debug_info("[OPS]%s)MKDIR operation from ID=%s\n",MPISERVER_ALIAS_NAME_STRING,head->id);
		ret = mpiServer_comm_readdata(sd, (char *)&head->u_st_mpiServer_msg.op_mkdir, sizeof(struct st_mpiServer_mkdir), head->id);
		if(ret == -1){
			return -1;
		}
	
		break;
	case MPISERVER_RMDIR_DIR:
		debug_info("[OPS]%s)RMDIR operation from ID=%s\n",MPISERVER_ALIAS_NAME_STRING,head->id);
		ret = mpiServer_comm_readdata(sd, (char *)&head->u_st_mpiServer_msg.op_rmdir, sizeof(struct st_mpiServer_rmdir), head->id);
		if(ret == -1){
			return -1;
		}
	
		break;
	case MPISERVER_FLUSH_FILE:
		debug_info("[OPS]%s)FLUSH operation from ID=%s\n",MPISERVER_ALIAS_NAME_STRING,head->id);
		ret = mpiServer_comm_readdata(sd, (char *)&head->u_st_mpiServer_msg.op_flush, sizeof(struct st_mpiServer_flush), head->id);
		if(ret == -1){
			return -1;
		}
	
		break;
	case MPISERVER_PRELOAD_FILE:
		debug_info("[OPS]%s)PRELOAD operation from ID=%s\n",MPISERVER_ALIAS_NAME_STRING,head->id);
		ret = mpiServer_comm_readdata(sd, (char *)&head->u_st_mpiServer_msg.op_preload, sizeof(struct st_mpiServer_preload), head->id);
		if(ret == -1){
			return -1;
		}
	
		break;
        case MPISERVER_GETID:
                debug_info("[OPS]%s)GETID operation from ID=%s\n",MPISERVER_ALIAS_NAME_STRING,head->id);
/*
		ret = mpiServer_comm_readdata(sd, (char *)&head->id, sizeof(MPISERVER_ID), head->id);
		if(ret == -1){
			return -1;
		}
*/	
                break;
	case MPISERVER_FINALIZE:
		debug_info("[OPS]%s)FINALIZE operation from ID=%s\n",MPISERVER_ALIAS_NAME_STRING,head->id);
/*
		ret = mpiServer_comm_readdata(sd, (char *)&head->u_st_mpiServer_msg.op_end, sizeof(struct st_mpiServer_end), head->id);
		if(ret == -1){
			return -1;
		}
*/	
		break;
	case MPISERVER_END:
		debug_info("[OPS]%s)END operation from ID=%s\n",MPISERVER_ALIAS_NAME_STRING,head->id);
/*
		ret = mpiServer_comm_readdata(sd, (char *)&head->u_st_mpiServer_msg.op_end, sizeof(struct st_mpiServer_end), head->id);
		if(ret == -1){
			return -1;
		}
*/	
		break;
	}
	return head->type;
}


void mpiServer_op_open(int sd, struct st_mpiServer_msg *head){
	int fd;
	char s[255];
	
	//sprintf(s,"%s", head->u_st_mpiServer_msg.op_open.path);
	strcpy(s,head->u_st_mpiServer_msg.op_open.path);

	debug_info("[OPS]%s> begin open(%s) ID=%s -> %d\n",MPISERVER_ALIAS_NAME_STRING,s,head->id,fd);
	
	fd = open(s, O_RDWR);
	//if(fd == -1){
	//	mpiServer_create_spacename(s);
	//}	
	debug_info("[OPS]%s> end open(%s) ID=%s ->%d\n",MPISERVER_ALIAS_NAME_STRING,s, head->id, fd);	
	mpiServer_comm_writedata(sd, (char *)&fd, sizeof(int), head->id);
	debug_info("[OPS]%s)OPEN operation from ID=%s\n",MPISERVER_ALIAS_NAME_STRING,head->id);
}





void mpiServer_op_creat(int sd, struct st_mpiServer_msg *head){
	int fd;
	char s[255];

	
	//sprintf(s,"%s", head->u_st_mpiServer_msg.op_creat.path);
	strcpy(s,head->u_st_mpiServer_msg.op_creat.path);
	
	//printf("[OPS]%s)creat(%s) ID=%s ->\n",MPISERVER_ALIAS_NAME_STRING,s,head->id);
	debug_info("[OPS]%s)begin creat(%s) ID=%s ->\n",MPISERVER_ALIAS_NAME_STRING,s,head->id);
	//fd = open(s, O_CREAT | O_TRUNC | O_RDWR, 0666);
	fd = open(s, O_CREAT | O_RDWR, 0777);
	if(fd == -1){
		mpiServer_create_spacename(s);
	        //fd = open(s, O_CREAT | O_TRUNC | O_RDWR, 0666);
	        fd = open(s, O_CREAT | O_RDWR, 0666);
	}	
	
	debug_info("[OPS]%s)end creat(%s) ID=%s -> %d\n",MPISERVER_ALIAS_NAME_STRING,s,head->id,fd);
	mpiServer_comm_writedata(sd,(char *)&fd,sizeof(int), head->id);
	debug_info("[OPS]%s)end CREAT operation from ID=%s ->%d\n",MPISERVER_ALIAS_NAME_STRING,head->id,fd);
}


void mpiServer_op_flush(int sd, struct st_mpiServer_msg *head){
	int ret = 0;
	//char s_exe[1024];

	
	//sprintf(s,"%s", head->u_st_mpiServer_msg.op_creat.path);
	//strcpy(s,head->u_st_mpiServer_msg.op_flush.path);
	
	debug_info("[OPS]%s)begin flush(%s) ID=%s ->\n",MPISERVER_ALIAS_NAME_STRING, head->u_st_mpiServer_msg.op_flush.virtual_path, head->id);
/*	
	sprintf(s_exe,"%s %s %s\n", MPISERVER_FLUSH_EXE,
				head->u_st_mpiServer_msg.op_flush.virtual_path,
				head->u_st_mpiServer_msg.op_flush.storage_path);
	debug_info("[OPS]begin system->%s\n",s_exe);
	ret = system(s_exe);
	debug_info("[OPS]end system->%s\n",s_exe);

	debug_info("[OPS]%s)open flush(%s) ID=%s -> %d\n",MPISERVER_ALIAS_NAME_STRING,
							head->u_st_mpiServer_msg.op_flush.virtual_path,
							sd, head->id,ret);
*/	
	mpiServer_comm_writedata(sd, (char *)&ret, sizeof(int), head->id);
	debug_info("[OPS]%s)end FLUSH operation from ID=%s\n",MPISERVER_ALIAS_NAME_STRING,head->id);
}


void mpiServer_op_preload(int sd, struct st_mpiServer_msg *head){
	int ret;
	//char s_exe[1024];

	
	//sprintf(s,"%s", head->u_st_mpiServer_msg.op_creat.path);
	//strcpy(s,head->u_st_mpiServer_msg.op_flush.path);
	
	debug_info("[OPS]%s)begin preload(%s,%s) ID=%s\n", MPISERVER_ALIAS_NAME_STRING,
							     head->u_st_mpiServer_msg.op_preload.virtual_path,
							     head->u_st_mpiServer_msg.op_preload.storage_path,
							     head->id);
	ret = 0;

	ret = mpiServer_d2xpn(head->u_st_mpiServer_msg.op_preload.virtual_path,
                              head->u_st_mpiServer_msg.op_preload.storage_path,
			      head->u_st_mpiServer_msg.op_preload.opt);


	debug_info("[OPS]%s)end preload(%s,%s) ID=%s -> %d\n",MPISERVER_ALIAS_NAME_STRING,
							head->u_st_mpiServer_msg.op_preload.virtual_path,
							head->u_st_mpiServer_msg.op_preload.storage_path,
							head->id, ret);

	mpiServer_comm_writedata(sd, (char *)&ret, sizeof(int),  head->id);

	debug_info("[OPS]%s)end PRELOAD operation from ID=%s\n", MPISERVER_ALIAS_NAME_STRING,head->id);
}




void mpiServer_op_close(int sd, struct st_mpiServer_msg *head)
{
	debug_info("[OPS]%s)begin close: fd %d ID=%s\n",MPISERVER_ALIAS_NAME_STRING,
							head->u_st_mpiServer_msg.op_close.fd,
							head->id);
	
	close(head->u_st_mpiServer_msg.op_close.fd);
	debug_info("[OPS]%s)begin close: fd %d ID=%s\n",MPISERVER_ALIAS_NAME_STRING,
							head->u_st_mpiServer_msg.op_close.fd,
							head->id);

	debug_info("[OPS]%s)end CLOSE operation from ID=%s\n",MPISERVER_ALIAS_NAME_STRING,head->id);
}


void mpiServer_op_rm(int sd, struct st_mpiServer_msg *head)
{
	char s[255];

	//sprintf(s,"%s", head->u_st_mpiServer_msg.op_rm.path);
	strcpy(s, head->u_st_mpiServer_msg.op_rm.path);

	debug_info("[OPS]%s)begin unlink: path %s ID=%s\n",MPISERVER_ALIAS_NAME_STRING,
						head->u_st_mpiServer_msg.op_rm.path,
						head->id);
	unlink(s);

	debug_info("[OPS]%s)end unlink: path %s ID=%s\n",MPISERVER_ALIAS_NAME_STRING,
						head->u_st_mpiServer_msg.op_rm.path,
						head->id);
	debug_info("[OPS]%s)end RM operation from ID=%s\n",MPISERVER_ALIAS_NAME_STRING,head->id);
}

void mpiServer_op_read(int sd, struct st_mpiServer_msg *head)
{
	int size_req, size = 0, cont = 0;
	//char *buffer;
	struct st_mpiServer_read_req req;
	int SIZE;
#ifndef _MALLOC_
	char buffer[MAX_BUFFER_SIZE];
	SIZE = MAX_BUFFER_SIZE;
	debug_info("[OPS]%s)op_read: static buffer (%d) ID=%s\n",MPISERVER_ALIAS_NAME_STRING,MAX_BUFFER_SIZE,head->id);
#else
	char *buffer;
	SIZE = 0;
	debug_info("[OPS]%s)op_read: variable buffer (%d) ID=%s\n",MPISERVER_ALIAS_NAME_STRING,MAX_BUFFER_SIZE,head->id);
#endif
	

	//
	
	debug_info("[OPS]%s)begin read: fd %d offset %d size %d ID=%s\n",MPISERVER_ALIAS_NAME_STRING,
						head->u_st_mpiServer_msg.op_read.fd,
						(int)head->u_st_mpiServer_msg.op_read.offset,
						head->u_st_mpiServer_msg.op_read.size,
						head->id);	

/*
#ifdef _LARGEFILE64_
	printf("[OPS]%s)op_read: offset %lld\n",MPISERVER_ALIAS_NAME_STRING,head->u_st_mpiServer_msg.op_read.offset);
#else
	printf("[OPS]%s)op_read: offset %d\n",MPISERVER_ALIAS_NAME_STRING,(int)head->u_st_mpiServer_msg.op_read.offset);
#endif
	printf("[OPS]%s)op_read: size %d\n",MPISERVER_ALIAS_NAME_STRING,head->u_st_mpiServer_msg.op_read.size);
#endif		
*/
#ifdef _MALLOC_
	SIZE = head->u_st_mpiServer_msg.op_read.size;
	buffer = (char *)malloc(SIZE);
	//buffer = (char *)malloc(MAX_BUFFER_SIZE);
	//debug_info("[OPS]%s)op_read: malloc(%d) ID=%s\n",MPISERVER_ALIAS_NAME_STRING,MAX_BUFFER_SIZE,head->id);
	debug_info("[OPS]%s)op_read: malloc(%d) ID=%s\n",MPISERVER_ALIAS_NAME_STRING,SIZE,head->id);
#endif
	//t1 = MPI_Wtime();
	do{	
#ifdef _LARGEFILE64_	
		lseek64(head->u_st_mpiServer_msg.op_read.fd,
				head->u_st_mpiServer_msg.op_read.offset+cont,
				0);
#else
	        debug_info("[OPS]%s)lseek: fd %d offset %d size %d ID=%s\n",MPISERVER_ALIAS_NAME_STRING,
						head->u_st_mpiServer_msg.op_read.fd,
						(int)head->u_st_mpiServer_msg.op_read.offset+cont,
						head->u_st_mpiServer_msg.op_read.size,
						head->id);	
		
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


		
		//usleep(rand()%1000);	
		//req.size = size_req;

		if(req.size < 0){
			perror("read:");
		}	
		cont += req.size;
		if((cont == head->u_st_mpiServer_msg.op_read.size) ||
			(req.size < size_req)){
			req.last = 1;
		}else{
			req.last = 0;
		}
		mpiServer_comm_writedata(sd, (char *)&req, sizeof(struct st_mpiServer_read_req), head->id);
		debug_info("[OPS]%s)op_read: send size %d\n",MPISERVER_ALIAS_NAME_STRING, req.size);

		if(req.size > 0){
			mpiServer_comm_writedata(sd, (char *)buffer, req.size, head->id);
			debug_info("[OPS]%s)op_read: send data\n",MPISERVER_ALIAS_NAME_STRING);
		}else{
			break;
		}
	}while((size >0)&&(!req.last));
	//t2 = MPI_Wtime();

        debug_info("[OPS]%s)end read: fd %d offset %d size %d ID=%s\n",MPISERVER_ALIAS_NAME_STRING,
                                                head->u_st_mpiServer_msg.op_read.fd,
                                                (int)head->u_st_mpiServer_msg.op_read.offset,
                                                size,
                                                head->id);
	
	//debug_info("[OPS]%s)op_read: %f sec.\n",MPISERVER_ALIAS_NAME_STRING,t2 - t1);
	debug_info("[OPS]%s)op_read: new_size %d\n",MPISERVER_ALIAS_NAME_STRING,size);
#ifdef _MALLOC_	
	debug_info("[OPS]%s)op_read: free ID=%s\n",MPISERVER_ALIAS_NAME_STRING,head->id);
	free(buffer);
#endif
        debug_info("[OPS]%s)end READ: fd %d offset %d size %d ID=%s\n",MPISERVER_ALIAS_NAME_STRING,
                                                head->u_st_mpiServer_msg.op_read.fd,
                                                (int)head->u_st_mpiServer_msg.op_read.offset,
                                                size,
                                                head->id);
}

void mpiServer_op_write(int sd, struct st_mpiServer_msg *head){
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

	debug_info("[OPS]%s)begin write: fd %d ID=%sn",MPISERVER_ALIAS_NAME_STRING,
						head->u_st_mpiServer_msg.op_write.fd,
						head->id);	

/*
	debug_info("[OPS]%s)op_write: fd %d\n",MPISERVER_ALIAS_NAME_STRING,head->u_st_mpiServer_msg.op_write.fd);
#ifdef _LARGEFILE64_
	printf("[OPS]%s)op_write: offset %lld\n",MPISERVER_ALIAS_NAME_STRING,head->u_st_mpiServer_msg.op_write.offset);
#else
	printf("[OPS]%s)op_write: offset %d\n",MPISERVER_ALIAS_NAME_STRING,(int)head->u_st_mpiServer_msg.op_write.offset);
#endif
	printf("[OPS]%s)op_write: size %d\n",MPISERVER_ALIAS_NAME_STRING,head->u_st_mpiServer_msg.op_write.size);
*/	
	//t1 = MPI_Wtime();

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
	mpiServer_comm_readdata(sd,(char *)buffer, size, head->id);

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
	mpiServer_comm_writedata(sd,(char *)&req,sizeof(struct st_mpiServer_write_req), head->id);

	#ifdef _MALLOC_	
		free(buffer);
	#endif

	debug_info("[OPS]%s)op_write: %d\n",MPISERVER_ALIAS_NAME_STRING, req.size);
	debug_info("[OPS]%s)end write: fd %d ID=%sn",MPISERVER_ALIAS_NAME_STRING,
						head->u_st_mpiServer_msg.op_write.fd,
						head->id);	
	debug_info("[OPS]%s)end WRITE operation from ID=%s\n",MPISERVER_ALIAS_NAME_STRING,head->id);
}

void mpiServer_op_mkdir(int sd, struct st_mpiServer_msg *head){
	char s[255];
	int ret;

	//sprintf(s,"%s/", head->u_st_mpiServer_msg.op_mkdir.path);
	strcpy(s, head->u_st_mpiServer_msg.op_mkdir.path);
	
	ret = mkdir(s, 0777);
	mpiServer_comm_writedata(sd,(char *)&ret,sizeof(int), head->id);
	debug_info("[OPS]%s)end MKDIR operation from ID=%s\n",MPISERVER_ALIAS_NAME_STRING,head->id);
}

void mpiServer_op_rmdir(int sd, struct st_mpiServer_msg *head){
	char s[255];
	int ret;
	//sprintf(s,"%s/", head->u_st_mpiServer_msg.op_rmdir.path);
	strcpy(s, head->u_st_mpiServer_msg.op_rmdir.path);
	ret = rmdir(s);
	mpiServer_comm_writedata(sd,(char *)&ret,sizeof(int), head->id);
	debug_info("[OPS]%s)end RMDIR operation from ID=%s\n",MPISERVER_ALIAS_NAME_STRING,head->id);
}

void mpiServer_op_setattr(int sd, struct st_mpiServer_msg *head){
	debug_info("[OPS]%s)begin SETATTR operation from ID=%s\n",MPISERVER_ALIAS_NAME_STRING,head->id);
	debug_info("[OPS]%s)SETATTR operation to be implemented !!\n",MPISERVER_ALIAS_NAME_STRING);
	debug_info("[OPS]%s)end   SETATTR operation from ID=%s\n",MPISERVER_ALIAS_NAME_STRING,head->id);
}

void mpiServer_op_getattr(int sd, struct st_mpiServer_msg *head){
	char s[255];
	struct st_mpiServer_attr_req req;

	debug_info("[OPS]%s)begin getattr(%s) ID=%sn",MPISERVER_ALIAS_NAME_STRING,
						head->u_st_mpiServer_msg.op_getattr.path,	
						head->id);	
	//sprintf(s,"%s/", head->u_st_mpiServer_msg.op_rmdir.path);
	strcpy(s, head->u_st_mpiServer_msg.op_getattr.path);
	req.status = stat(s, &req.attr);
	mpiServer_comm_writedata(sd,(char *)&req,sizeof(struct st_mpiServer_attr_req), head->id);
	debug_info("[OPS]%s)end getattr(%s) ID=%sn",MPISERVER_ALIAS_NAME_STRING,
						head->u_st_mpiServer_msg.op_getattr.path,	
						head->id);	
	debug_info("[OPS]%s)end GETATTR operation from ID=%s\n",MPISERVER_ALIAS_NAME_STRING,head->id);
}



void mpiServer_op_getid(int sd, struct st_mpiServer_msg *head){
        debug_info("[OPS]%s)begin GETID ID=%s\n",MPISERVER_ALIAS_NAME_STRING,
                                                head->id);
        //sprintf(s,"%s/", head->u_st_mpiServer_msg.op_rmdir.path);
        mpiServer_comm_writedata(sd,(char *)head->id, MPISERVER_ID, head->id);
        debug_info("[OPS]%s)end GETID operation from ID=%s\n",MPISERVER_ALIAS_NAME_STRING,head->id);
}

