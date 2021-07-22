#include "nfi/nfi_ftp/ftp.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>


/****************************************************************
 ****************************************************************/
 
int ftp_create_connection_control(char *name, int port)
{ 
  /* estructuras utilizas en el proceso de conexión */
  struct sockaddr_in raddr;
  struct hostent *hp;
 // struct timeval wait;
  /* variables auxiliares utilizadas en el proceso*/
  int sock;
  
  sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  bzero((char *)&raddr, sizeof(raddr));
  
  /* obtengo la direccion del host del servidor FTP a partir del nombre */
  //perror("no se que pasa\n");
  hp = gethostbyname(name);
 
  
  if(hp == NULL){
  return -1;
  }

 /* se inicializan los parametros del socket */

  memcpy(&(raddr.sin_addr), hp->h_addr, hp->h_length);
  raddr.sin_family = AF_INET;
  raddr.sin_port=htons(port);/* puerto al que me voy a conectar.*/
  //sock = RPC_ANYSOCK;

// se establece la conexión
connect(sock, (struct sockaddr *) &raddr, sizeof(raddr));

if (sock==-1) return -1; //no hemos podido establecer a conexión


return sock;
}

/****************************************************************
 ****************************************************************/
void ftp_close_connection_control(int sc)
{
	close(sc);
	//printf("Cerrada conexión de control \n");
}


/****************************************************************
 ****************************************************************/
int ftp_create_connection_data(char *name, int port)
{
 /* estructuras utilizas en el proceso de conexión */
  struct sockaddr_in raddr;
  struct hostent *hp;
 // struct timeval wait;
  /* variables auxiliares utilizadas en el proceso*/
  int sock;

  sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  bzero((char *)&raddr, sizeof(raddr));

  /* obtengo la direccion del host del servidor FTP a partir del nombre */
  //printf("Connection data:   %s, port %d \n",name,port);
  hp = gethostbyname(name);
  
  if(hp == NULL){
  return -1;
  }

 /* se inicializan los parametros del socket */

  memcpy(&(raddr.sin_addr), hp->h_addr, hp->h_length);
  raddr.sin_family = AF_INET;
  raddr.sin_port=htons(port);/* puerto al que me voy a conectar.*/
  //sock = RPC_ANYSOCK;

// se establece la conexión

  connect(sock, (struct sockaddr *) &raddr, sizeof(raddr));

//printf("El socket de datos en la conexión es %d\n",sock);
  
if (sock==-1) return -1; //no hemos podido establecer a conexión


return sock;
}


/****************************************************************
 ****************************************************************/
void ftp_close_connection_data(int sc)
{
	close(sc);
	//printf("Cerrada conexión de datos \n");
}





static int
select_fd (int fd, int maxtime, int writep)
{
  fd_set fds, exceptfds;
  struct timeval timeout;

  FD_ZERO (&fds);
  FD_SET (fd, &fds);
  FD_ZERO (&exceptfds);
  FD_SET (fd, &exceptfds);
  timeout.tv_sec = maxtime;
  timeout.tv_usec = 0;
  /* HPUX reportedly warns here.  What is the correct incantation?  */
  return select (fd + 1, writep ? NULL : &fds, writep ? &fds : NULL,
		 &exceptfds, &timeout);
}
/****************************************************************
 ****************************************************************/

void send_command(char * comand, int socket)
{
	//int i=0;
	int res = 0;
	int len = 0;
	
	len=strlen(comand);
	
//printf("El comando es: %s, length %i, socket %d\n",comand,len,socket);
	while (len>0)
	{
	do{
		do
		{
			res= select_fd(socket,10,1);//Espero 10 segundos
		}while (res ==-1 );
		
		if (res <=0)
		{
		//printf("%d\n",len);
		
			if (res ==0) printf("No se pudo mandar....");
			break;
		}
		
		res=write(socket,comand,len ); // envía la petición
	}
	while (res ==-1 );
	
	if (res <= 0)
	
	break;
	
	comand += res;
	len -= res;
	}
/*	
	printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>El tamaño de la respuesta es : %d, %d ", res, strlen(comand)+1);
	//fflush( socket);
	while(i<strlen(comand)+1){
		if((comand[i] != '\r') && (comand[i] != '\n')){
			printf("%c",comand[i]);
		}else{
			if(comand[i] == '\r'){
				printf("CR");
			}else{
				printf("LF\n");
			}
		}
		i++;
	}*/
}


/****************************************************************
 ****************************************************************/
