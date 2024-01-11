
  /*
   *  Copyright 2020-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos
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

#include "nfi/nfi_nfs/nfs.h"

void printfh(fhandle fh)
{
	unsigned int i,j;

	printf("FH: ");
	for(j=0;j<31;j++){
		i = fh[j]+256;
		printf("%u:", (unsigned int)(i%256));
	}
	i = fh[31]+256;
	printf("%u",i%256);
	printf("\n");
}

/*int getFirstDir(char *dir, char *path)
{
	int i, j;
	
  	//printf("++ path = %s ++\n", path);
	
	i = 0;
	while((path[i] != '\0')&&(path[i] != '/')){
                i++;
        }
	strncpy(dir, path , i);

	dir[i] = '\0';
	
  	//printf("++ dir = %s ++\n",dir);	
	
	
	while((path[i] != '\0')&&(path[i] == '/')){		
		i++;
	}
	
	j = 0;
        while(path[i+j] != '\0'){

	        path[j] = path[i+j];
		j++;
        }
	
        path[j] = '\0';
  	//printf("++ dir = %s path = %s ++\n",dir, path);	
	return j;
}*/


/****************************************************************
 * Funcion: create_conection_mount				*
 * Funcion declarada en la interfaz nfs_mount.h			*
 *								*
 * Esta función crea una conexión entre el cliente ya el	*
 * servidor MNT. El protocolo de conexión establecida puede	*
 * ser TCP o UDP, aunque por defecto se utiliza UDP.		*
 * Esta conexión solo puede ser utilizada por un proceso a	*
 * la vez.							*
 *								*
 * Entrada: nombre del servidor NFS				* 
 * Salida: Un puntero a una estructura CLIENT (es la conexion	*
 * realizada). Si devuelve NULL es que a ocurrido un error en	*
 * el proceso.							*
 ****************************************************************/
CLIENT* create_connection_mount(char *name, int type)
{ 
  /* puntero a la conexión*/
  CLIENT *cli;  
  /* estructuras utilizas en el proceso de conexión */
  struct sockaddr_in raddr;
  struct hostent *hp;
  struct timeval wait; 
  /* variables auxiliares utilizadas en el proceso*/
  int sock,ret;
  char s[MNTNAMLEN];

  
#ifdef DEBUG_MNT
  printf("Ready to do creation connection with MNT service\n");
#endif     
  /*  
      Esta es la manera original de realizar la conexión
      cli=clnt_create(name, MOUNTPROG ,MOUNTVERS ,"udp");
  */
  
  /* obtengo la direccion del host del servidor NFS a partir del nombre */
  hp = gethostbyname(name);
  if(hp == NULL){
    return NULL;
  }
  /* se inicializan los parametros del socket */
  bzero((char *)&raddr, sizeof(raddr));
  raddr.sin_family = AF_INET;
  memcpy(&(raddr.sin_addr), hp->h_addr, hp->h_length);
  raddr.sin_port=htons(0);
  sock = RPC_ANYSOCK;

  if(type == NFS_TCP){
    /* con esta funcion se realiza la creacion de la conexion tcp */
#ifdef DEBUG_MNT
    printf("Creation connection TCP client with MNT service\n");
#endif
    cli = clnttcp_create(&raddr, MOUNT1_PROGRAM, MOUNT1_VERSION, &sock, SENDSZ, RECVSZ );
  }else{
    if(type == NFS_UDP){
      /* tiempo maximo de espera entre la petición de cliente */
      /* y la respuesta del servidor segundos */
      wait.tv_sec=1;
      /* milisegundos */
      wait.tv_usec=0;
      /* con esta funcion se realiza la creacion de la conexion udp */
#ifdef DEBUG_MNT
      printf("Creation connection UDP client with MNT service\n");
#endif
      cli=clntudp_bufcreate(&raddr, MOUNT1_PROGRAM, MOUNT1_VERSION, wait, &sock, SENDSZ, RECVSZ);
    }
    else{
      cli=NULL;
    }
  }
  
  if(cli==NULL){
#ifdef DEBUG_MNT
    printf("Error connection MNT service\n");
#endif
    return cli;
  }
  
  /* se elimina la autenticacion que  existe por defecto */
  auth_destroy(cli->cl_auth);
  /* se obtiene el nombre del host del cliente para añadir a los parametros de autenticacion */
  ret = gethostname(s, 200);
  if(ret < 0){
    close_connection_mount(cli);
    return NULL;
  }
  /* con esto se añade cierta seguridad a la comunicación entre el cliente y el servidor */ 
  /* el tipo de seguridad utilizada es UNIX (vease la Rfc de las RPCs para mas información)*/
  cli->cl_auth=authunix_create(s,getuid(),getgid(),0,NULL); 

#ifdef DEBUG_MNT
      printf("cl: %p \n",cli);
#endif
  return cli;
}



/****************************************************************
 * Funcion: close_conection_mount				*
 * Funcion declarada en la interfaz nfs_mount.h	        *
 *								*
 * Esta función elimina una conexión realizada a un		*
 * servidor MNT.						*
 *								*
 * Entrada: puntero a la estructura CLIENT.			*
 * Salida: no se devuelve ninguna salida.			*
 ****************************************************************/
void close_connection_mount(CLIENT *cl)
{
  /* elimino la autenticacion */
  auth_destroy(cl->cl_auth);

  /* elimino la estructura */
  clnt_destroy(cl);

  /* la inicializo */
  // cl=NULL; // <- for that you need CLIENT **cl and *cl=NULL...

#ifdef DEBUG_MNT
  printf("Close connection MNT\n");
#endif
} 

/****************************************************************
 * Funcion: nfs_mount						*
 * Funcion declarada en la interfaz nfs_mount.h		*
 *								*
 * Esta función obtiene el manejador inicial. Se obtiene	*
 * montando el directorio pasado por parametro.			*
 *								*
 * Entradas:							*
 *	- el path absoluto del cual se desea obtener		*
 *	  el manejador.						*
 *	- un manejador, que sera el manejador del path		*
 *        en caso de tener exito la llamada.			*
 *	- puntero a la estructura CLIENT (es decir,		*
 *	  la conexion MNT).					*
 *								*
 * Salida: Un entero que indica error en caso de ser negativo,	*
 *		   y exito en caso de ser igual a 0.  		*
 ****************************************************************/
