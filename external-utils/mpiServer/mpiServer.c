
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


    // Pool
    /*#define MAX_OPERATIONS 256
    struct st_th operations_buffer[MAX_OPERATIONS]; // buffer
    int n_operation = 0;
    int position = 0;*/




    //int id_all = 0;




    /*pthread_mutex_t m_pool;
    pthread_cond_t c_pool_no_full;
    pthread_cond_t C_poll_no_empty;
    pthread_mutex_t m_pool_end;*/

    /* //pthread_attr_t t_attr;
    pthread_t thid[MAX_THREADS];*/
    


    

    int the_end = 0;
    mpiServer_param_st params;


    /* ... Functions / Funciones ......................................... */

    void sigint_handler ( int signal )
    {  
      printf("[MAIN] Signal %d received => ending execution...", signal) ;
      the_end = 1;
    }


    void worker_pool_function ( void )
    {
      struct st_th th;

      while(1)
      {
        // Dequeue operation
        if (params.thread_mode == TH_POOL)
        {
          th = mpiServer_worker_pool_dequeue ( the_end );
        }

        worker_function ( th );
        
      }

      pthread_exit(0);
    }


    void worker_function ( struct st_th th )
    {
      //int op;
      struct st_mpiServer_msg head;
      int ret;
      //int rank_client_id;
      
      // check params...
      if (NULL == th.params) {
          debug_warning("[WORKERS] (ID=%d): NULL params", th.id) ;
          return;
      }

      //head.type = MPISERVER_END;

      debug_info("[WORKERS] (ID=%d): mpiServer_read_operation and arguments...\n", th.id);
      //op = mpiServer_read_operation(th.params, th.sd, &head, &rank_client_id);
        
      debug_info("[WORKERS] (ID=%d): begin to do operation '%s' OP_ID %d\n", th.id, mpiServer_op2string(th.type_op), th.type_op);

      switch(th.type_op)
      {
        //Connection API
        case MPISERVER_DISCONNECT:
          break;
        case MPISERVER_FINALIZE:
          the_end = 1;
          break;


        //File API
        case MPISERVER_OPEN_FILE:
          ret = mpiServer_comm_read_data(th.params, th.sd, (char *)&head.u_st_mpiServer_msg.op_open, sizeof(struct st_mpiServer_open), th.rank_client_id);
          if (ret == -1) {
            //return -1;
            return;
          }
          mpiServer_op_open(th.params, th.sd, &head, th.rank_client_id);
          break;
        case MPISERVER_CREAT_FILE:

          ret = mpiServer_comm_read_data(th.params, th.sd, (char *)&head.u_st_mpiServer_msg.op_creat, sizeof(struct st_mpiServer_creat), th.rank_client_id);
          if (ret == -1) {
            //return -1;
            return;
          }
          mpiServer_op_creat(th.params, th.sd, &head, th.rank_client_id);
          break;
        case MPISERVER_READ_FILE:
          ret = mpiServer_comm_read_data(th.params, th.sd, (char *)&head.u_st_mpiServer_msg.op_read, sizeof(struct st_mpiServer_read), th.rank_client_id);
          if (ret == -1) {
             //return -1;
            return;
          }
          mpiServer_op_read(th.params, th.sd, &head, th.rank_client_id);
          break;
        case MPISERVER_WRITE_FILE:
          ret = mpiServer_comm_read_data(th.params, th.sd, (char *)&head.u_st_mpiServer_msg.op_write, sizeof(struct st_mpiServer_write), th.rank_client_id);
          if (ret == -1) {
            //return -1;
            return;
          }
          mpiServer_op_write(th.params, th.sd, &head, th.rank_client_id);
          break;
        case MPISERVER_CLOSE_FILE:
          ret = mpiServer_comm_read_data(th.params, th.sd, (char *)&head.u_st_mpiServer_msg.op_close, sizeof(struct st_mpiServer_close), th.rank_client_id);
          if (ret == -1) {
            //return -1;
            return;
          }
          mpiServer_op_close(th.params, th.sd, &head, th.rank_client_id);
          break;
        case MPISERVER_RM_FILE:
          ret = mpiServer_comm_read_data(th.params, th.sd, (char *)&head.u_st_mpiServer_msg.op_rm, sizeof(struct st_mpiServer_rm), th.rank_client_id);
          if (ret == -1) {
            //return -1;
            return;
          }
          mpiServer_op_rm(th.params, th.sd, &head, th.rank_client_id);
          break;
        case MPISERVER_GETATTR_FILE:
          ret = mpiServer_comm_read_data(th.params, th.sd, (char *)&head.u_st_mpiServer_msg.op_getattr, sizeof(struct st_mpiServer_getattr), th.rank_client_id);
          if (ret == -1) {
            //return -1;
            return;
          }
          mpiServer_op_getattr(th.params, th.sd, &head, th.rank_client_id);
          break;
        case MPISERVER_SETATTR_FILE:
          ret = mpiServer_comm_read_data(th.params, th.sd, (char *)&head.u_st_mpiServer_msg.op_setattr, sizeof(struct st_mpiServer_setattr), th.rank_client_id);
          if (ret == -1) {
            //return -1;
            return;
          }
          mpiServer_op_setattr(th.params, th.sd, &head, th.rank_client_id);
          break;

        //Directory API  
        case MPISERVER_MKDIR_DIR:
          ret = mpiServer_comm_read_data(th.params, th.sd, (char *)&head.u_st_mpiServer_msg.op_mkdir, sizeof(struct st_mpiServer_mkdir), th.rank_client_id);
          if (ret == -1) {
            //return -1;
            return;
          }
          mpiServer_op_mkdir(th.params, th.sd, &head, th.rank_client_id);
          break;
        case MPISERVER_RMDIR_DIR:
          ret = mpiServer_comm_read_data(th.params, th.sd, (char *)&head.u_st_mpiServer_msg.op_rmdir, sizeof(struct st_mpiServer_rmdir), th.rank_client_id);
          if (ret == -1) {
            //return -1;
            return;
          }
          mpiServer_op_rmdir(th.params, th.sd, &head, th.rank_client_id);
          break;

        //Optimization API
        case MPISERVER_PRELOAD_FILE:
          ret = mpiServer_comm_read_data(th.params, th.sd, (char *)&head.u_st_mpiServer_msg.op_preload, sizeof(struct st_mpiServer_preload), th.rank_client_id);
          if (ret == -1) {
            //return -1;
            return;
          }
          mpiServer_op_preload(th.params, th.sd, &head, th.rank_client_id);
          break;
        case MPISERVER_FLUSH_FILE:
          ret = mpiServer_comm_read_data(th.params, th.sd, (char *)&head.u_st_mpiServer_msg.op_flush, sizeof(struct st_mpiServer_flush), th.rank_client_id);
          if (ret == -1) {
            //return -1;
            return;
          }
          mpiServer_op_flush(th.params, th.sd, &head, th.rank_client_id);
          break;
      }
      
      debug_info("[WORKERS] (ID=%d) end to do operation '%s'\n", th.id, mpiServer_op2string(th.type_op));
        
    
      //debug_info("[WORKERS] mpiServer_worker_run (ID=%d) close\n", th.id);
      //mpiServer_comm_close(th.params) ;
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
      if (params.thread_mode == TH_OP)
      {
        debug_info("[MAIN] mpiServer_init_worker\n");
        mpiServer_init_worker() ;
      }

      // Initialize and launch worker pool
      if (params.thread_mode == TH_POOL)
      {
        debug_info("[MAIN] mpiServer_init_worker_pool\n");
        mpiServer_init_worker_pool ( );
        debug_info("[MAIN] mpiServer_launch_worker_pool launch\n");
        mpiServer_launch_worker_pool(worker_pool_function);
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
            debug_info("[MAIN] mpiServer_launch_worker (ID=%d)\n", rank_client_id) ;
            mpiServer_launch_worker(&params, sd, head.type, rank_client_id, worker_function) ;
          }

          // Enqueue the operation on the buffer
          if (params.thread_mode == TH_POOL)
          {
            debug_info("[MAIN] mpiServer_worker_pool_enqueue (ID=%d)\n", rank_client_id);
            mpiServer_worker_pool_enqueue ( sd, &params, head.type, rank_client_id );
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
        debug_info("[WORKERS] mpiServer_wait_workers\n");
        mpiServer_wait_workers() ; //TODO ???
        debug_info("[WORKERS] mpiServer_comm_destroy\n");
        mpiServer_comm_destroy(&params) ;
      }

      // Destroy worker pool
      if (params.thread_mode == TH_POOL)
      {
        debug_info("[WORKERS] mpiServer_destroy_worker_pool\n");
        mpiServer_destroy_worker_pool();
      }

      // return OK 
      return 0 ;
    }


   /* ................................................................... */

