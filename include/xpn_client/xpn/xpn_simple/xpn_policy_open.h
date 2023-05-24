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
 * @file xpn_policy_open.h
 * @brief Header file to 'TODO'.
 *
 * Header file to 'TODO'.
 *
 * @authors Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
 * @date  Jul 22, 2021
 * @bug No known bugs.
 */

#ifndef _XPN_POLICY_OPEN_H
#define _XPN_POLICY_OPEN_H

/************************************************
 *  ... Includes
 ***********************************************/
#include "xpn_file.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define XPN_META_SIZE (4 * KB)

	enum xpn_work
	{
		no_xpn_op = -1,
		op_xpn_getattr = 0,
		op_xpn_setattr = 1,
		op_xpn_open = 2,
		op_xpn_creat = 3,
		op_xpn_close = 4,
		op_xpn_remove = 5,
		op_xpn_rename = 6,
		op_xpn_read = 7,
		op_xpn_write = 8,
		op_xpn_mkdir = 9,
		op_xpn_rmdir = 10,
		op_xpn_opendir = 11,
		op_xpn_readdir = 12,
		op_xpn_closedir = 13,
		op_xpn_statfs = 14,
		op_xpn_preload = 15,
		op_xpn_flush = 16,
	};

	/************************************************
	 *  ... Functions
	 ***********************************************/

	/**
	 * @brief 'TODO'.
	 *
	 * 'TODO'.
	 *
	 * @param p 'TODO'.
	 * @return 'TODO'.
	 */
	ssize_t XpnGetSizeThreads(struct xpn_partition *p);

	/**
	 * @brief 'TODO'.
	 *
	 * 'TODO'.
	 *
	 * @param serv 'TODO'.
	 * @param abs_path 'TODO'.
	 * @param url_serv 'TODO'.
	 * @par Returns
	 *    Nothing.
	 */
	void XpnGetURLServer(struct nfi_server *serv, char *abs_path, char *url_serv);

	/**
	 * @brief 'TODO'.
	 *
	 * 'TODO'.
	 *
	 * @param op 'TODO'.
	 * @param pd 'TODO'.
	 * @param abs_path 'TODO'.
	 * @param fd 'TODO'.
	 * @param servers 'TODO'.
	 * @param type 'TODO'.
	 * @return 'TODO'.
	 */
	int XpnGetServers(int op, int pd, char *abs_path, int fd, struct nfi_server ***servers, int type);

	/**
	 * @brief 'TODO'.
	 *
	 * 'TODO'.
	 *
	 * @param mdata 'TODO'.
	 * @param pos 'TODO'.
	 * @return 'TODO'.
	 */
	int XpnGetMetadataPos(struct xpn_metadata *mdata, int pos);

	/**
	 * @brief 'TODO'.
	 *
	 * 'TODO'.
	 *
	 * @param mdata 'TODO'.
	 * @param pd 'TODO'.
	 * @param path 'TODO'.
	 * @return 'TODO'.
	 */
	int XpnCreateMetadata(struct xpn_metadata *mdata, int pd, char *path);

	/**
	 * @brief 'TODO'.
	 *
	 * 'TODO'.
	 *
	 * @param mdata 'TODO'.
	 * @param nserv 'TODO'.
	 * @param servers 'TODO'.
	 * @param fh 'TODO'.
	 * @param path 'TODO'.
	 * @param pd 'TODO'.
	 * @return 'TODO'.
	 */
	int XpnReadMetadata(struct xpn_metadata *mdata, int nserv, struct nfi_server **servers, struct xpn_fh *fh, char *path, int pd);

	/**
	 * @brief 'TODO'.
	 *
	 * 'TODO'.
	 *
	 * @param mdata 'TODO'.
	 * @param nserv 'TODO'.
	 * @param servers 'TODO'.
	 * @param fh 'TODO'.
	 * @param path 'TODO'.
	 * @return 'TODO'.
	 */
	int XpnUpdateMetadata(struct xpn_metadata *mdata, int nserv, struct nfi_server **servers, struct xpn_fh *fh, char *path);

	/**
	 * @brief 'TODO'.
	 *
	 * 'TODO'.
	 *
	 * @param mdata 'TODO'.
	 * @param fh 'TODO'.
	 * @param servers 'TODO'.
	 * @param path 'TODO'.
	 * @return 'TODO'.
	 */
	int XpnGetFh(struct xpn_metadata *mdata, struct nfi_fhandle **fh, struct nfi_server *servers, char *path);

	/**
	 * @brief 'TODO'.
	 *
	 * 'TODO'.
	 *
	 * @param fd 'TODO'.
	 * @param st 'TODO'.
	 * @return 'TODO'.
	 */
	int XpnGetAtribFd(int fd, struct stat *st);

	/**
	 * @brief 'TODO'.
	 *
	 * 'TODO'.
	 *
	 * @param path 'TODO'.
	 * @param st 'TODO'.
	 * @return 'TODO'.
	 */
	int XpnGetAtribPath(char *path, struct stat *st);

#ifdef __cplusplus
}
#endif

#endif
