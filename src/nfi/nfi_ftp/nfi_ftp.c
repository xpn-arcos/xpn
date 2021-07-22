#include "nfi/nfi_ftp/nfi_ftp.h"




/************************************************************
 * Gets attributes of the entries on the list	    	    *
 ************************************************************/
void get_attr_list(struct nfi_attr *attr,char *respuesta)
{
	char separator[2] = " ";
   	char *ptr;

   	//printf( "linea = %s\n", respuesta );

   	ptr = strtok( respuesta, separator );    // Primera llamada => Primer token
   	//printf( "%s\n", ptr );
   	
	if (ptr[1]=='-') attr->at_type=NFIFILE;
	else attr->at_type = NFIDIR;
	
	//Donde están los tipos de modo de protección??????
//	attr->at_mode=**********************
	
	//number of links
	if( (ptr = strtok( NULL, separator )) != NULL )    // Posteriores llamadas
	{
     		//printf( "%s\n", ptr );
		//attr->
	}
	//UID
	if( (ptr = strtok( NULL, separator )) != NULL )    // Posteriores llamadas
	{
     		//printf( "%s\n", ptr );
		//attr->at_uid=ptr;
	}
	//GID
	if( (ptr = strtok( NULL, separator )) != NULL )    // Posteriores llamadas
	{
     		//printf( "%s\n", ptr );
		//attr->at_gid=ptr;
	}
	//size
	if( (ptr = strtok( NULL, separator )) != NULL )    // Posteriores llamadas
	{
     		//printf( "%s\n", ptr );
		//attr->at_size=ptr;
	}
	
	//Y el resto de los parámetros que hago con ellos???
	//fecha
	/*if( (ptr = strtok( NULL, separator )) != NULL )    // Posteriores llamadas
	{
     		//printf( "%s\n", ptr );
		strcpy(attr->at_atime,ptr);
	}*/
	
}
//struct nfi_attr{
//	int 	at_type; 	/* FILE or DIR			*/
//	mode_t 	at_mode;	/* protection			*/
//	uid_t	at_uid;		/* UID				*/
//	gid_t	at_gid;		/* GID				*/
//	off_t	at_size;	/* size of file			*/
//	u_long  at_blksize;	/* blocksize for filesystem I/O */
//	u_long  at_blocks;  	/* number of blocks allocated 	*/
//	time_t  at_atime;   	/* time of last access 		*/
//	time_t  at_mtime;   	/* time of last modification 	*/
//	time_t  at_ctime;   	/* time of last change 		*/
//	void	*private_info;
//};




/************************************************************
 * Init Local 					    	    *
 ************************************************************/
