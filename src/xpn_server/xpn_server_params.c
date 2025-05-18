/*
 *  Copyright 2020-2025 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Dario Muñoz Muñoz
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

  #include "xpn_server_params.h"
  #include "base/ns.h"


  /* ... Functions / Funciones ......................................... */

     void xpn_server_params_show(xpn_server_param_st * params)
     {
         debug_info("[Server=%d] [XPN_SERVER_PARAMS] [xpn_server_params_show] >> Begin\n", params->rank);

         printf(" | * XPN server current configuration:\n");

         // * host
         printf(" |\t-h  <host>:\t'%s'\n", params->srv_name);

         // * shutdown_file
         printf(" |\t-f  <path>:\t'%s'\n", params->shutdown_file);

         // * await
         if (params->await_stop == 1) {
             printf(" |\t-w  await true\n");
         }

         // * threads
         if (params->thread_mode_connections == TH_NOT) {
             printf(" |\t-t  <int>:\tWithout threads\n");
         } else
         if (params->thread_mode_connections == TH_POOL) {
             printf(" |\t-t  <int>:\tThread Pool activated\n");
         } else
         if (params->thread_mode_connections == TH_OP) {
             printf(" |\t-t  <int>:\tThread on Demand activated\n");
         } else {
             printf(" |\t-t  <int>:\tError: unknown\n");
         }

         // * server_type
         if (params->server_type == XPN_SERVER_TYPE_MPI) {
             printf(" |\t-s  <int>:\tmpi_server\n");
         } else
         if (params->server_type == XPN_SERVER_TYPE_SCK) {
             printf(" |\t-s  <int>:\tsck_server\n");
         } else {
             printf(" |\t-s  <int>:\tError: unknown\n");
         }

         // * IP version
         printf(" |\t-i  <IP version>:\t'%d'\n", params->ipv);

         // use of mqtt
         if (params->mosquitto_mode == 1) {
             printf(" |\t-m <mqtt_qos>:\t%d\n", params->mosquitto_qos);
         }

         debug_info("[Server=%d] [XPN_SERVER_PARAMS] [xpn_server_params_show] << End\n", params->rank);
     }

     void xpn_server_params_show_usage(void)
     {
         debug_info("[Server=%d] [XPN_SERVER_PARAMS] [xpn_server_params_show_usage] >> Begin\n", -1);

         printf("Usage:\n");
         printf("\t-f  <path>\n");
         printf("\t       ^ file with servers to be shutdowned\n");
         printf("\t-h  <host>\n");
         printf("\t       ^ server name\n");
         printf("\t-w\n");
         printf("\t       await for servers to stop\n");
         printf("\t-s  <server_type as string>\n");
         printf("\t       mpi (for MPI-based    server)\n");
         printf("\t       sck (for socket-based server)\n");
         printf("\t-t  <thread level as integer>\n");
         printf("\t       0 (without thread)\n");
         printf("\t       1 (thread pool)\n");
         printf("\t       2 (thread on demand)\n");
         printf("\t-i  <IP version as integer>\n");
         printf("\t       4 (IPv4)\n");
         printf("\t       6 (IPv6)\n");
         printf("\t-m  <QoS as integer>\n");
         printf("\t       0 (QoS 0)\n");
         printf("\t       1 (QoS 1)\n");
         printf("\t       2 (QoS 2)\n");

         debug_info("[Server=%d] [XPN_SERVER_PARAMS] [xpn_server_params_show_usage] << End\n", -1);
     }

     int xpn_server_params_get(xpn_server_param_st * params, int argc, char * argv[])
     {
         int qos_mode_mqtt = 0;

         debug_info("[Server=%d] [XPN_SERVER_PARAMS] [xpn_server_params_get] >> Begin\n", params->rank);

         // set default values
         params->argc = argc;
         params->argv = argv;

         params->size = 0;
         params->rank = 0;

         params->thread_mode_connections = TH_NOT;
         params->thread_mode_operations  = TH_NOT;

         params->ipv = SCK_IP4;
         params->server_type = XPN_SERVER_TYPE_SCK;
         #ifdef ENABLE_MPI_SERVER
         params->server_type = XPN_SERVER_TYPE_MPI;
         #endif

         params->await_stop = 0;
         strcpy(params->srv_name, "");
         ns_get_hostname(params->srv_name);
         strcpy(params->port_name, "");
         strcpy(params->port_name_conn, "");
         strcpy(params->port_name_no_conn, "");

         // default values for mqtt
         params->mosquitto_mode = 0;
         params->mosquitto_qos  = 0;

         // update user requests
         debug_info("[Server=%d] [XPN_SERVER_PARAMS] [xpn_server_params_get] Get user configuration\n", params->rank);

         for (int i = 0; i < argc; i++)
	 {
             switch (argv[i][0])
	     {
                case '-':
                     switch (argv[i][1])
                     {
                       case 'f':
                            if ((i + 1) < argc)
                                 strcpy(params->shutdown_file, argv[i + 1]);
                            else printf("ERROR: empty shutdown file name.\n");
                            i++;
                            break;

                       case 't':
                            if ((i + 1) < argc)
			    {
                                if (isdigit(argv[i + 1][0]))
				{
                                    int thread_mode_aux = utils_str2int(argv[i + 1], TH_NOT);
                                    if ((thread_mode_aux >= TH_NOT) && (thread_mode_aux <= TH_OP)) {
                                        params->thread_mode_connections = thread_mode_aux;
                                        params->thread_mode_operations  = thread_mode_aux;
                                    } else {
                                        printf("ERROR: unknown option -t '%s'\n", argv[i + 1]);
                                    }
                                } else {
                                    if (strcmp("without", argv[i + 1]) == 0) {
                                        params->thread_mode_connections = TH_NOT;
                                        params->thread_mode_operations  = TH_NOT;
                                    } else if (strcmp("pool", argv[i + 1]) == 0) {
                                        params->thread_mode_connections = TH_POOL;
                                        params->thread_mode_operations  = TH_POOL;
                                    } else if (strcmp("on_demand", argv[i + 1]) == 0) {
                                        params->thread_mode_connections = TH_OP;
                                        params->thread_mode_operations  = TH_OP;
                                    } else {
                                        printf("ERROR: unknown option -t '%s'\n", argv[i + 1]);
                                    }
                                }
                            }
                            i++;
                            break;

                       case 's':
                            if ((i + 1) < argc) {
                                if (strcmp("mpi", argv[i + 1]) == 0) {
                                    params->server_type = XPN_SERVER_TYPE_MPI;
                                } else
                                if (strcmp("sck", argv[i + 1]) == 0) {
                                    params->server_type = XPN_SERVER_TYPE_SCK;
                                } else {
                                    printf("ERROR: unknown option -s '%s'\n", argv[i + 1]);
                                }
                            }
                            i++;
                            break;

                       case 'h':
                            if ((i + 1) < argc)
                                 strcpy(params->srv_name, argv[i + 1]);
                            else printf("ERROR: empty server name.\n");
                            i++;
                            break;

                       case 'w':
                            params->await_stop = 1;
                            break;

                       case 'm':
                            qos_mode_mqtt = utils_str2int(argv[i + 1], 0);
                            if ((qos_mode_mqtt < 0) || (qos_mode_mqtt > 2)) {
                                printf("ERROR: unknown QoS value for MQTT. Default value 0 selected\n");
                                qos_mode_mqtt = 0;
                            }

                            params->mosquitto_mode = 1;
                            params->mosquitto_qos  = qos_mode_mqtt;
                            i++;
                            break;

                       case 'i':
                            params->ipv = utils_str2int(argv[i + 1], SCK_IP4);
                            break;

                       default:
                            break;
                     }
                     break;

                 default:
                     break;
             }
         }

         // In sck_server worker for operations has to be sequential because you don't want to have to make a socket per operation.
         // It can be done because it is not reentrant
         if (params->server_type == XPN_SERVER_TYPE_SCK) {
             params->thread_mode_operations = TH_NOT;
         }

         debug_info("[Server=%d] [XPN_SERVER_PARAMS] [xpn_server_params_get] << End\n", params->rank);

         return 1;
     }


  /* ................................................................... */