int nfs_mount(char *dir, fhandle fhand, CLIENT *cli )
{	
  fhstatus fh;
  int res;
  
#ifdef DEBUG_MNT
  printf("Ready to do MOUNT (%s)\n",dir);
#endif
  /**************************************************************** 
   * los argumentos de la llamada MNT son :			  *
   *	ENTRADA:					          *
   *	- el path que se desea montar				  *
   *	SALIDA:							  *
   *	- una estructura con un status y el manejador en caso 	  *
   *	  de ser el status igual la NFS_OK			  *
   ****************************************************************/
  res = mountproc_mnt_1(&dir,&fh,cli);
  /* comprobamos el estado de la conexión */
  if(res != NFS_OK){
    
#ifdef DEBUG_MNT
    printf("Error in MOUNT (%s) STATUS_CONNECTION (%d)\n",dir,res);
#endif
    /* en caso de error de conexion devolvemos un error generico 
       de error de conexión */
    return NFSERR_CONNECT;
  }
  else{	 	  
    if(fh.status != NFS_OK){
      /* en caso de que fh.status sea distinto a NFS_OK, *
       * devolvemos el error pero con valor negativo     */
#ifdef DEBUG_MNT
      printf("Error in MOUNT (%s) STATUS (%d)\n",dir,res);
#endif
      return -(int)fh.status;
    }          
  }
  /* en caso de exito */
  /* copiamos el manejador obtenido al manejador pasado por parametro */
  memcpy(fhand, fh.fhstatus_u.directory, FHSIZE);
#ifdef DEBUG_MNT
  printf("Successfully MOUNT (%s) STATUS (%d)\n",dir,res);
  printf("fhand: "); printfh(fhand);
#endif
  return NFS_OK;
}

/****************************************************************
 * Funcion: nfs_umount						*
 * Funcion declarada en la interfaz nfs_mount.h		*
 *								*
 * Esta función elimina la entrada en la tabla de path's	*
 * montados en el servidor.					*
 *								*
 * Entradas:							*
 *	- el path absoluto de un directorio montado		*
 *	- puntero a la estructura CLIENT (es decir,		*
 *	  la conexion MNT).					*
 *								*
 * Salida: Un entero que indica error en caso de ser negativo,	*
 *		   y exito en caso de ser igual a 0.		*
 ****************************************************************/
int nfs_umount(char *path, CLIENT *cli)
{
  int res;
  
#ifdef DEBUG_MNT
  printf("Ready to do UMOUNT (%s)\n",path);
#endif
  /**************************************************************** 
   * los argumentos de la llamada MNT son :			  *
   *	ENTRADA:						  *
   *	- el path que se desea desmontar			  *
   ****************************************************************/
  res = mountproc_umnt_1(&path, NULL, cli);
#ifdef DEBUG_MNT
  printf("Error in UMOUNT (%s) STATUS_CONNECTION (%d)\n", path, res);	
#endif
  
  if(res != NFS_OK)
    /* en caso de que ocurra un error en la conexion se 
       devuelve este codigo de error */
    return NFSERR_CONNECT;
  else
#ifdef DEBUG_MNT
    printf("Successfully UMOUNT (%s) STATUS (%d)\n",path,res);
#endif
  return NFS_OK;
}
 
/****************************************************************
 * Funcion: nfs_export						*
 * Funcion declarada en la interfaz nfs_mount.h		*
 *								*
 * Esta función recoge todos los directorios exportados por un	*
 * servidor.							*
 *								*
 * Entradas:							*
 *	- un puntero a una estructura de tipo exports, que	*
 *	  en caso de exito, apuntara a una lista que		*
 *	  contiene los directorios esportados por el		*
 *	  servidor, junto a los permisos asociados al		*
 *	  directorio.						*
 *	- puntero a la estructura CLIENT (es decir,		*
 *	  la conexion MNT).					*

 * Salida: Un entero que indica error en caso de ser negativo,	*
 *	   y exito en caso de ser igual a 0.			*
 ****************************************************************/
int nfs_export(exports *exp, CLIENT *cli)
{  
  
  int res;
#ifdef DEBUG_MNT
  printf("Ready to do EXPORT ()\n");
#endif 
  
  /**************************************************************** 
   * los argumentos de la llamada MNT son :			  *
   *	SALIDA:							  *
   *	- la lista de directorios exportados del servidor	  *
   ****************************************************************/
  res = mountproc_export_1(NULL,exp,cli);  

  

	  if(res!=NFS_OK){
#ifdef DEBUG_MNT
    /* en caso de que ocurra un error en la conexion se devuelve este codigo de error */
	printf("Error in EXPORT STATUS_CONNECTION (%d)\n",res);
  #endif  
    return NFSERR_CONNECT;
	  }
  else{
#ifdef DEBUG_MNT
    printf("Successfully EXPORT STATUS (%d)\n",res);
  #endif  
    return NFS_OK;
  }
}


/****************************************************************
 * Funcion: create_conection_nfs				*
 * Funcion declarada en la interfaz nfs_nfs.h		        *	
 *								*
 * Esta función crea una conexión entre el cliente ya el	*
 * servidor NFS. El protocolo de conexión establecida puede	*
 * ser TCP o UDP, aunque por defecto se utiliza UDP.		*
 * Esta conexión solo puede ser utilizada por un proceso a	*
 * la vez.							*
 *								*
 * Entrada: nombre del servidor NFS			        *
 * Salida: Un puntero a una estructura CLIENT (es la conexion	*
 * realizada). Si devuelve NULL es que a ocurrido un error en	*
 * el proceso.							*
 ****************************************************************/
CLIENT* create_connection_nfs(char *name, int type)
{ 
  /* puntero a la conexión*/
  CLIENT *cli;  
  /* estructuras utilizas en el proceso de conexión */
  struct sockaddr_in raddr;
  struct hostent *hp;
  struct timeval wait; 
  /* variables auxiliares utilizadas en el proceso*/
  int sock,ret;
  char s[MNTNAMLEN];
  
#ifdef DEBUG_NFS
  printf("Ready to do creation connection with NFS service\n");
#endif
  /*   
       Esta es la manera original de realizar la conexión
       cli=clnt_create(name, NFS_PROGRAM ,NFS_VERSION ,"udp"); 
  */
  
  
  /* obtengo la direccion del host del servidor NFS a partir del nombre */
  hp = gethostbyname(name);
  if(hp == NULL){
    return NULL;
  }
  /* se inicializan los parametros del socket */
  //bzero((char *)&raddr, sizeof(raddr));
  raddr.sin_family = AF_INET;
  memcpy(&(raddr.sin_addr), hp->h_addr, hp->h_length);
  raddr.sin_port=htons(0);
  sock = RPC_ANYSOCK;
  
  
  if(type == NFS_TCP){
    /* con esta funcion se realiza la creacion de la conexion tcp */
#ifdef DEBUG_NFS
    printf("Creation connection TCP client with NFS service\n");
#endif
    cli = clnttcp_create(&raddr, NFS2_PROGRAM, NFS2_VERSION, &sock, SENDSZ, RECVSZ );
  }else{
    if(type == NFS_UDP){
      /* tiempo maximo de espera entre la petición de cliente */
      /* y la respuesta del servidor segundos */
      wait.tv_sec=1;
      /* milisegundos */
      wait.tv_usec=0;
      /* con esta funcion se realiza la creacion de la conexion udp */
#ifdef DEBUG_NFS
      printf("Creation connection UDP client with NFS service\n");
#endif
      cli=clntudp_bufcreate(&raddr, NFS2_PROGRAM, NFS2_VERSION, wait, &sock, SENDSZ, RECVSZ);
    }
    else{
      cli=NULL;
    }
  }
  
  if(cli==NULL){
#ifdef DEBUG_NFS
    printf("Error connection NFS service\n");
#endif
    return cli;
  }
  
  /* se elimina la autenticacion que  existe por defecto */
  auth_destroy(cli->cl_auth);
  /* se obtiene el nombre del host del cliente para añadir a los parametros de autenticacion */
  ret = gethostname(s, 200);
  if(ret < 0){
     close_connection_nfs(cli);
     return NULL;
  }
  /* con esto se añade cierta seguridad a la comunicación entre el cliente y el servidor */ 
  /* el tipo de seguridad utilizada es UNIX (vease la Rfc de las RPCs para mas información)*/
  cli->cl_auth=authunix_create(s,getuid(),getgid(),0,NULL); 
  return cli;
}


