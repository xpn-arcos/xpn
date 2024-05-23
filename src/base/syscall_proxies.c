
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

#include "base/syscall_proxies.h"


/* ... Const / Const ................................................. */


/* ... Global variables / Variables globales ........................ */

int     (*real_open    )(char *, int, mode_t)  = NULL;
int     (*real_open64  )(char *, int, mode_t)  = NULL;
int     (*real___open_2)(char *, int)          = NULL;
int     (*real_creat   )(const char *, mode_t) = NULL;
int     (*real_mkstemp )(char*)                = NULL;
int     (*real_close   )(int)                  = NULL;

ssize_t (*real_read )(int, void*, size_t)       = NULL;
ssize_t (*real_write)(int, const void*, size_t) = NULL;

ssize_t (*real_pread   )(int, void *, size_t, off_t)       = NULL;
ssize_t (*real_pwrite  )(int, const void *, size_t, off_t) = NULL;
ssize_t (*real_pread64 )(int, void *, size_t, off_t)       = NULL;
ssize_t (*real_pwrite64)(int, const void *, size_t, off_t) = NULL;

off_t   (*real_lseek)(int, off_t, int)          = NULL;
off64_t (*real_lseek64)(int, off64_t, int)      = NULL;
int     (*real_ftruncate)(int, off_t)           = NULL;

int     (*real_stat     )(int, char *,       struct stat   *) = NULL;
int     (*real_xstat64  )(int, const char *, struct stat64 *) = NULL;
int     (*real_lstat    )(int, char *,       struct stat   *) = NULL;
int     (*real_lxstat64 )(int, const char *, struct stat64 *) = NULL;
int     (*real_fstat    )(int, int,          struct stat   *) = NULL;
int     (*real_fxstat64 )(int, int,          struct stat64 *) = NULL;
int     (*real_fstatat  )(int, const char *, struct stat   *, int) = NULL;
int     (*real_fstatat64)(int, const char *, struct stat64 *, int) = NULL;

int     (*real_rename)(const char *, const  char *) = NULL;
int     (*real_unlink)(char *) = NULL;
int     (*real_remove)(char *) = NULL;

FILE*   (*real_fopen )(const char *, const char *) = NULL;
FILE*   (*real_fdopen)(int, const char *)          = NULL;
int     (*real_fclose)(FILE *)                     = NULL;

size_t  (*real_fread )(void *, size_t, size_t, FILE *)       = NULL;
size_t  (*real_fwrite)(const void *, size_t, size_t, FILE *) = NULL;

int     (*real_fseek )(FILE *, long int, int) = NULL;
long    (*real_ftell )(FILE *) = NULL;
void    (*real_rewind)(FILE *) = NULL;
int     (*real_feof ) (FILE *) = NULL;

DIR*              (*real_opendir  )(char*) = NULL;
DIR*              (*real_opendir64)(char*) = NULL;
long              (*real_telldir  )(DIR *) = NULL;
void              (*real_seekdir  )(DIR *, long) = NULL;
struct dirent   * (*real_readdir  )(DIR *) = NULL;
struct dirent64 * (*real_readdir64)(DIR *) = NULL;
int               (*real_closedir )(DIR *) = NULL;

int     (*real_mkdir)(char *, mode_t) = NULL;
int     (*real_rmdir)(char *) = NULL;

int     (*real_fork)(void) = NULL;
int     (*real_pipe)(int *) = NULL;
int     (*real_dup)(int) = NULL;
int     (*real_dup2)(int, int) = NULL;
void    (*real_exit)(int) = NULL;
int     (*real_chdir)(char *) = NULL;
int     (*real_chmod)(char *, mode_t) = NULL;
int     (*real_fchmod)(int, mode_t) = NULL;
int     (*real_chown)(char *, uid_t, gid_t) = NULL;
int     (*real_fcntl)(int, int, long) = NULL;
int     (*real_access)(const char *, int) = NULL;
char*   (*real_realpath)(const char *restrict, char *restrict) = NULL;
int     (*real_fsync)(int) = NULL;
int     (*real_flock)(int, int) = NULL;
void*   (*real_mmap)(void *, size_t, int, int, int, off_t) = NULL;


/* ... Functions / Funciones ......................................... */

