
/*
 *  Copyright 2020-2022 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
 *
 *  This file is part of mpiServer.
 *
 *  mpiServer is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  mpiServer is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with mpiServer.  If not, see <http://www.gnu.org/licenses/>.
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
     * TCPSERVER_DNS: indica donde se encuentra el sistema traductor de 
     * <id> <hostname> <port>
     */

    #include "tcpServer.h"
    #include "tcpServer_ops.h"
    #include "tcpServer_workers.h"
    #include "tcpServer_comm.h"
    #include "tcpServer_d2xpn.h"
    #include "tcpServer_params.h"


    /* ... Global variables / Variables globales ......................... */

    struct tcpServer_param_st tcpServer_params;

    /* GLOBAL VARIABLES */
    char *TCPSERVER_ALIAS_NAME_STRING;
    char *TCPSERVER_FILE_STRING;
    char *TCPSERVER_DIRBASE_STRING;

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
      int cont;

      // Initializing...
      setbuf(stdout,NULL);  
      setbuf(stderr,NULL);

      signal(SIGINT, sigint_handler);

      // Get parameters..
      if (params_get(argc,argv, &tcpServer_params) == -1)
      {
        params_show_usage();
        exit(-1);
      }
      params_show_values(&tcpServer_params);

      // Initialize tcp_comm...
      tcpServer_comm_init(tcpServer_params.name, tcpServer_params.port, tcpServer_params.file);

      // Initialize tcp_worker...
      tcpServer_init_worker();

      // Request loop...
      cont = 1;
      while (cont)
      {
        debug_info("tcpServer_accept_comm()\n");
        sd = tcpServer_accept_comm();
        if(sd == -1){
          break;
        }

        debug_info("tcpServer_launch_worker()\n");
        tcpServer_launch_worker(sd);
      }

      tcpServer_close_comm();
      xpn_destroy();
      
      return 0;
    }