/****************************************************************
 * Funcion: close_conection_nfs					*
 * Funcion declarada en la interfaz nfs_nfs.h			*
 *								*
 * Esta función elimina una conexión realizada a un	        *
 * servidor NFS.						*
 *								*
 * Entrada: puntero a la estructura CLIENT.			*
 * Salida: no se devuelve ninguna salida.			*
 ****************************************************************/
void close_connection_nfs(CLIENT *cl)
{
  /* elimino la autenticacion */
  auth_destroy(cl->cl_auth);

  /* elimino la estructura */
  clnt_destroy(cl);

  /* la inicializo */
  //cl=NULL; <- for that, CLIENT **cl and *cl=NULL

#ifdef DEBUG_NFS
  printf("Close connection NFS\n");
#endif
} 


/****************************************************************
 * Esta funcion inicializa la estructura timevalNfs.            *
 * Entradas:                                                    *
 *      - un puntero a la estructura a rellenar.                *
 ****************************************************************/
void setDate(timevalNfs *t){

  t->seconds=0;
  t->useconds=0;
}

/****************************************************************
 * Esta funcion inicializa la estructura sattr.                 *
 * Entradas:                                                    *
 *      - un puntero a la estructura a rellenar.                *
 *      - los permisos que el fichero va a tener                *
 ****************************************************************/
void setAttr( sattr *at,unsigned int mode){
  
  at->mode = mode;
  at->uid = getuid();
  at->gid = getgid();
  at->size = 0;
  setDate(&(at->atime));
  setDate(&(at->mtime));
  
}


/****************************************************************
 * Funcion: nfs_setattr					*
 * Funcion declarada en la interfaz nfs_nfs.h		        *
 *								*
 * Esta función incorpora los atributos a un objeto del sistema *
 * de ficheros del cual se tiene el manejador.                  *
 *								*
 * Entradas:							*
 *	- un manejador, que sera el manejador delobjeto del que *
 *        se desea obtener los atributos.		        *
 *      - un puntero a una estructura de tipo fatt (reservada   *
 *        por el usuario) que sera rellenada en caso de tener   *
 *        exito la llamada.		                        *
 *	- puntero a la estructura CLIENT (es decir,		*
 *	  la conexion NFS).					*
 *								*
 * Salida: Un entero que indica error en caso de ser negativo,	*
 *	   y exito en caso de ser igual a 0.  		        *
 ****************************************************************/
int nfs_setattr(fhandle fh, fattr *fatt, CLIENT *cl){
    attrstat at;
    int res;
    sattrargs sat;
    
#ifdef DEBUG_NFS
    printf("Ready to do SETATTR ()\n");
#endif
  
  memcpy(sat.file, fh, FHSIZE); 
  sat.attributes.mode   = fatt->mode;
  sat.attributes.uid    = fatt->uid;
  sat.attributes.gid    = fatt->gid;
  sat.attributes.size   = fatt->size;
  memcpy(&sat.attributes.atime, &(fatt->atime), sizeof(timevalNfs));
  memcpy(&sat.attributes.mtime, &(fatt->mtime), sizeof(timevalNfs));
  
  if((res = nfsproc_setattr_2(&sat, &at, cl)) != NFS_OK){
#ifdef DEBUG_NFS
      printf("Error in SETATTR () STATUS_CONNECTION (%d)\n",res);
#endif
      return NFSERR_CONNECT;
    }
    
    if(at.status != NFS_OK){
#ifdef DEBUG_NFS
       printf("Error in SETATTR () STATUS (%d)\n",-(int)at.status);
#endif
       return -(int)at.status;
    }
    
    memcpy(fatt, &at.attrstat_u.attributes, sizeof(fattr));
    
#ifdef DEBUG_NFS
    printf("successfully SETATTR () STATUS (%d)\n",-(int)at.status);
#endif
  return NFS_OK;
    
}
	



/****************************************************************
 * Funcion: nfs_getattr					*
 * Funcion declarada en la interfaz nfs_nfs.h		        *
 *								*
 * Esta función obtiene los atributos del objeto del sistema    *
 * de ficheros del cual se tiene el manejador.                  *
 *								*
 * Entradas:							*
 *	- un manejador, que sera el manejador delobjeto del que *
 *        se desea obtener los atributos.		        *
 *      - un puntero a una estructura de tipo fatt (reservada   *
 *        por el usuario) que sera rellenada en caso de tener   *
 *        exito la llamada.		                        *
 *	- puntero a la estructura CLIENT (es decir,		*
 *	  la conexion NFS).					*
 *								*
 * Salida: Un entero que indica error en caso de ser negativo,	*
 *	   y exito en caso de ser igual a 0.  		        *
 ****************************************************************/
