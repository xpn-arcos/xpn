
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
  #include "mpiServer_comm.h"
  #include "mpiServer_d2xpn.h"


  /* ... Global variables / Variables globales ......................... */

  mpiServer_param_st params;


  /* ... Functions / Funciones ......................................... */

  void sigint_handler ( int signal )
  {  
    printf("[MAIN] Signal %d received => ending execution...", signal) ;
    //the_end = 1; //TODO: enviar a los threads
  }

  /*
   * Main
   */

  int main ( int argc, char *argv[] )
  {
    MPI_Comm sd ;
    int ret ;
    struct st_mpiServer_msg head ;


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




    // Initialize worker
    if (params.thread_mode == TH_OP || params.thread_mode == TH_CLI) //REVISAR
    {
      debug_info("[MAIN] mpiServer_worker_ondemand_init\n");
      mpiServer_worker_ondemand_init() ;
    }
    if (params.thread_mode == TH_POOL)
    {
      debug_info("[MAIN] mpiServer_worker_pool_init\n");
      mpiServer_worker_pool_init ( );
    }

    
    // Loop: receiving + processing
    while (1)
    {
      debug_info("[MAIN] mpiServer_accept_comm()\n") ;
      sd = mpiServer_comm_accept(&params) ; //Accept por aplicacion
      if (sd == -1) {
        continue ;
      }

      int rank_client_id;




      if (params.thread_mode == TH_CLI)
      {
        printf("CLIENT MODE\n");

        int ret, n_clients; 

        ret = MPI_Comm_remote_size(sd, &n_clients) ;
        if (MPI_SUCCESS != ret) {
            debug_error("Server[%d]: MPI_Comm_size fails :-(", params.rank) ;
            return -1 ;
        }

        for (int i = 0; i < n_clients; ++i)
        {
          mpiServer_worker_ondemand_launch_client(&params, sd, head.type, rank_client_id, worker_client_function);
        }
      }






      else{
        printf("OTHERS MODE\n");
        while (1){
          ret = mpiServer_comm_read_operation(&params, sd, (char *)&head.type, sizeof(head.type), &rank_client_id);
          if (ret == -1) {
            debug_info("[OPS] (ID=%s)  mpiServer_comm_readdata fail\n") ;
            return -1;
          }

          //TODO ver si es desconectar o finalizar y hacer un break aqui
          if (head.type == MPISERVER_DISCONNECT || head.type == MPISERVER_FINALIZE)
          {
            printf("Ha llegado un DISCONNECT\n");
            break;
          }

          // Launch worker to execute the operation
          if (params.thread_mode == TH_OP)
          {
            debug_info("[MAIN] mpiServer_worker_ondemand_launch (ID=%d)\n", rank_client_id) ;
            mpiServer_worker_ondemand_launch(&params, sd, head.type, rank_client_id, worker_function) ;
          }

          // Enqueue the operation on the buffer
          if (params.thread_mode == TH_POOL)
          {
            debug_info("[MAIN] mpiServer_worker_pool_enqueue (ID=%d)\n", rank_client_id);
            mpiServer_worker_pool_enqueue ( sd, &params, head.type, rank_client_id );
          }
        }
      }



      if (head.type == MPISERVER_FINALIZE)
      {
        break;
      }

      debug_info("[WORKERS] mpiServer_worker_run (ID=%d) close\n", rank_client_id);
      mpiServer_comm_close(&params) ;
   
    }

    // Wait and finalize for all current workers
    if (params.thread_mode == TH_OP)
    {
      debug_info("[WORKERS] mpiServer_workers_ondemand_wait\n");
      mpiServer_workers_ondemand_wait() ; //TODO ???
      debug_info("[WORKERS] mpiServer_comm_destroy\n");
      mpiServer_comm_destroy(&params) ;
    }

    // Destroy worker pool
    if (params.thread_mode == TH_POOL)
    {
      debug_info("[WORKERS] mpiServer_worker_pool_destroy\n");
      mpiServer_worker_pool_destroy();
    }

    // return OK 
    return 0 ;
  }


 /* ................................................................... */

