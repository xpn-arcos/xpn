
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


  /* ... Global Variable / Variable Globales ........................... */

struct tcp_server_node_st
{
	char host[255];
	int  port;
	char name[255];
};

static int load = 0;
static struct tcp_server_node_st tcp_server_node[MAX_TCP_SERVER_NODES];
static int num_tcp_server_nodes = 0;


  /* ... Functions / Funciones ......................................... */

  
int tcp_server_readFile ( void )
{
    FILE * fd;
    char * name = NULL;

    printf("[NS_TCP] begin the translation\n");

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
            tcp_server_node[num_tcp_server_nodes].port))
    {
        num_tcp_server_nodes++;

        if (num_tcp_server_nodes >= MAX_TCP_SERVER_NODES) {
            fprintf(stderr, "Error: num_tcp_server_nodes >= MAX_TCP_SERVER_NODES\n");
            exit(0);
        }
    }
    fclose(fd);
    printf("[NS_TCP] end the translation\n");

    return 0 ;
}

int tcp_server_translate(char * server, char * newserver, int * port)
{
    int i;

    debug_info("[NS_TCP] Buscando 1 ... %s\n", server);

    /*************************************/
    /* DON'T WORK WITH THREADS */
    if (!load) {
        load = 1;
        printf("[NS_TCP] Cargando Fichero ... \n");

        tcp_server_readFile();
    }
    /*************************************/
    printf("[NS_TCP] Buscando 2 ... %s\n", server);

    for (i = 0; i < num_tcp_server_nodes; i++)
    {
        if (strcmp(server, tcp_server_node[i].name) == 0)
	{
            strcpy(newserver, tcp_server_node[i].host);
            *port = tcp_server_node[i].port;

            debug_info("[NS_TCP] Encontrado ... %s %d\n",
                        tcp_server_node[i].host, tcp_server_node[i].port);
            return 0;
        }
    }

    if (i == num_tcp_server_nodes)
    {
        fprintf(stderr, "translate: error %s not found (%d)\n", server, num_tcp_server_nodes);
	return -1;
    }

    return 0 ;
}


int tcp_server_updateFile(char * name, char * file, int port)
{
  char   host[1024];
  FILE * f;

  debug_info("[COMM] begin tcp_server_comm_init(%s, %d, %s)\n", name, port, file);

  // save host name and port...
  f = fopen(file, "a+");
  if (f == NULL) {
    perror("FOPEN: ");
    return -1;
  }

  gethostname(host, 1024);
  fprintf(f, "%s %s %d\r\n", name, host, port);
  fclose(f);

  return 0;
}

