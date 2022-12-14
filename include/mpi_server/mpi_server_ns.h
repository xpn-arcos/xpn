
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

#ifndef _MPI_SERVER_NS_H_
#define _MPI_SERVER_NS_H_


      #include "all_system.h"
      #include "base/utils.h"


      /*
       *  API
       */

      int mpi_server_dns_publish   ( char * param_srv_name, char * dns_file, char * port_name ) ;
      int mpi_server_dns_unpublish ( char * dns_file ) ;
      int mpi_server_dns_lookup    ( char * param_srv_name, char * port_name ) ;


#endif

