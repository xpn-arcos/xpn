
/*
 *  Copyright 2020-2023 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos
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


  /* ... Include / Inclusion ........................................... */

     #include "tcp_server_comm.h"


  /* ... Global Variable / Variable Globales ........................... */

struct tcp_server_node_st{
	char host[255];
	int  port;
	char name[255];
};

static int load = 0;
static struct tcp_server_node_st tcp_server_node[MAX_TCP_SERVER_NODES];
static int num_tcp_server_nodes = 0;


  /* ... Functions / Funciones ......................................... */

   void tcp_server_readFile()
   {
	FILE *fd;
	char *name = NULL;

	debug_info("[NFI_COMM]begin the translation\n");

	name = getenv(TCP_SERVER_FILE);
	if((name == NULL)|| (strcmp(name, "") == 0)){
		name = TCP_SERVER_FILE_DEFAULT;
	}

	fd = fopen(name,"r");
	if(fd == NULL){
		fprintf(stderr,"tcp_server_readFile: can't open %s\n",name);
		exit(-1);
	}

	while(EOF != fscanf(fd,"%s %s %d",
		tcp_server_node[num_tcp_server_nodes].name,
		tcp_server_node[num_tcp_server_nodes].host,
		&tcp_server_node[num_tcp_server_nodes].port))
	{
			/*
			printf("[NFI_COMM]-%d> %s %s %d -\n",
			num_tcp_server_nodes,
			tcp_server_node[num_tcp_server_nodes].name,
			tcp_server_node[num_tcp_server_nodes].host,
			tcp_server_node[num_tcp_server_nodes].port);
			*/
			num_tcp_server_nodes++;

			if(num_tcp_server_nodes >= MAX_TCP_SERVER_NODES){
				fprintf(stderr,"Error: num_tcp_server_nodes >= MAX_TCP_SERVER_NODES\n");
				exit(0);
			}
	}
	fclose(fd);

	debug_info("[NFI_COMM]end the translation\n");
   }


   void tcp_server_translate(char *server, char *newserver, int *port)
   {
        int i;

	/*************************************/
	debug_info("[NFI_COMM]Buscando 1 ... %s\n",server);
	/* DON'T WORK WITH THREADS */
	if(!load){
		load = 1;
		debug_info("[NFI_COMM]Cargando Fichero ... \n");
		tcp_server_readFile();
	}

	/*************************************/
	debug_info("[NFI_COMM]Buscando 2 ... %s\n",server);
	for(i=0;i<num_tcp_server_nodes;i++)
	{
		if(strcmp(server, tcp_server_node[i].name) == 0)
		{
			strcpy(newserver, tcp_server_node[i].host);

			 /*
			 printf("[NFI_COMM]Encontrado ... %s %d\n",
				tcp_server_node[i].host,
				tcp_server_node[i].port);
				*/
			 debug_info("[NFI_COMM]Encontrado ... %s %d\n",
				tcp_server_node[i].host,
				tcp_server_node[i].port);

			*port = tcp_server_node[i].port;
			break;
		}
	}

	if(i == num_tcp_server_nodes){
		fprintf(stderr,"translate: error %s not found (%d)\n",server,num_tcp_server_nodes);
		exit(-1);
	}
   }


   int tcp_server_write_data_test(int fd, char *id)
   {
	/*****************************TEST****************************************/
	int ret;
	char buffer_temp[CONST_TEMP], aux;
	int i;

	debug_info("[NFI_COMM] ===init write test ID=%s --th:%d--===\n",id,(int)pthread_self());
	for (i=0;i<CONST_TEMP;i++){
		aux = (char)(i%128);
		buffer_temp[i] = aux;
	}

	//ret = write(fd, buffer_temp, CONST_TEMP); //TODO

	ssize_t (*real_write)(int, const void*, size_t);
	real_write = (ssize_t (*)(int, const void*, size_t)) dlsym(RTLD_NEXT,"write");
	ret = real_write(fd, buffer_temp, CONST_TEMP);

	if(ret != CONST_TEMP ){
		printf("[NFI_COMM]client:ERROR TEST(1) write_data(%d): err %d  ID=%s --th:%d--\n",fd,ret,id,(int)pthread_self());
		perror("[NFI_COMM]client:ERROR TEST(1) write");
			exit(-1);
	}
	debug_info("[NFI_COMM] send write test ok ID=%s\n",id);
	bzero(buffer_temp, CONST_TEMP);
	//ret = read(fd, buffer_temp, CONST_TEMP); //TODO

	ssize_t (*real_read)(int, void*, size_t);
	real_read = (ssize_t (*)(int, void*, size_t)) dlsym(RTLD_NEXT,"read");
	ret = real_read(fd, buffer_temp, CONST_TEMP);


	if(ret != CONST_TEMP )
	{
		printf("[NFI_COMM]client:ERROR TEST(2) write_data(%d): err %d  ID=%s --th:%d--\n",fd,ret,id,(int)pthread_self());
		perror("[NFI_COMM]client:ERROR TEST(2) write");
		exit(-1);
	}
	debug_info("[NFI_COMM] receive test ok ID=%s\n",id);
	for (i=0;i<CONST_TEMP;i++)
	{
		aux = (char)(i%128);
		if(buffer_temp[i] != aux){
			printf("[NFI_COMM]client:ERROR TEST(3) write_data(%d): err %d buffer_temp[%d] = %d ID=%s --th:%d--\n",fd,ret, i, buffer_temp[i], id,(int)pthread_self());
			perror("[NFI_COMM]client:ERROR TEST(3) write");
			exit(-1);
		}
	}
	debug_info("[NFI_COMM] ===check write test ok ID=%s --th:%d--===\n",id,(int)pthread_self());
	/*****************************TEST****************************************/

	return 0;
}


