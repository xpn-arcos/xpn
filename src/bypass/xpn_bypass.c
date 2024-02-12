
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

#include "xpn_bypass.h"


/* ... Const / Const ................................................. */


/* ... Global variables / Variables globales ........................ */

/**
 * This variable indicates if expand has already been initialized or not.
 *   0 indicates that expand has NOT been initialized yet.
 *   1 indicates that expand has already been initialized.
 */
static int xpn_adaptor_initCalled = 0;
static int xpn_adaptor_initCalled_getenv = 0; // env variable obtained

/**
 * This variable contains the prefix which will be considerated as expand partition.
 */
char *xpn_adaptor_partition_prefix = "/tmp/expand/"; // Original --> xpn://
int   xpn_prefix_change_verified = 0;


/* ... Auxiliar functions / Funciones auxiliares ......................................... */

/**
 * Check that the path contains the prefix of XPN
 */
int is_xpn_prefix   ( const char * path )
{
  if (0 == xpn_prefix_change_verified)
  {
    xpn_prefix_change_verified = 1;

    char * env_prefix = getenv("XPN_MOUNT_POINT");
    if (env_prefix != NULL)
    {
      xpn_adaptor_partition_prefix = env_prefix;
    }
  }
  
  const char *prefix = (const char *)xpn_adaptor_partition_prefix;

  return ( !strncmp(prefix, path, strlen(prefix)) && strlen(path) > strlen(prefix) );
}

/**
 * Skip the XPN prefix
 */
const char * skip_xpn_prefix ( const char * path )
{
  return (const char *)(path + strlen(xpn_adaptor_partition_prefix));
}


/**
 * File descriptors table management
 */
struct generic_fd * fdstable = NULL;
long   fdstable_size = 0L;
long   fdstable_first_free = 0L;

void fdstable_realloc ( void )
{
  long old_size = fdstable_size;
  struct generic_fd * fdstable_aux = fdstable;

  debug_info("[BYPASS] >> Begin fdstable_realloc....\n");

  if ( NULL == fdstable )
  {
    fdstable_size = (long) MAX_FDS;
    fdstable = (struct generic_fd *) malloc(fdstable_size * sizeof(struct generic_fd));
  }
  else
  {
    fdstable_size = fdstable_size * 2;
    fdstable = (struct generic_fd *) realloc((struct generic_fd *)fdstable, fdstable_size * sizeof(struct generic_fd));
  }

  if ( NULL == fdstable )
  {
    debug_error( "[BYPASS:%s:%d] Error: out of memory\n", __FILE__, __LINE__);
    if (fdstable_aux != NULL) {
      free(fdstable_aux);
    }

    exit(-1);
  }
  
  for (int i = old_size; i < fdstable_size; ++i)
  {
    fdstable[i].type = FD_FREE;
    fdstable[i].real_fd = -1;
    fdstable[i].is_file = -1;
  }

  debug_info("[BYPASS] << After fdstable_realloc....\n");
}

void fdstable_init ( void )
{
  debug_info("[BYPASS] >> Begin fdstable_init....\n");

  fdstable_realloc();

  debug_info("[BYPASS] << After fdstable_init....\n");
}

struct generic_fd fdstable_get ( int fd )
{
  struct generic_fd ret;
  
  debug_info("[BYPASS] >> Begin fdstable_get....\n");
  debug_info("[BYPASS]    1) fd  => %d\n", fd);

  if ((NULL != fdstable) && (fd >= PLUSXPN))
  {
    fd = fd - PLUSXPN;
    ret = fdstable[fd];
  }
  else
  {
    ret.type = FD_SYS;
    ret.real_fd = fd;
  }

  debug_info("[BYPASS]\t fdstable_get -> type: %d ; real_fd: %d\n", ret.type, ret.real_fd);
  debug_info("[BYPASS] << After fdstable_get....\n");

  return ret;
}

int fdstable_put ( struct generic_fd fd )
{
  debug_info("[BYPASS] >> Begin fdstable_put....\n");

  for (int i = fdstable_first_free; i < fdstable_size; ++i)
  {
    if ( fdstable[i].type == FD_FREE ) {
      fdstable[i] = fd;
      fdstable_first_free = (long)(i + 1);

      debug_info("[BYPASS]\t fdstable_put -> fd %d ; type: %d ; real_fd: %d\n", i + PLUSXPN, fdstable[i].type, fdstable[i].real_fd);
      debug_info("[BYPASS] << After fdstable_put....\n");

      return i + PLUSXPN;
    }
  }

  long old_size = fdstable_size;

  fdstable_realloc();

  if ( fdstable[old_size].type == FD_FREE ) {
    fdstable[old_size] = fd;

    debug_info("[BYPASS]\t fdstable_put -> fd %ld ; type: %d ; real_fd: %d\n", old_size + PLUSXPN, fdstable[old_size].type, fdstable[old_size].real_fd);
    debug_info("[BYPASS] << After fdstable_put....\n");

    return old_size + PLUSXPN;
  }

  debug_info("[BYPASS]\t fdstable_put -> -1\n");
  debug_info("[BYPASS] << After fdstable_put....\n");

  return -1;
}

int fdstable_remove ( int fd )
{
  debug_info("[BYPASS] >> Begin fdstable_remove....\n");
  debug_info("[BYPASS]    1) fd  => %d\n", fd);

  if (fd < PLUSXPN) {
    debug_info("[BYPASS] << After fdstable_remove....\n");

    return 0;
  }

  fd = fd - PLUSXPN;
  fdstable[fd].type    = FD_FREE;
  fdstable[fd].real_fd = -1;
  fdstable[fd].is_file = -1;

  if (fd < fdstable_first_free) {
    fdstable_first_free = fd;
  }

  debug_info("[BYPASS] << After fdstable_remove....\n");

  return 0;
}

int add_xpn_file_to_fdstable ( int fd )
{
  struct stat st;
  struct generic_fd virtual_fd;
  
  debug_info("[BYPASS] >> Begin add_xpn_file_to_fdstable....\n");
  debug_info("[BYPASS]    1) fd  => %d\n", fd);

  int ret = fd;

  // check arguments
  if (fd < 0) {
    debug_info("[BYPASS]\t add_xpn_file_to_fdstable -> %d\n", ret);
    debug_info("[BYPASS] << After add_xpn_file_to_fdstable....\n");

    return ret;
  } 

  // fstat(fd...
  xpn_fstat(fd, &st);

  // setup virtual_fd
  virtual_fd.type    = FD_XPN;
  virtual_fd.real_fd = fd;
  virtual_fd.is_file = (S_ISDIR(st.st_mode)) ? 0 : 1;

  // insert into fdstable
  ret = fdstable_put ( virtual_fd );

  debug_info("[BYPASS]\t add_xpn_file_to_fdstable -> %d\n", ret);
  debug_info("[BYPASS] << After add_xpn_file_to_fdstable....\n");

  return ret;
}


/**
 * Dir table management
 */
DIR ** fdsdirtable = NULL;
long   fdsdirtable_size = 0L;
long   fdsdirtable_first_free = 0L;

void fdsdirtable_realloc ( void )
{
  long          old_size = fdsdirtable_size;
  DIR ** fdsdirtable_aux = fdsdirtable;
  
  debug_info("[BYPASS] >> Begin fdsdirtable_realloc....\n");
  
  if ( NULL == fdsdirtable )
  {
    fdsdirtable_size = (long) MAX_DIRS;
    fdsdirtable = (DIR **) malloc(MAX_DIRS * sizeof(DIR *));
  }
  else
  {
    fdsdirtable_size = fdsdirtable_size * 2;
    fdsdirtable = (DIR **) realloc((DIR **)fdsdirtable, fdsdirtable_size * sizeof(DIR *));
  }

  if ( NULL == fdsdirtable )
  {
    debug_error( "[BYPASS:%s:%d] Error: out of memory\n", __FILE__, __LINE__);
    if (NULL != fdsdirtable_aux) {
      free(fdsdirtable_aux);
    }

    exit(-1);
  }
  
  for (int i = old_size; i < fdsdirtable_size; ++i) {
    fdsdirtable[i] = NULL;
  }

  debug_info("[BYPASS] << After fdsdirtable_realloc....\n");
}

void fdsdirtable_init ( void )
{
  debug_info("[BYPASS] >> Begin fdsdirtable_init....\n");

  fdsdirtable_realloc();

  debug_info("[BYPASS] << After fdsdirtable_init....\n");
}

int fdsdirtable_get ( DIR * dir )
{
  debug_info("[BYPASS] >> Begin fdsdirtable_get....\n");
  debug_info("[BYPASS]    1) dir  => %p\n", dir);

  for (int i = 0; i < fdsdirtable_size; ++i)
  {
    if ( fdsdirtable[i] == dir ) {
      debug_info("[BYPASS]\t fdsdirtable_get -> %d\n", i);
      debug_info("[BYPASS] << After fdsdirtable_get....\n");

      return i;
    }
  }

  debug_info("[BYPASS]\t fdsdirtable_get -> -1\n");
  debug_info("[BYPASS] << After fdsdirtable_get....\n");

  return -1;
}

int fdsdirtable_put ( DIR * dir )
{
  // preparing the "file side" of the directory
  struct generic_fd virtual_fd;
  int fd;
  int vfd;
  
  debug_info("[BYPASS] >> Begin fdsdirtable_put....\n");
  debug_info("[BYPASS]    1) dir  => %p\n", dir);

  fd = dirfd(dir);

  virtual_fd.type    = FD_XPN;
  virtual_fd.real_fd = fd;
  virtual_fd.is_file = 0;

  // insert into the dirtable (and fdstable)
  for (int i = fdsdirtable_first_free; i < fdsdirtable_size; ++i)
  {
    if (fdsdirtable[i] == NULL)
    {
      fdsdirtable[i] = dir;
      fdsdirtable_first_free = (long)(i + 1);

      vfd = fdstable_put ( virtual_fd );
      dir->fd = vfd;

      debug_info("[BYPASS]\t fdsdirtable_put -> 0\n");
      debug_info("[BYPASS] << After fdsdirtable_put....\n");

      return 0;
    }
  }

  long old_size = fdstable_size;
  fdsdirtable_realloc();

  if (fdsdirtable[old_size] == NULL)
  {
    fdsdirtable[old_size] = dir;
    fdsdirtable_first_free = (long)(old_size + 1);

    vfd = fdstable_put ( virtual_fd );
    dir->fd = vfd;

    debug_info("[BYPASS]\t fdsdirtable_put -> 0\n");
    debug_info("[BYPASS] << After fdsdirtable_put....\n");

    return 0;
  }

  debug_info("[BYPASS]\t fdsdirtable_put -> -1\n");
  debug_info("[BYPASS] << After fdsdirtable_put....\n");

  return -1;
}

