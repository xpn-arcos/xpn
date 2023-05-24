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
 * @file nfs_clnt.c
 * @brief File to 'TODO'.
 *
 * File to 'TODO'.
 *
 * @authors Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
 * @date  Jul 22, 2021
 * @bug No known bugs.
 */

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



#ifdef WIN32

/************************************************
 *  ... Includes
 ***********************************************/
#include "nfi_nfs.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = {25, 0};

enum clnt_stat mountproc_null_1(argp, res, clnt) void *argp;
void *res;
CLIENT *clnt;
{
	return (clnt_call(clnt, MOUNTPROC_NULL, xdr_void, argp, xdr_void, (char *)res, TIMEOUT));
}

enum clnt_stat mountproc_mnt_1(argp, res, clnt)
dirpath *argp;
fhstatus *res;
CLIENT *clnt;
{
	return (clnt_call(clnt, MOUNTPROC_MNT, xdr_dirpath, argp, xdr_fhstatus, (char *)res, TIMEOUT));
}

enum clnt_stat mountproc_dump_1(argp, res, clnt) void *argp;
mountlist *res;
CLIENT *clnt;
{
	return (clnt_call(clnt, MOUNTPROC_DUMP, xdr_void, argp, xdr_mountlist, (char *)res, TIMEOUT));
}

enum clnt_stat
mountproc_umnt_1(argp, res, clnt)
dirpath *argp;
void *res;
CLIENT *clnt;
{
	return (clnt_call(clnt, MOUNTPROC_UMNT, xdr_dirpath, argp, xdr_void, (char *)res, TIMEOUT));
}

enum clnt_stat
	mountproc_umntall_1(argp, res, clnt) void *argp;
void *res;
CLIENT *clnt;
{
	return (clnt_call(clnt, MOUNTPROC_UMNTALL, xdr_void, argp, xdr_void, (char *)res, TIMEOUT));
}

enum clnt_stat
	mountproc_export_1(argp, res, clnt) void *argp;
exports *res;
CLIENT *clnt;
{
	return (clnt_call(clnt, MOUNTPROC_EXPORT, xdr_void, argp, xdr_exports, res, TIMEOUT));
}

enum clnt_stat
	nfsproc_null_2(argp, res, clnt) void *argp;
void *res;
CLIENT *clnt;
{
	return (clnt_call(clnt, NFSPROC_NULL, xdr_void, argp, xdr_void, (char *)res, TIMEOUT));
}

enum clnt_stat
	nfsproc_getattr_2(argp, res, clnt) void *argp;
attrstat *res;
CLIENT *clnt;
{
	return (clnt_call(clnt, NFSPROC_GETATTR, xdr_fhandle, argp, xdr_attrstat, res, TIMEOUT));
}

enum clnt_stat
nfsproc_setattr_2(argp, res, clnt)
sattrargs *argp;
attrstat *res;
CLIENT *clnt;
{
	return (clnt_call(clnt, NFSPROC_SETATTR, xdr_sattrargs, argp, xdr_attrstat, res, TIMEOUT));
}

enum clnt_stat
	nfsproc_root_2(argp, res, clnt) void *argp;
void *res;
CLIENT *clnt;
{

	return (clnt_call(clnt, NFSPROC_ROOT, xdr_void, argp, xdr_void, (char *)res, TIMEOUT));
}

enum clnt_stat
nfsproc_lookup_2(argp, res, clnt)
diropargs *argp;
diropres *res;
CLIENT *clnt;
{

	return (clnt_call(clnt, NFSPROC_LOOKUP, xdr_diropargs, argp, xdr_diropres, res, TIMEOUT));
}

enum clnt_stat
	nfsproc_readlink_2(argp, res, clnt) void *argp;
readlinkres *res;
CLIENT *clnt;
{

	return (clnt_call(clnt, NFSPROC_READLINK, xdr_fhandle, argp, xdr_readlinkres, res, TIMEOUT));
}

enum clnt_stat
nfsproc_read_2(argp, res, clnt)
readargs *argp;
readres *res;
CLIENT *clnt;
{

	return (clnt_call(clnt, NFSPROC_READ, xdr_readargs, argp, xdr_readres, res, TIMEOUT));
}

enum clnt_stat
	nfsproc_writecache_2(argp, res, clnt) void *argp;
void *res;
CLIENT *clnt;
{

	return (clnt_call(clnt, NFSPROC_WRITECACHE, xdr_void, argp, xdr_void, (char *)res, TIMEOUT));
}

enum clnt_stat
nfsproc_write_2(argp, res, clnt)
writeargs *argp;
attrstat *res;
CLIENT *clnt;
{

	return (clnt_call(clnt, NFSPROC_WRITE, xdr_writeargs, argp, xdr_attrstat, res, TIMEOUT));
}

enum clnt_stat
nfsproc_create_2(argp, res, clnt)
createargs *argp;
diropres *res;
CLIENT *clnt;
{
	return (clnt_call(clnt, NFSPROC_CREATE, xdr_createargs, argp, xdr_diropres, res, TIMEOUT));
}

