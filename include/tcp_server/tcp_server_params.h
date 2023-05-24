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

/**
 * @file tcp_server_params.h
 * @brief Header file to 'TODO'.
 *
 * Header file to 'TODO'.
 *
 * @authors Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
 * @date  Jul 22, 2021
 * @bug No known bugs.
 */

#ifndef _TCP_SERVER_PARAMS_H_
#define _TCP_SERVER_PARAMS_H_

/************************************************
 *  ... Includes
 ***********************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "base/utils.h"
#include "base/workers.h"
// #include <semaphore.h>

/************************************************
 *  ... Constants
 ***********************************************/

#ifndef TCP_SERVER_DIRBASE_DEFAULT
#define TCP_SERVER_DIRBASE_DEFAULT "/tmp"
#endif

#ifndef TCP_SERVER_PORT_DEFAULT
#define TCP_SERVER_PORT_DEFAULT 9999
#endif

#ifndef TCP_SERVER_IOSIZE_DEFAULT
#define TCP_SERVER_IOSIZE_DEFAULT (MB)
#endif

#define TCP_MAX_PORT_NAME 1024

/************************************************
 *  ... Datatype
 ***********************************************/

/** @struct tcp_server_param_st
 *  This is a struct
 *
 *  @var tcp_server_param_st::size
 *    A 'TODO'.
 *  @var tcp_server_param_st::rank
 *    A 'TODO'.
 *  @var tcp_server_param_st::port_name
 *    A 'TODO'.
 *  @var tcp_server_param_st::srv_name
 *    A 'TODO'.
 *  @var tcp_server_param_st::name
 *    A 'TODO'.
 *  @var tcp_server_param_st::port
 *    A 'TODO'.
 *  @var tcp_server_param_st::IOsize
 *    A 'TODO'.
 *  @var tcp_server_param_st::dirbase
 *    A 'TODO'.
 *  @var tcp_server_param_st::dns_file
 *    A 'TODO'.
 *  @var tcp_server_param_st::shutdown_file
 *    A 'TODO'.
 *  @var tcp_server_param_st::thread_mode
 *    A 'TODO'.
 *  @var tcp_server_param_st::mosquitto_mode
 *    A 'TODO'.
 *  @var tcp_server_param_st::mosquitto_qos
 *    A 'TODO'.
 *  @var tcp_server_param_st::client
 *    A 'TODO'.
 *  @var tcp_server_param_st::global_sock
 *    A 'TODO'.
 *  @var tcp_server_param_st::argc
 *    A 'TODO'.
 *  @var tcp_server_param_st::argv
 *    A 'TODO'.
 */
typedef struct
{
  // server identification
  int size;
  int rank;
  char port_name[TCP_MAX_PORT_NAME];
  char srv_name[TCP_MAX_PORT_NAME];

  char name[TCP_MAX_PORT_NAME];
  char port[TCP_MAX_PORT_NAME];
  int IOsize;

  char dirbase[PATH_MAX];
  char dns_file[PATH_MAX];
  char shutdown_file[PATH_MAX];

  // server configuration
  int thread_mode;

  // mqtt configuration
  int mosquitto_mode;
  int mosquitto_qos;

#ifdef HAVE_MOSQUITTO_H
  struct mosquitto *mqtt;
#endif

  // Semaphore for clients
  // char sem_name_server [PATH_MAX];

  // Semaphore for server disk
  // sem_t disk_sem;

  // associated client
  int client;
  int global_sock;

  // server arguments
  int argc;
  char **argv;

} tcp_server_param_st;

/************************************************
 *  ... Functions
 ***********************************************/

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @par Parameters
 *    None.
 * @par Returns
 *    Nothing.
 */
void tcp_server_params_show_usage(void);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param params  'TODO'.
 * @param argc  'TODO'.
 * @param argv  'TODO'.
 * @return 'TODO'.
 */
int tcp_server_params_get(tcp_server_param_st *params, int argc, char *argv[]);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param params  'TODO'.
 * @par Returns
 *    Nothing.
 */
void tcp_server_params_show(tcp_server_param_st *params);

#endif
