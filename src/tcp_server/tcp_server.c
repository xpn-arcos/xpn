
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

    /* defines usados
     * _LARGEFILE64_: para soporte de ficheros mayores de 4GB
     * _MPI_: para lanzar los servidores como procesos MPI
     * DBG_MAIN: imprimo depuracion
     * _LOG_: imprimo errores
     * _COMPRESS_: uso del sistema de compresion lzf
     */

    /* VARIABLES DE ENTORNO:
     * TCP_SERVER_DNS: indica donde se encuentra el sistema traductor de 
     * <id> <hostname> <port>
     */

    #include "tcp_server.h"
    #include "tcp_server_ops.h"
    #include "tcp_server_workers.h"
    #include "tcp_server_comm.h"
    #include "tcp_server_d2xpn.h"
    #include "tcp_server_params.h"


    /* ... Global variables / Variables globales ......................... */

    struct tcp_server_param_st tcp_server_params;

    /* GLOBAL VARIABLES */
    char *TCP_SERVER_ALIAS_NAME_STRING;
    char *TCP_SERVER_FILE_STRING;
    char *TCP_SERVER_DIRBASE_STRING;

    /* INTERNAL CONST & STRUCTS */
    extern int errno;
    //pthread_t th;


    /* ... Functions / Funciones ......................................... */

    void sigint_handler ( int signal )
    {
     printf("Signal %d received !!", signal) ;
     exit(0) ;
    }


    int main(int argc, char *argv[])
    {
      int sd;
      int id = 0;
      //int cont;

      // Initializing...
      setbuf(stdout,NULL);  
      setbuf(stderr,NULL);

      signal(SIGINT, sigint_handler);

      // Get parameters..
      if (params_get(argc,argv, &tcp_server_params) == -1)
      {
        params_show_usage();
        exit(-1);
      }
      params_show_values(&tcp_server_params);

      // Initialize tcp_comm...
      tcp_server_comm_init(tcp_server_params.name, tcp_server_params.port, tcp_server_params.file);

      // Initialize tcp_worker...
      if (tcp_server_params.thread_mode == TH_OP)
      {
        debug_info("[MAIN] tcp_server_init_worker\n");
        tcp_server_init_worker();
      }

      // Initialize and launch worker pool
      if (tcp_server_params.thread_mode == TH_POOL)
      {
        debug_info("[MAIN] tcp_server_init_worker_pool\n");
        tcp_server_init_worker_pool ( );
        debug_info("[MAIN] tcp_server_launch_worker_pool launch\n");
        tcp_server_launch_worker_pool(worker_pool_function);
      }




      // Request loop...
      //cont = 1;
      while (1)
      {
        debug_info("tcp_server_accept_comm()\n");
        sd = tcp_server_accept_comm();
        if(sd == -1){
          break;
        }

        // Launch worker to execute the operation
        if (tcp_server_params.thread_mode == TH_OP)
        {
          debug_info("[MAIN] tcp_server_launch_worker\n") ;
          tcp_server_launch_worker(sd);
        }

        // Enqueue the operation on the buffer
        if (tcp_server_params.thread_mode == TH_POOL)
        {
          debug_info("[MAIN] tcp_server_worker_pool_enqueue\n");
          tcp_server_worker_pool_enqueue ( sd, id );
          id++;
        }

      }

      // Destroy worker pool
      if (tcp_server_params.thread_mode == TH_POOL)
      {
        debug_info("[WORKERS] tcp_server_destroy_worker_pool\n");
        tcp_server_destroy_worker_pool();
      }

      tcp_server_close_comm();
      //xpn_destroy();

      //TODO: los otros hilos no se destruyen??

      

      return 0;
    }