int fdsdirtable_remove ( DIR * dir )
{
  debug_info("[BYPASS] >> Begin fdsdirtable_remove....\n");
  debug_info("[BYPASS]    1) dir  => %p\n", dir);

  for (int i = 0; i < fdsdirtable_size; ++i)
  {
    if (fdsdirtable[i] == dir)
    {
      fdstable_remove ( dir->fd );
      dir->fd = dir->fd - PLUSXPN;

      fdsdirtable[i] = NULL;

      if (i < fdsdirtable_first_free) {
        fdsdirtable_first_free = i;
      }

      debug_info("[BYPASS]\t fdsdirtable_remove -> 0\n");
      debug_info("[BYPASS] << After fdsdirtable_remove....\n");

      return 0;
    }
  }

  debug_info("[BYPASS]\t fdsdirtable_remove -> -1\n");
  debug_info("[BYPASS] << After fdsdirtable_remove....\n");

  return -1;
}

DIR fdsdirtable_getfd ( DIR * dir )
{
  DIR aux_dirp;
  
  debug_info("[BYPASS] >> Begin fdsdirtable_getfd....\n");
  debug_info("[BYPASS]    1) dir  => %p\n", dir);

  aux_dirp = *dir;
  
  struct generic_fd virtual_fd = fdstable_get ( aux_dirp.fd );
  aux_dirp.fd = virtual_fd.real_fd;

  debug_info("[BYPASS] << After fdsdirtable_getfd....\n");

  return aux_dirp;
}


/**
 * stat management
 */
int stat_to_stat64 ( struct stat64 *buf, struct stat *st )
{
  buf->st_dev     = (__dev_t)      st->st_dev;
  buf->st_ino     = (__ino64_t)    st->st_ino;
  buf->st_mode    = (__mode_t)     st->st_mode;
  buf->st_nlink   = (__nlink_t)    st->st_nlink;
  buf->st_uid     = (__uid_t)      st->st_uid;
  buf->st_gid     = (__gid_t)      st->st_gid;
  buf->st_rdev    = (__dev_t)      st->st_rdev;
  buf->st_size    = (__off64_t)    st->st_size;
  buf->st_blksize = (__blksize_t)  st->st_blksize;
  buf->st_blocks  = (__blkcnt64_t) st->st_blocks;
  buf->st_atime   = (__time_t)     st->st_atime;
  buf->st_mtime   = (__time_t)     st->st_mtime;
  buf->st_ctime   = (__time_t)     st->st_ctime;

  return 0;
}

int stat64_to_stat ( struct stat *buf, struct stat64 *st )
{
  buf->st_dev     = (__dev_t)     st->st_dev;
  buf->st_ino     = (__ino_t)     st->st_ino;
  buf->st_mode    = (__mode_t)    st->st_mode;
  buf->st_nlink   = (__nlink_t)   st->st_nlink;
  buf->st_uid     = (__uid_t)     st->st_uid;
  buf->st_gid     = (__gid_t)     st->st_gid;
  buf->st_rdev    = (__dev_t)     st->st_rdev;
  buf->st_size    = (__off_t)     st->st_size;
  buf->st_blksize = (__blksize_t) st->st_blksize;
  buf->st_blocks  = (__blkcnt_t)  st->st_blocks;
  buf->st_atime   = (__time_t)    st->st_atime;
  buf->st_mtime   = (__time_t)    st->st_mtime;
  buf->st_ctime   = (__time_t)    st->st_ctime;

  return 0;
}


/*
 * This function checks if expand has already been initialized.
 * If not, it initialize it.
 */
int xpn_adaptor_keepInit ( void )
{
  int    ret;
  char * xpn_adaptor_initCalled_env = NULL;
  
  debug_info("[BYPASS] >> Begin xpn_adaptor_keepInit....\n");

  if (0 == xpn_adaptor_initCalled_getenv)
  {
    xpn_adaptor_initCalled_env = getenv("INITCALLED");
    xpn_adaptor_initCalled     = 0;

    if (xpn_adaptor_initCalled_env != NULL) {
      xpn_adaptor_initCalled = atoi(xpn_adaptor_initCalled_env);
    }

    xpn_adaptor_initCalled_getenv = 1;
  }
  
  ret = 0;

  // If expand has not been initialized, then initialize it.
  if (0 == xpn_adaptor_initCalled)
  {
    xpn_adaptor_initCalled = 1; //TODO: Delete
    setenv("INITCALLED", "1", 1);

    debug_info("[BYPASS]\t Begin xpn_init()\n");

    fdstable_init ();
    fdsdirtable_init ();
    ret = xpn_init();

    debug_info("[BYPASS]\t After xpn_init() -> %d\n", ret);

    if (ret < 0)
    {
      debug_error( "ERROR: Expand xpn_init couldn't be initialized :-(\n");
      xpn_adaptor_initCalled = 0;
      setenv("INITCALLED", "0", 1);
    }
    else
    {
      xpn_adaptor_initCalled = 1;
      setenv("INITCALLED", "1", 1);
    }
  }

  debug_info("[BYPASS]\t xpn_adaptor_keepInit -> %d\n", ret);
  debug_info("[BYPASS] << After xpn_adaptor_keepInit....\n");

  return ret;
}


/* ... Functions / Funciones ......................................... */

// File API
int open ( const char *path, int flags, ... )
{
  int ret, fd;
  va_list ap;
  mode_t mode = 0;

  va_start(ap, flags);

  mode = va_arg(ap, mode_t);

  debug_info("[BYPASS] >> Begin open....\n");
  debug_info("[BYPASS]    1) Path  => %s\n", path);
  debug_info("[BYPASS]    2) Flags => %d\n", flags);
  debug_info("[BYPASS]    3) Mode  => %d\n", mode);

  // This if checks if variable path passed as argument starts with the expand prefix.
  if (is_xpn_prefix(path))
  {
    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    // It is an XPN partition, so we redirect the syscall to expand syscall
    debug_info("[BYPASS]\t xpn_open (%s,%o)\n",path + strlen(xpn_adaptor_partition_prefix), flags);

    if (mode != 0) {
      fd = xpn_open(skip_xpn_prefix(path), flags, mode);
    }
    else {
      fd = xpn_open(skip_xpn_prefix(path), flags);
    }

    debug_info("[BYPASS]\t xpn_open (%s,%o) -> %d\n", skip_xpn_prefix(path), flags, fd);

    ret = add_xpn_file_to_fdstable(fd);
  }
  // Not an XPN partition. We must link with the standard library.
  else 
  {
    debug_info("[BYPASS]\t dlsym_open (%s,%o,%o)\n", path, flags, mode);

    ret = dlsym_open2((char *)path, flags, mode);

    debug_info("[BYPASS]\t dlsym_open (%s,%o,%o) -> %d\n", path, flags, mode, ret);
  }

  va_end(ap);

  debug_info("[BYPASS] << After open....\n");

  return ret;
}


int open64 ( const char *path, int flags, ... )
{
  int fd, ret;
  va_list ap;
  mode_t mode = 0;

  va_start(ap, flags);

  mode = va_arg(ap, mode_t);

  debug_info("[BYPASS] >> Begin open64....\n");
  debug_info("[BYPASS]    1) Path  => %s\n", path);
  debug_info("[BYPASS]    2) flags => %d\n", flags);
  debug_info("[BYPASS]    3) mode  => %d\n", mode);

  // This if checks if variable path passed as argument starts with the expand prefix.
  if (is_xpn_prefix(path))
  {
    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    // It is an XPN partition, so we redirect the syscall to expand syscall
    debug_info("[BYPASS]\t xpn_open (%s,%o)\n",path + strlen(xpn_adaptor_partition_prefix), flags);

    if (mode != 0) {
      fd = xpn_open(skip_xpn_prefix(path), flags, mode);
    }
    else {
      fd = xpn_open(skip_xpn_prefix(path), flags);
    }

    debug_info("[BYPASS]\t xpn_open (%s,%o) -> %d\n", skip_xpn_prefix(path), flags, fd);

    ret = add_xpn_file_to_fdstable(fd);
  }
  // Not an XPN partition. We must link with the standard library
  else
  {
    debug_info("[BYPASS]\t dlsym_open64 (%s,%o,%o)\n", path, flags, mode);

    ret = dlsym_open64((char *)path, flags, mode);

    debug_info("[BYPASS]\t dlsym_open64 (%s,%o,%o) -> %d\n", path, flags, mode, ret);
  }

  va_end(ap);

  debug_info("[BYPASS] << After open64....\n");

  return ret;
}


#ifndef HAVE_ICC

int __open_2 ( const char *path, int flags, ... )
{
  int fd, ret;
  va_list ap;
  mode_t mode = 0;

  va_start(ap, flags);
  mode = va_arg(ap, mode_t);

  debug_info("[BYPASS] >> Begin __open_2....\n");
  debug_info("[BYPASS]    1) Path  => %s\n", path);
  debug_info("[BYPASS]    2) flags => %d\n", flags);
  debug_info("[BYPASS]    3) mode  => %d\n", mode);

  // This if checks if variable path passed as argument starts with the expand prefix.
  if (is_xpn_prefix(path))
  {
    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    // It is an XPN partition, so we redirect the syscall to expand syscall
    debug_info("[BYPASS]\t xpn_open (%s,%o)\n",path + strlen(xpn_adaptor_partition_prefix), flags);

    if (mode != 0) {
      fd=xpn_open(skip_xpn_prefix(path), flags, mode);
    }
    else {
      fd=xpn_open(skip_xpn_prefix(path), flags);
    }

    debug_info("[BYPASS]\t xpn_open (%s,%o) -> %d\n", skip_xpn_prefix(path), flags, fd);

    ret = add_xpn_file_to_fdstable(fd);
  }
  // Not an XPN partition. We must link with the standard library
  else
  {
    debug_info("[BYPASS]\t try to dlsym___open_2 %s\n", path);

    ret = dlsym___open_2((char *)path, flags);

    debug_info("[BYPASS]\t dlsym___open_2 %s -> %d\n", path, ret);
  }

  va_end(ap);

  debug_info("[BYPASS] << After __open_2....\n");

  return ret;
}

#endif

int creat ( const char *path, mode_t mode )
{
  int fd,ret;

  debug_info("[BYPASS] >> Begin creat....\n");

  // This if checks if variable path passed as argument starts with the expand prefix.
  if (is_xpn_prefix(path))
  {
    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    // It is an XPN partition, so we redirect the syscall to expand syscall
    debug_info("[BYPASS]\t try to creat %s", skip_xpn_prefix(path));

    fd  = xpn_creat((const char *)skip_xpn_prefix(path),mode);
    ret = add_xpn_file_to_fdstable(fd);

    debug_info("[BYPASS]\t creat %s -> %d", skip_xpn_prefix(path), ret);
  }
  // Not an XPN partition. We must link with the standard library
  else
  {
    debug_info("[BYPASS]\t try to dlsym_creat %s\n", path);

    ret = dlsym_creat(path, mode);

    debug_info("[BYPASS]\t dlsym_creat %s -> %d\n", path, ret);
  }

  debug_info("[BYPASS] << After creat....\n");
  return ret;
}