int nfs_getattr(fhandle fh, fattr *fatt, CLIENT *cl)
{
  attrstat at;
  int res;

#ifdef DEBUG_NFS
    printf("Ready to do GETATTR ()\n");
#endif
  /**************************************************************** 
   * los argumentos de la llamada GETATTR son :			  * 
   *	ENTRADA:						  *
   *	- un manejador valido                                     *
   *    SALIDA:                                                   *
   *    - una estructura con los atributos del objeto del sistema *
   *      de ficheros al cual se refire el manejador de la        *
   *      entrada.                                                *
   ****************************************************************/  
  if((res = nfsproc_getattr_2(fh, &at, cl)) != NFS_OK){
#ifdef DEBUG_NFS
    printf("Error in GETATTR () STATUS_CONNECTION (%d)\n",res);
#endif
    return NFSERR_CONNECT;
  }
  if(at.status != NFS_OK){
#ifdef DEBUG_NFS
    printf("Error in GETATTR () STATUS (%d)\n",-(int)at.status);
#endif
    return -(int)at.status;
  }  
  memcpy(fatt, &at.attrstat_u.attributes, sizeof(fattr));
#ifdef DEBUG_NFS
  printf("successfully GETATTR () STATUS (%d)\n",-(int)at.status);
#endif   
  return NFS_OK;
}

/****************************************************************
 * Funcion: nfs_lookup					        *
 * Funcion declarada en la interfaz nfs_nfs.h		        *
 *								*
 * Esta función obtiene el manejador de un path pasado por      *
 * parametro, mediante el manejador del directorio que contiene *
 * ese path.                                                    *
 *								*
 * Entradas:							*
 *	- el manejador del directorio.		                *
 *      - el path del cual se quiere el nuevo manejador.        *
 *      - el nuevo manejador.                                   *
 *      - un puntero a una estructura de tipo fatt (reservada   *
 *        por el usuario) que sera rellenada en caso de tener   *
 *        exito la llamada en caso de tener un valer distinto   *
 *        a NULL.		                                *
 *	- puntero a la estructura CLIENT (es decir,		*
 *	  la conexion NFS).					*
 *								*
 * Salida: Un entero que indica error en caso de ser negativo,	*
 *	   y exito en caso de ser mayor o igual a 0 (en este    *
 *         caso, indica el tipo de objeto del sistema de        *
 *         ficheros del cual se ha conseguido el manejador).    *
 ****************************************************************/
int nfs_lookup(fhandle fhin, char *path , fhandle fhout, fattr *att, CLIENT *cl)
{
  /* argumento de entrada en la llamada RPC*/
  diropargs arg;
  /* argumento de salida */
  diropres res;   
  char path_aux[MNTNAMLEN], dir_aux[MNTNAMLEN];
  int ret, end;
  fhandle fh;
  
#ifdef DEBUG_NFS
      printf("Ready to do LOOKUP (%s)\n",path);
      printf("fhin: "); printfh(fhin);
      printf("cl: %p \n",cl);
#endif 
  
  if((strcmp(path,"")==0)||(strcmp(path,"/")==0)){
  	memcpy(fhout, fhin, FHSIZE);
#ifdef DEBUG_NFS
        printf("Successfully LOOKUP (%s) STATUS (%d)\n",path,0);
        printf("fhout: "); printfh(fhout);
#endif 
	return NFS_OK;	  
  }
  memcpy(fh, fhin, FHSIZE);
  
  strcpy(path_aux, path);
  memcpy(arg.dir, fhin, FHSIZE);
  ret = -1;

  do{ 
	  /* se inicializan los argumentos de entrada */
	  memcpy(arg.dir, fh, FHSIZE);
	  /* separar ultimo dir del path */
	  end = getFirstDir(dir_aux, path_aux);
	  arg.name = dir_aux;
	  //printf("dir_aux = %s strlen = %d\n",arg.name,strlen(arg.name));
	  if(strlen(arg.name) == 0){
		  continue;
	  }
		  
	  /**************************************************************** 
	   * los argumentos de la llamada LOOKUP son :			  * 
	   *	ENTRADA:						  *
	   *	- una estructura con el manejador y un path               *
	   *    SALIDA:                                                   *
	   *    - una estructura con el manejador y los atributos         *
	   ****************************************************************/
	  ret=nfsproc_lookup_2(&arg, &res, cl);
	  //printf("ret = %d end = %d res.status = %d\n",ret, end,res.status);
	  if (ret != NFS_OK){
		
#ifdef DEBUG_NFS
    		printf("Error connection in LOOKUP (%s) STATUS (%d)\n", path, ret);
#endif
		return -1;
	  }
	  if (res.status!= 0){
		//printf("salgo de la condicion\n");
		break;
	  }
	  if(end == 0){
      	  	memcpy(fhout, res.diropres_u.fhand_attr.file, FHSIZE);		
	  }else{
		if(strcmp(path_aux,"/") == 0){
			memcpy(fhout, res.diropres_u.fhand_attr.file, FHSIZE);
			end = 0;
		}else{
			memcpy(fh, res.diropres_u.fhand_attr.file, FHSIZE);
		}
	  }
	   
  }while(end != 0);

  if(ret == NFS_OK)
  	if(res.status == NFS_OK){     
	      memcpy(fhout, res.diropres_u.fhand_attr.file, FHSIZE);      
	      if(att != NULL){
		/* si el puntero no es NULL, se rellena la estructura */
#ifdef DEBUG_NFS
      printf("Copy attributes in LOOKUP (%s)\n",path);
#endif 
		memcpy(att, &res.diropres_u.fhand_attr.attributes, sizeof(fattr));
      	      }
#ifdef DEBUG_NFS
      printf("Successfully LOOKUP (%s) STATUS (%d)\n",
	     path, res.diropres_u.fhand_attr.attributes.type);
      printf("fhout: "); printfh(fhout);
#endif 
      return res.diropres_u.fhand_attr.attributes.type;           
    }  
    else{

#ifdef DEBUG_NFS
    printf("Error in LOOKUP (%s) STATUS (%d)\n", path, -(int)res.status);
#endif
      return -(int)res.status;
    }
  else{	  
#ifdef DEBUG_NFS
    printf("Error in LOOKUP (%s) CONNECT_STATUS (%d)\n", path, ret);
#endif    
    return NFSERR_CONNECT;   
  } 
}




/****************************************************************
 * Funcion: nfs_read				                *
 * Funcion declarada en la interfaz nfs_nfs.h		        *
 *								*
 * Esta función lee los datos de un fichero                     *
 *								*
 * Entradas:							*
 *	- el manejador del fichero.		                *
 *      - el offset del fichero, que indica desde donde empezar *
 *        a leer.                                               *
 *      - el buffer de datos donde se guardan los datos leidos. *
 *      - el tamaño de los datos que se desean leer.            *
 *	- puntero a la estructura CLIENT (es decir,		*
 *	  la conexion NFS).					*
 *								*
 * Salida: Un entero que indica error en caso de ser negativo,	*
 *	   y exito en caso de ser mayor o igual a 0. En caso de *
 *         exito el valor devuelto es el valor de bytes leidos. *
 *         Si se leen menos bytes de los pedidos, es que se ha  *
 *         llegado al final de fichero.                         *
 ****************************************************************/
