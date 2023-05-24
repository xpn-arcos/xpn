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
 * @file nfi_local_err.h
 * @brief Header file to 'TODO'.
 *
 * Header file to 'TODO'.
 *
 * @authors Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
 * @date  Jul 22, 2021
 * @bug No known bugs.
 */

#ifndef _NFI_LOCAL_ERR_H_
#define _NFI_LOCAL_ERR_H_

#ifdef __cplusplus
extern "C"
{
#endif

	enum nfi_local_err
	{
		LOCALERR_PARAM = 0,
		LOCALERR_MEMORY = 1,
		LOCALERR_URL = 2,
		LOCALERR_MNTCONNECTION = 3,
		LOCALERR_MOUNT = 4,
		LOCALERR_NFSCONNECTION = 5,
		LOCALERR_GETATTR = 6,
		LOCALERR_LOOKUP = 7,
		LOCALERR_READ = 8,
		LOCALERR_WRITE = 9,
		LOCALERR_CREATE = 10,
		LOCALERR_REMOVE = 11,
		LOCALERR_MKDIR = 12,
		LOCALERR_READDIR = 13,
		LOCALERR_STATFS = 14,
		LOCALERR_NOTDIR = 15,
	};

	void local_err(int err);

#ifdef __cplusplus
}
#endif

#endif