/*****************************TEST****************************************/
int tcp_server_read_data_test(int fd, char *id)
{
	int ret;
	char buffer_temp[CONST_TEMP], aux;
	int i;

	debug_info("[NFI_COMM] ===init read test ID=%s --th:%d--===\n",id,(int)pthread_self());

	bzero(buffer_temp, CONST_TEMP);

	//ret = read(fd, buffer_temp, CONST_TEMP); //TODO

	ssize_t (*real_read)(int, void*, size_t);
	real_read = (ssize_t (*)(int, void*, size_t)) dlsym(RTLD_NEXT,"read");
	ret = real_read(fd, buffer_temp, CONST_TEMP);

	if(ret != CONST_TEMP ){
		printf("[NFI_COMM]client:ERROR TEST(1) read_data(%d): err %d  ID=%s --th:%d--\n",fd,ret,id,(int)pthread_self());
		perror("[NFI_COMM]client:ERROR TEST(1) read");
		exit(-1);
	}
	debug_info("[NFI_COMM] receive read test ok ID=%s --th:%d--\n",id,(int)pthread_self());
	for (i=0;i<CONST_TEMP;i++){
		aux = (char)(i%128);
		if(buffer_temp[i] != aux ){
			printf("[NFI_COMM]client:ERROR TEST(2) read_data(%d): err %d buffer_temp[%d] = %d ID=%s --th:%d--\n",fd,ret, i, buffer_temp[i], id,(int)pthread_self());
			perror("[NFI_COMM]client:ERROR TEST(2) read");
			exit(-1);
		}
	}
	debug_info("[NFI_COMM] check read test ok ID=%s --th:%d--\n",id,(int)pthread_self());

	for (i=0;i<CONST_TEMP;i++){
		aux = (char)(i%128);
		buffer_temp[i] = aux;
	}

	//ret = write(fd, buffer_temp, CONST_TEMP); //TODO

	ssize_t (*real_write)(int, const void*, size_t);
	real_write = (ssize_t (*)(int, const void*, size_t)) dlsym(RTLD_NEXT,"write");
	ret = real_write(fd, buffer_temp, CONST_TEMP);


	if(ret != CONST_TEMP ){
		printf("[NFI_COMM]client:ERROR TEST(3) read_data(%d): err %d  ID=%s --th:%d--\n",fd,ret,id,(int)pthread_self());
		perror("[NFI_COMM]client:ERROR TEST(3) read");
		exit(-1);
	}
	debug_info("[NFI_COMM] ===send read test ok ID=%s --th:%d--===\n",id,(int)pthread_self());
	/*****************************TEST****************************************/
	return 0;
}


/*********************************************************************/
int tcp_server_connect(char *server)
{
	struct hostent *hp;
	struct sockaddr_in server_addr;
	int port, sd, ret;
	char newserver[PATH_MAX];
	int flag = 1;

	/**************************************************
	if(1){
		struct hostent *hp;
		hp = gethostbyname ("localhost");
		if(hp == NULL){
			perror("Error gethostbyname:");
		}else{
			perror("Ok gethostbyname:");
		}
	}
	**************************************************/

	bzero(newserver, PATH_MAX);
	//debug_info("[NFI_COMM]----TRANSLATE server = %s URL = %s\n",server, url);
	tcp_server_translate(server, newserver, &port);
	debug_info("[NFI_COMM]----SERVER = %s NEWSERVER = %s PORT = %d\n",server, newserver, port);

	sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sd <0){
		perror("socket:");
		return -1;
	}
	debug_info("[NFI_COMM]----SERVER = %s NEWSERVER = %s PORT = %d ==> %d\n",server, newserver, port, sd);

	if (setsockopt (sd, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag)) == -1){
		perror("setsockopt: ");
		return -1;
	}

	//NEW

	int val = 1024 * 1024; //1 MB

	if (setsockopt(sd, SOL_SOCKET, SO_SNDBUF, (char *) &val, sizeof(int)) == -1){
		perror("setsockopt: ");
		return -1;
	}

	val = 1024 * 1024; //1 MB
	if (setsockopt(sd, SOL_SOCKET, SO_RCVBUF, (char *) &val, sizeof(int)) == -1){
		perror("setsockopt: ");
		return -1;
	}

	/**************************************************/

	hp = gethostbyname (newserver);
	if(hp == NULL){
		//tcp_server_err(TCP_SERVERERR_MEMORY);

		fprintf(stderr,"nfi_tcp_server_init: error gethostbyname %s (%s,%d)\n",
				server, newserver, port);
		return -1;
	}
	debug_info("[NFI_COMM]server = %s-%d\n",newserver,port);
	bzero((char *)&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	memcpy (&(server_addr.sin_addr), hp->h_addr, hp->h_length);
      //server_addr.sin_port = htons(TCP_SERVER_PORT);
	server_addr.sin_port = htons(port);
	debug_info("[NFI_COMM]Antes de connect to %s\n",newserver);

	//se establece la conexi�n
	ret = connect(sd, (struct sockaddr *) &server_addr, sizeof(server_addr));
	debug_info("[NFI_COMM]%s)connect(%s,%d) = %d\n",server,newserver,port,ret);
	if(ret == -1){
		//tcp_server_err(TCP_SERVERERR_MEMORY);
		fprintf(stderr,"nfi_tcp_server_init: error in connect %s (%s,%d)\n",
				server, newserver, port);
			perror("nfi_tcp_server_init:");
		return -1;
	}

	return sd;
}


