#include "myServer_comm.h"


/*****************************/
struct myServer_node_st{
	char host[255];
	int  port;
	char name[255];
};



static int load = 0;
static struct myServer_node_st myServer_node[MAX_MYSERVER_NODES];
static int num_myServer_nodes = 0;
/****************************/


	
void myServer_readFile(){

	FILE *fd;
	char *name = NULL;
	
#ifdef DBG_COMM
	printf("[NFI_COMM]begin the translation\n");
#endif
	name = getenv(MYSERVER_FILE);
	if((name == NULL)|| (strcmp(name, "") == 0)){
		name = MYSERVER_FILE_DEFAULT;		
	}
	
	fd = fopen(name,"r");
	if(fd == NULL){
		fprintf(stderr,"myServer_readFile: can't open %s\n",name);
		exit(-1);
	}
	while(EOF != fscanf(fd,"%s %s %d",
		myServer_node[num_myServer_nodes].name,
		myServer_node[num_myServer_nodes].host,
		&myServer_node[num_myServer_nodes].port)){
			/*	
			printf("[NFI_COMM]-%d> %s %s %d -\n",
			num_myServer_nodes,
			myServer_node[num_myServer_nodes].name,
			myServer_node[num_myServer_nodes].host,
			myServer_node[num_myServer_nodes].port);
			*/
			num_myServer_nodes++;
			
			if(num_myServer_nodes >= MAX_MYSERVER_NODES){
				fprintf(stderr,"Error: num_myServer_nodes >= MAX_MYSERVER_NODES\n");
				exit(0);
			}
		}
		fclose(fd);
#ifdef DBG_COMM
	printf("[NFI_COMM]end the translation\n");
#endif
}


void myServer_translate(char *server, char *newserver, int *port){
    int i;
	
	/*************************************/
#ifdef DBG_COMM
	printf("[NFI_COMM]Buscando 1 ... %s\n",server);
#endif
	/* DON'T WORK WITH THREADS */
	if(!load){
		load = 1;
#ifdef DBG_COMM
		printf("[NFI_COMM]Cargando Fichero ... \n");
#endif
		myServer_readFile();
	}
	/*************************************/
#ifdef DBG_COMM
	printf("[NFI_COMM]Buscando 2 ... %s\n",server);
#endif
	for(i=0;i<num_myServer_nodes;i++){
		if(strcmp(server, myServer_node[i].name) == 0){
			strcpy(newserver, myServer_node[i].host);
			 
			 /*
			 printf("[NFI_COMM]Encontrado ... %s %d\n",
				myServer_node[i].host,
				myServer_node[i].port);
				*/
#ifdef DBG_COMM
			 printf("[NFI_COMM]Encontrado ... %s %d\n",
				myServer_node[i].host,
				myServer_node[i].port);
#endif
			
			*port = myServer_node[i].port;
			break;
		}
	}
	if(i == num_myServer_nodes){
		fprintf(stderr,"translate: error %s not found (%d)\n",server,num_myServer_nodes);
		exit(-1);
	}
}


int myServer_write_data_test(int fd, char *id){
	/*****************************TEST****************************************/
	int ret;	
	char buffer_temp[CONST_TEMP], aux;
	int i;	

#ifdef DBG_COMM
	printf("[NFI_COMM] ===init write test ID=%s --th:%d--===\n",id,(int)pthread_self());
#endif
	for (i=0;i<CONST_TEMP;i++){
		aux = (char)(i%128);
		buffer_temp[i] = aux;
	}

	ret = write(fd, buffer_temp, CONST_TEMP); 
	if(ret != CONST_TEMP ){
		printf("[NFI_COMM]client:ERROR TEST(1) write_data(%d): err %d  ID=%s --th:%d--\n",fd,ret,id,(int)pthread_self());
		perror("[NFI_COMM]client:ERROR TEST(1) write");
			exit(-1);
	}
#ifdef DBG_COMM
	printf("[NFI_COMM] send write test ok ID=%s\n",id);
#endif
	bzero(buffer_temp, CONST_TEMP);	
	ret = read(fd, buffer_temp, CONST_TEMP); 
	if(ret != CONST_TEMP ){
		printf("[NFI_COMM]client:ERROR TEST(2) write_data(%d): err %d  ID=%s --th:%d--\n",fd,ret,id,(int)pthread_self());
		perror("[NFI_COMM]client:ERROR TEST(2) write");
			exit(-1);
	}
#ifdef DBG_COMM
	printf("[NFI_COMM] receive test ok ID=%s\n",id);
#endif
	for (i=0;i<CONST_TEMP;i++){
		aux = (char)(i%128);
		if(buffer_temp[i] != aux){
			printf("[NFI_COMM]client:ERROR TEST(3) write_data(%d): err %d buffer_temp[%d] = %d ID=%s --th:%d--\n",fd,ret, i, buffer_temp[i], id,(int)pthread_self());
			perror("[NFI_COMM]client:ERROR TEST(3) write");
			exit(-1);
		}
	}
#ifdef DBG_COMM
	printf("[NFI_COMM] ===check write test ok ID=%s --th:%d--===\n",id,(int)pthread_self());
#endif
	/*****************************TEST****************************************/
	return 0;
}




