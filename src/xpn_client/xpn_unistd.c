
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


/* ... Const / Const ................................................. */


/* ... Global variables / Variables globales ........................ */


/* ... Functions / Funciones ......................................... */

int xpn_init ( void )
{
  int ret = -1;

  debug_info("[XPN_UNISTD] [xpn_init] >> Begin\n");

  XPN_API_LOCK();
  ret = xpn_simple_init();
  XPN_API_UNLOCK();

  debug_info("[XPN_UNISTD] [xpn_init] >> End\n");

  return ret;
}

int xpn_destroy ( void )
{
  int ret = -1;

  debug_info("[XPN_UNISTD] [xpn_destroy] >> Begin\n");

  XPN_API_LOCK();
  ret = xpn_simple_destroy();
  XPN_API_UNLOCK();

  debug_info("[XPN_UNISTD] [xpn_destroy] >> End\n");

  return ret;
}

int xpn_creat ( const char *path, mode_t perm )
{
  int ret = -1;

  debug_info("[XPN_UNISTD] [xpn_creat] >> Begin\n");

  XPN_API_LOCK();
  ret = xpn_simple_creat(path, perm);
  XPN_API_UNLOCK();

  debug_info("[XPN_UNISTD] [xpn_creat] >> End\n");

  return ret;
}

int xpn_open ( const char *path, int flags, ... )
{
  va_list ap;
  int     ret  = -1;
  mode_t  mode = 0;

  debug_info("[XPN_UNISTD] [xpn_open] >> Begin\n");

  // get mode
  if ((flags & O_CREAT) > 0)
  {
    va_start(ap, flags);
    mode = va_arg(ap, mode_t);
    va_end(ap);
  }

  // call simple_open
  XPN_API_LOCK();
  ret = xpn_simple_open(path, flags, mode);
  XPN_API_UNLOCK();

  debug_info("[XPN_UNISTD] [xpn_open] >> End\n");

  // return ret
  return ret;
}

int xpn_close ( int fd )
{
  int ret = -1;

  debug_info("[XPN_UNISTD] [xpn_close] >> Begin\n");

  XPN_API_LOCK();
  ret = xpn_simple_close(fd);
  XPN_API_UNLOCK();

  debug_info("[XPN_UNISTD] [xpn_close] >> End\n");

  return ret;
}

ssize_t xpn_read  ( int fd, void *buffer, size_t size )
{
  ssize_t ret = -1;

  debug_info("[XPN_UNISTD] [xpn_read] >> Begin\n");

  XPN_API_LOCK();
  ret = xpn_simple_read(fd, buffer, size);
  XPN_API_UNLOCK();

  debug_info("[XPN_UNISTD] [xpn_read] >> End\n");

  return ret;
}

ssize_t xpn_write ( int fd, const void *buffer, size_t size )
{
  ssize_t ret = -1;

  debug_info("[XPN_UNISTD] [xpn_write] >> Begin\n");

  XPN_API_LOCK();
  ret = xpn_simple_write(fd, buffer, size);
  XPN_API_UNLOCK();

  debug_info("[XPN_UNISTD] [xpn_write] >> End\n");

  return ret;
}

off_t   xpn_lseek ( int fd, off_t offset, int flag )
{
  off_t ret = (off_t) -1;

  debug_info("[XPN_UNISTD] [xpn_lseek] >> Begin\n");

  XPN_API_LOCK();
  ret = xpn_simple_lseek(fd, offset, flag);
  XPN_API_UNLOCK();

  debug_info("[XPN_UNISTD] [xpn_lseek] >> End\n");

  return ret;
}

char* xpn_getcwd ( char *path, size_t size )
{
  char * ret = NULL;

  debug_info("[XPN_UNISTD] [xpn_getcwd] >> Begin\n");

  XPN_API_LOCK();
  ret = xpn_simple_getcwd(path, size);
  XPN_API_UNLOCK();

  debug_info("[XPN_UNISTD] [xpn_getcwd] >> End\n");

  return ret;
}

int xpn_chdir(char *path)
{
  int ret = -1;

  debug_info("[XPN_UNISTD] [xpn_chdir] >> Begin\n");

  XPN_API_LOCK();
  ret = xpn_simple_chdir(path);
  XPN_API_UNLOCK();

  debug_info("[XPN_UNISTD] [xpn_chdir] >> End\n");

  return ret;
}

int xpn_mkdir ( const char *path, mode_t perm )
{
  int ret = -1;

  debug_info("[XPN_UNISTD] [xpn_mkdir] >> Begin\n");

  XPN_API_LOCK();
  ret = xpn_simple_mkdir(path, perm);
  XPN_API_UNLOCK();

  debug_info("[XPN_UNISTD] [xpn_mkdir] >> End\n");

  return ret;
}

int xpn_rmdir(const char *path)
{
  int ret = -1;

  debug_info("[XPN_UNISTD] [xpn_rmdir] >> Begin\n");

  XPN_API_LOCK();
  ret = xpn_simple_rmdir(path);
  XPN_API_UNLOCK();

  debug_info("[XPN_UNISTD] [xpn_rmdir] >> End\n");

  return ret;
}

DIR *xpn_opendir ( const char *path )
{
  DIR *ret = NULL;

  debug_info("[XPN_UNISTD] [xpn_opendir] >> Begin\n");

  XPN_API_LOCK();
  ret = xpn_simple_opendir(path);
  XPN_API_UNLOCK();

  debug_info("[XPN_UNISTD] [xpn_opendir] >> End\n");

  return ret;
}

struct dirent* xpn_readdir ( DIR *dirp )
{
  struct dirent* ret = NULL;

  debug_info("[XPN_UNISTD] [xpn_readdir] >> Begin\n");

