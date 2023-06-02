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
 * @file nfi_nfs3.h
 * @brief Header file to 'TODO'.
 *
 * Header file to 'TODO'.
 *
 * @authors Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
 * @date  Jul 22, 2021
 * @bug No known bugs.
 */

#ifndef _NFI_NFS3_H
#define _NFI_NFS3_H

/************************************************
 *  ... Includes
 ***********************************************/
#include "all_system.h"
#include "base/path_misc.h"
#include "nfi/nfi.h"
#include "nfi/nfi_nfs3/nfs3.h"
#include "nfi/nfi_nfs3/nfi_nfs3_err.h"

/** @struct nfi_nfs3_server
 *  This is a struct
 *
 *  @var nfi_nfs3_server::cl
 *    A 'TODO'.
 *  @var nfi_nfs3_server::fh
 *    A 'TODO'.
 */
struct nfi_nfs3_server
{
	CLIENT *cl;
	fhandle3 fh;
};

/** @struct nfi_nfs3_fhandle
 *  This is a struct
 *
 *  @var nfi_nfs3_fhandle::eofdir
 *    A 'TODO'.
 *  @var nfi_nfs3_fhandle::cookie
 *    A 'TODO'.
 *  @var nfi_nfs3_fhandle::fh
 *    A 'TODO'.
 */
struct nfi_nfs3_fhandle
{
	int eofdir;
	cookieverf3 cookie;
	fhandle3 fh;
};

/************************************************************
 * PRIVATE FUNCTIONS TO USE NFS SERVERS			    *
 ************************************************************/
/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param nfs_att 'TODO'.
 * @param nfi_att 'TODO'.
 * @par Returns
 *    Nothing.
 */
void nfi_to_nfs3_attr(fattr3 *nfs_att, struct nfi_attr *nfi_att);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param nfi_att 'TODO'.
 * @param nfs_att 'TODO'.
 * @par Returns
 *    Nothing.
 */
void nfs3_to_nfi_attr(struct nfi_attr *nfi_att, fattr3 *nfs_att);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param nfi_inf 'TODO'.
 * @param nfs_inf 'TODO'.
 * @par Returns
 *    Nothing.
 */
void nfs3_to_nfi_info(struct nfi_info *nfi_inf, fsinfo3resok *nfs_inf);

/************************************************************
 * PUBLIC FUNCTIONS TO USE NFS SERVERS			    *
 ************************************************************/

/**
 * @brief Init NFS server.
 *
 * 'TODO'.
 *
 * @param url 'TODO'.
 * @param serv 'TODO'.
 * @param attr 'TODO'.
 * @return 'TODO'.
 */
int nfi_nfs3_init(char *url, struct nfi_server *serv, struct nfi_attr_server *attr);

/**
 * @brief Destroy NFS operations.
 *
 * 'TODO'.
 *
 * @param server 'TODO'.
 * @return 'TODO'.
 */
int nfi_nfs3_destroy(struct nfi_server *server);

/**
 * @brief Reconnect to the NFS server.
 *
 * 'TODO'.
 *
 * @param server 'TODO'.
 * @return 'TODO'.
 */
int nfi_nfs3_reconnect(struct nfi_server *server);

/**
 * @brief Disconnect to the server.
 *
 * 'TODO'.
 *
 * @param server 'TODO'.
 * @return 'TODO'.
 */
int nfi_nfs3_disconnect(struct nfi_server *server);

/************************************************************
 * GENERIC FUNCTIONS 										*
 ************************************************************/

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param server 'TODO'.
 * @param fh 'TODO'.
 * @param attr 'TODO'.
 * @return 'TODO'.
 */
int nfi_nfs3_getattr(struct nfi_server *server, struct nfi_fhandle *fh, struct nfi_attr *attr);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param server 'TODO'.
 * @param fh 'TODO'.
 * @param attr 'TODO'.
 * @return 'TODO'.
 */
int nfi_nfs3_setattr(struct nfi_server *server, struct nfi_fhandle *fh, struct nfi_attr *attr);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param server 'TODO'.
 * @param url 'TODO'.
 * @param fho 'TODO'.
 * @return 'TODO'.
 */
int nfi_nfs3_open(struct nfi_server *server, char *url, struct nfi_fhandle *fho);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param server 'TODO'.
 * @param fh 'TODO'.
 * @return 'TODO'.
 */
int nfi_nfs3_close(struct nfi_server *server, struct nfi_fhandle *fh);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param server 'TODO'.
 * @param fh 'TODO'.
 * @param buffer 'TODO'.
 * @param offset 'TODO'.
 * @param size 'TODO'.
 * @return 'TODO'.
 */
ssize_t nfi_nfs3_read(struct nfi_server *server, struct nfi_fhandle *fh, void *buffer, off_t offset, size_t size);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param server 'TODO'.
 * @param fh 'TODO'.
 * @param buffer 'TODO'.
 * @param offset 'TODO'.
 * @param size 'TODO'.
 * @return 'TODO'.
 */
ssize_t nfi_nfs3_write(struct nfi_server *server, struct nfi_fhandle *fh, void *buffer, off_t offset, size_t size);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param server 'TODO'.
 * @param url 'TODO'.
 * @param attr 'TODO'.
 * @param fh 'TODO'.
 * @return 'TODO'.
 */
int nfi_nfs3_create(struct nfi_server *server, char *url, struct nfi_attr *attr, struct nfi_fhandle *fh);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param server 'TODO'.
 * @param url 'TODO'.
 * @return 'TODO'.
 */
int nfi_nfs3_remove(struct nfi_server *server, char *url);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param server 'TODO'.
 * @param old_url 'TODO'.
 * @param new_url 'TODO'.
 * @return 'TODO'.
 */
int nfi_nfs3_rename(struct nfi_server *server, char *old_url, char *new_url);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param server 'TODO'.
 * @param url 'TODO'.
 * @param attr 'TODO'.
 * @param fh 'TODO'.
 * @return 'TODO'.
 */
int nfi_nfs3_mkdir(struct nfi_server *server, char *url, struct nfi_attr *attr, struct nfi_fhandle *fh);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param server 'TODO'.
 * @param url 'TODO'.
 * @return 'TODO'.
 */
int nfi_nfs3_rmdir(struct nfi_server *server, char *url);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param server 'TODO'.
 * @param url 'TODO'.
 * @param fho 'TODO'.
 * @return 'TODO'.
 */
int nfi_nfs3_opendir(struct nfi_server *server, char *url, struct nfi_fhandle *fho);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param server 'TODO'.
 * @param fhd 'TODO'.
 * @param entry 'TODO'.
 * @param type 'TODO'.
 * @return 'TODO'.
 */
int nfi_nfs3_readdir(struct nfi_server *server, struct nfi_fhandle *fhd, char *entry, unsigned char *type);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param server 'TODO'.
 * @param fh 'TODO'.
 * @return 'TODO'.
 */
int nfi_nfs3_closedir(struct nfi_server *server, struct nfi_fhandle *fh);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param server 'TODO'.
 * @param inf 'TODO'.
 * @return 'TODO'.
 */
int nfi_nfs3_statfs(struct nfi_server *server, struct nfi_info *inf);

#endif