int ftruncate ( int fd, off_t length )
{
  debug_info("[BYPASS] >> Begin ftruncate...\n");

  int ret = -1;

  struct generic_fd virtual_fd = fdstable_get ( fd );

  // This if checks if variable fd passed as argument is a expand fd.
  if(virtual_fd.type == FD_XPN)
  {
    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    // It is an XPN partition, so we redirect the syscall to expand syscall
    debug_info("[BYPASS]\t try to xpn_ftruncate\n");

    ret = xpn_ftruncate(virtual_fd.real_fd, length);

    debug_info("[BYPASS]\t xpn_ftruncate -> %d\n", ret);
  }
  // Not an XPN partition. We must link with the standard library
  else
  {
    debug_info("[BYPASS]\t try to dlsym_ftruncate %d,%ld\n", fd, length);

    ret = dlsym_ftruncate(fd, length);

    debug_info("[BYPASS]\t dlsym_ftruncate %d,%ld -> %d\n", fd, length, ret);
  }

  debug_info("[BYPASS] << After ftruncate...\n");

  return ret;
}

ssize_t read ( int fd, void *buf, size_t nbyte )
{         
  ssize_t ret = -1;

  debug_info("[BYPASS] >> Begin read...\n");
  debug_info("[BYPASS]    * fd=%d\n",    fd);
  debug_info("[BYPASS]    * buf=%p\n",   buf);
  debug_info("[BYPASS]    * byte=%ld\n", nbyte);

  struct generic_fd virtual_fd = fdstable_get ( fd );

  // This if checks if variable fd passed as argument is a expand fd.
  if(virtual_fd.type == FD_XPN)
  {
    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    // It is an XPN partition, so we redirect the syscall to expand syscall
    if (virtual_fd.is_file == 0)
    {
      debug_error("[BYPASS:%s:%d] Error: is not a file\n", __FILE__, __LINE__);
      debug_info("[BYPASS] << After read...\n");

      errno = EISDIR;
      return -1;
    }

    debug_info("[BYPASS]\t try to xpn_read %d, %p, %ld\n", virtual_fd.real_fd, buf, nbyte);

    ret = xpn_read(virtual_fd.real_fd, buf, nbyte);

    debug_info("[BYPASS]\t xpn_read %d, %p, %ld -> %ld\n", virtual_fd.real_fd, buf, nbyte, ret);
  }
  // Not an XPN partition. We must link with the standard library
  else
  {
    debug_info("[BYPASS]\t try to dlsym_read %d,%p,%ld\n", fd, buf, nbyte);

    ret = dlsym_read(fd, buf, nbyte);

    debug_info("[BYPASS]\t dlsym_read %d,%p,%ld -> %ld\n", fd, buf, nbyte, ret);
  }

  debug_info("[BYPASS] << After read...\n");

  return ret;
}

ssize_t write ( int fd, const void *buf, size_t nbyte )
{
  ssize_t ret = -1;

  debug_info("[BYPASS] >> Begin write...\n");
  debug_info("[BYPASS]    * fd=%d\n",    fd);
  debug_info("[BYPASS]    * buf=%p\n",   buf);
  debug_info("[BYPASS]    * byte=%ld\n", nbyte);

  struct generic_fd virtual_fd = fdstable_get ( fd );

  // This if checks if variable fd passed as argument is a expand fd.
  if(virtual_fd.type == FD_XPN)
  {
    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    // It is an XPN partition, so we redirect the syscall to expand syscall
    if (virtual_fd.is_file == 0)
    {
      debug_error("[BYPASS:%s:%d] Error: is not a file\n", __FILE__, __LINE__);
      debug_info("[BYPASS] << After write...\n");

      errno = EISDIR;
      return -1;
    }

    debug_info("[BYPASS]\t try to xpn_write %d, %p, %ld\n", virtual_fd.real_fd, buf, nbyte);

    ret = xpn_write(virtual_fd.real_fd, (void *)buf, nbyte);

    debug_info("[BYPASS]\t xpn_write %d, %p, %ld -> %ld\n", virtual_fd.real_fd, buf, nbyte, ret);
  }
  // Not an XPN partition. We must link with the standard library
  else
  {
    debug_info("[BYPASS]\t try to dlsym_write %d,%p,%ld\n", fd, buf, nbyte);

    ret = dlsym_write(fd, (void *)buf, nbyte);

    debug_info("[BYPASS]\t dlsym_write %d,%p,%ld -> %ld\n", fd, buf, nbyte, ret);
  }

  debug_info("[BYPASS] << After write...\n");

  return ret;
}

ssize_t pread ( int fd, void *buf, size_t count, off_t offset )
{
  ssize_t ret = -1;

  debug_info("[BYPASS] >> Begin pread...\n");
  debug_info("[BYPASS]    * fd=%d\n",    fd);
  debug_info("[BYPASS]    * buf=%p\n",   buf);
  debug_info("[BYPASS]    * count=%ld\n", count);
  debug_info("[BYPASS]    * offset=%ld\n", offset);

  struct generic_fd virtual_fd = fdstable_get ( fd );

  // This if checks if variable fd passed as argument is a expand fd.
  if(virtual_fd.type == FD_XPN)
  {
    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    // It is an XPN partition, so we redirect the syscall to expand syscall
    if (virtual_fd.is_file == 0)
    {
      debug_error("[BYPASS:%s:%d] Error: is not a file\n", __FILE__, __LINE__);
      debug_info("[BYPASS] << After pread...\n");

      errno = EISDIR;
      return -1;
    }

    debug_info("[BYPASS]\t try to xpn_read %d, %p, %ld, %ld\n", virtual_fd.real_fd, buf, count, offset);

    ret = xpn_lseek(virtual_fd.real_fd, offset, SEEK_SET);
    if (ret != -1) {
      ret = xpn_read(virtual_fd.real_fd, buf, count);
    }
    if (ret != -1) {
      xpn_lseek(virtual_fd.real_fd, -ret, SEEK_CUR);
    }

    debug_info("[BYPASS]\t xpn_read %d, %p, %ld -> %ld\n", virtual_fd.real_fd, buf, count, ret);
  }
  // Not an XPN partition. We must link with the standard library
  else
  {
    debug_info("[BYPASS]\t try to dlsym_pread %d,%p,%ld\n", fd, buf, count);

    ret = dlsym_pread(fd,buf, count, offset);

    debug_info("[BYPASS]\t dlsym_pread %d,%p,%ld -> %ld\n", fd, buf, count, ret);
  }

  debug_info("[BYPASS] << After pread...\n");

  return ret;
}

ssize_t pwrite ( int fd, const void *buf, size_t count, off_t offset )
{
  ssize_t ret = -1;

  debug_info("[BYPASS] >> Begin pwrite...\n");
  debug_info("[BYPASS]    * fd=%d\n",    fd);
  debug_info("[BYPASS]    * buf=%p\n",   buf);
  debug_info("[BYPASS]    * count=%ld\n", count);
  debug_info("[BYPASS]    * offset=%ld\n", offset);

  struct generic_fd virtual_fd = fdstable_get ( fd );

  // This if checks if variable fd passed as argument is a expand fd.
  if(virtual_fd.type == FD_XPN)
  {
    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    // It is an XPN partition, so we redirect the syscall to expand syscall
    if (virtual_fd.is_file == 0)
    {
      debug_error("[BYPASS:%s:%d] Error: is not a file\n", __FILE__, __LINE__);
      debug_info("[BYPASS] << After pwrite...\n");

      errno = EISDIR;
      return -1;
    }

    debug_info("[BYPASS]\t try to xpn_write %d, %p, %ld, %ld\n", virtual_fd.real_fd, buf, count, offset);

    ret = xpn_lseek(virtual_fd.real_fd, offset, SEEK_SET);
    if (ret != -1) {
      ret = xpn_write(virtual_fd.real_fd, buf, count);
    }
    if (ret != -1) {
      xpn_lseek(virtual_fd.real_fd, -ret, SEEK_CUR);
    }

    debug_info("[BYPASS]\t xpn_write %d, %p, %ld -> %ld\n", virtual_fd.real_fd, buf, count, ret);
  }
  // Not an XPN partition. We must link with the standard library
  else
  {
    debug_info("[BYPASS]\t try to dlsym_pwrite %d, %p, %ld, %ld\n", fd, buf, count, offset);

    ret = dlsym_pwrite(fd, buf, count, offset);

    debug_info("[BYPASS]\t dlsym_pwrite %d, %p, %ld, %ld -> %ld\n", fd, buf, count, offset, ret);
  }

  debug_info("[BYPASS] << After pwrite...\n");

  return ret;
}

ssize_t pread64 ( int fd, void *buf, size_t count, off_t offset )
{
  ssize_t ret = -1;

  debug_info("[BYPASS] >> Begin pread64...\n");
  debug_info("[BYPASS]    * fd=%d\n",    fd);
  debug_info("[BYPASS]    * buf=%p\n",   buf);
  debug_info("[BYPASS]    * count=%ld\n", count);
  debug_info("[BYPASS]    * offset=%ld\n", offset);

  struct generic_fd virtual_fd = fdstable_get ( fd );

  // This if checks if variable fd passed as argument is a expand fd.
  if(virtual_fd.type == FD_XPN)
  {
    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    // It is an XPN partition, so we redirect the syscall to expand syscall
    if (virtual_fd.is_file == 0)
    {
      debug_error("[BYPASS:%s:%d] Error: is not a file\n", __FILE__, __LINE__);
      debug_info("[BYPASS] << After pread64...\n");

      errno = EISDIR;
      return -1;
    }

    debug_info("[BYPASS]\t try to xpn_read %d, %p, %ld, %ld\n", virtual_fd.real_fd, buf, count, offset);

    ret = xpn_lseek(virtual_fd.real_fd, offset, SEEK_SET);
    if (ret != -1) {
      ret = xpn_read(virtual_fd.real_fd, buf, count);
    }
    if (ret != -1) {
      xpn_lseek(virtual_fd.real_fd, -ret, SEEK_CUR);
    }

    debug_info("[BYPASS]\t xpn_read %d, %p, %ld -> %ld\n", virtual_fd.real_fd, buf, count, ret);
  }
  // Not an XPN partition. We must link with the standard library
  else
  {
    debug_info("[BYPASS]\t try to dlsym_pread64 %d,%p,%ld\n", fd, buf, count);

    ret = dlsym_pread64(fd,buf, count, offset);

    debug_info("[BYPASS]\t dlsym_pread64 %d,%p,%ld -> %ld\n", fd, buf, count, ret);
  }

  debug_info("[BYPASS] << After pread64...\n");

  return ret;
}

