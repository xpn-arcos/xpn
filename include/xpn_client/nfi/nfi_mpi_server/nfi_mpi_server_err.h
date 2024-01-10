
  /*
   *  Copyright 2000-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
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

#ifndef _NFI_MPI_SERVER_ERR_H_
#define _NFI_MPI_SERVER_ERR_H_


 #ifdef  __cplusplus
    extern "C" {
 #endif

enum nfi_mpi_server_err
{
	MPI_SERVERERR_PARAM = 0,
	MPI_SERVERERR_MEMORY = 1,
	MPI_SERVERERR_URL = 2,
	MPI_SERVERERR_MNTCONNECTION = 3,
	MPI_SERVERERR_MOUNT = 4,
	MPI_SERVERERR_NFSCONNECTION = 5,		
	MPI_SERVERERR_GETATTR = 6,
	MPI_SERVERERR_LOOKUP = 7,
	MPI_SERVERERR_READ = 8,
	MPI_SERVERERR_WRITE = 9,
	MPI_SERVERERR_CREATE = 10,
	MPI_SERVERERR_REMOVE = 11,
	MPI_SERVERERR_MKDIR = 12,
	MPI_SERVERERR_READDIR = 13,
	MPI_SERVERERR_STATFS = 14,
	MPI_SERVERERR_NOTDIR = 15,
};

void mpi_server_err(int err);


 #ifdef  __cplusplus
     }
 #endif


#endif
