
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

   #include "mq_server_ops.h"


/* ... Functions / Funciones ......................................... */

void mq_server_op_subscribe ( xpn_server_param_st *params, struct st_xpn_server_msg *head )
{
    if (params->mosquitto_mode != 1) {
    	debug_info("WARNING: mosquitto is not enabled :-(\n") ;
    	return ;
    }

    #ifdef HAVE_MOSQUITTO_H

    	//char * s;
	    char * extra = "/#";
	    char * sm = malloc(strlen(head->u_st_xpn_server_msg.op_open.path) + strlen(extra) + 1);
	    strcpy(sm, head->u_st_xpn_server_msg.op_open.path);
	    strcat(sm, extra);

	    debug_info("[%d]\tBEGIN OPEN MOSQUITTO MQ_SERVER WS - %s\n", __LINE__, sm);

	    int rc = mosquitto_subscribe(params->mqtt, NULL, sm, params->mosquitto_qos);
	    if (rc != MOSQ_ERR_SUCCESS) {
	        debug_info(stderr, "Error subscribing open: %s\n", mosquitto_strerror(rc));
	        mosquitto_disconnect(params->mqtt);
	    }

	    debug_info("[%d]\tEND OPEN MOSQUITTO MQ_SERVER WS - %s\n\n", __LINE__, sm);

    #endif
}


void mq_server_op_unsubscribe ( xpn_server_param_st *params, struct st_xpn_server_msg *head )
{
    if (params->mosquitto_mode != 1) 
    {
    	debug_info("WARNING: mosquitto is not enabled :-(\n") ;
    	return ;
    }

    #ifdef HAVE_MOSQUITTO_H

	char * extra = "/#";
	char * sm = malloc(strlen(head->u_st_xpn_server_msg.op_close.path) + strlen(extra) + 1);
	strcpy(sm, head->u_st_xpn_server_msg.op_close.path);
	strcat(sm, extra);

	char * s;
	s = head->u_st_xpn_server_msg.op_close.path;

	debug_info("[%d]\tBEGIN CLOSE MOSQUITTO MQ_SERVER - WS \n\n", __LINE__);

    mosquitto_unsubscribe(params->mqtt, NULL, sm);
    mosquitto_unsubscribe(params->mqtt, NULL, s);

    debug_info("[%d]\tEND CLOSE MOSQUITTO MQ_SERVER - WS %s\n\n", __LINE__, sm);

    #endif
}


/* ................................................................... */