ssize_t pwrite64 ( int fd, const void *buf, size_t count, off_t offset )
{
  ssize_t ret = -1;

  debug_info("[BYPASS] >> Begin pwrite64...\n");
  debug_info("[BYPASS]    * fd=%d\n",    fd);
  debug_info("[BYPASS]    * buf=%p\n",   buf);
  debug_info("[BYPASS]    * count=%ld\n", count);
  debug_info("[BYPASS]    * offset=%ld\n", offset);

  struct generic_fd virtual_fd = fdstable_get ( fd );

  // This if checks if variable fd passed as argument is a expand fd.
  if(virtual_fd.type == FD_XPN)
  {
    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    // It is an XPN partition, so we redirect the syscall to expand syscall
    if (virtual_fd.is_file == 0)
    {
      debug_error("[BYPASS:%s:%d] Error: is not a file\n", __FILE__, __LINE__);
      debug_info("[BYPASS] << After pwrite64...\n");

      errno = EISDIR;
      return -1;
    }

    debug_info("[BYPASS]\t try to xpn_write %d, %p, %ld, %ld\n", virtual_fd.real_fd, buf, count, offset);

    ret = xpn_lseek(virtual_fd.real_fd, offset, SEEK_SET);
    if (ret != -1) {
      ret = xpn_write(virtual_fd.real_fd, buf, count);
    }
    if (ret != -1) {
      xpn_lseek(virtual_fd.real_fd, -ret, SEEK_CUR);
    }

    debug_info("[BYPASS]\t xpn_write %d, %p, %ld -> %ld\n", virtual_fd.real_fd, buf, count, ret);
  }
  // Not an XPN partition. We must link with the standard library
  else
  {
    debug_info("[BYPASS]\t try to dlsym_pwrite64 %d, %p, %ld, %ld\n", fd, buf, count, offset);

    ret = dlsym_pwrite64(fd, buf, count, offset);

    debug_info("[BYPASS]\t dlsym_pwrite64 %d, %p, %ld, %ld -> %ld\n", fd, buf, count, offset, ret);
  }

  debug_info("[BYPASS] << After pwrite64...\n");

  return ret;
}

off_t lseek ( int fd, off_t offset, int whence )
{
  off_t ret = (off_t) -1;

  debug_info("[BYPASS] >> Begin lseek...\n");

  struct generic_fd virtual_fd = fdstable_get ( fd );

  // This if checks if variable fd passed as argument is a expand fd.
  if(virtual_fd.type == FD_XPN)
  {
    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    // It is an XPN partition, so we redirect the syscall to expand syscall
    debug_info("[BYPASS]\t xpn_lseek %d,%ld,%d\n", fd, offset, whence);

    ret = xpn_lseek(virtual_fd.real_fd, offset, whence);

    debug_info("[BYPASS]\t xpn_lseek %d,%ld,%d -> %ld\n", fd, offset, whence, ret);
  }
  // Not an XPN partition. We must link with the standard library
  else
  {
    debug_info("[BYPASS]\t try to dlsym_lseek %d,%ld,%d\n", fd, offset, whence);

    ret = dlsym_lseek(fd, offset, whence);

    debug_info("[BYPASS]\t dlsym_lseek %d,%ld,%d -> %ld\n", fd, offset, whence, ret);
  }

  debug_info("[BYPASS] << After lseek...\n");

  return ret;
}

off64_t lseek64 ( int fd, off64_t offset, int whence )
{
  off64_t ret = (off64_t) -1;

  debug_info("[BYPASS] >> Begin lseek64...\n");

  struct generic_fd virtual_fd = fdstable_get ( fd );

  // This if checks if variable fd passed as argument is a expand fd.
  if(virtual_fd.type == FD_XPN)
  {
    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    // It is an XPN partition, so we redirect the syscall to expand syscall
    debug_info("[BYPASS]\t xpn_lseek64 %d,%ld,%d\n", fd, offset, whence);

    ret = xpn_lseek(virtual_fd.real_fd, offset, whence);

    debug_info("[BYPASS]\t xpn_lseek64 %d,%ld,%d -> %ld\n", fd, offset, whence, ret);
  }
  // Not an XPN partition. We must link with the standard library
  else
  {
    debug_info("[BYPASS]\t try to dlsym_lseek64 %d,%ld,%d\n", fd, offset, whence);

    ret = dlsym_lseek64(fd, offset, whence);

    debug_info("[BYPASS]\t dlsym_lseek64 %d,%ld,%d -> %ld\n", fd, offset, whence, ret);
  }

  debug_info("[BYPASS] << After lseek64...\n");

  return ret;
}

int __lxstat64 ( int ver, const char *path, struct stat64 *buf )
{
  int ret;
  struct stat st;

  debug_info("[BYPASS] >> Begin __lxstat64...\n");
  debug_info("[BYPASS]    1) Ver   => %d\n", ver);
  debug_info("[BYPASS]    2) Path  => %s\n", path);
  debug_info("[BYPASS]    3) Buf   => %p\n", buf);

  // This if checks if variable path passed as argument starts with the expand prefix.
  if (is_xpn_prefix(path))
  {
    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    // It is an XPN partition, so we redirect the syscall to expand syscall
    debug_info("[BYPASS]\t try to xpn_stat %s\n", skip_xpn_prefix(path));

    ret = xpn_stat(skip_xpn_prefix(path), &st);
    if (ret >= 0) {
      stat_to_stat64(buf, &st);
    }

    debug_info("[BYPASS]\t xpn_stat %s -> %d\n", skip_xpn_prefix(path), ret);
  }
  // Not an XPN partition. We must link with the standard library
  else
  {
    debug_info("[BYPASS]\t try to dlsym_lxstat64\n");

    ret = dlsym_lxstat64(ver, (const char *)path, buf);

    debug_info("[BYPASS]\t dlsym_lxstat64 -> %d\n", ret);
  }

  debug_info("[BYPASS] << After __lxstat64...\n");

  return ret;
}

int __xstat64 ( int ver, const char *path, struct stat64 *buf )
{
  int ret;
  struct stat st;

  debug_info("[BYPASS] >> Begin __xstat64...\n");
  debug_info("[BYPASS]    1) Path  => %s\n", path);

  // This if checks if variable path passed as argument starts with the expand prefix.
  if (is_xpn_prefix( path))
  {
    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    // It is an XPN partition, so we redirect the syscall to expand syscall
    debug_info("[BYPASS]\t xpn_stat\n");

    ret = xpn_stat(skip_xpn_prefix(path), &st);
    if (ret >= 0) {
      stat_to_stat64(buf, &st);
    }

    debug_info("[BYPASS]\t xpn_stat -> %d\n", ret);
  }
  // Not an XPN partition. We must link with the standard library
  else
  {
    debug_info("[BYPASS]\t try to dlsym_xstat64\n");

    ret = dlsym_xstat64(ver,(const char *)path, buf);

    debug_info("[BYPASS]\t dlsym_xstat64 -> %d\n", ret);
  }

  debug_info("[BYPASS] << After __xstat64...\n");

  return ret;
}

int __fxstat64 ( int ver, int fd, struct stat64 *buf )
{
  int ret;
  struct stat st;

  debug_info("[BYPASS] >> Begin __fxstat64...\n");
  debug_info("[BYPASS]    1) fd  => %d\n", fd);

  struct generic_fd virtual_fd = fdstable_get ( fd );

  // This if checks if variable fd passed as argument is a expand fd.
  if(virtual_fd.type == FD_XPN)
  {
    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    // It is an XPN partition, so we redirect the syscall to expand syscall
    debug_info("[BYPASS]\t xpn_fstat\n");

    ret = xpn_fstat(virtual_fd.real_fd, &st);
    if (ret >= 0) {
      stat_to_stat64(buf, &st);
    }

    debug_info("[BYPASS]\t xpn_fstat -> %d\n", ret);
  }
  // Not an XPN partition. We must link with the standard library
  else
  {
    debug_info("[BYPASS]\t try to dlsym_fxstat64\n");

    ret = dlsym_fxstat64(ver,fd, buf);

    debug_info("[BYPASS]\t dlsym_fxstat64 -> %d\n", ret);
  }

  debug_info("[BYPASS] << After __fxstat64...\n");

  return ret;
}

int __lxstat ( int ver, const char *path, struct stat *buf )
{
  int ret;

  debug_info("[BYPASS] >> Begin __lxstat...\n");
  debug_info("[BYPASS]    1) Path  => %s\n", path);

  // This if checks if variable path passed as argument starts with the expand prefix.
  if (is_xpn_prefix(path))
  {
    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    // It is an XPN partition, so we redirect the syscall to expand syscall
    debug_info("[BYPASS]\t xpn_stat %s\n",       skip_xpn_prefix(path));

    ret = xpn_stat(skip_xpn_prefix(path), buf);

    debug_info("[BYPASS]\t xpn_stat %s -> %d\n", skip_xpn_prefix(path), ret);
  }
  // Not an XPN partition. We must link with the standard library
  else
  {
    debug_info("[BYPASS]\t try to dlsym_lstat\n");

    ret = dlsym_lstat(ver,(const char *)path, buf);

    debug_info("[BYPASS]\t dlsym_lstat -> %d\n", ret);
  }

  debug_info("[BYPASS] << After __lxstat...\n");

  return ret;
}

int __xstat ( int ver, const char *path, struct stat *buf )
{
  int ret;

  debug_info("[BYPASS] >> Begin __xstat...\n");
  debug_info("[BYPASS]    1) Path  => %s\n", path);

  // This if checks if variable path passed as argument starts with the expand prefix.
  if (is_xpn_prefix(path))
  {
    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    // It is an XPN partition, so we redirect the syscall to expand syscall
    debug_info("[BYPASS]\t xpn_stat %s\n",       skip_xpn_prefix(path));

    ret = xpn_stat(skip_xpn_prefix(path), buf);

    debug_info("[BYPASS]\t xpn_stat %s -> %d\n", skip_xpn_prefix(path), ret);
  }
  // Not an XPN partition. We must link with the standard library
  else
  {
    debug_info("[BYPASS]\t try to dlsym_stat\n");

    ret = dlsym_stat(ver,(const char *)path, buf);

    debug_info("[BYPASS]\t dlsym_stat -> %d\n", ret);
  }

  debug_info("[BYPASS] << After __xstat...\n");

  return ret;
}

