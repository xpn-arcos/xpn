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

/* ... Include / Inclusion ........................................... */

#include "tcp_server_comm.h"


/* ... Global variables / Variables globales ......................... */

static int global_sock = 0;

/*MOSQUITTO STRUCT*/
struct mosquitto *mosqtcpserver;

/* ... MOSQUITTO Functions / MOSQUITO Funciones ......................................... */
void on_connect(struct mosquitto *mosqtcpserver, void *obj, int reason_code)
{
	int rc;
	//printf("on_connect: %s\n", mosquitto_connack_string(reason_code));

	if(reason_code != 0){
		mosquitto_disconnect(mosqtcpserver);
	}

	rc = mosquitto_subscribe(mosqtcpserver, NULL, "prueba.txt", 0);
	if(rc != MOSQ_ERR_SUCCESS){
		fprintf(stderr, "Error subscribing open: %s\n", mosquitto_strerror(rc));
		mosquitto_disconnect(mosqtcpserver);
	}

}


void on_subscribe(struct mosquitto *mosqtcpserver, void *obj, int mid, int qos_count, const int *granted_qos)
{
	int i;
	bool have_subscription = false;

	for(i=0; i<qos_count; i++){
		//printf("on_subscribe: %d:granted qos = %d\n", i, granted_qos[i]);
		if(granted_qos[i] <= 2){
			have_subscription = true;
		}
	}
	if(have_subscription == false){
		fprintf(stderr, "Error: All subscriptions rejected.\n");
		mosquitto_disconnect(mosqtcpserver);
	}
}













/* ... Functions / Funciones ......................................... */

// TEST
int tcp_server_comm_writedata_test(int fd, char * id) {
  int ret;
  char buffer_temp[CONST_TEMP], aux;
  int i;

  debug_info("[COMM] ===init write test ID=%s:%p --th:%d--===\n", id, id, (int) pthread_self());
  for (i = 0; i < CONST_TEMP; i++) {
    aux = (char)(i % 128);
    buffer_temp[i] = aux;
  }

  ret = write(fd, buffer_temp, CONST_TEMP);
  if (ret == 0) {
    perror("[COMM] server:ERROR TEST(0) write");
    return 0;
  }
  if (ret != CONST_TEMP) {
    debug_info("[COMM] server:ERROR TEST(1) write_data(%d): err %d  ID=%s:%p --th:%d--\n", fd, ret, id, id, (int) pthread_self());
    //exit(-1);
    perror("[COMM] server:ERROR TEST(1) write");
  }
  debug_info("[COMM] send write test ok ID=%s\n", id);

  bzero(buffer_temp, CONST_TEMP);
  ret = read(fd, buffer_temp, CONST_TEMP);
  if (ret == 0) {
    perror("[COMM] server:ERROR TEST(1.1) write");
    return 0;
  }
  if (ret != CONST_TEMP) {
    debug_info("[COMM] server:ERROR TEST(2) write_data(%d): err %d  ID=%s:%p --th:%d--\n", fd, ret, id, id, (int) pthread_self());
    //exit(-1);
    perror("[COMM] server:ERROR TEST(2) write");
  }
  debug_info("[COMM] receive write test ok ID=%s:%p\n", id, id);
  for (i = 0; i < CONST_TEMP; i++) {
    aux = (char)(i % 128);
    if (buffer_temp[i] != aux) {
      debug_info("[COMM] server:ERROR TEST(3) write_data(%d): err %d(%d!=%d) ID=%s:%p --th:%d--\n", fd, ret, buffer_temp[i], aux, id, id, (int) pthread_self());
      perror("[COMM] server:ERROR TEST(3) write");
    }
  }
  debug_info("[COMM] ===check write test ok ID=%s:%p--th:%d--===\n", id, id, (int) pthread_self());

  return 0;
}