ssize_t nfs_read(fhandle fh, void *data, off_t offset, size_t size, CLIENT *cl)
{
  /* argumento de entrada a la llamada RPC */
  readargs args;
  /* argumento de salida de la llamada RPC */
  readres res; 
  size_t i, rd; 
  int ret;
  
#ifdef DEBUG_NFS
  printf("Ready to do READ (%d,%d)\n", (uint)size,(uint) offset);
#endif 
  
  /* Si se quieren leer 0 bytes o menos, directamente se sale de la función */
  if(size <= 0){
#ifdef DEBUG_NFS
    printf("successfully READ (%d, %d) STATUS (%d)\n", (uint)size, (uint)offset, (uint)size);
#endif   
    return 0;
  }
  /* MAXDATA en el tamaño maximo que se puede leer en cada llamada RCP */
  
  /* este contador indica cuantos bytes se han leido */
  /* inicialmente es igual 0 */
  i=0;
  /* mientras el tamaño de lo escrito sea menor al tamaño pedido */
  while(i < size){
    /* si lo que queda por leer es mayor a MAXDATA, el tamaño a leer es
       MAXDATA */
    if((size - i) >= MAXDATA) 
      rd = MAXDATA; 
    else{ 
      /* si es menor, el tamaño a leer en la llamada RPC es size - i*/
      rd = size - i; 
    }
    /* se rellenan los argumentos de entrada */
    memcpy(args.file, fh, FHSIZE);
    args.offset = offset + i;
    args.count = rd;
    res.readres_u.fich_read.data.nfsdata_val = (char *)data + i;

  /****************************************************************
   * los argumentos de la llamada READ son :			  *
   *	ENTRADA:						  *
   *	- una estructura con el manejador, offset, y tamaño que   *
   *      se desea leer.                                          *
   *    SALIDA:                                                   *
   *    - una estructura con los datos leido y el tamaño leido.   *
   ****************************************************************/      
    ret = nfsproc_read_2(&args, &res, cl);
    
    if(ret != NFS_OK){

#ifdef DEBUG_NFS_ERR
      printf("Error in READ (%d,%d) CONNECT_STATUS (%d)\n", (uint)size, (uint)offset, ret);
#endif     
      return NFSERR_CONNECT;
    }
    else{        
      if(res.status != NFS_OK){	 
#ifdef DEBUG_NFS_ERR
      printf("Error in READ (%d,%d) STATUS (%d)\n", (uint)size, (uint)offset, -(int)res.status);
#endif   
  	  return -(ssize_t)(res.status);	
      }
      else{ 
	/* incremento lo leido */
	i = res.readres_u.fich_read.data.nfsdata_len + i; 	
	/* si lo leido < que lo que se pide -> EOF */
	if(res.readres_u.fich_read.data.nfsdata_len < rd){
#ifdef DEBUG_NFS
	  printf("successfully READ (%u, %u) STATUS (%lu)\n", (unsigned)size, (unsigned)offset, (unsigned long)i);
#endif 
	  return (ssize_t)i;
	}
      }
    }
    
  }
#ifdef DEBUG_NFS
  printf("successfully READ (%u, %u) STATUS (%lu)\n", (unsigned)size, (unsigned)offset, (unsigned long)i);
#endif  
  
  
  return (ssize_t)i;
}



/****************************************************************
 * Funcion: nfs_write				                *
 * Funcion declarada en la interfaz nfs_nfs.h		        *
 *								*
 * Esta función escribe datos en un fichero.                    *
 *								*
 * Entradas:							*
 *	- el manejador del fichero.		                *
 *      - el offset del fichero, que indica desde donde empezar *
 *        a escribir.                                           *
 *      - el buffer con los datos que se debean ecribir.        *
 *      - el tamaño de los datos que se desean escribir.        *
 *	- puntero a la estructura CLIENT (es decir,		*
 *	  la conexion NFS).					*
 *								*
 * Salida: Un entero que indica error en caso de ser negativo,	*
 *	   y exito en caso de ser mayor o igual a 0. En caso de *
 *         exito el valor devuelto es el valor de bytes leidos. *
 *         Si se leen menos bytes de los pedidos, es que se ha  *
 *         llegado al final de fichero.                         *
 ****************************************************************/
ssize_t nfs_write(fhandle fh, void *data, off_t offset, size_t size, CLIENT *cl)
{
  /* argumento de entrada a la llamada RPC */
  writeargs argsWrite;
  /* argumento de salida de la llamada RPC */
  attrstat resWrite;
  int i, wr, ret;
  
       
#ifdef DEBUG_NFS
  printf("Ready to do WRITE (%d,%d)\n", (uint)size, (uint)offset);
#endif 
  
  /* Si se quieren escribir 0 bytes o menos, directamente se sale de la función */
  if(size <= 0){
#ifdef DEBUG_NFS
    printf("successfully WRITE (%d, %d) STATUS (%d)\n", (uint)size, (uint)offset, (uint)size);
#endif 
    
    return 0;
  }
  /* MAXDATA en el tamaño maximo que se puede escribir en cada llamada RCP */
  
  /* este contador indica cuantos bytes se han escrito */
  /* inicialmente es igual 0 */
  i=0;
  /* mientras el tamaño de lo escrito sea menor al tamaño pedido */
  while((unsigned int)i < size){
    if((size - i) >= MAXDATA) 
      wr = MAXDATA; 
    else{ 
      /* si es menor, el tamaño a escribir en la llamada RPC es size - i*/
      wr = size - i; 
    }
    /* se rellenan los datos de la petición*/
    memcpy(argsWrite.file, fh, FHSIZE);
    argsWrite.offset = offset + i;
    argsWrite.data.nfsdata_val = (char *)data + i;
    argsWrite.data.nfsdata_len = wr;
  /****************************************************************
   * los argumentos de la llamada WRITE son :			  *
   *	ENTRADA:						  *
   *	- una estructura con el manejador, offset, y tamaño que   *
   *      se desea escribir.                                      *
   *    SALIDA:                                                   *
   *    - una estructura con los nuevo atributos del fichero.     *
   ****************************************************************/      
    ret=nfsproc_write_2(&argsWrite, &resWrite, cl);
   
    if(ret == NFS_OK){
      if(resWrite.status != NFS_OK){
#ifdef DEBUG_NFS_ERR
      printf("Error in WRITE (%d,%d) STATUS (%d)\n", (uint)size, (uint)offset, -(int)resWrite.status);
#endif 	
        return -(ssize_t)resWrite.status;

      }
    }
    else{

#ifdef DEBUG_NFS_ERR
      printf("Error in WRITE (%d,%d) CONNECT_STATUS (%d)\n", (uint)size, (uint)offset, ret);
#endif      
      return NFSERR_CONNECT;
    }
    /* incremento el tamaño de lo leido */
    i = i + wr;
   
  }
#ifdef DEBUG_NFS
  printf("successfully WRITE (%d, %d) STATUS (%d)\n", (uint)size, (uint)offset, i);
#endif 
  
  return (ssize_t)i;
}




