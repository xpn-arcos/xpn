#include "myServer_workers.h"

int busy_worker;
pthread_mutex_t m_worker;
pthread_cond_t c_worker;
pthread_t th_worker;

struct st_th st_worker;
int th_cont = 0; 



int myServer_init_worker(){
	//int i;

	//c_worker = PTHREAD_COND_INITIALIZER; 
	//m_worker = PTHREAD_MUTEX_INITIALIZER;

	busy_worker = TRUE;
	pthread_cond_init(&c_worker, NULL);
	pthread_mutex_init(&m_worker, NULL);
	return 0;
}



int myServer_launch_worker(int sd){
	
	pthread_attr_t   tattr;
	//pthread_t *th;	
	//int i;
	int ret;

	pthread_attr_init(&tattr);
	pthread_attr_setdetachstate(&tattr,PTHREAD_CREATE_DETACHED);
 	pthread_attr_setstacksize (&tattr , STACK_SIZE);

#ifdef DBG_WORKERS
	printf("[WORKERS]pthread_create(%d)\n",sd);
#endif
	
	st_worker.value = sd;	
	st_worker.id = th_cont++;	

	busy_worker = TRUE;
	
#ifdef DBG_WORKERS 
  printf("[WORKERS]pthread_create: antes create_thread myServer_worker_run\n");
#endif 
	//usleep(100);
	ret = pthread_create(&th_worker, &tattr, (void *)(myServer_worker_run), (void *)&st_worker);
	
#ifdef DBG_WORKERS 
  printf("[WORKERS]pthread_create: desp. create_thread myServer_worker_run = %d\n", ret);
#endif 
	if(ret != 0){
		perror("pthread_create: Error en create_thread: ");
	}

	
#ifdef DBG_WORKERS 
  printf("[WORKERS]pthread_create: antes lock myServer_worker_run\n");
#endif 
	pthread_mutex_lock(&m_worker);
#ifdef DBG_WORKERS 
  printf("[WORKERS]pthread_create: desp. lock myServer_worker_run\n");
#endif 
	while(busy_worker == TRUE){
#ifdef DBG_WORKERS 
  printf("[WORKERS]pthread_create: antes wait myServer_worker_run\n");
#endif 
		pthread_cond_wait(&c_worker, &m_worker);
#ifdef DBG_WORKERS 
  printf("[WORKERS]pthread_create: desp. wait myServer_worker_run\n");
#endif 
	}

#ifdef DBG_WORKERS 
  printf("[WORKERS]pthread_create: busy_worker= TRUE myServer_worker_run\n");
#endif 
	busy_worker = TRUE;

#ifdef DBG_WORKERS 
  printf("[WORKERS]pthread_create: antes unlock myServer_worker_run\n");
#endif 
	pthread_mutex_unlock(&m_worker);
#ifdef DBG_WORKERS 
  printf("[WORKERS]pthread_create: desp. unlock myServer_worker_run\n");
#endif 
	// siguiente hijo
	
/*
	myServer_worker_run((void*)sd);
*/		
	return 0;
}

