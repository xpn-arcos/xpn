
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

#include "nfi/nfi_nfs3/nfs3.h"
//#define DEBUG_NFS 1
//#define DEBUG_MNT 1




void printfh3(char *s, fhandle3 *fh){
  //fh->fhandle3_val = (char *)malloc(sizeof(char)*FHSIZE3);
  //fh->fhandle3_len = FHSIZE3;

        unsigned int i,j;

        printf("%s ",s);
        for(j=0;j<FHSIZE3-1;j++){
                i = fh->fhandle3_val[j]+256;
                printf("i%256: %u",i%256);
		//if (j==FHSIZE3/2){
        	//	printf("\n");
		//}
        }
        i = fh->fhandle3_val[FHSIZE3-1]+256;
        printf("i%256: %u", i%256);
        printf("\n");

  return;
}
/****************************************************************
 * Funcion: create_conection_mount				*
 * Funcion declarada en la interfaz nfs3_mount.h	       	*
 *								*
 * Esta función crea una conexión entre el cliente ya el	*
 * servidor MNT. El protocolo de conexión establecida puede	*
 * ser TCP o UDP, aunque por defecto se utiliza UDP.		*
 * Esta conexión solo puede ser utilizada por un proceso a	*
 * la vez.							*
 *							        *
 * Entrada: nombre del servidor NFS				* 
 * Salida: Un puntero a una estructura CLIENT (es la conexion	*
 * realizada). Si devuelve NULL es que a ocurrido un error en	*
 * el proceso.							*
 ****************************************************************/
CLIENT* create_connection_mount3(char *name, int type)
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
  printf("Ready to do creation connection with MNT3 service\n");
#endif     
  /*  
      Esta es la manera original de realizar la conexión
      cli=clnt_create(name, MOUNTPROG ,MOUNTVERS ,"udp");
  */
  
  /* obtengo la direccion del host del servidor NFS a partir del nombre */
  hp = gethostbyname(name);
  if(hp == NULL){
    fprintf(stderr,"create_connection_mount3: Fail gethostbyname %s\n.",name);
    return NULL;
  }
  /* se inicializan los parametros del socket */
  bzero((char *)&raddr, sizeof(raddr));
  raddr.sin_family = AF_INET;
  memcpy(&(raddr.sin_addr), hp->h_addr, hp->h_length);
  raddr.sin_port=htons(0);
  sock = RPC_ANYSOCK;

  if(type == NFS3_TCP){
    /* con esta funcion se realiza la creacion de la conexion tcp */
#ifdef DEBUG_MNT
    printf("Creation connection TCP client with MNT3 service\n");
#endif
    cli = clnttcp_create(&raddr, MOUNT3_PROGRAM, MOUNT3_VERSION, &sock, NFS3_SENDSZ, NFS3_RECVSZ);
  }else{
    if(type == NFS3_UDP){
      /* tiempo maximo de espera entre la petición de cliente */
      /* y la respuesta del servidor segundos */
      wait.tv_sec=1;
      /* milisegundos */
      wait.tv_usec=0;
      /* con esta funcion se realiza la creacion de la conexion udp */
#ifdef DEBUG_MNT
      printf("Creation connection UDP client with MNT3 service\n");
#endif
      cli=clntudp_bufcreate(&raddr, MOUNT3_PROGRAM, MOUNT3_VERSION, wait, &sock, NFS3_SENDSZ, NFS3_RECVSZ);
    }
    else{
      cli=NULL;
    }
  }
  
  if(cli==NULL){
    fprintf(stderr,"create_connection_mount3: Can't create connection with MNT service with %s \n.",name);
#ifdef DEBUG_MNT
    printf("Error connection MNT3 service\n");
#endif
    return cli;
  }
  
  /* se elimina la autenticacion que  existe por defecto */
  auth_destroy(cli->cl_auth);
  /* se obtiene el nombre del host del cliente para añadir a los parametros de autenticacion */
  ret = gethostname(s, 200);
  if(ret < 0){
    fprintf(stderr,"create_connection_mount3: Fail gethostname %s\n.",s);
    close_connection_mount3(cli);
    return NULL;
  }
  /* con esto se añade cierta seguridad a la comunicación entre el cliente y el servidor */ 
  /* el tipo de seguridad utilizada es UNIX (vease la Rfc de las RPCs para mas información)*/
  cli->cl_auth=authunix_create(s,getuid(),getgid(),0,NULL); 
  //printf("LINUX--%p\n",cli->cl_auth);
  
  return cli;
}



