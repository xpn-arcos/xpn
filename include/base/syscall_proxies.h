  // File API
  int dlsym_open(char *, int, mode_t);
  int dlsym_open64(char *, int, mode_t);

  int dlsym_creat(const char *, mode_t);

  int dlsym_ftruncate(int, off_t);

  ssize_t dlsym_read(int, void*, size_t);

  ssize_t dlsym_write(int, const void*, size_t);

  off_t dlsym_lseek(int, off_t, int);

  int dlsym_lxstat64(int, const char *, struct stat64 *);
  int dlsym_xstat64(int, const char *, struct stat64 *);
  int dlsym_fxstat64(int, int, struct stat64 *);
  int dlsym_lstat(int, char *, struct stat *);
  int dlsym_stat(int, char *, struct stat *);
  int dlsym_fstat(int, int, struct stat *);

  int dlsym_close(int);

  int dlsym_unlink(char *);

  

  // Directory API
  DIR* dlsym_opendir(char*);
  DIR* dlsym_opendir64(char*);

  int dlsym_mkdir(char *, mode_t);

  struct dirent * dlsym_readdir(DIR *);

  int dlsym_closedir(DIR*);

  int dlsym_rmdir(char *);



  // Proccess API
  int dlsym_fork(void);

  int dlsym_dup(int);
  int dlsym_dup2(int, int);

  void dlsym_exit(int);



  // Manager API
  int dlsym_chdir(char *);

  int dlsym_chmod(char *, mode_t);
  int dlsym_fchmod(int, mode_t);

  int dlsym_chown(char *, uid_t, gid_t);

  int dlsym_fcntl(int, int, long);