// File API
int dlsym_open (char *path, int flags)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_open] >> Begin\n");

  if (real_open == NULL) {
      real_open = (int (*)(char *, int, mode_t)) dlsym(RTLD_NEXT, "open");
  }
  
  int fd = real_open((char *)path, flags, 0);

  debug_info("[SYSCALL_PROXIES] [dlsym_open] >> End\n");

  return fd;
}

int dlsym_open2 (char *path, int flags, mode_t mode)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_open2] >> Begin\n");

  if (real_open == NULL) {
      real_open = (int (*)(char *, int, mode_t)) dlsym(RTLD_NEXT, "open");
  }
  
  int fd = real_open((char *)path, flags, mode);

  debug_info("[SYSCALL_PROXIES] [dlsym_open2] >> End\n");

  return fd;
}

int dlsym_open64 (char *path, int flags, mode_t mode)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_open64] >> Begin\n");

  if (real_open64 == NULL){
      real_open64 = (int (*)(char *, int, mode_t)) dlsym(RTLD_NEXT, "open64");
  }
  
  int fd = real_open64((char *)path, flags, mode);

  debug_info("[SYSCALL_PROXIES] [dlsym_open64] >> End\n");

  return fd;
}

int dlsym___open_2 (char *path, int flags)
{
  debug_info("[SYSCALL_PROXIES] [dlsym___open_2] >> Begin\n");

  if (real___open_2 == NULL) {
      real___open_2 = (int (*)(char *, int)) dlsym(RTLD_NEXT, "__open");
  }
  
  int fd = real___open_2((char *)path, flags);

  debug_info("[SYSCALL_PROXIES] [dlsym___open_2] >> End\n");

  return fd;
}

int dlsym_creat (const char *path, mode_t mode)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_creat] >> Begin\n");

  if (real_creat == NULL) {
      real_creat = (int (*)(const char *, mode_t)) dlsym(RTLD_NEXT, "creat");
  }

  int fd = real_creat(path,mode);

  debug_info("[SYSCALL_PROXIES] [dlsym_creat] >> End\n");
 
  return fd;
}

int dlsym_mkstemp (char *template)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_mkstemp] >> Begin\n");

  if (real_mkstemp == NULL) {
      real_mkstemp = (int(*)(char *)) dlsym(RTLD_NEXT, "mkstemp");
  }
  
  int ret = real_mkstemp(template);

  debug_info("[SYSCALL_PROXIES] [dlsym_mkstemp] >> End\n");

  return ret;
}

int dlsym_ftruncate (int fd, off_t length)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_ftruncate] >> Begin\n");

  if (real_ftruncate == NULL){
      real_ftruncate = (int (*)(int, off_t)) dlsym(RTLD_NEXT, "ftruncate");
  }

  int ret = real_ftruncate(fd, length);

  debug_info("[SYSCALL_PROXIES] [dlsym_ftruncate] >> End\n");
 
  return ret;
}

ssize_t dlsym_read (int fd, void *buf, size_t nbyte)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_read] >> Begin\n");

  if (real_read == NULL){
      real_read = (ssize_t (*)(int, void*, size_t)) dlsym(RTLD_NEXT, "read");
  }

  ssize_t ret = real_read(fd, buf, nbyte);

  debug_info("[SYSCALL_PROXIES] [dlsym_read] >> End\n");

  return ret;
}

ssize_t dlsym_write (int fd, void *buf, size_t nbyte)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_write] >> Begin\n");

  if (real_write == NULL){
      real_write = (ssize_t (*)(int, const void*, size_t)) dlsym(RTLD_NEXT, "write");
  }

  ssize_t ret = real_write(fd, buf, nbyte);

  debug_info("[SYSCALL_PROXIES] [dlsym_write] >> End\n");

  return ret;
}

ssize_t dlsym_pread (int fd, void *buf, size_t count, off_t offset)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_pread] >> Begin\n");

  if (real_pread == NULL){
      real_pread = (ssize_t (*)(int, void *, size_t, off_t)) dlsym(RTLD_NEXT, "pread");
  }

  ssize_t ret = real_pread(fd, buf, count, offset);

  debug_info("[SYSCALL_PROXIES] [dlsym_pread] >> End\n");

  return ret;
}