/****************************************************************
 * Funcion: close_conection_mount				*
 * Funcion declarada en la interfaz nfs3_mount.h	        *
 *								*
 * Esta función elimina una conexión realizada a un		*
 * servidor MNT.						*
 *								*
 * Entrada: puntero a la estructura CLIENT.			*
 * Salida: no se devuelve ninguna salida.			*
 ****************************************************************/
void close_connection_mount3(CLIENT *cl)
{
  /* elimino la autenticacion */
  //printf("1 cl->cl_auth = %p\n",cl->cl_auth);
  //printf("1 cl = %p\n",cl);

  if (!cl)
  {
  	if (!(cl->cl_auth)) {
  	    auth_destroy(cl->cl_auth);
  	}

  	/* elimino la estructura */
  	clnt_destroy(cl);
  }

  /* la inicializo */
  // cl=NULL; // <- for that you need CLIENT **cl and *cl=NULL...

#ifdef DEBUG_MNT
  printf("Close connection MNT3\n");
#endif
} 

/****************************************************************
 * Funcion: nfs3_mount						*
 * Funcion declarada en la interfaz nfs3_mount.h		*
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
int nfs3_mount(char *dir, fhandle3 *fhand, CLIENT *cli )
{	
  mountres3 fh1;
  int res;
  //char *dirAux;

#ifdef DEBUG_MNT
  printf("Ready to do MOUNT3 (%s)\n",dir);
#endif

  fhand->fhandle3_val = (char *)malloc(sizeof(char)*FHSIZE3); 
  fhand->fhandle3_len = FHSIZE3; 
  //fh1 = (mountres3 *)malloc(sizeof(mountres3));
  //fh1->mountres3_u.mountinfo.fhandle.fhandle3_val = (char *)malloc(sizeof(char)*FHSIZE3);
  //fhand->fhandle3_val = fh.mountres3_u.mountinfo.fhandle.fhandle3_val = NULL;
  //fhand->fhandle3_len = fh1->mountres3_u.mountinfo.fhandle.fhandle3_len = FHSIZE3;
  //fhand->fhandle3_val = fh1->mountres3_u.mountinfo.fhandle.fhandle3_val;
  fh1.mountres3_u.mountinfo.fhandle.fhandle3_len = fhand->fhandle3_len;
  fh1.mountres3_u.mountinfo.fhandle.fhandle3_val = fhand->fhandle3_val;
  fh1.mountres3_u.mountinfo.auth_flavors.auth_flavors_len = 0;
  fh1.mountres3_u.mountinfo.auth_flavors.auth_flavors_val = NULL;

  
  /**************************************************************** 
   * los argumentos de la llamada MNT son :			  *
   *	ENTRADA:					          *
   *	- el path que se desea montar				  *
   *	SALIDA:							  *
   *	- una estructura con un status y el manejador en caso 	  *
   *	  de ser el status igual la 0			  *
   ****************************************************************/
  
  //printf("antes mountproc_null_3\n"); 
 // res = mountproc3_null_3(NULL,NULL, cli);
  //printf("desp. mountproc_null_3\n"); 
  //if(res != 0){
  //	clnt_perror(cli,"Error:"); 
  //}
  //printf("antes mountproc3_mnt_3\n"); 
  res = mountproc3_mnt_3(&dir,&fh1,cli);
  //printf("desp. mountproc3_mnt_3\n"); 
  /* comprobamos el estado de la conexión */
  if(res != 0){

    //free(fh1);
#ifdef DEBUG_MNT
    printf("Error in MOUNT3 (%s) STATUS_CONNECTION (%d)\n",dir,res);
#endif
    /* en caso de error de conexion devolvemos un error generico
       de error de conexión */
    return -1;
  }
  else{
    if(fh1.fhs_status != 0){
      /* en caso de que fh.status sea distinto a 0, *
       * devolvemos el error pero con valor negativo     */
#ifdef DEBUG_MNT
      printf("Error in MOUNT3 (%s) STATUS (%d)\n",dir,res);
#endif
      //free(fh1);
      fprintf(stderr,"nfs3_mount3: Error in MOUNT (%s) STATUS (%d)\n",dir,res);
      return -(int)fh1.fhs_status;
    }
  }
  /* en caso de exito */
  /* copiamos el manejador obtenido al manejador pasado por parametro */
  //fhand->fhandle3_len = fh1->mountres3_u.mountinfo.fhandle.fhandle3_len;
  //memcpy(fhand->fhandle3_val, fh.mountres3_u.mountinfo.fhandle.fhandle3_val, fhand->fhandle3_len);
  //fhand->fhandle3_val = fh1->mountres3_u.mountinfo.fhandle.fhandle3_val;

  //free(fh1);
  //printf("despues %p\n",fh.mountres3_u.mountinfo.fhandle.fhandle3_val);
