/*
 *  Copyright 2000-2023 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
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

/**
 * @file nfs3.c
 * @brief File to 'TODO'.
 *
 * File to 'TODO'.
 *
 * @authors Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
 * @date  Jul 22, 2021
 * @bug No known bugs.
 */

/************************************************
 *  ... Includes
 ***********************************************/
#include "nfi/nfi_nfs3/nfs3.h"
// #define DEBUG_NFS 1
// #define DEBUG_MNT 1

/************************************************
 *  ... Functions
 ***********************************************/

void printfh3(char *s, fhandle3 *fh)
{
  // fh->fhandle3_val = (char *)malloc(sizeof(char)*FHSIZE3);
  // fh->fhandle3_len = FHSIZE3;

  unsigned int i, j;

  printf("%s ", s);
  for (j = 0; j < FHSIZE3 - 1; j++)
  {
    i = fh->fhandle3_val[j] + 256;
    printf("i%256: %u", i % 256);
    // if (j==FHSIZE3/2){
    //	printf("\n");
    //}
  }
  i = fh->fhandle3_val[FHSIZE3 - 1] + 256;
  printf("i%256: %u", i % 256);
  printf("\n");

  return;
}

/**
 * @brief Create connection client-server.
 *
 * This function creates a connection between the client
 * and the MNT server. The established connection protocol
 * can be TCP or UDP, although UDP is used by default.
 * This connection can only be used by one process at a time.
 *
 * @param name NFS server name.
 * @param type  'TODO'.
 * @return A pointer to a CLIENT structure (it is the connection
 * made). If it returns NULL, an error has occurred in the process.
 */
CLIENT *create_connection_mount3(char *name, int type)
{
  /* puntero a la conexi�n*/
  CLIENT *cli;
  /* estructuras utilizas en el proceso de conexi�n */
  struct sockaddr_in raddr;
  struct hostent *hp;
  struct timeval wait;
  /* variables auxiliares utilizadas en el proceso*/
  int sock, ret;
  char s[MNTNAMLEN];

#ifdef DEBUG_MNT
  printf("Ready to do creation connection with MNT3 service\n");
#endif
  /*
      Esta es la manera original de realizar la conexi�n
      cli=clnt_create(name, MOUNTPROG ,MOUNTVERS ,"udp");
  */

  /* obtengo la direccion del host del servidor NFS a partir del nombre */
  hp = gethostbyname(name);
  if (hp == NULL)
  {
    fprintf(stderr, "create_connection_mount3: Fail gethostbyname %s\n.", name);
    return NULL;
  }
  /* se inicializan los parametros del socket */
  bzero((char *)&raddr, sizeof(raddr));
  raddr.sin_family = AF_INET;
  memcpy(&(raddr.sin_addr), hp->h_addr, hp->h_length);
  raddr.sin_port = htons(0);
  sock = RPC_ANYSOCK;

  if (type == NFS3_TCP)
  {
    /* con esta funcion se realiza la creacion de la conexion tcp */
#ifdef DEBUG_MNT
    printf("Creation connection TCP client with MNT3 service\n");
#endif
    cli = clnttcp_create(&raddr, MOUNT3_PROGRAM, MOUNT3_VERSION, &sock, NFS3_SENDSZ, NFS3_RECVSZ);
  }
  else
  {
    if (type == NFS3_UDP)
    {
      /* tiempo maximo de espera entre la petici�n de cliente */
      /* y la respuesta del servidor segundos */
      wait.tv_sec = 1;
      /* milisegundos */
      wait.tv_usec = 0;
      /* con esta funcion se realiza la creacion de la conexion udp */
#ifdef DEBUG_MNT
      printf("Creation connection UDP client with MNT3 service\n");
#endif
      cli = clntudp_bufcreate(&raddr, MOUNT3_PROGRAM, MOUNT3_VERSION, wait, &sock, NFS3_SENDSZ, NFS3_RECVSZ);
    }
    else
    {
      cli = NULL;
    }
  }

  if (cli == NULL)
  {
    fprintf(stderr, "create_connection_mount3: Can't create connection with MNT service with %s \n.", name);
#ifdef DEBUG_MNT
    printf("Error connection MNT3 service\n");
#endif
    return cli;
  }

  /* se elimina la autenticacion que  existe por defecto */
  auth_destroy(cli->cl_auth);
  /* se obtiene el nombre del host del cliente para a�adir a los parametros de autenticacion */
  ret = gethostname(s, 200);
  if (ret < 0)
  {
    fprintf(stderr, "create_connection_mount3: Fail gethostname %s\n.", s);
    close_connection_mount3(cli);
    return NULL;
  }
  /* con esto se a�ade cierta seguridad a la comunicaci�n entre el cliente y el servidor */
  /* el tipo de seguridad utilizada es UNIX (vease la Rfc de las RPCs para mas informaci�n)*/
#ifdef LINUX
  cli->cl_auth = authunix_create(s, getuid(), getgid(), 0, NULL);
  // printf("LINUX--%p\n",cli->cl_auth);

#else
  cli->cl_auth = authunix_create(s, 501, 501, 0, NULL);
#endif
  return cli;
}

