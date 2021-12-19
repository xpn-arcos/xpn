
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

      #include "mpiServer_comm.h"
      #include <dlfcn.h>


   /* ... Constants / Constantes ........................................ */

      /* ENVIROMENT VARIABLE: DNS SERVICE */
      #define MPISERVER_FILE "MPISERVER_FILE"
      #define MPISERVER_FILE_DEFAULT "/etc/xpn/mpiServer.dns"

      /* Nuevo */
      #define CONST_TEMP              1024
      #define MAX_MPISERVER_NODES      256


   /* ... Global variables / Variables globales ......................... */

      struct mpiServer_node_st
      {
      	      char host[255];
	      int  port;
	      char name[255];
      };

      static int load = 0;
      static struct mpiServer_node_st mpiServer_node[MAX_MPISERVER_NODES];
      static int num_mpiServer_nodes = 0;


   /* ... Functions / Funciones ......................................... */

      /*
       * Debug
       */

#ifdef DBG_COMM
  #define dbgcomm_error(...)    fprintf(stderr, __VA_ARGS__)
  #define dbgcomm_warning(...)  fprintf(stderr, __VA_ARGS__)
  #define dbgcomm_info(...)     fprintf(stdout, __VA_ARGS__)
#else
  #define dbgcomm_error(...)
  #define dbgcomm_warning(...)
  #define dbgcomm_info(...)