int receive_answer(int socket, char * respuest)
{
	int i;
	int res=0;
	int fin =0;
	int salir=0;
	char resp[2048];
	char cod[4];
	char num[3];
	char slash ='-';

	resp[0]='\0';
	cod[0] ='\0';
	respuest[0]='\0';
	
	//printf("----%d--------------%s---------------\n",socket,resp);
	
	/* &resp[0] = resp */
	
	/*****************************************************************/
	
	
	do
    {
  
	  do
	    {
	      res = select_fd (socket, 10, 0);//pongo 10 segundos,porque 300 es el máximo de la conexión
	    }
	  while (res == -1);
	  if (res <= 0)
	    {
//	      if (res == 0)
	    //  printf("Pasado el timeout........,salimos,pero sin error");
	      return 0;
	    }

      res = read(socket,resp, sizeof(char)*1024);
    }
  while (res == -1);

	

	
	//res = read(socket, (char *) resp, sizeof(char)*1024);
	
	
//if (fin<=0) {
//printf("---------------------------------------problemas al leer el socket...\n");
//return -1;	
//}	
	/*****************************************************************/
//printf("El tamaño de la REspuesta es: %d..en el socket %d.\n",res,socket);
	resp[res]='\0';
	
	
	//strncpy(cod,resp,3); EL VALGRIND DICE QUE SE SUPERPONEN???
	
	cod[0]=resp[0];
	cod[1]=resp[1];
	cod[2]=resp[2];
	cod[3] ='\0';
	
	strcpy(respuest,resp);
	
//printf("El código de la respuesta es %i",atoi(cod));
	
	if (((strchr(resp,slash))!= NULL) && (atoi(cod)!=0))  {
//printf("Entro por la primera....\n");
       		fin=1;//esto es por si los mensajes llegan desordenados, así se si ya ha mandado todo porla conexión de control
		
		
		while (salir==0){
		cod[3]='\0';
		i=0;
	//printf("Entro pora la primera..2.%c.",resp[3]);
		while(i<res){
			//printf("%c\n",resp[i]);
			if((resp[i] != '\r') && (resp[i] != '\n')){
//				printf("%c",resp[i]);
			}else{
				if(resp[i] == '\r'){
//					printf("CR");
				}else{
//					printf("LF\n");
				}
			}
		i++;
		}

//		strncpy(resp,resp+3,strlen(resp)); Si borro el código de la respuesta luego no tengo forma de saber si es el findel mensaje multilinea


//printf("Entro por la primera...%c.\n",(strstr(resp,cod)+3)[0]);
//printf("RESP...COD..%s...%s....\n",cod,resp);
		
		//si el código se encuentra en la respuesta, sin contar con el código y no va seguido de -, he recibido la respuesta multilinea
		
		//if ((strstr(resp+3,cod)==NULL) || ((strstr(resp+3,cod)+3)[0]==' '))
		if (multiline(resp)==0)
		 {
			 
			 fin=1;//para que no lea más de la conexión. Damos por hecho que cuando lee - tras un código, es el fin de la transmisión
			 salir=1;
			 
//printf("Nos salimos de recibir Answer %d\n",fin);
		 }
		 else{
//printf("Leo la Siguiente línea\n");
			 resp[0]='\0';
			 res = read(socket, resp, sizeof(char)*1024);//a lo mejor tendría que poner un goto???
			 resp[res]='\0';
		 }
		}
	}
	else {
	//printf("SEgunda parte\n");
	i=0;
	num[0]='\0';
	//If I didn't copy resp now, after "while" it's content is a nonsense
	
		while(i<res){
			//printf("%c\n",resp[i]);
			if((resp[i] != '\r') && (resp[i] != '\n')){
				//puesto pq el servidor a veces nos manda las respuestas anidadas y no anidadas juntas.
				if ((resp[i] == '-')&&(atoi(strncpy(num,resp,i)))){
			//		printf("El número es %s\n",num);
					fin=1;
				}
//				printf("%c",resp[i]);
				//strcat(respuest,resp[i]);
			}else{
				if(resp[i] == '\r'){
//					printf("CR");
					//strcat(respuest,resp[i]);
				}else{
//					printf("LF\n");
					//strcat(respuest,resp[i]);
				}
			}
		i++;
		}
		resp[res]='\0';
	}
 	// recibe la respuesta
	//i++;
	
	
	//printf("---------------%s---------%s---------\n",respuest,resp);
	//printf("\n");
return fin;
}
/**************************************************************************************************************/
int receive_partial_data(int socket, char * respuest, int tam)
{
	int i;
	int res=0;
	char resp[1024];

	resp[0]='\0';
	//printf("---------------------------------\n");
//printf("El tamaño es %d",tam);
	if (tam >= 1024)
	{
		res = read(socket, (char *) &resp, sizeof(char)*1024);
		tam = tam -1024;
	}
	else
		{
			res = read(socket, (char *) &resp, sizeof(char)*tam);
			tam=0;
		}


//printf("El tamaño de la REsPuesta es: %d...",res);
	resp[res]='\0';
	i=0;
		//printf("Entro pora la primera..2.%c.",resp[3]);
		while(i<res){
			//printf("%c\n",resp[i]);
			if((resp[i] != '\r') && (resp[i] != '\n')){
//				printf("%c",resp[i]);
			}else{
				if(resp[i] == '\r'){
//					printf("CR");
				}else{
//					printf("LF\n");
				}
			}
		i++;
		}
 	// recibe la respuesta
	i++;
	strcpy(respuest,resp);
	//printf("---------------------------------\n");
	//printf("\n");
return tam;
}

