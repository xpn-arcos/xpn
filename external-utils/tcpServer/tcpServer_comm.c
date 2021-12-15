#include "tcpServer_comm.h"

static int global_sock = 0;



int tcpServer_comm_writedata_test(int fd, char *id){
        /*****************************TEST****************************************/
	int ret;
        char buffer_temp[CONST_TEMP], aux;
        int i;

#ifdef DBG_COMM
        printf("[COMM] ===init write test ID=%s:%p --th:%d--===\n",id,id,(int)pthread_self());
#endif
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
#ifdef DBG_COMM
        printf("[COMM] send write test ok ID=%s\n",id);
#endif

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
#ifdef DBG_COMM
        printf("[COMM] receive write test ok ID=%s:%p\n",id,id);
#endif
        for (i=0;i<CONST_TEMP;i++){
		aux = (char)(i%128);
                if(buffer_temp[i] != aux){
                        printf("[COMM]server:ERROR TEST(3) write_data(%d): err %d(%d!=%d) ID=%s:%p --th:%d--\n",fd,ret,buffer_temp[i],aux,id,id,(int)pthread_self());
			perror("[COMM]server:ERROR TEST(3) write");
                }
        }
#ifdef DBG_COMM
        printf("[COMM] ===check write test ok ID=%s:%p--th:%d--===\n",id,id,(int)pthread_self());
#endif
        /*****************************TEST****************************************/
        return 0;
}


int tcpServer_comm_readdata_test(int fd, char *id){

        /*****************************TEST****************************************/
	int ret;
        char buffer_temp[CONST_TEMP], aux;
        int i;


	bzero(buffer_temp, CONST_TEMP);
#ifdef DBG_COMM
        printf("[COMM] ===init read test ok ID=%s:%p --th:%d--===\n",id,id,(int)pthread_self());
#endif

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
#ifdef DBG_COMM
        printf("[COMM] receive read test ok ID=%s:%p\n",id,id);
#endif
        for (i=0;i<CONST_TEMP;i++){
		aux = (char)(i%128);
                if(buffer_temp[i] != aux){
                        printf("[COMM]server:ERROR TEST(2) write_data(%d): err %d(%d!=%d)  ID=%s:%p --th:%d--\n",fd,ret,buffer_temp[i],aux,id,id,(int)pthread_self());
                        //printf("[COMM]server:ERROR TEST(2) read_data(%d): err %d  ID=%s:%p\n",fd,ret,id,id);
			perror("[COMM]server:ERROR TEST(2) read");
                }
        }
#ifdef DBG_COMM
        printf("[COMM] check read test ok ID=%s:%p --th:%d--\n",id,id,(int)pthread_self());
#endif

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
#ifdef DBG_COMM
        printf("[COMM] ===send read test ok ID=%s:%p--th:%d--===\n",id,id,(int)pthread_self());
#endif
        /*****************************TEST****************************************/
        return 0;
}




