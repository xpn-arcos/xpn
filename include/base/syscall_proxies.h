  // File API
  int (*real_open)(char *, int, mode_t);
  int (*real_open64)(char *, int, mode_t);

  int (*real_creat)(const char *, mode_t);

  int (*real_ftruncate)(int, off_t);

  ssize_t (*real_read)(int, void*, size_t);

  ssize_t (*real_write)(int, const void*, size_t);

  off_t (*real_lseek)(int, off_t, int);

  int (*real_lxstat64)(int, const char *, struct stat64 *);
  int (*real_xstat64)(int, const char *, struct stat64 *);
  int (*real_fxstat64)(int, int, struct stat64 *);
  int (*real_lstat)(int, char *, struct stat *);
  int (*real_stat)(int, char *, struct stat *);
  int (*real_fstat)(int, int, struct stat *);

  int (*real_close)(int);

  int (*real_unlink)(char *);

  

  // Directory API
  DIR* (*real_opendir)(char*);
  DIR* (*real_opendir64)(char*);

  int (*real_mkdir)(char *, mode_t);

  struct dirent * (*real_readdir)(DIR *);

  int (*real_closedir)(DIR*);

  int (*real_rmdir)(char *);



  // Proccess API
  int (*real_fork)();

  int (*real_dup)(int);
  int (*real_dup2)(int, int);

  void (*real_exit)(int);



  // Manager API
  int (*real_chdir)(char *);

  int (*real_chmod)(char *, mode_t);
  int (*real_fchmod)(int, mode_t);

  int (*real_chown)(char *, uid_t, gid_t);

  int (*real_fcntl)(int, int, long);