/******************************************************************************/
int  get_port(char * respuest)
{
	int ip1, ip2, ip3, ip4, p1, p2 ,puerto;
	char s1[255];
	char *t1;

//printf("Hay que hacer el port de: %s",respuest);
	sprintf(s1,strstr(respuest,"(")+1);
//printf("El paréntesis está en: %s",s1);
	p1=0;
	p2=0;
	t1 = strtok(s1,",");
	if(t1 != NULL)
	{
//printf("IP1:%s\n",t1);
	  ip1=atoi(t1);
	}
	t1 = strtok(NULL, ",");
	if(t1 != NULL)
	{
// printf("IP2:%s\n",t1);
	  ip2=atoi(t1);
	}
	t1 = strtok(NULL, ",");
	if(t1 != NULL)
	{
//printf("IP3:%s\n",t1);
	  ip3=atoi(t1);
	}
	t1 = strtok(NULL, ",");
	if(t1 != NULL)
	{
//printf("IP4:%s\n",t1);
	  ip4=atoi(t1);
	}
	t1 = strtok(NULL, ",");
	if(t1 != NULL)
	{
//printf("P1:%s\n",t1);
	  p1=atoi(t1);
	}
	t1 = strtok(NULL, ")");
	if(t1 != NULL)
	{
//printf("P2:%s\n",t1);
	  p2=atoi(t1);
	}
	puerto=p1*256+p2;
//printf("El puerto es :%d\n",puerto);
	/*
	printf("La longitud de aux es: %d",strlen(aux));
	printf("lslslslslsl %c", aux);
//	p1=atoi(str
*/
	return puerto;
}/******************************************************************************/
int get_filename(char * ruta)
{
	char s1[255];

//printf("La ruta de la que hacer el fichero es: %s",ruta);

	while (atoi((strstr(ruta,"/")+1))<((int)strlen(ruta))){
		sprintf(s1,strstr(ruta,"/")+1);
//printf("La última barra: %s",s1);
	}
//printf("La última barra: %s",s1);

	/*t1 = strtok(s1,",");
	if(t1 != NULL)
	{
	  //printf("IP1:%s\n",t1);
	  ip1=atoi(t1);
	}
	puerto=p1*256+p2;*/
//printf("El fichero es :%s\n",s1);
	/*
	printf("La longitud de aux es: %d",strlen(aux));
	printf("lslslslslsl %c", aux);
//	p1=atoi(str
*/
strcpy(ruta,s1);
return 0;
}

/******************************************************************************/
int ftp_abor(int socket)
{
	char comand[255];
	char respuest[255];
	int state=0;
	int fin=0;

	strcpy(comand,"ABOR\r\n");

	strcpy(respuest,"");
	send_command(comand, socket);
	fin = receive_answer(socket,respuest);
	//tengo que hacer esto porque cuando el fichero es muy grande
	//le mando el ABOR, y no cierra inmediatamente la conexion de 
	//control, con lo cual luego se queda colgado esperando la 
	//respuesta.
	
	if (((strstr(respuest,TRANS_ABOR)!=NULL)||(strstr(respuest,LOCAL_TRANS_ABOR)!=NULL) )||(strstr(respuest,ABOR_OK_DATA)!=NULL))
	{
		//printf("Cerrada conexión de datos para ABOR\n");
		state=1;

	if (strstr(respuest,ABOR_OK)==NULL) fin = receive_answer(socket,respuest);
	}

	if (strstr(respuest,ABOR_OK)!=NULL)
	{

	//	printf("Tarea Finalizada\n");
		if ((((strstr(respuest,TRANS_ABOR)==NULL)||(strstr(respuest,LOCAL_TRANS_ABOR)==NULL) )||(strstr(respuest,ABOR_OK_DATA)==NULL))&&(state<1)){
			//printf("En el caso de recibir primero el 226 y esperar más???\n");
			//hay un caso especial, cuando al final no da tiempo a recibir los otros mensajes de error y
			//solo recibimos el 226. Por eso, y para no desacompasar, esperamos al timeout del
			//receive, pero si no recibimos nada, lo damos por válido. Para minimizar estos 
			//tiempos de espera, ponemos en el receive_answer un timeout muy pequeño.
			fin = receive_answer(socket,respuest);
			//printf("****************************************************\n");
			ftp_noop(socket);
		}
		state=1;
	}
	else 
	{
		fprintf(stderr,"Error en el servicio FTP %s\n",respuest);
		state =0;
	}
return state;
}


/******************************************************************************/
int ftp_login(int socket, char *login, char *pass, char * respuest)
{
	char comand[255];
	int state=0;
	int fin=0;

	sprintf(comand,"USER %s\r\n",login);

	strcpy(respuest,"");
	send_command(comand, socket);
	fin = receive_answer(socket,respuest);

	/*strcpy(comand,"PASS CirE;18D\r\n");

	send_command(comand, socket);
	fin = receive_answer(socket,respuest);	*/
	if (strstr(respuest,PASS_NEED)!=NULL)
	{
		strcpy(respuest,"");
//printf("Cadena encontrada y se necesita password\n");
		sprintf(comand,"PASS %s\r\n",pass);
		send_command(comand, socket);
		fin = receive_answer(socket,respuest);
	}
	if (strstr(respuest,OK_PASS_ACEP)!=NULL)
	{
	//MIRAR SI EL STR ESTÁ BIEN O NO

		//printf("Tarea Finalizada\n");
		state=1;
	}
	else 
	{
		fprintf(stderr,"Error en el servicio FTP %s\n",respuest);
		state =0;
	}
return state;
}

