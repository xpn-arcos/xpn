
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
    /*INIT MOSQUITTO CLIENT SIDE */

    #ifdef HAVE_MOSQUITTO_H
        int rc = 0;

        // server_aux->xpn_mosquitto_mode = atoi(env_mosquitto);
        server_aux->xpn_mosquitto_mode = utils_getenv_int("XPN_MQTT", 0) ;

	// MQTT initialization
        if (server_aux->xpn_mosquitto_mode == 1)
        {
	    server_aux->xpn_mosquitto_qos = utils_getenv_int("XPN_MQTT_QOS", 0) ;

            mosquitto_lib_init();

            server_aux -> mqtt = mosquitto_new(NULL, true, NULL);
            if (server_aux -> mqtt == NULL)
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

    // if not mosquitto then return
    if (server_aux -> xpn_mosquitto_mode != 1) {
        return ;
    }

    mosquitto_disconnect(server_aux -> mqtt);
    mosquitto_destroy(server_aux -> mqtt);
    mosquitto_lib_cleanup();

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
            if (ret != MOSQ_ERR_SUCCESS)
            {
                fprintf(stderr, "Error publishing write: %s\n", mosquitto_strerror(ret));
                free(topic);
                return -1;
            }
            
            //printf("PUBLISH --------------- topic: %s\n", topic);

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


/* ................................................................... */

