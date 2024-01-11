 
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

extern int errno;

/* this interface must be changed */
void nfs_err(int err)
{
	switch(err)
	{
		case NFSERR_PARAM:
			errno = -1;
			break;
		
		case NFSERR_MEMORY:
			errno = -1;
			break;
			
		case NFSERR_URL:
			errno = -1;
			break;

		case NFSERR_MNTCONNECTION:
			errno = -1;
			break;

		case NFSERR_MOUNT:
			errno = -1;
			break;

		case NFSERR_NFSCONNECTION:
			errno = -1;
			break;
			
		case NFSERR_GETATTR:
			errno = -1;
			break;
	}
};
