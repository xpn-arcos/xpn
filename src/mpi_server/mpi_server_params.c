
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

  #include "mpi_server_params.h"


  /* ... Functions / Funciones ......................................... */

  void mpi_server_params_show ( mpi_server_param_st *params )
  {
    DEBUG_BEGIN() ;

    printf(" * MPI server current configuration:\n");
    printf("\t-ns <path>:\t'%s'\n",   params->dns_file) ;
    if(params->thread_mode == TH_POOL){
      printf("\t-tp:\t\tThread Pool Activated\n") ;
    }
    if(params->thread_mode == TH_OP){
      printf("\t-ntp:\t\tThread Pool Deactivated (Using Thread per Client)\n") ;
    }
    printf("\t-d <path>:\t'%s'\n",   params->dirbase) ;
    printf("\t-f <path>:\t'%s'\n",   params->host_file) ;

    DEBUG_END() ;
  }
  

  void mpi_server_params_show_usage ( void )
  {
    DEBUG_BEGIN() ;

    printf("Usage:\n");
    printf("\t-f  <path>: file of servers to be shutdown\n") ;
    printf("\t-ns <path>: file for service name\n") ;
    printf("\t-tp:  use thread-pool\n") ;
    printf("\t-ntp: use thread per-client\n") ;
    printf("\t-d  <string>: name of the base directory\n") ;

    DEBUG_END() ;
  }
  

  int mpi_server_params_get ( mpi_server_param_st *params, int argc, char *argv[] )
  {
    DEBUG_BEGIN() ;

    // set default values
    params->argc = argc ;
    params->argv = argv ;
    params->size = 0 ;
    params->rank = 0 ;
    params->thread_mode = TH_POOL ;
    strcpy(params->port_name, "") ;
    strcpy(params->srv_name,  "") ;
    strcpy(params->dirbase,   MPI_SERVER_DIRBASE_DEFAULT) ;
    strcpy(params->dns_file,  "") ;
  
    // update user requests
    for (int i=0; i<argc; i++)
    {
      switch (argv[i][0])
      {
        case '-':
          switch (argv[i][1])
          {
            case 'n':
              if ((strlen(argv[i]) == 3) && (argv[i][2] == 's')){
                strcpy(params->dns_file, argv[i+1]);
                i++;
              }
              if ((strlen(argv[i]) == 4) && (argv[i][2] == 't') && (argv[i][3] == 'p')){
                params->thread_mode = TH_OP;
                i++;
              }
              break;           
            case 'f':
              strcpy(params->host_file, argv[i+1]);
              i++;
              break;          
            case 'd':
              strcpy(params->dirbase, argv[i+1]);
              i++;
              break;
            case 't':
              if ((strlen(argv[i]) == 3) && (argv[i][2] == 'p')){
                params->thread_mode = TH_POOL;
                i++;
              }
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

    // return OK
    DEBUG_END() ;
    return 1;
  }


  /* ................................................................... */

