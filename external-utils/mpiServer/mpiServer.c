
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
     #include "mpiServer_params.h"
     #include "mpiServer_utils.h"
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


  /* ... Functions / Funciones ......................................... */


  void sigint_handler ( int signal )
  {  
       printf("[MAIN] Signal %d received => ending execution...", signal) ;
       the_end = 1;

  // TODO: unblock MPI_Comm_accept :-/
  }

  void mpiServer_run ( struct st_th th )
  {
    // check params...
    if (NULL == th.params) {
      debug_warning("[WORKERS] (ID=%d): NULL params", th.id) ;
      return;
    }

    debug_info("[WORKERS] (ID=%d): begin to do operation '%s' OP_ID %d\n", th.id, mpiServer_op2string(th.type_op), th.type_op);

    mpiServer_do_operation ( &th,  &the_end );

    debug_info("[WORKERS] (ID=%d) end to do operation '%s'\n", th.id, mpiServer_op2string(th.type_op));
   }


   /*
    * Main
    */

   int main ( int argc, char *argv[] )
   {
    MPI_Comm sd ;
    int ret ;
    struct st_mpiServer_msg head ;
    int rank_client_id, disconnect;

    // Get parameters..
    ret = mpiServer_params_get(&params, argc, argv) ;
    if (ret < 0) {
      mpiServer_params_show_usage() ;
      exit(-1) ;
    }

    mpiServer_params_show(&params) ;

    // Initialize
    signal(SIGINT, sigint_handler) ;
    mpiServer_utils_init() ;
    mpiServer_comm_init(&params) ;
    mpiServer_workers_init ( params.thread_mode );
    
    // Loop: receiving + processing
    the_end = 0;
    while (!the_end)
    {
      debug_info("[MAIN] mpiServer_accept_comm()\n") ;
      sd = mpiServer_comm_accept(&params) ; //Accept por aplicacion
      if (sd == -1) {
        continue ;
      }

      disconnect = 0;
      while (!disconnect)
      {
        ret = mpiServer_comm_read_operation(&params, sd, (char *)&head.type, sizeof(head.type), &rank_client_id);
        if (ret == -1) {
          debug_info("[OPS] (ID=%s)  mpiServer_comm_readdata fail\n") ;
          return -1;
        }

        if (head.type == MPISERVER_DISCONNECT || head.type == MPISERVER_FINALIZE)
        {
          debug_info("DISCONNECT received\n");
          disconnect = 1;
        }

        // Launch worker to execute the operation
        mpiServer_workers_launch( &params, sd, head.type, rank_client_id, mpiServer_run ) ;
      }
      
      if (head.type == MPISERVER_FINALIZE) {
        the_end = 1;
      }

      debug_info("[WORKERS] mpiServer_worker_run (ID=%d) close\n", rank_client_id);
      mpiServer_comm_close(&params) ;
   
    }

    // Wait and finalize for all current workers
    debug_info("[WORKERS] mpiServer_workers_destroy\n");
    mpiServer_workers_destroy ( params.thread_mode );
    debug_info("[MAIN] mpiServer_comm_destroy\n");
    mpiServer_comm_destroy(&params) ;

    // return OK 
    return 0 ;
   }


 /* ................................................................... */

