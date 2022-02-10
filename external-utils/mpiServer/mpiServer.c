
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
    #define MAX_OPERATIONS 256
    struct st_th operations_buffer[MAX_OPERATIONS]; // buffer
    int n_operation = 0;
    int position = 0;




    int id_all = 0;




    pthread_mutex_t mutex;
    pthread_cond_t no_full;
    pthread_cond_t no_empty;
    pthread_mutex_t mend;






    

    int the_end = 0;
    mpiServer_param_st params;


    /* ... Functions / Funciones ......................................... */

    void sigint_handler ( int signal )
    {  
      printf("[MAIN] Signal %d received => ending execution...", signal) ;
      the_end = 1;
    }


















    //void worker_function ( struct st_th th )
    void worker_function ( void )
    {
      struct st_th th;
      //int op;
      struct st_mpiServer_msg head;
      int ret;


      int id;

      //int rank_client_id;

      while(1){

        pthread_mutex_lock(&mutex);
        while (n_operation == 0) {
          if (the_end==1) {
            pthread_mutex_unlock(&mutex);
            pthread_exit(0);
          }
          pthread_cond_wait(&no_empty, &mutex);
        }
      

        id = id_all;
        id_all++;

        th = operations_buffer[position];
        position = (position + 1) % MAX_OPERATIONS;
        n_operation--;

        pthread_cond_signal(&no_full);
        pthread_mutex_unlock(&mutex);



        printf("%d\n", id);


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
        
      }

      pthread_exit(0);

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




      //mpiServer_init_worker() ;
      






      //Threads pool
      pthread_attr_t t_attr;
      pthread_t thid[MAX_THREADS];
      int pos = 0;




      pthread_mutex_init(&mutex,NULL);
      pthread_cond_init(&no_full,NULL);
      pthread_cond_init(&no_empty,NULL);
      pthread_mutex_init(&mend,NULL);

      pthread_attr_init(&t_attr);
      for (int i = 0; i < MAX_THREADS; i++){
        if (pthread_create(&thid[i], NULL, (void *)(worker_function), NULL) !=0){
          perror("Error creating thread pool\n");
          return -1;
        }
      }

      //Threads pool End









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





          //Pool
          pthread_mutex_lock(&mutex);
          while (n_operation == MAX_OPERATIONS){
            pthread_cond_wait(&no_full, &mutex);
          }

          struct st_th st_worker;

          st_worker.sd                = sd;
          st_worker.params            = &params ;
          st_worker.type_op           = head.type ;
          st_worker.rank_client_id    = rank_client_id ;

          operations_buffer[pos] = st_worker;
          pos = (pos+1) % MAX_OPERATIONS;
          n_operation++;
          pthread_cond_signal(&no_empty);
          pthread_mutex_unlock(&mutex);

          //Pool end



          //debug_info("[MAIN] mpiServer_launch_worker()\n") ;
          //mpiServer_launch_worker(&params, sd, head.type, rank_client_id, worker_function) ;

        }

        if (head.type == MPISERVER_FINALIZE)
        {
          break;
        }

        debug_info("[WORKERS] mpiServer_worker_run (ID=%d) close\n", rank_client_id);
        mpiServer_comm_close(&params) ;
     
      }

      // Wait for all current workers
      //mpiServer_wait_workers() ; //TODO ???

      // Finalize
      //mpiServer_comm_destroy(&params) ;


      //Pool
      pthread_mutex_lock(&mend);
      the_end=1;
      pthread_mutex_unlock(&mend);

      pthread_mutex_lock(&mutex);
      pthread_cond_broadcast(&no_empty);
      pthread_mutex_unlock(&mutex);


      for (int i=0;i<MAX_THREADS;i++){
        pthread_join(thid[i],NULL);
      }

      pthread_mutex_destroy(&mutex);
      pthread_cond_destroy(&no_full);
      pthread_cond_destroy(&no_empty);
      pthread_mutex_destroy(&mend);
      //Pool end

      // return OK 
      return 0 ;
    }


   /* ................................................................... */