#ifdef DEBUG_MNT
  printf("Successfully MOUNT3 (%s) STATUS (%d)\n",dir,res);
#endif
  //printf("*** cli->cl_auth = %p\n",cli->cl_auth);
 
  return 0;
}

/****************************************************************
 * Funcion: nfs3_umount						*
 * Funcion declarada en la interfaz nfs3_mount.h		*
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
int nfs3_umount(char *path, CLIENT *cli)
{
  int res;
  
#ifdef DEBUG_MNT
  printf("Ready to do UMOUNT3 (%s)\n",path);
#endif
  /**************************************************************** 
   * los argumentos de la llamada MNT son :			  *
   *	ENTRADA:						  *
   *	- el path que se desea desmontar			  *
   ****************************************************************/
  res = mountproc3_umnt_3(&path, NULL, cli);
  
  if(res != 0){
    #ifdef DEBUG_MNT
      printf("Error in UMOUNT3 (%s) STATUS_CONNECTION (%d)\n", path, res);	
    #endif
      fprintf(stderr,"nfs3_umount: Error in UMOUNT3 (%s) STATUS_CONNECTION (%d)\n", path, res);	
    /* en caso de que ocurra un error en la conexion se 
       devuelve este codigo de error */
    return -1;
  }else
#ifdef DEBUG_MNT
    printf("Successfully UMOUNT3 (%s) STATUS (%d)\n",path,res);
#endif
  return 0;
}
 
/****************************************************************
 * Funcion: nfs3_export						*
 * Funcion declarada en la interfaz nfs3_mount.h		*
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
int nfs3_export(__attribute__((__unused__)) exports3 *exp, __attribute__((__unused__)) CLIENT *cli)
{  
    return 0;
}


/****************************************************************
 * Funcion: create_conection_nfs				*
 * Funcion declarada en la interfaz nfs3_nfs.h		        *	
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
CLIENT* create_connection_nfs3(char *name, int type)
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
  printf("Ready to do creation connection with NFS3 service\n");
#endif
  /*   
       Esta es la manera original de realizar la conexión
       cli=clnt_create(name, NFS_PROGRAM ,NFS_VERSION ,"udp"); 
  */
  
  
  /* obtengo la direccion del host del servidor NFS a partir del nombre */
  hp = gethostbyname(name);
  if(hp == NULL){
    fprintf(stderr,"create_connection_nfs3: Fail gethostbyname %s\n.",name);
    return NULL;
  }
  /* se inicializan los parametros del socket */
  //bzero((char *)&raddr, sizeof(raddr));
  raddr.sin_family = AF_INET;
  memcpy(&(raddr.sin_addr), hp->h_addr, hp->h_length);
  raddr.sin_port=htons(0);
  sock = RPC_ANYSOCK;
  
  
  if(type == NFS3_TCP){
    /* con esta funcion se realiza la creacion de la conexion tcp */
#ifdef DEBUG_NFS
    printf("Creation connection TCP client with NFS service\n");
#endif
    cli = clnttcp_create(&raddr, NFS3_PROGRAM, NFS3_VERSION, &sock, NFS3_SENDSZ, NFS3_RECVSZ );
  }else{
    if(type == NFS3_UDP){
      /* tiempo maximo de espera entre la petición de cliente */
      /* y la respuesta del servidor segundos */
      wait.tv_sec=1;
      /* milisegundos */
      wait.tv_usec=0;
      /* con esta funcion se realiza la creacion de la conexion udp */
#ifdef DEBUG_NFS
      printf("Creation connection UDP client with NFS3 service\n");
#endif
      cli=clntudp_bufcreate(&raddr, NFS3_PROGRAM, NFS3_VERSION, wait, &sock, NFS3_SENDSZ, NFS3_RECVSZ);
    }
    else{
      cli=NULL;
    }
  }
  
  if(cli==NULL){
#ifdef DEBUG_NFS
    printf("Error connection NFS3 service\n");
#endif
    fprintf(stderr,"create_connection_nfs3: Can't create connection with NFS service with %s \n.",name);
    return cli;
  }
  
  /* se elimina la autenticacion que  existe por defecto */
  auth_destroy(cli->cl_auth);
  /* se obtiene el nombre del host del cliente para añadir a los parametros de autenticacion */
  ret = gethostname(s, 200);
  if(ret < 0){
     fprintf(stderr,"create_connection_nfs3: Fail gethostname %s\n.",name);
     close_connection_nfs3(cli);
     return NULL;
  }
  /* con esto se añade cierta seguridad a la comunicación entre el cliente y el servidor */ 
  /* el tipo de seguridad utilizada es UNIX (vease la Rfc de las RPCs para mas información)*/
  cli->cl_auth=authunix_create(s,getuid(),getgid(),0,NULL); 
  return cli;
}