  XPN_API_LOCK();
  ret = xpn_simple_readdir(dirp);
  XPN_API_UNLOCK();

  debug_info("[XPN_UNISTD] [xpn_readdir] >> End\n");

  return ret;
}

int xpn_closedir ( DIR *dirp )
{
  int ret = -1;

  debug_info("[XPN_UNISTD] [xpn_closedir] >> Begin\n");

  XPN_API_LOCK();
  ret = xpn_simple_closedir(dirp);
  XPN_API_UNLOCK();

  debug_info("[XPN_UNISTD] [xpn_closedir] >> End\n");

  return ret;
}

void xpn_rewinddir ( DIR *dirp )
{
  debug_info("[XPN_UNISTD] [xpn_rewinddir] >> Begin\n");

  XPN_API_LOCK();
  xpn_simple_rewinddir(dirp);
  XPN_API_UNLOCK();

  debug_info("[XPN_UNISTD] [xpn_rewinddir] >> End\n");
}

int xpn_unlink ( const char *path )
{
  int ret = -1;

  debug_info("[XPN_UNISTD] [xpn_unlink] >> Begin\n");

  XPN_API_LOCK();
  ret = xpn_simple_unlink(path);
  XPN_API_UNLOCK();

  debug_info("[XPN_UNISTD] [xpn_unlink] >> End\n");

  return ret;
}

int xpn_rename ( const char *path, const char *newpath )
{
  int ret = -1;

  debug_info("[XPN_UNISTD] [xpn_rename] >> Begin\n");

  XPN_API_LOCK();
  ret = xpn_simple_rename(path, newpath);
  XPN_API_UNLOCK();

  debug_info("[XPN_UNISTD] [xpn_rename] >> End\n");

  return ret;
}

int xpn_truncate ( const char *path,  off_t length )
{
  int ret = -1;

  debug_info("[XPN_UNISTD] [xpn_truncate] >> Begin\n");

  XPN_API_LOCK();
  ret = xpn_simple_truncate(path, length);
  XPN_API_UNLOCK();

  debug_info("[XPN_UNISTD] [xpn_truncate] >> End\n");

  return ret;
}

int xpn_ftruncate ( int fd,  off_t length )
{
  int ret = -1;

  debug_info("[XPN_UNISTD] [xpn_ftruncate] >> Begin\n");

  XPN_API_LOCK();
  ret = xpn_simple_ftruncate(fd, length);
  XPN_API_UNLOCK();

  debug_info("[XPN_UNISTD] [xpn_ftruncate] >> End\n");

  return ret;
}

int xpn_stat (const char *path, struct stat *sb )
{
  int ret = -1;

  debug_info("[XPN_UNISTD] [xpn_stat] >> Begin\n");

  XPN_API_LOCK();
  ret = xpn_simple_stat(path, sb);
  XPN_API_UNLOCK();

  debug_info("[XPN_UNISTD] [xpn_stat] >> End\n");

  return ret;
}

int xpn_fstat ( int fd, struct stat *sb )
{
  int ret = -1;

  debug_info("[XPN_UNISTD] [xpn_fstat] >> Begin\n");

  XPN_API_LOCK();
  ret = xpn_simple_fstat(fd, sb);
  XPN_API_UNLOCK();

  debug_info("[XPN_UNISTD] [xpn_fstat] >> End\n");

  return ret;
}

int xpn_chown ( const char *path,  uid_t owner,  gid_t group )
{
  int ret = -1;

  debug_info("[XPN_UNISTD] [xpn_chown] >> Begin\n");

  XPN_API_LOCK();
  ret = xpn_simple_chown(path, owner, group);
  XPN_API_UNLOCK();

  debug_info("[XPN_UNISTD] [xpn_chown] >> End\n");

  return ret;
}

int xpn_fchown (int  fd,  uid_t owner,  gid_t group )
{
  int ret = -1;

  debug_info("[XPN_UNISTD] [xpn_fchown] >> Begin\n");

  XPN_API_LOCK();
  ret = xpn_simple_fchown(fd, owner, group);
  XPN_API_UNLOCK();

  debug_info("[XPN_UNISTD] [xpn_fchown] >> End\n");

  return ret;
}

int xpn_chmod ( const char *path,  mode_t mode )
{
  int ret = -1;

  debug_info("[XPN_UNISTD] [xpn_chmod] >> Begin\n");

  XPN_API_LOCK();
  ret = xpn_simple_chmod(path, mode);
  XPN_API_UNLOCK();

  debug_info("[XPN_UNISTD] [xpn_chmod] >> End\n");

  return ret;
}

int xpn_fchmod ( int fd,  mode_t mode )
{
  int ret = -1;

  debug_info("[XPN_UNISTD] [xpn_fchmod] >> Begin\n");

  XPN_API_LOCK();
  ret = xpn_simple_fchmod(fd, mode);
  XPN_API_UNLOCK();

  debug_info("[XPN_UNISTD] [xpn_fchmod] >> End\n");

  return ret;
}

int xpn_dup ( int fd )
{
  int ret = -1;

  debug_info("[XPN_UNISTD] [xpn_dup] >> Begin\n");

  XPN_API_LOCK();
  ret = xpn_simple_dup(fd);
  XPN_API_UNLOCK();

  debug_info("[XPN_UNISTD] [xpn_dup] >> End\n");

  return ret;
}

int xpn_dup2 ( int fd, int fd2 )
{
  int ret = -1;

  debug_info("[XPN_UNISTD] [xpn_dup2] >> Begin\n");

  XPN_API_LOCK();
  ret = xpn_simple_dup2(fd, fd2);
  XPN_API_UNLOCK();

  debug_info("[XPN_UNISTD] [xpn_dup2] >> End\n");

  return ret;
}


/* ................................................................... */
