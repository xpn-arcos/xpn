  /* ... Include / Inclusion ........................................... */

  #include "base/syscall_poxis.h"



  /* ... Functions / Funciones ......................................... */


  // File API
  int (*real_open)(char *, int, mode_t) = NULL;
  int dlsym_open(char *path, int flags, mode_t mode)
  {
    debug_info("dlsym_open: Before open\n");
    debug_info("dlsym_open: Path => %s\n",path);

    if (real_open == NULL){
        real_open = dlsym(RTLD_NEXT,"open");
    }
    
    int fd = real_open((char *)path, flags, mode);

    debug_info("dlsym_open: (%s,%o) return %d\n",path,flags,fd);

    return fd;
  }

  int (*real_open64)(char *, int, mode_t) = NULL;
  int dlsym_open64(char *path, int flags, mode_t mode)
  {
    debug_info("dlsym_open64: Before open\n");
    debug_info("dlsym_open64: Path => %s\n",path);

    if (real_open64 == NULL){
        real_open64 = dlsym(RTLD_NEXT,"open64");
    }
    
    int fd = real_open64((char *)path, flags, mode);

    debug_info("dlsym_open64: (%s,%o) return %d\n",path,flags,fd);

    return fd;
  }


  int (*real_creat)(const char *, mode_t) = NULL;
  int dlsym_creat(const char *path, mode_t mode)
  {
    debug_info("dlsym_creat: Before creat\n");
    debug_info("dlsym_creat: Path => %s\n",path);

    if (real_creat == NULL){
        real_creat = dlsym(RTLD_NEXT,"creat");
    }

    int fd = real_creat(path,mode);

    debug_info("dlsym_creat: (%s,%o) return %d\n",path,mode,fd);
   
    return fd;
  }


  int (*real_ftruncate)(int, off_t) = NULL;
  int dlsym_ftruncate(int fd, off_t length)
  {
    debug_info("Before ftruncate....\n");

    if (real_ftruncate == NULL){
        real_ftruncate = dlsym(RTLD_NEXT,"ftruncate");
    }

    return real_ftruncate(fd, length);
  }


  ssize_t (*real_read)(int, void*, size_t) = NULL;
  ssize_t dlsym_read(int fd, void *buf, size_t nbyte)
  {
    debug_info("Before read....\n");

    if (real_read == NULL){
        real_read = dlsym(RTLD_NEXT,"read");
    }

    return real_read(fd,buf, nbyte);
  }


  ssize_t (*real_write)(int, const void*, size_t) = NULL;
  ssize_t dlsym_write(int fd, void *buf, size_t nbyte)
  {
    debug_info("Before write....\n");

    if (real_write == NULL){
        real_write = dlsym(RTLD_NEXT,"write");
    }

    return real_write(fd,buf, nbyte);
  }


  off_t (*real_lseek)(int, off_t, int) = NULL;
  off_t dlsym_lseek(int fd, off_t offset, int whence)
  {
    debug_info("Before lseek....\n");

    if (real_lseek == NULL){
        real_lseek = dlsym(RTLD_NEXT,"lseek");
    }

    return real_lseek(fd,offset, whence);
  }









  int dlsym_lxstat64(int, const char *, struct stat64 *)
  {

  }

  int dlsym_xstat64(int, const char *, struct stat64 *)
  {

  }

  int dlsym_fxstat64(int, int, struct stat64 *)
  {

  }

  int dlsym_lstat(int, char *, struct stat *)
  {

  }

  int dlsym_stat(int, char *, struct stat *)
  {

  }

  int dlsym_fstat(int, int, struct stat *)
  {

  }






  int (*real_close)(int) = NULL;
  int dlsym_close(int fd)
  {
    debug_info("Before close....\n");

    if (real_close == NULL){
        real_close = dlsym(RTLD_NEXT,"close");
    }

    return real_close(fd);
  }


  int (*real_unlink)(char *) = NULL;
  int dlsym_unlink(char *path)
  {
    debug_info("dlsym_unlink: before unlink...\n");

    if (real_unlink == NULL){
        real_unlink = dlsym(RTLD_NEXT,"unlink");
    }
    
    return real_unlink((char *)path);
  }


  

  // Directory API

  DIR* (*real_opendir)(char*) = NULL;
  DIR* dlsym_opendir(char *dirname)
  {
    debug_info("dlsym_opendir: before opendir...\n");

    if (real_opendir == NULL){
        real_opendir = dlsym(RTLD_NEXT,"opendir");
    }
    
    return real_opendir((char *)dirname);
  }

  DIR* (*real_opendir64)(char*) = NULL;
  DIR* dlsym_opendir64(char *dirname)
  {
    debug_info("dlsym_opendir64: before opendir64...\n");

    if (real_opendir64 == NULL){
        real_opendir64 = dlsym(RTLD_NEXT,"opendir64");
    }
    
    return real_opendir64((char *)dirname);
  }


  int (*real_mkdir)(char *, mode_t) = NULL;
  int dlsym_mkdir(char *path, mode_t mode)
  {
    debug_info("dlsym_mkdir: before mkdir...\n");

    if (real_mkdir == NULL){
        real_mkdir = dlsym(RTLD_NEXT,"mkdir");
    }
    
    return real_mkdir((char *)path,mode);
  }

  struct dirent * (*real_readdir)(DIR *) = NULL;
  struct dirent * dlsym_readdir(DIR *dirp)
  {
    debug_info("dlsym_readdir: before readdir...\n");

    if (real_readdir == NULL){
        real_readdir = dlsym(RTLD_NEXT,"readdir");
    }
    
    return real_readdir(dirp);
  }

  int (*real_closedir)(DIR*) = NULL;
  int dlsym_closedir(DIR*)
  {
    debug_info("dlsym_closedir: before closedir...\n");

    if (real_closedir == NULL){
        real_closedir = dlsym(RTLD_NEXT,"closedir");
    }
    
    return real_closedir(dirp);
  }


  int (*real_rmdir)(char *) = NULL;
  int dlsym_rmdir(char *path)
  {
    debug_info("dlsym_rmdir: before rmdir...\n");

    if (real_rmdir == NULL){
        real_rmdir = dlsym(RTLD_NEXT,"rmdir");
    }
    
    return real_rmdir((char *)path);
  }




  // Proccess API
  int (*real_fork)() = NULL;
  int dlsym_fork(void)
  {
    debug_info("dlsym_fork: before fork...\n");

    if (real_fork == NULL){
        real_fork = dlsym(RTLD_NEXT,"fork");
    }
    
    return real_fork();
  }


  int (*real_dup)(int) = NULL;
  int dlsym_dup(int fd)
  {
    debug_info("dlsym_dup: before dup...\n");

    if (real_dup == NULL){
        real_dup = dlsym(RTLD_NEXT,"dup");
    }
    
    return real_dup(fd);
  }

  int (*real_dup2)(int, int) = NULL;
  int dlsym_dup2(int fd, int fd2)
  {
    debug_info("dlsym_dup2: before dup2...\n");

    if (real_dup2 == NULL){
        real_dup2 = dlsym(RTLD_NEXT,"dup2");
    }
    
    return real_dup2(fd, fd2);
  }


  void (*real_exit)(int) = NULL;
  void dlsym_exit(int status)
  {
    debug_info("dlsym_exit: before exit...\n");

    if (real_exit == NULL){
        real_exit = dlsym(RTLD_NEXT,"exit");
    }
    
    return real_exit(status);
  }




  // Manager API
  int (*real_chdir)(char *) = NULL;
  int dlsym_chdir(char * path)
  {
    debug_info("dlsym_chdir: before chdir...\n");

    if (real_chdir == NULL){
        real_chdir = dlsym(RTLD_NEXT,"chdir");
    }
    
    return real_chdir((char *)path);
  }


  int (*real_chmod)(char *, mode_t) = NULL;
  int dlsym_chmod( char *path, mode_t mode)
  {
    debug_info("dlsym_chmod: before chmod...\n");

    if (real_chmod == NULL){
        real_chmod = dlsym(RTLD_NEXT,"chmod");
    }
    
    return real_chmod((char *)path, mode);
  }


  int (*real_fchmod)(int, mode_t) = NULL;
  int dlsym_fchmod(int fd, mode_t mode)
  {
    debug_info("dlsym_fchmod: before fchmod...\n");

    if (real_fchmod == NULL){
        real_fchmod = dlsym(RTLD_NEXT,"fchmod");
    }
    
    return real_chmod(fd,mode);
  }


  int (*real_chown)(char *, uid_t, gid_t) = NULL;
  int dlsym_chown(char *path, uid_t owner, gid_t group)
  {
    debug_info("dlsym_chown: before chown...\n");

    if (real_chown == NULL){
        real_chown = dlsym(RTLD_NEXT,"chown");
    }
    
    return real_chown((char *)path, owner, group);
  }


  int (*real_fcntl)(int, int, long) = NULL;
  int dlsym_fcntl(int fd, int cmd, long arg)
  {
    debug_info("dlsym_fcntl: before fcntl...\n");

    if (real_fcntl == NULL){
        real_fcntl = dlsym(RTLD_NEXT,"fcntl");
    }
    
    return real_fcntl(fd, cmd, arg);
  }