#endif

      /*
       * DNS
       */

      void mpiServer_readFile ( void )
      {
	FILE *fd;
	char *name = NULL;
	
	dbgcomm_info("[NFI_COMM] begin of reading DNS info\n");

	name = getenv(MPISERVER_FILE);
	if ((name == NULL)|| (strcmp(name, "") == 0)) {
	     name = MPISERVER_FILE_DEFAULT;		
	}
	
	fd = fopen(name,"r");
	if (fd == NULL)
	{
		fprintf(stderr,"mpiServer_readFile: can't open %s\n",name);
		exit(-1);
	}

	while(EOF != fscanf(fd,"%s %s %d",
		mpiServer_node[num_mpiServer_nodes].name,
		mpiServer_node[num_mpiServer_nodes].host,
		&mpiServer_node[num_mpiServer_nodes].port))
	{
			dbgcomm_info("[NFI_COMM]-%d> %s %s %d -\n",
			              num_mpiServer_nodes,
			              mpiServer_node[num_mpiServer_nodes].name,
			              mpiServer_node[num_mpiServer_nodes].host,
			              mpiServer_node[num_mpiServer_nodes].port);

			num_mpiServer_nodes++;
			if (num_mpiServer_nodes >= MAX_MPISERVER_NODES)
			{
				fprintf(stderr,"Error: num_mpiServer_nodes >= MAX_MPISERVER_NODES\n");
				exit(0);
			}
	}
	fclose(fd);

	dbgcomm_info("[NFI_COMM] end of reading DNS info\n");
      }

      void mpiServer_translate (char *server, char *newserver, int *port )
      {
        int i;
	
	dbgcomm_info("[NFI_COMM] Buscando 1 ... %s\n",server);
	/* DON'T WORK WITH THREADS */
	if (!load) {
	     load = 1;
	     dbgcomm_info("[NFI_COMM]Cargando Fichero ... \n");
	     mpiServer_readFile();
	}
	/*************************************/

	dbgcomm_info("[NFI_COMM] Buscando 2 ... %s\n",server);
	for (i=0; i<num_mpiServer_nodes; i++)
	{
		if (strcmp(server, mpiServer_node[i].name) == 0)
		{
			strcpy(newserver, mpiServer_node[i].host);
			dbgcomm_info("[NFI_COMM] Encontrado ... %s %d\n", mpiServer_node[i].host, mpiServer_node[i].port);
			*port = mpiServer_node[i].port;
			break;
		}
	}

	if (i == num_mpiServer_nodes) {
		fprintf(stderr,"translate: error %s not found (%d)\n",server,num_mpiServer_nodes);
		exit(-1);
	}
      }

      /*
       * Communication
       */

      int mpiServer_connect(char *server)
      {
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
	//dbgcomm_info("[NFI_COMM]----TRANSLATE server = %s URL = %s\n",server, url);	
	mpiServer_translate(server, newserver, &port);	
	dbgcomm_info("[NFI_COMM]----SERVER = %s NEWSERVER = %s PORT = %d\n",server, newserver, port);	
	

	sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sd <0){
		perror("socket:");
		return -1;
	}
	dbgcomm_info("[NFI_COMM]----SERVER = %s NEWSERVER = %s PORT = %d ==> %d\n",server, newserver, port, sd);	
	
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
		//mpiServer_err(MPISERVERERR_MEMORY);
		
		fprintf(stderr,"nfi_mpiServer_init: error gethostbyname %s (%s,%d)\n",
				server, newserver, port);
		return -1;
	}
	//dbgcomm_info("[NFI_COMM]server = %s-%d-%p\n",server,MPISERVER_PORT,hp);
	dbgcomm_info("[NFI_COMM]server = %s-%d\n",newserver,port);
	bzero((char *)&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;					
	memcpy (&(server_addr.sin_addr), hp->h_addr, hp->h_length);			
	//server_addr.sin_port = htons(MPISERVER_PORT);
	server_addr.sin_port = htons(port);
	dbgcomm_info("[NFI_COMM]Antes de connect to %s\n",newserver);
	//se establece la conexi�n
	ret = connect(sd, (struct sockaddr *) &server_addr, sizeof(server_addr));
	dbgcomm_info("[NFI_COMM]%s)connect(%s,%d) = %d\n",server,newserver,port,ret);
	if(ret == -1){
		//mpiServer_err(MPISERVERERR_MEMORY);
		fprintf(stderr,"nfi_mpiServer_init: error in connect %s (%s,%d)\n",
				server, newserver, port);
			perror("nfi_mpiServer_init:");
		return -1;
	}

	return sd;
      }

      ssize_t mpiServer_write_data(int fd, char *data, ssize_t size, char *id)
      {
	int ret = 0;
	int cont = 0;

	dbgcomm_info("[NFI_COMM]client: write_data(%d): %d ID=%s --th:%d--\n",fd,size,id,(int)pthread_self());

	if (size == 0){
	    return  0;
	}
	if (size < 0){
	    return  -1;
	}

	do{
		ssize_t (*real_write)(int, const void*, size_t);
		real_write = dlsym(RTLD_NEXT,"write");
		ret = real_write(fd, data+cont, size-cont);

	        dbgcomm_info("[NFI_COMM]client: write_data(%d): %d = %d ID=%s --th:%d--\n",fd,size,ret,id,(int)pthread_self());
                if(ret <= 0){
                        perror("client: Error write_comm:");
                }
		cont += ret;
	} while ((ret>0)&&(cont!=size));
	
	if (ret == -1)
	{
		dbgcomm_info("[NFI_COMM]client: write_data(%d): err %d  ID=%s --th:%d--\n",fd,ret,id,(int)pthread_self());
		perror("client: write_data");
		return ret;
	}
	dbgcomm_info("[NFI_COMM]client: write_data(%d): %d de %d ID=%s --th:%d--\n",fd,cont,size,id,(int)pthread_self());

	return size;
      }

      ssize_t mpiServer_read_data(int fd, char *data, ssize_t size, char *id)
      {
	int ret = 0;
	int cont = 0;

	dbgcomm_info("[NFI_COMM]client: read_data(%d): %d ID=%s --th:%d--\n",fd,size,id,(int)pthread_self());

	if(size == 0){
		return  0;
	}
	
	if(size < 0){
		return  -1;
	}

	do{
		ssize_t (*real_read)(int, void*, size_t);
		real_read = dlsym(RTLD_NEXT,"read");
		ret = real_read(fd, data+cont, size-cont);

	        dbgcomm_info("[NFI_COMM]client: read_data(%d): %d = %d ID=%s --th:%d--\n",fd,size,ret,id,(int)pthread_self());
                if(ret <= 0){
                        perror("client: Error read_comm:");
                }
		cont += ret;
	} while ((ret>0)&&(cont!= size));

	if (ret == -1) {
		dbgcomm_info("[NFI_COMM]client: read_data(%d): err %d  ID=%s --th:%d--\n",fd,ret,id,(int)pthread_self());
		perror("client: read_data");
		return ret;
	}
	dbgcomm_info("[NFI_COMM]client: read_data(%d): %d de %d ID=%s --th:%d--\n",fd,cont,size,id,(int)pthread_self());

	return size;
      }


   /* ................................................................... */

