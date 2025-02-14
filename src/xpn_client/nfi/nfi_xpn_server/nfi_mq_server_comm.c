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

/* ... Include / Inclusion ........................................... */

#include "nfi_mq_server_comm.h"


/* ... Functions / Funciones ......................................... */

void nfi_mq_server_init ( struct nfi_xpn_server *server_aux )
{
	server_aux -> xpn_mosquitto_mode = 0;
	char * env_mosquitto = getenv("XPN_MQTT");

	if (env_mosquitto == NULL) 
	{
          debug_info("WARNING: mosquitto is not enabled :,-(\n") ;
          return;
  }

    /*INIT MOSQUITTO CLIENT SIDE */

    #ifdef HAVE_MOSQUITTO_H
        int rc = 0;
	      char *end;

        // server_aux -> xpn_mosquitto_mode = atoi(env_mosquitto);
        server_aux -> xpn_mosquitto_mode = strtol(env_mosquitto, &end, 10);
        if (*end != '\0') {
          server_aux -> xpn_mosquitto_mode = 0 ;
          debug_info("WARNING: env_mosquitto is not a number :,-(\n") ;
        }

	// MQTT initialization
        if (server_aux -> xpn_mosquitto_mode == 1)
        {
            server_aux -> xpn_mosquitto_qos = 0; // QoS <- 0
            char * env_qos_mqtt = getenv("XPN_MQTT_QOS");
            
            if (env_qos_mqtt != NULL)
	    {
		// server_aux -> xpn_mosquitto_qos = atoi(env_qos_mqtt);
                server_aux -> xpn_mosquitto_qos = strtol(env_qos_mqtt, &end, 10);
		if (*end != '\0') {
                  server_aux -> xpn_mosquitto_qos = 0 ;
		  debug_info("WARNING: env_qos_mqtt is not a number :,-(\n") ;
		}
	    }

            mosquitto_lib_init();
            server_aux -> mqtt = mosquitto_new(NULL, true, NULL);

            if(server_aux -> mqtt == NULL)
            {
                fprintf(stderr, "Error: Out of memory.\n");
                return;
            }

            mosquitto_int_option(server_aux -> mqtt, MOSQ_OPT_TCP_NODELAY, 1);  
            mosquitto_int_option(server_aux -> mqtt, MOSQ_OPT_SEND_MAXIMUM, 65535);

            // printf("%s\n", server_aux->srv_name);
            rc = mosquitto_connect(server_aux -> mqtt, server_aux -> srv_name, 1883, 0);

            if (rc != MOSQ_ERR_SUCCESS)
            {
                mosquitto_destroy(server_aux -> mqtt);
                fprintf(stderr, "Error: %s\n", mosquitto_strerror(rc));
                return;
            }

            /* Run the network loop in a background thread, this call returns quickly. */
            rc = mosquitto_loop_start(server_aux -> mqtt);

            if (rc != MOSQ_ERR_SUCCESS)
            {
                mosquitto_destroy(server_aux -> mqtt);
                fprintf(stderr, "Error: %s\n", mosquitto_strerror(rc));
                return;
            }
        }

    #endif
}


void nfi_mq_server_destroy ( struct nfi_xpn_server *server_aux ) 
{

    #ifdef HAVE_MOSQUITTO_H

    if (server_aux -> xpn_mosquitto_mode == 1)
    {
        mosquitto_disconnect(server_aux -> mqtt);
        mosquitto_destroy(server_aux -> mqtt);
        mosquitto_lib_cleanup();
    }   

    #endif
}


ssize_t nfi_mq_server_publish ( struct nfi_xpn_server *server_aux, struct nfi_xpn_server_fhandle *fh_aux, void * buffer, off_t offset, size_t size )
{
    int ret, diff, cont;

    diff = size;
    cont = 0;

    int buffer_size = size;

    // Max buffer size
    if (buffer_size > MAX_BUFFER_SIZE) {
        buffer_size = MAX_BUFFER_SIZE;
    }

    // writes n times: number of bytes + write data (n bytes)
    do {

    	int bytes_to_write = 0;
        char *topic = malloc(strlen(fh_aux -> path) + sizeof(bytes_to_write) + sizeof(offset) + 3);
           
        if( diff > buffer_size )        bytes_to_write = buffer_size;
        else                            bytes_to_write = diff;

        #ifdef HAVE_MOSQUITTO_H

            sprintf(topic, "%s/%d/%ld", fh_aux -> path, bytes_to_write, offset);
            
            ret = mosquitto_publish(server_aux -> mqtt, NULL, topic, bytes_to_write, (char * ) buffer + cont, server_aux -> xpn_mosquitto_qos, false);

            //printf("PUBLISH --------------- topic: %s\n", topic);

            if(ret != MOSQ_ERR_SUCCESS)
            {
                fprintf(stderr, "Error publishing write: %s\n", mosquitto_strerror(ret));
                free(topic);
                return -1;
            }
            
        #endif

        if (ret < 0) 
        {
            fprintf(stderr, "(2)ERROR: nfi_mq_server_write: Error on write operation\n") ;
            return -1;
        }

        free(topic);
        cont = cont + bytes_to_write; //Send bytes
        diff = size - cont;

    } while ((diff > 0) && (ret != 0));

    ret = cont;

    return ret;
}