/****************************************************************
 * Funcion: nfs_create				                *
 * Funcion declarada en la interfaz nfs_nfs.h		        *
 *								*
 * Esta función crea un fichero.                                *
 *								*
 * Entradas:							*
 *      - el nombre del fichero que se desea crear.             *
 *	- el manejador del directorio donde se va a crear el    *
 *        fichero.		                                *
 *      - los permisos de acceso del fichero a crear.           *
 *      - un manejador de salida, que en caso de exito, sera el *
 *        manejador del fichero creado.                         *
 *      - un puntero a una estructura con los atributos del     *
 *        fichero, que en caso de exito se rellenara.           *
 *        Si no se quiere utilizar pongase a NULL.              *
 *	- puntero a la estructura CLIENT (es decir,		*
 *	  la conexion NFS).					*
 *								*
 * Salida: Un entero que indica error en caso de ser negativo,	*
 *	   y exito en caso de ser igual a 0.                    *
 ****************************************************************/
int nfs_create(fhandle fhin, char *file, mode_t mode, fhandle fhout, fattr *at, CLIENT *cl)
{
  /* argumento de entrada a la llamada RPC */
  createargs args;  
  /* argumento de salida de la llamada RPC*/
  diropres res;
  int ret;

#ifdef DEBUG_NFS
  printf("Ready to do CREATE (%s,%d)\n", file, mode);
  printf("fhin: ");printfh(fhin);
  printf("cl: %p \n",cl);
#endif
  /* se rellena la estructura de entrada */
  memcpy(args.where.dir, fhin, FHSIZE);
  args.where.name = file;
  /* se establecen ciertos atributos que tendra el fichero como los permisos */
  setAttr(&args.attributes, mode);
  /****************************************************************
   * los argumentos de la llamada CREATE son :			  *
   *	ENTRADA:						  *
   *	- una estructura con el manejador del directorio donde    *
   *      se va a crear el fichero, el nombre del fichero y los   *
   *      atributos del fichero                                   *
   *    SALIDA:                                                   *
   *    - una estructura con los nuevos atributos del fichero.    *
   ****************************************************************/
  if((ret = nfsproc_create_2(&args,&res,cl)) != NFS_OK){
#ifdef DEBUG_NFS
    printf("Error in CREATE (%s,%d) CONNECT_STATUS (%d)\n", file, mode, ret);
#endif 	    
    return NFSERR_CONNECT;    
  }
  if(res.status != NFS_OK){
#ifdef DEBUG_NFS
    printf("Error in CREATE (%s,%d) STATUS (%d)\n", file, mode, -(int)res.status);
#endif 	
    return -(int)res.status;
  }
  if(fhout != NULL)
  /* se copia el manejador */
    memcpy (fhout, res.diropres_u.fhand_attr.file, FHSIZE);
  /* si se desean los atributos del fichero, se copian los datos recogidos */
  if(at != NULL)
    memcpy(at, &res.diropres_u.fhand_attr.attributes, sizeof(fattr));
#ifdef DEBUG_NFS
  printf("successfully CREATE (%s,%d) STATUS (%d)\n", file, mode, -(int)res.status);
#endif 
  return NFS_OK;
  
}


/****************************************************************
 * Funcion: nfs_remove				                *
 * Funcion declarada en la interfaz nfs_nfs.h		        *
 *								*
 * Esta función borra un fichero.                               *
 *								*
 * Entradas:							*
 *	- el manejador del directorio donde se encuentra el     *
 *        fichero.		                                *
 *      - el nombre del fichero a borrar.                       *
 *	- puntero a la estructura CLIENT (es decir,		*
 *	  la conexion NFS).					*
 *								*
 * Salida: Un entero que indica error en caso de ser negativo,	*
 *	   y exito en caso de ser igual a 0.                    *
 ****************************************************************/
int nfs_remove(fhandle fh, char *file, CLIENT *cl)
{
  /* argumento de entrada a la llamada RPC */
  diropargs args;  
  /* argumento de salida de la llamada RPC */
  nfs_stat res;
  int ret;
#ifdef DEBUG_NFS
  printf("Ready to do REMOVE (%s)\n", file);
#endif
  
  /* se rellena la estructura de datos pasada como entrada a la RPC */
  memcpy(args.dir, fh, FHSIZE);
  args.name = file;
  /****************************************************************
   * los argumentos de la llamada REMOVE son :			  *
   *	ENTRADA:						  *
   *	- una estructura con el manejador del directorio donde    *
   *      se va a borrar el fichero y el nombre del fichero.      *
   *    SALIDA:                                                   *
   *    - un status del estado de la operación.                   *
   ****************************************************************/ 
  if((ret=nfsproc_remove_2(&args,&res,cl))!=NFS_OK){
#ifdef DEBUG_NFS
    printf("Error in REMOVE (%s) CONNECT_STATUS (%d)\n", file, ret);
#endif     
    return NFSERR_CONNECT;
  }
  if(res != NFS_OK){
#ifdef DEBUG_NFS
    printf("Error in REMOVE (%s) STATUS (%d)\n", file, -(int)res);
#endif       
    return -(int)(res);
  }
  else{
#ifdef DEBUG_NFS
    printf("Successfully REMOVE (%s) STATUS (%d)\n", file, -(int)res);
#endif  
    return NFS_OK;
  }
}


/****************************************************************
 * Funcion: nfs_rename				                *
 * Funcion declarada en la interfaz nfs_nfs.h		        *
 *								*
 * Esta función renombra un fichero o directorio.               *
 *								*
 * Entradas:							*
 *	- el manejador del directorio donde se encuentra el     *
 *        fichero o directorio.	                                *
 *      - el nombre del fichero o directorio a renombrar.       *
 *	- el manejador del directorio donde se  va a encontrar  *
 *        el fichero o directorio renombrado.                   *
 *      - el nuevo nombre del fichero o directorio.             *
 *	- puntero a la estructura CLIENT (es decir,		*
 *	  la conexion NFS).					*
 *								*
 * Salida: Un entero que indica error en caso de ser negativo,	*
 *	   y exito en caso de ser igual a 0.                    *
 ****************************************************************/