/* AUXILIAR FUNCTIONS */
int tcpServer_comm_init(char *name, int port, char *file){
	struct sockaddr_in server_addr;
	int val, ret;
	char host[255];
	FILE *f;
#ifdef  DBG_COMM	
  printf("[COMM]begin tcpServer_comm_init(%s, %d, %s)\n",name, port, file);
#endif
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
	server_addr.sin_port = htons(port);
	
	ret = bind(global_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if(ret == -1){
		perror("Error en el bind:");
		return -1;
	}
	listen(global_sock, 20);

	f = fopen(file, "a+");
	if(f != NULL){
		gethostname(host, 255);
		fprintf(f, "%s %s %d\r\n", name, host, port);	
		fclose(f);
	}
#ifdef  DBG_COMM	
  printf("[COMM]begin tcpServer_comm_init(%s, %d, %s)\n",name, port, file);
#endif
	return 0;
}

void tcpServer_close_comm(){
	close(global_sock);
}


int tcpServer_accept_comm(){
	struct sockaddr_in client_addr;
	int sc, flag = 1;
	int size = sizeof(struct sockaddr_in);
        #ifdef DBG_COMM
 printf("[COMM]begin tcpServer_accept_comm()\n");
        #endif



        #ifdef DBG_COMM
		printf("[COMM]antes accept conection ....\n");	
        #endif
	sc = accept(global_sock, (struct sockaddr *)&client_addr, &size);	
        #ifdef DBG_COMM
		printf("[COMM]desp. accept conection .... %d\n",sc);	
        #endif

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

        #ifdef DBG_COMM
 printf("[COMM]end tcpServer_accept_comm()\n");
        #endif
	return sc;
}

int tcpServer_destroy_comm(){
	return 0;
}


/********************************** 
Write data to client using sockets
***********************************/
ssize_t tcpServer_comm_writedata(int fd, char *data, ssize_t size, char *id){
	int ret = 0;
	int cont = 0;
#ifdef DBG_COMM	
	tcpServer_comm_writedata_test(fd, id);
#endif	

#ifdef DBG_COMM	
	printf("[COMM]server: begin write_comm(%d): %d data %p ID=%s:%p --th:%d--\n",fd,size,data,id,id,(int)pthread_self());
#endif	

	if(size == 0){
		return  0;
	}
	
	if(size < 0){
		return  -1;
	}
	
	do{
#ifdef DBG_COMM	
	printf("[COMM]server:write_comm(%d) antes: %d = %d data %p ID=%s:%p --th:%d--\n",fd,size,ret,data,id,id,(int)pthread_self());
#endif	
		ret = write(fd, data+cont, size-cont);
#ifdef DBG_COMM	
	printf("[COMM]server:write_comm(%d) desp: %d = %d data %p ID=%s:%p --th:%d--\n",fd,size,ret,data,id,id,(int)pthread_self());
#endif	
		if(ret < 0){
			perror("server: Error write_comm:");
		}
		cont += ret;
	}while((ret>0)&&(cont!=size));

	
	if(ret == -1){
#ifdef DBG_COMM
		printf("[COMM]server: Error write_comm(%d): -1 ID=%s:%p\n",fd,id,id);
#endif		
		return ret;
	}
#ifdef DBG_COMM	
	printf("[COMM]server: end write_comm(%d): %d of %d data %p ID=%s:%p --th:%d--\n",fd,cont,size,data,id,id,(int)pthread_self());
#endif
#ifdef DBG_COMM	
	tcpServer_comm_writedata_test(fd, id);
#endif	
	return size;
}


/********************************** 
Read data from client using sockets
***********************************/
ssize_t tcpServer_comm_readdata(int fd, char *data, ssize_t size, char *id){
	int ret = 0;
	int cont = 0;
#ifdef DBG_COMM	
	tcpServer_comm_readdata_test(fd, id);
#endif	

#ifdef DBG_COMM	
	printf("[COMM]server: begin read_comm(%d): %d data %p ID=%s:%p --th:%d--\n",fd,size,data,id,id,(int)pthread_self());
#endif	
	
	if(size == 0){
		return  0;
	}
	
	if(size < 0){
		return  -1;
	}
	
	do{
#ifdef DBG_COMM	
	printf("[COMM]server:read_comm(%d) antes: %d = %d data %p ID=%s:%p --th:%d--\n",fd,size,ret,data,id,id,(int)pthread_self());
#endif	
		ret = read(fd, data+cont, size-cont);
#ifdef DBG_COMM	
	printf("[COMM]server:read_comm(%d) desp: %d = %d data %p ID=%s:%p --th:%d--\n",fd,size,ret,data,id,id,(int)pthread_self());
#endif	
		if(ret < 0){
			perror("server: Error read_comm:");
		}
		cont += ret;
	}while((ret>0)&&(cont!=size));
	

	if(ret == -1){
#ifdef DBG_COMM	
		printf("[COMM]server: Error read_comm(%d): -1 ID=%s:%p\n",fd,id,id);
#endif	
		return ret;
	}
#ifdef DBG_COMM	
	printf("[COMM]server: end read_comm(%d): %d of %d data %p ID=%s:%p --th:%d--\n",fd,cont,size,data,id,id,(int)pthread_self());
#endif	

#ifdef DBG_COMM	
	tcpServer_comm_readdata_test(fd, id);
#endif	
	return size;
}

