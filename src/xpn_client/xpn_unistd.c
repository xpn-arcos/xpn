
  /*
   *  Copyright 2000-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
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
      #include "xpn_client/xpn/xpn_simple/xpn_simple_lib.h"
      #include "xpn_api_mutex.h"


  /* ... Functions / Funciones ......................................... */

    //
    // init - destroy
    //

    int xpn_init ( void )
    {
      int ret = -1 ;

      XPN_API_LOCK() ;
      ret = xpn_simple_init() ;
      XPN_API_UNLOCK() ;

      return ret ;
    }

    int xpn_destroy ( void )
    {
      int ret = -1 ;

      XPN_API_LOCK() ;
      ret = xpn_simple_destroy() ;
      XPN_API_UNLOCK() ;

      return ret ;
    }


    //
    // open - close - creat
    //

    int xpn_creat ( const char *path, mode_t perm )
    {
      int ret = -1 ;

      XPN_API_LOCK() ;
      ret = xpn_simple_creat(path, perm) ;
      XPN_API_UNLOCK() ;

      return ret ;
    }

    int xpn_open ( const char *path, int flags, ... )
    {
      va_list ap ;
      int     ret  = -1 ;
      mode_t  mode = 0 ;

      // get mode
      if ((flags & O_CREAT) > 0)
      {
        va_start(ap, flags);
        mode = va_arg(ap, mode_t);
        va_end(ap);
      }

      // call simple_open
      XPN_API_LOCK() ;
      ret = xpn_simple_open(path, flags, mode) ;
      XPN_API_UNLOCK() ;

      // return ret
      return ret ;
    }

    int xpn_close ( int fd )
    {
      int ret = -1 ;

      XPN_API_LOCK() ;
      ret = xpn_simple_close(fd) ;
      XPN_API_UNLOCK() ;

      return ret ;
    }


    //
    // read - write - lseek
    //

    ssize_t xpn_read  ( int fd, void *buffer, size_t size )
    {
      ssize_t ret = -1 ;

      XPN_API_LOCK() ;
      ret = xpn_simple_read(fd, buffer, size) ;
      XPN_API_UNLOCK() ;

      return ret ;
    }

    ssize_t xpn_write ( int fd, const void *buffer, size_t size )
    {
      ssize_t ret = -1 ;

      XPN_API_LOCK() ;
      ret = xpn_simple_write(fd, buffer, size) ;
      XPN_API_UNLOCK() ;

      return ret ;
    }

    off_t   xpn_lseek ( int fd, off_t offset, int flag )
    {
      off_t ret = (off_t) -1 ;

      XPN_API_LOCK() ;
      ret = xpn_simple_lseek(fd, offset, flag) ;
      XPN_API_UNLOCK() ;

      return ret ;
    }


    //
    // cwd - chdir
    //

    char* xpn_getcwd ( char *path, size_t size )
    {
      char * ret = NULL ;

      XPN_API_LOCK() ;
      ret = xpn_simple_getcwd(path, size) ;
      XPN_API_UNLOCK() ;

      return ret ;
    }

    int xpn_chdir(char *path)
    {
      int ret = -1 ;

      XPN_API_LOCK() ;
      ret = xpn_simple_chdir(path) ;
      XPN_API_UNLOCK() ;

      return ret ;
    }


    //
    // mkdir - rmdir
    //

    int xpn_mkdir ( const char *path, mode_t perm )
    {
      int ret = -1 ;

      XPN_API_LOCK() ;
      ret = xpn_simple_mkdir(path, perm) ;
      XPN_API_UNLOCK() ;

      return ret ;
    }

    int xpn_rmdir(const char *path)
    {
      int ret = -1 ;

      XPN_API_LOCK() ;
      ret = xpn_simple_rmdir(path) ;
      XPN_API_UNLOCK() ;

      return ret ;
    }


    //
    // opendir - closedir - readdir
    //

    DIR *xpn_opendir ( const char *path )
    {
      DIR *ret = NULL ;

      XPN_API_LOCK() ;
      ret = xpn_simple_opendir(path) ;
      XPN_API_UNLOCK() ;

      return ret ;
    }

    struct dirent* xpn_readdir ( DIR *dirp )
    {
      struct dirent* ret = NULL ;

      XPN_API_LOCK() ;
      ret = xpn_simple_readdir(dirp) ;
      XPN_API_UNLOCK() ;

      return ret ;
    }

    int  xpn_closedir ( DIR *dirp )
    {
      int ret = -1 ;

      XPN_API_LOCK() ;
      ret = xpn_simple_closedir(dirp) ;
      XPN_API_UNLOCK() ;

      return ret ;
    }

    void  xpn_rewinddir ( DIR *dirp )
    {
      XPN_API_LOCK() ;
      xpn_simple_rewinddir(dirp) ;
      XPN_API_UNLOCK() ;
    }


    //
    // unlink - rename - etc.
    //

    int xpn_unlink ( const char *path )
    {
      int ret = -1 ;

      XPN_API_LOCK() ;
      ret = xpn_simple_unlink(path) ;
      XPN_API_UNLOCK() ;

      return ret ;
    }

    int xpn_rename ( const char *path, const char *newpath )
    {
      int ret = -1 ;

      XPN_API_LOCK() ;
      ret = xpn_simple_rename(path, newpath) ;
      XPN_API_UNLOCK() ;

      return ret ;
    }

    int xpn_truncate ( const char *path,  off_t length )
    {
      int ret = -1 ;

      XPN_API_LOCK() ;
      ret = xpn_simple_truncate(path, length) ;
      XPN_API_UNLOCK() ;

      return ret ;
    }

    int xpn_ftruncate ( int fd,  off_t length )
    {
      int ret = -1 ;

      XPN_API_LOCK() ;
      ret = xpn_simple_ftruncate(fd, length) ;
      XPN_API_UNLOCK() ;

      return ret ;
    }

    int xpn_stat (const char *path, struct stat *sb )
    {
      int ret = -1 ;

      XPN_API_LOCK() ;
      ret = xpn_simple_stat(path, sb) ;
      XPN_API_UNLOCK() ;

      return ret ;
    }

    int xpn_fstat ( int fd, struct stat *sb )
    {
      int ret = -1 ;

      XPN_API_LOCK() ;
      ret = xpn_simple_fstat(fd, sb) ;
      XPN_API_UNLOCK() ;

      return ret ;
    }

    int xpn_chown ( const char *path,  uid_t owner,  gid_t group )
    {
      int ret = -1 ;

      XPN_API_LOCK() ;
      ret = xpn_simple_chown(path, owner, group) ;
      XPN_API_UNLOCK() ;

      return ret ;
    }

    int xpn_fchown (int  fd,  uid_t owner,  gid_t group )
    {
      int ret = -1 ;

      XPN_API_LOCK() ;
      ret = xpn_simple_fchown(fd, owner, group) ;
      XPN_API_UNLOCK() ;

      return ret ;
    }

    int xpn_chmod ( const char *path,  mode_t mode )
    {
      int ret = -1 ;

      XPN_API_LOCK() ;
      ret = xpn_simple_chmod(path, mode) ;
      XPN_API_UNLOCK() ;

      return ret ;
    }

    int xpn_fchmod ( int fd,  mode_t mode )
    {
      int ret = -1 ;

      XPN_API_LOCK() ;
      ret = xpn_simple_fchmod(fd, mode) ;
      XPN_API_UNLOCK() ;

      return ret ;
    }


    //
    // dup - dup2
    //

    int xpn_dup ( int fd )
    {
      int ret = -1 ;

      XPN_API_LOCK() ;
      ret = xpn_simple_dup(fd) ;
      XPN_API_UNLOCK() ;

      return ret ;
    }

    int xpn_dup2 ( int fd, int fd2 )
    {
      int ret = -1 ;

      XPN_API_LOCK() ;
      ret = xpn_simple_dup2(fd, fd2) ;
      XPN_API_UNLOCK() ;

      return ret ;
    }


   /* ................................................................... */

