
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

     #include "all_system.h"
     #include "base/utils.h"
     #include "mpiServer_params.h"
     #include "mpiServer_ops.h"
     #include "mpiServer_workers.h"
     #include "mpiServer_workers_common.h"
     #include "mpiServer_workers_ondemand.h"
     #include "mpiServer_workers_pool.h"
     #include "mpiServer_comm.h"
     #include "mpiServer_d2xpn.h"


  /* ... Global variables / Variables globales ......................... */

    mpiServer_param_st params;
    int the_end = 0;

  /* ... Auxiliar Functions / Funciones Auxiliares ......................................... */

    void mpiServer_run ( struct st_th th )
    {
      int ret ;
      struct st_mpiServer_msg head ;

      // check params...
      if (NULL == th.params) {
        debug_warning("[WORKERS] (ID=%d): NULL params", th.id) ;
        return;
      }

      int disconnect = 0;
      while (!disconnect)
      {
        ret = mpiServer_comm_read_operation(th.params, th.sd, (char *)&(head.type), 1, &(th.rank_client_id));
        if (ret == -1) {
          debug_info("[OPS] (ID=%s)  mpiServer_comm_readdata fail\n") ;
          return;
        }

        th.type_op = head.type;

        if (th.type_op == MPISERVER_DISCONNECT || th.type_op == MPISERVER_FINALIZE)
        {
          debug_info("DISCONNECT received\n");
          disconnect = 1;
        }

        debug_info("[WORKERS] (ID=%d): begin to do operation '%s' OP_ID %d\n", th.id, mpiServer_op2string(th.type_op), th.type_op);

        mpiServer_do_operation ( &th,  &the_end );

        debug_info("[WORKERS] (ID=%d) end to do operation '%s'\n", th.id, mpiServer_op2string(th.type_op));

      }

      debug_info("[WORKERS] mpiServer_worker_run (ID=%d) close\n", th.rank_client_id);

      //th.params->client = th.sd;

      mpiClient_comm_close(th.sd) ;
    }


  /* ... Functions / Funciones ......................................... */

    int mpiServer_up ( void )
    {
      MPI_Comm sd ;

      // Initialize
      debug_msg_init() ;
      mpiServer_comm_init(&params) ;
      mpiServer_workers_init ( params.thread_mode );
      
      // Loop: receiving + processing
      the_end = 0;
      while (!the_end)
      {
        debug_info("[MAIN] mpiServer_accept_comm()\n") ;

        sd = mpiServer_comm_accept(&params) ;
        if (sd == -1) {
          continue ;
        }

        mpiServer_workers_launch( &params, sd, mpiServer_run ) ;
      }

      // Wait and finalize for all current workers
      debug_info("[WORKERS] mpiServer_workers_destroy\n");
      mpiServer_workers_destroy ( params.thread_mode );
      debug_info("[MAIN] mpiServer_comm_destroy\n");
      mpiServer_comm_destroy(&params) ;

      // return OK 
      return 0 ;
    }

    //TODO: rediseñar
    int mpiServer_down ( int argc, char *argv[] ) {

      int rank, nservers, ret, buf;
      char port_name[MPI_MAX_PORT_NAME];
      char srv_name[1024] ;
      MPI_Comm server;

      MPI_Init(&argc, &argv);
      MPI_Comm_rank(MPI_COMM_WORLD, &rank);

      // Lookup port name
      sprintf(srv_name, "mpiServer.%d", 0) ; //TODO: cambiar
      ret = MPI_Lookup_name(srv_name, MPI_INFO_NULL, port_name) ;
      if (MPI_SUCCESS != ret) {
          printf("MPI_Lookup_name fails\n") ;
          return -1;
      }
      // Connect with servers
      ret = MPI_Comm_connect( port_name, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &server ); //TODO: cambiar a self
      if (MPI_SUCCESS != ret) {
          printf("MPI_Comm_connect fails\n") ;
          return -1;
      }

      //Get number of servers
      MPI_Comm_remote_size(server, &nservers);

      //Finalize all servers
      MPI_Barrier(MPI_COMM_WORLD);

      if (rank == 0)
      {
        for (int i = 0; i < nservers; i++) { 
          buf = MPISERVER_FINALIZE; 
          MPI_Send( &buf, 1, MPI_INT, i, 0, server );
        }
      }

      MPI_Comm_disconnect( &server );
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
      ret = mpiServer_params_get(&params, argc, argv) ;
      if (ret < 0) {
        mpiServer_params_show_usage() ;
        exit(-1) ;
      }

      mpiServer_params_show(&params) ;

      if (params.exec_mode != SERV_UP)
      {
        ret = mpiServer_down (argc, argv);
      }
      else{
        ret = mpiServer_up ();
      }

      return ret;

     }


 /* ................................................................... */

