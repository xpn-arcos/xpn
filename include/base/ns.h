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
 * @file ns.h
 * @brief Header file to 'TODO'.
 *
 * Header file to 'TODO'.
 *
 * @authors Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
 * @date  Jul 22, 2021
 * @bug No known bugs.
 */

#ifndef _NS_H_
#define _NS_H_

/************************************************
 *  ... Includes
 ***********************************************/
#include "all_system.h" /// Library for 'TODO'
#include "base/utils.h" /// Library for 'TODO'

/************************************************
 *  ... ENVIROMENT VARIABLE: DNS SERVICE
 ***********************************************/
#ifndef MPI_SERVER_DNS_FILE_DEFAULT
#define MPI_SERVER_DNS_FILE_DEFAULT "/tmp/mpi_dns.txt"
#endif

#ifndef TCP_SERVER_DNS_FILE_DEFAULT
#define TCP_SERVER_DNS_FILE_DEFAULT "/tmp/tcp_dns.txt"
#endif

#ifndef MAX_TCP_SERVER_NODES
#define MAX_TCP_SERVER_NODES 256
#endif

#ifndef CONST_TEMP
#define CONST_TEMP 1024
#endif

/************************************************
 *  ... Functions
 ***********************************************/

/**
 * @brief Get hostname.
 *
 * 'TODO'.
 *
 * @param srv_name 'TODO'.
 * @par Returns
 *      Nothing.
 */
void ns_get_hostname(char *srv_name);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 * 
 * @par Parameters
 *    None.
 * @return 'TODO'.
 */
char *ns_get_host_ip(void);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param dns_file 'TODO'.
 * @param protocol 'TODO'.
 * @param param_srv_name 'TODO'.
 * @param srv_ip 'TODO'.
 * @param port_name 'TODO'.
 * @return 'TODO'.
 */
int ns_publish(char *dns_file, char *protocol, char *param_srv_name, char *srv_ip, char *port_name);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param dns_file 'TODO'.
 * @param protocol 'TODO'.
 * @param param_srv_name 'TODO'.
 * @return 'TODO'.
 */
int ns_unpublish(char *dns_file, char *protocol, char *param_srv_name);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param protocol 'TODO'.
 * @param param_srv_name 'TODO'.
 * @param srv_ip 'TODO'.
 * @param port_name 'TODO'.
 * @return 'TODO'.
 */
int ns_lookup(char *protocol, char *param_srv_name, char *srv_ip, char *port_name);

#endif