int __fxstat ( int ver, int fd, struct stat *buf )
{
  int ret = -1;

  debug_info("[BYPASS] >> Begin __fxstat...\n");
  debug_info("[BYPASS]    1) fd  => %d\n", fd);
  debug_info("[BYPASS]    2) buf => %p\n", buf);

  struct generic_fd virtual_fd = fdstable_get ( fd );

  if (virtual_fd.type == FD_XPN)
  {
    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    // It is an XPN partition, so we redirect the syscall to expand syscall
    debug_info("[BYPASS]\t xpn_fstat\n");

    ret = xpn_fstat(virtual_fd.real_fd, buf);

    debug_info("[BYPASS]\t xpn_fstat -> %d\n", ret);
  }
  // Not an XPN partition. We must link with the standard library
  else
  {
    debug_info("[BYPASS]\t try to dlsym_fstat\n");

    ret = dlsym_fstat(ver, fd, buf);

    debug_info("[BYPASS]\t dlsym_fstat -> %d\n", ret);
  }

  debug_info("[BYPASS] << After __fxstat...\n");

  return ret;
}

/*
int __fxstatat64 ( __attribute__((__unused__)) int ver, int dirfd, const char *path, struct stat64 *buf, int flags )
{
  int    ret = -1;
  struct stat st;

  debug_info("[BYPASS] >> Begin __fxstatat64...\n");
  debug_info("[BYPASS]    * ver:   %d\n", ver);
  debug_info("[BYPASS]    * dirfd: %d\n", dirfd);
  debug_info("[BYPASS]    * path:  %s\n", path);
  debug_info("[BYPASS]    * buf:   %p\n", buf);
  debug_info("[BYPASS]    * flags: %o\n", flags);

  // This if checks if variable path passed as argument starts with the expand prefix.
  if (is_xpn_prefix( path))
  {
    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    // It is an XPN partition, so we redirect the syscall to expand syscall
    // TODO: if path is relative -> use dirfd as CWD
    // TODO: use flags (see man fstatat

    debug_info("[BYPASS]\t Begin xpn_stat %s\n", skip_xpn_prefix(path));

    ret = xpn_stat(skip_xpn_prefix(path), &st);
    if (ret >= 0) {
        stat_to_stat64(buf, &st);
    }

    debug_info("[BYPASS]\t xpn_stat %s -> %d\n", skip_xpn_prefix(path), ret);
  }
  // Not an XPN partition. We must link with the standard library
  else
  {
    debug_info("[BYPASS]\t dlsym_fstatat64\n");

    ret = dlsym_fstatat64(dirfd, path, buf, flags);

    debug_info("[BYPASS]\t dlsym_fstatat64 -> %d\n", ret);
  }

  debug_info("[BYPASS] << After __fxstatat64...\n");

  return ret;
}

int __fxstatat ( __attribute__((__unused__)) int ver, int dirfd, const char *path, struct stat *buf, int flags )
{
  int  ret = -1;

  debug_info("[BYPASS] >> Begin __fxstatat...\n");
  debug_info("[BYPASS]    * ver:   %d\n", ver);
  debug_info("[BYPASS]    * path:  %s\n", path);
  debug_info("[BYPASS]    * dirfd: %d\n", dirfd);
  debug_info("[BYPASS]    * flags: %o\n", flags);

  // This if checks if variable path passed as argument starts with the expand prefix.
  if (is_xpn_prefix(path))
  {
    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();
    
    // It is an XPN partition, so we redirect the syscall to expand syscall
    // TODO: if path is relative -> use dirfd as CWD
    // TODO: use flags (see man fstatat

    debug_info("[BYPASS]\t Begin xpn_stat %s\n", skip_xpn_prefix(path));

    ret = xpn_stat(skip_xpn_prefix(path), buf);

    debug_info("[BYPASS]\t xpn_stat %s -> %d\n", skip_xpn_prefix(path), ret);
  }
  // Not an XPN partition. We must link with the standard library
  else
  {
    debug_info("[BYPASS]\t dlsym_fstatat %d,%p,%p,%d\n", dirfd, path, buf, flags);

    ret = dlsym_fstatat(dirfd, path, buf, flags);

    debug_info("[BYPASS]\t dlsym_fstatat %d,%p,%p,%d -> %d\n", dirfd, path, buf, flags, ret);
  }

  debug_info("[BYPASS] << After __fxstatat...\n");

  return ret;
}
*/

int close ( int fd )
{
  debug_info("[BYPASS] >> Begin close....\n");
  debug_info("[BYPASS]    * FD = %d\n", fd);

  int ret = -1;

  struct generic_fd virtual_fd = fdstable_get ( fd );

  // This if checks if variable fd passed as argument is a expand fd.
  if(virtual_fd.type == FD_XPN)
  {
    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    // It is an XPN partition, so we redirect the syscall to expand syscall
    debug_info("[BYPASS]\t xpn_close %d\n", virtual_fd.real_fd);

    ret = xpn_close(virtual_fd.real_fd);
    fdstable_remove(fd);

    debug_info("[BYPASS]\t xpn_close %d -> %d\n", virtual_fd.real_fd, ret);
  }
  // Not an XPN partition. We must link with the standard library
  else
  {
    debug_info("[BYPASS]\t try to dlsym_close\n");

    ret = dlsym_close(fd);

    debug_info("[BYPASS]\t dlsym_close -> %d\n", ret);
  }

  debug_info("[BYPASS] << After close....\n");

  return ret;
}

int rename ( const char *old_path, const char *new_path )
{
  int ret = -1;

  debug_info("[BYPASS] >> Begin rename....\n");
  debug_info("[BYPASS]    1) old Path %s\n", old_path);
  debug_info("[BYPASS]    2) new Path %s\n", new_path);

  // This if checks if variable path passed as argument starts with the expand prefix.
  if(is_xpn_prefix( old_path) && is_xpn_prefix( new_path))
  {
    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    // It is an XPN partition, so we redirect the syscall to expand syscall
    debug_info("[BYPASS]\t xpn_rename %s %s\n", skip_xpn_prefix(old_path), skip_xpn_prefix(new_path));

    ret = xpn_rename(skip_xpn_prefix(old_path), skip_xpn_prefix(new_path));

    debug_info("[BYPASS]\t xpn_rename %s %s\n -> %d", skip_xpn_prefix(old_path), skip_xpn_prefix(new_path), ret);
  }
  // Not an XPN partition. We must link with the standard library
  else 
  {
    debug_info("[BYPASS]\t try to dlsym_rename %s, %s\n", old_path, new_path);

    ret = dlsym_rename(old_path, new_path);

    debug_info("[BYPASS]\t dlsym_rename %s, %s -> %d\n", old_path, new_path, ret);
  }

  debug_info("[BYPASS] << After rename....\n");

  return ret;
}

int unlink ( const char *path )
{
  int ret = -1;

  debug_info("[BYPASS] >> Begin unlink...\n");
  debug_info("[BYPASS]    1) Path %s\n", path);

  // This if checks if variable path passed as argument starts with the expand prefix.
  if (is_xpn_prefix(path))
  {
    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    // It is an XPN partition, so we redirect the syscall to expand syscall
    debug_info("[BYPASS]\t xpn_unlink\n");

    ret = (xpn_unlink(skip_xpn_prefix(path)));

    debug_info("[BYPASS]\t xpn_unlink -> %d\n", ret);
  }
  // Not an XPN partition. We must link with the standard library
  else
  {
    debug_info("[BYPASS]\t dlsym_unlink\n");

    ret = dlsym_unlink((char *)path);

    debug_info("[BYPASS]\t dlsym_unlink -> %d\n", ret);
  }

  debug_info("[BYPASS] << After unlink....\n");

  return ret;
}

int remove ( const char *path )
{
  int ret = -1;

  debug_info("[BYPASS] >> Begin remove...\n");
  debug_info("[BYPASS]    1) Path %s\n", path);

  // This if checks if variable path passed as argument starts with the expand prefix.
  if (is_xpn_prefix(path))
  {
    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    // It is an XPN partition, so we redirect the syscall to expand syscall
    struct stat buf;
    ret = xpn_stat(skip_xpn_prefix(path), &buf);

    if ((buf.st_mode & S_IFMT) == S_IFREG) 
    {
      debug_info("[BYPASS]\t xpn_unlink\n");

      ret = (xpn_unlink(skip_xpn_prefix(path)));

      debug_info("[BYPASS]\t xpn_unlink -> %d\n", ret);
    }
    else if ((buf.st_mode & S_IFMT) == S_IFDIR)
    {
      debug_info("[BYPASS]\t xpn_rmdir\n");

      ret = xpn_rmdir( (skip_xpn_prefix(path)) );

      debug_info("[BYPASS]\t xpn_rmdir -> %d\n", ret);
    }

  }
  // Not an XPN partition. We must link with the standard library
  else
  {
    debug_info("[BYPASS]\t dlsym_remove\n");

    ret = dlsym_remove((char *)path);

    debug_info("[BYPASS]\t dlsym_remove -> %d\n", ret);
  }

  debug_info("[BYPASS] << After remove....\n");

  return ret;
}

// File API (stdio)
FILE *fopen ( const char *path, const char *mode )
{
  FILE * ret;

  debug_info("[BYPASS] >> Begin fopen....\n");
  debug_info("[BYPASS]    1) Path  => %s\n", path);
  debug_info("[BYPASS]    2) Mode  => %s\n", mode);

  // This if checks if variable path passed as argument starts with the expand prefix.
  if (is_xpn_prefix(path))
  {
    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    // It is an XPN partition, so we redirect the syscall to expand syscall
    debug_info("[BYPASS]\t xpn_open (%s)\n",path + strlen(xpn_adaptor_partition_prefix));

    int fd;
    switch (mode[0])
    {
      case 'r':
          fd=xpn_open(skip_xpn_prefix(path), O_RDONLY | O_CREAT, 0640);
          break;
      case 'w':
          fd=xpn_open(skip_xpn_prefix(path), O_WRONLY | O_CREAT | O_TRUNC, 0640);
          break;
      default:
          fd=xpn_open(skip_xpn_prefix(path), O_RDWR | O_CREAT | O_TRUNC, 0640);
          break;
    }

    int xpn_fd = add_xpn_file_to_fdstable(fd);

    debug_info("[BYPASS]\t xpn_open (%s) -> %d\n", skip_xpn_prefix(path), fd);

    debug_info("[BYPASS]\t fdopen %d\n", xpn_fd);

    ret = fdopen(xpn_fd, mode);

    debug_info("[BYPASS]\t fdopen %d -> %p\n", xpn_fd, ret);
  }
  // Not an XPN partition. We must link with the standard library.
  else 
  {
    debug_info("[BYPASS]\t dlsym_fopen (%s,%s)\n", path, mode);

    ret = dlsym_fopen((const char *)path, mode);

    debug_info("[BYPASS]\t dlsym_fopen (%s,%s) -> %p\n", path, mode, ret);
  }

  debug_info("[BYPASS] << After fopen.... %s\n", path);

  return ret;
}

