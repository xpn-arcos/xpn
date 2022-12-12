
/*
 *  Copyright 2020-2022 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
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

    #include "tcp_server_workers.h"


    /* ... Global variables / Variables globales ......................... */

    int busy_worker;
    pthread_mutex_t m_worker;
    pthread_cond_t c_worker;
    pthread_t th_worker;

    struct st_th st_worker;
    int th_cont = 0;

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


    /* ... Functions / Funciones ......................................... */

    int tcp_server_init_worker()
    {
        //int i;

        //c_worker = PTHREAD_COND_INITIALIZER;
        //m_worker = PTHREAD_MUTEX_INITIALIZER;

        busy_worker = TRUE;
        pthread_cond_init(&c_worker, NULL);
        pthread_mutex_init(&m_worker, NULL);
        return 0;
    }

    int tcp_server_init_worker_pool ( void ){
        DEBUG_BEGIN() ;

        pthread_mutex_init(&m_pool,NULL);
        pthread_cond_init (&c_pool_no_full,NULL);
        pthread_cond_init (&c_poll_no_empty,NULL);
        pthread_mutex_init(&m_pool_end,NULL);

        DEBUG_END() ;

        return 0;
    }



    int tcp_server_launch_worker(int sd)
    {
        pthread_attr_t   tattr;
        //pthread_t *th;
        //int i;
        int ret;

        pthread_attr_init(&tattr);
        pthread_attr_setdetachstate(&tattr,PTHREAD_CREATE_DETACHED);
        pthread_attr_setstacksize (&tattr , STACK_SIZE);

        debug_info("[WORKERS] pthread_create(%d)\n",sd);

        st_worker.sd = sd;
        st_worker.id = th_cont++;

        busy_worker = TRUE;

        debug_info("[WORKERS] pthread_create: antes create_thread tcp_server_worker_run\n");
        //usleep(100);
        ret = pthread_create(&th_worker, &tattr, (void *(*)(void *))(tcp_server_worker_run), (void *)&st_worker);

        debug_info("[WORKERS] pthread_create: desp. create_thread tcp_server_worker_run = %d\n", ret);
        if (ret != 0) {
            perror("pthread_create: Error en create_thread: ");
        }

        debug_info("[WORKERS] pthread_create: antes lock tcp_server_worker_run\n");
        pthread_mutex_lock(&m_worker);
        debug_info("[WORKERS] pthread_create: desp. lock tcp_server_worker_run\n");
        while (busy_worker == TRUE)
        {
            debug_info("[WORKERS] pthread_create: antes wait tcp_server_worker_run\n");
            pthread_cond_wait(&c_worker, &m_worker);
            debug_info("[WORKERS] pthread_create: desp. wait tcp_server_worker_run\n");
        }

        debug_info("[WORKERS] pthread_create: busy_worker= TRUE tcp_server_worker_run\n");
        busy_worker = TRUE;

        debug_info("[WORKERS] pthread_create: antes unlock tcp_server_worker_run\n");
        pthread_mutex_unlock(&m_worker);
        debug_info("[WORKERS] pthread_create: desp. unlock tcp_server_worker_run\n");

        // siguiente hijo
    /*
        tcp_server_worker_run((void*)sd);
    */
        return 0;
    }

    int tcp_server_launch_worker_pool ( void (*worker_pool_function)(void) ){
        DEBUG_BEGIN() ;

        for (int i = 0; i < MAX_THREADS; i++){
          debug_info("[WORKERS] pthread_create: create_thread tcp_server_launch_worker_pool\n") ;
          if (pthread_create(&thid[i], NULL, (void *(*)(void *))(worker_pool_function), NULL) !=0){
            perror("Error creating thread pool\n");
            return -1;
          }
        }

        DEBUG_END() ;

        return 0;
    }



    /* thread process */
    void * tcp_server_worker_run ( void *arg )
    {
      int    sd;
      struct st_th *th;

      int cont_aux;
      char host[255];
      char id[1024];


      th = (struct st_th *)arg;

      sd = th->sd;
      cont_aux = th->id;
      gethostname(host, 255);
      sprintf(id,"[%s:%d:%d]", host, cont_aux, sd);

      debug_info("[WORKERS] begin tcp_server_worker_run(%s)\n", id);

      debug_info("[WORKERS] client: tcp_server_worker_run(%s) antes lock\n", id);
      pthread_mutex_lock(&m_worker);
      debug_info("[WORKERS] client: tcp_server_worker_run(%s) desp. lock\n", id);

      //cont_aux = cont;
      //gethostname(host, 255);
      //cont++;

      debug_info("[WORKERS] client: tcp_server_worker_run(%s) busy_worker = FALSE\n", id);

      busy_worker = FALSE;
      debug_info("[WORKERS] client: tcp_server_worker_run(%s) signal\n", id);
      //pthread_cond_signal(&c_worker);
      pthread_cond_broadcast(&c_worker);
      debug_info("[WORKERS] client: tcp_server_worker_run(%s) antes unlock\n", id);
      pthread_mutex_unlock(&m_worker);
      debug_info("[WORKERS] client: tcp_server_worker_run(%s) desp. unlock\n", id);

      worker_function (*th);

      /*do
      {
        head.type = TCP_SERVER_END;
        strcpy(head.id, id);
        debug_info("[WORKERS] tcp_server_read_operation begin (%s)\n", id);
        op = tcp_server_read_operation(sd, &head);
        strcpy(head.id, id);
        debug_info("[WORKERS] OP = %d; ID =%s\n", op, id);
     
        switch(op){
            case TCP_SERVER_OPEN_FILE:
                debug_info("[WORKERS] (ID=%s) tcp_server_op_open begin (sd, &head);\n", id);
                tcp_server_op_open(sd, &head);
                debug_info("[WORKERS] (ID=%s) tcp_server_op_open end (sd, &head);\n", id);
                break;
            case TCP_SERVER_CREAT_FILE:
                debug_info("[WORKERS] (ID=%s) tcp_server_op_creat begin (sd, &head);\n", id);
                tcp_server_op_creat(sd, &head);
                debug_info("[WORKERS] (ID=%s) tcp_server_op_creat end(sd, &head);\n", id);
                break;
            case TCP_SERVER_READ_FILE:
                debug_info("[WORKERS] (ID=%s) tcp_server_op_read begin (sd, &head);\n", id);
                tcp_server_op_read(sd, &head);
                debug_info("[WORKERS] (ID=%s) tcp_server_op_read end (sd, &head);\n", id);
                break;
            case TCP_SERVER_WRITE_FILE:
                debug_info("[WORKERS] (ID=%s) tcp_server_op_write begin (sd, &head);\n", id);
                tcp_server_op_write(sd, &head);
                debug_info("[WORKERS] (ID=%s) tcp_server_op_write end (sd, &head);\n", id);
                break;
            case TCP_SERVER_CLOSE_FILE:
                debug_info("[WORKERS] (ID=%s) tcp_server_op_close begin (sd, &head);\n", id);
                tcp_server_op_close(sd, &head);
                debug_info("[WORKERS] (ID=%s) tcp_server_op_close end (sd, &head);\n", id);
                break;
            case TCP_SERVER_RM_FILE:
                debug_info("[WORKERS] (ID=%s) tcp_server_op_rm begin (sd, &head);\n", id);
                tcp_server_op_rm(sd, &head);
                debug_info("[WORKERS] (ID=%s) tcp_server_op_rm end (sd, &head);\n", id);
                break;
            case TCP_SERVER_GETATTR_FILE:
                debug_info("[WORKERS] (ID=%s) tcp_server_op_getattr begin (sd, &head);\n", id);
                tcp_server_op_getattr(sd, &head);
                debug_info("[WORKERS] (ID=%s) tcp_server_op_getattr end (sd, &head);\n", id);
                break;
            case TCP_SERVER_SETATTR_FILE:
                debug_info("[WORKERS] (ID=%s) tcp_server_op_setattr begin (sd, &head);\n", id);
                tcp_server_op_setattr(sd, &head);
                debug_info("[WORKERS] (ID=%s) tcp_server_op_setattr end (sd, &head);\n", id);
                break;
            case TCP_SERVER_MKDIR_DIR:
                debug_info("[WORKERS] (ID=%s) tcp_server_op_mkdir begin (sd, &head);\n", id);
                tcp_server_op_mkdir(sd, &head);
                debug_info("[WORKERS] (ID=%s) tcp_server_op_mkdir end (sd, &head);\n", id);
                break;
            case TCP_SERVER_RMDIR_DIR:
                debug_info("[WORKERS] (ID=%s) tcp_server_op_rmdir begin (sd, &head);\n", id);
                tcp_server_op_rmdir(sd, &head);
                debug_info("[WORKERS] (ID=%s) tcp_server_op_rmdir end (sd, &head);\n", id);
                break;
            case TCP_SERVER_PRELOAD_FILE:
                debug_info("[WORKERS] (ID=%s) tcp_server_op_preload begin (sd, &head);\n", id);
                tcp_server_op_preload(sd, &head);
                debug_info("[WORKERS] (ID=%s) tcp_server_op_preload end (sd, &head);\n", id);
                break;
            case TCP_SERVER_FLUSH_FILE:
                debug_info("[WORKERS] (ID=%s) tcp_server_op_flush begin (sd, &head);\n", id);
                tcp_server_op_flush(sd, &head);
                debug_info("[WORKERS] (ID=%s) tcp_server_op_flush end (sd, &head);\n", id);
                break;
            case TCP_SERVER_GETID:
                debug_info("[WORKERS] (ID=%s) tcp_server_op_getid begin (sd, &head);\n", id);
                tcp_server_op_getid(sd, &head);
                debug_info("[WORKERS] (ID=%s) tcp_server_op_getid end (sd, &head);\n", id);
                break;
            case TCP_SERVER_FINALIZE:
                debug_info("[WORKERS] (ID=%s) tcp_server_op_finalize begin (sd, &head);\n", id);
                op = TCP_SERVER_FINALIZE;
                printf("[WORKERS] EXIT\n");
                debug_info("[WORKERS] (ID=%s) tcp_server_op_finalize end (sd, &head);\n", id);
                exit(0);
                break;
            default:
                debug_info("[WORKERS] (ID=%s) tcp_server_op_end begin (sd, &head);\n", id);
                op = TCP_SERVER_END;
                //printf("[WORKERS] EXIT\n");
                debug_info("[WORKERS] (ID=%s) tcp_server_op_end end(sd, &head);\n", id);
                break;
        }

      } while (op != TCP_SERVER_END);*/

      debug_info("[WORKERS] tcp_server_worker_run (ID=%s): close\n", id);
      close(sd);

      debug_info("[WORKERS] end tcp_server_worker_run (ID=%s): end\n", id);
      pthread_exit(0);
      return NULL;
    }


    void tcp_server_worker_pool_enqueue ( int sd, int id )
    {
        DEBUG_BEGIN() ;

        debug_info("[WORKERS] client(%d): tcp_server_worker_pool_enqueue(...) lock\n", id);
        pthread_mutex_lock(&m_pool);
        while (n_operation == MAX_OPERATIONS){
          debug_info("[WORKERS] client(%d): tcp_server_worker_pool_enqueue(...) wait c_pool_no_full\n", id);
          pthread_cond_wait(&c_pool_no_full, &m_pool);
        }

        debug_info("[WORKERS] client(%d): tcp_server_worker_pool_enqueue(...) copy arguments\n", id);
        struct st_th st_worker;

        st_worker.sd = sd;
        st_worker.id = id;

        debug_info("[WORKERS] client(%d): tcp_server_worker_pool_enqueue(...) enqueue\n", id);
        operations_buffer[enq_pos] = st_worker;
        enq_pos = (enq_pos+1) % MAX_OPERATIONS;
        n_operation++;

        debug_info("[WORKERS] client(%d): tcp_server_worker_pool_enqueue(...) signal c_poll_no_empty\n", id);
        pthread_cond_signal(&c_poll_no_empty);
        debug_info("[WORKERS] client(%d): tcp_server_worker_pool_enqueue(...) unlock\n", id);
        pthread_mutex_unlock(&m_pool);

        DEBUG_END() ;
    }

    struct st_th tcp_server_worker_pool_dequeue ( int the_end ) {
        DEBUG_BEGIN() ;

        struct st_th th;

        debug_info("[WORKERS] client(%d): tcp_server_worker_pool_dequeue(...) lock\n", th.id);
        pthread_mutex_lock(&m_pool);

        while (n_operation == 0) {
          if ( pool_end==1 || the_end==1 ) {
            debug_info("[WORKERS] client(%d): tcp_server_worker_pool_dequeue(...) unlock end\n", th.id);
            pthread_mutex_unlock(&m_pool);
            debug_info("[WORKERS] client(%d): tcp_server_worker_pool_dequeue(...) exit\n", th.id);
            pthread_exit(0);
          }
          debug_info("[WORKERS] client(%d): tcp_server_worker_pool_dequeue(...) wait c_poll_no_empty\n", th.id);
          pthread_cond_wait(&c_poll_no_empty, &m_pool);
        }

        debug_info("[WORKERS] thread id = %ld\n", pthread_self());

        debug_info("[WORKERS] client(%d): tcp_server_worker_pool_dequeue(...) dequeue\n", th.id);
        th = operations_buffer[deq_pos];
        deq_pos = (deq_pos + 1) % MAX_OPERATIONS;
        n_operation--;

        debug_info("[WORKERS] client(%d): tcp_server_worker_pool_dequeue(...) signal c_pool_no_full\n", th.id);
        pthread_cond_signal(&c_pool_no_full);

        debug_info("[WORKERS] client(%d): tcp_server_worker_pool_dequeue(...) unlock\n", th.id);
        pthread_mutex_unlock(&m_pool);

        DEBUG_END() ;

        return th;
    }



    void worker_function ( struct st_th th )
    {
        struct st_tcp_server_msg head;
        //int ret;
        int op;

        do
        {
            /* */
            head.type = (char) TCP_SERVER_END;
            strcpy(head.id, (char*) &th.id);
            debug_info("[WORKERS] tcp_server_read_operation begin (%d)\n", th.id);
            op = tcp_server_read_operation(th.sd, &head);
            strcpy(head.id, (char*) &th.id);
            debug_info("[WORKERS] OP = %d; ID =%d\n", op, th.id);
         
            switch(op){
                case TCP_SERVER_OPEN_FILE:
                    debug_info("[WORKERS] (ID=%s) tcp_server_op_open begin (th.sd, &head);\n", th.id);
                    tcp_server_op_open(th.sd, &head);
                    debug_info("[WORKERS] (ID=%s) tcp_server_op_open end (th.sd, &head);\n", th.id);
                    break;
                case TCP_SERVER_CREAT_FILE:
                    debug_info("[WORKERS] (ID=%s) tcp_server_op_creat begin (th.sd, &head);\n", th.id);
                    tcp_server_op_creat(th.sd, &head);
                    debug_info("[WORKERS] (ID=%s) tcp_server_op_creat end(th.sd, &head);\n", th.id);
                    break;
                case TCP_SERVER_READ_FILE:
                    debug_info("[WORKERS] (ID=%s) tcp_server_op_read begin (th.sd, &head);\n", th.id);
                    tcp_server_op_read(th.sd, &head);
                    debug_info("[WORKERS] (ID=%s) tcp_server_op_read end (th.sd, &head);\n", th.id);
                    break;
                case TCP_SERVER_WRITE_FILE:
                    debug_info("[WORKERS] (ID=%s) tcp_server_op_write begin (th.sd, &head);\n", th.id);
                    tcp_server_op_write(th.sd, &head);
                    debug_info("[WORKERS] (ID=%s) tcp_server_op_write end (th.sd, &head);\n", th.id);
                    break;
                case TCP_SERVER_CLOSE_FILE:
                    debug_info("[WORKERS] (ID=%s) tcp_server_op_close begin (th.sd, &head);\n", th.id);
                    tcp_server_op_close(th.sd, &head);
                    debug_info("[WORKERS] (ID=%s) tcp_server_op_close end (th.sd, &head);\n", th.id);
                    break;
                case TCP_SERVER_RM_FILE:
                    debug_info("[WORKERS] (ID=%s) tcp_server_op_rm begin (th.sd, &head);\n", th.id);
                    tcp_server_op_rm(th.sd, &head);
                    debug_info("[WORKERS] (ID=%s) tcp_server_op_rm end (th.sd, &head);\n", th.id);
                    break;
                case TCP_SERVER_GETATTR_FILE:
                    debug_info("[WORKERS] (ID=%s) tcp_server_op_getattr begin (th.sd, &head);\n", th.id);
                    tcp_server_op_getattr(th.sd, &head);
                    debug_info("[WORKERS] (ID=%s) tcp_server_op_getattr end (th.sd, &head);\n", th.id);
                    break;
                case TCP_SERVER_SETATTR_FILE:
                    debug_info("[WORKERS] (ID=%s) tcp_server_op_setattr begin (th.sd, &head);\n", th.id);
                    tcp_server_op_setattr(th.sd, &head);
                    debug_info("[WORKERS] (ID=%s) tcp_server_op_setattr end (th.sd, &head);\n", th.id);
                    break;
                case TCP_SERVER_MKDIR_DIR:
                    debug_info("[WORKERS] (ID=%s) tcp_server_op_mkdir begin (th.sd, &head);\n", th.id);
                    tcp_server_op_mkdir(th.sd, &head);
                    debug_info("[WORKERS] (ID=%s) tcp_server_op_mkdir end (th.sd, &head);\n", th.id);
                    break;
                case TCP_SERVER_RMDIR_DIR:
                    debug_info("[WORKERS] (ID=%s) tcp_server_op_rmdir begin (th.sd, &head);\n", th.id);
                    tcp_server_op_rmdir(th.sd, &head);
                    debug_info("[WORKERS] (ID=%s) tcp_server_op_rmdir end (th.sd, &head);\n", th.id);
                    break;
                case TCP_SERVER_PRELOAD_FILE:
                    debug_info("[WORKERS] (ID=%s) tcp_server_op_preload begin (th.sd, &head);\n", th.id);
                    tcp_server_op_preload(th.sd, &head);
                    debug_info("[WORKERS] (ID=%s) tcp_server_op_preload end (th.sd, &head);\n", th.id);
                    break;
                case TCP_SERVER_FLUSH_FILE:
                    debug_info("[WORKERS] (ID=%s) tcp_server_op_flush begin (th.sd, &head);\n", th.id);
                    tcp_server_op_flush(th.sd, &head);
                    debug_info("[WORKERS] (ID=%s) tcp_server_op_flush end (th.sd, &head);\n", th.id);
                    break;
                case TCP_SERVER_GETID:
                    debug_info("[WORKERS] (ID=%s) tcp_server_op_getid begin (th.sd, &head);\n", th.id);
                    tcp_server_op_getid(th.sd, &head);
                    debug_info("[WORKERS] (ID=%s) tcp_server_op_getid end (th.sd, &head);\n", th.id);
                    break;
                case TCP_SERVER_FINALIZE:
                    debug_info("[WORKERS] (ID=%s) tcp_server_op_finalize begin (th.sd, &head);\n", th.id);
                    op = TCP_SERVER_FINALIZE;
                    printf("[WORKERS] EXIT\n");
                    debug_info("[WORKERS] (ID=%s) tcp_server_op_finalize end (th.sd, &head);\n", th.id);
                    exit(0);
                    break;
                default:
                    debug_info("[WORKERS] (ID=%s) tcp_server_op_end begin (th.sd, &head);\n", th.id);
                    op = TCP_SERVER_END;
                    //printf("[WORKERS] EXIT\n");
                    debug_info("[WORKERS] (ID=%s) tcp_server_op_end end(th.sd, &head);\n", th.id);
                    break;
            }

        } while (op != TCP_SERVER_END);
    }

    void worker_pool_function ( void )
    {
        int is_true = 1;
        struct st_th th;

        while(is_true)
        {
          // Dequeue operation
          th = tcp_server_worker_pool_dequeue ( the_end );
          
          worker_function ( th );
          
        }

        pthread_exit(0);
    }



    void tcp_server_destroy_worker_pool ( void ) {
        DEBUG_BEGIN() ;

        debug_info("[WORKERS] client: tcp_server_destroy_worker_pool(...) lock\n");
        pthread_mutex_lock(&m_pool_end);
        pool_end=1;
        debug_info("[WORKERS] client: tcp_server_destroy_worker_pool(...) unlock\n");
        pthread_mutex_unlock(&m_pool_end);

        debug_info("[WORKERS] client: tcp_server_destroy_worker_pool(...) lock\n");
        pthread_mutex_lock(&m_pool);
        debug_info("[WORKERS] client: tcp_server_destroy_worker_pool(...) broadcast\n");
        pthread_cond_broadcast(&c_poll_no_empty);
        debug_info("[WORKERS] client: tcp_server_destroy_worker_pool(...) unlock\n");
        pthread_mutex_unlock(&m_pool);

        for (int i=0;i<MAX_THREADS;i++){
          debug_info("[WORKERS] client: tcp_server_destroy_worker_pool(...) join\n");
          pthread_join(thid[i],NULL);
        }

        debug_info("[WORKERS] client: tcp_server_destroy_worker_pool(...) destroy\n");
        pthread_mutex_destroy(&m_pool);
        pthread_cond_destroy(&c_pool_no_full);
        pthread_cond_destroy(&c_poll_no_empty);
        pthread_mutex_destroy(&m_pool_end);

        DEBUG_END() ;
    }
