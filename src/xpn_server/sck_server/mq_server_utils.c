
/*
 *  Copyright 2020-2025 Felix Garcia Carballeira, Diego Camarmas Alonso, Elias del Pozo Puñal, Alejandro Calderon Mateos, Dario Muñoz Muñoz
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

   #include "mq_server_utils.h"


/* ... Globals / Globales ............................................ */

   CircularQueueMQ queue_mq;


/* ... Functions / Funciones ......................................... */

// get time...
double get_time_ops ( void )
{
    struct timeval  tp;
    struct timezone tzp;

    gettimeofday( & tp, & tzp);
    return ((double) tp.tv_sec + .000001 * (double) tp.tv_usec);
}

double get_time ( void )
{
    struct timeval  tp;
    struct timezone tzp;

    gettimeofday(&tp,&tzp);
    return((double) tp.tv_sec + .000001 * (double) tp.tv_usec);
}

// CircularQueue
void queue_mq_init ( void )
{
    queue_mq.front =  0;
    queue_mq.rear  = -1;
    queue_mq.count =  0;

    pthread_mutex_init( & queue_mq.mutex, NULL);
    pthread_cond_init ( & queue_mq.not_empty, NULL);
    pthread_cond_init ( & queue_mq.not_full, NULL);
}

void enqueue_mq ( ThreadData * client )
{
    pthread_mutex_lock( &queue_mq.mutex );
    while (queue_mq.count >= QUEUE_MQ_SIZE)
    {
        pthread_cond_wait( & queue_mq.not_full, & queue_mq.mutex);
    }

    queue_mq.rear = (queue_mq.rear + 1) % QUEUE_MQ_SIZE;
    queue_mq.queue[queue_mq.rear] = client;
    queue_mq.count++;

    pthread_cond_signal( & queue_mq.not_empty);
    pthread_mutex_unlock( & queue_mq.mutex);
}

ThreadData * dequeue_mq ( void )
{
    pthread_mutex_lock( &queue_mq.mutex );
    while (queue_mq.count <= 0)
    {
        pthread_cond_wait( & queue_mq.not_empty, & queue_mq.mutex );
    }

    ThreadData * client = queue_mq.queue[queue_mq.front];
    queue_mq.front = (queue_mq.front + 1) % QUEUE_MQ_SIZE;
    queue_mq.count--;

    pthread_cond_signal( & queue_mq.not_full);
    pthread_mutex_unlock( & queue_mq.mutex);

    return client;
}


/* ................................................................... */