FILE * fdopen (int fd, const char *mode )
{
  debug_info("[BYPASS] >> Begin fdopen....\n");
  debug_info("[BYPASS]    1) fd = %d\n", fd);
  debug_info("[BYPASS]    2) mode = %s\n", mode);

  FILE *fp;

  struct generic_fd virtual_fd = fdstable_get ( fd );

  // This if checks if variable fd passed as argument is a expand fd.
  if(virtual_fd.type == FD_XPN)
  {
    debug_info("[BYPASS]\t try to dlsym_fdopen 1\n");

    fp = dlsym_fopen("/dev/null", mode);
    fp->_fileno = fd;

    debug_info("[BYPASS]\t dlsym_fdopen -> %p\n", fp);
  }
  // Not an XPN partition. We must link with the standard library
  else
  {
    debug_info("[BYPASS]\t try to dlsym_fdopen 2\n");

    fp = dlsym_fdopen(fd, mode);

    debug_info("[BYPASS]\t dlsym_fdopen -> %p\n", fp);
  }

  debug_info("[BYPASS] << After fdopen....\n");

  return fp;
}

int fclose ( FILE *stream )
{
  debug_info("[BYPASS] >> Begin fclose....\n");
  debug_info("[BYPASS]    1) stream = %p\n", stream);

  int ret = -1;

  int fd = fileno(stream);

  struct generic_fd virtual_fd = fdstable_get ( fd );

  // This if checks if variable fd passed as argument is a expand fd.
  if(virtual_fd.type == FD_XPN)
  {
    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    // It is an XPN partition, so we redirect the syscall to expand syscall
    debug_info("[BYPASS]\t xpn_close %d\n", virtual_fd.real_fd);

    ret = xpn_close(virtual_fd.real_fd);
    fdstable_remove(fd);

    debug_info("[BYPASS]\t xpn_close %d -> %d\n", virtual_fd.real_fd, ret);
  }
  // Not an XPN partition. We must link with the standard library
  else
  {
    debug_info("[BYPASS]\t try to fdlsym_close\n");

    ret = dlsym_fclose(stream);

    debug_info("[BYPASS]\t dlsym_fclose -> %d\n", ret);
  }

  debug_info("[BYPASS] << After fclose....\n");

  return ret;
}

size_t fread ( void *ptr, size_t size, size_t nmemb, FILE *stream )
{         
  size_t ret = (size_t) -1;

  debug_info("[BYPASS] >> Begin fread...\n");
  debug_info("[BYPASS]    1) ptr=%p\n",    ptr);
  debug_info("[BYPASS]    2) size=%ld\n",  size);
  debug_info("[BYPASS]    3) nmemb=%ld\n", nmemb);
  debug_info("[BYPASS]    4) stream=%p\n", stream);

  int fd = fileno(stream);
  struct generic_fd virtual_fd = fdstable_get ( fd );

  // This if checks if variable fd passed as argument is a expand fd.
  if(virtual_fd.type == FD_XPN)
  {
    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    // It is an XPN partition, so we redirect the syscall to expand syscall
    if (virtual_fd.is_file == 0) {
      debug_error("[BYPASS:%s:%d] Error: is not a file\n", __FILE__, __LINE__);
      debug_info("[BYPASS] << After fread...\n");

      errno = EISDIR;
      return -1;
    }

    int buf_size = size * nmemb;

    debug_info("[BYPASS]\t try to xpn_read %d, %p, %d\n", virtual_fd.real_fd, ptr, buf_size);

    ret = xpn_read(virtual_fd.real_fd, ptr, buf_size);
    ret = ret / size; // Number of items read

    debug_info("[BYPASS]\t xpn_read %d, %p, %d -> %ld\n", virtual_fd.real_fd, ptr, buf_size, ret);
  }
  // Not an XPN partition. We must link with the standard library
  else
  {
    debug_info("[BYPASS]\t try to dlsym_fread %p,%ld,%ld,%p\n", ptr, size, nmemb, stream);

    ret = dlsym_fread(ptr, size, nmemb, stream);

    debug_info("[BYPASS]\t dlsym_fread %p,%ld,%ld,%p -> %ld\n", ptr, size, nmemb, stream, ret);
  }

  debug_info("[BYPASS] << After fread...\n");

  return ret;
}

size_t fwrite ( const void *ptr, size_t size, size_t nmemb, FILE *stream )
{         
  size_t ret = (size_t) -1;

  debug_info("[BYPASS] >> Begin fwrite...\n");
  debug_info("[BYPASS]    1) ptr=%p\n",    ptr);
  debug_info("[BYPASS]    2) size=%ld\n",  size);
  debug_info("[BYPASS]    3) nmemb=%ld\n", nmemb);
  debug_info("[BYPASS]    4) stream=%p\n", stream);

  int fd = fileno(stream);
  struct generic_fd virtual_fd = fdstable_get ( fd );

  // This if checks if variable fd passed as argument is a expand fd.
  if(virtual_fd.type == FD_XPN)
  {
    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    // It is an XPN partition, so we redirect the syscall to expand syscall
    if (virtual_fd.is_file == 0) {
      debug_error("[BYPASS:%s:%d] Error: is not a file\n", __FILE__, __LINE__);
      debug_info("[BYPASS] << After fwrite...\n");

      errno = EISDIR;
      return -1;
    }

    int buf_size = size * nmemb;

    debug_info("[BYPASS]\t try to xpn_write %d, %p, %d\n", virtual_fd.real_fd, ptr, buf_size);
    ret = xpn_write(virtual_fd.real_fd, ptr, buf_size);
    ret = ret / size; // Number of items Written

    debug_info("[BYPASS]\t xpn_write %d, %p, %d -> %ld\n", virtual_fd.real_fd, ptr, buf_size, ret);
  }
  // Not an XPN partition. We must link with the standard library
  else
  {
    debug_info("[BYPASS]\t try to dlsym_fwrite %p,%ld,%ld,%p\n", ptr, size, nmemb, stream);

    ret = dlsym_fwrite(ptr, size, nmemb, stream);

    debug_info("[BYPASS]\t dlsym_fwrite %p,%ld,%ld,%p -> %ld\n", ptr, size, nmemb, stream, ret);
  }

  debug_info("[BYPASS] << After fwrite...\n");

  return ret;
}

int fseek ( FILE *stream, long int offset, int whence )
{
  int ret = -1;

  debug_info("[BYPASS] >> Begin fseek...\n");
  debug_info("[BYPASS]    1) stream=%p\n",  stream);
  debug_info("[BYPASS]    2) offset=%ld\n", offset);
  debug_info("[BYPASS]    3) whence=%d\n",  whence);

  int fd = fileno(stream);
  struct generic_fd virtual_fd = fdstable_get ( fd );

  // This if checks if variable fd passed as argument is a expand fd.
  if(virtual_fd.type == FD_XPN)
  {
    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    // It is an XPN partition, so we redirect the syscall to expand syscall
    debug_info("[BYPASS]\t xpn_lseek %d,%ld,%d\n", virtual_fd.real_fd, offset, whence);

    ret = xpn_lseek(virtual_fd.real_fd, offset, whence);

    debug_info("[BYPASS]\t xpn_lseek %d,%ld,%d -> %d\n", virtual_fd.real_fd, offset, whence, ret);
  }
  // Not an XPN partition. We must link with the standard library
  else
  {
    debug_info("[BYPASS]\t try to dlsym_fseek %p,%ld,%d\n", stream, offset, whence);

    ret = dlsym_fseek(stream, offset, whence);

    debug_info("[BYPASS]\t dlsym_fseek %p,%ld,%d -> %d\n", stream, offset, whence, ret);
  }

  debug_info("[BYPASS] << After fseek...\n");

  return ret;
}

long ftell ( FILE *stream )
{
  debug_info("[BYPASS] >> Begin ftell....\n");
  debug_info("[BYPASS]    1) stream = %p\n", stream);

  long ret = -1;

  int fd = fileno(stream);
  struct generic_fd virtual_fd = fdstable_get ( fd );

  // This if checks if variable fd passed as argument is a expand fd.
  if(virtual_fd.type == FD_XPN)
  {
    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    // It is an XPN partition, so we redirect the syscall to expand syscall
    debug_info("[BYPASS]\t xpn_lseek %d\n", virtual_fd.real_fd);

    ret = xpn_lseek(virtual_fd.real_fd, 0, SEEK_CUR);

    debug_info("[BYPASS]\t xpn_lseek %d -> %ld\n", virtual_fd.real_fd, ret);
  }
  // Not an XPN partition. We must link with the standard library
  else
  {
    debug_info("[BYPASS]\t try to dlsym_ftell\n");

    ret = dlsym_ftell(stream);

    debug_info("[BYPASS]\t dlsym_ftell -> %ld\n", ret);
  }

  debug_info("[BYPASS] << After ftell....\n");

  return ret;
}

int  feof(FILE *stream)
{
  debug_info("[BYPASS] >> Begin feof....\n");
  debug_info("[BYPASS]    1) stream = %p\n", stream);

  int ret = -1;

  int fd = fileno(stream);
  struct generic_fd virtual_fd = fdstable_get ( fd );

  // This if checks if variable fd passed as argument is a expand fd.
  if(virtual_fd.type == FD_XPN)
  {
    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    // It is an XPN partition, so we redirect the syscall to expand syscall
    debug_info("[BYPASS]\t xpn_lseek %d\n", virtual_fd.real_fd);

    int ret1, ret2;
    ret1 = xpn_lseek(virtual_fd.real_fd, 0, SEEK_CUR);
    if (ret != -1) {
      debug_info("[BYPASS] << After feof....\n");

      return ret;
    }
    ret2 = xpn_lseek(virtual_fd.real_fd, 0, SEEK_END);
    if (ret != -1) {
      debug_info("[BYPASS] << After feof....\n");

      return ret;
    }

    if (ret1 != ret2) {
      ret = 0;
    }
    else {
      ret = 1;
    }

    debug_info("[BYPASS]\t xpn_lseek %d -> %d\n", virtual_fd.real_fd, ret);
  }
  // Not an XPN partition. We must link with the standard library
  else
  {
    debug_info("[BYPASS]\t try to dlsym_feof\n");

    ret = dlsym_feof(stream);

    debug_info("[BYPASS]\t dlsym_feof -> %d\n", ret);
  }

  debug_info("[BYPASS] << After feof....\n");

  return ret;
}

// Directory API