int nfs_rename(fhandle fh, char *name, fhandle fhR, char *nameR, CLIENT *cl)
{
  /* argumento de entrada a la llamada RPC */
  renameargs args;
  /* argumento de salida de la llamada RPC */
  nfs_stat res;
  int ret;

#ifdef DEBUG_NFS
  printf("Ready to do RENAME (%s,%s)\n", name, nameR);
#endif
  /* se rellena la estructura de datos pasada como entrada a la RPC */
  /* se rellenan los campos del fichero original */
  memcpy(args.from.dir, fh, FHSIZE);  
  args.from.name = name;
  /* se rellenan los campos del fichero renombrado */
  memcpy(args.to.dir, fhR, FHSIZE);
  args.to.name = nameR;
  
  /****************************************************************
   * los argumentos de la llamada RENAME son :			  *
   *	ENTRADA:						  *
   *	- una estructura con el manejador del directorio donde    * 
   *      se encuentra el fichero o directorio, el nombre del     *
   *      fichero o directorio, el manejador del directorio donde *
   *      se quiere el fichero o directorio y el nuevo nombre del * 
   *      fichero o directorio.                                   *
   *    SALIDA:                                                   *
   *    - un status de la operación.                              *
   ****************************************************************/ 
  if((ret=nfsproc_rename_2(&args,&res,cl))!=NFS_OK){    
#ifdef DEBUG_NFS
    printf("Error in RENAME (%s,%s) CONNECT_STATUS (%d)\n", name, nameR, ret);
#endif    
    return NFSERR_CONNECT;
  } 
  if(res != NFS_OK){
#ifdef DEBUG_NFS
    printf("Error in RENAME (%s,%s) STATUS (%d)\n", name, nameR, ret);
#endif
    return -(int)(res);
  }
  else{
#ifdef DEBUG_NFS
  printf("successfully RENAME (%s,%s) STATUS (%d)\n", name, nameR, -(int)res);
#endif
    return NFS_OK;
  }
 
}




/****************************************************************
 * Funcion: nfs_mkdir				                *
 * Funcion declarada en la interfaz nfs_nfs.h		        *
 *								*
 * Esta función crea un directorio.                             *
 *								*
 * Entradas:							*
 *	- el manejador del directorio donde se va a crear el    *
 *        directorio.		                                *
 *      - el nombre del directorio que se desea crear.          *
 *      - los permisos de acceso del directorio a crear.        *
 *      - un manejador de salida, que en caso de exito, sera el *
 *        manejador del directorio creado.                      *
 *      - un puntero a una estructura con los atributos del     *
 *        directorio, que en caso de exito se rellenara.        *
 *        Si no se quiere utilizar pongase a NULL.              *
 *	- puntero a la estructura CLIENT (es decir,		*
 *	  la conexion NFS).					*
 *								*
 * Salida: Un entero que indica error en caso de ser negativo,	*
 *	   y exito en caso de ser igual a 0.                    *
 ****************************************************************/
int nfs_mkdir(fhandle fhin, char *dir, mode_t mode, fhandle fhout, fattr *at, CLIENT *cl)
{
  /* argumento de entrada en la RPC */
  createargs args;
  /* argumento de salida de la RPC */
  diropres res;
  int ret;

#ifdef DEBUG_NFS
  printf("Ready to do MKDIR (%s)\n", dir);
#endif

  /*se rellenan los datos de entrada */
  memcpy(args.where.dir, fhin, FHSIZE);
  args.where.name = dir;
  setAttr(&(args.attributes), mode);
  
  /****************************************************************
   * los argumentos de la llamada MKDIR son :			  *
   *	ENTRADA:						  *
   *	- una estructura con el manejador del directorio donde    *
   *      se va a crear el directorio, el nombre del fichero y    *
   *      los atributos del directorio                            *
   *    SALIDA:                                                   *
   *    - una estructura con los nuevos atributos del directorio. *
   ****************************************************************/  
  if((ret = nfsproc_mkdir_2(&args, &res, cl))==NFS_OK){
    if (res.status == NFS_OK){
#ifdef DEBUG_NFS
      printf("successfully MKDIR (%s) STATUS (%d)\n", dir, -(int)res.status);
#endif 
      if(fhout != NULL)
	/* se copia el manejador */
	memcpy (fhout, res.diropres_u.fhand_attr.file, FHSIZE);
      /* si se desean los atributos del fichero, se copian los datos recogidos */
      if(at != NULL)
	memcpy(at, &(res.diropres_u.fhand_attr.attributes), sizeof(fattr));
      return NFS_OK;	
    }
    else{
#ifdef DEBUG_NFS
    printf("Error in MKDIR (%s) STATUS (%d)\n", dir, -(int)res.status);
#endif  
      return -(int) res.status;          
    }
  }
  else{
#ifdef DEBUG_NFS
    printf("Error in MKDIR (%s) CONNECT_STATUS (%d)\n", dir, ret);
#endif   
    return NFSERR_CONNECT;    
  }  
}


/****************************************************************
 * Funcion: nfs_rmdir				                *
 * Funcion declarada en la interfaz nfs_nfs.h		        *
 *								*
 * Esta función borra un directorio.                            *
 *								*
 * Entradas:							*
 *	- el manejador del directorio donde se encuentra el     *
 *        directorio.		                                *
 *      - el nombre del directorio a borrar.                    *
 *	- puntero a la estructura CLIENT (es decir,		*
 *	  la conexion NFS).					*
 *								*
 * Salida: Un entero que indica error en caso de ser negativo,	*
 *	   y exito en caso de ser igual a 0.                    *
 ****************************************************************/
int nfs_rmdir(fhandle fh, char *dir, CLIENT *cl){
  /* argumento de entrada en la RPC */
  diropargs args;
  /* argumento de salida en la RPC */
  nfs_stat res;
  int ret;

#ifdef DEBUG_NFS
  printf("Ready to do RMDIR (%s)\n", dir);
#endif
  /* se rellena la peticion */
  memcpy(args.dir, fh, FHSIZE);
  args.name = dir;
  
  /****************************************************************
   * los argumentos de la llamada RMDIR son :			  *
   *	ENTRADA:						  *
   *	- una estructura con el manejador del directorio donde    *
   *      se va a borrar el directorio y el nombre del directorio *
   *    SALIDA:                                                   *
   *    - un status del estado de la operación.                   *
   ****************************************************************/
  if ((ret = nfsproc_rmdir_2(&args, &res, cl))==NFS_OK)
    if (res == NFS_OK){
#ifdef DEBUG_NFS
      printf("successfully MKDIR (%s) STATUS (%d)\n", dir, -(int)res);
#endif 
      return NFS_OK;
    }
    else{
#ifdef DEBUG_NFS
      printf("Error in RMDIR (%s) STATUS (%d)\n", dir, -(int)res);
#endif
      return -(int)res;
    }
  else
#ifdef DEBUG_NFS
    printf("Error in RMDIR (%s) CONNECT_STATUS (%d)\n", dir, ret);
#endif
    return NFSERR_CONNECT;    
}




