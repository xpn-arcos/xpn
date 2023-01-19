
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

    #include "all_system.h"
    #include "base/utils.h"
    #include "mpi_server_params.h"
    #include "mpi_server_ops.h"
    #include "base/workers.h"
    #include "mpi_server_comm.h"
    #include "mpi_server_d2xpn.h"


  /* ... Global variables / Variables globales ......................... */

    mpi_server_param_st params;
    worker_t worker;
    int the_end = 0;

  /* ... Auxiliar Functions / Funciones Auxiliares ......................................... */

    void mpi_server_run ( struct st_th th )
    {
      debug_info("[WORKERS] (ID=%d): begin to do operation '%s' OP_ID %d\n", th.id, mpi_server_op2string(th.type_op), th.type_op);

      mpi_server_do_operation ( &th,  &the_end );

      debug_info("[WORKERS] (ID=%d) end to do operation '%s'\n", th.id, mpi_server_op2string(th.type_op));
    }

    void mpi_server_dispatcher ( struct st_th th )
    {
      int ret ;
      
      // check params...
      if (NULL == th.params) {
        printf("[WORKERS] (ID=%d): NULL params", th.id) ;
        return;
      }

      int disconnect = 0;
      while (!disconnect)
      {
        ret = mpi_server_comm_read_operation(th.params, th.sd, (char *)&(th.type_op), 1, &(th.rank_client_id));
        if (ret == -1) {
          debug_info("[OPS] (ID=%s) mpi_server_comm_readdata fail\n") ;
          return;
        }

        if (th.type_op == MPI_SERVER_DISCONNECT || th.type_op == MPI_SERVER_FINALIZE)
        {
          debug_info("DISCONNECT received\n");
          disconnect = 1;
        }
        else{
          //Launch worker per operation
          struct st_th th_arg;
          th_arg.params = &params;
          th_arg.sd = th.sd;
          th_arg.function = mpi_server_run;
          th_arg.type_op = th.type_op;
          th_arg.rank_client_id = th.rank_client_id;

          workers_launch ( &worker, th_arg, mpi_server_run );
        }
      }

      debug_info("[WORKERS] mpi_server_worker_run (ID=%d) close\n", th.rank_client_id);

      mpiClient_comm_close(th.sd) ;
    }


  /* ... Functions / Funciones ......................................... */

    int mpi_server_up ( void )
    {
      MPI_Comm sd ;
      struct st_mpi_server_msg head ;
      int rank_client_id;
      int ret;


      // Initialize
      debug_msg_init() ;
      mpi_server_comm_init(&params) ;
      workers_init ( &worker, params.thread_mode );


      //Initialize semaphore for server disks
      ret = sem_init(&(params.disk_sem), 0, 1);
      if (ret == -1)
      {
        return -1; //TODO: message
      }

      //Initialize semaphore for clients
      char serv_name [HOST_NAME_MAX];
      gethostname(serv_name, HOST_NAME_MAX);
      sprintf(params.sem_name_server, "%s%d", serv_name, getpid());
      
      sem_t *sem_server = sem_open(params.sem_name_server, O_CREAT, 0777, 1);
      if (sem_server == 0)
      {
        printf("Semaphore error\n");
        return -1;
      }

      // Loop: receiving + processing
      the_end = 0;
      while (!the_end)
      {
        debug_info("[MAIN] mpi_server_accept_comm()\n") ;

        params.client = MPI_COMM_NULL ;

        sd = mpi_server_comm_accept(&params) ;
        if (sd == MPI_COMM_NULL) {
          continue ;
        }

        ret = mpi_server_comm_read_operation(&params, sd, (char *)&(head.type), 1, &(rank_client_id));
        if (ret == -1) {
          debug_info("[OPS] (ID=%s)  mpi_server_comm_readdata fail\n") ;
          return -1;
        }

        if (head.type == MPI_SERVER_FINALIZE)
        {
          the_end = 1;
          continue;
        }
        else{
          //Launch dispatcher per aplication
          struct st_th th_arg;
          th_arg.params = &params;
          th_arg.sd = sd;
          th_arg.function = mpi_server_dispatcher;
          th_arg.type_op = 0;
          th_arg.rank_client_id = 0;

          workers_launch ( &worker, th_arg, mpi_server_dispatcher );
        }
      }

      // Wait and finalize for all current workers
      debug_info("[WORKERS] workers_destroy\n");
      workers_destroy  ( &worker );
      debug_info("[MAIN] mpi_server_comm_destroy\n");
      mpi_server_comm_destroy(&params) ;

      //Close semaphores
      sem_destroy(&(params.disk_sem));
      sem_unlink(params.sem_name_server);

      // return OK 
      return 0 ;
    }

    int mpi_server_down ( int argc, char *argv[] )
    {
      int  ret, buf;
      char port_name[MPI_MAX_PORT_NAME];
      char srv_name[1024] ;
      char dns_name[2048] ;
      MPI_Comm server;
      FILE *file;

      printf("----------------\n");
      printf("Stopping servers\n");
      printf("----------------\n\n");

      MPI_Init(&argc, &argv);

      // open file
      file = fopen(params.host_file, "r");
      if (file == NULL) {
        printf("Invalid file %s\n", params.host_file);
        return -1;
      }

      while (fscanf(file, "%[^\n] ", srv_name) != EOF) {
        // Lookup port name
        ret = ns_lookup (srv_name, port_name);
        if (ret == -1)
        {
          printf("Server %s not found\n", dns_name) ;
          continue;
        }

        // Connect with servers
        ret = MPI_Comm_connect( port_name, MPI_INFO_NULL, 0, MPI_COMM_SELF, &server );
        if (MPI_SUCCESS != ret) {
          printf("MPI_Comm_connect fails\n") ;
          continue;
        }
        buf = MPI_SERVER_FINALIZE; 
        MPI_Send( &buf, 1, MPI_INT, 0, 0, server );

        //MPI_Comm_disconnect( &server ); //TODO: fail
      }

      fclose(file);

      MPI_Finalize();

      return 0;
    }

  

    /*
     * Main
     */

    int main ( int argc, char *argv[] )
    {

      int ret ;

      // Get parameters..
      ret = mpi_server_params_get(&params, argc, argv) ;
      if (ret < 0) {
        mpi_server_params_show_usage() ;
        return -1;
      }

      char * exec_name = basename(argv[0]);
      printf("%s\n", exec_name);
      
      if (strcasecmp(exec_name, "xpn_stop_mpi_server") == 0)
      {
        ret = mpi_server_down (argc, argv);
      }
      else{
        ret = mpi_server_up ();
      }

      return ret;
     }


 /* ................................................................... */

