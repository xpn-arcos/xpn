
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


      int xpn_creat(const char *path, mode_t perm)
      {
          return xpn_simple_creat(path, perm) ;
      }

/*
      int xpn_open(const char *path, int flags , ...)
      {
          TODO: pack ... into stdarg

          return xpn_simple_open2(const char *path, int flags , args) ;
      }
*/

      int xpn_close(int fd)
      {
          return xpn_simple_close(fd) ;
      }


      int xpn_unlink(const char *path)
      {
          return xpn_simple_unlink(path) ;
      }

      int xpn_rename(const char *path, const char *newpath)
      {
          return xpn_simple_rename(path, newpath) ;
      }

      int xpn_truncate( const char *path,  off_t length)
      {
          return xpn_simple_truncate(path, length) ;
      }

      int xpn_ftruncate( int fd,  off_t length)
      {
          return xpn_simple_ftruncate(fd, length) ;
      }

      int xpn_stat(const char *path, struct stat *sb)
      {
          return xpn_simple_stat(path, sb) ;
      }

      int xpn_fstat(int fd, struct stat *sb)
      {
          return xpn_simple_fstat(fd, sb) ;
      }

      int xpn_chown( const char *path,  uid_t owner,  gid_t group)
      {
          return xpn_simple_chown(path, owner, group) ;
      }

      int xpn_fchown(int  fd,  uid_t owner,  gid_t group)
      {
          return xpn_simple_fchown(fd, owner, group) ;
      }

      int xpn_chmod( const char *path,  mode_t mode)
      {
          return xpn_simple_chmod(path, mode) ;
      }

      int xpn_fchmod( int fd,  mode_t mode)
      {
          return xpn_simple_fchmod(fd, mode) ;
      }


      int xpn_dup(int fd)
      {
          return xpn_simple_dup(fd) ;
      }

      int xpn_dup2(int fd, int fd2)
      {
          return xpn_dup2(fd, fd2) ;
      }


   /* ................................................................... */

