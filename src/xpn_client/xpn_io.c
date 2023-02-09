
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


   /* ... Include / Inclusion ........................................... */

      #include "xpn.h"


   /* ... Functions / Funciones ......................................... */


      ssize_t xpn_read  ( int fd, void *buffer, size_t size )
      {
	  return xpn_simple_read(fd, buffer, size) ;
      }

      ssize_t xpn_write ( int fd, const void *buffer, size_t size )
      {
	  return xpn_simple_write(fd, buffer, size) ;
      }

      off_t   xpn_lseek ( int fd, off_t offset, int flag )
      {
          return xpn_simple_lseek(fd, offset, flag) ;
      }


   /* ................................................................... */