/****************************************************************
 * Funcion: close_conection_nfs					*
 * Funcion declarada en la interfaz nfs3_nfs.h			*
 *								*
 * Esta función elimina una conexión realizada a un	        *
 * servidor NFS.						*
 *								*
 * Entrada: puntero a la estructura CLIENT.			*
 * Salida: no se devuelve ninguna salida.			*
 ****************************************************************/
void close_connection_nfs3(CLIENT *cl)
{
  /* elimino la autenticacion */
  if(!cl){

  	if(!cl->cl_auth){
  		auth_destroy(cl->cl_auth);
	}

  	/* elimino la estructura */
  	clnt_destroy(cl);
  }

  /* la inicializo */
  // cl=NULL; <- for that, CLIENT **cl and *cl=NULL

#ifdef DEBUG_NFS
  printf("Close connection NFS\n");
#endif
} 


/****************************************************************
 * Esta funcion inicializa la estructura timevalNfs.            *
 * Entradas:                                                    *
 *      - un puntero a la estructura a rellenar.                *
 ****************************************************************/
/*
void nfs3_setDate(nfstime3 *t){
}
*/

/****************************************************************
 * Esta funcion inicializa la estructura sattr.                 *
 * Entradas:                                                    *
 *      - un puntero a la estructura a rellenar.                *
 *      - los permisos que el fichero va a tener                *
 ****************************************************************/
/*
void nfs3_setAttr( sattr3 *at,unsigned int mode){
}
*/