int mkdir ( const char *path, mode_t mode )
{
  int ret = -1;

  debug_info("[BYPASS] >> Begin mkdir...\n");
  debug_info("[BYPASS]    1) Path %s\n", path);

  // This if checks if variable path passed as argument starts with the expand prefix.
  if (is_xpn_prefix(path))
  {
    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    // It is an XPN partition, so we redirect the syscall to expand syscall
    debug_info("[BYPASS]\t xpn_mkdir %s\n",       (skip_xpn_prefix(path)));

    ret = xpn_mkdir( (skip_xpn_prefix(path)) ,mode );

    debug_info("[BYPASS]\t xpn_mkdir %s -> %d\n", (skip_xpn_prefix(path)), ret);
  }
  // Not an XPN partition. We must link with the standard library
  else
  {
    debug_info("[BYPASS]\t try to dlsym_mkdir\n");

    ret = dlsym_mkdir((char *)path,mode);

    debug_info("[BYPASS]\t dlsym_mkdir -> %d\n", ret);
  }

  debug_info("[BYPASS] << After mkdir...\n");

  return ret;
}

DIR *opendir ( const char *dirname )
{
  DIR * ret;

  debug_info("[BYPASS] >> Begin opendir...\n");
  debug_info("[BYPASS]    1) dirname %s\n", dirname);

  // This if checks if variable path passed as argument starts with the expand prefix.
  if(is_xpn_prefix(dirname))
  {
    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    // It is an XPN partition, so we redirect the syscall to expand syscall
    debug_info("[BYPASS]\t xpn_opendir\n");

    ret = xpn_opendir((const char *)(dirname+strlen(xpn_adaptor_partition_prefix)));
    if (ret != NULL) {
      fdsdirtable_put ( ret );
    }

    debug_info("[BYPASS]\t xpn_opendir -> %p\n", ret);
  }
  // Not an XPN partition. We must link with the standard library
  else
  {
    debug_info("[BYPASS]\t try dlsym_opendir\n");
    ret = dlsym_opendir((char *)dirname);
    debug_info("[BYPASS]\t dlsym_opendir -> %p\n", ret);
  }

  debug_info("[BYPASS] << After opendir...\n");
  return ret;
}

struct dirent *readdir ( DIR *dirp )
{
  struct dirent *ret;

  debug_info("[BYPASS] >> Begin readdir...\n");
  debug_info("[BYPASS]    1) dirp %p\n", dirp);

  if (fdsdirtable_get( dirp ) != -1)
  {
    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    // It is an XPN partition, so we redirect the syscall to expand syscall
    debug_info("[BYPASS]\t xpn_readdir\n");

    DIR aux_dirp = fdsdirtable_getfd( dirp );
    ret = xpn_readdir(&aux_dirp);

    debug_info("[BYPASS]\t xpn_readdir -> %p\n", ret);
  }
  // Not an XPN partition. We must link with the standard library
  else
  {
    debug_info("[BYPASS]\t try to dlsym_readdir\n");

    ret = dlsym_readdir(dirp);

    debug_info("[BYPASS]\t dlsym_readdir -> %p\n", ret);
  }

  debug_info("[BYPASS] << After readdir...\n");

  return ret;
}

struct dirent64 *readdir64 ( DIR *dirp )
{
  struct dirent *aux;
  struct dirent64 *ret = NULL;

  debug_info("[BYPASS] >> Begin readdir64...\n");
  debug_info("[BYPASS]    1) dirp %p\n", dirp);

  if (fdsdirtable_get( dirp ) != -1)
  {
    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    // It is an XPN partition, so we redirect the syscall to expand syscall
    debug_info("[BYPASS]\t xpn_readdir\n");

    DIR aux_dirp = fdsdirtable_getfd( dirp );
    aux = xpn_readdir(&aux_dirp);
    if (aux != NULL)
    {
      ret = (struct dirent64 *)malloc(sizeof(struct dirent64)); // TODO: change to static memory per dir... or where memory is free?
      ret->d_ino    = (__ino64_t)  aux->d_ino;
      ret->d_off    = (__off64_t)  aux->d_off;
      ret->d_reclen =              aux->d_reclen;
      ret->d_type   =              aux->d_type;
      strcpy(ret->d_name, aux->d_name);
    }

    debug_info("[BYPASS]\t xpn_readdir -> %p\n", ret);
  }
  // Not an XPN partition. We must link with the standard library
  else
  {
    debug_info("[BYPASS]\t try to dlsym_readdir64\n");

    ret = dlsym_readdir64(dirp);

    debug_info("[BYPASS]\t dlsym_readdir64 -> %p\n", ret);
  } 

  debug_info("[BYPASS] << After readdir64...\n");

  return ret;
}

int closedir ( DIR *dirp )
{
  int ret = -1;

  debug_info("[BYPASS] >> Begin closedir...\n");
  debug_info("[BYPASS]    1) dirp %p\n", dirp);

  if( fdsdirtable_get( dirp ) != -1 )
  {
    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    // It is an XPN partition, so we redirect the syscall to expand syscall
    debug_info("[BYPASS]\t xpn_closedir\n");

    fdsdirtable_remove( dirp );
    ret = xpn_closedir( dirp );

    debug_info("[BYPASS]\t xpn_closedir -> %d\n", ret);
  }
  // Not an XPN partition. We must link with the standard library
  else
  {
    debug_info("[BYPASS]\t try to dlsym_closedir\n");

    ret = dlsym_closedir(dirp);

    debug_info("[BYPASS]\t dlsym_closedir -> %d\n", ret);
  }

  debug_info("[BYPASS] << After closedir...\n");

  return ret;
}

int rmdir ( const char *path )
{
  int ret = -1;

  debug_info("[BYPASS] >> Begin rmdir...\n");
  debug_info("[BYPASS]    1) path %s\n", path);

  // This if checks if variable path passed as argument starts with the expand prefix.
  if (is_xpn_prefix(path))
  {
    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    // It is an XPN partition, so we redirect the syscall to expand syscall
    debug_info("[BYPASS]\t xpn_rmdir\n");

    ret = xpn_rmdir( (skip_xpn_prefix(path)) );

    debug_info("[BYPASS]\t xpn_rmdir -> %d\n", ret);
  }
  // Not an XPN partition. We must link with the standard library
  else
  {
    debug_info("[BYPASS]\t try to dlsym_rmdir\n");

    ret = dlsym_rmdir((char *)path);

    debug_info("[BYPASS]\t dlsym_rmdir -> %d\n", ret);
  }

  debug_info("[BYPASS] << After rmdir...\n");

  return ret;
}

// Proccess API

pid_t fork ( void )
{
  int ret = -1;

  debug_info("[BYPASS] >> Begin fork()\n");
  debug_info("[BYPASS]\t try to dlsym_fork\n");

  ret = dlsym_fork();
  if(0 == ret) {
    // We want the children to be initialized
    xpn_adaptor_initCalled = 0;
  }

  debug_info("[BYPASS]\t dlsym_fork -> %d\n", ret);
  debug_info("[BYPASS] << After fork()\n");

  return ret;
}

int pipe ( int pipefd[2] )
{
  debug_info("[BYPASS] >> Begin pipe()\n");
  debug_info("[BYPASS]    1) fd1 %d\n", pipefd[0]);
  debug_info("[BYPASS]    2) fd2 %d\n", pipefd[1]);
  debug_info("[BYPASS]\t try to dlsym_pipe\n");

  int ret = dlsym_pipe(pipefd);

  debug_info("[BYPASS]\t dlsym_pipe -> %d\n", ret);
  debug_info("[BYPASS] << After pipe()\n");

  return ret;
}

int dup ( int fd )
{
  debug_info("[BYPASS] >> Begin dup...\n");
  debug_info("[BYPASS]    1) fd %d\n", fd);

  int ret = -1;

  struct generic_fd virtual_fd = fdstable_get ( fd );

  // This if checks if variable fd passed as argument is a expand fd.
  if(virtual_fd.type == FD_XPN)
  {
    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    // It is an XPN partition, so we redirect the syscall to expand syscall
    debug_info("[BYPASS] xpn_dup\n");

    ret = xpn_dup(virtual_fd.real_fd);

    debug_info("[BYPASS]\t xpn_dup -> %d\n", ret);
  }
  // Not an XPN partition. We must link with the standard library
  else
  {
    debug_info("[BYPASS] dlsym_dup\n");

    ret = dlsym_dup(fd);

    debug_info("[BYPASS]\t dlsym_dup -> %d\n", ret);
  }

  debug_info("[BYPASS] << After dup()\n");

  return ret;
}

int dup2 ( int fd, int fd2 )
{
  debug_info("[BYPASS] >> Begin dup2...\n");
  debug_info("[BYPASS]    1) fd %d\n", fd);
  debug_info("[BYPASS]    2) fd2 %d\n", fd2);

  int ret = -1;

  struct generic_fd virtual_fd  = fdstable_get ( fd );
  struct generic_fd virtual_fd2 = fdstable_get ( fd2 );

  // This if checks if variable fd passed as argument is a expand fd.
  if(virtual_fd.type == FD_XPN)
  {
    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    // It is an XPN partition, so we redirect the syscall to expand syscall
    debug_info("[BYPASS] xpn_dup2\n");

    ret = xpn_dup2(virtual_fd.real_fd, virtual_fd2.real_fd);

    debug_info("[BYPASS]\t xpn_dup2 -> %d\n", ret);
  }
  // Not an XPN partition. We must link with the standard library
  else
  {
    debug_info("[BYPASS] dlsym_dup2\n");

    ret = dlsym_dup2(fd, fd2);

    debug_info("[BYPASS]\t dlsym_dup2 -> %d\n", ret);
  }

  debug_info("[BYPASS] << After dup2()\n");

  return ret;
}

void exit ( int status )
{
  debug_info("[BYPASS] >> Begin exit...\n");
  debug_info("[BYPASS]    1) status %d\n", status);

  if (xpn_adaptor_initCalled == 1)
  {
    debug_info("[BYPASS] xpn_destroy\n");

    xpn_destroy();
  }

  debug_info("[BYPASS] dlsym_exit\n");

  dlsym_exit(status);
  __builtin_unreachable();

  debug_info("[BYPASS] << After exit()\n");
}

// Manager API

int chdir ( const char *path )
{
  debug_info("[BYPASS] >> Begin chdir...\n");
  debug_info("[BYPASS]    1) path %s\n", path);

  int ret = -1;

  // This if checks if variable path passed as argument starts with the expand prefix.
  if (is_xpn_prefix(path))
  {
    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    // It is an XPN partition, so we redirect the syscall to expand syscall
    debug_info("[BYPASS] xpn_chdir\n");

    ret = xpn_chdir((char *)skip_xpn_prefix(path));

    debug_info("[BYPASS]\t xpn_chdir -> %d\n", ret);
  }
  // Not an XPN partition. We must link with the standard library
  else 
  {
    debug_info("[BYPASS] dlsym_chdir\n");

    ret = dlsym_chdir((char *)path);

    debug_info("[BYPASS]\t dlsym_chdir -> %d\n", ret);
  }

  debug_info("[BYPASS] << After chdir()\n");

  return ret;
}