/******************************************************************************/
int ftp_list(char * Server,int socket,struct list_dir *lista,char * ruta,char * respuest)
{
	char comand[255];
	char respuest2[1024];
	int state=0;
	int puerto;
	int socket2=0;
	int fin=0;
	int fin2=0;
	int ret;
	

	strcpy(comand,"TYPE A\r\n");
	
	if (socket==-1) return -1;
        send_command(comand, socket);
	
	respuest[0]='\0';
//printf("*********************************El socket es %d, la respuesta es %p\n",socket, respuest);
	fin = receive_answer(socket,respuest);
//printf("*********************************El socket es %d, la respuesta es %p\n",socket, respuest);
	
	if (strstr(respuest,OK)!=NULL)
	{
	  respuest[0]='\0';

		strcpy(comand,"PASV\r\n");

		send_command(comand, socket);
		respuest[0]='\0';
		fin = receive_answer(socket,respuest);

		/*Esto es para la conexion activa
		
		socket2 = ftp_create_connection_data("163.117.148.85",1084);
		strcpy(comand,"PORT 163,117,148,85,4,60\r\n");
		send_command(comand, socket);
		fin = receive_answer(socket,respuest);
		strcpy(comand,"STAT \r\n ");
		send_command(comand,socket);
		fin = receive_answer(socket,respuest);
		receive_answer(socket2,respuest,fin);*/

	}
	if (strstr(respuest,PASV_ACEP)!=NULL)
	{

		puerto = get_port(respuest);
//printf("El servidor es %s ,el puerto es %d",Server,puerto);

 respuest[0]='\0';


		socket2 = ftp_create_connection_data(Server,puerto);
		if (socket2==-1) {
			printf("NO SE PUDO CREAR EL SOCKET");
			return 0;//no sé si debería ser un perror.....
		}
		
//printf("En el list ----%s----\n", ruta);
		if (strcmp(ruta,"")==0)
		{
			sprintf(comand,"LIST \r\n");
		}
		else sprintf(comand,"LIST %s\r\n",ruta);

//printf("El comando es: %s\n",comand);
		send_command(comand, socket);
//printf("El socket para los datos es %d \n",socket2);	
		fin = receive_answer(socket,respuest);
//printf("La respuesta es: %s\n",respuest);
		
		ret= LIST_DIR_init(lista);
		if (ret<0) return ret;
respuest2[0]='\0';
//printf("el fin vale %d\n",fin);	
		fin2 = receive_answer(socket2,respuest2);
		//receive_answer(socket2,respuest2,fin);
//printf("La respuesta es: %s\n",respuest);
		//RECIBO EL LIST POR LA CONEXION DE DATOS
		while (strcmp(respuest2,"")!=0){
//printf("La lista es %s\n",lista->first->data);
			fill_struct(lista,respuest2);
			fin2 = receive_answer(socket2,respuest2);
		}
		
//printf("Tras recibirlo todo, cierro el socket de la conexión de datos %d\n",socket2);
		ftp_close_connection_data(socket2);
	
		//RECIBO EL MENSAJE DE FIN DE LA TRANSMISIÓN
		if(fin==0) fin = receive_answer(socket,respuest);

//printf("Tras recibir el list, la respuesta es %s*********\n",respuest);

	if (lista!=NULL) 
	{
		ret = LIST_DIR_list(lista);
		lista->dir_pos=0;
	}
	if (ret<0) return ret;
	
	}

	if (strstr(respuest,FICH_OK)!=NULL)
	{
	  //	strcpy(respuest,"");
		//printf("Recibiendo LIST");
/*		strcpy(comand,"PASS CirE;18D\r\n");
		send_command(comand, socket);
		fin = receive_answer(socket,respuest);
		*/
	}
	if (strstr(respuest,CLOSE_DATOS)!=NULL)
	{
		//printf("Directorio OK\n");
		state=1;
		//ftp_close_connection_data(socket2);
	}
	else
	{	
		fprintf(stderr,"Error en el servicio FTP %s\n",respuest);
		state =0;
	}
	strcpy(respuest,"");//inicialización para despues....
return state;
}
/******************************************************************************/
/* pos es la posición desde la que queremos mandar y tam es el tamaño a mandar del 
fichero ruta*/
int ftp_store(char * Server,int socket,char * ruta, int pos, int tam, char * buffer, char * ruta_local, int f,char * exe_dir)
{
	char comand[255];
	char dir_temporal[MAXPATHLEN];
	char fich[1024];
	int state=0;
	int puerto=0;
	int socket2=0;
	int fin=0;
	int n=0;
	char respuest[1024];
	//FILE    *f;

	
//printf("***************************************El servidor es %s ",Server);

	if (strcmp(ruta,"")==0)
	{
		printf("La ruta no existe\n");	
		return 0;
	}
//Ver si es obligatorio que la recepcion se haga pasivamente

	strcpy(comand,"MODE S\r\n");//aunque yo quería que fuera con MODE B (modo bloque)
strcpy(respuest,"");
	
	send_command(comand,socket);
//printf("1\n");
	fin = receive_answer(socket,respuest);
	//receive_answer(socket, respuest, fin);
//printf("La respuesta al MODE es :%s",respuest);
  

	if (strstr(respuest,OK)!= NULL)
	{
		strcpy(comand,"TYPE I\r\n");
		strcpy(respuest,"");
		send_command(comand, socket);
		fin = receive_answer(socket,respuest);
	}
	
	//Ver donde puse las constantes...
	if (strstr(respuest,OK)!=NULL)
	{
		strcpy(respuest,"");
		strcpy(comand,"PASV\r\n");
		send_command(comand, socket);
		fin = receive_answer(socket,respuest);
	}
	else return 0;
	if (strstr(respuest,PASV_ACEP)!=NULL)
	{
		puerto = get_port(respuest);
//printf("El servidor es %s ",Server);
//printf("El puerto de datos es: %d \n",puerto);

		socket2 = ftp_create_connection_data(Server,puerto);
		if (socket2==-1) return -1;
		//socket2 = ftp_create_connection_data(server,20);
//printf("El socket de datos es: %d \n",socket2);
		//printf("En el list ----%s----\n", ruta);
		if (strcmp(ruta,"")!=0)
		{
		strcpy(respuest,"");
		sprintf(comand,"REST %d\r\n",pos);
		send_command(comand,socket);
		fin = receive_answer(socket,respuest);
		}
		
		if (strstr(respuest, REST_OK)!=NULL)
		{
		
		//en respuest está lo que hay que mandar al fichero.....
	 
		//el fichero es el que me pasaron en el open
		
		/*tmpnam(raiz);
		strcpy(raiz_local,raiz);
		strcat(raiz,ruta);
		strcpy(ruta_local,raiz);
		
		printf("El nombre del fichero es : %s,%s\n",ruta_local,raiz_local);
		*/
		//abrimos el fichero de datos y sino existe, lo creamos
		/*if ((f = fopen( ruta_local, "a+")) == NULL ){
			//creo el fichero temporal
		*/
		strcpy(dir_temporal,ruta_local);
//printf("El directorio temporal es: %s\n",dir_temporal);
		n= getDir(dir_temporal);
//printf("El directorio temporal es: %s\n",dir_temporal);
		chdir(dir_temporal);
		//printf("El directorio temporal es: %s\n",dir_temporal);
		
			if (f) 
			{
				open(ruta_local,O_EXCL|O_CREAT|O_TRUNC, 0777);
//printf("Antes del write\n");
				//si no es un fichero local, creo el fichero y lo relleno con lo que haya en buffer.
				write(f,buffer,tam);//prueba para ver si está abierto y existe
				//close(f);
				//le decimos que empiece a copiar desde la posición 0
				pos=0;
				
			}
			else return 0;
		/*}
		else
		{
			//printf("El fichero ya existe en local, se enviará de ese\n");
			
		}*/
		//Nos cambiamos al directorio temporal 
		//chdir(raiz_local);
		
		sprintf(comand,"STOR %s\r\n",ruta);

//printf("comando es: *********%s*********",comand);
		//get_filename(ruta);
		//lo abro de escritura y demás en binario

		
			send_command(comand, socket);
			//printf("El socket para los datos es %d \n",socket2);
			fin = receive_answer(socket,respuest);
			//acepta la conexion
			if (strstr(respuest,FICH_OK)!= NULL)
			{
//printf("Enviamos el fichero desde la posición %d con tamaño %d \n",pos,tam);
				//while (! feof(f)){
					fich[0]='\0';
//printf("Voy a coger desde la posición %d, con tamaño %d\n",pos, tam);
                    			n= lseek(f,pos, SEEK_SET);
					if (n==0)
					{
						if (tam==-1){//si la variable tamaño vale -1, es que se va a enviar la totalidad del fichero.
//printf("Me meto por la parte del while.......\n");
							n=1;//inicializo la variable....
							while (n>0)
							{
							tam=1024;
							n=read(f,fich,tam);
							fich[tam+1]='\0';
							//printf("El resultado es: %s , tamaño %d\n",fich,strlen(fich));
							send_command(fich,socket2);
							}
						}
						else
						{
							while(tam>0){
								//para estar segura que cojo todo lo que necesito
								if (tam>=1024) n=1024;
								else n= tam;
	//printf("El tamaño es : %d,%d \n",n,tam);
								read(f,fich,n);
								fich[n]='\0';
	//printf("El resultado es: %s , tamaño %d\n",fich,tam);
								send_command(fich,socket2);
								tam=tam-n;
								//para ver que mando lo que necesito, voy a mandar otra cosa que no sea fich	
								//send_command("esto ",socket2);
							}
						}
					}
					else return 0;
				//}
				//mando el fin de fichero
				//send_command("\r\n",socket2);
				ftp_close_connection_data(socket2);
//	printf("\n Ya hemos enviado el fichero\n");
				fin = receive_answer(socket,respuest);
				//receive_answer(socket2,respuest,fin);
    				state=1;
			}
			else return 0;
		}

	}
	else 
	{
		fprintf(stderr,"Error en el servicio FTP %s\n",respuest);
		return 0;
	}
	
	//Vuelvo al directorio de trabajo			
	chdir(exe_dir);	
	
return state;
}