int tcp_server_comm_readdata_test(int fd, char * id) {
  int ret;
  char buffer_temp[CONST_TEMP], aux;
  int i;

  debug_info("[COMM] ===init read test ok ID=%s:%p --th:%d--===\n", id, id, (int) pthread_self());

  bzero(buffer_temp, CONST_TEMP);
  ret = read(fd, buffer_temp, CONST_TEMP);
  if (ret == 0) {
    perror("[COMM] server:ERROR TEST(0) read");
    return 0;
  }

  if (ret != CONST_TEMP) {
    debug_info("[COMM] server:ERROR TEST(1) read_data(%d): err %d ID=%s:%p --th:%d--\n", fd, ret, id, id, (int) pthread_self());
    //exit(-1);
    perror("[COMM] server:ERROR TEST(1) read");
  }
  debug_info("[COMM] receive read test ok ID=%s:%p\n", id, id);
  for (i = 0; i < CONST_TEMP; i++) {
    aux = (char)(i % 128);
    if (buffer_temp[i] != aux) {
      debug_info("[COMM] server:ERROR TEST(2) write_data(%d): err %d(%d!=%d)  ID=%s:%p --th:%d--\n", fd, ret, buffer_temp[i], aux, id, id, (int) pthread_self());
      //printf("[COMM] server:ERROR TEST(2) read_data(%d): err %d  ID=%s:%p\n",fd,ret,id,id);
      perror("[COMM] server:ERROR TEST(2) read");
    }
  }
  debug_info("[COMM] check read test ok ID=%s:%p --th:%d--\n", id, id, (int) pthread_self());

  for (i = 0; i < CONST_TEMP; i++) {
    aux = (char)(i % 128);
    buffer_temp[i] = aux;
  }

  ret = write(fd, buffer_temp, CONST_TEMP);
  if (ret == 0) {
    perror("[COMM] server:ERROR TEST(2.1) read");
    return 0;
  }
  if (ret != CONST_TEMP) {
    debug_info("[COMM] server:ERROR TEST(3) read_data(%d): err %d  ID=%s:%p --th:%d--\n", fd, ret, id, id, (int) pthread_self());
    perror("[COMM] server:ERROR TEST(3) read");
  }
  debug_info("[COMM] ===send read test ok ID=%s:%p--th:%d--===\n", id, id, (int) pthread_self());

  return 0;
}

