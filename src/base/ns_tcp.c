
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

  #include "ns_tcp.h"


  /* ... Functions / Funciones ......................................... */

  
void tcp_server_readFile() {

    FILE * fd;
    char * name = NULL;

    printf("[NFI_COMM]begin the translation\n");

    name = getenv(TCP_SERVER_FILE);
    if ((name == NULL) || (strcmp(name, "") == 0)) {
        name = TCP_SERVER_FILE_DEFAULT;
    }

    fd = fopen(name, "r");
    if (fd == NULL) {
        fprintf(stderr, "tcp_server_readFile: can't open %s\n", name);
        exit(-1);
    }
    while (EOF != fscanf(fd, "%s %s %d",
            tcp_server_node[num_tcp_server_nodes].name,
            tcp_server_node[num_tcp_server_nodes].host, &
            tcp_server_node[num_tcp_server_nodes].port)) {
        /*
        printf("[NFI_COMM]-%d> %s %s %d -\n",
        num_tcp_server_nodes,
        tcp_server_node[num_tcp_server_nodes].name,
        tcp_server_node[num_tcp_server_nodes].host,
        tcp_server_node[num_tcp_server_nodes].port);
        */
        num_tcp_server_nodes++;

        if (num_tcp_server_nodes >= MAX_TCP_SERVER_NODES) {
            fprintf(stderr, "Error: num_tcp_server_nodes >= MAX_TCP_SERVER_NODES\n");
            exit(0);
        }
    }
    fclose(fd);
    printf("[NFI_COMM]end the translation\n");

}

void tcp_server_translate(char * server, char * newserver, int * port) {
    int i;

    /*************************************/
    printf("[NFI_COMM]Buscando 1 ... %s\n", server);

    /* DON'T WORK WITH THREADS */
    if (!load) {
        load = 1;
        printf("[NFI_COMM]Cargando Fichero ... \n");

        tcp_server_readFile();
    }
    /*************************************/
    printf("[NFI_COMM]Buscando 2 ... %s\n", server);

    for (i = 0; i < num_tcp_server_nodes; i++) {
        if (strcmp(server, tcp_server_node[i].name) == 0) {
            strcpy(newserver, tcp_server_node[i].host);

            /*
       printf("[NFI_COMM]Encontrado ... %s %d\n",
        tcp_server_node[i].host,
        tcp_server_node[i].port);
        */
            #ifdef DBG_COMM
                printf("[NFI_COMM]Encontrado ... %s %d\n",
                tcp_server_node[i].host,
                tcp_server_node[i].port);
            #endif
                *port = tcp_server_node[i].port;
            break;
        }
    }
    if (i == num_tcp_server_nodes) {
        fprintf(stderr, "translate: error %s not found (%d)\n", server, num_tcp_server_nodes);
        exit(-1);
    }
}


int tcp_server_updateFile(char * name, char * file, int port)
{
  char host[255];
  FILE * f;

  // save host name and port...
  f = fopen(file, "a+");

  if (f == NULL) {
    perror ( "FOPEN: " );
    return -1;
  }

  gethostname(host, 255);
  fprintf(f, "%s %s %d\r\n", name, host, port);
  fclose(f);

  debug_info("[COMM] begin tcp_server_comm_init(%s, %d, %s)\n", name, port, file);

  return 0;
}

