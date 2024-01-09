
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

bool_t xdr_fhandle (XDR *xdrs, fhandle objp)
{

	 if (!xdr_opaque (xdrs, objp, FHSIZE))
		 return FALSE;
	return TRUE;
}

bool_t xdr_dirpath (XDR *xdrs, nfs_dirpath *objp)
{

	 if (!xdr_string (xdrs, objp, MNTPATHLEN))
		 return FALSE;
	return TRUE;
}

bool_t xdr_name (XDR *xdrs, nfs_name *objp)
{

	 if (!xdr_string (xdrs, objp, MNTNAMLEN))
		 return FALSE;
	return TRUE;
}

bool_t xdr_fhd (XDR *xdrs, fhd *objp)
{

	 if (!xdr_opaque (xdrs, objp->fh, FHSIZE))
		 return FALSE;
	return TRUE;
}

bool_t xdr_fhstatus (XDR *xdrs, fhstatus *objp)
{

	 if (!xdr_u_long (xdrs, &objp->status))
		 return FALSE;
	switch (objp->status) {
	case 0:
		 if (!xdr_fhandle (xdrs, objp->fhstatus_u.directory))
			 return FALSE;
		break;
	default:
		break;
	}
	return TRUE;
}

bool_t xdr_mountlist (XDR *xdrs, mountlist *objp)
{

	 if (!xdr_pointer (xdrs, (char **)objp, sizeof (struct mountbody), (xdrproc_t) xdr_mountbody))
		 return FALSE;
	return TRUE;
}

bool_t xdr_mountbody (XDR *xdrs, mountbody *objp)
{

	 if (!xdr_name (xdrs, &objp->ml_hostname))
		 return FALSE;
	 if (!xdr_dirpath (xdrs, &objp->ml_directory))
		 return FALSE;
	 if (!xdr_mountlist (xdrs, &objp->ml_next))
		 return FALSE;
	return TRUE;
}

bool_t xdr_groups (XDR *xdrs, groups *objp)
{

	 if (!xdr_pointer (xdrs, (char **)objp, sizeof (struct groupnode), (xdrproc_t) xdr_groupnode))
		 return FALSE;
	return TRUE;
}

bool_t xdr_groupnode (XDR *xdrs, groupnode *objp)
{

	 if (!xdr_name (xdrs, &objp->gr_name))
		 return FALSE;
	 if (!xdr_groups (xdrs, &objp->gr_next))
		 return FALSE;
	return TRUE;
}

bool_t xdr_exports (XDR *xdrs, exports *objp)
{

	 if (!xdr_pointer (xdrs, (char **)objp, sizeof (struct exportnode), (xdrproc_t) xdr_exportnode))
		 return FALSE;
	return TRUE;
}

bool_t xdr_exportnode (XDR *xdrs, exportnode *objp)
{

	 if (!xdr_dirpath (xdrs, &objp->ex_dir))
		 return FALSE;
	 if (!xdr_groups (xdrs, &objp->ex_groups))
		 return FALSE;
	 if (!xdr_exports (xdrs, &objp->ex_next))
		 return FALSE;
	return TRUE;
}

bool_t xdr_filename (XDR *xdrs, filename *objp)
{

	 if (!xdr_string (xdrs, objp, NFSMAXNAMLEN))
		 return FALSE;
	return TRUE;
}

bool_t xdr_path (XDR *xdrs, path *objp)
{

	 if (!xdr_string (xdrs, objp, NFSMAXPATHLEN))
		 return FALSE;
	return TRUE;
}

bool_t xdr_nfscookie (XDR *xdrs, nfscookie objp)
{

	 if (!xdr_opaque (xdrs, objp, COOKIESIZE))
		 return FALSE;
	return TRUE;
}

bool_t xdr_nfsdata (XDR *xdrs, nfsdata *objp)
{

	 if (!xdr_bytes (xdrs, (char **)&objp->nfsdata_val, (u_int *) &objp->nfsdata_len, MAXDATA))
		 return FALSE;
	return TRUE;
}

bool_t xdr_nfs_stat (XDR *xdrs, nfs_stat *objp)
{

	 if (!xdr_enum (xdrs, (enum_t *) objp))
		 return FALSE;
	return TRUE;
}

bool_t xdr_ftype (XDR *xdrs, ftype *objp)
{

	 if (!xdr_enum (xdrs, (enum_t *) objp))
		 return FALSE;
	return TRUE;
}

