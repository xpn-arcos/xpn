
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


  /* ... Global variables / Variables globales ......................... */

  /*Worker variables*/

  int             busy_worker;
  pthread_mutex_t m_worker;
  pthread_cond_t  c_worker;

  pthread_cond_t  c_nworkers;
  long            n_workers;
  int             th_cont = 0;

  /*Worker pool variables*/

  pthread_mutex_t m_pool;
  pthread_cond_t c_pool_no_full;
  pthread_cond_t c_poll_no_empty;
  pthread_mutex_t m_pool_end;

  pthread_t thid[MAX_THREADS];

  struct st_th operations_buffer[MAX_OPERATIONS]; // buffer
  int n_operation = 0;
  int deq_pos = 0;
  int enq_pos = 0;
  int pool_end = 0;

  int the_end = 0;

  /**************************/



  /* ... Functions / Funciones ......................................... */

  int mpiServer_init_worker ( void )
  {
    DEBUG_BEGIN() ;

    busy_worker = TRUE;
    n_workers   = 0L;

    pthread_cond_init (&c_worker,   NULL);
    pthread_cond_init (&c_nworkers, NULL);
    pthread_mutex_init(&m_worker,   NULL);

    DEBUG_END() ;

    return 0;
  }

  int mpiServer_init_worker_pool ( void )
  {
    DEBUG_BEGIN() ;

    pthread_mutex_init(&m_pool,NULL);
    pthread_cond_init (&c_pool_no_full,NULL);
    pthread_cond_init (&c_poll_no_empty,NULL);
    pthread_mutex_init(&m_pool_end,NULL);

    DEBUG_END() ;

    return 0;
  }



  int mpiServer_launch_worker ( mpiServer_param_st * params, MPI_Comm sd, int type_op, int rank_client_id, void (*worker_function)(struct st_th) )
  {
    int ret;
    pthread_attr_t th_attr;
    pthread_t      th_worker;
    struct st_th   st_worker;

    DEBUG_BEGIN() ;

    pthread_attr_init(&th_attr);
    pthread_attr_setdetachstate(&th_attr, PTHREAD_CREATE_DETACHED);
    pthread_attr_setstacksize  (&th_attr, STACK_SIZE);
    busy_worker = TRUE;

    // prepare arguments...
    st_worker.sd              = sd;
    st_worker.id              = th_cont++;
    st_worker.params          = params ;
    st_worker.type_op         = type_op ;
    st_worker.rank_client_id  = rank_client_id ;
    st_worker.function        = worker_function ;

    // create thread...
    debug_info("[WORKERS] pthread_create: create_thread mpiServer_worker_run\n") ;
    ret = pthread_create(&th_worker, &th_attr, (void *)(mpiServer_worker_run), (void *)&st_worker);
    if (ret != 0){
      debug_error("[WORKERS] pthread_create %d\n", ret);
      perror("pthread_create: Error en create_thread: ");
    }

    // wait to copy args...
    debug_info("[WORKERS] pthread_create: lock mpiServer_worker_run\n");
    pthread_mutex_lock(&m_worker);

    while (busy_worker == TRUE)
    {
      debug_info("[WORKERS] pthread_create: wait mpiServer_worker_run\n");
      pthread_cond_wait(&c_worker, &m_worker);
    }

    debug_info("[WORKERS] pthread_create: busy_worker= TRUE mpiServer_worker_run\n");
    busy_worker = TRUE;
    debug_info("[WORKERS] pthread_create: unlock mpiServer_worker_run\n");
    pthread_mutex_unlock(&m_worker);

    DEBUG_END() ;
    return 0;
  }

  int mpiServer_launch_worker_pool ( void (*worker_pool_function)(void) )
  {
    DEBUG_BEGIN() ;

    for (int i = 0; i < MAX_THREADS; i++){
      debug_info("[WORKERS] pthread_create: create_thread mpiServer_launch_worker_pool\n") ;
      if (pthread_create(&thid[i], NULL, (void *)(worker_pool_function), NULL) !=0){
        perror("Error creating thread pool\n");
        return -1;
      }
    }

    DEBUG_END() ;

    return 0;
  }



  void mpiServer_worker_run ( void *arg )
  {
      struct st_th th;
  
      DEBUG_BEGIN() ;

      // prolog... 
      debug_info("[WORKERS] client(%d): mpiServer_worker_run(...) lock\n", th.id);
      pthread_mutex_lock(&m_worker);
      debug_info("[WORKERS] client(%d): mpiServer_worker_run(...) copy arguments\n", th.id);
      memcpy(&th, arg, sizeof(struct st_th)) ;
      debug_info("[WORKERS] client(%d): mpiServer_worker_run(...) busy_worker = FALSE\n", th.id);
      busy_worker = FALSE;
      debug_info("[WORKERS] client(%d): mpiServer_worker_run(...) n_workers++\n", th.id);
      n_workers++ ;
      debug_info("[WORKERS] client(%d): mpiServer_worker_run(...) signal c_worker\n", th.id);
      pthread_cond_broadcast(&c_worker); // pthread_cond_signal(&c_worker);
      debug_info("[WORKERS] client(%d): mpiServer_worker_run(...) unlock\n", th.id);
      pthread_mutex_unlock(&m_worker);
  
      // do function code...
      th.function(th) ;

      // epilog...
      debug_info("[WORKERS] client(%d): mpiServer_worker_run(...) lock\n", th.id);
      pthread_mutex_lock(&m_worker);
      debug_info("[WORKERS] client(%d): mpiServer_worker_run(...) n_workers--\n", th.id);
      n_workers-- ;
      debug_info("[WORKERS] client(%d): mpiServer_worker_run(...) signal c_nworkers\n", th.id);
      pthread_cond_broadcast(&c_nworkers); // pthread_cond_signal(&c_nworkers);
      debug_info("[WORKERS] client(%d): mpiServer_worker_run(...) unlock\n", th.id);
      pthread_mutex_unlock(&m_worker);

      DEBUG_END() ;

      // end
      pthread_exit(0);
  }



  void mpiServer_worker_pool_enqueue ( MPI_Comm sd, mpiServer_param_st *params, int op_type, int rank_client_id)
  {
    DEBUG_BEGIN() ;

    debug_info("[WORKERS] client(%d): mpiServer_worker_pool_enqueue(...) lock\n", rank_client_id);
    pthread_mutex_lock(&m_pool);
    while (n_operation == MAX_OPERATIONS){
      debug_info("[WORKERS] client(%d): mpiServer_worker_pool_enqueue(...) wait c_pool_no_full\n", rank_client_id);
      pthread_cond_wait(&c_pool_no_full, &m_pool);
    }

    debug_info("[WORKERS] client(%d): mpiServer_worker_pool_enqueue(...) copy arguments\n", rank_client_id);
    struct st_th st_worker;

    st_worker.sd                = sd;
    st_worker.params            = params ;
    st_worker.type_op           = op_type ;
    st_worker.rank_client_id    = rank_client_id ;

    debug_info("[WORKERS] client(%d): mpiServer_worker_pool_enqueue(...) enqueue\n", rank_client_id);
    operations_buffer[enq_pos] = st_worker;
    enq_pos = (enq_pos+1) % MAX_OPERATIONS;
    n_operation++;

    debug_info("[WORKERS] client(%d): mpiServer_worker_pool_enqueue(...) signal c_poll_no_empty\n", rank_client_id);
    pthread_cond_signal(&c_poll_no_empty);
    debug_info("[WORKERS] client(%d): mpiServer_worker_pool_enqueue(...) unlock\n", rank_client_id);
    pthread_mutex_unlock(&m_pool);

    DEBUG_END() ;
  }

  struct st_th mpiServer_worker_pool_dequeue ( int the_end )
  {
    DEBUG_BEGIN() ;

    struct st_th th;

    debug_info("[WORKERS] client(%d): mpiServer_worker_pool_dequeue(...) lock\n", th.id);
    pthread_mutex_lock(&m_pool);

    while (n_operation == 0) {
      if ( pool_end==1 || the_end==1 ) {
        debug_info("[WORKERS] client(%d): mpiServer_worker_pool_dequeue(...) unlock end\n", th.id);
        pthread_mutex_unlock(&m_pool);
        debug_info("[WORKERS] client(%d): mpiServer_worker_pool_dequeue(...) exit\n", th.id);
        pthread_exit(0);
      }
      debug_info("[WORKERS] client(%d): mpiServer_worker_pool_dequeue(...) wait c_poll_no_empty\n", th.id);
      pthread_cond_wait(&c_poll_no_empty, &m_pool);
    }

    debug_info("[WORKERS] thread id = %ld\n", pthread_self());

    debug_info("[WORKERS] client(%d): mpiServer_worker_pool_dequeue(...) dequeue\n", th.id);
    th = operations_buffer[deq_pos];
    deq_pos = (deq_pos + 1) % MAX_OPERATIONS;
    n_operation--;

    debug_info("[WORKERS] client(%d): mpiServer_worker_pool_dequeue(...) signal c_pool_no_full\n", th.id);
    pthread_cond_signal(&c_pool_no_full);

    debug_info("[WORKERS] client(%d): mpiServer_worker_pool_dequeue(...) unlock\n", th.id);
    pthread_mutex_unlock(&m_pool);

    DEBUG_END() ;

    return th;
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

    //TODO --> Mover a ops??
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

  void worker_pool_function ( void )
  {
    struct st_th th;

    while(1)
    {
      // Dequeue operation
      th = mpiServer_worker_pool_dequeue ( the_end );
      
      worker_function ( th );
      
    }

    pthread_exit(0);
  }



  void mpiServer_wait_workers ( void )
  {
    DEBUG_BEGIN() ;

    // wait to n_workers be zero...
    debug_info("[WORKERS] pthread_create: lock mpiServer_wait_workers\n");
    pthread_mutex_lock(&m_worker);
    while (n_workers != 0)
    {
      debug_info("[WORKERS] pthread_create: wait mpiServer_wait_workers\n");
      pthread_cond_wait(&c_nworkers, &m_worker);
    }
    debug_info("[WORKERS] pthread_create: unlock mpiServer_wait_workers\n");
    pthread_mutex_unlock(&m_worker);

    DEBUG_END() ;
  }



  void mpiServer_destroy_worker_pool ( void )
  {
    DEBUG_BEGIN() ;

    debug_info("[WORKERS] client: mpiServer_destroy_worker_pool(...) lock\n");
    pthread_mutex_lock(&m_pool_end);
    pool_end=1;
    debug_info("[WORKERS] : mpiServer_destroy_worker_pool(...) unlock\n");
    pthread_mutex_unlock(&m_pool_end);

    debug_info("[WORKERS] : mpiServer_destroy_worker_pool(...) lock\n");
    pthread_mutex_lock(&m_pool);
    debug_info("[WORKERS] : mpiServer_destroy_worker_pool(...) broadcast\n");
    pthread_cond_broadcast(&c_poll_no_empty);
    debug_info("[WORKERS] : mpiServer_destroy_worker_pool(...) unlock\n");
    pthread_mutex_unlock(&m_pool);

    for (int i=0;i<MAX_THREADS;i++){
      debug_info("[WORKERS] : mpiServer_destroy_worker_pool(...) join\n");
      pthread_join(thid[i],NULL);
    }

    debug_info("[WORKERS] : mpiServer_destroy_worker_pool(...) destroy\n");
    pthread_mutex_destroy(&m_pool);
    pthread_cond_destroy(&c_pool_no_full);
    pthread_cond_destroy(&c_poll_no_empty);
    pthread_mutex_destroy(&m_pool_end);

    DEBUG_END() ;
  }






























































    int mpiServer_launch_worker_client ( mpiServer_param_st * params, MPI_Comm sd, int type_op, int rank_client_id, void (*worker_client_function)(struct st_th) )
    {
      int ret;
      pthread_attr_t th_attr;
      pthread_t      th_worker;
      struct st_th   st_worker;

      DEBUG_BEGIN() ;

      pthread_attr_init(&th_attr);
      pthread_attr_setdetachstate(&th_attr, PTHREAD_CREATE_DETACHED);
      pthread_attr_setstacksize  (&th_attr, STACK_SIZE);
      busy_worker = TRUE;

      // prepare arguments...
      st_worker.sd              = sd;
      st_worker.id              = th_cont++;
      st_worker.params          = params ;
      st_worker.type_op         = type_op ;
      st_worker.rank_client_id  = rank_client_id ;
      st_worker.function        = worker_client_function ;

      // create thread...
      debug_info("[WORKERS] pthread_create: create_thread mpiServer_worker_run\n") ;
      ret = pthread_create(&th_worker, &th_attr, (void *)(mpiServer_worker_client_run), (void *)&st_worker);
      if (ret != 0){
        debug_error("[WORKERS] pthread_create %d\n", ret);
        perror("pthread_create: Error en create_thread: ");
      }

      // wait to copy args...
      debug_info("[WORKERS] pthread_create: lock mpiServer_worker_run\n");
      pthread_mutex_lock(&m_worker);

      while (busy_worker == TRUE)
      {
        debug_info("[WORKERS] pthread_create: wait mpiServer_worker_run\n");
        pthread_cond_wait(&c_worker, &m_worker);
      }

      debug_info("[WORKERS] pthread_create: busy_worker= TRUE mpiServer_worker_run\n");
      busy_worker = TRUE;
      debug_info("[WORKERS] pthread_create: unlock mpiServer_worker_run\n");
      pthread_mutex_unlock(&m_worker);

      DEBUG_END() ;
      return 0;
    }












    void mpiServer_worker_client_run ( void *arg )
    {
        struct st_th th;
    
        DEBUG_BEGIN() ;

        // prolog... 
        debug_info("[WORKERS] client(%d): mpiServer_worker_run(...) lock\n", th.id);
        pthread_mutex_lock(&m_worker);
        debug_info("[WORKERS] client(%d): mpiServer_worker_run(...) copy arguments\n", th.id);
        memcpy(&th, arg, sizeof(struct st_th)) ;
        debug_info("[WORKERS] client(%d): mpiServer_worker_run(...) busy_worker = FALSE\n", th.id);
        busy_worker = FALSE;
        debug_info("[WORKERS] client(%d): mpiServer_worker_run(...) n_workers++\n", th.id);
        n_workers++ ;
        debug_info("[WORKERS] client(%d): mpiServer_worker_run(...) signal c_worker\n", th.id);
        pthread_cond_broadcast(&c_worker); // pthread_cond_signal(&c_worker);
        debug_info("[WORKERS] client(%d): mpiServer_worker_run(...) unlock\n", th.id);
        pthread_mutex_unlock(&m_worker);
    
        // do function code...
        th.function(th) ;

        // epilog...
        debug_info("[WORKERS] client(%d): mpiServer_worker_run(...) lock\n", th.id);
        pthread_mutex_lock(&m_worker);
        debug_info("[WORKERS] client(%d): mpiServer_worker_run(...) n_workers--\n", th.id);
        n_workers-- ;
        debug_info("[WORKERS] client(%d): mpiServer_worker_run(...) signal c_nworkers\n", th.id);
        pthread_cond_broadcast(&c_nworkers); // pthread_cond_signal(&c_nworkers);
        debug_info("[WORKERS] client(%d): mpiServer_worker_run(...) unlock\n", th.id);
        pthread_mutex_unlock(&m_worker);

        DEBUG_END() ;

        // end
        pthread_exit(0);
    }






















    void worker_client_function ( struct st_th th )
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

      while (1){

        int ret, n_clients; 

        ret = MPI_Comm_remote_size(th.sd, &n_clients) ;

        printf("RANK ID %d COMM SIZE %d\n", th.rank_client_id, n_clients);

        ret = mpiServer_comm_read_operation_client (&th.params, th.sd, (char *)&head.type, sizeof(head.type), th.rank_client_id);
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
    
        debug_info("[WORKERS] (ID=%d): mpiServer_read_operation and arguments...\n", th.id);
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
      }
      
      debug_info("[WORKERS] (ID=%d) end to do operation '%s'\n", th.id, mpiServer_op2string(th.type_op));
        
    
      //debug_info("[WORKERS] mpiServer_worker_run (ID=%d) close\n", th.id);
      //mpiServer_comm_close(th.params) ;
    }


  /* ................................................................... */