int nfi_ftp_init(char *url, struct nfi_server *serv, struct nfi_fhandle *fh)
{
	
char server[MAXPATHLEN], dir[MAXPATHLEN] ,prt [MAXPATHLEN], login[MAXPATHLEN] ,passwd[MAXPATHLEN],port_string[MAXPATHLEN],raiz[MAXPATHLEN];
	int ret;
	struct nfi_ftp_server *server_aux;
	struct nfi_ftp_fhandle *fhandle_aux;
	int socket1;
	int state, port,f;
	char respuesta[1024];
	char file[1024];
	char ruta_local[MAXPATHLEN],fich_temp[MAXPATHLEN];
	int fin=0;
	int n=0;
	char *url_aux;
	
//printf("La URL es %s\n",url);
	url_aux=url;
	
	//struct hostent *hp;

	//If server structure is null, rise an error.
	if(serv == NULL){
		ftp_err(FTPERR_PARAM);
		return -1;
	}

	/* Inicializo la estructura del servidor que contendrá la información de las operaciones que
	se pueden realizar con el servidor e información acerca del servidor. */
	
	serv->ops = (struct nfi_ops *)malloc(sizeof(struct nfi_ops));
	if (serv->ops == NULL){
		ftp_err(FTPERR_MEMORY);
		return -1;
	}

	serv->ops->nfi_reconnect  = nfi_ftp_reconnect;
	serv->ops->nfi_disconnect = nfi_ftp_disconnect;

	serv->ops->nfi_getattr	= nfi_ftp_getattr;
	serv->ops->nfi_setattr	= nfi_ftp_setattr;

	serv->ops->nfi_open	= nfi_ftp_open;
	serv->ops->nfi_close	= nfi_ftp_close;

	serv->ops->nfi_read	= nfi_ftp_read;
	serv->ops->nfi_write	= nfi_ftp_write;

	serv->ops->nfi_create	= nfi_ftp_create;
	serv->ops->nfi_remove	= nfi_ftp_remove;
	serv->ops->nfi_rename	= nfi_ftp_rename;

	serv->ops->nfi_mkdir	= nfi_ftp_mkdir;
	serv->ops->nfi_rmdir	= nfi_ftp_rmdir;
	serv->ops->nfi_opendir	= nfi_ftp_opendir;
	serv->ops->nfi_readdir	= nfi_ftp_readdir;
	serv->ops->nfi_closedir	= nfi_ftp_closedir;
	serv->ops->nfi_statfs	= nfi_ftp_statfs;

	dir[0]='\0';
	
	//ret = ParseURL(url,  prt, NULL, NULL, server,  NULL,  dir);
	//Parseo la URL pasada por parámetro
	
	
	ret = ParseURL(url_aux,  prt, login, passwd, server,  port_string,  dir);

//printf("-----------------%s, %s, %s, %s, %s\n",login, passwd, server,  port_string,  dir);
	if(ret <0 ){//ha ocurrido algún error al parsear la URL
		ftp_err(FTPERR_URL);
		free(serv->ops);
		return -1;
	}
	

	server_aux = (struct nfi_ftp_server *)malloc(sizeof(struct nfi_ftp_server));
	if(server_aux == NULL){
		ftp_err(FTPERR_MEMORY);
		return -1;
	}
	fh->server= (struct nfi_server *)serv;
	
	if (strcmp(dir,"NULL")!=0)
	{	
		fhandle_aux = (struct nfi_ftp_fhandle *)malloc(sizeof(struct nfi_ftp_fhandle));
		
		fh->priv_fh=(struct nfi_ftp_fhandle *)fhandle_aux;
		
		file[0]='\0';
//printf("dir es %s\n",dir);
		fhandle_aux->path[0]='\0';
		strcpy(fhandle_aux->path, dir);
 		n=getNameFile(file,dir);//llamada a procedimiento de Luismi, probar si realmente funciona
		strcpy(fhandle_aux->file, file);
		//fhandle_aux->list == NULL;
	}
	/************************************************************/
//printf("el servidor es :%s \n",server);
	port=atoi(port_string);//transformo la cadena a integer
	socket1 = ftp_create_connection_control(server,port);
//printf("el resultado1 es %d\n",socket1);//tengo el socket

	server_aux->socket= socket1;
	strcpy(server_aux->url,url);
	strcpy(server_aux->user,"");
	strcpy(server_aux->password,"");	
	strcpy(server_aux->user,login);	
	strcpy(server_aux->password,passwd);
	
	server_aux->port= port;
	fin =receive_answer(server_aux->socket,respuesta);

	
	state = ftp_login(server_aux->socket,server_aux->user,server_aux->password,respuesta);
	
	if (state!=1)
	{
		perror("PROBLEMAS CON EL LOGIN\n");
		free(server_aux);
		free(fhandle_aux);
		return -1;
	}
	
	
	
	
	
	/************************************************************/
	serv->private_info = (void *)server_aux;
//printf("%s, %d, %s, %s \n",server_aux->url, server_aux->socket,server_aux->user,server_aux->password);
	//free(server_aux);
	//free(fhandle_aux);
	
	//serv->protocol = FTP;

	serv->server = (char *)malloc(sizeof(char)*(strlen(server)+1));	/* server address */
	if(serv->server == NULL){
		ftp_err(FTPERR_MEMORY);
		return -1;
	}
	strcpy(serv->server, server);

	serv->url = (char *)malloc(sizeof(char)*(strlen(url)+1));	/* server address */
	if(serv->url == NULL){
		ftp_err(FTPERR_MEMORY);
		return -1;
	}

	strcpy(serv->url, url);
        serv->wrk = (struct nfi_worker *)malloc(sizeof(struct nfi_worker));
	memset(serv->wrk, 0, sizeof(struct nfi_worker));
	if(strcmp(prt, "filehilos") == 0){
		//printf("filehilos\n");
		nfi_worker_init(serv->wrk, serv, 1);
	}
	else{
		nfi_worker_init(serv->wrk, serv, 0);
	}
	
	//Creamos el directorio y el fichero temporal.....
	n = getDir(dir);
//printf("El directorio a moverse es: %s\n",dir);
	
	//nos movemos en el servidor al directorio al que se van a realizar las operaciones
	if (strcmp(dir,"")!= 0) ftp_cwd(server_aux->socket,dir,respuesta);
			
	//almacenamos el nombre del directorio de trabajo
	getcwd(server_aux->exe_dir,strlen(server_aux->exe_dir));
//printf("Almacenamos el directorio actual de ejecución.....%s\n",server_aux->exe_dir);
	//almacenamos la información del nombre del fichero y del path
	/*strcpy(fh_aux->path,path);
	n = getNameFile(file,path);
	strcpy(fh_aux->file,file);
	*/
	
//printf("Almacenado path %s y fichero %s......?????????????????\n",fhandle_aux->path,fhandle_aux->file);
	
	ruta_local[0]='\0';
	strcpy(raiz,"/tmp/expand-XXXXXX");
	mkdtemp(raiz);
	chmod(raiz,0777);
	
	//me meto al directorio
	chdir(raiz);
//printf("el directorio temporal será %s\n",raiz);
	
	//creo el fichero
	strcpy(fich_temp,"XXXXXX");
	
	f = mkstemp(fich_temp);
	strcat(raiz,"/");
	strcpy(ruta_local,raiz);
	strcat(ruta_local,fich_temp);
	
//printf("El nombre del fichero es : %s,%s\n",ruta_local,fhandle_aux->file);
	
	
//printf("el directorio/fichero temporal será ------%s-------\n",ruta_local);
	
	
	if (f>0) 
	{//No ha habido error con el fichero
//printf("entramos a lo del enlace.............\n");
		/*open(ruta_local,O_EXCL|O_CREAT|O_TRUNC, 0777);
		printf("Antes del put\n");
		strcpy(cadena,"HOlitaa....");
		write(f,cadena,strlen(cadena));//prueba para ver si está abierto y existe
		close(f);*/
//printf("1\n");
		server_aux->fd=f;
//printf("2 %s\n",ruta_local);
		strcpy(server_aux->ruta_local,ruta_local);
//printf("3 %s\n",server_aux->ruta_local);
		/* Esto se hace en el open
		n=link(ruta_local,fhandle_aux->file);
		printf("4\n");
		if (n==0) printf("ENLACE TEMPORAL CREADO\n");
		else {
			printf("El error es %s",strerror(errno));
			return -1;
		}*/
	//Vuelvo al directorio de trabajo			
	chdir(server_aux->exe_dir);	
			
	}
	else {
		return -1;
//		printf("NO SE CREÓ EL FICHERO\n");
	}
	
		
	//Inicializo las variables para que no tengan porquería
	strcpy(prt,"");
	strcpy(login,"");
	strcpy(passwd,"");
	strcpy(server,"");
	strcpy(port_string,"");
	strcpy(dir,"");
	
	return 0;
	}

/************************************************************
 * Disconnect to the server				    *
 * **********************************************************/
int nfi_ftp_disconnect(struct nfi_server *serv)
{
	struct nfi_ftp_server *server_aux;
	char respues[MAXPATHLEN], dir[MAXPATHLEN];
	int n=0;
	
	
	
	
	if (serv == NULL)
		return 0;

	if(serv->ops != NULL)
		free(serv->ops);

	server_aux = (struct nfi_ftp_server *)serv->private_info;
	
	
	
	
	if(server_aux != NULL){
	
	
		/*if (close(server_aux->fd)!=0) 
		{
			printf("El fichero se cerró con errores\n");
			return 0;
		}*/
	
		//en el servidor vuelvo al directorio original
		ftp_cdup(server_aux->socket,respues);
		
		//vuelvo al directorio /tmp
		chdir("/tmp");
		
//printf("El directorio/fichero temporal es %s\n",server_aux->ruta_local);
		
		//Estaba en el STORE
		//printf("BORRO EL ENLACE AL FICHERO\n");
		//unlink(ruta);
	
		//borro el fichero temporal, local
		unlink(server_aux->ruta_local);
		
		//borro el directorio temporal
		strcpy(dir,server_aux->ruta_local);
//printf("El directorio a borrar es: %s\n",dir);
		n=getDir(dir);
//printf("El directorio a borrar es: %s\n",dir);
		n=rmdir(dir);
		//if (n==-1) printf("El error es %s",strerror(errno));
		
	
		//tendría que poner a NULL todos los atributos de la estructura????????
		free(serv->private_info);
		ftp_close_connection_control(server_aux->socket);
	
	}

	if(serv->url != NULL){
		free(serv->url);
	}

	if(serv->server != NULL){
		free(serv->server);
	}
	
	//serv->protocol = -1;
	
	nfi_worker_end(serv->wrk);
	
	return 0;
}



