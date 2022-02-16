
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

  #include "mpiServer_workers.h"


  /* ... Functions / Funciones ......................................... */

     int mpiServer_init_worker ( int thread_mode )
     {
         if (thread_mode == TH_OP || thread_mode == TH_CLI) //REVISAR
         {
           debug_info("[MAIN] mpiServer_worker_ondemand_init\n");
           mpiServer_worker_ondemand_init() ;
         }

         if (thread_mode == TH_POOL)
         {
           debug_info("[MAIN] mpiServer_worker_pool_init\n");
           mpiServer_worker_pool_init ( );
         }
     }

     int mpiServer_launch_worker ( mpiServer_param_st * params, MPI_Comm sd, int type_op, int rank_client_id, void (*worker_function)(struct st_th) )
     {
     }

     void mpiServer_wait_workers ( void )
     {
     }


  /* ................................................................... */