int myServer_read_data_test(int fd, char *id){

	/*****************************TEST****************************************/
	int ret;	
	char buffer_temp[CONST_TEMP], aux;
	int i;	

#ifdef DBG_COMM
	printf("[NFI_COMM] ===init read test ID=%s --th:%d--===\n",id,(int)pthread_self());
#endif
	
	bzero(buffer_temp, CONST_TEMP);	
	ret = read(fd, buffer_temp, CONST_TEMP); 

	if(ret != CONST_TEMP ){
		printf("[NFI_COMM]client:ERROR TEST(1) read_data(%d): err %d  ID=%s --th:%d--\n",fd,ret,id,(int)pthread_self());
		perror("[NFI_COMM]client:ERROR TEST(1) read");
		exit(-1);
	}
#ifdef DBG_COMM
	printf("[NFI_COMM] receive read test ok ID=%s --th:%d--\n",id,(int)pthread_self());
#endif
	for (i=0;i<CONST_TEMP;i++){
		aux = (char)(i%128);
		if(buffer_temp[i] != aux ){
			printf("[NFI_COMM]client:ERROR TEST(2) read_data(%d): err %d buffer_temp[%d] = %d ID=%s --th:%d--\n",fd,ret, i, buffer_temp[i], id,(int)pthread_self());
			perror("[NFI_COMM]client:ERROR TEST(2) read");
			exit(-1);
		}
	}
#ifdef DBG_COMM
	printf("[NFI_COMM] check read test ok ID=%s --th:%d--\n",id,(int)pthread_self());
#endif

	for (i=0;i<CONST_TEMP;i++){
		aux = (char)(i%128);
		buffer_temp[i] = aux;
	}

	ret = write(fd, buffer_temp, CONST_TEMP); 
	if(ret != CONST_TEMP ){
		printf("[NFI_COMM]client:ERROR TEST(3) read_data(%d): err %d  ID=%s --th:%d--\n",fd,ret,id,(int)pthread_self());
		perror("[NFI_COMM]client:ERROR TEST(3) read");
		exit(-1);
	}
#ifdef DBG_COMM
	printf("[NFI_COMM] ===send read test ok ID=%s --th:%d--===\n",id,(int)pthread_self());
#endif
	/*****************************TEST****************************************/
	return 0;
}

/*********************************************************************/
int myServer_connect(char *server){

	struct hostent *hp;
	struct sockaddr_in server_addr;
	int port, sd, ret; 
	char newserver[MAXPATHLEN];

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

	bzero(newserver, MAXPATHLEN);
#ifdef DBG_COMM
	//printf("[NFI_COMM]----TRANSLATE server = %s URL = %s\n",server, url);	
#endif	
	myServer_translate(server, newserver, &port);	
#ifdef DBG_COMM
	printf("[NFI_COMM]----SERVER = %s NEWSERVER = %s PORT = %d\n",server, newserver, port);	
#endif	
	

	sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sd <0){
		perror("socket:");
		return -1;
	}
#ifdef DBG_COMM
	printf("[NFI_COMM]----SERVER = %s NEWSERVER = %s PORT = %d ==> %d\n",server, newserver, port, sd);	
#endif	
	
	if (setsockopt (sd, IPPROTO_TCP, 
		TCP_NODELAY, &flag, sizeof(flag)) == -1){
		return -1;
	}
	
	/**************************************************/

	hp = gethostbyname (newserver);
	if(hp == NULL){
		//myServer_err(MYSERVERERR_MEMORY);
		
		fprintf(stderr,"nfi_myServer_init: error gethostbyname %s (%s,%d)\n",
				server, newserver, port);
		return -1;
	}
