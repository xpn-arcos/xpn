
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


/* ... Include / Inclusion ........................................... */

#include "mpi_server_params.h"
#include "base/ns.h"


/* ... Const / Const ................................................. */


/* ... Global variables / Variables globales ........................ */


/* ... Functions / Funciones ......................................... */

void mpi_server_params_show ( mpi_server_param_st *params )
{
  debug_info("[Server=%d] [MPI_SERVER_PARAMS] [mpi_server_params_show] >> Begin\n", params->rank);

  printf(" * MPI server current configuration:\n");
  // * dns_file
  printf("\t-ns <path>:\t'%s'\n",   params->dns_file);
  // * threads
  if (params->thread_mode == TH_NOT) {
    printf("\t-t  <int>:\tWithout threads\n");
  }
  if (params->thread_mode == TH_POOL) {
    printf("\t-t  <int>:\tThread Pool Activated\n");
  }
  if (params->thread_mode == TH_OP) {
    printf("\t-t  <int>:\tThread on demand\n");
  }
  // * dirbase
  printf("\t-d  <path>:\t'%s'\n",   params->dirbase);
  // * shutdown_file
  printf("\t-f  <path>:\t'%s'\n",   params->shutdown_file);
  // * connections
  printf("\t-c  <# accepts per server>:\t%d\n",   params->number_accepts);

  debug_info("[Server=%d] [MPI_SERVER_PARAMS] [mpi_server_params_show] << End\n", params->rank);
}

void mpi_server_params_show_usage ( void )
{
  debug_info("[Server=%d] [MPI_SERVER_PARAMS] [mpi_server_params_show_usage] >> Begin\n", -1) ;

  printf("Usage:\n") ;
  printf("\t-ns <path>:     file for service name\n") ;
  printf("\t-t  <int>:      0 (without thread); 1 (thread pool); 2 (on demand)\n") ;
  printf("\t-d  <string>:   name of the base directory\n") ;
  printf("\t-f  <path>:     file of servers to be shutdown\n") ;
  printf("\t-c  <accepts>:  number of accepts per server\n") ;

  debug_info("[Server=%d] [MPI_SERVER_PARAMS] [mpi_server_params_show_usage] << End\n", -1) ;
}

int mpi_server_params_get ( mpi_server_param_st *params, int argc, char *argv[] )
{
  debug_info("[Server=%d] [MPI_SERVER_PARAMS] [mpi_server_params_get] >> Begin\n", params->rank);

  // set default values
  params->argc = argc;
  params->argv = argv;
  params->size = 0;
  params->rank = 0;
  params->thread_mode = TH_NOT;
  strcpy(params->port_name, "");
  strcpy(params->srv_name,  "");
  strcpy(params->dirbase,   MPI_SERVER_DIRBASE_DEFAULT);
  strcpy(params->dns_file,  MPI_SERVER_DNS_FILE_DEFAULT);
  params->number_accepts = -1;

  // update user requests
  debug_info("[Server=%d] [MPI_SERVER_PARAMS] [mpi_server_params_get] Get user configuration\n", params->rank);

  char *end = NULL ;
  for (int i=0; i<argc; i++)
  {
    switch (argv[i][0])
    {
      case '-':
        switch (argv[i][1])
        {
          case 'n':
            if ((strlen(argv[i]) == 3) && (argv[i][2] == 's'))
            {
              strcpy(params->dns_file, argv[i+1]);
              i++;
            }
            break;

          case 'f':
            strcpy(params->shutdown_file, argv[i+1]);
            i++;
            break;

          case 'd':
            strcpy(params->dirbase, argv[i+1]);
            i++;
            break;

          case 't':
            if ((i+1) < argc)
            {
              if (isdigit(argv[i+1][0]))
              {
                int thread_mode_aux = atoi(argv[i+1]);

                if (thread_mode_aux >= TH_NOT && thread_mode_aux <= TH_OP) {
                  params->thread_mode = thread_mode_aux;
                }
                else {
                  printf("ERROR: unknown option %s\n", argv[i+1]);
                }
              }
              else
              {
                if (strcmp("without", argv[i+1]) == 0) {
                  params->thread_mode = TH_NOT;
                }
                else if (strcmp("pool", argv[i+1]) == 0) {
                  params->thread_mode = TH_POOL;
                }
                else if (strcmp("on_demand", argv[i+1]) == 0) {
                  params->thread_mode = TH_OP;
                }
                else {
                  printf("ERROR: unknown option %s\n", argv[i+1]);
                }
              }
            }
            i++;
            break;

          case 'c':
            params->number_accepts = strtol(argv[i+1], &end, 10) ;
            if (*end != '\0') {
              params->number_accepts = -1 ;
            }
            i++;
            break;

          case 'h':
            return -1;

          default:
            break;
        }
        break;

      default:
        break;
    }
  }

  debug_info("[Server=%d] [MPI_SERVER_PARAMS] [mpi_server_params_get] << End\n", params->rank);

  return 1;
}


/* ................................................................... */