enum clnt_stat
nfsproc_remove_2(argp, res, clnt)
diropargs *argp;
nfs_stat *res;
CLIENT *clnt;
{

	return (clnt_call(clnt, NFSPROC_REMOVE, xdr_diropargs, argp, xdr_nfs_stat, res, TIMEOUT));
}

enum clnt_stat
nfsproc_rename_2(argp, res, clnt)
renameargs *argp;
nfs_stat *res;
CLIENT *clnt;
{

	return (clnt_call(clnt, NFSPROC_RENAME, xdr_renameargs, argp, xdr_nfs_stat, res, TIMEOUT));
}

enum clnt_stat
nfsproc_link_2(argp, res, clnt)
linkargs *argp;
nfs_stat *res;
CLIENT *clnt;
{
	return (clnt_call(clnt, NFSPROC_LINK, xdr_linkargs, argp, xdr_nfs_stat, res, TIMEOUT));
}

enum clnt_stat
nfsproc_symlink_2(argp, res, clnt)
symlinkargs *argp;
nfs_stat *res;
CLIENT *clnt;
{

	return (clnt_call(clnt, NFSPROC_SYMLINK, xdr_symlinkargs, argp, xdr_nfs_stat, res, TIMEOUT));
}

enum clnt_stat
nfsproc_mkdir_2(argp, res, clnt)
createargs *argp;
diropres *res;
CLIENT *clnt;
{

	return (clnt_call(clnt, NFSPROC_MKDIR, xdr_createargs, argp, xdr_diropres, res, TIMEOUT));
}

enum clnt_stat
nfsproc_rmdir_2(argp, res, clnt)
diropargs *argp;
nfs_stat *res;
CLIENT *clnt;
{
	return (clnt_call(clnt, NFSPROC_RMDIR, xdr_diropargs, argp, xdr_nfs_stat, res, TIMEOUT));
}

enum clnt_stat
nfsproc_readdir_2(argp, res, clnt)
readdirargs *argp;
readdirres *res;
CLIENT *clnt;
{
	return (clnt_call(clnt, NFSPROC_READDIR, xdr_readdirargs, argp, xdr_readdirres, res, TIMEOUT));
}

enum clnt_stat
nfsproc_statfs_2(argp, res, clnt)
char *argp;
statfsres *res;
CLIENT *clnt;
{
	return (clnt_call(clnt, NFSPROC_STATFS, xdr_fhandle, argp, xdr_statfsres, res, TIMEOUT));
}

#endif

#ifdef LINUX

/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "nfi_nfs.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = {25, 0};

enum clnt_stat
mountproc_null_1(void *argp, void *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, MOUNTPROC_NULL,
					  (xdrproc_t)xdr_void, (caddr_t)argp,
					  (xdrproc_t)xdr_void, (caddr_t)clnt_res,
					  TIMEOUT));
}

enum clnt_stat
mountproc_mnt_1(nfs_dirpath *argp, fhstatus *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, MOUNTPROC_MNT,
					  (xdrproc_t)xdr_dirpath, (caddr_t)argp,
					  (xdrproc_t)xdr_fhstatus, (caddr_t)clnt_res,
					  TIMEOUT));
}

enum clnt_stat
mountproc_dump_1(void *argp, mountlist *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, MOUNTPROC_DUMP,
					  (xdrproc_t)xdr_void, (caddr_t)argp,
					  (xdrproc_t)xdr_mountlist, (caddr_t)clnt_res,
					  TIMEOUT));
}

enum clnt_stat
mountproc_umnt_1(nfs_dirpath *argp, void *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, MOUNTPROC_UMNT,
					  (xdrproc_t)xdr_dirpath, (caddr_t)argp,
					  (xdrproc_t)xdr_void, (caddr_t)clnt_res,
					  TIMEOUT));
}

enum clnt_stat
mountproc_umntall_1(void *argp, void *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, MOUNTPROC_UMNTALL,
					  (xdrproc_t)xdr_void, (caddr_t)argp,
					  (xdrproc_t)xdr_void, (caddr_t)clnt_res,
					  TIMEOUT));
}

enum clnt_stat
mountproc_export_1(void *argp, exports *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, MOUNTPROC_EXPORT,
					  (xdrproc_t)xdr_void, (caddr_t)argp,
					  (xdrproc_t)xdr_exports, (caddr_t)clnt_res,
					  TIMEOUT));
}

enum clnt_stat
nfsproc_null_2(void *argp, void *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, NFSPROC_NULL,
					  (xdrproc_t)xdr_void, (caddr_t)argp,
					  (xdrproc_t)xdr_void, (caddr_t)clnt_res,
					  TIMEOUT));
}

enum clnt_stat
nfsproc_getattr_2(char *argp, attrstat *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, NFSPROC_GETATTR,
					  (xdrproc_t)xdr_fhandle, (caddr_t)argp,
					  (xdrproc_t)xdr_attrstat, (caddr_t)clnt_res,
					  TIMEOUT));
}

