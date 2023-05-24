/*
 *  Copyright 2000-2023 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
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
 * @file tcp_server_comm.h
 * @brief Header file to 'TODO'.
 *
 * Header file to 'TODO'.
 *
 * @authors Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
 * @date  Jul 22, 2021
 * @bug No known bugs.
 */

#ifndef _TCP_SERVER_COMM_H_
#define _TCP_SERVER_COMM_H_

/************************************************
 *  ... Includes
 ***********************************************/
#include "all_system.h"
#include "tcp_server_params.h"
#include "base/utils.h"
#include "base/ns.h"
#include "base/ns.h"
#include "tcp_server_ops.h"

/************************************************
 *  ... Functions
 ***********************************************/

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param params 'TODO'.
 * @return 'TODO'.
 */
int tcpClient_comm_init(tcpClient_param_st *params);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param params 'TODO'.
 * @return 'TODO'.
 */
int tcpClient_comm_destroy(tcpClient_param_st *params);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param params 'TODO'.
 * @return 'TODO'.
 */
int tcpClient_comm_connect(tcpClient_param_st *params);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param params 'TODO'.
 * @return 'TODO'.
 */
int tcpClient_comm_disconnect(tcpClient_param_st *params);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param params 'TODO'.
 * @return 'TODO'.
 */
int tcpClient_comm_locality(tcpClient_param_st *params);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param fd 'TODO'.
 * @param data 'TODO'.
 * @param size 'TODO'.
 * @param msg_id 'TODO'.
 * @return 'TODO'.
 */
ssize_t tcpClient_write_operation(int fd, char *data, ssize_t size, char *msg_id);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param fd 'TODO'.
 * @param data 'TODO'.
 * @param size 'TODO'.
 * @param msg_id 'TODO'.
 * @return 'TODO'.
 */
ssize_t tcpClient_write_data(int fd, char *data, ssize_t size, char *msg_id);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param fd 'TODO'.
 * @param data 'TODO'.
 * @param size 'TODO'.
 * @param msg_id 'TODO'.
 * @return 'TODO'.
 */
ssize_t tcpClient_read_data(int fd, char *data, ssize_t size, char *msg_id);

#endif