/**
 * @brief Close connection client-server.
 *
 * This function deletes a connection made to an MNT server.
 *
 * @param cl Pointer to the CLIENT structure.
 * @par Returns
 *    Nothing.
 */
void close_connection_mount3(CLIENT *cl)
{
  /* elimino la autenticacion */
  // printf("1 cl->cl_auth = %p\n",cl->cl_auth);
  // printf("1 cl = %p\n",cl);

  if (!cl)
  {
    if (!(cl->cl_auth))
    {
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

/**
 * @brief Get initial handler.
 *
 * This function obtains the initial handler.
 * It is obtained by mounting the directory passed by parameter.
 *
 * @param dir The absolute path from which the handler is to be obtained.
 * @param fhand A handler, which will be the path handler in case of a successful call.
 * @param cli Pointer to the CLIENT structure (i.e. the MNT connection).
 *
 * @return An integer that indicates error in case of a
 * negative value, and success in case of a value equal to 0.
 */
int nfs3_mount(char *dir, fhandle3 *fhand, CLIENT *cli)
{
  mountres3 fh1;
  int res;
  // char *dirAux;

#ifdef DEBUG_MNT
  printf("Ready to do MOUNT3 (%s)\n", dir);
#endif

  fhand->fhandle3_val = (char *)malloc(sizeof(char) * FHSIZE3);
  fhand->fhandle3_len = FHSIZE3;
  // fh1 = (mountres3 *)malloc(sizeof(mountres3));
  // fh1->mountres3_u.mountinfo.fhandle.fhandle3_val = (char *)malloc(sizeof(char)*FHSIZE3);
  // fhand->fhandle3_val = fh.mountres3_u.mountinfo.fhandle.fhandle3_val = NULL;
  // fhand->fhandle3_len = fh1->mountres3_u.mountinfo.fhandle.fhandle3_len = FHSIZE3;
  // fhand->fhandle3_val = fh1->mountres3_u.mountinfo.fhandle.fhandle3_val;
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

  // printf("antes mountproc_null_3\n");
  // res = mountproc3_null_3(NULL,NULL, cli);
  // printf("desp. mountproc_null_3\n");
  // if(res != 0){
  //	clnt_perror(cli,"Error:");
  //}
  // printf("antes mountproc3_mnt_3\n");
  res = mountproc3_mnt_3(&dir, &fh1, cli);
  // printf("desp. mountproc3_mnt_3\n");
  /* comprobamos el estado de la conexi�n */
  if (res != 0)
  {

    // free(fh1);
#ifdef DEBUG_MNT
    printf("Error in MOUNT3 (%s) STATUS_CONNECTION (%d)\n", dir, res);
#endif
    /* en caso de error de conexion devolvemos un error generico
       de error de conexi�n */
    return -1;
  }
  else
  {
    if (fh1.fhs_status != 0)
    {
      /* en caso de que fh.status sea distinto a 0, *
       * devolvemos el error pero con valor negativo     */
#ifdef DEBUG_MNT
      printf("Error in MOUNT3 (%s) STATUS (%d)\n", dir, res);
#endif
      // free(fh1);
      fprintf(stderr, "nfs3_mount3: Error in MOUNT (%s) STATUS (%d)\n", dir, res);
      return -(int)fh1.fhs_status;
    }
  }
  /* en caso de exito */
  /* copiamos el manejador obtenido al manejador pasado por parametro */
  // fhand->fhandle3_len = fh1->mountres3_u.mountinfo.fhandle.fhandle3_len;
  // memcpy(fhand->fhandle3_val, fh.mountres3_u.mountinfo.fhandle.fhandle3_val, fhand->fhandle3_len);
  // fhand->fhandle3_val = fh1->mountres3_u.mountinfo.fhandle.fhandle3_val;

  // free(fh1);
  // printf("despues %p\n",fh.mountres3_u.mountinfo.fhandle.fhandle3_val);
#ifdef DEBUG_MNT
  printf("Successfully MOUNT3 (%s) STATUS (%d)\n", dir, res);
#endif
  // printf("*** cli->cl_auth = %p\n",cli->cl_auth);

  return 0;
}

/**
 * @brief NFS3 umount.
 *
 * This function deletes the entry in the table
 * of path's mounted on the server.
 *
 * @param path The absolute path of a mounted directory.
 * @param cli Pointer to the CLIENT structure (i.e. the MNT connection).
 *
 * @return An integer indicating error if negative,
 * and success in case of being equal to 0.
 */
int nfs3_umount(char *path, CLIENT *cli)
{
  int res;

#ifdef DEBUG_MNT
  printf("Ready to do UMOUNT3 (%s)\n", path);
#endif
  /****************************************************************
   * los argumentos de la llamada MNT son :			  *
   *	ENTRADA:						  *
   *	- el path que se desea desmontar			  *
   ****************************************************************/
  res = mountproc3_umnt_3(&path, NULL, cli);

  if (res != 0)
  {
#ifdef DEBUG_MNT
    printf("Error in UMOUNT3 (%s) STATUS_CONNECTION (%d)\n", path, res);
#endif
    fprintf(stderr, "nfs3_umount: Error in UMOUNT3 (%s) STATUS_CONNECTION (%d)\n", path, res);
    /* en caso de que ocurra un error en la conexion se
       devuelve este codigo de error */
    return -1;
  }
  else
#ifdef DEBUG_MNT
    printf("Successfully UMOUNT3 (%s) STATUS (%d)\n", path, res);
#endif
  return 0;
}

/**
 * @brief Directories exported.
 *
 * This function collects all the directories
 * exported by a server.
 *
 * @param exp Pointer to a structure of type exports,
 * which on success will point to a list containing the
 * directories exported by the server, along with the
 * permissions associated with the directory.
 * @param cli Pointer to the CLIENT structure (i.e. the MNT connection).
 *
 * @return An integer indicating error in case of being negative, and success in case of being equal to 0.
 */
int nfs3_export(__attribute__((__unused__)) exports3 *exp, __attribute__((__unused__)) CLIENT *cli)
{
  return 0;
}

/**
 * @brief Create connection.
 *
 * This function creates a connection between the client
 * and the NFS server. The established connection protocol
 * can be TCP or UDP, although UDP is used by default.
 * This connection can only be used by one process at a time.
 *
 * @param name NFS server name.
 * @param type 'TODO'
 * @return A pointer to a CLIENT structure (this is
 * the connection made). If it returns NULL, an error has
 * occurred in the process.
 */
CLIENT *create_connection_nfs3(char *name, int type)
{
  /* puntero a la conexi�n*/
  CLIENT *cli;
  /* estructuras utilizas en el proceso de conexi�n */
  struct sockaddr_in raddr;
  struct hostent *hp;
  struct timeval wait;
  /* variables auxiliares utilizadas en el proceso*/
  int sock, ret;
  char s[MNTNAMLEN];

#ifdef DEBUG_NFS
  printf("Ready to do creation connection with NFS3 service\n");
#endif
  /*
       Esta es la manera original de realizar la conexi�n
       cli=clnt_create(name, NFS_PROGRAM ,NFS_VERSION ,"udp");
  */

  /* obtengo la direccion del host del servidor NFS a partir del nombre */
  hp = gethostbyname(name);
  if (hp == NULL)
  {
    fprintf(stderr, "create_connection_nfs3: Fail gethostbyname %s\n.", name);
    return NULL;
  }
  /* se inicializan los parametros del socket */
  // bzero((char *)&raddr, sizeof(raddr));
  raddr.sin_family = AF_INET;
  memcpy(&(raddr.sin_addr), hp->h_addr, hp->h_length);
  raddr.sin_port = htons(0);
  sock = RPC_ANYSOCK;

  if (type == NFS3_TCP)
  {
    /* con esta funcion se realiza la creacion de la conexion tcp */
#ifdef DEBUG_NFS
    printf("Creation connection TCP client with NFS service\n");
#endif
    cli = clnttcp_create(&raddr, NFS3_PROGRAM, NFS3_VERSION, &sock, NFS3_SENDSZ, NFS3_RECVSZ);
  }
  else
  {
    if (type == NFS3_UDP)
    {
      /* tiempo maximo de espera entre la petici�n de cliente */
      /* y la respuesta del servidor segundos */
      wait.tv_sec = 1;
      /* milisegundos */
      wait.tv_usec = 0;
      /* con esta funcion se realiza la creacion de la conexion udp */
#ifdef DEBUG_NFS
      printf("Creation connection UDP client with NFS3 service\n");
#endif
      cli = clntudp_bufcreate(&raddr, NFS3_PROGRAM, NFS3_VERSION, wait, &sock, NFS3_SENDSZ, NFS3_RECVSZ);
    }
    else
    {
      cli = NULL;
    }
  }

  if (cli == NULL)
  {
#ifdef DEBUG_NFS
    printf("Error connection NFS3 service\n");
#endif
    fprintf(stderr, "create_connection_nfs3: Can't create connection with NFS service with %s \n.", name);
    return cli;
  }

  /* se elimina la autenticacion que  existe por defecto */
  auth_destroy(cli->cl_auth);
  /* se obtiene el nombre del host del cliente para a�adir a los parametros de autenticacion */
  ret = gethostname(s, 200);
  if (ret < 0)
  {
    fprintf(stderr, "create_connection_nfs3: Fail gethostname %s\n.", name);
    close_connection_nfs3(cli);
    return NULL;
  }
  /* con esto se a�ade cierta seguridad a la comunicaci�n entre el cliente y el servidor */
  /* el tipo de seguridad utilizada es UNIX (vease la Rfc de las RPCs para mas informaci�n)*/
#ifdef LINUX
  cli->cl_auth = authunix_create(s, getuid(), getgid(), 0, NULL);
#else
  cli->cl_auth = authunix_create(s, 501, 501, 0, NULL);
#endif
  return cli;
}

/**
 * @brief Close connection.
 *
 * This function deletes a connection made to an NFS server.
 *
 * @param cl Pointer to the CLIENT structure.
 * @par Returns
 *    Nothing.
 */
void close_connection_nfs3(CLIENT *cl)
{
  /* elimino la autenticacion */
  if (!cl)
  {

    if (!cl->cl_auth)
    {
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

/**
 * @brief Set attributes.
 *
 * This function incorporates the attributes into a
 * file system object for which you have the handle.
 *
 * @param fh A handler, which will be the handler of the object from which you want to get the attributes.
 * @param fatt A pointer to a structure of type fatt (reserved by the user) that will be filled in case the call succeeds.
 * @param cl A pointer to the CLIENT structure (i.e. the NFS connection).
 * @return An integer indicating error in case of being negative, and success in case of being equal to 0.
 */
int nfs3_setattr(fhandle3 *fh, fattr3 *fatt, CLIENT *cl)
{
  wccstat3 res;
  int ret;
  setattr3args sat;

#ifdef DEBUG_NFS
  printf("Ready to do SETATTR ()\n");
#endif

  memcpy(&sat.object, fh, NFS3_FHSIZE);
  sat.new_attributes.mode.set_uint32_u.val = fatt->mode;
  sat.new_attributes.uid.set_uint32_u.val = fatt->uid;
  sat.new_attributes.gid.set_uint32_u.val = fatt->gid;
  sat.new_attributes.size.set_uint64_u.val = fatt->size;
  sat.new_attributes.atime.set_time_u.time = fatt->atime;
  sat.new_attributes.mtime.set_time_u.time = fatt->mtime;

  if ((ret = nfsproc3_setattr_3(&sat, &res, cl)) != NFS3_OK)
  {
#ifdef DEBUG_NFS
    printf("Error in SETATTR () STATUS_CONNECTION (%d)\n", ret);
#endif
    return -1;
  }

  if (res.status != NFS3_OK)
  {
#ifdef DEBUG_NFS
    printf("Error in SETATTR () STATUS (%d)\n", -(int)res.status);
#endif
    return -(int)res.status;
  }

#ifdef DEBUG_NFS
  printf("successfully SETATTR () STATUS (%d)\n", -(int)res.status);
#endif
  return NFS3_OK;
}

/**
 * @brief Get attributes.
 *
 * This function gets the attributes of the file system object
 * for which you have the handle.
 *
 * @param fh  handler, which will be the handler of the object
 * from which you want to obtain the attributes.
 * @param fatt Pointer to a structure of type fatt
 * (reserved by the user) that will be filled in case
 * the call is successful.
 * @param cl Pointer to the CLIENT structure (i.e. the NFS connection).
 * @return An integer indicating error if negative,
 * and success in case of being equal to 0.
 */
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
  if ((ret = nfsproc3_getattr_3(fh, &res, cl)) != NFS3_OK)
  {
#ifdef DEBUG_NFS
    printf("Error in GETATTR () STATUS_CONNECTION (%d)\n", ret);
#endif
    return -1;
  }
  if (res.status != NFS3_OK)
  {
#ifdef DEBUG_NFS
    printf("Error in GETATTR () STATUS (%d)\n", -(int)res.status);
#endif
    return -(int)res.status;
  }
  memcpy(fatt, &res.getattr3res_u.attributes, sizeof(fattr3));
#ifdef DEBUG_NFS
  printf("successfully GETATTR () STATUS (%d)\n", -(int)res.status);
#endif
  return NFS3_OK;
}

/**
 * @brief Get Handle.
 *
 * This function gets the handle to a path passed
 * as a parameter, via the handle to the directory
 * containing that path.
 *
 * @param fhin The directory handle.
 * @param path The path from which the new handle is wanted.
 * @param fhout The new handle.
 * @param att Pointer to a structure of type fattr3 (reserved by the user)
 * that will be filled in case the call succeeds if it has a value other than NULL.
 * @param cl Pointer to the CLIENT structure (i.e. the NFS connection).
 * 
 * @return An integer that indicates error in case of being negative,
 * and success in case of being greater or equal to 0 (in this case,
 * it indicates the type of object of the file system from which
 * the handler has been obtained).
 */
int nfs3_lookup(fhandle3 *fhin, char *path, fhandle3 *fhout, fattr3 *att, CLIENT *cl)
{

  diropargs3 arg;
  lookup3res res;
  fhandle3 fh;
  char path_aux[MNTNAMLEN], dir_aux[MNTNAMLEN];
  int ret;
  int end;

#ifdef DEBUG_NFS
  printf("Ready to do LOOKUP (%s)\n", path);
#endif

  if ((strcmp(path, "") == 0) || (strcmp(path, "/") == 0))
  {
    fhout->fhandle3_len = fhin->fhandle3_len;
    fhout->fhandle3_val = fhin->fhandle3_val;

    return 0;
  }

  strcpy(path_aux, path);

  fh.fhandle3_len = fhin->fhandle3_len;
  fh.fhandle3_val = fhin->fhandle3_val;
  res.lookup3res_u.resok.object.fhandle3_val = (char *)malloc(FHSIZE3 * sizeof(char));

  ret = -1;

  do
  {
    /* se inicializan los argumentos de entrada */
    arg.dir.fhandle3_len = fh.fhandle3_len;
    arg.dir.fhandle3_val = fh.fhandle3_val;
    /* separar ultimo dir del path */

    end = getFirstDir(dir_aux, path_aux);
    arg.name = dir_aux;

    // printf("dir_aux = %s strlen = %d\n",arg.name,strlen(arg.name));
    if (strlen(arg.name) == 0)
    {
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
    ret = nfsproc3_lookup_3(&arg, &res, cl);
    // printf("ret = %d end = %d res.status = %d\n",ret, end,res.status);
    if (end == 0)
    {
      fhout->fhandle3_len = res.lookup3res_u.resok.object.fhandle3_len;
      fhout->fhandle3_val = res.lookup3res_u.resok.object.fhandle3_val;
    }
    else
    {
      if (strcmp(path_aux, "/") == 0)
      {
        fhout->fhandle3_len = res.lookup3res_u.resok.object.fhandle3_len;
        fhout->fhandle3_val = res.lookup3res_u.resok.object.fhandle3_val;
        end = 0;
      }
      else
      {
        fh.fhandle3_len = res.lookup3res_u.resok.object.fhandle3_len;
        fh.fhandle3_val = res.lookup3res_u.resok.object.fhandle3_val;
      }
    }
  } while (end != 0);

  if (ret == 0)
    if (res.status == 0)
    {
      fhout->fhandle3_len = res.lookup3res_u.resok.object.fhandle3_len;
      fhout->fhandle3_val = res.lookup3res_u.resok.object.fhandle3_val;
      if (att != NULL)
      {
        /* si el puntero no es NULL, se rellena la estructura */
#ifdef DEBUG_NFS3
        printf("Copy attributes in LOOKUP (%s)\n", path);
#endif
        /* copiar atributos ..... TO DO*/
        if (res.lookup3res_u.resok.obj_attributes.present)
        {
          memcpy(att, &res.lookup3res_u.resok.obj_attributes.post_op_attr_u.attributes, sizeof(fattr3));
        }
      }
#ifdef DEBUG_NFS3
      printf("Successfully LOOKUP (%s) STATUS (%d)\n",
             path, res.lookup3res_u.resok.obj_attributes.post_op_attr_u.attributes.type);
#endif
      return res.lookup3res_u.resok.obj_attributes.post_op_attr_u.attributes.type;
    }
    else
    {

#ifdef DEBUG_NFS
      printf("Error in LOOKUP (%s) STATUS (%d)\n", path, -(int)res.status);
#endif
      return -(int)res.status;
    }
  else
  {
#ifdef DEBUG_NFS
    printf("Error in LOOKUP (%s) CONNECT_STATUS (%d)\n", path, ret);
#endif
    return -1;
  }
  return 0;
}

/**
 * @brief Read Data.
 *
 * This function reads data from a file.
 *
 * @param fh The file handle.
 * @param data The data buffer where the read data is stored.
 * @param offset The offset of the file, which indicates where to start reading from.
 * @param size The size of the data to be read.
 * @param cl Pointer to the CLIENT structure (i.e. the NFS connection).
 *
 * @return An integer indicating error if negative, and success
 * if greater than or equal to 0. In case of success the returned
 * value is the value of bytes read. If less bytes are read than
 * requested, the end of file has been reached.
 */
ssize_t nfs3_read(fhandle3 *fh, void *data, off_t offset, size_t size, CLIENT *cl)
{
  /* argumento de entrada a la llamada RPC */
  read3args args;
  /* argumento de salida de la llamada RPC */
  read3res res;
  size_t i, rd;
  int ret;

#ifdef DEBUG_NFS
  printf("Ready to do READ3 (%d,%d)\n", (uint)size, (uint)offset);
#endif

  /* Si se quieren leer 0 bytes o menos, directamente se sale de la funci�n */
  if (size <= 0)
  {
#ifdef DEBUG_NFS
    printf("successfully READ3 (%d, %d) STATUS (%d)\n", (uint)size, (uint)offset, (uint)size);
#endif
    return 0;
  }
  /* MAXDATA en el tama�o maximo que se puede leer en cada llamada RCP */

  /* este contador indica cuantos bytes se han leido */
  /* inicialmente es igual 0 */
  i = 0;
  /* mientras el tama�o de lo escrito sea menor al tama�o pedido */
  while (i < size)
  {
    /* si lo que queda por leer es mayor a MAXDATA, el tama�o a leer es
       MAXDATA */
    if ((size - i) >= MAXDATA3)
      rd = MAXDATA3;
    else
    {
      /* si es menor, el tama�o a leer en la llamada RPC es size - i*/
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
     *	- una estructura con el manejador, offset, y tama�o que   *
     *      se desea leer.                                          *
     *    SALIDA:                                                   *
     *    - una estructura con los datos leido y el tama�o leido.   *
     ****************************************************************/
    ret = nfsproc3_read_3(&args, &res, cl);
    // nfsproc3_read_3(read3args *, read3res *, CLIENT *);
    if (ret != 0)
    {

#ifdef DEBUG_NFS_ERR
      printf("Error in READ (%d,%d) CONNECT_STATUS (%d)\n", (uint)size, (uint)offset, ret);
#endif
      return -1;
    }
    else
    {
      if (res.status != 0)
      {
#ifdef DEBUG_NFS_ERR
        printf("Error in READ (%d,%d) STATUS (%d)\n", (uint)size, (uint)offset, -(int)res.status);
#endif
        return -(ssize_t)(res.status);
      }
      else
      {
        /* incremento lo leido */
        i = res.read3res_u.resok.data.data_len + i;
        /* si lo leido < que lo que se pide -> EOF */
        if (res.read3res_u.resok.data.data_len < rd)
        {
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

/**
 * @brief Write Data.
 *
 * This function writes data to a file.
 *
 * @param fh The file handle.
 * @param data The buffer with the data to be written.
 * @param offset The offset of the file, which indicates where to start writing from.
 * @param size The size of the data to be written.
 * @param cl Pointer to the CLIENT structure (i.e. the NFS connection).
 *
 * @return An integer indicating error if negative, and success if greater
 * than or equal to 0. In case of success the returned value is the value
 * of bytes read. If less bytes are read than requested, the end of file has been reached.
 */
ssize_t nfs3_write(fhandle3 *fh, void *data, off_t offset, size_t size, CLIENT *cl)
{
  /* argumento de entrada a la llamada RPC */
  write3args args;
  /* argumento de salida de la llamada RPC */
  write3res res;
  int i, wr, ret;

#ifdef DEBUG_NFS
  printf("Ready to do WRITE (%d,%d)\n", (uint)size, (uint)offset);
  printfh3("Write: ", fh);
#endif

  /* Si se quieren escribir 0 bytes o menos, directamente se sale de la funci�n */
  if (size <= 0)
  {
#ifdef DEBUG_NFS
    printf("successfully WRITE (%d, %d) STATUS (%d)\n", (uint)size, (uint)offset, (uint)size);
#endif

    return 0;
  }
  /* MAXDATA en el tama�o maximo que se puede escribir en cada llamada RCP */

  /* este contador indica cuantos bytes se han escrito */
  /* inicialmente es igual 0 */
  i = 0;
  /* mientras el tama�o de lo escrito sea menor al tama�o pedido */
  while ((unsigned int)i < size)
  {
    if ((size - i) >= MAXDATA3)
      wr = MAXDATA3;
    else
    {
      /* si es menor, el tama�o a escribir en la llamada RPC es size - i*/
      wr = size - i;
    }

    /* se rellenan los datos de la petici�n*/
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
     *	- una estructura con el manejador, offset, y tama�o que   *
     *      se desea escribir.                                      *
     *    SALIDA:                                                   *
     *    - una estructura con los nuevo atributos del fichero.     *
     ****************************************************************/
    ret = nfsproc3_write_3(&args, &res, cl);
    // printf("ret = %d res.status = %d\n",ret, res.status);
    // if (ret !=0)
    //   clnt_perror(cl, "Write: ");
    if (ret == 0)
    {
      if (res.status != 0)
      {
#ifdef DEBUG_NFS_ERR
        printf("Error in WRITE (%d,%d) STATUS (%d)\n", (uint)size, (uint)offset, -(int)res.status);
#endif
        return -(ssize_t)res.status;
      }
    }
    else
    {

#ifdef DEBUG_NFS_ERR
      printf("Error in WRITE (%d,%d) CONNECT_STATUS (%d)\n", (uint)size, (uint)offset, ret);
#endif
      return -1;
    }
    /* incremento el tama�o de lo leido */
    i = i + wr;
  }
#ifdef DEBUG_NFS
  printf("successfully WRITE (%d, %d) STATUS (%d)\n", (uint)size, (uint)offset, i);
#endif

  // res.write3res_u.resok.count
  return (ssize_t)i;
}

/**
 * @brief Create file.
 *
 * This function creates a file.
 *
 * @param fhin The directory handle where the file is to be created.
 * @param file The name of the file to be created.
 * @param mode The access permissions of the file to be created.
 * @param fhout An output handle, which in case of success,
 * will be the handle of the created file.
 * @param at A pointer to a structure with the attributes of the file,
 * which in case of success will be filled in.
 * If you do not want to use it, set it to NULL.
 * @param cl A pointer to the CLIENT structure (i.e. the NFS connection).
 *
 * @return An integer that indicates error in case
 * of being negative, and success in case of being equal to 0.
 */
int nfs3_create(fhandle3 *fhin, char *file, mode_t mode, fhandle3 *fhout, __attribute__((__unused__)) fattr3 *at, CLIENT *cl)
{
  /* argumento de entrada a la llamada RPC */
  create3args args;
  /* argumento de salida de la llamada RPC*/
  diropres3 res;
  // fhandle3 fh;
  int ret;

  // printf("Ready to do CREATE (%s,%d)\n", file, mode);
#ifdef DEBUG_NFS
  printf("Ready to do CREATE (%s,%d)\n", file, mode);
  printfh3("Create begin: ", fhin);
#endif
  /* se rellena la estructura de entrada */

  // printf("fhIn = %d - %p\n", fhin->fhandle3_len, fhin->fhandle3_val);
  // args.where.dir.fhandle3_len = fh.fhandle3_len;
  // args.where.dir.fhandle3_val = fh.fhandle3_val;
  // args.where.dir.fhandle3_len = fhin->fhandle3_len;
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

  res.diropres3_u.resok.obj.post_op_fh3_u.handle.fhandle3_val = (char *)malloc(sizeof(char) * FHSIZE3);
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
  // printf("antes de llamar a nfsproc3_create_3\n");
  if ((ret = nfsproc3_create_3(&args, &res, cl)) != 0)
  {
    // printf("Error in CREATE (%s,%d) CONNECT_STATUS (%d)\n", file, mode, ret);
#ifdef DEBUG_NFS
    printf("Error in CREATE (%s,%d) CONNECT_STATUS (%d)\n", file, mode, ret);
#endif
    return -1;
  }
  if (res.status != 0)
  {
    // printf("Error in CREATE (%s,%d) STATUS (%d)\n", file, mode, -(int)res.status);
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
    // if(at != NULL){
    //    memcpy(at, &res.diropres_u.fhand_attr.attributes, sizeof(fattr));
    //    // TODO
    // }
  }
#ifdef DEBUG_NFS
  printf("successfully CREATE (%s,%d) STATUS (%d)\n", file, mode, -(int)res.status);
  printfh3("Create end: ", fhout);
#endif

  return 0;
}

/**
 * @brief Remove File.
 *
 * This function deletes a file.
 *
 * @param fh The directory handle where the file is located.
 * @param file The name of the file to delete.
 * @param cl Pointer to the CLIENT structure (i.e. the NFS connection).
 *
 * @return An integer indicating error if negative, and success if 0.
 */
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
   *    - un status del estado de la operaci�n.                   *
   ****************************************************************/
  if ((ret = nfsproc3_remove_3(&args, &res, cl)) != 0)
  {
#ifdef DEBUG_NFS
    printf("Error in REMOVE (%s) CONNECT_STATUS (%d)\n", file, ret);
#endif
    return -1;
  }
  if (res.status != 0)
  {
#ifdef DEBUG_NFS
    printf("Error in REMOVE (%s) STATUS (%d)\n", file, -(int)res.status);
#endif
    return -(int)(res.status);
  }
  else
  {
#ifdef DEBUG_NFS
    printf("Successfully REMOVE (%s) STATUS (%d)\n", file, -(int)res.status);
#endif
    return 0;
  }
}

/**
 * @brief Rename File or Directory.
 *
 * This function renames a file or directory.
 *
 * @param fh The directory handle where the file or directory is located.
 * @param name The name of the file or directory to rename.
 * @param fhR The directory handle of the directory where
 * the renamed file or directory is to be found.
 * @param nameR The new name of the file or directory.
 * @param cl Pointer to the CLIENT structure (i.e. the NFS connection).
 *
 * @return An integer indicating error if negative, and success if 0.
 */
int nfs3_rename(__attribute__((__unused__)) fhandle3 *fh, __attribute__((__unused__)) char *name, __attribute__((__unused__)) fhandle3 *fhR, __attribute__((__unused__)) char *nameR, __attribute__((__unused__)) CLIENT *cl)
{
  return 0;
}

/**
 * @brief Create a Directory.
 *
 * This function creates a directory.
 *
 * @param fhin The directory handle where the directory is to be created.
 * @param dir The name of the directory to be created.
 * @param mode The access permissions of the directory to be created.
 * @param fhout An output handle, which in case of success, will be the handle of the created directory.
 * @param at A pointer to a structure with the attributes of the directory,
 * which in case of success will be filled in. If you do not want to use it, set it to NULL.
 * @param cl Pointer to the CLIENT structure (i.e. the NFS connection).
 *
 * @return An integer that indicates error in case of being negative, and success in case of being equal to 0.
 */
int nfs3_mkdir(__attribute__((__unused__)) fhandle3 *fhin, __attribute__((__unused__)) char *dir, __attribute__((__unused__)) mode_t mode, __attribute__((__unused__)) fhandle3 *fhout, __attribute__((__unused__)) fattr3 *at, __attribute__((__unused__)) CLIENT *cl)
{
  return 0;
}

/**
 * @brief Remove a directory.
 *
 * This function deletes a directory.
 *
 * @param fh The directory handle where the directory is located.
 * @param dir The name of the directory to be deleted.
 * @param cl Pointer to the CLIENT structure (i.e. the NFS connection).
 *
 * @return An integer that indicates error in case of being negative,
 * and success in case of being equal to 0.
 */
int nfs3_rmdir(__attribute__((__unused__)) fhandle3 *fh, __attribute__((__unused__)) char *dir, __attribute__((__unused__)) CLIENT *cl)
{
  return 0;
}

/**
 * @brief Read entries.
 *
 * This function reads entries from a directory.
 *
 * @param fh The directory handle of the directory from which you want to read entries.
 * @param cookie A cookie, which is filled with the last entry read.
 * It is used to know where you want to start reading the entries.
 * If you want to read from the first entry, the cookie must have a value of 0.
 * @param entr A pointer to an array of strings, which is where,
 * in case of success, the entries will be stored.
 * @param cl Pointer to the CLIENT structure (i.e. the NFS connection).
 *
 * @return An integer that indicates error in case of being negative,
 * and success in case of being equal or greater than 0.
 * If greater than or equal to 0, this number indicates the number of entries read.
 */
int nfs3_readdir(__attribute__((__unused__)) fhandle3 *fh, __attribute__((__unused__)) cookieverf3 cookie, __attribute__((__unused__)) char *entr, __attribute__((__unused__)) CLIENT *cl)
{
  return 0;
}

/**
 * @brief Get features Server.
 *
 * This function obtains features from the NFS server.
 *
 * @param arg The directory handle.
 * @param inf A pointer to a structure of type info containing the server information, in case of success.
 * @param cl A pointer to the CLIENT structure (i.e. the NFS connection).
 *
 * @return An integer indicating error in case of being negative,
 * and success in case of being equal to 0.
 */
int nfs3_statfs(__attribute__((__unused__)) fhandle3 *arg, __attribute__((__unused__)) fsinfo3resok *inf, __attribute__((__unused__)) CLIENT *cl)
{
  return 0;
}