enum clnt_stat
nfsproc_setattr_2(sattrargs *argp, attrstat *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, NFSPROC_SETATTR,
					  (xdrproc_t)xdr_sattrargs, (caddr_t)argp,
					  (xdrproc_t)xdr_attrstat, (caddr_t)clnt_res,
					  TIMEOUT));
}

enum clnt_stat
nfsproc_root_2(void *argp, void *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, NFSPROC_ROOT,
					  (xdrproc_t)xdr_void, (caddr_t)argp,
					  (xdrproc_t)xdr_void, (caddr_t)clnt_res,
					  TIMEOUT));
}

enum clnt_stat
nfsproc_lookup_2(diropargs *argp, diropres *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, NFSPROC_LOOKUP,
					  (xdrproc_t)xdr_diropargs, (caddr_t)argp,
					  (xdrproc_t)xdr_diropres, (caddr_t)clnt_res,
					  TIMEOUT));
}

enum clnt_stat
nfsproc_readlink_2(char *argp, readlinkres *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, NFSPROC_READLINK,
					  (xdrproc_t)xdr_fhandle, (caddr_t)argp,
					  (xdrproc_t)xdr_readlinkres, (caddr_t)clnt_res,
					  TIMEOUT));
}

enum clnt_stat
nfsproc_read_2(readargs *argp, readres *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, NFSPROC_READ,
					  (xdrproc_t)xdr_readargs, (caddr_t)argp,
					  (xdrproc_t)xdr_readres, (caddr_t)clnt_res,
					  TIMEOUT));
}

enum clnt_stat
nfsproc_writecache_2(void *argp, void *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, NFSPROC_WRITECACHE,
					  (xdrproc_t)xdr_void, (caddr_t)argp,
					  (xdrproc_t)xdr_void, (caddr_t)clnt_res,
					  TIMEOUT));
}

enum clnt_stat
nfsproc_write_2(writeargs *argp, attrstat *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, NFSPROC_WRITE,
					  (xdrproc_t)xdr_writeargs, (caddr_t)argp,
					  (xdrproc_t)xdr_attrstat, (caddr_t)clnt_res,
					  TIMEOUT));
}

enum clnt_stat
nfsproc_create_2(createargs *argp, diropres *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, NFSPROC_CREATE,
					  (xdrproc_t)xdr_createargs, (caddr_t)argp,
					  (xdrproc_t)xdr_diropres, (caddr_t)clnt_res,
					  TIMEOUT));
}

enum clnt_stat
nfsproc_remove_2(diropargs *argp, nfs_stat *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, NFSPROC_REMOVE,
					  (xdrproc_t)xdr_diropargs, (caddr_t)argp,
					  (xdrproc_t)xdr_nfs_stat, (caddr_t)clnt_res,
					  TIMEOUT));
}

enum clnt_stat
nfsproc_rename_2(renameargs *argp, nfs_stat *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, NFSPROC_RENAME,
					  (xdrproc_t)xdr_renameargs, (caddr_t)argp,
					  (xdrproc_t)xdr_nfs_stat, (caddr_t)clnt_res,
					  TIMEOUT));
}

enum clnt_stat
nfsproc_link_2(linkargs *argp, nfs_stat *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, NFSPROC_LINK,
					  (xdrproc_t)xdr_linkargs, (caddr_t)argp,
					  (xdrproc_t)xdr_nfs_stat, (caddr_t)clnt_res,
					  TIMEOUT));
}

enum clnt_stat
nfsproc_symlink_2(symlinkargs *argp, nfs_stat *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, NFSPROC_SYMLINK,
					  (xdrproc_t)xdr_symlinkargs, (caddr_t)argp,
					  (xdrproc_t)xdr_nfs_stat, (caddr_t)clnt_res,
					  TIMEOUT));
}

enum clnt_stat
nfsproc_mkdir_2(createargs *argp, diropres *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, NFSPROC_MKDIR,
					  (xdrproc_t)xdr_createargs, (caddr_t)argp,
					  (xdrproc_t)xdr_diropres, (caddr_t)clnt_res,
					  TIMEOUT));
}

enum clnt_stat
nfsproc_rmdir_2(diropargs *argp, nfs_stat *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, NFSPROC_RMDIR,
					  (xdrproc_t)xdr_diropargs, (caddr_t)argp,
					  (xdrproc_t)xdr_nfs_stat, (caddr_t)clnt_res,
					  TIMEOUT));
}

enum clnt_stat
nfsproc_readdir_2(readdirargs *argp, readdirres *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, NFSPROC_READDIR,
					  (xdrproc_t)xdr_readdirargs, (caddr_t)argp,
					  (xdrproc_t)xdr_readdirres, (caddr_t)clnt_res,
					  TIMEOUT));
}

enum clnt_stat
nfsproc_statfs_2(char *argp, statfsres *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, NFSPROC_STATFS,
					  (xdrproc_t)xdr_fhandle, (caddr_t)argp,
					  (xdrproc_t)xdr_statfsres, (caddr_t)clnt_res,
					  TIMEOUT));
}

#endif
