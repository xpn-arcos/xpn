
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

/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "nfi/nfi_nfs3/nfs3.h"

bool_t
xdr_uint64 (XDR *xdrs, uint64 *objp)
{
	//register int32_t *buf;

	 if (!xdr_uint64_t (xdrs, objp))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_int64 (XDR *xdrs, int64 *objp)
{
	//register int32_t *buf;

	 if (!xdr_int64_t (xdrs, objp))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_uint32 (XDR *xdrs, uint32 *objp)
{
	//register int32_t *buf;

	 if (!xdr_u_int (xdrs, objp))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_int32 (XDR *xdrs, int32 *objp)
{
	//register int32_t *buf;

	 if (!xdr_int (xdrs, objp))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_filename3 (XDR *xdrs, filename3 *objp)
{
	//register int32_t *buf;

	 if (!xdr_string (xdrs, objp, ~0))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_nfspath3 (XDR *xdrs, nfspath3 *objp)
{
	//register int32_t *buf;

	 if (!xdr_string (xdrs, objp, ~0))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_cookieverf3 (XDR *xdrs, cookieverf3 objp)
{
	//register int32_t *buf;

	 if (!xdr_opaque (xdrs, objp, NFS3_COOKIEVERFSIZE))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_createverf3 (XDR *xdrs, createverf3 objp)
{
	//register int32_t *buf;

	 if (!xdr_opaque (xdrs, objp, NFS3_CREATEVERFSIZE))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_writeverf3 (XDR *xdrs, writeverf3 objp)
{
	//register int32_t *buf;

	 if (!xdr_opaque (xdrs, objp, NFS3_WRITEVERFSIZE))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_fhandle3 (XDR *xdrs, fhandle3 *objp)
{
	//register int32_t *buf;

	 if (!xdr_bytes (xdrs, (char **)&objp->fhandle3_val, (u_int *) &objp->fhandle3_len, NFS3_FHSIZE))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_mountstat3 (XDR *xdrs, mountstat3 *objp)
{
	//register int32_t *buf;

	 if (!xdr_enum (xdrs, (enum_t *) objp))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_mountres3_ok (XDR *xdrs, mountres3_ok *objp)
{
	//register int32_t *buf;

	 if (!xdr_fhandle3 (xdrs, &objp->fhandle))
		 return FALSE;
	 if (!xdr_array (xdrs, (char **)&objp->auth_flavors.auth_flavors_val, (u_int *) &objp->auth_flavors.auth_flavors_len, ~0,
		sizeof (int), (xdrproc_t) xdr_int))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_mountres3 (XDR *xdrs, mountres3 *objp)
{
	////register int32_t *buf;

	 if (!xdr_mountstat3 (xdrs, &objp->fhs_status))
		 return FALSE;
	switch (objp->fhs_status) {
	case MNT3_OK:
		 if (!xdr_mountres3_ok (xdrs, &objp->mountres3_u.mountinfo))
			 return FALSE;
		break;
	default:
		break;
	}
	return TRUE;
}

bool_t
xdr_mountlist3 (XDR *xdrs, mountlist3 *objp)
{
	//register int32_t *buf;

	 if (!xdr_pointer (xdrs, (char **)objp, sizeof (struct mountbody3), (xdrproc_t) xdr_mountbody3))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_mountbody3 (XDR *xdrs, mountbody3 *objp)
{
	//register int32_t *buf;

	 if (!xdr_filename3 (xdrs, &objp->ml_hostname))
		 return FALSE;
	 if (!xdr_nfspath3 (xdrs, &objp->ml_directory))
		 return FALSE;
	 if (!xdr_mountlist3 (xdrs, &objp->ml_next))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_groups3 (XDR *xdrs, groups3 *objp)
{
	//register int32_t *buf;

	 if (!xdr_pointer (xdrs, (char **)objp, sizeof (struct groupnode3), (xdrproc_t) xdr_groupnode3))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_groupnode3 (XDR *xdrs, groupnode3 *objp)
{
	//register int32_t *buf;

	 if (!xdr_filename3 (xdrs, &objp->gr_name))
		 return FALSE;
	 if (!xdr_groups3 (xdrs, &objp->gr_next))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_exports3 (XDR *xdrs, exports3 *objp)
{
	//register int32_t *buf;

	 if (!xdr_pointer (xdrs, (char **)objp, sizeof (struct exportnode3), (xdrproc_t) xdr_exportnode3))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_exportnode3 (XDR *xdrs, exportnode3 *objp)
{
	//register int32_t *buf;

	 if (!xdr_nfspath3 (xdrs, &objp->ex_dir))
		 return FALSE;
	 if (!xdr_groups3 (xdrs, &objp->ex_groups3))
		 return FALSE;
	 if (!xdr_exports3 (xdrs, &objp->ex_next))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_nfsstat3 (XDR *xdrs, nfsstat3 *objp)
{
	//register int32_t *buf;

	 if (!xdr_enum (xdrs, (enum_t *) objp))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_ftype3 (XDR *xdrs, ftype3 *objp)
{
	//register int32_t *buf;

	 if (!xdr_enum (xdrs, (enum_t *) objp))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_specdata3 (XDR *xdrs, specdata3 *objp)
{
	//register int32_t *buf;

	 if (!xdr_uint32 (xdrs, &objp->major))
		 return FALSE;
	 if (!xdr_uint32 (xdrs, &objp->minor))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_nfstime3 (XDR *xdrs, nfstime3 *objp)
{
	//register int32_t *buf;

	 if (!xdr_uint32 (xdrs, &objp->seconds))
		 return FALSE;
	 if (!xdr_uint32 (xdrs, &objp->nseconds))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_fattr3 (XDR *xdrs, fattr3 *objp)
{
	//register int32_t *buf;

	 if (!xdr_ftype3 (xdrs, &objp->type))
		 return FALSE;
	 if (!xdr_uint32 (xdrs, &objp->mode))
		 return FALSE;
	 if (!xdr_uint32 (xdrs, &objp->nlink))
		 return FALSE;
	 if (!xdr_uint32 (xdrs, &objp->uid))
		 return FALSE;
	 if (!xdr_uint32 (xdrs, &objp->gid))
		 return FALSE;
	 if (!xdr_uint64 (xdrs, &objp->size))
		 return FALSE;
	 if (!xdr_uint64 (xdrs, &objp->used))
		 return FALSE;
	 if (!xdr_specdata3 (xdrs, &objp->rdev))
		 return FALSE;
	 if (!xdr_uint64 (xdrs, &objp->fsid))
		 return FALSE;
	 if (!xdr_uint64 (xdrs, &objp->fileid))
		 return FALSE;
	 if (!xdr_nfstime3 (xdrs, &objp->atime))
		 return FALSE;
	 if (!xdr_nfstime3 (xdrs, &objp->mtime))
		 return FALSE;
	 if (!xdr_nfstime3 (xdrs, &objp->ctime))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_post_op_attr (XDR *xdrs, post_op_attr *objp)
{
	//register int32_t *buf;

	 if (!xdr_bool (xdrs, &objp->present))
		 return FALSE;
	switch (objp->present) {
	case TRUE:
		 if (!xdr_fattr3 (xdrs, &objp->post_op_attr_u.attributes))
			 return FALSE;
		break;
	case FALSE:
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

bool_t
xdr_wcc_attr (XDR *xdrs, wcc_attr *objp)
{
	//register int32_t *buf;

	 if (!xdr_uint64 (xdrs, &objp->size))
		 return FALSE;
	 if (!xdr_nfstime3 (xdrs, &objp->mtime))
		 return FALSE;
	 if (!xdr_nfstime3 (xdrs, &objp->ctime))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_pre_op_attr (XDR *xdrs, pre_op_attr *objp)
{
	//register int32_t *buf;

	 if (!xdr_bool (xdrs, &objp->present))
		 return FALSE;
	switch (objp->present) {
	case TRUE:
		 if (!xdr_wcc_attr (xdrs, &objp->pre_op_attr_u.attributes))
			 return FALSE;
		break;
	case FALSE:
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

bool_t
xdr_wcc_data (XDR *xdrs, wcc_data *objp)
{
	//register int32_t *buf;

	 if (!xdr_pre_op_attr (xdrs, &objp->before))
		 return FALSE;
	 if (!xdr_post_op_attr (xdrs, &objp->after))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_post_op_fh3 (XDR *xdrs, post_op_fh3 *objp)
{
	//register int32_t *buf;

	 if (!xdr_bool (xdrs, &objp->present))
		 return FALSE;
	switch (objp->present) {
	case TRUE:
		 if (!xdr_fhandle3 (xdrs, &objp->post_op_fh3_u.handle))
			 return FALSE;
		break;
	case FALSE:
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

bool_t
xdr_set_uint32 (XDR *xdrs, set_uint32 *objp)
{
	//register int32_t *buf;

	 if (!xdr_bool (xdrs, &objp->set))
		 return FALSE;
	switch (objp->set) {
	case TRUE:
		 if (!xdr_uint32 (xdrs, &objp->set_uint32_u.val))
			 return FALSE;
		break;
	default:
		break;
	}
	return TRUE;
}

bool_t
xdr_set_uint64 (XDR *xdrs, set_uint64 *objp)
{
	//register int32_t *buf;

	 if (!xdr_bool (xdrs, &objp->set))
		 return FALSE;
	switch (objp->set) {
	case TRUE:
		 if (!xdr_uint64 (xdrs, &objp->set_uint64_u.val))
			 return FALSE;
		break;
	default:
		break;
	}
	return TRUE;
}

bool_t
xdr_time_how (XDR *xdrs, time_how *objp)
{
	//register int32_t *buf;

	 if (!xdr_enum (xdrs, (enum_t *) objp))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_set_time (XDR *xdrs, set_time *objp)
{
	////register int32_t *buf;

	 if (!xdr_time_how (xdrs, &objp->set))
		 return FALSE;
	switch (objp->set) {
	case SET_TO_CLIENT_TIME:
		 if (!xdr_nfstime3 (xdrs, &objp->set_time_u.time))
			 return FALSE;
		break;
	default:
		break;
	}
	return TRUE;
}

bool_t
xdr_sattr3 (XDR *xdrs, sattr3 *objp)
{
	//register int32_t *buf;

	 if (!xdr_set_uint32 (xdrs, &objp->mode))
		 return FALSE;
	 if (!xdr_set_uint32 (xdrs, &objp->uid))
		 return FALSE;
	 if (!xdr_set_uint32 (xdrs, &objp->gid))
		 return FALSE;
	 if (!xdr_set_uint64 (xdrs, &objp->size))
		 return FALSE;
	 if (!xdr_set_time (xdrs, &objp->atime))
		 return FALSE;
	 if (!xdr_set_time (xdrs, &objp->mtime))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_diropargs3 (XDR *xdrs, diropargs3 *objp)
{
	//register int32_t *buf;

	 if (!xdr_fhandle3 (xdrs, &objp->dir))
		 return FALSE;
	 if (!xdr_filename3 (xdrs, &objp->name))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_diropres3ok (XDR *xdrs, diropres3ok *objp)
{
	//register int32_t *buf;

	 if (!xdr_post_op_fh3 (xdrs, &objp->obj))
		 return FALSE;
	 if (!xdr_post_op_attr (xdrs, &objp->obj_attributes))
		 return FALSE;
	 if (!xdr_wcc_data (xdrs, &objp->dir_wcc))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_diropres3 (XDR *xdrs, diropres3 *objp)
{
	//register int32_t *buf;

	 if (!xdr_nfsstat3 (xdrs, &objp->status))
		 return FALSE;
	switch (objp->status) {
	case NFS3_OK:
		 if (!xdr_diropres3ok (xdrs, &objp->diropres3_u.resok))
			 return FALSE;
		break;
	default:
		 if (!xdr_wcc_data (xdrs, &objp->diropres3_u.resfail))
			 return FALSE;
		break;
	}
	return TRUE;
}

bool_t
xdr_wccstat3 (XDR *xdrs, wccstat3 *objp)
{
	//register int32_t *buf;

	 if (!xdr_nfsstat3 (xdrs, &objp->status))
		 return FALSE;
	switch (objp->status) {
	case -1:
		break;
	default:
		 if (!xdr_wcc_data (xdrs, &objp->wccstat3_u.wcc))
			 return FALSE;
		break;
	}
	return TRUE;
}

bool_t
xdr_getattr3res (XDR *xdrs, getattr3res *objp)
{
	//register int32_t *buf;

	 if (!xdr_nfsstat3 (xdrs, &objp->status))
		 return FALSE;
	switch (objp->status) {
	case NFS3_OK:
		 if (!xdr_fattr3 (xdrs, &objp->getattr3res_u.attributes))
			 return FALSE;
		break;
	default:
		break;
	}
	return TRUE;
}

bool_t
xdr_sattrguard3 (XDR *xdrs, sattrguard3 *objp)
{
	//register int32_t *buf;

	 if (!xdr_bool (xdrs, &objp->check))
		 return FALSE;
	switch (objp->check) {
	case TRUE:
		 if (!xdr_nfstime3 (xdrs, &objp->sattrguard3_u.ctime))
			 return FALSE;
		break;
	case FALSE:
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

bool_t
xdr_setattr3args (XDR *xdrs, setattr3args *objp)
{
	//register int32_t *buf;

	 if (!xdr_fhandle3 (xdrs, &objp->object))
		 return FALSE;
	 if (!xdr_sattr3 (xdrs, &objp->new_attributes))
		 return FALSE;
	 if (!xdr_sattrguard3 (xdrs, &objp->guard))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_lookup3resok (XDR *xdrs, lookup3resok *objp)
{
	//register int32_t *buf;

	 if (!xdr_fhandle3 (xdrs, &objp->object))
		 return FALSE;
	 if (!xdr_post_op_attr (xdrs, &objp->obj_attributes))
		 return FALSE;
	 if (!xdr_post_op_attr (xdrs, &objp->dir_attributes))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_lookup3res (XDR *xdrs, lookup3res *objp)
{
	//register int32_t *buf;

	 if (!xdr_nfsstat3 (xdrs, &objp->status))
		 return FALSE;
	switch (objp->status) {
	case NFS3_OK:
		 if (!xdr_lookup3resok (xdrs, &objp->lookup3res_u.resok))
			 return FALSE;
		break;
	default:
		 if (!xdr_post_op_attr (xdrs, &objp->lookup3res_u.resfail))
			 return FALSE;
		break;
	}
	return TRUE;
}

bool_t
xdr_access3args (XDR *xdrs, access3args *objp)
{
	//register int32_t *buf;

	 if (!xdr_fhandle3 (xdrs, &objp->object))
		 return FALSE;
	 if (!xdr_uint32 (xdrs, &objp->access))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_access3resok (XDR *xdrs, access3resok *objp)
{
	//register int32_t *buf;

	 if (!xdr_post_op_attr (xdrs, &objp->obj_attributes))
		 return FALSE;
	 if (!xdr_uint32 (xdrs, &objp->access))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_access3res (XDR *xdrs, access3res *objp)
{
	//register int32_t *buf;

	 if (!xdr_nfsstat3 (xdrs, &objp->status))
		 return FALSE;
	switch (objp->status) {
	case NFS3_OK:
		 if (!xdr_access3resok (xdrs, &objp->access3res_u.resok))
			 return FALSE;
		break;
	default:
		 if (!xdr_post_op_attr (xdrs, &objp->access3res_u.resfail))
			 return FALSE;
		break;
	}
	return TRUE;
}

bool_t
xdr_readlink3resok (XDR *xdrs, readlink3resok *objp)
{
	//register int32_t *buf;

	 if (!xdr_post_op_attr (xdrs, &objp->symlink_attributes))
		 return FALSE;
	 if (!xdr_nfspath3 (xdrs, &objp->data))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_readlink3res (XDR *xdrs, readlink3res *objp)
{
	//register int32_t *buf;

	 if (!xdr_nfsstat3 (xdrs, &objp->status))
		 return FALSE;
	switch (objp->status) {
	case NFS3_OK:
		 if (!xdr_readlink3resok (xdrs, &objp->readlink3res_u.resok))
			 return FALSE;
		break;
	default:
		 if (!xdr_post_op_attr (xdrs, &objp->readlink3res_u.resfail))
			 return FALSE;
		break;
	}
	return TRUE;
}

bool_t
xdr_read3args (XDR *xdrs, read3args *objp)
{
	//register int32_t *buf;

	 if (!xdr_fhandle3 (xdrs, &objp->file))
		 return FALSE;
	 if (!xdr_uint64 (xdrs, &objp->offset))
		 return FALSE;
	 if (!xdr_uint32 (xdrs, &objp->count))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_read3resok (XDR *xdrs, read3resok *objp)
{
	//register int32_t *buf;

	 if (!xdr_post_op_attr (xdrs, &objp->file_attributes))
		 return FALSE;
	 if (!xdr_uint32 (xdrs, &objp->count))
		 return FALSE;
	 if (!xdr_bool (xdrs, &objp->eof))
		 return FALSE;
	 if (!xdr_bytes (xdrs, (char **)&objp->data.data_val, (u_int *) &objp->data.data_len, ~0))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_read3res (XDR *xdrs, read3res *objp)
{
	//register int32_t *buf;

	 if (!xdr_nfsstat3 (xdrs, &objp->status))
		 return FALSE;
	switch (objp->status) {
	case NFS3_OK:
		 if (!xdr_read3resok (xdrs, &objp->read3res_u.resok))
			 return FALSE;
		break;
	default:
		 if (!xdr_post_op_attr (xdrs, &objp->read3res_u.resfail))
			 return FALSE;
		break;
	}
	return TRUE;
}

bool_t
xdr_stable_how (XDR *xdrs, stable_how *objp)
{
	//register int32_t *buf;

	 if (!xdr_enum (xdrs, (enum_t *) objp))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_write3args (XDR *xdrs, write3args *objp)
{
	//register int32_t *buf;

	 if (!xdr_fhandle3 (xdrs, &objp->file))
		 return FALSE;
	 if (!xdr_uint64 (xdrs, &objp->offset))
		 return FALSE;
	 if (!xdr_uint32 (xdrs, &objp->count))
		 return FALSE;
	 if (!xdr_stable_how (xdrs, &objp->stable))
		 return FALSE;
	 if (!xdr_bytes (xdrs, (char **)&objp->data.data_val, (u_int *) &objp->data.data_len, ~0))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_write3resok (XDR *xdrs, write3resok *objp)
{
	//register int32_t *buf;

	 if (!xdr_wcc_data (xdrs, &objp->file_wcc))
		 return FALSE;
	 if (!xdr_uint32 (xdrs, &objp->count))
		 return FALSE;
	 if (!xdr_stable_how (xdrs, &objp->committed))
		 return FALSE;
	 if (!xdr_writeverf3 (xdrs, objp->verf))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_write3res (XDR *xdrs, write3res *objp)
{
	//register int32_t *buf;

	 if (!xdr_nfsstat3 (xdrs, &objp->status))
		 return FALSE;
	switch (objp->status) {
	case NFS3_OK:
		 if (!xdr_write3resok (xdrs, &objp->write3res_u.resok))
			 return FALSE;
		break;
	default:
		 if (!xdr_wcc_data (xdrs, &objp->write3res_u.resfail))
			 return FALSE;
		break;
	}
	return TRUE;
}

bool_t
xdr_createmode3 (XDR *xdrs, createmode3 *objp)
{
	//register int32_t *buf;

	 if (!xdr_enum (xdrs, (enum_t *) objp))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_createhow3 (XDR *xdrs, createhow3 *objp)
{
	//register int32_t *buf;

	 if (!xdr_createmode3 (xdrs, &objp->mode))
		 return FALSE;
	switch (objp->mode) {
	case UNCHECKED:
	case GUARDED:
		 if (!xdr_sattr3 (xdrs, &objp->createhow3_u.obj_attributes))
			 return FALSE;
		break;
	case EXCLUSIVE:
		 if (!xdr_createverf3 (xdrs, objp->createhow3_u.verf))
			 return FALSE;
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

bool_t
xdr_create3args (XDR *xdrs, create3args *objp)
{
	////register int32_t *buf;

	 if (!xdr_diropargs3 (xdrs, &objp->where))
		 return FALSE;
	 if (!xdr_createhow3 (xdrs, &objp->how))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_mkdir3args (XDR *xdrs, mkdir3args *objp)
{
	//register int32_t *buf;

	 if (!xdr_diropargs3 (xdrs, &objp->where))
		 return FALSE;
	 if (!xdr_sattr3 (xdrs, &objp->attributes))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_symlinkdata3 (XDR *xdrs, symlinkdata3 *objp)
{
	//register int32_t *buf;

	 if (!xdr_sattr3 (xdrs, &objp->symlink_attributes))
		 return FALSE;
	 if (!xdr_nfspath3 (xdrs, &objp->symlink_data))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_symlink3args (XDR *xdrs, symlink3args *objp)
{
	//register int32_t *buf;

	 if (!xdr_diropargs3 (xdrs, &objp->where))
		 return FALSE;
	 if (!xdr_symlinkdata3 (xdrs, &objp->symlink))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_devicedata3 (XDR *xdrs, devicedata3 *objp)
{
	//register int32_t *buf;

	 if (!xdr_sattr3 (xdrs, &objp->dev_attributes))
		 return FALSE;
	 if (!xdr_specdata3 (xdrs, &objp->spec))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_mknoddata3 (XDR *xdrs, mknoddata3 *objp)
{
	//register int32_t *buf;

	 if (!xdr_ftype3 (xdrs, &objp->type))
		 return FALSE;
	switch (objp->type) {
	case NF3CHR:
	case NF3BLK:
		 if (!xdr_devicedata3 (xdrs, &objp->mknoddata3_u.device))
			 return FALSE;
		break;
	case NF3SOCK:
	case NF3FIFO:
		 if (!xdr_sattr3 (xdrs, &objp->mknoddata3_u.pipe_attributes))
			 return FALSE;
		break;
	default:
		break;
	}
	return TRUE;
}

bool_t
xdr_mknod3args (XDR *xdrs, mknod3args *objp)
{
	//register int32_t *buf;

	 if (!xdr_diropargs3 (xdrs, &objp->where))
		 return FALSE;
	 if (!xdr_mknoddata3 (xdrs, &objp->what))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_rename3args (XDR *xdrs, rename3args *objp)
{
	//register int32_t *buf;

	 if (!xdr_diropargs3 (xdrs, &objp->from))
		 return FALSE;
	 if (!xdr_diropargs3 (xdrs, &objp->to))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_rename3wcc (XDR *xdrs, rename3wcc *objp)
{
	//register int32_t *buf;

	 if (!xdr_wcc_data (xdrs, &objp->fromdir_wcc))
		 return FALSE;
	 if (!xdr_wcc_data (xdrs, &objp->todir_wcc))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_rename3res (XDR *xdrs, rename3res *objp)
{
	//register int32_t *buf;

	 if (!xdr_nfsstat3 (xdrs, &objp->status))
		 return FALSE;
	switch (objp->status) {
	case -1:
		break;
	default:
		 if (!xdr_rename3wcc (xdrs, &objp->rename3res_u.res))
			 return FALSE;
		break;
	}
	return TRUE;
}

bool_t
xdr_link3args (XDR *xdrs, link3args *objp)
{
	//////register int32_t *buf;

	 if (!xdr_fhandle3 (xdrs, &objp->file))
		 return FALSE;
	 if (!xdr_diropargs3 (xdrs, &objp->link))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_link3wcc (XDR *xdrs, link3wcc *objp)
{
	//register int32_t *buf;

	 if (!xdr_post_op_attr (xdrs, &objp->file_attributes))
		 return FALSE;
	 if (!xdr_wcc_data (xdrs, &objp->linkdir_wcc))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_link3res (XDR *xdrs, link3res *objp)
{
	//register int32_t *buf;

	 if (!xdr_nfsstat3 (xdrs, &objp->status))
		 return FALSE;
	switch (objp->status) {
	case -1:
		break;
	default:
		 if (!xdr_link3wcc (xdrs, &objp->link3res_u.res))
			 return FALSE;
		break;
	}
	return TRUE;
}

bool_t
xdr_readdir3args (XDR *xdrs, readdir3args *objp)
{
	//register int32_t *buf;

	 if (!xdr_fhandle3 (xdrs, &objp->dir))
		 return FALSE;
	 if (!xdr_uint64 (xdrs, &objp->cookie))
		 return FALSE;
	 if (!xdr_cookieverf3 (xdrs, objp->cookieverf))
		 return FALSE;
	 if (!xdr_uint32 (xdrs, &objp->count))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_entry3 (XDR *xdrs, entry3 *objp)
{
	//register int32_t *buf;

	 if (!xdr_uint64 (xdrs, &objp->fileid))
		 return FALSE;
	 if (!xdr_filename3 (xdrs, &objp->name))
		 return FALSE;
	 if (!xdr_uint64 (xdrs, &objp->cookie))
		 return FALSE;
	 if (!xdr_pointer (xdrs, (char **)&objp->nextentry, sizeof (entry3), (xdrproc_t) xdr_entry3))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_dirlist3 (XDR *xdrs, dirlist3 *objp)
{
	//register int32_t *buf;

	 if (!xdr_pointer (xdrs, (char **)&objp->entries, sizeof (entry3), (xdrproc_t) xdr_entry3))
		 return FALSE;
	 if (!xdr_bool (xdrs, &objp->eof))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_readdir3resok (XDR *xdrs, readdir3resok *objp)
{
	//register int32_t *buf;

	 if (!xdr_post_op_attr (xdrs, &objp->dir_attributes))
		 return FALSE;
	 if (!xdr_cookieverf3 (xdrs, objp->cookieverf))
		 return FALSE;
	 if (!xdr_dirlist3 (xdrs, &objp->reply))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_readdir3res (XDR *xdrs, readdir3res *objp)
{
	//register int32_t *buf;

	 if (!xdr_nfsstat3 (xdrs, &objp->status))
		 return FALSE;
	switch (objp->status) {
	case NFS3_OK:
		 if (!xdr_readdir3resok (xdrs, &objp->readdir3res_u.resok))
			 return FALSE;
		break;
	default:
		 if (!xdr_post_op_attr (xdrs, &objp->readdir3res_u.resfail))
			 return FALSE;
		break;
	}
	return TRUE;
}

bool_t
xdr_readdirplus3args (XDR *xdrs, readdirplus3args *objp)
{
	//register int32_t *buf;

	 if (!xdr_fhandle3 (xdrs, &objp->dir))
		 return FALSE;
	 if (!xdr_uint64 (xdrs, &objp->cookie))
		 return FALSE;
	 if (!xdr_cookieverf3 (xdrs, objp->cookieverf))
		 return FALSE;
	 if (!xdr_uint32 (xdrs, &objp->dircount))
		 return FALSE;
	 if (!xdr_uint32 (xdrs, &objp->maxcount))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_entryplus3 (XDR *xdrs, entryplus3 *objp)
{
	//register int32_t *buf;

	 if (!xdr_uint64 (xdrs, &objp->fileid))
		 return FALSE;
	 if (!xdr_filename3 (xdrs, &objp->name))
		 return FALSE;
	 if (!xdr_uint64 (xdrs, &objp->cookie))
		 return FALSE;
	 if (!xdr_post_op_attr (xdrs, &objp->name_attributes))
		 return FALSE;
	 if (!xdr_post_op_fh3 (xdrs, &objp->name_handle))
		 return FALSE;
	 if (!xdr_pointer (xdrs, (char **)&objp->nextentry, sizeof (entryplus3), (xdrproc_t) xdr_entryplus3))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_dirlistplus3 (XDR *xdrs, dirlistplus3 *objp)
{
	//register int32_t *buf;

	 if (!xdr_pointer (xdrs, (char **)&objp->entries, sizeof (entryplus3), (xdrproc_t) xdr_entryplus3))
		 return FALSE;
	 if (!xdr_bool (xdrs, &objp->eof))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_readdirplus3resok (XDR *xdrs, readdirplus3resok *objp)
{
	//register int32_t *buf;

	 if (!xdr_post_op_attr (xdrs, &objp->dir_attributes))
		 return FALSE;
	 if (!xdr_cookieverf3 (xdrs, objp->cookieverf))
		 return FALSE;
	 if (!xdr_dirlistplus3 (xdrs, &objp->reply))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_readdirplus3res (XDR *xdrs, readdirplus3res *objp)
{
	//register int32_t *buf;

	 if (!xdr_nfsstat3 (xdrs, &objp->status))
		 return FALSE;
	switch (objp->status) {
	case NFS3_OK:
		 if (!xdr_readdirplus3resok (xdrs, &objp->readdirplus3res_u.resok))
			 return FALSE;
		break;
	default:
		 if (!xdr_post_op_attr (xdrs, &objp->readdirplus3res_u.resfail))
			 return FALSE;
		break;
	}
	return TRUE;
}

bool_t
xdr_fsstat3resok (XDR *xdrs, fsstat3resok *objp)
{
	//register int32_t *buf;

	 if (!xdr_post_op_attr (xdrs, &objp->obj_attributes))
		 return FALSE;
	 if (!xdr_uint64 (xdrs, &objp->tbytes))
		 return FALSE;
	 if (!xdr_uint64 (xdrs, &objp->fbytes))
		 return FALSE;
	 if (!xdr_uint64 (xdrs, &objp->abytes))
		 return FALSE;
	 if (!xdr_uint64 (xdrs, &objp->tfiles))
		 return FALSE;
	 if (!xdr_uint64 (xdrs, &objp->ffiles))
		 return FALSE;
	 if (!xdr_uint64 (xdrs, &objp->afiles))
		 return FALSE;
	 if (!xdr_uint32 (xdrs, &objp->invarsec))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_fsstat3res (XDR *xdrs, fsstat3res *objp)
{
	//register int32_t *buf;

	 if (!xdr_nfsstat3 (xdrs, &objp->status))
		 return FALSE;
	switch (objp->status) {
	case NFS3_OK:
		 if (!xdr_fsstat3resok (xdrs, &objp->fsstat3res_u.resok))
			 return FALSE;
		break;
	default:
		 if (!xdr_post_op_attr (xdrs, &objp->fsstat3res_u.resfail))
			 return FALSE;
		break;
	}
	return TRUE;
}

bool_t
xdr_fsinfo3resok (XDR *xdrs, fsinfo3resok *objp)
{
	//register int32_t *buf;

	 if (!xdr_post_op_attr (xdrs, &objp->obj_attributes))
		 return FALSE;
	 if (!xdr_uint32 (xdrs, &objp->rtmax))
		 return FALSE;
	 if (!xdr_uint32 (xdrs, &objp->rtpref))
		 return FALSE;
	 if (!xdr_uint32 (xdrs, &objp->rtmult))
		 return FALSE;
	 if (!xdr_uint32 (xdrs, &objp->wtmax))
		 return FALSE;
	 if (!xdr_uint32 (xdrs, &objp->wtpref))
		 return FALSE;
	 if (!xdr_uint32 (xdrs, &objp->wtmult))
		 return FALSE;
	 if (!xdr_uint32 (xdrs, &objp->dtpref))
		 return FALSE;
	 if (!xdr_uint64 (xdrs, &objp->maxfilesize))
		 return FALSE;
	 if (!xdr_nfstime3 (xdrs, &objp->time_delta))
		 return FALSE;
	 if (!xdr_uint32 (xdrs, &objp->properties))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_fsinfo3res (XDR *xdrs, fsinfo3res *objp)
{
	//register int32_t *buf;

	 if (!xdr_nfsstat3 (xdrs, &objp->status))
		 return FALSE;
	switch (objp->status) {
	case NFS3_OK:
		 if (!xdr_fsinfo3resok (xdrs, &objp->fsinfo3res_u.resok))
			 return FALSE;
		break;
	default:
		 if (!xdr_post_op_attr (xdrs, &objp->fsinfo3res_u.resfail))
			 return FALSE;
		break;
	}
	return TRUE;
}

bool_t
xdr_pathconf3resok (XDR *xdrs, pathconf3resok *objp)
{
	register int32_t *buf;


	if (xdrs->x_op == XDR_ENCODE) {
		 if (!xdr_post_op_attr (xdrs, &objp->obj_attributes))
			 return FALSE;
		 if (!xdr_uint32 (xdrs, &objp->linkmax))
			 return FALSE;
		 if (!xdr_uint32 (xdrs, &objp->name_max))
			 return FALSE;
		buf = XDR_INLINE (xdrs, 4 * BYTES_PER_XDR_UNIT);
		if (buf == NULL) {
			 if (!xdr_bool (xdrs, &objp->no_trunc))
				 return FALSE;
			 if (!xdr_bool (xdrs, &objp->chown_restricted))
				 return FALSE;
			 if (!xdr_bool (xdrs, &objp->case_insensitive))
				 return FALSE;
			 if (!xdr_bool (xdrs, &objp->case_preserving))
				 return FALSE;
		} else {
			IXDR_PUT_BOOL(buf, objp->no_trunc);
			IXDR_PUT_BOOL(buf, objp->chown_restricted);
			IXDR_PUT_BOOL(buf, objp->case_insensitive);
			IXDR_PUT_BOOL(buf, objp->case_preserving);
		}
		return TRUE;
	} else if (xdrs->x_op == XDR_DECODE) {
		 if (!xdr_post_op_attr (xdrs, &objp->obj_attributes))
			 return FALSE;
		 if (!xdr_uint32 (xdrs, &objp->linkmax))
			 return FALSE;
		 if (!xdr_uint32 (xdrs, &objp->name_max))
			 return FALSE;
		buf = XDR_INLINE (xdrs, 4 * BYTES_PER_XDR_UNIT);
		if (buf == NULL) {
			 if (!xdr_bool (xdrs, &objp->no_trunc))
				 return FALSE;
			 if (!xdr_bool (xdrs, &objp->chown_restricted))
				 return FALSE;
			 if (!xdr_bool (xdrs, &objp->case_insensitive))
				 return FALSE;
			 if (!xdr_bool (xdrs, &objp->case_preserving))
				 return FALSE;
		} else {
			objp->no_trunc = IXDR_GET_BOOL(buf);
			objp->chown_restricted = IXDR_GET_BOOL(buf);
			objp->case_insensitive = IXDR_GET_BOOL(buf);
			objp->case_preserving = IXDR_GET_BOOL(buf);
		}
	 return TRUE;
	}

	 if (!xdr_post_op_attr (xdrs, &objp->obj_attributes))
		 return FALSE;
	 if (!xdr_uint32 (xdrs, &objp->linkmax))
		 return FALSE;
	 if (!xdr_uint32 (xdrs, &objp->name_max))
		 return FALSE;
	 if (!xdr_bool (xdrs, &objp->no_trunc))
		 return FALSE;
	 if (!xdr_bool (xdrs, &objp->chown_restricted))
		 return FALSE;
	 if (!xdr_bool (xdrs, &objp->case_insensitive))
		 return FALSE;
	 if (!xdr_bool (xdrs, &objp->case_preserving))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_pathconf3res (XDR *xdrs, pathconf3res *objp)
{
	//register int32_t *buf;

	 if (!xdr_nfsstat3 (xdrs, &objp->status))
		 return FALSE;
	switch (objp->status) {
	case NFS3_OK:
		 if (!xdr_pathconf3resok (xdrs, &objp->pathconf3res_u.resok))
			 return FALSE;
		break;
	default:
		 if (!xdr_post_op_attr (xdrs, &objp->pathconf3res_u.resfail))
			 return FALSE;
		break;
	}
	return TRUE;
}

bool_t
xdr_commit3args (XDR *xdrs, commit3args *objp)
{
	//register int32_t *buf;

	 if (!xdr_fhandle3 (xdrs, &objp->file))
		 return FALSE;
	 if (!xdr_uint64 (xdrs, &objp->offset))
		 return FALSE;
	 if (!xdr_uint32 (xdrs, &objp->count))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_commit3resok (XDR *xdrs, commit3resok *objp)
{
	//register int32_t *buf;

	 if (!xdr_wcc_data (xdrs, &objp->file_wcc))
		 return FALSE;
	 if (!xdr_writeverf3 (xdrs, objp->verf))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_commit3res (XDR *xdrs, commit3res *objp)
{
	//register int32_t *buf;

	 if (!xdr_nfsstat3 (xdrs, &objp->status))
		 return FALSE;
	switch (objp->status) {
	case NFS3_OK:
		 if (!xdr_commit3resok (xdrs, &objp->commit3res_u.resok))
			 return FALSE;
		break;
	default:
		 if (!xdr_wcc_data (xdrs, &objp->commit3res_u.resfail))
			 return FALSE;
		break;
	}
	return TRUE;
}
