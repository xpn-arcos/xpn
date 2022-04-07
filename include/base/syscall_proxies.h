#ifndef _SYSCALL_PROXIES_H
#define _SYSCALL_PROXIES_H

  #include <dlfcn.h>
  #include <sys/stat.h>
  #include <dirent.h>
  #include <stdlib.h>

  #include "utils.h"


 #ifdef  __cplusplus
    extern "C" {
 #endif


  // File API
  int dlsym_open(char *path, int flags, mode_t mode);
  int dlsym_open64(char *path, int flags, mode_t mode);
  
  int dlsym_creat(const char *path, mode_t mode);

  int dlsym_ftruncate(int fd, off_t length);
  
  ssize_t dlsym_read(int fd, void *buf, size_t nbyte);
  ssize_t dlsym_write(int fd, void *buf, size_t nbyte);

  off_t dlsym_lseek(int fd, off_t offset, int whence);

  int dlsym_lxstat64(int ver, const char *path, struct stat64 *buf);
  int dlsym_xstat64(int ver, const char *path, struct stat64 *buf);
  int dlsym_fxstat64(int ver, int fd, struct stat64 *buf);
  int dlsym_lstat(int ver, const char *path, struct stat *buf);
  int dlsym_stat(int ver, const char *path, struct stat *buf);
  int dlsym_fstat(int ver, int fd, struct stat *buf);
  
  int dlsym_close(int fd);
  
  int dlsym_unlink(char *path);

  
  // Directory API

  DIR* dlsym_opendir(char *dirname);
  DIR* dlsym_opendir64(char *dirname);

  int dlsym_mkdir(char *path, mode_t mode);
  
  struct dirent * dlsym_readdir(DIR *dirp);
  struct dirent64 * dlsym_readdir64(DIR *dirp);

  int dlsym_closedir(DIR*);
  
  int dlsym_rmdir(char *path);
  

  // Proccess API
  int dlsym_fork(void);
  
  int dlsym_dup(int fd);
  int dlsym_dup2(int fd, int fd2);

  void dlsym_exit(int status);


  // Manager API
  int dlsym_chdir(char * path);
  
  int dlsym_chmod( char *path, mode_t mode);
  
  int dlsym_fchmod(int fd, mode_t mode);
  
  int dlsym_chown(char *path, uid_t owner, gid_t group);
  
  int dlsym_fcntl(int fd, int cmd, long arg);


  #ifdef  __cplusplus
    }
  #endif

#endif