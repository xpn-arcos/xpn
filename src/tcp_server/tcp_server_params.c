/*
 *  Copyright 2020-2023 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos
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


/* ... Functions / Funciones ......................................... */

void tcp_server_params_show(tcp_server_param_st * params)
{
    DEBUG_BEGIN();

    printf(" * TCP-server current configuration:\n");
    printf("\t-n  <string>:\t'%s'\n", params -> name);
    printf("\t-p  <int>:\t'%s'\n",    params -> port);
    printf("\t-io <int>:\t%d\n",      params -> IOsize);

    printf("\t-ns <string>:\t'%s'\n", params -> dns_file);
    printf("\t-d  <string>:\t'%s'\n", params -> dirbase);

    if (params -> thread_mode == TH_POOL) {
        printf("\t-tp:\t\tThread Pool Activated\n");
    }
    if (params -> thread_mode == TH_OP) {
        printf("\t-ntp:\t\tThread Pool Deactivated (Using Thread per Client)\n");
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
    printf("\t-d  <string>: name of the base directory\n");

    printf("\t-tp:  use thread-pool\n") ;
    printf("\t-ntp: use thread per-client\n");

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
    strcpy(params -> dns_file, "");

    gethostname(params->name, TCP_MAX_PORT_NAME);
    sprintf(params->port, "%d", TCP_SERVER_PORT_DEFAULT);
    params->IOsize  = TCP_SERVER_IOSIZE_DEFAULT;

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
                if ((strlen(argv[i]) == 2)) {
                    strcpy(params -> name, argv[i + 1]);
                    i++;
                }
                if ((strlen(argv[i]) == 3) && (argv[i][2] == 's')) {
                    strcpy(params -> dns_file, argv[i + 1]);
                    i++;
                }
                if ((strlen(argv[i]) == 4) && (argv[i][2] == 't') && (argv[i][3] == 'p')) {
                    params -> thread_mode = TH_OP;
                    i++;
                }
                break;
            case 'i':
                if ((strlen(argv[i]) == 3) && (argv[i][2] == 'o')) {
                    params -> IOsize = atoi(argv[i + 1]);
                    i++;
                }
                break;
            case 'f':
                strcpy(params -> host_file, argv[i + 1]);
                i++;
                break;
            case 'd':
                strcpy(params -> dirbase, argv[i + 1]);
                i++;
                break;
            case 't':
                if ((strlen(argv[i]) == 3) && (argv[i][2] == 'p')) {
                    params -> thread_mode = TH_POOL;
                    i++;
                }
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