/************************************************************
 * Reconnect to the NFS server				    *
 ************************************************************/
int nfi_ftp_reconnect(struct nfi_server *serv)
{
	 int n =0;
	char server[MAXPATHLEN], dir[MAXPATHLEN] ,prt [MAXPATHLEN], login[MAXPATHLEN] ,passwd[MAXPATHLEN], port_string[MAXPATHLEN];
	char respuesta[NFIMAXPATHLEN],ruta_local[MAXPATHLEN], fich_temp[MAXPATHLEN], raiz[MAXPATHLEN];
	int socket1=0;
	int fin=0;
	int f;
	int state;
	int ret, port;
	struct nfi_ftp_server *server_aux;
	//struct nfi_ftp_fhandle *fhandle_aux;


//printf("Reconectamos...........\n");
	
	ret = ParseURL(serv->url,  prt, login, passwd, server,  port_string,  dir);
	
	if(ret <0 ){
		ftp_err(FTPERR_URL);
		return -1;
	}

	
	server_aux = (struct nfi_ftp_server *)malloc(sizeof(struct nfi_ftp_server));
	if(server_aux == NULL){
		ftp_err(FTPERR_MEMORY);
		return -1;
	}

	
	if (strcmp(dir,"NULL")!=0)
	{	
		/*fhandle_aux = (void *)malloc(sizeof(struct nfi_ftp_fhandle));
		
		fh->priv_fh=(struct nfi_ftp_fhandle *)fhandle_aux;
		
		file[0]='\0';
		printf("dir es %s\n",dir);
		fhandle_aux->path[0]='\0';
		strcpy(fhandle_aux->path, dir);
 		n=getNameFile(file,dir);//llamada a procedimiento de Luismi, probar si realmente funciona
		strcpy(fhandle_aux->file, file);
		//fhandle_aux->list == NULL;*/
	}
	
	/************************************************************/
	
	port=atoi(port_string);//transformo la cadena a integer
	socket1 = ftp_create_connection_control(server,port);

	server_aux->socket= socket1;
	
	strcpy(server_aux->user,"");
	strcpy(server_aux->password,"");	
	strcpy(server_aux->user,login);	
	strcpy(server_aux->password,passwd);
	
	server_aux->port= port;
	fin =receive_answer(server_aux->socket,respuesta);

	
	state = ftp_login(server_aux->socket,server_aux->user,server_aux->password,respuesta);
	
	if (state!=1)
	{
		perror("PROBLEMAS CON EL LOGIN\n");
		free(server_aux);
		//free(fhandle_aux);
		return -1;
	}
	

	
	//navegar por la estructura de directorios
	
	
	/************************************************************/
	
	//Creamos el directorio y el fichero temporal.....
	n = getDir(dir);
//printf("El directorio a moverse es: %s\n",dir);
	//nos movemos en el servidor al directorio al que se van a realizar las operaciones
	ftp_cwd(server_aux->socket,dir,respuesta);
			
	//almacenamos el nombre del directorio de trabajo
	getcwd(server_aux->exe_dir,strlen(server_aux->exe_dir));
//printf("Almacenamos el directorio actual de ejecución.....%s\n",server_aux->exe_dir);
	//almacenamos la información del nombre del fichero y del path
	/*strcpy(fh_aux->path,path);
	n = getNameFile(file,path);
	strcpy(fh_aux->file,file);
	*/
	
//printf("Almacenado path %s y fichero %s......?????????????????\n",fhandle_aux->path,fhandle_aux->file);
	
	ruta_local[0]='\0';
	strcpy(raiz,"/tmp/expand-XXXXXX");
	mkdtemp(raiz);
	chmod(raiz,0777);
	
	//me meto al directorio
	chdir(raiz);
//printf("el directorio temporal será %s\n",raiz);
	
	//creo el fichero
	strcpy(fich_temp,"XXXXXX");
	
	f = mkstemp(fich_temp);
	strcat(raiz,"/");
	strcpy(ruta_local,raiz);
	strcat(ruta_local,fich_temp);
	
//printf("El nombre del fichero es : %s,%s\n",ruta_local,fhandle_aux->file);
	
	
//printf("el directorio/fichero temporal será ------%s-------\n",ruta_local);
	
	
	if (f>0) 
	{//No ha habido error con el fichero
//printf("entramos a lo del enlace.............\n");
		/*open(ruta_local,O_EXCL|O_CREAT|O_TRUNC, 0777);
		printf("Antes del put\n");
		strcpy(cadena,"HOlitaa....");
		write(f,cadena,strlen(cadena));//prueba para ver si está abierto y existe
		close(f);*/
		server_aux->fd=f;
//printf("2 %s\n",ruta_local);
		strcpy(server_aux->ruta_local,ruta_local);
//printf("3 %s\n",server_aux->ruta_local);
		/* Esto se hace en el open
		n=link(ruta_local,fhandle_aux->file);
		printf("4\n");
		if (n==0) printf("ENLACE TEMPORAL CREADO\n");
		else {
			printf("El error es %s",strerror(errno));
			return -1;
		}*/
	//Vuelvo al directorio de trabajo			
	chdir(server_aux->exe_dir);	
			
	}
	else {
		return -1;
		printf("NO SE CREÓ EL FICHERO\n");
	}
	
		
	//Inicializo las variables para que no tengan porquería
	strcpy(prt,"");
	strcpy(login,"");
	strcpy(passwd,"");
	strcpy(server,"");
	strcpy(port_string,"");
	strcpy(dir,"");
	
	serv->private_info = (void *)server_aux;
	
	return 0;
	}

/************************************************************
 * Destroy NFS operations				    *
 ************************************************************/