bool_t xdr_timevalNfs (XDR *xdrs, timevalNfs *objp)
{

	 if (!xdr_u_long (xdrs, &objp->seconds))
		 return FALSE;
	 if (!xdr_u_long (xdrs, &objp->useconds))
		 return FALSE;
	return TRUE;
}

bool_t xdr_fattr (XDR *xdrs, fattr *objp)
{
        register int32_t *buf;

	if (xdrs->x_op == XDR_ENCODE) {
		 if (!xdr_ftype (xdrs, &objp->type))
			 return FALSE;
		buf = XDR_INLINE (xdrs, 10 * BYTES_PER_XDR_UNIT);
		if (buf == NULL) {
			 if (!xdr_u_long (xdrs, &objp->mode))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->nlink))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->uid))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->gid))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->size))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->blocksize))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->rdev))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->blocks))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->fsid))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->fileid))
				 return FALSE;

		} else {
		IXDR_PUT_U_LONG(buf, objp->mode);
		IXDR_PUT_U_LONG(buf, objp->nlink);
		IXDR_PUT_U_LONG(buf, objp->uid);
		IXDR_PUT_U_LONG(buf, objp->gid);
		IXDR_PUT_U_LONG(buf, objp->size);
		IXDR_PUT_U_LONG(buf, objp->blocksize);
		IXDR_PUT_U_LONG(buf, objp->rdev);
		IXDR_PUT_U_LONG(buf, objp->blocks);
		IXDR_PUT_U_LONG(buf, objp->fsid);
		IXDR_PUT_U_LONG(buf, objp->fileid);
		}
		 if (!xdr_timevalNfs (xdrs, &objp->atime))
			 return FALSE;
		 if (!xdr_timevalNfs (xdrs, &objp->mtime))
			 return FALSE;
		 if (!xdr_timevalNfs (xdrs, &objp->ctime))
			 return FALSE;
		return TRUE;
	} else if (xdrs->x_op == XDR_DECODE) {
		 if (!xdr_ftype (xdrs, &objp->type))
			 return FALSE;
		buf = XDR_INLINE (xdrs, 10 * BYTES_PER_XDR_UNIT);
		if (buf == NULL) {
			 if (!xdr_u_long (xdrs, &objp->mode))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->nlink))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->uid))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->gid))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->size))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->blocksize))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->rdev))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->blocks))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->fsid))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->fileid))
				 return FALSE;

		} else {
		objp->mode = IXDR_GET_U_LONG(buf);
		objp->nlink = IXDR_GET_U_LONG(buf);
		objp->uid = IXDR_GET_U_LONG(buf);
		objp->gid = IXDR_GET_U_LONG(buf);
		objp->size = IXDR_GET_U_LONG(buf);
		objp->blocksize = IXDR_GET_U_LONG(buf);
		objp->rdev = IXDR_GET_U_LONG(buf);
		objp->blocks = IXDR_GET_U_LONG(buf);
		objp->fsid = IXDR_GET_U_LONG(buf);
		objp->fileid = IXDR_GET_U_LONG(buf);
		}
		 if (!xdr_timevalNfs (xdrs, &objp->atime))
			 return FALSE;
		 if (!xdr_timevalNfs (xdrs, &objp->mtime))
			 return FALSE;
		 if (!xdr_timevalNfs (xdrs, &objp->ctime))
			 return FALSE;
	 return TRUE;
	}

	 if (!xdr_ftype (xdrs, &objp->type))
		 return FALSE;
	 if (!xdr_u_long (xdrs, &objp->mode))
		 return FALSE;
	 if (!xdr_u_long (xdrs, &objp->nlink))
		 return FALSE;
	 if (!xdr_u_long (xdrs, &objp->uid))
		 return FALSE;
	 if (!xdr_u_long (xdrs, &objp->gid))
		 return FALSE;
	 if (!xdr_u_long (xdrs, &objp->size))
		 return FALSE;
	 if (!xdr_u_long (xdrs, &objp->blocksize))
		 return FALSE;
	 if (!xdr_u_long (xdrs, &objp->rdev))
		 return FALSE;
	 if (!xdr_u_long (xdrs, &objp->blocks))
		 return FALSE;
	 if (!xdr_u_long (xdrs, &objp->fsid))
		 return FALSE;
	 if (!xdr_u_long (xdrs, &objp->fileid))
		 return FALSE;
	 if (!xdr_timevalNfs (xdrs, &objp->atime))
		 return FALSE;
	 if (!xdr_timevalNfs (xdrs, &objp->mtime))
		 return FALSE;
	 if (!xdr_timevalNfs (xdrs, &objp->ctime))
		 return FALSE;
	return TRUE;
}

