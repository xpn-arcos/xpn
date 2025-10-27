
/*
 *  Copyright 2020-2025 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Elias del Pozo Puñal
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


#ifndef _XPN_PROXY_CLIENT_H_
#define _XPN_PROXY_CLIENT_H_

  /* ... Include / Inclusion ........................................... */

     #include "config.h"

     #include <stdio.h>
     #include <stdlib.h>
     #include <string.h>
     #include <signal.h>
     #include <unistd.h>
     #include <sys/socket.h>
     #include <netinet/in.h>
     #include <arpa/inet.h>

     #include "xpn_server/xpn_server_ops.h"
     #include "base/socket.h"
     #include "base/service_socket.h"
     #include "base/utils.h"


  /* ... Functions / Funciones ......................................... */

  // xpn_cwd.c
  char *      xpn_getcwd (char *path, size_t size);
  int         xpn_chdir  (char *path);

  // xpn_dir.c
  int         xpn_mkdir (const char *path, mode_t perm);
  int         xpn_rmdir (const char *path);

  // xpn_init.c
  int         xpn_init    ( void );
  int         xpn_destroy ( void );

  // xpn_open.c
  int         xpn_creat     (const char *path, mode_t perm);
  int         xpn_open      (const char *path, int flags , ...);
  int         xpn_close     (int fd);
  int         xpn_unlink    (const char *path);
  int         xpn_rename    (const char *path, const char *newpath);
  int         xpn_fstat     (int fd, struct stat *sb);
  int         xpn_stat      (const char *path, struct stat *sb);
  int         xpn_chown     (const char *path, uid_t owner, gid_t group);
  int         xpn_fchown    (int fd, uid_t owner, gid_t group);
  int         xpn_chmod     (const char *path, mode_t mode);
  int         xpn_fchmod    (int fd, mode_t mode);
  int         xpn_truncate  (const char *path, off_t length);
  int         xpn_ftruncate (int fd, off_t length);
  int         xpn_dup       (int fd);
  int         xpn_dup2      (int fd, int fd2);

  // xpn_opendir.c
  DIR *           xpn_opendir   (const char *path);
  int             xpn_closedir  (DIR *dirp);
  struct dirent*  xpn_readdir   (DIR *dirp);
  void            xpn_rewinddir (DIR *dirp);

  // xpn_rw.c
  ssize_t     xpn_read   (int fd, void *buffer, size_t size);
  ssize_t     xpn_write  (int fd, const void *buffer, size_t size);
  off_t       xpn_lseek  (int fd, off_t offset, int flag);

  // xpn_fopen.c
  FILE *      xpn_fopen (const char *filename, const char *mode);

  // xpn_fclose.c
  int         xpn_fclose (FILE *stream);

  // xpn_fread
  //int       xpn_getc           (FILE *stream);
  size_t      xpn_fread          (void *ptr, size_t size, size_t nmemb, FILE *stream);
  size_t      xpn_fread_prefetch (void *ptr, size_t size, size_t nmemb, FILE *stream);

  // xpn_fwrite.c
  //int       xpn_putc   (int c, FILE *stream);
  size_t      xpn_fwrite (const void *ptr, size_t size, size_t nmemb, FILE *stream);

  // xpn_fflush.c
  int         xpn_fflush (FILE *stream);

  // xpn_fseek.c
  int         xpn_fseek (FILE *stream, off_t offset, int whence);

  // xpn_ftell.c
  long        xpn_ftell (FILE *stream);

  // xpn_fileno.c
  int         xpn_fileno (FILE *stream);

  // xpn_setbuf.c
  void        xpn_setbuf     (FILE *stream, char *buf);
  void        xpn_setbuffer  (FILE *stream, char *buf, size_t size);
  void        xpn_setlinebuf (FILE *stream);

  // xpn_setvbuf.c
  int     xpn_setvbuf (FILE *stream, char *buf, int mode, size_t size);


  /* ................................................................... */

#endif

