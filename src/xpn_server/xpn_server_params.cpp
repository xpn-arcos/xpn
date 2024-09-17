
/*
 *  Copyright 2020-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Dario Muñoz Muñoz
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

#include "xpn_server_params.hpp"

#include "base/ns.h"

namespace XPN {

/* ... Functions / Funciones ......................................... */

void xpn_server_params::show() {
    debug_info("[Server=%d] [XPN_SERVER_PARAMS] [xpn_server_params_show] >> Begin\n", rank);

    printf(" | * MPI server current configuration:\n");

    // * server_type
    if (server_type == XPN_SERVER_TYPE_MPI) {
        printf(" |\t-s  <int>:\tmpi_server\n");
    } else if (server_type == XPN_SERVER_TYPE_SCK) {
        printf(" |\t-s  <int>:\tsck_server\n");
    } else {
        printf(" |\t-s  <int>:\tError: unknown\n");
    }

    // * threads
    if (thread_mode_connections == TH_NOT) {
        printf(" |\t-t  <int>:\tWithout threads\n");
    } else if (thread_mode_connections == TH_POOL) {
        printf(" |\t-t  <int>:\tThread Pool Activated\n");
    } else if (thread_mode_connections == TH_OP) {
        printf(" |\t-t  <int>:\tThread on demand\n");
    } else {
        printf(" |\t-t  <int>:\tError: unknown\n");
    }

    // * shutdown_file
    printf(" |\t-f  <path>:\t'%s'\n", shutdown_file);
    // * host
    printf(" |\t-h  <host>:\t'%s'\n", srv_name);
    // * await
    if (await_stop == 1) {
        printf(" |\t-w  await true\n");
    }

    debug_info("[Server=%d] [XPN_SERVER_PARAMS] [xpn_server_params_show] << End\n", rank);
}

void xpn_server_params::show_usage() {
    debug_info("[Server=%d] [XPN_SERVER_PARAMS] [xpn_server_params_show_usage] >> Begin\n", -1);

    printf("Usage:\n");
    printf("\t-s  <server_type>:   mpi (for mpi server); sck (for sck server)\n");
    printf("\t-t  <int>:           0 (without thread); 1 (thread pool); 2 (on demand)\n");
    printf("\t-f  <path>:          file of servers to be shutdown\n");
    printf("\t-h  <host>:          host server to be shutdown\n");
    printf("\t-w                   await for servers to stop\n");

    debug_info("[Server=%d] [XPN_SERVER_PARAMS] [xpn_server_params_show_usage] << End\n", -1);
}

xpn_server_params::xpn_server_params(int argc, char *argv[]) {
    debug_info("[Server=%d] [XPN_SERVER_PARAMS] [xpn_server_params_get] >> Begin\n", params->rank);

    // set default values
    argc = argc;
    argv = argv;
    size = 0;
    rank = 0;
    thread_mode_connections = TH_NOT;
    thread_mode_operations = TH_NOT;
    server_type = XPN_SERVER_TYPE_SCK;
#ifdef ENABLE_MPI_SERVER
    server_type = XPN_SERVER_TYPE_MPI;
#endif
    await_stop = 0;
    strcpy(port_name, "");
    strcpy(srv_name, "");

    // update user requests
    debug_info("[Server=%d] [XPN_SERVER_PARAMS] [xpn_server_params_get] Get user configuration\n", rank);

    for (int i = 0; i < argc; i++) {
        switch (argv[i][0]) {
            case '-':
                switch (argv[i][1]) {
                    case 'f':
                        strcpy(shutdown_file, argv[i + 1]);
                        i++;
                        break;

                    case 't':
                        if ((i + 1) < argc) {
                            if (isdigit(argv[i + 1][0])) {
                                int thread_mode_aux = atoi(argv[i + 1]);

                                if (thread_mode_aux >= TH_NOT && thread_mode_aux <= TH_OP) {
                                    thread_mode_connections = thread_mode_aux;
                                    thread_mode_operations = thread_mode_aux;
                                } else {
                                    printf("ERROR: unknown option %s\n", argv[i + 1]);
                                    show_usage();
                                }
                            } else {
                                if (strcmp("without", argv[i + 1]) == 0) {
                                    thread_mode_connections = TH_NOT;
                                    thread_mode_operations = TH_NOT;
                                } else if (strcmp("pool", argv[i + 1]) == 0) {
                                    thread_mode_connections = TH_POOL;
                                    thread_mode_operations = TH_POOL;
                                } else if (strcmp("on_demand", argv[i + 1]) == 0) {
                                    thread_mode_connections = TH_OP;
                                    thread_mode_operations = TH_OP;
                                } else {
                                    printf("ERROR: unknown option %s\n", argv[i + 1]);
                                    show_usage();
                                }
                            }
                        }
                        i++;
                        break;

                    case 's':
                        if ((i + 1) < argc) {
                            if (strcmp("mpi", argv[i + 1]) == 0) {
                                server_type = XPN_SERVER_TYPE_MPI;
                            } else if (strcmp("sck", argv[i + 1]) == 0) {
                                server_type = XPN_SERVER_TYPE_SCK;
                            } else {
                                printf("ERROR: unknown option %s\n", argv[i + 1]);
                                show_usage();
                            }
                        }
                        i++;
                        break;

                    case 'h':
                        strcpy(srv_name, argv[i + 1]);
                        break;
                    case 'w':
                        await_stop = 1;
                        break;

                    default:
                        show_usage();
                        exit(EXIT_FAILURE);
                        break;
                }
                break;

            default:
                break;
        }
    }

    // In sck_server worker for operations has to be sequential because you don't want to have to make a socket per
    // operation. It can be done because it is not reentrant
    if (server_type == XPN_SERVER_TYPE_SCK) {
        thread_mode_operations = TH_NOT;
    }

    debug_info("[Server=%d] [XPN_SERVER_PARAMS] [xpn_server_params_get] << End\n", rank);
}

/* ................................................................... */

}  // namespace XPN