bool_t xdr_sattr (XDR *xdrs, sattr *objp)
{


        register int32_t *buf;
	if (xdrs->x_op == XDR_ENCODE) {
		buf = XDR_INLINE (xdrs, 4 * BYTES_PER_XDR_UNIT);
		if (buf == NULL) {
			 if (!xdr_u_long (xdrs, &objp->mode))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->uid))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->gid))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->size))
				 return FALSE;

		} else {
		IXDR_PUT_U_LONG(buf, objp->mode);
		IXDR_PUT_U_LONG(buf, objp->uid);
		IXDR_PUT_U_LONG(buf, objp->gid);
		IXDR_PUT_U_LONG(buf, objp->size);
		}
		 if (!xdr_timevalNfs (xdrs, &objp->atime))
			 return FALSE;
		 if (!xdr_timevalNfs (xdrs, &objp->mtime))
			 return FALSE;
		return TRUE;
	} else if (xdrs->x_op == XDR_DECODE) {
		buf = XDR_INLINE (xdrs, 4 * BYTES_PER_XDR_UNIT);
		if (buf == NULL) {
			 if (!xdr_u_long (xdrs, &objp->mode))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->uid))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->gid))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->size))
				 return FALSE;

		} else {
		objp->mode = IXDR_GET_U_LONG(buf);
		objp->uid = IXDR_GET_U_LONG(buf);
		objp->gid = IXDR_GET_U_LONG(buf);
		objp->size = IXDR_GET_U_LONG(buf);
		}
		 if (!xdr_timevalNfs (xdrs, &objp->atime))
			 return FALSE;
		 if (!xdr_timevalNfs (xdrs, &objp->mtime))
			 return FALSE;
	 return TRUE;
	}

	 if (!xdr_u_long (xdrs, &objp->mode))
		 return FALSE;
	 if (!xdr_u_long (xdrs, &objp->uid))
		 return FALSE;
	 if (!xdr_u_long (xdrs, &objp->gid))
		 return FALSE;
	 if (!xdr_u_long (xdrs, &objp->size))
		 return FALSE;
	 if (!xdr_timevalNfs (xdrs, &objp->atime))
		 return FALSE;
	 if (!xdr_timevalNfs (xdrs, &objp->mtime))
		 return FALSE;
	return TRUE;
}

bool_t xdr_attrstat (XDR *xdrs, attrstat *objp)
{

	 if (!xdr_nfs_stat (xdrs, &objp->status))
		 return FALSE;
	switch (objp->status) {
	case NFS_OK:
		 if (!xdr_fattr (xdrs, &objp->attrstat_u.attributes))
			 return FALSE;
		break;
	default:
		break;
	}
	return TRUE;
}

bool_t xdr_diropargs (XDR *xdrs, diropargs *objp)
{

	 if (!xdr_fhandle (xdrs, objp->dir))
		 return FALSE;
	 if (!xdr_filename (xdrs, &objp->name))
		 return FALSE;
	return TRUE;
}

bool_t xdr_diropok (XDR *xdrs, diropok *objp)
{

	 if (!xdr_fhandle (xdrs, objp->file))
		 return FALSE;
	 if (!xdr_fattr (xdrs, &objp->attributes))
		 return FALSE;
	return TRUE;
}

bool_t xdr_diropres (XDR *xdrs, diropres *objp)
{

	 if (!xdr_nfs_stat (xdrs, &objp->status))
		 return FALSE;
	switch (objp->status) {
	case NFS_OK:
		 if (!xdr_diropok (xdrs, &objp->diropres_u.fhand_attr))
			 return FALSE;
		break;
	default:
		break;
	}
	return TRUE;
}

bool_t xdr_sattrargs (XDR *xdrs, sattrargs *objp)
{

	 if (!xdr_fhandle (xdrs, objp->file))
		 return FALSE;
	 if (!xdr_sattr (xdrs, &objp->attributes))
		 return FALSE;
	return TRUE;
}

bool_t xdr_readlinkres (XDR *xdrs, readlinkres *objp)
{

	 if (!xdr_nfs_stat (xdrs, &objp->status))
		 return FALSE;
	switch (objp->status) {
	case NFS_OK:
		 if (!xdr_path (xdrs, &objp->readlinkres_u.data))
			 return FALSE;
		break;
	default:
		break;
	}
	return TRUE;
}

