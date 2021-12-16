#include "mpiServer_comm.h"

static int global_sock = 0;



int mpiServer_comm_writedata_test(int fd, char *id){
        /*****************************TEST****************************************/
	int ret;
        char buffer_temp[CONST_TEMP], aux;
        int i;

        debug_printf("[COMM] ===init write test ID=%s:%p --th:%d--===\n",id,id,(int)pthread_self());
        for (i=0;i<CONST_TEMP;i++){
		aux = (char)(i%128);
                buffer_temp[i] = aux;
        }

        ret = write(fd, buffer_temp, CONST_TEMP);
	if(ret == 0){
		perror("[COMM]server:ERROR TEST(0) write");
		return 0;
	}
        if(ret != CONST_TEMP ){
                printf("[COMM]server:ERROR TEST(1) write_data(%d): err %d  ID=%s:%p --th:%d--\n",fd,ret,id,id,(int)pthread_self());
		//exit(-1);
		perror("[COMM]server:ERROR TEST(1) write");
        }
        debug_printf("[COMM] send write test ok ID=%s\n",id);

	bzero(buffer_temp, CONST_TEMP);
        ret = read(fd, buffer_temp, CONST_TEMP);
	if(ret == 0){
		perror("[COMM]server:ERROR TEST(1.1) write");
		return 0;
	}
        if(ret != CONST_TEMP ){
                printf("[COMM]server:ERROR TEST(2) write_data(%d): err %d  ID=%s:%p --th:%d--\n",fd,ret,id,id,(int)pthread_self());
		//exit(-1);
		perror("[COMM]server:ERROR TEST(2) write");
        }
        debug_printf("[COMM] receive write test ok ID=%s:%p\n",id,id);
        for (i=0;i<CONST_TEMP;i++){
		aux = (char)(i%128);
                if(buffer_temp[i] != aux){
                        printf("[COMM]server:ERROR TEST(3) write_data(%d): err %d(%d!=%d) ID=%s:%p --th:%d--\n",fd,ret,buffer_temp[i],aux,id,id,(int)pthread_self());
			perror("[COMM]server:ERROR TEST(3) write");
                }
        }
        debug_printf("[COMM] ===check write test ok ID=%s:%p--th:%d--===\n",id,id,(int)pthread_self());
        /*****************************TEST****************************************/
        return 0;
}


int mpiServer_comm_readdata_test(int fd, char *id){

        /*****************************TEST****************************************/
	int ret;
        char buffer_temp[CONST_TEMP], aux;
        int i;


	bzero(buffer_temp, CONST_TEMP);
        debug_printf("[COMM] ===init read test ok ID=%s:%p --th:%d--===\n",id,id,(int)pthread_self());

        ret = read(fd, buffer_temp, CONST_TEMP);
	if(ret == 0){
		perror("[COMM]server:ERROR TEST(0) read");
		return 0;
	}

        if(ret != CONST_TEMP ){
                printf("[COMM]server:ERROR TEST(1) read_data(%d): err %d ID=%s:%p --th:%d--\n",fd,ret,id,id,(int)pthread_self());
		//exit(-1);
		perror("[COMM]server:ERROR TEST(1) read");
        }
        debug_printf("[COMM] receive read test ok ID=%s:%p\n",id,id);
        for (i=0;i<CONST_TEMP;i++){
		aux = (char)(i%128);
                if(buffer_temp[i] != aux){
                        printf("[COMM]server:ERROR TEST(2) write_data(%d): err %d(%d!=%d)  ID=%s:%p --th:%d--\n",fd,ret,buffer_temp[i],aux,id,id,(int)pthread_self());
                        //printf("[COMM]server:ERROR TEST(2) read_data(%d): err %d  ID=%s:%p\n",fd,ret,id,id);
			perror("[COMM]server:ERROR TEST(2) read");
                }
        }
        debug_printf("[COMM] check read test ok ID=%s:%p --th:%d--\n",id,id,(int)pthread_self());

        for (i=0;i<CONST_TEMP;i++){
		aux = (char)(i%128);
                buffer_temp[i] = aux;
        }

        ret = write(fd, buffer_temp, CONST_TEMP);
	if(ret == 0){
		perror("[COMM]server:ERROR TEST(2.1) read");
		return 0;
	}
        if(ret != CONST_TEMP ){
                printf("[COMM]server:ERROR TEST(3) read_data(%d): err %d  ID=%s:%p --th:%d--\n",fd,ret,id,id,(int)pthread_self());
		perror("[COMM]server:ERROR TEST(3) read");
        }
        debug_printf("[COMM] ===send read test ok ID=%s:%p--th:%d--===\n",id,id,(int)pthread_self());
        /*****************************TEST****************************************/
        return 0;
}




