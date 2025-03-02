
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

   #include "mq_server_comm.h"


/* ... Globals / Globales ............................................ */

   int file2 = -1;
   int opened = 0;
   int write_total = 0;
   //int its = 0;


/* ... Functions / Funciones ......................................... */


// Función que se ejecutara en el hilo
void * process_message(__attribute__((__unused__)) void * arg) 
{
    while (1)
    {
        ThreadData * thread_data = dequeue_mq();
        //struct ThreadData * thread_data = (struct ThreadData *) data;

        // Copiar el mensaje en una variable local para manipularla
        char topic[PATH_MAX], path[PATH_MAX];
        bzero (topic, PATH_MAX);
        bzero (path, PATH_MAX);

        int to_write1, offset;


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-truncation"

        strncpy(topic, thread_data -> topic, PATH_MAX);

#pragma GCC diagnostic pop
        // Encontrar la posición del último y el penúltimo slash
        int last_slash = -1;
        int penultimate_slash = -1;
        for (int i = 0; topic[i] != '\0'; i++) 
        {
            if (topic[i] == '/') 
            {
                penultimate_slash = last_slash;
                last_slash = i;
            }
        }

        // Extraer el path y los dos enteros usando sscanf y las posiciones de los slashes

        if (penultimate_slash >= 0 && last_slash > penultimate_slash) 
        {
            // Si hay dos slashes, extraer el path y ambos enteros
            strncpy(path, topic, penultimate_slash);
            path[penultimate_slash] = '\0';
            sscanf( & topic[penultimate_slash + 1], "%d/%d", & to_write1, & offset);

        } 
        else if (last_slash >= 0) 
        {
            // Si solo hay un slash, extraer solo el path y el primer entero
            strncpy(path, topic, last_slash);
            path[last_slash] = '\0';
            sscanf( & topic[last_slash + 1], "%d", & to_write1);
            offset = 0;

        } 
        else 
        {
            // Si no hay slashes, asumir que todo es el path
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-truncation"
            strncpy(path, topic, PATH_MAX - 1);
#pragma GCC diagnostic pop
            path[PATH_MAX - 1] = '\0';
            to_write1 = 0;
            offset = 0;
        }

        //debug_info("\n%s - %s %d %d\n", topic, path, to_write1, offset);

        //char * buffer = NULL;
	int size, diff, cont = 0, to_write = 0, size_written = 0;

        // initialize counters
        size = to_write1;
        if (size > MAX_BUFFER_SIZE) 
        {
            size = MAX_BUFFER_SIZE;
        }
        diff = size - cont;
        //double start_time = 0.0, total_time = 0.0;
        //start_time = get_time();
        //Open file
        int fd = open(path, O_WRONLY | O_CREAT, 0700);
        if (fd < 0) 
        {
	    perror("open: ") ;
            pthread_exit(NULL);
        }

        // malloc a buffer of size...
        /*buffer = (char * ) malloc(size);
        if (NULL == buffer)
        {
            close(fd);
            return;
        }

        bzero(buffer, MAX_BUFFER_SIZE);*/

        /*char copy_header[20];
        strncpy(copy_header, thread_data -> msg, 20);

        if ((strstr(copy_header, "FIN") != NULL))
        {
            struct timeval current_time;
            gettimeofday(&current_time, NULL);
            time_t now = current_time.tv_sec;
            struct tm *timeinfo;
            timeinfo = localtime(&now);

            char time_str[20];
            strftime(time_str, sizeof(time_str), "%H:%M:%S", timeinfo);
            //debug_info("ENDW - %s\n", time_str);

        }
        else if ((strstr(copy_header, "INI") != NULL))
        {
            struct timeval current_time;
            gettimeofday(&current_time, NULL);
            time_t now = current_time.tv_sec;
            struct tm *timeinfo;
            timeinfo = localtime(&now);

            char time_str[20];
            strftime(time_str, sizeof(time_str), "%H:%M:%S", timeinfo);
            //debug_info("STARTW - %s\n", time_str);
        }*/

        if (diff > size)
	     to_write = size;
        else to_write = diff;

        filesystem_lseek(fd, offset + cont, SEEK_SET);
        //debug_info("to write: %d\t msg: %s", to_write, thread_data -> msg);
        size_written = filesystem_write(fd, thread_data -> msg, to_write);
	if (size_written < 0) {
	    printf("process_message: filesystem_write return error\n") ;
	}

        // loop...
        /*do 
        {
            if (diff > size) to_write = size;
            else to_write = diff;

            // read data from TCP and write into the file
            lseek(fd, offset + cont, SEEK_SET);
            //debug_info("to write: %d\t msg: %s", to_write, thread_data -> msg);
            size_written = write(fd, thread_data -> msg, to_write);

            // update counters
            cont = cont + size_written; // Received bytes
            diff = to_write - cont;

        } while ((diff > 0) && (size_written != 0));*/

        close(fd);
        //total_time = (get_time() - start_time);
        //debug_info("%s;%.8f\n", path, total_time);
        //FREE_AND_NULL(buffer);

        // Liberar memoria y finalizar el hilo
        free(thread_data -> msg);
        free(thread_data -> topic);
        free(thread_data);
    }
    pthread_exit(NULL);
}