bool_t xdr_readargs (XDR *xdrs, readargs *objp)
{

	 if (!xdr_fhandle (xdrs, objp->file))
		 return FALSE;
	 if (!xdr_u_long (xdrs, &objp->offset))
		 return FALSE;
	 if (!xdr_u_long (xdrs, &objp->count))
		 return FALSE;
	 if (!xdr_u_long (xdrs, &objp->totalcount))
		 return FALSE;
	return TRUE;
}

bool_t xdr_datosRes (XDR *xdrs, datosRes *objp)
{

	 if (!xdr_fattr (xdrs, &objp->attributes))
		 return FALSE;
	 if (!xdr_nfsdata (xdrs, &objp->data))
		 return FALSE;
	return TRUE;
}

bool_t xdr_readres (XDR *xdrs, readres *objp)
{

	 if (!xdr_nfs_stat (xdrs, &objp->status))
		 return FALSE;
	switch (objp->status) {
	case NFS_OK:
		 if (!xdr_datosRes (xdrs, &objp->readres_u.fich_read))
			 return FALSE;
		break;
	default:
		break;
	}
	return TRUE;
}

bool_t xdr_writeargs (XDR *xdrs, writeargs *objp)
{


        register int32_t *buf;
	if (xdrs->x_op == XDR_ENCODE) {
		 if (!xdr_fhandle (xdrs, objp->file))
			 return FALSE;
		buf = XDR_INLINE (xdrs, 3 * BYTES_PER_XDR_UNIT);
		if (buf == NULL) {
			 if (!xdr_u_long (xdrs, &objp->beginoffset))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->offset))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->totalcount))
				 return FALSE;

		} else {
		IXDR_PUT_U_LONG(buf, objp->beginoffset);
		IXDR_PUT_U_LONG(buf, objp->offset);
		IXDR_PUT_U_LONG(buf, objp->totalcount);
		}
		 if (!xdr_nfsdata (xdrs, &objp->data))
			 return FALSE;
		return TRUE;
	} else if (xdrs->x_op == XDR_DECODE) {
		 if (!xdr_fhandle (xdrs, objp->file))
			 return FALSE;
		buf = XDR_INLINE (xdrs, 3 * BYTES_PER_XDR_UNIT);
		if (buf == NULL) {
			 if (!xdr_u_long (xdrs, &objp->beginoffset))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->offset))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->totalcount))
				 return FALSE;

		} else {
		objp->beginoffset = IXDR_GET_U_LONG(buf);
		objp->offset = IXDR_GET_U_LONG(buf);
		objp->totalcount = IXDR_GET_U_LONG(buf);
		}
		 if (!xdr_nfsdata (xdrs, &objp->data))
			 return FALSE;
	 return TRUE;
	}

	 if (!xdr_fhandle (xdrs, objp->file))
		 return FALSE;
	 if (!xdr_u_long (xdrs, &objp->beginoffset))
		 return FALSE;
	 if (!xdr_u_long (xdrs, &objp->offset))
		 return FALSE;
	 if (!xdr_u_long (xdrs, &objp->totalcount))
		 return FALSE;
	 if (!xdr_nfsdata (xdrs, &objp->data))
		 return FALSE;
	return TRUE;
}

bool_t xdr_createargs (XDR *xdrs, createargs *objp)
{

	 if (!xdr_diropargs (xdrs, &objp->where))
		 return FALSE;
	 if (!xdr_sattr (xdrs, &objp->attributes))
		 return FALSE;
	return TRUE;
}

bool_t xdr_renameargs (XDR *xdrs, renameargs *objp)
{

	 if (!xdr_diropargs (xdrs, &objp->from))
		 return FALSE;
	 if (!xdr_diropargs (xdrs, &objp->to))
		 return FALSE;
	return TRUE;
}

bool_t xdr_linkargs (XDR *xdrs, linkargs *objp)
{

	 if (!xdr_fhandle (xdrs, objp->from))
		 return FALSE;
	 if (!xdr_diropargs (xdrs, &objp->to))
		 return FALSE;
	return TRUE;
}

bool_t xdr_symlinkargs (XDR *xdrs, symlinkargs *objp)
{

	 if (!xdr_diropargs (xdrs, &objp->from))
		 return FALSE;
	 if (!xdr_path (xdrs, &objp->to))
		 return FALSE;
	 if (!xdr_sattr (xdrs, &objp->attributes))
		 return FALSE;
	return TRUE;
}

bool_t xdr_readdirargs (XDR *xdrs, readdirargs *objp)
{

	 if (!xdr_fhandle (xdrs, objp->dir))
		 return FALSE;
	 if (!xdr_nfscookie (xdrs, objp->cookie))
		 return FALSE;
	 if (!xdr_u_long (xdrs, &objp->count))
		 return FALSE;
	return TRUE;
}