/****************************************************************
 * Funcion: nfs3_setattr					*
 * Funcion declarada en la interfaz nfs3_nfs.h		        *
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
int nfs3_setattr(fhandle3 *fh, fattr3 *fatt, CLIENT *cl){
    wccstat3 res;
    int ret;
    setattr3args sat;
    
#ifdef DEBUG_NFS
    printf("Ready to do SETATTR ()\n");
#endif
  
  memcpy(&sat.object, fh, NFS3_FHSIZE);
  sat.new_attributes.mode.set_uint32_u.val  = fatt->mode;
  sat.new_attributes.uid.set_uint32_u.val   = fatt->uid;
  sat.new_attributes.gid.set_uint32_u.val   = fatt->gid;
  sat.new_attributes.size.set_uint64_u.val  = fatt->size;
  sat.new_attributes.atime.set_time_u.time  = fatt->atime;
  sat.new_attributes.mtime.set_time_u.time  = fatt->mtime;
  
  if((ret = nfsproc3_setattr_3(&sat, &res, cl)) != NFS3_OK){
#ifdef DEBUG_NFS
      printf("Error in SETATTR () STATUS_CONNECTION (%d)\n",ret);
#endif
      return -1;
    }
    
    if(res.status != NFS3_OK){
#ifdef DEBUG_NFS
       printf("Error in SETATTR () STATUS (%d)\n",-(int)res.status);
#endif
       return -(int)res.status;
    }
    
#ifdef DEBUG_NFS
    printf("successfully SETATTR () STATUS (%d)\n",-(int)res.status);
#endif
  return NFS3_OK;
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
int nfs3_getattr(fhandle3 *fh, fattr3 *fatt, CLIENT *cl)
{
  getattr3res res;
  int ret;

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
  if((ret = nfsproc3_getattr_3(fh, &res, cl)) != NFS3_OK){
#ifdef DEBUG_NFS
    printf("Error in GETATTR () STATUS_CONNECTION (%d)\n",ret);
#endif
    return -1;
  }
  if(res.status != NFS3_OK){
#ifdef DEBUG_NFS
    printf("Error in GETATTR () STATUS (%d)\n",-(int)res.status);
#endif
    return -(int)res.status;
  }  
  memcpy(fatt, &res.getattr3res_u.attributes, sizeof(fattr3));
#ifdef DEBUG_NFS
  printf("successfully GETATTR () STATUS (%d)\n",-(int)res.status);
#endif   
  return NFS3_OK;
}

/****************************************************************
 * Funcion: nfs3_lookup					        *
 * Funcion declarada en la interfaz nfs3_nfs.h		        *
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
int nfs3_lookup(fhandle3 *fhin, char *path , fhandle3 *fhout, fattr3 *att, CLIENT *cl)
{

	diropargs3 arg;
	lookup3res res;
	fhandle3 fh;
        char path_aux[MNTNAMLEN], dir_aux[MNTNAMLEN];
	int ret;
	int end;

#ifdef DEBUG_NFS
	printf("Ready to do LOOKUP (%s)\n",path);
#endif
	      
  if((strcmp(path,"")==0)||(strcmp(path,"/")==0)){
  	  fhout->fhandle3_len = fhin->fhandle3_len;
  	  fhout->fhandle3_val = fhin->fhandle3_val;
  
	  return 0;
  }
  
  strcpy(path_aux, path);
    
  fh.fhandle3_len = fhin->fhandle3_len;
  fh.fhandle3_val = fhin->fhandle3_val;
  res.lookup3res_u.resok.object.fhandle3_val = (char *) malloc(FHSIZE3*sizeof(char));
  
  ret = -1;
  
  do{
	  /* se inicializan los argumentos de entrada */
	  arg.dir.fhandle3_len = fh.fhandle3_len;
	  arg.dir.fhandle3_val = fh.fhandle3_val;
	  /* separar ultimo dir del path */
			                
	  end = getFirstDir(dir_aux, path_aux);
	  arg.name = dir_aux;
					
	  //printf("dir_aux = %s strlen = %d\n",arg.name,strlen(arg.name));
      if(strlen(arg.name) == 0){						 
	    continue;
	  }						                                         
       /****************************************************************
	   * los argumentos de la llamada LOOKUP son :                    *
	   * ENTRADA:                                                     *
	   *   - una estructura con el manejador y un path                *
	   * SALIDA:                                                      *
	   *   - una estructura con el manejador y los atributos          *
	   *                                                              *     
	   ****************************************************************/
	   ret=nfsproc3_lookup_3(&arg, &res, cl); 
	   //printf("ret = %d end = %d res.status = %d\n",ret, end,res.status);
	   if(end == 0){		   
		   fhout->fhandle3_len = res.lookup3res_u.resok.object.fhandle3_len;
		   fhout->fhandle3_val = res.lookup3res_u.resok.object.fhandle3_val;
	  }else{
		if(strcmp(path_aux,"/") == 0){
		   fhout->fhandle3_len = res.lookup3res_u.resok.object.fhandle3_len;
		   fhout->fhandle3_val = res.lookup3res_u.resok.object.fhandle3_val;
		   end = 0;
		}else{
		   fh.fhandle3_len = res.lookup3res_u.resok.object.fhandle3_len;
		   fh.fhandle3_val = res.lookup3res_u.resok.object.fhandle3_val;
		}	
	  }
  }while(end != 0);

  if(ret == 0)
  	if(res.status == 0){     
		   fhout->fhandle3_len = res.lookup3res_u.resok.object.fhandle3_len;
		   fhout->fhandle3_val = res.lookup3res_u.resok.object.fhandle3_val;     
	  if(att != NULL){
		/* si el puntero no es NULL, se rellena la estructura */
#ifdef DEBUG_NFS3
      printf("Copy attributes in LOOKUP (%s)\n",path);
#endif 
		/* copiar atributos ..... TO DO*/
	    if(res.lookup3res_u.resok.obj_attributes.present){
	    	memcpy(att,&res.lookup3res_u.resok.obj_attributes.post_op_attr_u.attributes,sizeof(fattr3));
	    }
  	  }
#ifdef DEBUG_NFS3
      printf("Successfully LOOKUP (%s) STATUS (%d)\n",
	     path, res.lookup3res_u.resok.obj_attributes.post_op_attr_u.attributes.type);
#endif 
      return res.lookup3res_u.resok.obj_attributes.post_op_attr_u.attributes.type;           
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
    return -1;   
  }
  return 0;
}