ssize_t dlsym_pwrite (int fd, const void *buf, size_t count, off_t offset)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_pwrite] >> Begin\n");

  if (real_pwrite == NULL){
      real_pwrite = (ssize_t (*)(int, const void *, size_t, off_t)) dlsym(RTLD_NEXT, "pwrite");
  }

  ssize_t ret = real_pwrite(fd, buf, count, offset);

  debug_info("[SYSCALL_PROXIES] [dlsym_pwrite] >> End\n");

  return ret;
}

ssize_t dlsym_pread64 (int fd, void *buf, size_t count, off_t offset)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_pread64] >> Begin\n");

  if (real_pread64 == NULL){
      real_pread64 = (ssize_t (*)(int, void *, size_t, off_t)) dlsym(RTLD_NEXT, "pread64");
  }

  ssize_t ret = real_pread64(fd, buf, count, offset);

  debug_info("[SYSCALL_PROXIES] [dlsym_pread64] >> End\n");

  return ret;
}

ssize_t dlsym_pwrite64 (int fd, const void *buf, size_t count, off_t offset)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_pwrite64] >> Begin\n");

  if (real_pwrite64 == NULL){
      real_pwrite64 = (ssize_t (*)(int, const void *, size_t, off_t)) dlsym(RTLD_NEXT, "pwrite64");
  }

  ssize_t ret = real_pwrite64(fd, buf, count, offset);

  debug_info("[SYSCALL_PROXIES] [dlsym_pwrite64] >> End\n");

  return ret;
}

off_t dlsym_lseek (int fd, off_t offset, int whence)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_lseek] >> Begin\n");

  if (real_lseek == NULL){
      real_lseek = (off_t (*)(int, off_t, int)) dlsym(RTLD_NEXT, "lseek");
  }

  off_t ret = real_lseek(fd, offset, whence);

  debug_info("[SYSCALL_PROXIES] [dlsym_lseek] >> End\n");

  return ret;
}

off64_t dlsym_lseek64 (int fd, off64_t offset, int whence)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_lseek64] >> Begin\n");

  if (real_lseek64 == NULL){
      real_lseek64 = (off64_t (*)(int, off64_t, int)) dlsym(RTLD_NEXT, "lseek64");
  }

  off64_t ret = real_lseek64(fd, offset, whence);

  debug_info("[SYSCALL_PROXIES] [dlsym_lseek64] >> End\n");

  return ret;
}

int dlsym_lxstat64 (int ver, const char *path, struct stat64 *buf)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_lxstat64] >> Begin\n");

  if (real_lxstat64 == NULL){
      real_lxstat64 = (int (*)(int, const char *, struct stat64 *)) dlsym(RTLD_NEXT, "__lxstat64");
  }

  int ret = real_lxstat64(ver,(char *)path, buf);

  debug_info("[SYSCALL_PROXIES] [dlsym_lxstat64] >> End\n");

  return ret;
}

int dlsym_xstat64 (int ver, const char *path, struct stat64 *buf)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_xstat64] >> Begin\n");

  if (real_xstat64 == NULL){
      real_xstat64 = (int (*)(int, const char *, struct stat64 *)) dlsym(RTLD_NEXT, "__xstat64");
  }

  int ret = real_xstat64(ver,(char *)path, buf);

  debug_info("[SYSCALL_PROXIES] [dlsym_xstat64] >> End\n");

  return ret;
}

int dlsym_fxstat64 (int ver, int fd, struct stat64 *buf)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_fxstat64] >> Begin\n");

  if (real_fxstat64 == NULL){
      real_fxstat64 = (int (*)(int, int, struct stat64 *)) dlsym(RTLD_NEXT, "__fxstat64");
  }

  int ret = real_fxstat64(ver,fd, buf);

  debug_info("[SYSCALL_PROXIES] [dlsym_fxstat64] >> End\n");

  return ret;
}

int dlsym_lstat (int ver, const char *path, struct stat *buf)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_lstat] >> Begin\n");

  if (real_lstat == NULL){
      real_lstat = (int (*)(int, char *, struct stat *)) dlsym(RTLD_NEXT, "__lxstat");
  }

  int ret = real_lstat(ver,(char *)path, buf);

  debug_info("[SYSCALL_PROXIES] [dlsym_lstat] >> End\n");

  return ret;
}