bool_t xdr_entry (XDR *xdrs, entry *objp)
{

	 if (!xdr_u_long (xdrs, &objp->fileid))
		 return FALSE;
	 if (!xdr_filename (xdrs, &objp->name))
		 return FALSE;
	 if (!xdr_nfscookie (xdrs, objp->cookie))
		 return FALSE;
	 if (!xdr_pointer (xdrs, (char **)&objp->nextentry, sizeof (entry), (xdrproc_t) xdr_entry))
		 return FALSE;
	return TRUE;
}

bool_t xdr_readdirok (XDR *xdrs, readdirok *objp)
{

	 if (!xdr_pointer (xdrs, (char **)&objp->entries, sizeof (entry), (xdrproc_t) xdr_entry))
		 return FALSE;
	 if (!xdr_bool (xdrs, &objp->eof))
		 return FALSE;
	return TRUE;
}

bool_t xdr_readdirres (XDR *xdrs, readdirres *objp)
{

	 if (!xdr_nfs_stat (xdrs, &objp->status))
		 return FALSE;
	switch (objp->status) {
	case NFS_OK:
		 if (!xdr_readdirok (xdrs, &objp->readdirres_u.entradasDir))
			 return FALSE;
		break;
	default:
		break;
	}
	return TRUE;
}

	
bool_t xdr_nfs_info (XDR *xdrs, nfs_info *objp)
{

        register int32_t *buf;
       if (xdrs->x_op == XDR_ENCODE) {
               buf = XDR_INLINE(xdrs,5 * BYTES_PER_XDR_UNIT);
               if (buf == NULL) {
                    if (!xdr_u_long (xdrs, &objp->tsize))
                          return FALSE;
                    if (!xdr_u_long (xdrs, &objp->bsize))
                          return FALSE;
                    if (!xdr_u_long (xdrs, &objp->blocks))
                          return FALSE;
                    if (!xdr_u_long (xdrs, &objp->bfree))
                          return FALSE;
                    if (!xdr_u_long (xdrs, &objp->bavail))
                          return FALSE;
                } else {
		    IXDR_PUT_U_LONG(buf, objp->tsize);
		    IXDR_PUT_U_LONG(buf, objp->bsize);
		    IXDR_PUT_U_LONG(buf, objp->blocks);
		    IXDR_PUT_U_LONG(buf, objp->bfree);
		    IXDR_PUT_U_LONG(buf, objp->bavail);
		 }
	         return TRUE;

        } else if (xdrs->x_op == XDR_DECODE) {
              buf = XDR_INLINE(xdrs,5 * BYTES_PER_XDR_UNIT);
              if (buf == NULL) {
                    if (!xdr_u_long (xdrs, &objp->tsize))
                          return FALSE;
                    if (!xdr_u_long (xdrs, &objp->bsize))
                          return FALSE;
                    if (!xdr_u_long (xdrs, &objp->blocks))
                          return FALSE;
                    if (!xdr_u_long (xdrs, &objp->bfree))
                          return FALSE;
                    if (!xdr_u_long (xdrs, &objp->bavail))
                          return FALSE;
               } else {
                    objp->tsize = IXDR_GET_U_LONG(buf);
                    objp->bsize = IXDR_GET_U_LONG(buf);
                    objp->blocks = IXDR_GET_U_LONG(buf);
                    objp->bfree = IXDR_GET_U_LONG(buf);
                    objp->bavail = IXDR_GET_U_LONG(buf);
          }
           return TRUE;
	}
        if (!xdr_u_long (xdrs, &objp->tsize))
             return FALSE;
        if (!xdr_u_long (xdrs, &objp->bsize))
             return FALSE;
        if (!xdr_u_long (xdrs, &objp->blocks))
             return FALSE;
        if (!xdr_u_long (xdrs, &objp->bfree))
             return FALSE;
        if (!xdr_u_long (xdrs, &objp->bavail))
             return FALSE;
        return TRUE;
}
	
bool_t xdr_statfsres (XDR *xdrs, statfsres *objp)
{
		
    if (!xdr_nfs_stat (xdrs, &objp->status))
          return FALSE;
    switch (objp->status) {
        case NFS_OK:
          if (!xdr_nfs_info (xdrs, &objp->statfsres_u.inf))
              return FALSE;
          break;
        default:
          break;
    }
    return TRUE;
}