int nfi_ftp_destroy(struct nfi_server *serv,struct nfi_fhandle *fh)
{
	struct nfi_ftp_server *server_aux;
	struct nfi_ftp_fhandle *fh_aux;
	char respues[1024], dir[NFIMAXPATHLEN];
	int n=0;
	
	
	
	if (serv == NULL)
		return 0;
	
	if(serv->ops != NULL)
		free(serv->ops);

	server_aux = (struct nfi_ftp_server *)serv->private_info;

	if(server_aux != NULL){
        	
		
		//tendría que poner a NULL todos los atributos de la estructura????????
		
		/*if (close(server_aux->fd)!=0) 
		{
			printf("El fichero se cerró con errores\n");
			return 0;
		}*/
		fh_aux = (struct nfi_ftp_fhandle *) fh->priv_fh;
		
		//en el servidor vuelvo al directorio original
		ftp_cdup(server_aux->socket,respues);
		
//		printf("Terminado el DUP ahora a borrar..........\n");
		//vuelvo al directorio /tmp
		chdir("/tmp");
		
//printf("El directorio/fichero temporal es %s\n",server_aux->ruta_local);
		
		//Estaba en el STORE
		//printf("BORRO EL ENLACE AL FICHERO\n");
		//unlink(ruta);
	
		//borro el fichero temporal, local
		unlink(server_aux->ruta_local);
		
		if (server_aux->fd!=0) unlink(fh_aux->file);//Por si no se ha hecho el close antes
		
		//if (n==-1) printf("El error es %s",strerror(errno));
		
		//borro el directorio temporal
		strcpy(dir,server_aux->ruta_local);
//printf("El directorio a borrar es: %s\n",dir);
		n=getDir(dir);
//printf("El directorio a borrar es: %s\n",dir);
		n=rmdir(dir);
		if (n==-1) printf("El error es %s",strerror(errno));
		
		/* free memory */
		free(fh->priv_fh);
		fh->priv_fh = NULL;
	
		//tendría que poner a NULL todos los atributos de la estructura????????
		free(serv->private_info);
		ftp_close_connection_control(server_aux->socket);
		
	}

	if(serv->url != NULL){
		free(serv->url);
	}

	if(serv->server != NULL){
		free(serv->server);
	}

	//serv->protocol = -1;
	
	nfi_worker_end(serv->wrk);

	return 0;
}




/************************************************************
 * GENERIC FUNCTIONS 					    *
 ************************************************************/

