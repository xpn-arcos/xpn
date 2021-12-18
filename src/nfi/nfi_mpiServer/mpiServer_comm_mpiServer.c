
/*
 *  Copyright 2020-2022 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
 *
 *  This file is part of mpiServer.
 *
 *  mpiServer is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  mpiServer is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with mpiServer.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


   /* ... Include / Inclusion ........................................... */

      #include "mpiServer_comm.h"


   /* ... Functions / Funciones ......................................... */

      int     mpiServer_comm_init      ( mpiServer_param_st *params )
      {
              int ret, provided ;
      	      char srv_name[1024] ;
              MPI_Info info ;
      
              debug_info("[COMM] begin mpiServer_comm_init(...)\n") ;
      
              // MPI_Init
              ret = MPI_Init_thread(&(params->argc), &(params->argv), MPI_THREAD_MULTIPLE, &provided) ;
              if (MPI_SUCCESS != ret) {
                  debug_error("Server[%d]: MPI_Init fails :-(", -1) ;
                  return -1 ;
              }
      
              // params->rank = comm_rank()
              ret = MPI_Comm_rank(MPI_COMM_WORLD, &(params->rank)) ;
              if (MPI_SUCCESS != ret) {
                  debug_error("Server[%d]: MPI_Comm_rank fails :-(", params->rank) ;
                  return -1 ;
              }
      
              // params->size = comm_size()
              ret = MPI_Comm_size(MPI_COMM_WORLD, &(params->size)) ;
              if (MPI_SUCCESS != ret) {
                  debug_error("Server[%d]: MPI_Comm_size fails :-(", params->rank) ;
                  return -1 ;
              }
      
              // Open server port...
              ret = MPI_Open_port(MPI_INFO_NULL, params->port_name) ;
              if (MPI_SUCCESS != ret) {
                  debug_error("Server[%d]: MPI_Open_port fails :-(", params->rank) ;
                  return -1 ;
              }
      
              // Publish port name
              MPI_Info_create(&info) ;
              MPI_Info_set(info, "ompi_global_scope", "true") ;
              sprintf(srv_name, "mpiServer.%d", params->rank) ;
      
              ret = MPI_Publish_name(srv_name, info, params->port_name) ;
              if (MPI_SUCCESS != ret) {
                  debug_error("Server[%d]: MPI_Publish_name fails :-(", params->rank) ;
                  return -1 ;
              }
      
              debug_info("[COMM] server %d available at %s\n", params->rank, params->port_name) ;
              debug_info("[COMM] server %d accepting...\n",    params->rank) ;
              debug_info("[COMM] end mpiServer_comm_init(...)\n") ;
      
      	      // Return OK
      	      return 1 ;
      }
      
      int mpiServer_comm_destroy ( mpiServer_param_st *params )
      {
              int ret ;
      
              debug_info("[COMM] begin mpiServer_comm_destroy(...)\n") ;
      
              // Close port
              MPI_Close_port(params->port_name) ;
      
              // Unpublish port name
              ret = MPI_Unpublish_name(params->srv_name, MPI_INFO_NULL, params->port_name) ;
              if (MPI_SUCCESS != ret) {
                  debug_error("Server[%d]: MPI_Unpublish_name fails :-(", params->rank) ;
                  return -1 ;
              }
      
              // Finalize
              ret = MPI_Finalize() ;
              if (MPI_SUCCESS != ret) {
                  debug_error("Server[%d]: MPI_Finalize fails :-(", params->rank) ;
                  return -1 ;
              }
      
              debug_info("[COMM] end mpiServer_comm_destroy(...)\n") ;
      
              // Return OK
              return 1 ;
      }
      
      int mpiServer_comm_accept ( mpiServer_param_st *params )
      {
      	      int ret ;
      
              debug_info("[COMM] begin mpiServer_accept_comm(...)\n") ;
      
              // Accept
              ret = MPI_Comm_accept(params->port_name, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &(params->client)) ;
              if (MPI_SUCCESS != ret) {
                  debug_error("Server[%d]: MPI_Comm_accept fails :-(", params->rank) ;
                  return -1 ;
              }
      
              debug_info("[COMM] end mpiServer_accept_comm(...)\n") ;
      
              // Return client MPI_Comm
      	      return (int)(params->client) ;
      }
      
      int mpiServer_comm_close ( mpiServer_param_st *params )
      {
              int ret ;
      
              // Disconnect
              ret = MPI_Comm_disconnect(&(params->client)) ;
              if (MPI_SUCCESS != ret) {
                  debug_error("Server[%d]: MPI_Comm_disconnect fails :-(", params->rank) ;
                  return -1 ;
              }
      
              // Return OK
              return 1 ;
      }
      
      ssize_t mpiServer_comm_writedata ( mpiServer_param_st *params, int fd, char *data, ssize_t size )
      {
      	      int ret ;
      
      	      debug_info("[COMM] server: begin comm_writedata(...)\n") ;
      
      	      // Check params
      	      if (size == 0){
      	          return 0;
      	      }
      	      if (size < 0){
                  debug_warning("Server[%d]: size < 0", params->rank) ;
      	          return -1;
      	      }
      	      if (NULL == params) {
                  debug_warning("Server[%d]: NULL params", params->rank) ;
      	          return -1;
      	      }
      	
              // Send message
              ret = MPI_Send(data, size, MPI_CHAR, 0, 0, (MPI_Comm)fd) ;
              if (MPI_SUCCESS != ret) {
                  debug_warning("Server[%d]: MPI_Recv fails :-(", params->rank) ;
              }
      
      	      debug_info("[COMM] server: end comm_writedata(...)\n") ;
      
              // Return bytes written
      	      return size;
      }
      
      ssize_t mpiServer_comm_readdata ( mpiServer_param_st *params, int fd, char *data, ssize_t size )
      {
      	      int ret ;
      	      MPI_Status status ;
      
      	      debug_info("[COMM] server: begin comm_readdata(...)\n") ;
      
      	      // Check params
      	      if (size == 0) {
      	          return  0;
      	      }
      	      if (size < 0){
                  debug_warning("Server[%d]: size < 0", params->rank) ;
      	          return  -1;
      	      }
      	      if (NULL == params) {
                  debug_warning("Server[%d]: NULL params", params->rank) ;
      	          return -1;
      	      }
      	
              // Get message
      	      ret = MPI_Recv(data, size, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, (MPI_Comm)fd, &status);
              if (MPI_SUCCESS != ret) {
                  debug_warning("Server[%d]: MPI_Recv fails :-(", params->rank) ;
              }
      
      	      debug_info("[COMM] server: end comm_readdata(...)\n") ;
      
              // Return bytes read
      	      return size;
      }


   /* ................................................................... */