/* AUXILIAR FUNCTIONS */
int     mpiServer_comm_init      ( mpiServer_param_st *params )
{
	struct sockaddr_in server_addr;
	int val, ret;
	char host[255];
	FILE *f;

        debug_printf("[COMM]begin mpiServer_comm_init(%s, %d, %s)\n",params->name, params->port, params->file);
	val = 1;
	/* create the connections */
	global_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(global_sock<0){
		perror("error en el socket:");
		return -1;
	}

	val = 1;
	if (setsockopt (global_sock, IPPROTO_TCP,TCP_NODELAY, &val, sizeof(val)) == -1){
                perror("setsockopt: ");
                return -1;
        }

        //NEW

        val = 1024 * 1024; //1 MB

        if (setsockopt(global_sock, SOL_SOCKET, SO_SNDBUF, (char *) &val, sizeof(int)) == -1){
                perror("setsockopt: ");
                return -1;
        }

        val = 1024 * 1024; //1 MB
        if (setsockopt(global_sock, SOL_SOCKET, SO_RCVBUF, (char *) &val, sizeof(int)) == -1){
                perror("setsockopt: ");
                return -1;
        }


	val = 1;	
	ret = setsockopt(global_sock, SOL_SOCKET, SO_REUSEADDR, (char *) &val, sizeof(int));
	if(ret == -1){
		perror("error en el setsockopt:");
		return -1;
	}
	bzero((char *)&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(params->port);
	
	ret = bind(global_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if(ret == -1){
		perror("Error en el bind:");
		return -1;
	}
	listen(global_sock, 20);

	f = fopen(params->file, "a+");
	if(f != NULL){
		gethostname(host, 255);
		fprintf(f, "%s %s %d\r\n", params->name, params->host, params->port);	
		fclose(f);
	}
        debug_printf("[COMM]begin mpiServer_comm_init(%s, %d, %s)\n",params->name, params->port, params->file);
	return 0;
}

void mpiServer_close_comm ( void )
{
	close(global_sock);
}

int mpiServer_accept_comm ( void )
{
	struct sockaddr_in client_addr;
	int sc, flag = 1;
	int size = sizeof(struct sockaddr_in);

        debug_printf("[COMM]begin mpiServer_accept_comm()\n");

	debug_printf("[COMM]antes accept conection ....\n");	
	sc = accept(global_sock, (struct sockaddr *)&client_addr, &size);	
	debug_printf("[COMM]desp. accept conection .... %d\n",sc);	

	if(sc == -1){
		perror("accept: ");
	}	

	//printf("[COMM]accept conection ....\n");	
	if (setsockopt (sc, IPPROTO_TCP,TCP_NODELAY, &flag, sizeof(flag)) == -1){
		perror("setsockopt: ");
		return -1;
	}

	//NEW
	int val = 1024 * 1024; //1 MB

	if (setsockopt(sc, SOL_SOCKET, SO_SNDBUF, (char *) &val, sizeof(int)) == -1){
		perror("setsockopt: ");
		return -1;
	}

	val = 1024 * 1024; //1 MB
	if (setsockopt(sc, SOL_SOCKET, SO_RCVBUF, (char *) &val, sizeof(int)) == -1){
		perror("setsockopt: ");
		return -1;
	}

        debug_printf("[COMM]end mpiServer_accept_comm()\n");
	return sc;
}

int mpiServer_destroy_comm(){
	return 0;
}


/********************************** 
Write data to client using sockets
***********************************/
ssize_t mpiServer_comm_writedata(int fd, char *data, ssize_t size, char *id){
	int ret = 0;
	int cont = 0;

#ifdef DEBUG
	mpiServer_comm_writedata_test(fd, id);
#endif	
	debug_printf("[COMM]server: begin write_comm(%d): %d data %p ID=%s:%p --th:%d--\n",fd,size,data,id,id,(int)pthread_self());

	if(size == 0){
		return  0;
	}
	
	if(size < 0){
		return  -1;
	}
	
	do{
	        debug_printf("[COMM]server:write_comm(%d) antes: %d = %d data %p ID=%s:%p --th:%d--\n",fd,size,ret,data,id,id,(int)pthread_self());
		ret = write(fd, data+cont, size-cont);
	        debug_printf("[COMM]server:write_comm(%d) desp: %d = %d data %p ID=%s:%p --th:%d--\n",fd,size,ret,data,id,id,(int)pthread_self());
		if(ret < 0){
			perror("server: Error write_comm:");
		}
		cont += ret;
	}while((ret>0)&&(cont!=size));

	
	if(ret == -1){
		debug_printf("[COMM]server: Error write_comm(%d): -1 ID=%s:%p\n",fd,id,id);
		return ret;
	}
	debug_printf("[COMM]server: end write_comm(%d): %d of %d data %p ID=%s:%p --th:%d--\n",fd,cont,size,data,id,id,(int)pthread_self());

#ifdef DEBUG
	mpiServer_comm_writedata_test(fd, id);
#endif	
	return size;
}


/********************************** 
Read data from client using sockets
***********************************/
ssize_t mpiServer_comm_readdata(int fd, char *data, ssize_t size, char *id){
	int ret = 0;
	int cont = 0;

#ifdef DEBUG
	mpiServer_comm_readdata_test(fd, id);
#endif	

	debug_printf("[COMM]server: begin read_comm(%d): %d data %p ID=%s:%p --th:%d--\n",fd,size,data,id,id,(int)pthread_self());
	if(size == 0){
		return  0;
	}
	
	if(size < 0){
		return  -1;
	}
	
	do{
	        debug_printf("[COMM]server:read_comm(%d) antes: %d = %d data %p ID=%s:%p --th:%d--\n",fd,size,ret,data,id,id,(int)pthread_self());
	        ret = read(fd, data+cont, size-cont);
	        debug_printf("[COMM]server:read_comm(%d) desp: %d = %d data %p ID=%s:%p --th:%d--\n",fd,size,ret,data,id,id,(int)pthread_self());
		if(ret < 0){
			perror("server: Error read_comm:");
		}
		cont += ret;
	}while((ret>0)&&(cont!=size));
	

	if(ret == -1){
		debug_printf("[COMM]server: Error read_comm(%d): -1 ID=%s:%p\n",fd,id,id);
		return ret;
	}
	debug_printf("[COMM]server: end read_comm(%d): %d of %d data %p ID=%s:%p --th:%d--\n",fd,cont,size,data,id,id,(int)pthread_self());

#ifdef DEBUG
	mpiServer_comm_readdata_test(fd, id);
#endif	

	return size;
}