int dlsym_stat (int ver, const char *path, struct stat *buf)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_stat] >> Begin\n");

  if (real_stat == NULL){
      real_stat = (int (*)(int, char *, struct stat *)) dlsym(RTLD_NEXT, "__xstat");
  }

  int ret = real_stat(ver,(char *)path, buf);

  debug_info("[SYSCALL_PROXIES] [dlsym_stat] >> End\n");

  return ret;
}

int dlsym_fstat (int ver, int fd, struct stat *buf)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_fstat] >> Begin\n");

  if (real_fstat == NULL){
      real_fstat = (int (*)(int, int, struct stat *)) dlsym(RTLD_NEXT, "__fxstat");
  }

  int ret = real_fstat(ver,fd, buf);

  debug_info("[SYSCALL_PROXIES] [dlsym_fstat] >> End\n");

  return ret;
}

int dlsym_fstatat (int dfd, const char *path, struct stat *buf, int flags)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_fstatat] >> Begin\n");

  if (real_fstatat == NULL){
      real_fstatat = (int (*)(int, const char *, struct stat *, int)) dlsym(RTLD_NEXT, "fstatat");
  }

  int ret = real_fstatat(dfd,(char *)path, buf, flags);

  debug_info("[SYSCALL_PROXIES] [dlsym_fstatat] >> End\n");

  return ret;
}

int dlsym_fstatat64 (int dfd, const char *path, struct stat64 *buf, int flags)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_fstatat64] >> Begin\n");

  if (real_fstatat64 == NULL){
      real_fstatat64 = (int (*)(int, const char *, struct stat64 *, int)) dlsym(RTLD_NEXT, "fstatat64");
  }

  int ret = real_fstatat64(dfd,(char *)path, buf, flags);

  debug_info("[SYSCALL_PROXIES] [dlsym_fstatat64] >> End\n");

  return ret;
}

int dlsym_close (int fd)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_close] >> Begin\n");

  if (real_close == NULL){
      real_close = (int (*)(int)) dlsym(RTLD_NEXT, "close");
  }

  int ret = real_close(fd);

  debug_info("[SYSCALL_PROXIES] [dlsym_close] >> End\n");

  return ret;
}

int dlsym_rename (const char *old_path, const char *new_path)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_rename] >> Begin\n");

  if (real_rename == NULL){
      real_rename = (int (*)(const char *, const  char *)) dlsym(RTLD_NEXT, "rename");
  }

  int ret = real_rename(old_path, new_path);

  debug_info("[SYSCALL_PROXIES] [dlsym_rename] >> End\n");

  return ret;
}

int dlsym_unlink (char *path)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_unlink] >> Begin\n");

  if (real_unlink == NULL){
      real_unlink = (int (*)(char *)) dlsym(RTLD_NEXT, "unlink");
  }
  
  int ret = real_unlink((char *)path);

  debug_info("[SYSCALL_PROXIES] [dlsym_unlink] >> End\n");

  return ret;
}

int dlsym_remove (char *path)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_remove] >> Begin\n");

  if (real_remove == NULL){
      real_remove = (int (*)(char *)) dlsym(RTLD_NEXT, "remove");
  }
  
  int ret = real_remove((char *)path);

  debug_info("[SYSCALL_PROXIES] [dlsym_remove] >> End\n");

  return ret;
}

// File API (stdio)
FILE* dlsym_fopen(const char *filename, const char *mode)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_fopen] >> Begin\n");

  if (real_fopen == NULL) {
      real_fopen = (FILE* (*)(const char *, const char *)) dlsym(RTLD_NEXT, "fopen");
  }
  
  FILE* fd = real_fopen((const char *) filename, mode);

  debug_info("[SYSCALL_PROXIES] [dlsym_fopen] >> End\n");

  return fd;
}

FILE* dlsym_fdopen(int fd, const char *mode)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_fdopen] >> Begin\n");

  if (real_fdopen == NULL) {
      real_fdopen = (FILE* (*)(int, const char *)) dlsym(RTLD_NEXT, "fdopen");
  }
  
  FILE* fp = real_fdopen(fd, mode);

  debug_info("[SYSCALL_PROXIES] [dlsym_fdopen] >> End\n");

  return fp;
}

