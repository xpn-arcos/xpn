
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

  #include "mpi_server_workers.h"


  /* ... Global variables / Variables globales ......................... */

    worker_t worker;


  /* ... Functions / Funciones ......................................... */

    int mpi_server_workers_init ( int thread_mode )
    {
      workers_init ( &worker, thread_mode );


         /*if (thread_mode == TH_OP) //REVISAR
         {
           debug_info("[MAIN] mpi_server_worker_ondemand_init\n");
           mpi_server_worker_ondemand_init() ;
         }

         if (thread_mode == TH_POOL)
         {
           debug_info("[MAIN] mpi_server_worker_pool_init\n");
           mpi_server_worker_pool_init ( );
         }*/

      return 1;
    }

    int mpi_server_workers_launch ( mpi_server_param_st * params, MPI_Comm sd, int type_op, int rank_client_id, void (*worker_function)(struct st_th) )
    {
      struct st_th th_arg;
      th_arg.params = params;
      th_arg.sd = sd;
      th_arg.function = worker_function;
      th_arg.type_op = type_op;
      th_arg.rank_client_id = rank_client_id;

      workers_launch ( &worker, th_arg, worker_function );


      /*if (params->thread_mode == TH_OP)
      {
        debug_info("[MAIN] mpi_server_worker_ondemand_launch (ID=%d)\n", rank_client_id) ;
        mpi_server_worker_ondemand_launch( params, sd, type_op, rank_client_id, worker_function ) ;
      }

      // Enqueue the operation on the buffer
      if (params->thread_mode == TH_POOL)
      {
        debug_info("[MAIN] mpi_server_worker_pool_enqueue (ID=%d)\n", rank_client_id);
        mpi_server_worker_pool_enqueue ( params, sd, type_op, rank_client_id, worker_function );
      }*/

      return 1;
     }

    void mpi_server_workers_destroy ( int thread_mode )
    {
      workers_destroy  ( &worker );

      //thread_mode = thread_mode; // to be removed...

      /*if (thread_mode == TH_OP)
      {
        debug_info("[WORKERS] mpi_server_workers_ondemand_destroy\n");
        mpi_server_workers_ondemand_destroy() ;
      }

      // Destroy worker pool
      if (thread_mode == TH_POOL)
      {
        debug_info("[WORKERS] mpi_server_worker_pool_destroy\n");
        mpi_server_worker_pool_destroy();
      }*/
    }


  /* ................................................................... */
