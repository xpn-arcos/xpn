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

	debug_info("[OPS] (ID=%s) begin write: fd %d ID=xn", params->srv_name, head->u_st_mpiServer_msg.op_write.fd);

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

	debug_info("[OPS] (ID=%s) op_write on fd %d with %d bytes", params->srv_name, head->u_st_mpiServer_msg.op_write.fd, req.size);

	debug_info("[OPS] (ID=%s) end write: fd %d ID=xn", params->srv_name, head->u_st_mpiServer_msg.op_write.fd);
}