// MOSQUITTO FILE
#ifdef HAVE_MOSQUITTO_H

void on_message( __attribute__((__unused__)) struct mosquitto * mqtt, void * obj, const struct mosquitto_message * msg) 
{
    if (NULL == obj) {
        debug_info("ERROR: obj is NULL :-( \n");
    }

    /*if (strstr(copy_header, "FIN;") != NULL)
    {
        total_ends += 1;
    }*/

    // Crear una estructura para pasar al hilo
    ThreadData * thread_data = (ThreadData *) malloc(sizeof(ThreadData));

    thread_data -> topic = strdup(msg -> topic);
    thread_data -> msg = (char * ) malloc(msg -> payloadlen + 1);
    bzero(thread_data -> msg, msg -> payloadlen + 1);
    memcpy(thread_data -> msg, msg -> payload, msg -> payloadlen);
    thread_data -> msg[msg -> payloadlen] = '\0';

    enqueue_mq(thread_data);

    /*pthread_t thread_id;
    pthread_create( & thread_id, NULL, process_enqueue, (void * ) thread_data);
    pthread_detach(thread_id);*/

    /*if (opened == 0) 
    {
        char * xpn_time1 = getenv("XPN_TIME");
        //char * xpn_end = getenv("XPN_END_WRITE");
        //write_total = atoi(xpn_end);

        if (xpn_time1 == NULL) 
        {
            debug_info("[TCP-SERVER] Error: process_client\n");
        } 
        else 
        {
            file2 = open(xpn_time1, O_APPEND | O_WRONLY, 0777);
            if (file2 < 0) 
            {
                debug_info("[TCP-SERVER] ERROR: process_client2 %s\n", xpn_time1);
            }
            opened = 1;
        }
    }*/
    
    //its++;
    //mosquitto_unsubscribe(mqtt, NULL, path);
}

#endif


int mq_server_mqtt_init ( xpn_server_param_st * params ) 
{
    if (params -> mosquitto_mode != 1) {
      debug_info("WARNING: mosquitto is not enabled :-(\n") ;
      return 1;
    }

    #ifdef HAVE_MOSQUITTO_H
    
        #ifndef MOSQ_OPT_TCP_NODELAY
        #define MOSQ_OPT_TCP_NODELAY 1
        #endif

        debug_info("[%d]\tBEGIN INIT MOSQUITTO MQ_SERVER\n\n", __LINE__);

        mosquitto_lib_init();

        params -> mqtt = mosquitto_new(NULL, true, NULL);
        if (params -> mqtt == NULL) {
            debug_info(stderr, "Error: Out of memory.\n");
            return 1;
        }

        //mosquitto_connect_callback_set(params -> mqtt, on_connect);
        //mosquitto_subscribe_callback_set(params -> mqtt, on_subscribe);
        mosquitto_message_callback_set(params -> mqtt, on_message);
        mosquitto_int_option(params -> mqtt, MOSQ_OPT_TCP_NODELAY, 1);

        int rc = mosquitto_connect(params -> mqtt, "localhost", 1883, 0);
        if (rc != MOSQ_ERR_SUCCESS) {
            mosquitto_destroy(params -> mqtt);
            debug_info(stderr, "[%d]\tERROR INIT MOSQUITTO MQ_SERVER: %s\n", __LINE__, mosquitto_strerror(rc));
            return 1;
        }

        /* Run the network loop in a background thread, this call returns quickly. */
        rc = mosquitto_loop_start(params -> mqtt);
        if (rc != MOSQ_ERR_SUCCESS) {
            mosquitto_destroy(params -> mqtt);
            debug_info(stderr, "Error: %s\n", mosquitto_strerror(rc));
            return 1;
        }

        //mosquitto_loop_forever(params -> mqtt, -1, 1);
        debug_info("[%d]\tEND INIT MOSQUITTO MQ_SERVER\n\n", __LINE__);


        //Mosquitto pool thread
        int nthreads_mq = 128;
        pthread_t threads_mq[nthreads_mq];
        queue_mq_init();

        // Crear el grupo de hilos (thread pool)
        for (int i = 0; i < nthreads_mq; i++) {
            pthread_create( &threads_mq[i], NULL, process_message, &i );
        }

        return 0 ; // OK: 0, ERROR: 1        
    #endif
}

int mq_server_mqtt_destroy ( xpn_server_param_st * params ) 
{
    if (params -> mosquitto_mode != 1) {
      debug_info("WARNING: mosquitto is not enabled :-(\n") ;
      return 1;
    }

    #ifdef HAVE_MOSQUITTO_H
        debug_info("[%d]\tBEGIN DESTROY MOSQUITTO MQ_SERVER\n\n", __LINE__);
        mosquitto_lib_cleanup();
        mosquitto_loop_stop(params -> mqtt, true);
        debug_info("[%d]\tEND DESTROY MOSQUITTO MQ_SERVER\n\n", __LINE__);

        return 0 ; // OK: 0, ERROR: 1
    #endif
}


/* ................................................................... */