/******************************************************************************/
/* pos es la posición desde la que queremos mandar y tam es el tamaño a recibir del 
fichero ruta*/
int ftp_retr(char * Server,int socket,char * ruta, int pos, int tam, char * respuest)
{
	char comand[255];
	char cadena[MAXPATHLEN];
	char respues_recv[1024];
	int state=0;
	int puerto=0;
	int socket2=0;
	int fin=0;
	int n=0;
	int total=0;//si la variable tamaño vale -1, es que se va a recibir la totalidad del fichero.
	
	
	total=tam;
	
	if (strcmp(ruta,"")==0)
	{
		return 0;
	}
//Ver si es obligatorio que la recepcion se haga pasivamente

	strcpy(comand,"TYPE I\r\n");
	strcpy(respuest,"");
	send_command(comand, socket);
	fin = receive_answer(socket,respuest);

	//Ver donde puse las constantes...

	if (strstr(respuest,OK)!=NULL)
	{
		strcpy(respuest,"");
		strcpy(comand,"PASV\r\n");
		send_command(comand, socket);
		fin = receive_answer(socket,respuest);
	}
	else 
	{
		//si ha ocurrido un error, lo volvemos a intentar, ya que hay veces que nos devuelve 2 226 si ser respuesta multiple
		strcpy(comand,"TYPE I\r\n");
	        strcpy(respuest,"");
	        send_command(comand, socket);
	        fin = receive_answer(socket,respuest);
		
		if (strstr(respuest,OK)!=NULL)
		{
			strcpy(respuest,"");
			strcpy(comand,"PASV\r\n");
			send_command(comand,socket);
			fin = receive_answer(socket,respuest);
		}
		else
		{	
			fprintf(stderr,"Error en el servicio FTP %s\n",respuest);
			return 0;
		}
	}
	if (strstr(respuest,PASV_ACEP)!=NULL)
	{
		puerto = get_port(respuest);
//printf("El servidor es %s ",Server);
//printf("El puerto de datos es: %d \n",puerto);

		socket2 = ftp_create_connection_data(Server,puerto);
//printf("El socket de datos es: %d \n",socket2);
		//printf("En el list ----%s----\n", ruta);
		if (strcmp(ruta,"")!=0)
		{
			strcpy(respuest,"");
			sprintf(comand,"REST %d\r\n",pos);
			send_command(comand,socket);
			fin = receive_answer(socket,respuest);
		}
		
		if (strstr(respuest, REST_OK)!=NULL)
		{
		
			//getcwd(cadena,MAXPATHLEN);	
			//chdir("/tmp");	
		
//printf("La ruta local es: %s\n",ruta);
			sprintf(comand,"RETR %s\r\n",ruta);

			//printf("comando es: %s \n",comand);
			//get_filename(ruta);
			//lo abro de escritura y demás en binario

//printf("El nombre del fichero es : %s\n",ruta);
			//abrimos el fichero de datos
			/*if ((f = fopen( ruta, "wb+")) == NULL )
			{
				printf("ERROR: no puedo abrir el fichero\n");
				f= tmpfile();
				if (f) printf("FICHERO TEMPORAL CREADO\n");
				else return 0;
			}
			else
			{
				fprintf(stderr,"Error en el servicio FTP %s\n",respuest);
				return 0;
			}*/
//printf("El comando es: %s\n",comand);
			send_command(comand, socket);
			//printf("El socket para los datos es %d \n",socket2);
			fin = receive_answer(socket,respuest);
			//si no nos devuelve 550, hay que comprobar que esté al principio de una linea
//			printf("La longitud es: %d, %d\n",strlen(respuest),strlen(strstr(respuest,NO_DISP)));
			//if ((strstr(respuest,NO_DISP)== NULL)&&(strlen(respuest)!=(strlen(strstr(respuest,NO_DISP)))))
			
			if (strstr(respuest,NO_DISP)== NULL)
			{
				if (total==-1) //voy a recibir todo el fichero
				{
					tam=1024;
					while (tam>1023){
					//si recibo menos de 1024, es que he llegado al fin del fichero
						respues_recv[0]='\0';
						tam=receive_partial_data(socket2,respues_recv,tam);
//	printf("El tamaño de salida es %d\n",tam);
						memcpy( respuest, respues_recv, tam );
						/*fseek(f, pos, SEEK_SET);
						fputs(respuest,f);*/
						//En respuest ya está en el buffer................
					}
				}
				else
				{
					while (tam>0){
						if (tam >= 1024) n=1024;
						else n= tam;
					
						tam=receive_partial_data(socket2,respues_recv,n);
//	printf("El tamaño de salida es %d\n",tam);
						memcpy( respuest, respues_recv, tam );
						tam=tam-n;
						/*fseek(f, pos, SEEK_SET);
						fputs(respuest,f);*/
					}
				}
//printf("Antes del close connection\n");				
				ftp_close_connection_data(socket2);
				ftp_abor(socket);
				if(fin==0) fin = receive_answer(socket,respuest);
				state=1;
//printf("Pasamos al directorio: %s\n",cadena);
				chdir(cadena);
			}
			else return 0;
				
		}
	}
	else 
	{
	//	fprintf(stderr,"Error en el servicio FTP %s\n",respuest);
		return 0;
	}
return state;
}