int nfi_ftp_getattr(struct nfi_server *serv,  struct nfi_fhandle *fh, struct nfi_attr *attr){
	int ret;
	//struct stat st;
	char respuesta[MAXPATHLEN];

	struct nfi_ftp_server *server_aux;
	struct nfi_ftp_fhandle *fh_aux;


	if (attr == NULL){
		ftp_err(FTPERR_PARAM);
		return -1;
	}


	if (serv == NULL){
		ftp_err(FTPERR_PARAM);
		return -1;
	}


	if (fh == NULL){
		ftp_err(FTPERR_PARAM);
		return -1;
	}

	if (fh->priv_fh == NULL){
		ftp_err(FTPERR_PARAM);
		return -1;
	}


#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_ftp_reconnect(serv);
		if(ret <0){
			/* ftp_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               ftp_err(FTPERR_PARAM);
               return -1;
        }
#endif
	server_aux = (struct nfi_ftp_server *) serv->private_info;
	fh_aux = (struct nfi_ftp_fhandle *) fh->priv_fh;

/*	ret = stat(fh_aux->path, &st);
	if(ret < 0){
		ftp_err(FTPERR_GETATTR);
		return -1;
	}
*/
	fh_aux->lista_dir = NULL;

	ret=ftp_list(server_aux->url,server_aux->socket,fh_aux->lista_dir,fh_aux->path,respuesta);
	
	if (ret==1)
	{
		 //printf("Operación de LIst conseguida");
		 get_attr_list(attr,respuesta);//hay que hacerlo **********************************************
	}
	else {
		ftp_err(FTPERR_GETATTR);
		//printf("PROBLEMAS CON EL LIST");
		return -1;
	}


 	//FTPtoNFIattr(attr, &st);
	return 0;
}

/************************************************************
 * Set attributes to files			    	    *
 ************************************************************/

int nfi_ftp_setattr(struct nfi_server *serv,  struct nfi_fhandle *fh, struct nfi_attr *attr){
	//FTP has no way to put attributes
	return -1;
}

/************************************************************
 * Open a file 					    	    *
 ************************************************************/

int nfi_ftp_open(struct nfi_server *serv,  char *url, struct nfi_fhandle *fho )
{
	char dir[NFIMAXPATHLEN], server[NFIMAXPATHLEN], file[NFIMAXPATHLEN] ,prt [MAXPATHLEN], login[MAXPATHLEN] ,passwd[MAXPATHLEN], respuesta[MAXPATHLEN],port_string[MAXPATHLEN];
	int ret;
	int port=0;
	char path[1024];
	//struct stat st;
	struct nfi_ftp_server *server_aux;
	struct nfi_ftp_fhandle *fh_aux;
	struct list_dir *fh_aux_lista;
	int n=0;
	//int f;
	//char cadena[255];//esto quitarlo despues es solo para pruebas...

	fh_aux = (struct nfi_ftp_fhandle *) fho->priv_fh;
			
//printf("DEntro del FH hay ............. %s****%s*********\n",server_aux->ruta_local,server_aux->url);
		
	
	if (serv == NULL){
		ftp_err(FTPERR_PARAM);
		return -1;
	}

	if (fho == NULL){
		ftp_err(FTPERR_PARAM);
		return -1;
}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_ftp_reconnect(serv);
		if(ret <0){
			/* ftp_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               ftp_err(FTPERR_PARAM);
               return -1;
        }
#endif
//printf("la url del servidor es %s, y la del string %s\n",serv->url, url);//revisar porque no se almacena lo bueno en la estructura...

	
	
	
//printf("socket %d, url %s, user %s, pass %s, port %d \n", server->private_info->socket, server.private_info->url, server.private_info->user, server.private_info->password, server.private_info->port);
	ret = ParseURL(url,  prt, login, passwd, server,  port_string,  dir);
	if(ret < 0){
		ftp_err(FTPERR_URL);
		return -1;
	}

	fho->url = (char *)malloc(strlen(url)+1);
	if(fho->url == NULL){
		ftp_err(FTPERR_MEMORY);
		return -1;
	}

	strcpy(fho->url, url);

	/*fh_aux = (struct nfi_ftp_fhandle *)malloc(sizeof(struct nfi_ftp_fhandle));
	if (fh_aux == NULL){
		ftp_err(FTPERR_MEMORY);
		free(fho->url);
		return -1;
	}
*/
	server_aux = (struct nfi_ftp_server *) serv->private_info;

	//DEBO RELLENAR TODOS LOS CAMPOS??? SINO LO PONGO, EN EL SERVER_AUX no está lo correcto
	
	strcpy(server_aux->url,server);
	
//printf("El directorio %s y fichero es %s\n",dir,file);
	file[0]='\0';
//tengo que comprobar que el fichero existe y si existe, guardo el path y el nombre del fichero

//	n = getNameFile(file,dir); Esta llamada se hace a un procedimiento de string_misc, hay otro en ftp.c que se llama int get_filename(char * ruta), ver cual se necesita.........

	port=atoi(port_string);//transformo la cadena a integer
	
//printf("El directorio %s y fichero es %s\n",dir,file);

	/*
	No hace falta????, se rellenó en el init?????
	
	server_aux->socket= socket1;
	strcpy(server_aux->url,serv->url);
	strcpy(server_aux->user,login);
	strcpy(server_aux->password,passwd);
	server_aux->port= port;
	*/
		
	strcpy(path,dir);
	//	strcat(path,file);
	
//	printf("@@@@@@@@@@@url %s, socket %d, path %s \n",server_aux->url,server_aux->socket,path);
		
	fh_aux_lista = (struct list_dir *)malloc(sizeof(struct list_dir));
	if (fh_aux_lista == NULL){
		ftp_err(FTPERR_MEMORY);
		//debería liberar toda la estructura..........
		free(fho->url);
		return -1;
	}
		
	fh_aux->lista_dir= fh_aux_lista;
		
		
	ret = LIST_DIR_init(fh_aux->lista_dir) ;
	if (ret < 0) return ret ;

	respuesta[0]='\0';
	
	n=getNameFile(fh_aux->file,dir);
	//printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@EL nombre del fichero es %s\n",fh_aux->file);
	
	ret=ftp_list(server_aux->url,server_aux->socket,fh_aux->lista_dir,fh_aux->file,respuesta);
	if (ret==1)
	{
		//printf("Operación de LIst conseguida\n");		 
		//con el if comprobamos si ese nombre de fichero está en el list
		if ((fh_aux->lista_dir->first)!= NULL)
		{
//printf("El fichero existe %s , %s\n",server_aux->ruta_local,fh_aux->file);
			n=link(server_aux->ruta_local,fh_aux->file);
			if (n==0) printf("ENLACE TEMPORAL CREADO\n");
			else {
				//printf("El enlace temporal no se creó porque : %s\n",strerror(errno));
				if(strstr(strerror(errno),"File exists")==NULL) {
					//printf("dentro del IF");
					return -1;
				}
					
			}
		}
		else{
//		printf("No se encontró el fichero............\n");
		return -1;
		}
		
	}
	else 
	{
		//printf("PROBLEMAS CON EL LIST");
		return -1;
	}



	

        /* 
	lo necesito??? y lo siguiente?
	
	Lo que tengo que hacer es ver si el fichero existe en el list...........
	rellenar el fho (file_handler)
	
	
	
	if(S_ISDIR(st.st_mode)){
		fho->type = NFIDIR;
	}
	else{
		if(S_ISREG(st.st_mode)){
			fho->type = NFIFILE;			
		}
		else{
        		ftp_err(FTPERR_GETATTR);
			free(fh_aux);
			free(fho->url);
			return -1;
		}
	}
	
	¿Como averiguo si es un fichero o es un directorio lo pasado por la url, con FTP?
	*/
	
	
	
	
	//fho->server = NULL;da igual, lo asignamos y ya está
	//fho->priv_fh = NULL; Hecho en el INIT
	fho->server = serv;
	fho->priv_fh = (void *) fh_aux;

	
	return 0;


}

/************************************************************
 * Close remote file				    	    *
 ************************************************************/

int nfi_ftp_close(struct nfi_server *server,  struct nfi_fhandle *fh){
	struct nfi_ftp_fhandle *fh_aux;
	struct nfi_ftp_server * server_aux;
	int ret;
	
	if (server == NULL){
		ftp_err(FTPERR_PARAM);
		return -1;
	}
	else
	{
		server_aux = (struct nfi_ftp_server *) server->private_info;
	}
	
	if (fh == NULL){
		ftp_err(FTPERR_PARAM);
		return -1;
	}

	if (fh->priv_fh != NULL){
		
		fh_aux = (struct nfi_ftp_fhandle *) fh->priv_fh;
		//elimino la información del path y el fichero
		//?? si luego voy a quitar el puntero, no
		
		/* free list memory */
		ret = LIST_DIR_finalize(fh_aux->lista_dir) ;
		if (ret < 0) return ret ;
		
		/*close File*/
		/*if (close(server_aux->fd)!=0) {
				printf("El fichero se cerró con errores\n");
				return 0;
		}
		else */
		unlink(fh_aux->file);//DEJARLO AQUÍ UNLINK SI SE HACE..............
		server_aux->fd = 0;//lo pongo a Cero para denotar que se ha cerrado el fichero.
		
	}
	
	if (fh->url != NULL){
		free(fh->url);
	}
	
	fh->type = NFINULL;
	fh->server = NULL;

	return 0;
}

/************************************************************
 * Read size from file				    	    *
 ************************************************************/

ssize_t nfi_ftp_read(	struct nfi_server *serv,
			struct nfi_fhandle *fh,
			void *buffer,
			off_t offset,
			size_t size)
{
	//ssize_t new_size;
	int ret;

	struct nfi_ftp_server *server_aux;
	struct nfi_ftp_fhandle *fh_aux;

	if (serv == NULL){
		ftp_err(FTPERR_PARAM);
		return -1;
	}


	if (fh == NULL){
		ftp_err(FTPERR_PARAM);
		return -1;
	}


#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_ftp_reconnect(serv);
		if(ret <0){
			/* ftp_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               ftp_err(FTPERR_PARAM);
               return -1;
        }
#endif


	fh_aux = (struct nfi_ftp_fhandle *) fh->priv_fh;
	server_aux = (struct nfi_ftp_server *) serv->private_info;

//	lseek(server_aux->fd, offset, SEEK_SET);

//printf("Procedemos a conseguir el fichero %s,%s, desde %d, con tamaño %d",fh_aux->file,fh_aux->path ,offset,size);

	ret=ftp_retr(server_aux->url,server_aux->socket,fh_aux->file ,offset,size,(char *)buffer);
	if (ret==1)
	{	//tras recuperar el fichero, meto en el buffer el fichero recuperado
	 	//printf("\n Operación de RETR conseguida\n");
		
	 
	}
	else {
		//printf("PROBLEMAS CON EL RETR");
		perror("error al leer\n");
		ftp_err(FTPERR_READ);
		return -1;
	}


/*	new_size = read(server_aux->fd,
			    buffer,
			    size);
	if(new_size < 0){
		perror("error al leer\n");
		ftp_err(FTPERR_READ);
		return -1;
	}
*/
        /**************************
	 * MARGA POR DIOSSSSSSSSSSSSSSSSSSSSSS DEVUELVE 
	 * EL TAMAÑOOOOOOOOOOOOOOOO DE LECTURA
	 * ***********************/
	return ret;
}

/************************************************************
 * Write size from file				    	    *
 ************************************************************/

ssize_t nfi_ftp_write(struct nfi_server *serv,
			struct nfi_fhandle *fh,
			void *buffer,
			off_t offset,
			size_t size)
{
	//ssize_t new_size;
        struct nfi_ftp_server *server_aux;
        struct nfi_ftp_fhandle *fh_aux;
	int ret;

	if (serv == NULL){
		ftp_err(FTPERR_PARAM);
		return -1;
	}


	if (fh == NULL){
		ftp_err(FTPERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_ftp_reconnect(serv);
		if(ret <0){
			/* ftp_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               ftp_err(FTPERR_PARAM);
               return -1;
        }
#endif


        fh_aux = (struct nfi_ftp_fhandle *) fh->priv_fh;
        server_aux = (struct nfi_ftp_server *) serv->private_info;

//printf("El valor de la url es %s,%s\n",server_aux->url,fh->url);
//printf("Procedemos a enviar el fichero %s, desde %d, con tamaño %d\n",fh_aux->file,offset,(int)size);
	
	ret=ftp_store(server_aux->url,server_aux->socket,fh_aux->file,offset,size,(char *)buffer,server_aux->ruta_local, server_aux->fd,server_aux->exe_dir);
	if (ret==1)
	{
		 //printf("Operación de STORE conseguida");
	}
	else {
		//printf("PROBLEMAS CON EL STORE");
		perror("error al escribir\n");
		ftp_err(FTPERR_WRITE);
		return -1;
	}
	
		

/*	lseek(server_aux->fd, offset, SEEK_SET);
	new_size = write(server_aux->fd,
			buffer,		
			size);

	if(new_size < 0){
		perror("error al escribir\n");
		ftp_err(FTPERR_WRITE);
		return -1;
	}
*/
	return ret;
}

/************************************************************
 * Create a file in server			    	    *
 ************************************************************/
int nfi_ftp_create(struct nfi_server *serv,  char *url, struct nfi_attr *attr, struct nfi_fhandle  *fh){
	char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN] ,prt [MAXPATHLEN], login[MAXPATHLEN] ,passwd[MAXPATHLEN];
	int ret,n;
	struct nfi_ftp_server *server_aux;
        struct nfi_ftp_fhandle *fh_aux;
	//struct stat st;

	if (serv == NULL){
		ftp_err(FTPERR_PARAM);
		return -1;
	}

	if (attr == NULL){
		ftp_err(FTPERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_ftp_reconnect(serv);
		if(ret <0){
			/* ftp_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               ftp_err(FTPERR_PARAM);
               return -1;
        }
#endif
	
	ret = ParseURL(url,  prt, login, passwd, server,  NULL,  dir);
	if(ret < 0){
		ftp_err(FTPERR_URL);
		return -1;
	}
	/* private_info file handle */
	fh_aux = (struct nfi_ftp_fhandle *)malloc(sizeof(struct nfi_ftp_fhandle));
	if (fh_aux == NULL){
		ftp_err(FTPERR_MEMORY);
		return -1;
	}

	bzero(fh_aux, sizeof(struct nfi_ftp_fhandle));
	

        server_aux = (struct nfi_ftp_server *) serv->private_info;
	/* create the file into the directory */
	
	//**********************************************************
	//No tengo que crear nada, en cuanto mando los primeros bytes, 
	//me crea el fichero en la ruta
	//***********************************************************

	
	strcpy(fh_aux->path, dir);
	n=getNameFile(fh_aux->file,dir);
	//printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@EL nombre del fichero es %s\n",fh_aux->file);

	
        fh->priv_fh = (void *)fh_aux;

        fh->url = (char *)malloc(strlen(url)+1);
        if(fh->url == NULL){
               	ftp_err(FTPERR_MEMORY);
		free(fh_aux);
               	return -1;
        }
		
        strcpy(fh->url, url);
	//printf("El directorio a crear es: %s, %s\n",fh_aux->path,fh->url);
	/* printf*/
	
	/*memset (&st, 0, sizeof(struct stat));
	ret = stat(fh_aux->path, &st);
	if(ret < 0){
		printf("stat mal\n");
		ftp_err(FTPERR_GETATTR);
		free(fh->url);
		free(fh_aux);
		
		return -1;
	}*/
	
	//FTPtoNFIattr(attr, &st);

	return 0;
}

/************************************************************
 * Remove a file from server			    	    *
 ************************************************************/

int nfi_ftp_remove(struct nfi_server *serv,  char *url)
{

	char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN], file[NFIMAXPATHLEN] ,prt [MAXPATHLEN], login[MAXPATHLEN] ,passwd[MAXPATHLEN], respuesta[MAXPATHLEN];
	int ret , n;
	struct nfi_ftp_server *server_aux;
	if (serv == NULL){
		ftp_err(FTPERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_ftp_reconnect(serv);
		if(ret <0){
			/* ftp_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               ftp_err(FTPERR_PARAM);
               return -1;
        }
#endif


	server_aux = (struct nfi_ftp_server *)serv->private_info;

	ret = ParseURL(url,  prt, login, passwd, server,  NULL,  dir);
	if(ret < 0){
		ftp_err(FTPERR_URL);
		return -1;
	}
	n = getNameFile(file,dir);

	/* remove the file into the directory */
	//ret=ftp_dele(server_aux->socket,file,respuesta);
	ret=ftp_dele(server_aux->socket,dir,respuesta);
	
	if (ret==1)
	{
		 //printf("Operación de DELE conseguida\n");
	}
	else 
	{
		//printf("PROBLEMAS CON EL DELE\n");
		ftp_err(FTPERR_REMOVE);
		return -1;
	}


	/*
		ret = unlink(dir);

	if(ret < 0){
		ftp_err(FTPERR_REMOVE);
		return -1;
	}
*/


	return 0;
}

/************************************************************
 * Rename a file				    	    *
 ************************************************************/
int nfi_ftp_rename(struct nfi_server *serv,  char *old_url, char *new_url)
{
	char server[NFIMAXPATHLEN], old_dir[NFIMAXPATHLEN], new_dir[NFIMAXPATHLEN], file[NFIMAXPATHLEN], filenew[NFIMAXPATHLEN], respuesta[NFIMAXPATHLEN];
	int ret, n;
	struct nfi_ftp_server *server_aux;

	if (serv == NULL){
		ftp_err(FTPERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_ftp_reconnect(serv);
		if(ret <0){
			/* ftp_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               ftp_err(FTPERR_PARAM);
               return -1;
        }
#endif


	server_aux = (struct nfi_ftp_server *)serv->private_info;

	ret = ParseURL(old_url,  NULL, NULL, NULL, server,  NULL,  old_dir);
	if(ret < 0){
		ftp_err(FTPERR_URL);
		return -1;
	}
	ret = ParseURL(new_url,  NULL, NULL, NULL, server,  NULL,  new_dir);
	if(ret < 0){
		ftp_err(FTPERR_URL);
		return -1;
	}

n = getNameFile(file,old_dir);
n = getNameFile(filenew,new_dir);

	/* remove the file into the directory */
	ret=ftp_rename(server_aux->socket,file,filenew,respuesta);
	if (ret==1)
		{
		 //printf("Operación de RENAME conseguida\n");
		}
		else 
		{
			//printf("PROBLEMAS CON EL RENAME\n");
			ftp_err(FTPERR_RENAME);
			return -1;
		}


	/*
Aqui antes no había nada*/


	return 0;
}

/************************************************************
 * Make a directory on the server		    	    *
 ************************************************************/
int nfi_ftp_mkdir(struct nfi_server *serv,  char *url, struct nfi_attr *attr, struct nfi_fhandle *fh)
{

	char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN] ,prt [MAXPATHLEN], login[MAXPATHLEN] ,passwd[MAXPATHLEN], respuesta[MAXPATHLEN];
	int ret;
	//struct stat st;		/* FTP attributes */
        struct nfi_ftp_server *server_aux;
        struct nfi_ftp_fhandle *fh_aux;


	if (serv == NULL){
		ftp_err(FTPERR_PARAM);
		return -1;
	}

	if (attr == NULL){
		ftp_err(FTPERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_ftp_reconnect(serv);
		if(ret <0){
			/* ftp_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               ftp_err(FTPERR_PARAM);
               return -1;
        }
#endif

	server_aux = (struct nfi_ftp_server *)serv->private_info;

	ret = ParseURL(url,  prt, login, passwd, server,  NULL,  dir);
	if(ret < 0){
		ftp_err(FTPERR_URL);
		return -1;
	}

	/* private_info file handle */
	fh_aux = (struct nfi_ftp_fhandle *)malloc(sizeof(struct nfi_ftp_fhandle));
	if (fh_aux == NULL){
		ftp_err(FTPERR_MEMORY);
		return -1;
	}


	bzero(fh_aux, sizeof(struct nfi_ftp_fhandle));

	/* create the dir into the directory */

	ret=ftp_mkd(server_aux->socket,dir,respuesta);
		if (ret==1)
		{
		 //printf("Operación de MKD conseguida\n");
		}
		else
		{
			//printf("PROBLEMAS CON EL MKD\n");
			ftp_err(FTPERR_MKDIR);
			free(fh_aux);
			return -1;
		}


	/*ret = mkdir(dir, attr->at_mode);

	if(ret < 0){
		ftp_err(FTPERR_MKDIR);
		free(fh_aux);
		return -1;
	}
*/
        fh->priv_fh = (void *)fh_aux;

        fh->url = (char *)malloc(strlen(url)+1);
        if(fh->url == NULL){
               ftp_err(FTPERR_MEMORY);
	       free(fh_aux);
               return -1;
        }
        strcpy(fh->url, url);

	//FTPtoNFIattr(attr, &st);


	return 0;
}

/************************************************************
 * Delete a directory in the server		    	    *
 ************************************************************/
int nfi_ftp_rmdir(struct nfi_server *serv,  char *url)
{
	char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN],prt [MAXPATHLEN], login[MAXPATHLEN] ,passwd[MAXPATHLEN], respuesta[MAXPATHLEN];
	int ret;

        struct nfi_ftp_server *server_aux;

	if (serv == NULL){
		ftp_err(FTPERR_PARAM);
		return -1;
	}


#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_ftp_reconnect(serv);
		if(ret <0){
			/* ftp_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               ftp_err(FTPERR_PARAM);
               return -1;
        }
#endif


	server_aux = (struct nfi_ftp_server *)serv->private_info;

	ret = ParseURL(url,  prt, login, passwd, server,  NULL,  dir);
	if(ret < 0){
		ftp_err(FTPERR_URL);
		return -1;
	}


	ret=ftp_rmd(server_aux->socket,dir,respuesta);
	if (ret==1)
	{
	 //printf("Operación de RMD conseguida\n");
	}
	else 
	{
		//printf("PROBLEMAS CON EL RMD\n");
		ftp_err(FTPERR_REMOVE);
		return -1;
	}

	/*ret = rmdir(dir);	
	if(ret < 0){
		ftp_err(FTPERR_REMOVE);
		return -1;
	}*/

	return 0;
}

/************************************************************
 * Open a dir in the server			    	    *
 ************************************************************/
int nfi_ftp_opendir(struct nfi_server *serv,  char *url, struct nfi_fhandle *fh)
{
	char dir[NFIMAXPATHLEN], server[NFIMAXPATHLEN],prt [MAXPATHLEN], login[MAXPATHLEN] ,passwd[MAXPATHLEN], respuesta[MAXPATHLEN], port_string[MAXPATHLEN], file[MAXPATHLEN];
	int ret;	
	struct nfi_ftp_server *server_aux;
	struct nfi_ftp_fhandle *fh_aux;
	struct list_dir *fh_aux_lista;
	int port =0;
	int n=0;
	char path[1024];


	if (serv == NULL){
		ftp_err(FTPERR_PARAM);
		return -1;
	}

	if (fh == NULL){
		ftp_err(FTPERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_ftp_reconnect(serv);
		if(ret <0){
			/* ftp_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               ftp_err(FTPERR_PARAM);
               return -1;
        }
#endif
	
	prt[0]='\0';
	login[0]='\0';
	passwd[0]='\0';
	server[0]='\0';
	port_string[0]='\0';
	dir[0]='\0';
		

	ret = ParseURL(url,  prt, login, passwd, server,  port_string,  dir);
	if(ret < 0){
		ftp_err(FTPERR_URL);
		return -1;
	}
	
	

	fh->url = (char *)malloc(strlen(url)+1);
	if(fh->url == NULL){
		ftp_err(FTPERR_MEMORY);
		return -1;
	}

	strcpy(fh->url, url);

	fh_aux = (struct nfi_ftp_fhandle *)malloc(sizeof(struct nfi_ftp_fhandle));
	if (fh_aux == NULL){
		ftp_err(FTPERR_MEMORY);
		free(fh->url);
		return -1;
	}
	
	
	server_aux = (struct nfi_ftp_server *) serv->private_info;
	
	fh_aux_lista = (struct list_dir *)malloc(sizeof(struct list_dir));
	if (fh_aux_lista == NULL){
		ftp_err(FTPERR_MEMORY);
		//debería liberar toda la estructura..........
		free(fh->url);
		return -1;
	}
		
	fh_aux->lista_dir= fh_aux_lista;
	
	ret = LIST_DIR_init(fh_aux->lista_dir) ;
	if (ret < 0) return ret ;

		
	
	strcpy(server_aux->url,server);
	
	n = getNameFile(file,dir);
	port=atoi(port_string);//transformo la cadena a integer
	
//printf("El directorio %s y fichero es %s\n",dir,file);

	/*
	No hace falta????, se rellenó en el init?????
	
	server_aux->socket= socket1;
	strcpy(server_aux->url,serv->url);
	strcpy(server_aux->user,login);
	strcpy(server_aux->password,passwd);
	server_aux->port= port;
	*/
		
	strcpy(path,dir);
	strcat(path,file);
	
	
	respuesta[0]='\0';

//printf("Antes de entrar en el ftp_list respuesta es :%s, %p, %d",respuesta, respuesta,server_aux->socket);
	ret=ftp_list(server_aux->url,server_aux->socket,fh_aux->lista_dir,dir,respuesta);
	if (ret==1)
	{
		 //printf("Operación de LIst conseguida\n");
	}
	else
	{
		//printf("PROBLEMAS CON EL LIST\n");
		return -1;		
	}
	
	
	
	if (fh_aux->lista_dir->first!= NULL)
	{
		//printf("DIRECTORY FOUND %s, en %s\n",dir,respuesta);
	}
	else return -1;
	/*fh_aux->dir = opendir(dir);
        if (ret < 0 ){
	        ftp_err(FTPERR_MEMORY);		
		free(fh_aux);
		free(fho->url);
		return -1;		
	}
	
	server_aux->fd = ret;*/
	strcpy(fh_aux->path, dir);
	//supongo que aquí no me van a decir que haga el opendir de un fichero	
	fh->type = NFIDIR;			
	
	fh->server = NULL;
	fh->priv_fh = NULL;
	fh->server = serv;
	fh->priv_fh = (void *) fh_aux;

	return 0;

}


/************************************************************
 * List the directory				    	    *
 ************************************************************/
int nfi_ftp_readdir(struct nfi_server *serv,  struct nfi_fhandle *fh, char *entry , unsigned char *type)
{
	int ret;
	char respuesta[MAXPATHLEN];
	//struct dirent *ent;

        struct nfi_ftp_server *server_aux;
        struct nfi_ftp_fhandle *fh_aux;


	if (serv == NULL){
		ftp_err(FTPERR_PARAM);
		return -1;
	}


	if (fh == NULL){
		ftp_err(FTPERR_PARAM);
		return -1;
	}

	if (fh->priv_fh == NULL){
		ftp_err(FTPERR_PARAM);
		return -1;
	}


	if(fh->type != NFIDIR){
		ftp_err(FTPERR_NOTDIR);
		return -1;
	}
	
#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_ftp_reconnect(serv);
		if(ret <0){
			/* ftp_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               ftp_err(FTPERR_PARAM);
               return -1;
        }
#endif


	server_aux = (struct nfi_ftp_server *)serv->private_info;
	fh_aux = (struct nfi_ftp_fhandle *)fh->priv_fh;

	entry[0] = '\0';

	//If List is fill, then return the next element of the list.
	if((fh_aux->lista_dir->first->data!=NULL))
	{	
		//printf("Imprimo la posición %d \n",fh_aux->lista_dir->dir_pos);
		entry = seek_list(fh_aux->lista_dir->first,fh_aux->lista_dir->dir_pos);
		//printf("En esa posición está: %s\n",entry);
		if (entry != NULL) fh_aux->lista_dir->dir_pos++;
	}
	else
	{
	//else, list directory and fill list in structure
		ret = LIST_DIR_init(fh_aux->lista_dir) ;
		if (ret < 0) return ret ;
	
		ret=ftp_list(server_aux->url,server_aux->socket,fh_aux->lista_dir,fh_aux->path,respuesta);
		if (ret==1)
		{
		 //printf("Operación de LIst conseguida\n");
		}
		else printf("PROBLEMAS CON EL LIST\n");
		
		/*ret=fill_struct(fh_aux->lista_dir,respuesta);
		fh_aux->lista_dir->first->dir_pos = 0;
		
		entry = seek_list(fh_aux->list,fh_aux->list->dir_pos);		
		fh_aux->list->dir_pos++;
*/
	}
		
	
	
	
	/*ent = readdir(fh_aux->dir);

	if(ent == NULL){
		return 0;
	}*/
	//strcpy(entry, ent->d_name);
/*	free (ent);*/

	return 0;
}

/************************************************************
 * Go out directory				    	    *
 ************************************************************/
int nfi_ftp_closedir(struct nfi_server *serv,  struct nfi_fhandle *fh)
{
	struct nfi_ftp_fhandle *fh_aux;

	if (serv == NULL){
		ftp_err(FTPERR_PARAM);
		return -1;
	}
	if (fh == NULL){
		ftp_err(FTPERR_PARAM);
		return -1;
	}

	if (fh->priv_fh != NULL){
		fh_aux = (struct nfi_ftp_fhandle *) fh->priv_fh;
		//closedir(fh_aux->dir);
		//Si elimino el puntero elimino toda la información, así que con esto basta
		/* free memory */
		free(fh->priv_fh);
		fh->priv_fh = NULL;

	}
	
	if (fh->url != NULL){
		free(fh->url);
	}
	
	return 0;
	
}

/************************************************************
 * Statistics of the file service		    	    *
 ************************************************************/
int nfi_ftp_statfs(struct nfi_server *serv, struct nfi_info *inf)
{

	int ret;
	struct nfi_ftp_server *server_aux;
    //struct nfi_ftp_fhandle *fh_aux;


	if (serv == NULL){
		ftp_err(FTPERR_PARAM);
		return -1;
	}


/*	if (fh == NULL){
		ftp_err(FTPERR_PARAM);
		return -1;
	}

	if (fh->priv_fh == NULL){
		ftp_err(FTPERR_PARAM);
		return -1;
	}

	if(fh->type != NFIDIR){
		ftp_err(FTPERR_NOTDIR);
		return -1;
	}
*/
#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_ftp_reconnect(serv);
		if(ret <0){
			/* ftp_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               ftp_err(FTPERR_PARAM);
               return -1;
        }
#endif

	server_aux = (struct nfi_ftp_server *)serv->private_info;
	//fh_aux = (struct nfi_ftp_fhandle *)fh->priv_fh;

	ret=ftp_syst(server_aux->socket);
	if (ret==1)
	{
		 //printf("Operación de SYST conseguida\n");
	}
	else
	{
		//printf("PROBLEMAS CON EL SYST\n");
		return -1;		
	}



	
	return 0;
}