int  dlsym_fclose(FILE *stream)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_fclose] >> Begin\n");

  if (real_fclose == NULL) {
      real_fclose = (int (*)(FILE*)) dlsym(RTLD_NEXT, "fclose");
  }
  
  int ret = real_fclose(stream);

  debug_info("[SYSCALL_PROXIES] [dlsym_fclose] >> End\n");

  return ret;
}

size_t dlsym_fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_fread] >> Begin\n");

  if (real_fread == NULL) {
      real_fread = (size_t (*)(void *, size_t, size_t, FILE *)) dlsym(RTLD_NEXT, "fread");
  }
  
  size_t ret = real_fread(ptr, size, nmemb, stream);

  debug_info("[SYSCALL_PROXIES] [dlsym_fread] >> End\n");

  return ret;
}

size_t dlsym_fwrite (const void *ptr, size_t size, size_t nmemb, FILE *stream)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_fwrite] >> Begin\n");

  if (real_fwrite == NULL) {
      real_fwrite = (size_t (*)(const void *, size_t, size_t, FILE *)) dlsym(RTLD_NEXT, "fwrite");
  }
  
  size_t ret = real_fwrite(ptr, size, nmemb, stream);

  debug_info("[SYSCALL_PROXIES] [dlsym_fwrite] >> End\n");

  return ret;
}

int dlsym_fseek (FILE *stream, long int offset, int whence)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_fseek] >> Begin\n");

  if (real_fseek == NULL) {
      real_fseek = (int (*)(FILE *, long int, int)) dlsym(RTLD_NEXT, "fseek");
  }
  
  int ret = real_fseek(stream, offset, whence);

  debug_info("[SYSCALL_PROXIES] [dlsym_fseek] >> End\n");

  return ret;
}

long dlsym_ftell (FILE *stream)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_ftell] >> Begin\n");

  if (real_ftell== NULL) {
      real_ftell = (long (*)(FILE *)) dlsym(RTLD_NEXT, "ftell");
  }
  
  int ret = real_ftell(stream);

  debug_info("[SYSCALL_PROXIES] [dlsym_ftell] >> End\n");

  return ret;
}

void dlsym_rewind (FILE *stream)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_rewind] >> Begin\n");

  if (real_rewind == NULL) {
      real_rewind = (void(*)(FILE *)) dlsym(RTLD_NEXT, "rewind");
  }
  
  real_rewind(stream);

  debug_info("[SYSCALL_PROXIES] [dlsym_rewind] >> End\n");
}

int dlsym_feof (FILE *stream)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_feof] >> Begin\n");

  if (real_feof== NULL) {
      real_feof = (int (*)(FILE *)) dlsym(RTLD_NEXT, "feof");
  }
  
  int ret = real_feof(stream);

  debug_info("[SYSCALL_PROXIES] [dlsym_feof] >> End\n");

  return ret;
}

// Directory API
DIR* dlsym_opendir (char *dirname)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_opendir] >> Begin\n");

  if (real_opendir == NULL){
      real_opendir = (DIR* (*)(char*)) dlsym(RTLD_NEXT, "opendir");
  }
  
  DIR* ret = real_opendir((char *)dirname);

  debug_info("[SYSCALL_PROXIES] [dlsym_opendir] >> End\n");

  return ret;
}

DIR* dlsym_opendir64 (char *dirname)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_opendir64] >> Begin\n");

  if (real_opendir64 == NULL){
      real_opendir64 = (DIR* (*)(char*)) dlsym(RTLD_NEXT, "opendir64");
  }
  
  DIR* ret = real_opendir64((char *)dirname);

  debug_info("[SYSCALL_PROXIES] [dlsym_opendir64] >> End\n");

  return ret;
}

long dlsym_telldir (DIR *dirp)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_telldir] >> Begin\n");

  if (real_telldir == NULL){
      real_telldir = (long (*)(DIR *)) dlsym(RTLD_NEXT, "telldir");
  }
  
  long ret = real_telldir((DIR *)dirp);

  debug_info("[SYSCALL_PROXIES] [dlsym_telldir] >> End\n");

  return ret;
}

