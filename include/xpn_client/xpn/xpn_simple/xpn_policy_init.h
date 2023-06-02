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
 * @file xpn_policy_init.h
 * @brief Header file to 'TODO'.
 *
 * Header file to 'TODO'.
 *
 * @authors Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
 * @date  Jul 22, 2021
 * @bug No known bugs.
 */

#ifndef _XPN_POLICY_INIT_H
#define _XPN_POLICY_INIT_H

#ifdef ENABLE_MXML
#include "mxml.h"
#endif

/************************************************
 *  ... Includes
 ***********************************************/
#include "xpn_err.h"
#include "xpn.h"
#include "xpn_init.h"

#ifdef __cplusplus
extern "C"
{
#endif

	enum type_conf_connection
	{
		CONF_FILE = 0,
		CONF_RPC = 1,
		CONF_DESC = 2,
		CONF_MXML = 3,
	};

#define XPN_DATA_SERVER 0
#define XPN_META_SERVER 1

#ifdef ENABLE_MXML
#define CONF_DEFAULT CONF_MXML
#else
#define CONF_DEFAULT CONF_FILE
#endif

#ifdef ENABLE_MXML

#define XML_TAG_ELEM_PARTITION "partition"
#define XML_TAG_ELEM_DATANODE "data_node"
#define XML_TAG_ELEM_METADATANODE "metadata_node"

#define XML_TAG_ATTR_NAME "name"
#define XML_TAG_ATTR_TYPE "type"
#define XML_TAG_ATTR_BLOCKSIZE "bsize"
#define XML_TAG_ATTR_THREADS "threads"
#define XML_TAG_ATTR_TH_THRESHOLD "th_limit"
#define XML_TAG_ATTR_URL "url"
#define XML_TAG_ATTR_ID "id"

#define XML_DEFAULT_ATTR_TYPE "NORMAL"
#define XML_DEFAULT_ATTR_BLOCKSIZE "256K"
#define XML_DEFAULT_ATTR_THREADS "no"
#define XML_DEFAULT_ATTR_TH_THRESHOLD "-1"

	struct xml_connect_st
	{
		FILE *conf_f;
		FILE *profile_f;

		char conf_n[255];
		char profile_n[255];

		mxml_node_t *profile_tree;
		mxml_node_t *conf_tree;

		mxml_node_t *conf_partition_node;
		mxml_node_t *conf_meta_node;
		mxml_node_t *conf_data_node;

		mxml_node_t *profile_partition_node;
		mxml_node_t *profile_meta_node;
		mxml_node_t *profile_data_node;
	};
#endif

	struct conf_connect_st
	{
		int type;
		union
		{
			FILE *f;
#ifdef ENABLE_NFS3
			CLIENT *cl;
#endif
			int fd;
#ifdef ENABLE_MXML
			struct xml_connect_st xml;
#endif
			/* more types */
		} connect_u;
	};

#define XPN_CONF "XPN_CONF"

#ifdef ENABLE_MXML
#define XPN_PROFILE "XPN_PROFILE"
#define XPN_CONF_DEFAULT "/etc/xpn/xpn.conf.xml"
#define XPN_PROFILE_DEFAULT "/etc/xpn/profile.conf.xml"
#else
#define XPN_CONF_DEFAULT "/etc/xpn/xpn.conf"
#endif

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
	 * @return 'TODO'.
	 */
	struct conf_connect_st *xpn_partition_open(void);

	/**
	 * @brief 'TODO'.
	 *
	 * 'TODO'.
	 *
	 * @param fconf 'TODO'.
	 * @par Returns
	 *    Nothing.
	 */
	void xpn_partition_close(struct conf_connect_st *fconf);

	/**
	 * @brief 'TODO'.
	 *
	 * 'TODO'.
	 *
	 * @param fconf 'TODO'.
	 * @param name 'TODO'.
	 * @return 'TODO'.
	 */
	int xpn_get_next_partition(struct conf_connect_st *fconf, char *name);

	/**
	 * @brief 'TODO'.
	 *
	 * 'TODO'.
	 *
	 * @param fconf 'TODO'.
	 * @param name 'TODO'.
	 * @return 'TODO'.
	 */
	int xpn_get_id_partition(struct conf_connect_st *fconf, char *name);

	/**
	 * @brief 'TODO'.
	 *
	 * 'TODO'.
	 *
	 * @param fconf 'TODO'.
	 * @param part 'TODO'.
	 * @return 'TODO'.
	 */
	int xpn_get_info_partition(struct conf_connect_st *fconf, struct xpn_partition *part);

	/**
	 * @brief 'TODO'.
	 *
	 * 'TODO'.
	 *
	 * @param fconf 'TODO'.
	 * @param part 'TODO'.
	 * @param type 'TODO'.
	 * @return 'TODO'.
	 */
	int xpn_get_num_servers_partition(struct conf_connect_st *fconf, struct xpn_partition *part, int type);

	/**
	 * @brief 'TODO'.
	 *
	 * 'TODO'.
	 *
	 * @param pd 'TODO'.
	 * @return 'TODO'.
	 */
	struct xpn_partition *xpn_search_part(int pd);

	/**
	 * @brief 'TODO'.
	 *
	 * 'TODO'.
	 *
	 * @param fconf 'TODO'.
	 * @param part 'TODO'.
	 * @param serv 'TODO'.
	 * @param type 'TODO'.
	 * @return 'TODO'.
	 */
	int xpn_get_server(struct conf_connect_st *fconf, struct xpn_partition *part, struct nfi_server *serv, int type);

	/**
	 * @brief 'TODO'.
	 *
	 * 'TODO'.
	 *
	 * @param path 'TODO'.
	 * @return 'TODO'.
	 */
	int xpn_get_partition(char *path);

#ifdef __cplusplus
}
#endif

#endif