/******************************************************************************/
int ftp_cwd(int socket,char * ruta, char * respuest)
{
	char comand[255];
	int state=0;
	int fin=0;

	sprintf(comand,"CWD %s\r\n",ruta);

	strcpy(respuest,"");
//printf("El socket es : %d\n",socket);
	send_command(comand, socket);
	fin = receive_answer(socket,respuest);

	if (strstr(respuest,ACC_FICH_OK)!=NULL)
	{
	//MIRAR SI EL STR ESTÁ BIEN O NO

		//printf("Tarea Finalizada\n");
	}
	else {
		if (strstr(respuest,NO_DISP)!=NULL)
		{
	//		fprintf(stderr,"Error en el servicio FTP %s\n",respuest);
		}
	}
	state=1;

return state;
}

/******************************************************************************/
int ftp_mkd(int socket,char * ruta, char * respuest)
{
	char comand[255];
	int state=0;
	int fin=0;

	sprintf(comand,"MKD %s\r\n",ruta);

	strcpy(respuest,"");
	send_command(comand, socket);
	fin = receive_answer(socket,respuest);

	if (strstr(respuest,CREA_DIR_OK)!=NULL)
	{
	//MIRAR SI EL STR ESTÁ BIEN O NO

		//printf("Tarea Finalizada\n");
		state=1;
	}
	else
	{
		fprintf(stderr,"Error en el servicio FTP %s\n",respuest);
		state =0;
	}
return state;
}