/****************************************************************
 * Funcion: nfs3_read				                *
 * Funcion declarada en la interfaz nfs3_nfs.h		        *
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
ssize_t nfs3_read(fhandle3 *fh, void *data, off_t offset, size_t size, CLIENT *cl)
{
  /* argumento de entrada a la llamada RPC */
  read3args args;
  /* argumento de salida de la llamada RPC */
  read3res res; 
  size_t i, rd; 
  int ret;
  
#ifdef DEBUG_NFS
  printf("Ready to do READ3 (%d,%d)\n", (uint)size,(uint) offset);
#endif 
  
  /* Si se quieren leer 0 bytes o menos, directamente se sale de la función */
  if(size <= 0){
#ifdef DEBUG_NFS
    printf("successfully READ3 (%d, %d) STATUS (%d)\n", (uint)size, (uint)offset, (uint)size);
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
    if((size - i) >= MAXDATA3) 
      rd = MAXDATA3; 
    else{ 
      /* si es menor, el tamaño a leer en la llamada RPC es size - i*/
      rd = size - i; 
    }
    /* se rellenan los argumentos de entrada */
	args.file.fhandle3_len = fh->fhandle3_len;
	args.file.fhandle3_val = fh->fhandle3_val;     
    args.offset = offset + i;
    args.count = rd;
    res.read3res_u.resok.data.data_val = (char *)data + i;

  /****************************************************************
   * los argumentos de la llamada READ son :					  *
   *	ENTRADA:												  *
   *	- una estructura con el manejador, offset, y tamaño que   *
   *      se desea leer.                                          *
   *    SALIDA:                                                   *
   *    - una estructura con los datos leido y el tamaño leido.   *
   ****************************************************************/      
    ret = nfsproc3_read_3(&args, &res, cl);
	//nfsproc3_read_3(read3args *, read3res *, CLIENT *);
    if(ret != 0){

#ifdef DEBUG_NFS_ERR
      printf("Error in READ (%d,%d) CONNECT_STATUS (%d)\n", (uint)size, (uint)offset, ret);
#endif     
      return -1;
    }
    else{        
      if(res.status != 0){
#ifdef DEBUG_NFS_ERR
      printf("Error in READ (%d,%d) STATUS (%d)\n", (uint)size, (uint)offset, -(int)res.status);
#endif   
  	  return -(ssize_t)(res.status);	
      }
     else{ 
	/* incremento lo leido */
	i = res.read3res_u.resok.data.data_len + i; 	
	/* si lo leido < que lo que se pide -> EOF */
	if(res.read3res_u.resok.data.data_len < rd){
#ifdef DEBUG_NFS
	  printf("successfully READ (%d, %d) STATUS (%d)\n", (uint)size, (uint)offset, (uint)i);
#endif 
	  return (ssize_t)i;
	}
      }
    }
    
  }
#ifdef DEBUG_NFS
  printf("successfully READ (%d, %d) STATUS (%lu)\n", (uint)size, (uint)offset, i);
#endif  
  
  
  return (ssize_t)i;
	  
}



/****************************************************************
 * Funcion: nfs3_write				                *
 * Funcion declarada en la interfaz nfs3_nfs.h		        *
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
ssize_t nfs3_write(fhandle3 *fh, void *data, off_t offset, size_t size, CLIENT *cl)
{
  /* argumento de entrada a la llamada RPC */
  write3args args;
  /* argumento de salida de la llamada RPC */
  write3res res;
  int i, wr, ret;
  
       
#ifdef DEBUG_NFS
  printf("Ready to do WRITE (%d,%d)\n", (uint)size, (uint)offset);
  printfh3("Write: ",fh);
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
    if((size - i) >= MAXDATA3) 
      wr = MAXDATA3; 
    else{ 
      /* si es menor, el tamaño a escribir en la llamada RPC es size - i*/
      wr = size - i; 
    }

    /* se rellenan los datos de la petición*/
    args.file.fhandle3_len = fh->fhandle3_len;
    args.file.fhandle3_val = fh->fhandle3_val;
    args.offset = offset + i;
    args.stable = FILE_SYNC;
    args.stable = DATA_SYNC;
    args.stable = UNSTABLE;
    args.data.data_val = (char *)data + i;
    args.count = args.data.data_len = wr;
  /****************************************************************
   * los argumentos de la llamada WRITE son :			  *
   *	ENTRADA:						  *
   *	- una estructura con el manejador, offset, y tamaño que   *
   *      se desea escribir.                                      *
   *    SALIDA:                                                   *
   *    - una estructura con los nuevo atributos del fichero.     *
   ****************************************************************/      
    ret=nfsproc3_write_3(&args, &res, cl);
    //printf("ret = %d res.status = %d\n",ret, res.status);
    //if (ret !=0)
     //  clnt_perror(cl, "Write: ");
    if(ret == 0){
      if(res.status != 0){
#ifdef DEBUG_NFS_ERR
      printf("Error in WRITE (%d,%d) STATUS (%d)\n", (uint)size, (uint)offset, -(int)res.status);
#endif 	
        return -(ssize_t)res.status;

      }
    }
    else{

#ifdef DEBUG_NFS_ERR
      printf("Error in WRITE (%d,%d) CONNECT_STATUS (%d)\n", (uint)size, (uint)offset, ret);
#endif      
      return -1;
    }
    /* incremento el tamaño de lo leido */
    i = i + wr;
   
  }