/****************************************************************
 * Funcion: nfs_readdir				        *
 * Funcion declarada en la interfaz nfs_nfs.h		        *
 *								*
 * Esta función lee las entradas de un directorio.              *
 *								*
 * Entradas:							*
 *  - el manejador del directorio del cual se quiere            *
 *    leer las entradas.		                        *
 *  - Una cookie, la cual se rellena con la ultima entrada      *
 *    leida. Sirve para saber por donde se quiere empezar a     *
 *    leer las entradas. Si se quiere ller desde la primera     *
 *    entrada, la cookie tiene que tener un valor de 0.         *
 *  - el numero de entradas que se quieren leer. Este tamaño    *
 *    no puede ser muy grande ya que UDP no permite mensajes    *
 *    que mas de 8 KB en el caso de nfs2 y 64KB en el caso de   *
 *    nfs3.                                                     *
 *  - un puntero a un array de cadenas, que es donde en caso de *
 *    exito se guardaran las entradas                           *
 *  - puntero a la estructura CLIENT (es decir,		        *
 *    la conexion NFS).					        *
 *								*
 * Salida: Un entero que indica error en caso de ser negativo,	*
 *	   y exito en caso de ser igual o mayor a 0. En caso de *
 *	   ser mayor o igual a 0, este número indica el numero  *
 *	   de entradas leidas                                   *
 ****************************************************************/
int nfs_readdir(fhandle fh, nfscookie cookie, char *entr, CLIENT *cl){
  /* argumento de entrada de la llamada RPC */
  readdirargs args;
  /* argumento de salida de la llamada RPC */
  readdirres res;  
  int ret;

#ifdef DEBUG_NFS
  printf("Ready to do READDIR()\n");
#endif

  memcpy(args.dir,fh,FHSIZE);
  memcpy(args.cookie,cookie,COOKIESIZE);
  args.count = sizeof(struct readdirres) + sizeof(readdirok) + sizeof(entry);
  res.readdirres_u.entradasDir.entries = (entry *)malloc(sizeof(entry));
  if (res.readdirres_u.entradasDir.entries==NULL){
  	 #ifdef DEBUG_NFS
      printf("Error in READDIR() STATUS (%d)\n",  NFSERR_NULL);
	#endif

    return NFSERR_NULL;
  }
  
  res.readdirres_u.entradasDir.entries->name 
	  = (char *)malloc(NFSMAXPATHLEN * sizeof(char));
  
  if (res.readdirres_u.entradasDir.entries->name==NULL){
    free(res.readdirres_u.entradasDir.entries);
 	 #ifdef DEBUG_NFS
      printf("Error in READDIR() STATUS (%d)\n",  NFSERR_NULL);
	#endif

    return NFSERR_NULL;
  }

  if ((ret = nfsproc_readdir_2(&args,&res,cl))==NFS_OK){

 
    if (res.status==NFS_OK){      
          
   	 if (res.readdirres_u.entradasDir.entries!=NULL){
		 
	  strcpy(entr,res.readdirres_u.entradasDir.entries->name);
	  memcpy(cookie,res.readdirres_u.entradasDir.entries->cookie,COOKIESIZE);
	  ret=NFS_OK;

	}

	if (res.readdirres_u.entradasDir.eof==TRUE){
        //ha llegado al final se pone al principio la cookie
	
		bzero(cookie,COOKIESIZE);
		#ifdef DEBUG_NFS
		  printf("successfully READDIR (%s) STATUS (EOF)\n",entr );
	    #endif 
		ret=NFSERR_EOFDIR;

    	}

    
    
	#ifdef DEBUG_NFS
	else{
		  printf("successfully READDIR (%s) STATUS (%d)\n",entr, NFS_OK);
	}
	#endif
    if(res.readdirres_u.entradasDir.entries != NULL){
	    
	    free(res.readdirres_u.entradasDir.entries->name);
	    free(res.readdirres_u.entradasDir.entries);
    }
    else {
	    ret = NFSERR_EOFDIR;
    }
    return ret;
   
 }
 else{

      ret = -(int)res.status;
      free(res.readdirres_u.entradasDir.entries->name);
      free(res.readdirres_u.entradasDir.entries);
  	 #ifdef DEBUG_NFS
      printf("Error in READDIR() STATUS (%d)\n",  ret);
	#endif

     return ret;
  }
}
else {
    free(res.readdirres_u.entradasDir.entries->name);
    free(res.readdirres_u.entradasDir.entries);    
	#ifdef DEBUG_NFS
      printf("Error in READDIR() STATUS (%d)\n",  ret);
	#endif
    return NFSERR_CONNECT;    
  }
}
  


/****************************************************************
 * Funcion: nfs_statfs				                *
 * Funcion declarada en la interfaz nfs_nfs.h		        *
 *								*
 * Esta función obtiene caracteristicas del servidor NFS.       *
 *								*
 * Entradas:							*
 *	- el manejador del directorio.                          *
 *      - un puntero a una estructura de tipo info que contiene *
 *        la informacion del servidor , en caso de exito.       *
 *	- puntero a la estructura CLIENT (es decir,		*
 *	  la conexion NFS).					*
 *								*
 * Salida: Un entero que indica error en caso de ser negativo,	*
 *	   y exito en caso de ser igual a 0.                    *
 ****************************************************************/
int nfs_statfs(fhandle arg, struct nfs_info *inf, CLIENT *cl){
  /* argumento de salida */
  statfsres res;
  int ret;

#ifdef DEBUG_NFS
  printf("Ready to do STATFS ()\n");
#endif
  /****************************************************************
   * los argumentos de la llamada STATFS son :			  *
   *	ENTRADA:						  *
   *	- un manejador del directorio de donde obtener las        *
   *      caracteristicas.                                        *
   *    SALIDA:                                                   *
   *    - las caracteristicas del servidor.                       *
   ****************************************************************/
  if((ret = nfsproc_statfs_2(arg, &res, cl))!=NFS_OK){
#ifdef DEBUG_NFS
    printf("Error in STATFS () CONNECT_STATUS (%d)\n",ret);
#endif 
    return NFSERR_CONNECT;
  }else{
    
    if(res.status != NFS_OK){
#ifdef DEBUG_NFS
      printf("Error in STATFS () STATUS (%d)\n", -(int)res.status);
#endif      
      return -(int)res.status;
    }
    else{
#ifdef DEBUG_NFS
      printf("successfully STATFS () STATUS (%d)\n", NFS_OK);
#endif 
      memcpy(inf, &(res.statfsres_u.inf), sizeof(struct nfs_info));
      return NFS_OK;
    }
  }
}
