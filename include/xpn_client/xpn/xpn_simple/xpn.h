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
 * @file xpn.h
 * @brief Header file to 'TODO'.
 *
 * Header file to 'TODO'.
 *
 * @authors Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
 * @date  Jul 22, 2021
 * @bug No known bugs.
 */

#ifndef _XPN_H
#define _XPN_H

/************************************************
 *  ... Includes
 ***********************************************/
#include "nfi/nfi.h"
#include "nfi/nfi_worker.h"
#include "nfi/nfi_ops.h"
#include "nfi/nfi_lib.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define XPN_CONF "XPN_CONF"
#define XPN_HEADER_SIZE 8192

	/** @struct xpn_partition
	 *  Paralel struct partition.
	 *
	 *  @var xpn_partition::id
	 *    Id of partition.
	 *  @var xpn_partition::type
	 *    Type of file :NORMAL, RAID5,....
	 *  @var xpn_partition::name
	 *    Name of partition.
	 *  @var xpn_partition::block_size
	 *    Size of distribution used.
	 *  @var xpn_partition::size_threads
	 *    A 'TODO'.
	 *  @var xpn_partition::initialized
	 *    Whether it is already initialized or not.
	 *  @var xpn_partition::data_nserv
	 *    Number of server.
	 *  @var xpn_partition::data_serv
	 *    List of data servers in the partition.
	 *  @var xpn_partition::meta_nserv
	 *    Number of metadata_server.
	 *  @var xpn_partition::meta_serv
	 *    List of metadata servers in the partition.
	 */
	struct xpn_partition
	{
		int id;
		int type;
		char name[PATH_MAX];
		ssize_t block_size;
		ssize_t size_threads;

		int initialized;

		int data_nserv;
		struct nfi_server *data_serv;

		int meta_nserv;
		struct nfi_server *meta_serv;
	};

	/** @struct xpn_metadata
	 *  Metadata of xpn file.
	 *
	 *  @var xpn_metadata::id
	 *    Id of partition.
	 *  @var xpn_metadata::version
	 *    Number of version
	 *  @var xpn_metadata::type
	 *    Type of file: file or dir
	 *  @var xpn_metadata::block_size
	 *    Size of distribution used
	 *  @var xpn_metadata::size_threads
	 *   'TODO'
	 *  @var xpn_metadata::data_nserv
	 *    Number of servers
	 *  @var xpn_metadata::meta_nserv
	 *    Number of metadata servers
	 *  @var xpn_metadata::attr
	 *   'TODO'
	 *  @var xpn_metadata::type_policy
	 *    RAID0, RAID1
	 *  @var xpn_metadata::policy
	 *   'TODO'
	 *  @var xpn_metadata::policy_size
	 *   'TODO'
	 */
	struct xpn_metadata
	{
		int id;
		int version;
		int type;			/*  */
		ssize_t block_size; /*  */
		ssize_t size_threads;
		int data_nserv; /*  */
		int meta_nserv; /*  */
		struct nfi_attr attr;
		int type_policy; /*  */
		void *policy;
		int policy_size;
	};

	enum xpn_policy
	{
		POLICY_RAID0,
		POLICY_RAID1
	};

	/** @struct policy
	 *  'TODO'.
	 *
	 *  @var policy::first_node
	 *   'TODO'
	 */
	struct policy
	{
		int first_node;
	};

	/** @struct xpn_metadata_header
	 *  Metadata header of xpn file.
	 *
	 *  @var xpn_metadata_header::version
	 *   Number of version.
	 *  @var xpn_metadata_header::block_size
	 *   Size of distribution used.
	 *  @var xpn_metadata_header::data_nserv
	 *   Number of servers.
	 *  @var xpn_metadata_header::file_size
	 *   'TODO'
	 *  @var xpn_metadata_header::type_policy
	 *   RAID0, RAID1.
	 */
	struct xpn_metadata_header
	{
		int version;
		ssize_t block_size;
		int data_nserv;
		ssize_t file_size;
		int type_policy;
		// char    padding[512-(3*sizeof(int) - 2*sizeof(ssize_t))];   /* 484 = 512 bytes - 3*sizeof(int) - 2*sizeof(ssize_t) */
	};

#ifdef __cplusplus
}
#endif

#endif