void dlsym_seekdir (DIR *dirp, long loc)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_seekdir] >> Begin\n");

  if (real_seekdir == NULL){
      real_seekdir = (void (*)(DIR *, long)) dlsym(RTLD_NEXT, "seekdir");
  }
  
  real_seekdir((DIR *)dirp, (long) loc);

  debug_info("[SYSCALL_PROXIES] [dlsym_seekdir] >> End\n");
}

int dlsym_mkdir (char *path, mode_t mode)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_mkdir] >> Begin\n");

  if (real_mkdir == NULL){
      real_mkdir = (int (*)(char *, mode_t)) dlsym(RTLD_NEXT, "mkdir");
  }
  
  int ret = real_mkdir((char *)path,mode);

  debug_info("[SYSCALL_PROXIES] [dlsym_mkdir] >> End\n");

  return ret;
}

struct dirent * dlsym_readdir (DIR *dirp)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_readdir] >> Begin\n");

  if (real_readdir == NULL){
      real_readdir = (struct dirent * (*)(DIR *)) dlsym(RTLD_NEXT, "readdir");
  }
  
  struct dirent * ret = real_readdir(dirp);

  debug_info("[SYSCALL_PROXIES] [dlsym_readdir] >> End\n");

  return ret;
}

struct dirent64 * dlsym_readdir64 (DIR *dirp)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_readdir64] >> Begin\n");

  if (real_readdir64 == NULL){
      real_readdir64 = (struct dirent64 * (*)(DIR *)) dlsym(RTLD_NEXT, "readdir64");
  }
  
  struct dirent64 * ret = real_readdir64(dirp);

  debug_info("[SYSCALL_PROXIES] [dlsym_readdir64] >> End\n");

  return ret;
}

int dlsym_closedir (DIR* dirp)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_closedir] >> Begin\n");

  if (real_closedir == NULL){
      real_closedir = (int (*)(DIR*)) dlsym(RTLD_NEXT, "closedir");
  }
  
  int ret = real_closedir(dirp);

  debug_info("[SYSCALL_PROXIES] [dlsym_closedir] >> End\n");

  return ret;
}

int dlsym_rmdir (char *path)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_rmdir] >> Begin\n");

  if (real_rmdir == NULL){
      real_rmdir = (int (*)(char *)) dlsym(RTLD_NEXT, "rmdir");
  }
  
  int ret = real_rmdir((char *)path);

  debug_info("[SYSCALL_PROXIES] [dlsym_rmdir] >> End\n");

  return ret;
}

// Proccess API
int dlsym_fork (void)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_fork] >> Begin\n");

  if (real_fork == NULL){
      real_fork = (int (*)()) dlsym(RTLD_NEXT, "fork");
  }
  
  int ret = real_fork();

  debug_info("[SYSCALL_PROXIES] [dlsym_fork] >> End\n");

  return ret;
}

int dlsym_pipe (int pipefd[2])
{
  debug_info("[SYSCALL_PROXIES] [dlsym_pipe] >> Begin\n");

  if (real_pipe == NULL){
      real_pipe = (int (*)(int *)) dlsym(RTLD_NEXT, "pipe");
  }
  
  int ret = real_pipe(pipefd);

  debug_info("[SYSCALL_PROXIES] [dlsym_pipe] >> End\n");

  return ret;
}

int dlsym_dup (int fd)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_dup] >> Begin\n");

  if (real_dup == NULL){
      real_dup = (int (*)(int)) dlsym(RTLD_NEXT, "dup");
  }
  
  int ret = real_dup(fd);

  debug_info("[SYSCALL_PROXIES] [dlsym_dup] >> End\n");

  return ret;
}

int dlsym_dup2 (int fd, int fd2)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_dup2] >> Begin\n");

  if (real_dup2 == NULL){
      real_dup2 = (int (*)(int, int)) dlsym(RTLD_NEXT, "dup2");
  }
  
  int ret = real_dup2(fd, fd2);

  debug_info("[SYSCALL_PROXIES] [dlsym_dup2] >> End\n");

  return ret;
}

void dlsym_exit (int status)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_exit] >> Begin\n");

  if (real_exit == NULL){
      real_exit = (void (*)(int)) dlsym(RTLD_NEXT, "exit");
  }
  
  real_exit(status);

  debug_info("[SYSCALL_PROXIES] [dlsym_exit] >> End\n");
}