#ifdef DBG_COMM
	//printf("[NFI_COMM]server = %s-%d-%p\n",server,MYSERVER_PORT,hp);
	printf("[NFI_COMM]server = %s-%d\n",newserver,port);
#endif	
	bzero((char *)&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;					
	memcpy (&(server_addr.sin_addr), hp->h_addr, hp->h_length);			
	//server_addr.sin_port = htons(MYSERVER_PORT);
	server_addr.sin_port = htons(port);
#ifdef DBG_COMM
	printf("[NFI_COMM]Antes de connect to %s\n",newserver);
#endif	
	//se establece la conexión
	ret = connect(sd, (struct sockaddr *) &server_addr, sizeof(server_addr));
#ifdef DBG_COMM
	printf("[NFI_COMM]%s)connect(%s,%d) = %d\n",server,newserver,port,ret);
#endif	
	if(ret == -1){
		//myServer_err(MYSERVERERR_MEMORY);
		fprintf(stderr,"nfi_myServer_init: error in connect %s (%s,%d)\n",
				server, newserver, port);
			perror("nfi_myServer_init:");
		return -1;
	}
	return sd;
}

int myServer_write_data(int fd, char *data, ssize_t size, char *id){
	int ret = 0;
	int cont = 0;

#ifdef DBG_COMM
	myServer_write_data_test(fd, id);
#endif

	
#ifdef DBG_COMM
	printf("[NFI_COMM]client: write_data(%d): %d ID=%s --th:%d--\n",fd,size,id,(int)pthread_self());
#endif

	if(size == 0){
		return  0;
	}
	
	if(size < 0){
		return  -1;
	}

	do{
		ret = write(fd, data+cont, size-cont);
#ifdef DBG_COMM
	printf("[NFI_COMM]client: write_data(%d): %d = %d ID=%s --th:%d--\n",fd,size,ret,id,(int)pthread_self());
#endif
                if(ret <= 0){                                                                                               
                        perror("client: Error write_comm:");                                                                 
                }
		cont += ret;
	}while((ret>0)&&(cont!=size));
	
	if(ret == -1){
#ifdef DBG_COMM
		printf("[NFI_COMM]client: write_data(%d): err %d  ID=%s --th:%d--\n",fd,ret,id,(int)pthread_self());
#endif	
		perror("client: write_data");
		return ret;
	}
#ifdef DBG_COMM
	printf("[NFI_COMM]client: write_data(%d): %d de %d ID=%s --th:%d--\n",fd,cont,size,id,(int)pthread_self());
#endif	
#ifdef DBG_COMM
	myServer_write_data_test(fd, id);
#endif
	return size;
}

int myServer_read_data(int fd, char *data, ssize_t size, char *id){
	        
	int ret = 0;
	int cont = 0;


#ifdef DBG_COMM
	myServer_read_data_test(fd, id);
#endif


#ifdef DBG_COMM
	printf("[NFI_COMM]client: read_data(%d): %d ID=%s --th:%d--\n",fd,size,id,(int)pthread_self());
#endif

	if(size == 0){
		return  0;
	}
	
	if(size < 0){
		return  -1;
	}

	do{
		ret = read(fd, data+cont, size-cont);
#ifdef DBG_COMM
	printf("[NFI_COMM]client: read_data(%d): %d = %d ID=%s --th:%d--\n",fd,size,ret,id,(int)pthread_self());
#endif
                if(ret <= 0){                                                                                               
                        perror("client: Error read_comm:");                                                                 
                }
		cont += ret;
	}while((ret>0)&&(cont!= size));
	if(ret == -1){
#ifdef DBG_COMM
		printf("[NFI_COMM]client: read_data(%d): err %d  ID=%s --th:%d--\n",fd,ret,id,(int)pthread_self());
#endif	
		perror("client: read_data");
		return ret;
	}
#ifdef DBG_COMM
	printf("[NFI_COMM]client: read_data(%d): %d de %d ID=%s --th:%d--\n",fd,cont,size,id,(int)pthread_self());
#endif	
#ifdef DBG_COMM
	myServer_read_data_test(fd, id);
#endif
	return size;
}