/* AUXILIAR FUNCTIONS */
int tcp_server_comm_init(char * name, int port, char * file) {
  struct sockaddr_in server_addr;
  int val, ret;
  char host[255];
  FILE * f;

  debug_info("[COMM] begin tcp_server_comm_init(%s, %d, %s)\n", name, port, file);

  /* create the connections */
  global_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (global_sock < 0) {
    perror("error en el socket:");
    return -1;
  }

  // tcp_nodalay
  val = 1;
  if (setsockopt(global_sock, IPPROTO_TCP, TCP_NODELAY, & val, sizeof(val)) == -1) {
    perror("setsockopt: ");
    return -1;
  }

  //NEW
  val = 1024 * 1024; //1 MB

  if (setsockopt(global_sock, SOL_SOCKET, SO_SNDBUF, (char * ) & val, sizeof(int)) == -1) {
    perror("setsockopt: ");
    return -1;
  }

  val = 1024 * 1024; //1 MB
  if (setsockopt(global_sock, SOL_SOCKET, SO_RCVBUF, (char * ) & val, sizeof(int)) == -1) {
    perror("setsockopt: ");
    return -1;
  }

  // sock_reuseaddr
  val = 1;
  ret = setsockopt(global_sock, SOL_SOCKET, SO_REUSEADDR, (char * ) & val, sizeof(int));
  if (ret == -1) {
    perror("error en el setsockopt:");
    return -1;
  }
  bzero((char * ) & server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(port);

  // bind & listen
  ret = bind(global_sock, (struct sockaddr * ) & server_addr, sizeof(server_addr));
  if (ret == -1) {
    perror("Error en el bind:");
    return -1;
  }
  listen(global_sock, 20);

  ret = tcp_server_updateFile(name, file, port);

  if (ret == -1) {
    perror("[%d]\tError en tcp_server_updateFile:", __LINE__);
    return -1;
  }

  printf("[%d]\tBEGIN INIT MOSQUITTO TCP_SERVER\n\n", __LINE__);

  mosquitto_lib_init();

  mosqtcpserver = mosquitto_new(NULL, true, NULL);
  if(mosqtcpserver == NULL)
  {
    fprintf(stderr, "Error: Out of memory.\n");
    return 1;
  }

  mosquitto_connect_callback_set(mosqtcpserver, on_connect);
  mosquitto_subscribe_callback_set(mosqtcpserver, on_subscribe);
  mosquitto_message_callback_set(mosqtcpserver, on_message);

#ifndef MOSQ_OPT_TCP_NODELAY
  #define MOSQ_OPT_TCP_NODELAY 1
#endif

  mosquitto_int_option(mosqtcpserver, MOSQ_OPT_TCP_NODELAY, 1);

  int rc = mosquitto_connect(mosqtcpserver, "localhost", 1883, 60);

  if(rc != MOSQ_ERR_SUCCESS)
  {
    mosquitto_destroy(mosqtcpserver);
    fprintf(stderr, "[%d]\tERROR INIT MOSQUITTO TCP_SERVER: %s\n", __LINE__, mosquitto_strerror(rc));
    return 1;
  }

  printf("[%d]\tEND INIT MOSQUITTO TCP_SERVER\n\n", __LINE__);

  return 0;
}

void tcp_server_close_comm() {
  close(global_sock);
}

int tcp_server_accept_comm() {
  struct sockaddr_in client_addr;
  int sc, flag;
  socklen_t size = sizeof(struct sockaddr_in);

  debug_info("[COMM] begin tcp_server_accept_comm()\n");

  debug_info("[COMM] antes accept conection ....\n");
  sc = accept(global_sock, (struct sockaddr * ) & client_addr, & size);
  if (sc == -1) {
    perror("accept: ");
  }
  debug_info("[COMM] desp. accept conection .... %d\n", sc);

  // tcp_nodelay
  flag = 1;
  if (setsockopt(sc, IPPROTO_TCP, TCP_NODELAY, & flag, sizeof(flag)) == -1) {
    perror("setsockopt: ");
    return -1;
  }

  //NEW
  int val = 1024 * 1024; //1 MB

  if (setsockopt(sc, SOL_SOCKET, SO_SNDBUF, (char * ) & val, sizeof(int)) == -1) {
    perror("setsockopt: ");
    return -1;
  }

  val = 1024 * 1024; //1 MB
  if (setsockopt(sc, SOL_SOCKET, SO_RCVBUF, (char * ) & val, sizeof(int)) == -1) {
    perror("setsockopt: ");
    return -1;
  }

  debug_info("[COMM] end tcp_server_accept_comm()\n");

  return sc;
}

int tcp_server_destroy_comm() {

  printf("[%d]\tBEGIN DESTROY MOSQUITTO TCP_SERVER\n\n", __LINE__);
  mosquitto_loop_forever(mosqtcpserver, -1, 1);
  mosquitto_lib_cleanup();
  printf("[%d]\tEND DESTROY MOSQUITTO TCP_SERVER\n\n", __LINE__);

  return 0;
}

/**********************************
Write data to client using sockets
***********************************/
ssize_t tcp_server_comm_write_data(int fd, char * data, ssize_t size, char * id) {
  int ret = 0;
  int cont = 0;

  #ifdef DBG_COMM
  tcp_server_comm_writedata_test(fd, id);
  #endif

  debug_info("[COMM] server: begin write_comm(%d): %d data %p ID=%s:%p --th:%d--\n", fd, size, data, id, id, (int) pthread_self());

  // check params...
  if (size == 0) {
    return 0;
  }
  if (size < 0) {
    return -1;
  }
  if (NULL == id) {
    fprintf(stderr, "tcp_server_comm_writedata: NULL id argument.\n");
  }

  do {
    debug_info("[COMM] server:write_comm(%d) antes: %d = %d data %p ID=%s:%p --th:%d--\n", fd, size, ret, data, id, id, (int) pthread_self());
    ret = write(fd, data + cont, size - cont);
    if (ret < 0) {
      perror("server: Error write_comm:");
    }
    debug_info("[COMM] server:write_comm(%d) desp: %d = %d data %p ID=%s:%p --th:%d--\n", fd, size, ret, data, id, id, (int) pthread_self());
    cont += ret;
  } while ((ret > 0) && (cont != size));

  if (ret == -1) {
    debug_info("[COMM] server: Error write_comm(%d): -1 ID=%s:%p\n", fd, id, id);
    return ret;
  }
  debug_info("[COMM] server: end write_comm(%d): %d of %d data %p ID=%s:%p --th:%d--\n", fd, cont, size, data, id, id, (int) pthread_self());

  #ifdef DBG_COMM
  tcp_server_comm_writedata_test(fd, id);
  #endif

  return size;
}

/**********************************
Read data from client using sockets
***********************************/
ssize_t tcp_server_comm_read_data(int fd, char * data, ssize_t size, char * id) {
  int ret = 0;
  int cont = 0;

  #ifdef DBG_COMM
  tcp_server_comm_readdata_test(fd, id);
  #endif

  debug_info("[COMM] server: begin read_comm(%d): %d data %p ID=%s:%p --th:%d--\n", fd, size, data, id, id, (int) pthread_self());

  // check params...
  if (size == 0) {
    return 0;
  }
  if (size < 0) {
    return -1;
  }
  if (NULL == id) {
    fprintf(stderr, "tcp_server_comm_writedata: NULL id argument.\n");
  }

  do {
    debug_info("[COMM] server:read_comm(%d) antes: %d = %d data %p ID=%s:%p --th:%d--\n", fd, size, ret, data, id, id, (int) pthread_self());
    ret = read(fd, data + cont, size - cont);
    if (ret < 0) {
      perror("server: Error read_comm:");
    }
    debug_info("[COMM] server:read_comm(%d) desp: %d = %d data %p ID=%s:%p --th:%d--\n", fd, size, ret, data, id, id, (int) pthread_self());
    cont += ret;
  } while ((ret > 0) && (cont != size));

  if (ret == -1) {
    debug_info("[COMM] server: Error read_comm(%d): -1 ID=%s:%p\n", fd, id, id);
    return ret;
  }
  debug_info("[COMM] server: end read_comm(%d): %d of %d data %p ID=%s:%p --th:%d--\n", fd, cont, size, data, id, id, (int) pthread_self());

  #ifdef DBG_COMM
  tcp_server_comm_readdata_test(fd, id);
  #endif
  return size;
}

/* ................................................................... */
