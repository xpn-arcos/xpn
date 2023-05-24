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
 * @file nfi_lib.h
 * @brief Header file to 'TODO'.
 *
 * Header file to 'TODO'.
 *
 * @authors Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
 * @date  Jul 22, 2021
 * @bug No known bugs.
 */

#ifndef _NFI_LIB_H_
#define _NFI_LIB_H_

/************************************************
 *  ... Includes
 ***********************************************/
#include "nfi.h"
#include "nfi_worker.h"
#include "base/path_misc.h"

/* protocols */
#include "nfi_local.h"

// BEGIN OF ENABLE_MODULE BLOCK. Do not remove this line. //
// BEGIN OF ENABLE_NFS BLOCK. Do not remove this line. //
#ifdef ENABLE_NFS
#include "nfi_nfs.h"
#endif
// END OF ENABLE_NFS BLOCK. Do not remove this line. //
// BEGIN OF ENABLE_NFS3 BLOCK. Do not remove this line. //
#ifdef ENABLE_NFS3
#include "nfi_nfs3.h"
#endif
// END OF ENABLE_NFS3 BLOCK. Do not remove this line. //
// BEGIN OF ENABLE_TCP_SERVER BLOCK. Do not remove this line. //
#ifdef ENABLE_TCP_SERVER
#include "nfi_tcp_server.h"
#endif
// END OF ENABLE_TCP_SERVER BLOCK. Do not remove this line. //
// BEGIN OF ENABLE_MPI_SERVER BLOCK. Do not remove this line. //
#ifdef ENABLE_MPI_SERVER
#include "nfi_mpi_server.h"
#endif
// END OF ENABLE_MPI_SERVER BLOCK. Do not remove this line. //
// END OF ENABLE_MODULE BLOCK. Do not remove this line. //

#endif