int chmod ( const char *path, mode_t mode )
{
  debug_info("[BYPASS] >> Begin chmod...\n");
  debug_info("[BYPASS]    1) path %s\n", path);

  int ret = -1;

  // This if checks if variable path passed as argument starts with the expand prefix.
  if (is_xpn_prefix(path))
  {
    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    // It is an XPN partition, so we redirect the syscall to expand syscall
    debug_info("[BYPASS] xpn_chmod\n");

    ret = xpn_chmod(skip_xpn_prefix(path), mode);

    debug_info("[BYPASS]\t xpn_chmod -> %d\n", ret);
  }
  // Not an XPN partition. We must link with the standard library
  else
  {
    debug_info("[BYPASS] dlsym_chmod\n");

    ret = dlsym_chmod((char *)path, mode);

    debug_info("[BYPASS]\t dlsym_chmod -> %d\n", ret);
  }

  debug_info("[BYPASS] << After chmod()\n");

  return ret;
}

int fchmod ( int fd, mode_t mode )
{
  debug_info("[BYPASS] >> Begin fchmod...\n");
  debug_info("[BYPASS]    1) fd %d\n", fd);

  int ret = -1;

  struct generic_fd virtual_fd = fdstable_get ( fd );

  // This if checks if variable fd passed as argument is a expand fd.
  if(virtual_fd.type == FD_XPN)
  {
    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    // It is an XPN partition, so we redirect the syscall to expand syscall
    debug_info("[BYPASS] xpn_fchmod\n");

    ret = xpn_fchmod(fd,mode);

    debug_info("[BYPASS]\t dlsym_fchmod -> %d\n", ret);
  }
  // Not an XPN partition. We must link with the standard library
  else
  {
    debug_info("[BYPASS] dlsym_fchmod\n");

    ret = dlsym_fchmod(fd,mode);

    debug_info("[BYPASS]\t dlsym_fchmod -> %d\n", ret);
  }

  debug_info("[BYPASS] << After fchmod()\n");

  return ret;
}

int chown ( const char *path, uid_t owner, gid_t group )
{
  debug_info("[BYPASS] >> Begin chown...\n");
  debug_info("[BYPASS]    1) path %s\n", path);

  int ret = -1;

  // This if checks if variable path passed as argument starts with the expand prefix.
  if (is_xpn_prefix(path))
  {
    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    // It is an XPN partition, so we redirect the syscall to expand syscall
    debug_info("[BYPASS] xpn_chown\n");
    ret = xpn_chown(skip_xpn_prefix(path), owner, group);
    debug_info("[BYPASS]\t xpn_chown -> %d\n", ret);
  }
  // Not an XPN partition. We must link with the standard library
  else
  {
    debug_info("[BYPASS] dlsym_chown\n");
    ret = dlsym_chown((char *)path, owner, group);
    debug_info("[BYPASS]\t dlsym_chown -> %d\n", ret);
  }

  debug_info("[BYPASS] << After chown()\n");

  return ret;
}

int fcntl ( int fd, int cmd, long arg ) //TODO
{
  debug_info("[BYPASS] >> Begin fcntl...\n");
  debug_info("[BYPASS]    1) fd %d\n", fd);
  debug_info("[BYPASS]    2) cmd %d\n", cmd);
  debug_info("[BYPASS]    3) arg %ld\n", arg);

  int ret = -1;

  struct generic_fd virtual_fd = fdstable_get ( fd );

  // This if checks if variable fd passed as argument is a expand fd.
  if(virtual_fd.type == FD_XPN)
  {
    debug_info("[BYPASS] xpn_fcntl\n");

    //TODO
    ret = 0;

    debug_info("[BYPASS]\t xpn_fcntl -> %d\n", ret);
  }
  else
  {
    debug_info("[BYPASS] dlsym_fcntl\n");

    ret = dlsym_fcntl(fd, cmd, arg);

    debug_info("[BYPASS]\t dlsym_fcntl -> %d\n", ret);
  }

  debug_info("[BYPASS] << After fcntl()\n");

  return ret;
}

int access ( const char *path, int mode )
{
  struct stat64 stats;

  debug_info("[BYPASS] >> Begin access...\n");
  debug_info("[BYPASS]    1) path %s\n", path);
  debug_info("[BYPASS]    2) mode %d\n", mode);

  int ret = -1;

  // This if checks if variable path passed as argument starts with the expand prefix.
  if (is_xpn_prefix(path))
  {
    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    // It is an XPN partition, so we redirect the syscall to expand syscall
    debug_info("[BYPASS] xpn_access\n");

    if (__lxstat64(_STAT_VER, path, &stats)) {
      debug_info("[BYPASS]\t xpn_access -> -1\n");
      debug_info("[BYPASS] << After access()\n");

      return -1;
    }

    if (mode == F_OK) {
      debug_info("[BYPASS]\t xpn_access -> 0\n");
      debug_info("[BYPASS] << After access()\n");

      return 0;
    }

    if ((mode & X_OK) == 0 || (stats.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)))
    {
      debug_info("[BYPASS]\t xpn_access -> 0\n");
      debug_info("[BYPASS] << After access()\n");

      return 0;
    }
  }
  // Not an XPN partition. We must link with the standard library
  else
  {
    debug_info("[BYPASS] dlsym_access\n");

    ret = dlsym_access(path, mode);

    debug_info("[BYPASS]\t dlsym_access -> %d\n", ret);
  }

  debug_info("[BYPASS] << After access()\n");

  return ret;
}

char *realpath ( const char *restrict path, char *restrict resolved_path )
{
  debug_info("[BYPASS] >> Begin realpath...\n");
  debug_info("[BYPASS] 1) Path %s\n", path);

  // This if checks if variable path passed as argument starts with the expand prefix.
  if (is_xpn_prefix(path))
  {
    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    // It is an XPN partition, so we redirect the syscall to expand syscall
    debug_info("[BYPASS] xpn_realpath...\n");

    strcpy(resolved_path, path);

    return resolved_path;
  }
  // Not an XPN partition. We must link with the standard library
  else
  {
    debug_info("[BYPASS] Begin dlsym_realpath...\n");

    return dlsym_realpath(path, resolved_path);
  }
}

char * __realpath_chk ( const char * path, char * resolved_path, __attribute__((__unused__)) size_t resolved_len )
{
  debug_info("[BYPASS] >> Begin __realpath_chk...\n");
  debug_info("[BYPASS] 1) Path %s\n", path);

  // TODO: taken from https://refspecs.linuxbase.org/LSB_4.1.0/LSB-Core-generic/LSB-Core-generic/libc---realpath-chk-1.html
  // -> ... If resolved_len is less than PATH_MAX, then the function shall abort, and the program calling it shall exit.
  //
  //if (resolved_len < PATH_MAX) {
  //    return -1;
  //}

  // This if checks if variable path passed as argument starts with the expand prefix.
  if (is_xpn_prefix(path))
  {
    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    // It is an XPN partition, so we redirect the syscall to expand syscall
    debug_info("[BYPASS] xpn___realpath_chk...\n");

    strcpy(resolved_path, path);

    return resolved_path;
  }
  // Not an XPN partition. We must link with the standard library
  else
  {
    debug_info("[BYPASS] Begin dlsym_realpath...\n");

    return dlsym_realpath(path, resolved_path);
  }
}

int fsync ( int fd ) //TODO
{ 
  debug_info("[BYPASS] >> Begin fsync...\n");
  debug_info("[BYPASS] 1) fd %d\n", fd);

  int ret = -1;

  struct generic_fd virtual_fd = fdstable_get ( fd );

  // This if checks if variable fd passed as argument is a expand fd.
  if(virtual_fd.type == FD_XPN)
  {
    debug_info("[BYPASS] xpn_fsync\n");

    //TODO
    ret = 0;

    debug_info("[BYPASS]\t xpn_fsync -> %d\n", ret);
  }
  else
  {
    debug_info("[BYPASS] dlsym_fsync\n");

    ret = dlsym_fsync(fd);

    debug_info("[BYPASS]\t dlsym_fsync -> %d\n", ret);
  }

  debug_info("[BYPASS] << After fsync()\n");

  return ret;
}

int flock(int fd, int operation)
{
  int ret = -1;

  debug_info("[BYPASS] >> Begin flock...\n");
  debug_info("[BYPASS]    * fd=%d\n",        fd);
  debug_info("[BYPASS]    * operation=%d\n", operation);

  struct generic_fd virtual_fd = fdstable_get ( fd );

  // This if checks if variable fd passed as argument is a expand fd.
  if(virtual_fd.type == FD_XPN)
  {
    //TODO
    return 0;
  }
  // Not an XPN partition. We must link with the standard library
  else
  {
    debug_info("[BYPASS]\t try to dlsym_flock %d,%d\n", fd, operation);

    ret = dlsym_flock(fd, operation);

    debug_info("[BYPASS]\t dlsym_flock %d,%d -> %d\n", fd, operation, ret);
  }

  debug_info("[BYPASS] << After flock...\n");
  
  return ret;
}

// MPI API

int MPI_Init ( int *argc, char ***argv )
{
  char *value;

  debug_info("[BYPASS] >> Begin MPI_Init\n");

  // We must initialize expand if it has not been initialized yet.
  xpn_adaptor_keepInit ();

  // It is an XPN partition, so we redirect the syscall to expand syscall
  value = getenv("XPN_IS_MPI_SERVER");
  if (NULL == value) {
    debug_info("[BYPASS] << After MPI_Init\n");

    return PMPI_Init(argc, argv);
  }

  debug_info("[BYPASS] << After MPI_Init\n");

  return MPI_SUCCESS;
}

int MPI_Init_thread ( int *argc, char ***argv, int required, int *provided )
{
  char *value;

  debug_info("[BYPASS] >> Begin MPI_Init_thread\n");

  // We must initialize expand if it has not been initialized yet.
  xpn_adaptor_keepInit ();

  // It is an XPN partition, so we redirect the syscall to expand syscall
  value = getenv("XPN_IS_MPI_SERVER");
  if (NULL == value) {
    debug_info("[BYPASS] << After MPI_Init_thread\n");

    return PMPI_Init_thread( argc, argv, required, provided );
  }
  debug_info("[BYPASS] << After MPI_Init_thread\n");

  return MPI_SUCCESS;
}

int MPI_Finalize (void)
{
  char *value;

  debug_info("[BYPASS] >> Begin MPI_Finalize\n");

  value = getenv("XPN_IS_MPI_SERVER");
  if (NULL != value && xpn_adaptor_initCalled == 1) {
    debug_info("[BYPASS] xpn_destroy\n");

    xpn_destroy();
  }

  debug_info("[BYPASS] << After MPI_Finalize\n");

  return PMPI_Finalize();
}