/******************************************************************************/
int ftp_rmd(int socket,char * ruta, char * respuest)
{
	char comand[255];
	int state=0;
	int fin=0;

	sprintf(comand,"RMD %s\r\n",ruta);

	strcpy(respuest,"");
	send_command(comand, socket);
	fin = receive_answer(socket,respuest);

	if (strstr(respuest,ACC_FICH_OK)!=NULL)
	{
	//MIRAR SI EL STR ESTÁ BIEN O NO

		//printf("Tarea Finalizada\n");
		state=1;
	}
	else
	{
		fprintf(stderr,"Error en el servicio FTP %s\n",respuest);
		state =0;
	}
return state;
}

/******************************************************************************/
int ftp_rename(int socket,char * ruta, char * ruta2, char * respuest)
{
	char comand[255];
	int state=0;
	int fin=0;

	sprintf(comand,"RNFR %s\r\n",ruta);

	strcpy(respuest,"");
	send_command(comand, socket);
	fin = receive_answer(socket,respuest);

	if (strstr(respuest,REST_OK)!=NULL)
	{
	//MIRAR SI EL STR ESTÁ BIEN O NO

	sprintf(comand,"RNTO %s\r\n",ruta2);

	strcpy(respuest,"");
	send_command(comand, socket);
	fin = receive_answer(socket,respuest);
		if (strstr(respuest,ACC_FICH_OK)!=NULL)
		{

			//printf("Tarea Finalizada\n");
			state=1;
		}
		else
		{	
			fprintf(stderr,"Error en el servicio FTP %s\n",respuest);
			state =0;
		}
	}
	else
	{
		fprintf(stderr,"Error en el servicio FTP %s\n",respuest);
		state =0;
	}
return state;
}

