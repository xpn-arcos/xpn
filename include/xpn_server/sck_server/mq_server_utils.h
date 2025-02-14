
/*
 *  Copyright 2020-2025 Felix Garcia Carballeira, Diego Camarmas Alonso, Elias Del Pozo Puñal, Alejandro Calderon Mateos, Dario Muñoz Muñoz
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


#ifndef _MQ_SERVER_UTILS_H_
#define _MQ_SERVER_UTILS_H_

  #ifdef  __cplusplus
    extern "C" {
  #endif

  /* ... Include / Inclusion ........................................... */

     #include "all_system.h"
     #include <sys/time.h>
     #include "base/utils.h"


  /* ... Const / Constantes ............................................ */

     #define QUEUE_MQ_SIZE 1000000


  /* ... Data Types / Tipo de datos .................................... */

	typedef struct
	{
	    char * topic;
	    char * msg;
	}
	ThreadData;

	typedef struct
	{
	    ThreadData * queue[QUEUE_MQ_SIZE];
	    int front;
	    int rear;
	    int count;
	    pthread_mutex_t mutex;
	    pthread_cond_t not_empty;
	    pthread_cond_t not_full;
	}
	CircularQueueMQ;


  /* ... Functions / Funciones ......................................... */

     double get_time (void) ;
     double get_time_ops ( void ) ;

     void         queue_mq_init ( void ) ;
     void         enqueue_mq ( ThreadData * client ) ;
     ThreadData * dequeue_mq ( void ) ;


  /* ................................................................... */


  #ifdef  __cplusplus
    }
  #endif

#endif

