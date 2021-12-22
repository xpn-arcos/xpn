void mpiServer_op_read_orig ( mpiServer_param_st *params, int sd, struct st_mpiServer_msg *head )
{
	long size_req, size = 0, cont = 0;
	//char *buffer;
	struct st_mpiServer_read_req req;
	int SIZE;

#ifndef _MALLOC_
	char buffer[MAX_BUFFER_SIZE];
	SIZE = MAX_BUFFER_SIZE;
	debug_info("[OPS] (ID=%s) op_read: static buffer (%d) ID=x\n", params->srv_name,MAX_BUFFER_SIZE);
#else
	char *buffer;
	SIZE = 0;
	debug_info("[OPS] (ID=%s) op_read: variable buffer (%d) ID=x\n", params->srv_name,MAX_BUFFER_SIZE);
#endif

	//
	debug_info("[OPS] (ID=%s) begin read: fd %d offset %d size %d ID=x\n", params->srv_name,
						head->u_st_mpiServer_msg.op_read.fd,
						(int)head->u_st_mpiServer_msg.op_read.offset,
						head->u_st_mpiServer_msg.op_read.size);

#ifdef _MALLOC_
	SIZE = head->u_st_mpiServer_msg.op_read.size;
	buffer = (char *)malloc(SIZE);
	//buffer = (char *)malloc(MAX_BUFFER_SIZE);
	//debug_info("[OPS] (ID=%s) op_read: malloc(%d) ID=x\n", params->srv_name,MAX_BUFFER_SIZE);
	debug_info("[OPS] (ID=%s) op_read: malloc(%d) ID=x\n", params->srv_name,SIZE);
#endif
	//t1 = MPI_Wtime();
	do{
#ifdef _LARGEFILE64_
		lseek64(head->u_st_mpiServer_msg.op_read.fd,
				head->u_st_mpiServer_msg.op_read.offset+cont,
				0);
#else
	        debug_info("[OPS] (ID=%s) lseek: fd %d offset %d size %d ID=x\n", params->srv_name,
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
		debug_info("[OPS] (ID=%s) op_read: send size %d\n", params->srv_name, req.size);

		if(req.size > 0){
			mpiServer_comm_writedata(params, sd, (char *)buffer, req.size);
			debug_info("[OPS] (ID=%s) op_read: send data\n", params->srv_name);
		}else{
			break;
		}
	}while((size >0)&&(!req.last));

        debug_info("[OPS] (ID=%s) end read: fd %d offset %d size %d ID=x\n", params->srv_name,
                                                                       head->u_st_mpiServer_msg.op_read.fd,
                                                                       (int)head->u_st_mpiServer_msg.op_read.offset,
                                                                       size);

	debug_info("[OPS] (ID=%s) op_read: new_size %d\n", params->srv_name,size);
#ifdef _MALLOC_
	debug_info("[OPS] (ID=%s) op_read: free ID=x\n", params->srv_name);
	free(buffer);
#endif
        debug_info("[OPS] (ID=%s) end READ: fd %d offset %d size %d ID=x\n", params->srv_name,
                                                head->u_st_mpiServer_msg.op_read.fd,
                                                (int)head->u_st_mpiServer_msg.op_read.offset,
                                                size);
}