ssize_t tcp_server_write_data(int fd, char *data, ssize_t size, char *id)
{
	int ret = 0;
	int cont = 0;

#ifdef DBG_COMM
	tcp_server_write_data_test(fd, id);
#endif

	debug_info("[NFI_COMM]client: write_data(%d): %lu ID=%s --th:%d--\n", fd, (unsigned long)size, id, (int)pthread_self());

	if(size == 0){
		return  0;
	}

	if(size < 0){
		return  -1;
	}

	do{
		//ret = write(fd, data+cont, size-cont); //TODO

		ssize_t (*real_write)(int, const void*, size_t);
		real_write = (ssize_t (*)(int, const void*, size_t)) dlsym(RTLD_NEXT,"write");
		ret = real_write(fd, data+cont, size-cont);

//		printf("[NFI]write COMM: -> size %d \n",ret);


	debug_info("[NFI_COMM]client: write_data(%d): %lu = %d ID=%s --th:%d--\n", fd, (unsigned long)size, ret, id, (int)pthread_self());
                if(ret <= 0){
                        perror("client: Error write_comm:");
                }
		cont += ret;
	}while((ret>0)&&(cont!=size));

	if(ret == -1){
		debug_info("[NFI_COMM]client: write_data(%d): err %d  ID=%s --th:%d--\n", fd, ret, id, (int)pthread_self());
		perror("client: write_data");
		return ret;
	}

	debug_info("[NFI_COMM]client: write_data(%d): %d de %lu ID=%s --th:%d--\n", fd, cont, (unsigned long)size, id, (int)pthread_self());

#ifdef DBG_COMM
	tcp_server_write_data_test(fd, id);
#endif

	return size;
}


ssize_t tcp_server_read_data(int fd, char *data, ssize_t size, char *id)
{
	int ret = 0;
	int cont = 0;


#ifdef DBG_COMM
	tcp_server_read_data_test(fd, id);
#endif

	debug_info("[NFI_COMM]client: read_data(%d): %lu ID=%s --th:%d--\n",fd,(unsigned long)size,id,(int)pthread_self());

	if(size == 0){
		return  0;
	}

	if(size < 0){
		return  -1;
	}

	do{
		//ret = read(fd, data+cont, size-cont); //TODO

//		printf("REQ PRE READ %s\n", data);

		ssize_t (*real_read)(int, void*, size_t);
		real_read = (ssize_t (*)(int, void*, size_t)) dlsym(RTLD_NEXT,"read");
		ret = real_read(fd, data+cont, size-cont);

//		printf("REQ POST READ %s -- RET %d\n", data, ret);


		debug_info("[NFI_COMM]client: read_data(%d): %lu = %d ID=%s --th:%d--\n",fd,(unsigned long)size,ret,id,(int)pthread_self());
                if(ret <= 0){
                        perror("client: Error read_comm:");
                }
		cont += ret;

//		printf("CONT %d -- SIZE %d\n", cont, size);

	}while((ret>0)&&(cont!= size));

	if(ret == -1){
		debug_info("[NFI_COMM]client: read_data(%d): err %d  ID=%s --th:%d--\n",fd,ret,id,(int)pthread_self());
		perror("client: read_data");
		return ret;
	}

	debug_info("[NFI_COMM]client: read_data(%d): %d de %lu ID=%s --th:%d--\n",fd,cont,(unsigned long)size,id,(int)pthread_self());

#ifdef DBG_COMM
	tcp_server_read_data_test(fd, id);
#endif

	return size;
}


