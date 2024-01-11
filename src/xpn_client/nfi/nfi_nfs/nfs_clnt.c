
  /*
   *  Copyright 2020-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos
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

#include "nfi_nfs.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

enum clnt_stat 
mountproc_null_1(void *argp, void *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, MOUNTPROC_NULL,
		(xdrproc_t) xdr_void, (caddr_t) argp,
		(xdrproc_t) xdr_void, (caddr_t) clnt_res,
		TIMEOUT));
}

enum clnt_stat 
mountproc_mnt_1(nfs_dirpath *argp, fhstatus *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, MOUNTPROC_MNT,
		(xdrproc_t) xdr_dirpath, (caddr_t) argp,
		(xdrproc_t) xdr_fhstatus, (caddr_t) clnt_res,
		TIMEOUT));
}

enum clnt_stat 
mountproc_dump_1(void *argp, mountlist *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, MOUNTPROC_DUMP,
		(xdrproc_t) xdr_void, (caddr_t) argp,
		(xdrproc_t) xdr_mountlist, (caddr_t) clnt_res,
		TIMEOUT));
}

enum clnt_stat 
mountproc_umnt_1(nfs_dirpath *argp, void *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, MOUNTPROC_UMNT,
		(xdrproc_t) xdr_dirpath, (caddr_t) argp,
		(xdrproc_t) xdr_void, (caddr_t) clnt_res,
		TIMEOUT));
}

enum clnt_stat 
mountproc_umntall_1(void *argp, void *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, MOUNTPROC_UMNTALL,
		(xdrproc_t) xdr_void, (caddr_t) argp,
		(xdrproc_t) xdr_void, (caddr_t) clnt_res,
		TIMEOUT));
}

enum clnt_stat 
mountproc_export_1(void *argp, exports *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, MOUNTPROC_EXPORT,
		(xdrproc_t) xdr_void, (caddr_t) argp,
		(xdrproc_t) xdr_exports, (caddr_t) clnt_res,
		TIMEOUT));
}

enum clnt_stat 
nfsproc_null_2(void *argp, void *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, NFSPROC_NULL,
		(xdrproc_t) xdr_void, (caddr_t) argp,
		(xdrproc_t) xdr_void, (caddr_t) clnt_res,
		TIMEOUT));
}

enum clnt_stat 
nfsproc_getattr_2(char *argp, attrstat *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, NFSPROC_GETATTR,
		(xdrproc_t) xdr_fhandle, (caddr_t) argp,
		(xdrproc_t) xdr_attrstat, (caddr_t) clnt_res,
		TIMEOUT));
}

enum clnt_stat 
nfsproc_setattr_2(sattrargs *argp, attrstat *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, NFSPROC_SETATTR,
		(xdrproc_t) xdr_sattrargs, (caddr_t) argp,
		(xdrproc_t) xdr_attrstat, (caddr_t) clnt_res,
		TIMEOUT));
}

enum clnt_stat 
nfsproc_root_2(void *argp, void *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, NFSPROC_ROOT,
		(xdrproc_t) xdr_void, (caddr_t) argp,
		(xdrproc_t) xdr_void, (caddr_t) clnt_res,
		TIMEOUT));
}

enum clnt_stat 
nfsproc_lookup_2(diropargs *argp, diropres *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, NFSPROC_LOOKUP,
		(xdrproc_t) xdr_diropargs, (caddr_t) argp,
		(xdrproc_t) xdr_diropres, (caddr_t) clnt_res,
		TIMEOUT));
}

enum clnt_stat 
nfsproc_readlink_2(char *argp, readlinkres *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, NFSPROC_READLINK,
		(xdrproc_t) xdr_fhandle, (caddr_t) argp,
		(xdrproc_t) xdr_readlinkres, (caddr_t) clnt_res,
		TIMEOUT));
}

enum clnt_stat 
nfsproc_read_2(readargs *argp, readres *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, NFSPROC_READ,
		(xdrproc_t) xdr_readargs, (caddr_t) argp,
		(xdrproc_t) xdr_readres, (caddr_t) clnt_res,
		TIMEOUT));
}

enum clnt_stat 
nfsproc_writecache_2(void *argp, void *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, NFSPROC_WRITECACHE,
		(xdrproc_t) xdr_void, (caddr_t) argp,
		(xdrproc_t) xdr_void, (caddr_t) clnt_res,
		TIMEOUT));
}

enum clnt_stat 
nfsproc_write_2(writeargs *argp, attrstat *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, NFSPROC_WRITE,
		(xdrproc_t) xdr_writeargs, (caddr_t) argp,
		(xdrproc_t) xdr_attrstat, (caddr_t) clnt_res,
		TIMEOUT));
}

enum clnt_stat 
nfsproc_create_2(createargs *argp, diropres *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, NFSPROC_CREATE,
		(xdrproc_t) xdr_createargs, (caddr_t) argp,
		(xdrproc_t) xdr_diropres, (caddr_t) clnt_res,
		TIMEOUT));
}

enum clnt_stat 
nfsproc_remove_2(diropargs *argp, nfs_stat *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, NFSPROC_REMOVE,
		(xdrproc_t) xdr_diropargs, (caddr_t) argp,
		(xdrproc_t) xdr_nfs_stat, (caddr_t) clnt_res,
		TIMEOUT));
}

enum clnt_stat 
nfsproc_rename_2(renameargs *argp, nfs_stat *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, NFSPROC_RENAME,
		(xdrproc_t) xdr_renameargs, (caddr_t) argp,
		(xdrproc_t) xdr_nfs_stat, (caddr_t) clnt_res,
		TIMEOUT));
}

enum clnt_stat 
nfsproc_link_2(linkargs *argp, nfs_stat *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, NFSPROC_LINK,
		(xdrproc_t) xdr_linkargs, (caddr_t) argp,
		(xdrproc_t) xdr_nfs_stat, (caddr_t) clnt_res,
		TIMEOUT));
}

enum clnt_stat 
nfsproc_symlink_2(symlinkargs *argp, nfs_stat *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, NFSPROC_SYMLINK,
		(xdrproc_t) xdr_symlinkargs, (caddr_t) argp,
		(xdrproc_t) xdr_nfs_stat, (caddr_t) clnt_res,
		TIMEOUT));
}

enum clnt_stat 
nfsproc_mkdir_2(createargs *argp, diropres *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, NFSPROC_MKDIR,
		(xdrproc_t) xdr_createargs, (caddr_t) argp,
		(xdrproc_t) xdr_diropres, (caddr_t) clnt_res,
		TIMEOUT));
}

enum clnt_stat 
nfsproc_rmdir_2(diropargs *argp, nfs_stat *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, NFSPROC_RMDIR,
		(xdrproc_t) xdr_diropargs, (caddr_t) argp,
		(xdrproc_t) xdr_nfs_stat, (caddr_t) clnt_res,
		TIMEOUT));
}

enum clnt_stat 
nfsproc_readdir_2(readdirargs *argp, readdirres *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, NFSPROC_READDIR,
		(xdrproc_t) xdr_readdirargs, (caddr_t) argp,
		(xdrproc_t) xdr_readdirres, (caddr_t) clnt_res,
		TIMEOUT));
}

enum clnt_stat 
nfsproc_statfs_2(char *argp, statfsres *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, NFSPROC_STATFS,
		(xdrproc_t) xdr_fhandle, (caddr_t) argp,
		(xdrproc_t) xdr_statfsres, (caddr_t) clnt_res,
		TIMEOUT));
}