#ifdef DEBUG_NFS
  printf("successfully WRITE (%d, %d) STATUS (%d)\n", (uint)size, (uint)offset, i);
#endif 
  
  //res.write3res_u.resok.count
  return (ssize_t)i;
}




/****************************************************************
 * Funcion: nfs3_create				                *
 * Funcion declarada en la interfaz nfs3_nfs.h		        *
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
int nfs3_create(fhandle3 *fhin, char *file, mode_t mode, fhandle3 *fhout, __attribute__((__unused__)) fattr3 *at, CLIENT *cl)
{
  /* argumento de entrada a la llamada RPC */
  create3args args;  
  /* argumento de salida de la llamada RPC*/
  diropres3 res;
  //fhandle3 fh;
  int ret;
  

  //printf("Ready to do CREATE (%s,%d)\n", file, mode);
#ifdef DEBUG_NFS
  printf("Ready to do CREATE (%s,%d)\n", file, mode);
  printfh3("Create begin: ", fhin);
#endif
  /* se rellena la estructura de entrada */
  
  //printf("fhIn = %d - %p\n", fhin->fhandle3_len, fhin->fhandle3_val);
  //args.where.dir.fhandle3_len = fh.fhandle3_len;
  //args.where.dir.fhandle3_val = fh.fhandle3_val;  
  //args.where.dir.fhandle3_len = fhin->fhandle3_len;
  args.where.dir.fhandle3_len = FHSIZE3;
  args.where.dir.fhandle3_val = fhin->fhandle3_val;  
  args.where.name = file;
 // args.where.name = (char *) malloc(strlen(file)+1);
 // strcpy(args.where.name ,file);
  args.how.mode = UNCHECKED;
  /* se establecen ciertos atributos que tendra el fichero como los permisos */
  bzero(&args.how.createhow3_u.obj_attributes, sizeof(sattr3)); 
  args.how.createhow3_u.obj_attributes.mode.set = 1;
  args.how.createhow3_u.obj_attributes.mode.set_uint32_u.val = mode;
  
  res.diropres3_u.resok.obj.post_op_fh3_u.handle.fhandle3_val = (char *) malloc(sizeof(char)*FHSIZE3);
  res.diropres3_u.resok.obj.post_op_fh3_u.handle.fhandle3_len = FHSIZE3;
  /****************************************************************
   * los argumentos de la llamada CREATE son :			  *
   *	ENTRADA:						  *
   *	- una estructura con el manejador del directorio donde    *
   *      se va a crear el fichero, el nombre del fichero y los   *
   *      atributos del fichero                                   *
   *    SALIDA:                                                   *
   *    - una estructura con los nuevos atributos del fichero.    *
   ****************************************************************/
 //printf("antes de llamar a nfsproc3_create_3\n");
  if((ret = nfsproc3_create_3(&args,&res,cl)) != 0){
    //printf("Error in CREATE (%s,%d) CONNECT_STATUS (%d)\n", file, mode, ret);
#ifdef DEBUG_NFS
    printf("Error in CREATE (%s,%d) CONNECT_STATUS (%d)\n", file, mode, ret);
#endif 	    
    return -1;    
    
  }
  if(res.status != 0){
    //printf("Error in CREATE (%s,%d) STATUS (%d)\n", file, mode, -(int)res.status);
#ifdef DEBUG_NFS
    printf("Error in CREATE (%s,%d) STATUS (%d)\n", file, mode, -(int)res.status);
#endif 	
    return -(int)res.status;
  }

  if (fhout != NULL)
  {
    /* se copia el manejador */
    fhout->fhandle3_len = res.diropres3_u.resok.obj.post_op_fh3_u.handle.fhandle3_len;
    fhout->fhandle3_val = res.diropres3_u.resok.obj.post_op_fh3_u.handle.fhandle3_val;
    /* si se desean los atributos del fichero, se copian los datos recogidos */
    //if(at != NULL){
    //   memcpy(at, &res.diropres_u.fhand_attr.attributes, sizeof(fattr));
    //   // TODO 
    //}
  }