/******************************************************************************/
int ftp_dele(int socket,char * ruta, char * respuest)
{
	char comand[255];
	int state=0;
	int fin=0;

	sprintf(comand,"DELE %s\r\n",ruta);

	strcpy(respuest,"");
	send_command(comand, socket);
	fin = receive_answer(socket,respuest);

	if (strstr(respuest,ACC_FICH_OK)!=NULL)
	{
	//MIRAR SI EL STR ESTÁ BIEN O NO

		//printf("Tarea Finalizada\n");
		state=1;
	}
	else
	{
		fprintf(stderr,"Error en el servicio FTP %s\n",respuest);
		state =0;
	}
return state;
}
/******************************************************************************/
int ftp_cdup(int socket, char * respuest)
{
	char comand[255];
	int state=0;
	int fin=0;

	strcpy(comand,"CDUP\r\n");

	strcpy(respuest,"");
	send_command(comand, socket);
	fin = receive_answer(socket,respuest);

	if (strstr(respuest,ACC_FICH_OK)!=NULL)
	{
	//MIRAR SI EL STR ESTÁ BIEN O NO

		//printf("Tarea Finalizada\n");
		state=1;
	}
	else
	{
		fprintf(stderr,"Error en el servicio FTP %s\n",respuest);
		state =0;
	}
return state;
}
/******************************************************************************/
int ftp_syst(int socket)
{
	char comand[255];
	char respuest[255];
	int state=0;
	int fin=0;

	strcpy(comand,"SYST\r\n");

	strcpy(respuest,"");
	send_command(comand, socket);
	fin = receive_answer(socket,respuest);

	if (strstr(respuest,SYST_OK)!=NULL)
	{
	//MIRAR SI EL STR ESTÁ BIEN O NO

		//printf("Tarea Finalizada\n");
		state=1;
	}
	else
	{
		fprintf(stderr,"Error en el servicio FTP %s\n",respuest);
		state =0;
	}
return state;
}
/******************************************************************************/
int ftp_noop(int socket)
{
	char comand[255];
	char respuest[255];
	int state=0;
	int fin=0;

	strcpy(comand,"NOOP\r\n");

	strcpy(respuest,"");
	send_command(comand, socket);
	fin = receive_answer(socket,respuest);

	if (strstr(respuest,OK)!=NULL)
	{
	//MIRAR SI EL STR ESTÁ BIEN O NO

		//printf("Tarea Finalizada\n");
		state=1;
	}
	else
	{
		fprintf(stderr,"Error en el servicio FTP %s\n",respuest);
		state =0;
	}
return state;
}


/******************************************************************************/
int ftp_quit(int socket)
{
	char comand[255];
	char respuest[255];
	int state=0;
	int fin=0;

	strcpy(comand,"QUIT\r\n");

	strcpy(respuest,"");
	send_command(comand, socket);
	fin = receive_answer(socket,respuest);

	if (strstr(respuest,QUIT_OK)!=NULL)
	{
	//MIRAR SI EL STR ESTÁ BIEN O NO

		//printf("Tarea Finalizada\n");
		state=1;
	}
	else
	{
		fprintf(stderr,"Error en el servicio FTP %s\n",respuest);
		state =0;
	}
return state;
}

/************************************************************
 * Gives back position of a data		    	    *
 ************************************************************/
int seek_data_list(struct element_dir *list,char * file)
{
	struct element_dir *aux;
	int i=0;
	aux=list;

	while(aux)
	{
		//printf("En el path hay %s, %s\n",aux->data,file);
		if (strstr(aux->data,file)!=NULL) return i;//tambien podría poner aux->dir_pos
		else 	aux= aux->next;
	i++;
	}
return -1;
}



/************************************************************
 * Gives back position of a data		    	    *
 ************************************************************/
char * seek_list(struct element_dir *list,int pos)
{
	struct element_dir *aux;
	
	int i=0;
		
	aux=list;
	while(aux)
	{
		//printf("En el path hay %s\n",aux->data);
		if (pos== i) return aux->data;//tambien podría poner aux->dir_pos
		else 	aux= aux->next;
		i++;
	}
return NULL;
}


/************************************************************
 * Fills structure of list			    	    *
 ************************************************************/
int fill_struct
(
                struct list_dir *list,
		char * respuest
)
{

	int i=0;
	int ret ;


	//struct element_dir *aux;
	char data[MAXPATHLEN];
	
	data[0]='\0';
	
//printf("Rellenando la lista...........%c--%c---%d------\n",respuest[i],respuest[i+1],strlen(respuest));
	
	if ((strlen(respuest)==2) && ((respuest[i] == '\r') && (respuest[i+1] == '\n')))
	{
//printf("Elimino la marca de Fin \n");
		ret = LIST_DIR_delete_mark(list);
		if (ret < 0) return ret ;
//printf("En el list queda %s",list->last->data);
		return 0;
	}
	
	while(i<(int)strlen(respuest)+1)
	{
		//printf("1\n");
		//busco la primera aparición del caracter \r
		if((respuest[i] != '\r') && (respuest[i] != '\n')){
			//printf("Copio en  %s\n",data);
			strncat(data,&respuest[i],1);
			
		}else{
			
			if(respuest[i] == '\r')
			{ 
				data[i]='\0';
				i++;
			//printf("Antes de crear el nodo %s",data);
			
				  ret = LIST_DIR_insert(list,data) ;
				  if (ret < 0) return ret ;
				data[0]='\0';	 
			}
			else {
//printf("Copio en  %s\n", data);
				strncat(data,&respuest[i],1);
			}
		}
		i++;
	}
	//Part of list hasn't copy in list
	if ((i=strlen(respuest)+1) && (strlen(data)>0))
	{
		strncat(data,END_CHAR,1);
		//printf("11 %s\n",data);
		//printf("Antes de crear el nodo %s",data);
			
		ret = LIST_DIR_insert(list,data) ;
		if (ret < 0) return ret ;
	}
	

return 0;
}





