/*
 *  Copyright 2020-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos
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

#include "tcp_server_params.h"
#include "base/ns.h"


/* ... Functions / Funciones ......................................... */

void tcp_server_params_show(tcp_server_param_st * params)
{
  DEBUG_BEGIN();

  printf("   * TCP-server current configuration:\n");
  printf("\t-n  <string>:\t'%s'\n", params -> name);
  printf("\t-p  <int>:\t'%s'\n", params -> port);
  printf("\t-io <int>:\t%d\n", params -> IOsize);

  printf("\t-ns <string>:\t'%s'\n", params -> dns_file);
  if (params -> thread_mode == TH_NOT) {
    printf("\t-t:\t\tWithout threads\n");
  }
  if (params -> thread_mode == TH_POOL) {
    printf("\t-t:\t\tThread Pool Activated\n");
  }
  if (params -> thread_mode == TH_OP) {
    printf("\t-t:\t\tThread on demand\n");
  }

  printf("\t-d  <string>:\t'%s'\n", params -> dirbase);
  printf("\t-f <path>:\t'%s'\n",   params->shutdown_file) ;

  if (params -> mosquitto_mode == 1) {
    printf("\t-m <mqtt_qos>:\t%d\n", params -> mosquitto_qos);
  }

  DEBUG_END();
}

void tcp_server_params_show_usage(void)
{
  DEBUG_BEGIN();

  printf("Usage:\n");
  printf("\t-n  <string>: name of the server\n");
  printf("\t-p  <int>:    port number\n");
  printf("\t-io <int>:    IOsize\n");

  printf("\t-ns <path>: file for service name\n");
  printf("\t-t  <thread_mode>: 0 (without thread); 1 (thread pool); 2 (on demand)\n");
  printf("\t-d  <string>: name of the base directory\n") ;
  printf("\t-f  <path>: file of servers to be shutdown\n") ;

  printf("\t-m <mqtt_qos_mode>:   0 (QoS 0); 1 (QoS 1); 2 (QoS 2)\n");

  DEBUG_END();
}

int tcp_server_params_get(tcp_server_param_st * params, int argc, char * argv[])
{
  DEBUG_BEGIN();

  // set default values
  params -> argc = argc;
  params -> argv = argv;
  params -> size = 0;
  params -> rank = 0;
  params -> thread_mode = TH_POOL;
  strcpy(params -> port_name, "");
  strcpy(params -> srv_name, "");
  strcpy(params -> dirbase, TCP_SERVER_DIRBASE_DEFAULT);
  strcpy(params -> dns_file, TCP_SERVER_DNS_FILE_DEFAULT);

  gethostname(params -> name, TCP_MAX_PORT_NAME);
  sprintf(params -> port, "%d", TCP_SERVER_PORT_DEFAULT);
  params -> IOsize = TCP_SERVER_IOSIZE_DEFAULT;

  params -> mosquitto_mode = 0;
  params -> mosquitto_qos = 0;

  // update user requests
  for (int i = 0; i < argc; i++) 
  {
    switch (argv[i][0]) 
    {
    case '-':
      switch (argv[i][1]) 
      {
      case 'p':
        strcpy(params -> port, argv[i + 1]);
        i++;
        break;
      case 'n':
        if ((strlen(argv[i]) == 2)) 
        {
          strcpy(params -> name, argv[i + 1]);
          i++;
        }
        if ((strlen(argv[i]) == 3) && (argv[i][2] == 's')) 
        {
          strcpy(params -> dns_file, argv[i + 1]);
          i++;
        }
        if ((strlen(argv[i]) == 4) && (argv[i][2] == 't') && (argv[i][3] == 'p')) 
        {
          params -> thread_mode = TH_OP;
          i++;
        }
        break;
      case 'i':
        if ((strlen(argv[i]) == 3) && (argv[i][2] == 'o')) 
        {
          params -> IOsize = atoi(argv[i + 1]);
          i++;
        }
        break;
      case 'f':
        strcpy(params -> shutdown_file, argv[i + 1]);
        i++;
        break;
      case 'd':
        strcpy(params -> dirbase, argv[i + 1]);
        i++;
        break;
      case 't':
        if (isdigit(argv[i + 1][0])) 
        {
          int thread_mode_aux = atoi(argv[i + 1]);

          if (thread_mode_aux >= TH_NOT && thread_mode_aux <= TH_OP) 
          {
            params -> thread_mode = thread_mode_aux;
          } else 
          {
            printf("ERROR: unknown option %s\n", argv[i + 1]);
          }
        } else 
        {
          if (strcmp("without", argv[i + 1]) == 0) 
          {
            params -> thread_mode = TH_NOT;
          } else if (strcmp("pool", argv[i + 1]) == 0) 
          {
            params -> thread_mode = TH_POOL;
          } else if (strcmp("on_demand", argv[i + 1]) == 0) 
          {
            params -> thread_mode = TH_OP;
          } else 
          {
            printf("ERROR: unknown option %s\n", argv[i + 1]);
          }
        }
        i++;
        break;
      case 'm':
        params -> mosquitto_mode = 1;

        if (isdigit(argv[i + 1][0])) 
        {
          int qos_mode_mqtt = atoi(argv[i + 1]);

          if ( qos_mode_mqtt < 0 || qos_mode_mqtt > 2 )
          {
            printf("ERROR: unknown QoS value for MQTT. Default value 0 selected\n");
          }
          else
          {
            params -> mosquitto_qos = qos_mode_mqtt;
          }
        }
        else
        {
          printf("ERROR: unknown QoS value for MQTT. Default value 0 selected\n");
        }

        i++;
        break;
      case 'h':
        return -1;

      default:
        break;
      }
      break;

    default:
      break;
    }
  }

  // return OK
  DEBUG_END();
  return 1;
}

/* ................................................................... */