#ifdef DEBUG_NFS
  printf("successfully CREATE (%s,%d) STATUS (%d)\n", file, mode, -(int)res.status);
  printfh3("Create end: ", fhout);
#endif 

  return 0;
}


/****************************************************************
 * Funcion: nfs3_remove				                *
 * Funcion declarada en la interfaz nfs3_nfs.h		        *
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
int nfs3_remove(fhandle3 *fh, char *file, CLIENT *cl)
{
  /* argumento de entrada a la llamada RPC */
  diropargs3 args;  
  /* argumento de salida de la llamada RPC */
  wccstat3 res;
  int ret;
  
#ifdef DEBUG_NFS
  printf("Ready to do REMOVE (%s)\n", file);
#endif
  
  /* se rellena la estructura de datos pasada como entrada a la RPC */
  args.dir.fhandle3_len = fh->fhandle3_len;
  args.dir.fhandle3_val = fh->fhandle3_val;
  args.name = file;
  /****************************************************************
   * los argumentos de la llamada REMOVE son :			  *
   *	ENTRADA:						  *
   *	- una estructura con el manejador del directorio donde    *
   *      se va a borrar el fichero y el nombre del fichero.      *
   *    SALIDA:                                                   *
   *    - un status del estado de la operación.                   *
   ****************************************************************/ 
  if((ret=nfsproc3_remove_3(&args,&res,cl))!=0){
#ifdef DEBUG_NFS
    printf("Error in REMOVE (%s) CONNECT_STATUS (%d)\n", file, ret);
#endif     
    return -1;
  }
  if(res.status != 0){
#ifdef DEBUG_NFS
    printf("Error in REMOVE (%s) STATUS (%d)\n", file, -(int)res.status);
#endif       
    return -(int)(res.status);
  }
  else{
#ifdef DEBUG_NFS
    printf("Successfully REMOVE (%s) STATUS (%d)\n", file, -(int)res.status);
#endif  
    return 0;
  }
}


/****************************************************************
 * Funcion: nfs3_rename				                *
 * Funcion declarada en la interfaz nfs3_nfs.h		        *
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
int nfs3_rename(__attribute__((__unused__)) fhandle3 *fh, __attribute__((__unused__)) char *name, __attribute__((__unused__)) fhandle3 *fhR, __attribute__((__unused__)) char *nameR, __attribute__((__unused__)) CLIENT *cl)
{
	return 0;
}




/****************************************************************
 * Funcion: nfs3_mkdir				                *
 * Funcion declarada en la interfaz nfs3_nfs.h		        *
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
int nfs3_mkdir(__attribute__((__unused__)) fhandle3 *fhin, __attribute__((__unused__)) char *dir, __attribute__((__unused__)) mode_t mode, __attribute__((__unused__)) fhandle3 *fhout, __attribute__((__unused__)) fattr3 *at, __attribute__((__unused__)) CLIENT *cl)
{
	return 0;
}


/****************************************************************
 * Funcion: nfs3_rmdir				                *
 * Funcion declarada en la interfaz nfs3_nfs.h		        *
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
int nfs3_rmdir(__attribute__((__unused__)) fhandle3 *fh, __attribute__((__unused__)) char *dir, __attribute__((__unused__)) CLIENT *cl){
	return 0;
}




/****************************************************************
 * Funcion: nfs3_readdir				        *
 * Funcion declarada en la interfaz nfs3_nfs.h		        *
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
int nfs3_readdir(__attribute__((__unused__)) fhandle3 *fh, __attribute__((__unused__)) cookieverf3 cookie, __attribute__((__unused__)) char *entr, __attribute__((__unused__)) CLIENT *cl){
	return 0;
}
  


/****************************************************************
 * Funcion: nfs3_statfs				                *
 * Funcion declarada en la interfaz nfs3_nfs.h		        *
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
int nfs3_statfs(__attribute__((__unused__)) fhandle3 *arg, __attribute__((__unused__)) fsinfo3resok *inf, __attribute__((__unused__)) CLIENT *cl){
	return 0;
}