/* thread process */
void myServer_worker_run(void *arg)
{
 
  int op, sd; 
  struct st_myServer_msg head;
  struct st_th *th;

  int cont_aux;
  char host[255];
  char id[255];  


  th = (struct st_th *)arg;

  sd = th->value;
  cont_aux = th->id;
  gethostname(host, 255);
  sprintf(id,"[%s:%d:%d]", host, cont_aux, sd);

#ifdef DBG_WORKERS 
  printf("[WORKERS]begin myServer_worker_run(%s)\n", id);
#endif 

#ifdef DBG_WORKERS 
  printf("[WORKERS]client: myServer_worker_run(%s) antes lock\n", id);
#endif 
  pthread_mutex_lock(&m_worker);
#ifdef DBG_WORKERS 
  printf("[WORKERS]client: myServer_worker_run(%s) desp. lock\n", id);
#endif 

  
  //cont_aux = cont;
  //gethostname(host, 255);
  //cont++; 

#ifdef DBG_WORKERS 
  printf("[WORKERS]client: myServer_worker_run(%s) busy_worker = FALSE\n", id);
#endif 
 
  busy_worker = FALSE;
#ifdef DBG_WORKERS 
  printf("[WORKERS]client: myServer_worker_run(%s) signal\n", id);
#endif 
  //pthread_cond_signal(&c_worker);
  pthread_cond_broadcast(&c_worker);
#ifdef DBG_WORKERS 
  printf("[WORKERS]client: myServer_worker_run(%s) antes unlock\n", id);
#endif 
  pthread_mutex_unlock(&m_worker);
#ifdef DBG_WORKERS 
  printf("[WORKERS]client: myServer_worker_run(%s) desp. unlock\n", id);
#endif 

   

  do{
	  /* */
  	head.type = MYSERVER_END;
	strcpy(head.id, id);
#ifdef DBG_WORKERS	
	printf("[WORKERS]myServer_read_operation begin (%s)\n", id);
#endif		
	op = myServer_read_operation(sd, &head);
	strcpy(head.id, id);
#ifdef DBG_WORKERS	
	printf("[WORKERS]OP = %d; ID =%s\n", op, id);
#endif		
 		
	switch(op){
		case MYSERVER_OPEN_FILE:
#ifdef DBG_WORKERS				
			printf("[WORKERS]ID=%s)myServer_op_open begin (sd, &head);\n", id);
#endif			
			myServer_op_open(sd, &head);
#ifdef DBG_WORKERS				
			printf("[WORKERS]ID=%s)myServer_op_open end (sd, &head);\n", id);
#endif			
			break;
		case MYSERVER_CREAT_FILE:
#ifdef DBG_WORKERS				
			printf("[WORKERS]ID=%s)myServer_op_creat begin (sd, &head);\n", id);
#endif			
			myServer_op_creat(sd, &head);
#ifdef DBG_WORKERS				
			printf("[WORKERS]ID=%s)myServer_op_creat end(sd, &head);\n", id);
#endif			
			break;
		case MYSERVER_READ_FILE:
#ifdef DBG_WORKERS				
			printf("[WORKERS]ID=%s)myServer_op_read begin (sd, &head);\n", id);
#endif			
			myServer_op_read(sd, &head);
#ifdef DBG_WORKERS				
			printf("[WORKERS]ID=%s)myServer_op_read end (sd, &head);\n", id);
#endif			
			break;
		case MYSERVER_WRITE_FILE: 
#ifdef DBG_WORKERS				
			printf("[WORKERS]ID=%s)myServer_op_write begin (sd, &head);\n", id);
#endif			
			myServer_op_write(sd, &head);
#ifdef DBG_WORKERS				
			printf("[WORKERS]ID=%s)myServer_op_write end (sd, &head);\n", id);
#endif			
			break;
		case MYSERVER_CLOSE_FILE:
#ifdef DBG_WORKERS				
			printf("[WORKERS]ID=%s)myServer_op_close begin (sd, &head);\n", id);
#endif			
			myServer_op_close(sd, &head);
#ifdef DBG_WORKERS				
			printf("[WORKERS]ID=%s)myServer_op_close end (sd, &head);\n", id);
#endif			
			break;
		case MYSERVER_RM_FILE:
#ifdef DBG_WORKERS				
			printf("[WORKERS]ID=%s)myServer_op_rm begin (sd, &head);\n", id);
#endif			
			myServer_op_rm(sd, &head);
#ifdef DBG_WORKERS				
			printf("[WORKERS]ID=%s)myServer_op_rm end (sd, &head);\n", id);
#endif			
			break;
		case MYSERVER_GETATTR_FILE:
#ifdef DBG_WORKERS				
			printf("[WORKERS]ID=%s)myServer_op_getattr begin (sd, &head);\n", id);
#endif			
			myServer_op_getattr(sd, &head);
#ifdef DBG_WORKERS				
			printf("[WORKERS]ID=%s)myServer_op_getattr end (sd, &head);\n", id);
#endif			
			break;
		case MYSERVER_SETATTR_FILE:
#ifdef DBG_WORKERS				
			printf("[WORKERS]ID=%s)myServer_op_setattr begin (sd, &head);\n", id);
#endif			
			myServer_op_setattr(sd, &head);
#ifdef DBG_WORKERS				
			printf("[WORKERS]ID=%s)myServer_op_setattr end (sd, &head);\n", id);
#endif			
			break;
		case MYSERVER_MKDIR_DIR:
#ifdef DBG_WORKERS				
			printf("[WORKERS]ID=%s)myServer_op_mkdir begin (sd, &head);\n", id);
#endif			
			myServer_op_mkdir(sd, &head);
#ifdef DBG_WORKERS				
			printf("[WORKERS]ID=%s)myServer_op_mkdir end (sd, &head);\n", id);
#endif			
			break;
		case MYSERVER_RMDIR_DIR:
#ifdef DBG_WORKERS				
			printf("[WORKERS]ID=%s)myServer_op_rmdir begin (sd, &head);\n", id);
#endif			
			myServer_op_rmdir(sd, &head);
#ifdef DBG_WORKERS				
			printf("[WORKERS]ID=%s)myServer_op_rmdir end (sd, &head);\n", id);
#endif			
			break;
		case MYSERVER_PRELOAD_FILE:
#ifdef DBG_WORKERS				
			printf("[WORKERS]ID=%s)myServer_op_preload begin (sd, &head);\n", id);
#endif			
			myServer_op_preload(sd, &head);
#ifdef DBG_WORKERS				
			printf("[WORKERS]ID=%s)myServer_op_preload end (sd, &head);\n", id);
#endif			
			break;
		case MYSERVER_FLUSH_FILE:
#ifdef DBG_WORKERS				
			printf("[WORKERS]ID=%s)myServer_op_flush begin (sd, &head);\n", id);
#endif			
			myServer_op_flush(sd, &head);
#ifdef DBG_WORKERS				
			printf("[WORKERS]ID=%s)myServer_op_flush end (sd, &head);\n", id);
#endif			
			break;
		case MYSERVER_GETID:
#ifdef DBG_WORKERS				
			printf("[WORKERS]ID=%s)myServer_op_getid begin (sd, &head);\n", id);
#endif			
			myServer_op_getid(sd, &head);
#ifdef DBG_WORKERS				
			printf("[WORKERS]ID=%s)myServer_op_getid end (sd, &head);\n", id);
#endif			
			break;
                case MYSERVER_FINALIZE:                                                                                     
#ifdef DBG_WORKERS                                                                                                                
                        printf("[WORKERS]ID=%s)myServer_op_finalize begin (sd, &head);\n", id); 
#endif                                                                                                                      
                        op = MYSERVER_FINALIZE;                                                                             
                        printf("[WORKERS]EXIT\n");                                                                                   
#ifdef DBG_WORKERS                                                                                                                
                        printf("[WORKERS]ID=%s)myServer_op_finalize end (sd, &head);\n", id); 
#endif                                                                                                                      
                        exit(0);                                                                                            
                        break;                                                                                              

		default:
#ifdef DBG_WORKERS				
			printf("[WORKERS]ID=%s)myServer_op_end begin (sd, &head);\n", id);
#endif			
			op = MYSERVER_END;
			//printf("[WORKERS]EXIT\n");
#ifdef DBG_WORKERS				
			printf("[WORKERS]ID=%s)myServer_op_end end(sd, &head);\n", id);
#endif			
			break;
	}
			  
  }while(op != MYSERVER_END);

#ifdef DBG_WORKERS				
	printf("[WORKERS]myServer_worker_run(ID=%s): close\n", id);
#endif			
  close(sd);
	
#ifdef DBG_WORKERS 
  printf("[WORKERS]end myServer_worker_run (ID=%s): end\n", id);
#endif 
  pthread_exit(0);
}

