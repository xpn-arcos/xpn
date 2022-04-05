  /* ... Include / Inclusion ........................................... */

  #include "base/syscall_poxis.h"



  /* ... Functions / Funciones ......................................... */

  int (*real_fork)();
  real_fork = dlsym(RTLD_NEXT,"fork");

  int (*real_open)(char *, int, mode_t);
  real_open = dlsym(RTLD_NEXT,"open");

  int (*real_open64)(char *, int, mode_t);
  real_open64 = dlsym(RTLD_NEXT,"open64");

  int (*real_close)(int);
  real_close = dlsym(RTLD_NEXT,"close");

  int (*real_creat)(const char *, mode_t);
  real_creat = dlsym(RTLD_NEXT,"creat");

  int (*real_chdir)(char *);
  real_chdir = dlsym(RTLD_NEXT,"chdir");

  int (*real_mkdir)(char *, mode_t);
  real_mkdir = dlsym(RTLD_NEXT,"mkdir");

  int (*real_rmdir)(char *);
  real_rmdir = dlsym(RTLD_NEXT,"rmdir");

  int (*real_unlink)(char *);
  real_unlink = dlsym(RTLD_NEXT,"unlink");

  int (*real_chown)(char *, uid_t, gid_t);
  real_chown = dlsym(RTLD_NEXT,"chown");

  int (*real_ftruncate)(int, off_t);
  real_ftruncate = dlsym(RTLD_NEXT,"ftruncate");

  int (*real_dup)(int);
  real_dup = dlsym(RTLD_NEXT,"dup");

  int (*real_dup2)(int, int);
  real_dup2 = dlsym(RTLD_NEXT,"dup2");

  DIR* (*real_opendir)(char*);
  real_opendir = dlsym(RTLD_NEXT,"opendir");

  DIR* (*real_opendir64)(char*);
  real_opendir64 = dlsym(RTLD_NEXT,"readdir64");

  struct dirent * (*real_readdir)(DIR *);
  real_readdir = dlsym(RTLD_NEXT,"readdir");

  int (*real_closedir)(DIR*);
  real_closedir = dlsym(RTLD_NEXT,"closedir");

  int (*real_lxstat64)(int, const char *, struct stat64 *);
  real_lxstat64 = dlsym(RTLD_NEXT,"__lxstat64");

  int (*real_xstat64)(int, const char *, struct stat64 *);
  real_xstat64 = dlsym(RTLD_NEXT,"__xstat64");

  int (*real_fxstat64)(int, int, struct stat64 *);
  real_fxstat64 = dlsym(RTLD_NEXT,"__xstat64");

  int (*real_lstat)(int, char *, struct stat *);
  real_lstat = dlsym(RTLD_NEXT,"__lxstat");

  int (*real_stat)(int, char *, struct stat *);
  real_stat = dlsym(RTLD_NEXT,"__xstat");

  int (*real_fstat)(int, int, struct stat *);
  real_fstat = dlsym(RTLD_NEXT,"__fxstat");

  int (*real_chmod)(char *, mode_t);
  real_chmod = dlsym(RTLD_NEXT,"chmod");

  int (*real_fchmod)(int, mode_t);
  real_fchmod = dlsym(RTLD_NEXT,"fchmod");

  ssize_t (*real_read)(int, void*, size_t);
  real_read = dlsym(RTLD_NEXT,"read");

  ssize_t (*real_write)(int, const void*, size_t);
  real_write = dlsym(RTLD_NEXT,"write");

  off_t (*real_lseek)(int, off_t, int);
  real_lseek = dlsym(RTLD_NEXT,"lseek");

  int (*real_fcntl)(int, int, long);
  real_fcntl = dlsym(RTLD_NEXT,"fcntl");

  void (*real_exit)(int);
  real_exit = dlsym(RTLD_NEXT,"exit");