// File/Directory Metadata API
int dlsym_chdir (char * path)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_chdir] >> Begin\n");

  if (real_chdir == NULL){
      real_chdir = (int (*)(char *)) dlsym(RTLD_NEXT, "chdir");
  }
  
  int ret = real_chdir((char *)path);

  debug_info("[SYSCALL_PROXIES] [dlsym_chdir] >> End\n");

  return ret;
}

int dlsym_chmod (char *path, mode_t mode)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_chmod] >> Begin\n");

  if (real_chmod == NULL){
      real_chmod = (int (*)(char *, mode_t)) dlsym(RTLD_NEXT, "chmod");
  }
  
  int ret = real_chmod((char *)path, mode);

  debug_info("[SYSCALL_PROXIES] [dlsym_chmod] >> End\n");

  return ret;
}

int dlsym_fchmod (int fd, mode_t mode)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_fchmod] >> Begin\n");

  if (real_fchmod == NULL){
      real_fchmod = (int (*)(int, mode_t)) dlsym(RTLD_NEXT, "fchmod");
  }
  
  int ret = real_fchmod(fd,mode);

  debug_info("[SYSCALL_PROXIES] [dlsym_fchmod] >> End\n");

  return ret;
}

int dlsym_chown (char *path, uid_t owner, gid_t group)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_chown] >> Begin\n");

  if (real_chown == NULL){
      real_chown = (int (*)(char *, uid_t, gid_t)) dlsym(RTLD_NEXT, "chown");
  }
  
  int ret = real_chown((char *)path, owner, group);

  debug_info("[SYSCALL_PROXIES] [dlsym_chown] >> End\n");

  return ret;
}

int dlsym_fcntl (int fd, int cmd, long arg)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_fcntl] >> Begin\n");

  if (real_fcntl == NULL){
      real_fcntl = (int (*)(int, int, long)) dlsym(RTLD_NEXT, "fcntl");
  }
  
  int ret = real_fcntl(fd, cmd, arg);

  debug_info("[SYSCALL_PROXIES] [dlsym_fcntl] >> End\n");

  return ret;
}

int dlsym_access (const char *path, int mode)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_access] >> Begin\n");

  if (real_access == NULL) {
      real_access = (int (*)(const char *, int)) dlsym(RTLD_NEXT, "access");
  }
  
  int ret = real_access((char *)path, mode);

  debug_info("[SYSCALL_PROXIES] [dlsym_access] >> End\n");

  return ret;
}

char *dlsym_realpath (const char *restrict path, char *restrict resolved_path)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_realpath] >> Begin\n");

  if (real_realpath == NULL) {
      real_realpath = (char* (*)(const char *restrict, char *restrict)) dlsym(RTLD_NEXT, "realpath");
  }
  
  char* ret = real_realpath((char *)path, (char *)resolved_path);

  debug_info("[SYSCALL_PROXIES] [dlsym_realpath] >> End\n");

  return ret;
}

int dlsym_fsync (int fd)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_fsync] >> Begin\n");

  if (real_fsync == NULL){
      real_fsync = (int (*)(int)) dlsym(RTLD_NEXT, "fsync");
  }
  
  int ret = real_fsync(fd);

  debug_info("[SYSCALL_PROXIES] [dlsym_fsync] >> End\n");

  return ret;
}

int dlsym_flock (int fd, int operation)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_flock] >> Begin\n");

  if (real_flock == NULL){
      real_flock = (int (*)(int, int)) dlsym(RTLD_NEXT, "fsync");
  }
  
  int ret = real_flock(fd, operation);

  debug_info("[SYSCALL_PROXIES] [dlsym_flock] >> End\n");

  return ret;
}

// Memory API
void *dlsym_mmap (void *addr, size_t length, int prot, int flags, int fd, off_t offset)
{
  debug_info("[SYSCALL_PROXIES] [dlsym_mmap] >> Begin\n");

  if (real_mmap == NULL) {
      real_mmap = (void *(*)(void *, size_t, int, int, int, off_t)) dlsym(RTLD_NEXT, "mmap");
  }
  
  char* ret = real_mmap(addr, length, prot, flags, fd, offset);

  debug_info("[SYSCALL_PROXIES] [dlsym_mmap] >> End\n");

  return ret;
} 


/* ................................................................... */
