
  /*
   *  Copyright 2020-2023 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
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


  /* ... Variables / Variables ......................................... */

  int     (*real_open    )(char *, int, mode_t)   = NULL;
  int     (*real_open64  )(char *, int, mode_t)   = NULL;
  int     (*real___open_2)(char *, int)           = NULL;
  int     (*real_creat   )(const char *, mode_t)  = NULL;
  int     (*real_close   )(int) = NULL;

  ssize_t (*real_read )(int, void*, size_t)       = NULL;
  ssize_t (*real_write)(int, const void*, size_t) = NULL;
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

  DIR*              (*real_opendir  )(char*) = NULL;
  DIR*              (*real_opendir64)(char*) = NULL;
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
  void*   (*real_mmap)(void *, size_t, int, int, int, off_t) = NULL;


  /* ... Functions / Funciones ......................................... */

  //
  // File API
  //

  int dlsym_open(char *path, int flags)
  {
    debug_info("dlsym_open: before open...\n");
    debug_info("dlsym_open: Path => %s\n",path);
    debug_info("dlsym_open: flags => %d\n",flags);

    if (real_open == NULL) {
        real_open = (int (*)(char *, int, mode_t)) dlsym(RTLD_NEXT,"open");
    }
    
    int fd = real_open((char *)path, flags, 0);

    debug_info("dlsym_open: (%s,%o) return %d\n",path,flags,fd);

    return fd;
  }


  int dlsym_open2(char *path, int flags, mode_t mode)
  {
    debug_info("dlsym_open2: before open...\n");
    debug_info("dlsym_open2: Path => %s\n",path);
    debug_info("dlsym_open2: flags => %d\n",flags);
    debug_info("dlsym_open2: mode => %d\n",mode);

    if (real_open == NULL) {
        real_open = (int (*)(char *, int, mode_t)) dlsym(RTLD_NEXT,"open");
    }
    
    int fd = real_open((char *)path, flags, mode);

    debug_info("dlsym_open2: (%s,%o) return %d\n",path,flags,fd);

    return fd;
  }

  int dlsym_open64(char *path, int flags, mode_t mode)
  {
    debug_info("dlsym_open64: before open64...\n");
    debug_info("dlsym_open64: Path => %s\n",path);
    debug_info("dlsym_open64: flags => %d\n",flags);
    debug_info("dlsym_open64: mode => %d\n",mode);

    if (real_open64 == NULL){
        real_open64 = (int (*)(char *, int, mode_t)) dlsym(RTLD_NEXT,"open64");
    }
    
    int fd = real_open64((char *)path, flags, mode);

    debug_info("dlsym_open64: (%s,%o) return %d\n",path,flags,fd);

    return fd;
  }

  int dlsym___open_2(char *path, int flags)
  {
    debug_info("dlsym___open_2: before __open_2...\n");
    debug_info("dlsym___open_2: Path => %s\n",path);

    if (real___open_2 == NULL) {
        real___open_2 = (int (*)(char *, int)) dlsym(RTLD_NEXT,"__open");
    }
    
    int fd = real___open_2((char *)path, flags);

    debug_info("dlsym___open_2: (%s,%o) return %d\n",path,flags,fd);

    return fd;
  }

  int dlsym_creat(const char *path, mode_t mode)
  {
    debug_info("dlsym_cleat: before creat...\n");
    debug_info("dlsym_creat: Path => %s\n",path);

    if (real_creat == NULL){
        real_creat = (int (*)(const char *, mode_t)) dlsym(RTLD_NEXT,"creat");
    }

    int fd = real_creat(path,mode);

    debug_info("dlsym_creat: (%s,%o) return %d\n",path,mode,fd);
   
    return fd;
  }

  int dlsym_ftruncate(int fd, off_t length)
  {
    debug_info("dlsym_ftruncate: before ftruncate...\n");

    if (real_ftruncate == NULL){
        real_ftruncate = (int (*)(int, off_t)) dlsym(RTLD_NEXT,"ftruncate");
    }

    return real_ftruncate(fd, length);
  }

  ssize_t dlsym_read(int fd, void *buf, size_t nbyte)
  {
    debug_info("dlsym_read: before read...\n");

    if (real_read == NULL){
        real_read = (ssize_t (*)(int, void*, size_t)) dlsym(RTLD_NEXT,"read");
    }

    return real_read(fd,buf, nbyte);
  }

  ssize_t dlsym_write(int fd, void *buf, size_t nbyte)
  {
    debug_info("dlsym_write: before write...\n");

    if (real_write == NULL){
        real_write = (ssize_t (*)(int, const void*, size_t)) dlsym(RTLD_NEXT,"write");
    }

    return real_write(fd,buf, nbyte);
  }

  off_t dlsym_lseek(int fd, off_t offset, int whence)
  {
    debug_info("dlsym_lseek: before lseek...\n");

    if (real_lseek == NULL){
        real_lseek = (off_t (*)(int, off_t, int)) dlsym(RTLD_NEXT,"lseek");
    }

    return real_lseek(fd, offset, whence);
  }

  off64_t  dlsym_lseek64(int fd, off64_t offset, int whence)
  {
    debug_info("dlsym_lseek64: before lseek64...\n");

    if (real_lseek64 == NULL){
        real_lseek64 = (off64_t (*)(int, off64_t, int)) dlsym(RTLD_NEXT,"lseek64");
    }

    return real_lseek64(fd, offset, whence);
  }

  int dlsym_lxstat64(int ver, const char *path, struct stat64 *buf)
  {
    debug_info("dlsym_lxstat64: before _lxstat64...\n");

    if (real_lxstat64 == NULL){
        real_lxstat64 = (int (*)(int, const char *, struct stat64 *)) dlsym(RTLD_NEXT,"__lxstat64");
    }

    return real_lxstat64(ver,(char *)path, buf);
  }

  int dlsym_xstat64(int ver, const char *path, struct stat64 *buf)
  {
    debug_info("dlsym_xstat64: before _xstat64...\n");

    if (real_xstat64 == NULL){
        real_xstat64 = (int (*)(int, const char *, struct stat64 *)) dlsym(RTLD_NEXT,"__xstat64");
    }

    return real_xstat64(ver,(char *)path, buf);
  }

  int dlsym_fxstat64(int ver, int fd, struct stat64 *buf)
  {
    debug_info("dlsym_fxstat64: before _fxstat64...\n");

    if (real_fxstat64 == NULL){
        real_fxstat64 = (int (*)(int, int, struct stat64 *)) dlsym(RTLD_NEXT,"__fxstat64");
        //real_fxstat64 = (int (*)(int, int, struct stat64 *)) dlsym(RTLD_NEXT,"__xstat64");
    }

    return real_fxstat64(ver,fd, buf);
  }

  int dlsym_lstat(int ver, const char *path, struct stat *buf)
  {
    debug_info("dlsym_lstat: before _lstat...\n");

    if (real_lstat == NULL){
        real_lstat = (int (*)(int, char *, struct stat *)) dlsym(RTLD_NEXT,"__lxstat");
    }

    return real_lstat(ver,(char *)path, buf);
  }

  int dlsym_stat(int ver, const char *path, struct stat *buf)
  {
    debug_info("dlsym_stat: before _lxstat...\n");

    if (real_stat == NULL){
        real_stat = (int (*)(int, char *, struct stat *)) dlsym(RTLD_NEXT,"__xstat");
    }

    return real_stat(ver,(char *)path, buf);
  }

  int dlsym_fstat(int ver, int fd, struct stat *buf)
  {
    debug_info("dlsym_fstat: before _fxstat...\n");

    if (real_fstat == NULL){
        real_fstat = (int (*)(int, int, struct stat *)) dlsym(RTLD_NEXT,"__fxstat");
    }

    return real_fstat(ver,fd, buf);
  }

  int dlsym_fstatat (int dfd, const char *path, struct stat *buf, int flags)
  {
    debug_info("dlsym_fstatat: before fstatat...\n");

    if (real_fstatat == NULL){
        real_fstatat = (int (*)(int, const char *, struct stat *, int)) dlsym(RTLD_NEXT,"fstatat");
    }

    return real_fstatat(dfd,(char *)path, buf, flags);
  }

  int dlsym_fstatat64 (int dfd, const char *path, struct stat64 *buf, int flags)
  {
    debug_info("dlsym_fstatat64: before fstatat64...\n");

    if (real_fstatat64 == NULL){
        real_fstatat64 = (int (*)(int, const char *, struct stat64 *, int)) dlsym(RTLD_NEXT,"fstatat64");
    }

    return real_fstatat64(dfd,(char *)path, buf, flags);
  }

  int dlsym_close(int fd)
  {
    debug_info("dlsym_close: before close...\n");

    if (real_close == NULL){
        real_close = (int (*)(int)) dlsym(RTLD_NEXT,"close");
    }

    return real_close(fd);
  }

  int dlsym_rename(const char *old_path, const char *new_path)
  {
    debug_info("dlsym_rename: before rename...\n");

    if (real_rename == NULL){
        real_rename = (int (*)(const char *, const  char *)) dlsym(RTLD_NEXT,"rename");
    }

    return real_rename(old_path, new_path);
  }

  int dlsym_unlink(char *path)
  {
    debug_info("dlsym_unlink: before unlink...\n");

    if (real_unlink == NULL){
        real_unlink = (int (*)(char *)) dlsym(RTLD_NEXT,"unlink");
    }
    
    return real_unlink((char *)path);
  }


  //
  // Directory API
  //

  DIR* dlsym_opendir(char *dirname)
  {
    debug_info("dlsym_opendir: before opendir...\n");

    if (real_opendir == NULL){
        real_opendir = (DIR* (*)(char*)) dlsym(RTLD_NEXT,"opendir");
    }
    
    return real_opendir((char *)dirname);
  }

  DIR* dlsym_opendir64(char *dirname)
  {
    debug_info("dlsym_opendir64: before opendir64...\n");

    if (real_opendir64 == NULL){
        real_opendir64 = (DIR* (*)(char*)) dlsym(RTLD_NEXT,"opendir64");
    }
    
    return real_opendir64((char *)dirname);
  }

  int dlsym_mkdir(char *path, mode_t mode)
  {
    debug_info("dlsym_mkdir: before mkdir...\n");

    if (real_mkdir == NULL){
        real_mkdir = (int (*)(char *, mode_t)) dlsym(RTLD_NEXT,"mkdir");
    }
    
    return real_mkdir((char *)path,mode);
  }

  struct dirent * dlsym_readdir(DIR *dirp)
  {
    debug_info("dlsym_readdir: before readdir...\n");

    if (real_readdir == NULL){
        real_readdir = (struct dirent * (*)(DIR *)) dlsym(RTLD_NEXT,"readdir");
    }
    
    return real_readdir(dirp);
  }

  struct dirent64 * dlsym_readdir64(DIR *dirp)
  {
    debug_info("dlsym_readdir64: before readdir64...\n");

    if (real_readdir64 == NULL){
        real_readdir64 = (struct dirent64 * (*)(DIR *)) dlsym(RTLD_NEXT,"readdir64");
    }
    
    return real_readdir64(dirp);
  }

  int dlsym_closedir(DIR* dirp)
  {
    debug_info("dlsym_closedir: before closedir...\n");

    if (real_closedir == NULL){
        real_closedir = (int (*)(DIR*)) dlsym(RTLD_NEXT,"closedir");
    }
    
    return real_closedir(dirp);
  }

  int dlsym_rmdir(char *path)
  {
    debug_info("dlsym_rmdir: before rmdir...\n");

    if (real_rmdir == NULL){
        real_rmdir = (int (*)(char *)) dlsym(RTLD_NEXT,"rmdir");
    }
    
    return real_rmdir((char *)path);
  }


  //
  // Proccess API
  //

  int dlsym_fork(void)
  {
    debug_info("dlsym_fork: before fork...\n");

    if (real_fork == NULL){
        real_fork = (int (*)()) dlsym(RTLD_NEXT,"fork");
    }
    
    return real_fork();
  }

  int dlsym_pipe(int pipefd[2])
  {
    debug_info("dlsym_pipe: before pipe...\n");

    if (real_pipe == NULL){
        real_pipe = (int (*)(int *)) dlsym(RTLD_NEXT,"pipe");
    }
    
    return real_pipe(pipefd);
  }

  int dlsym_dup(int fd)
  {
    debug_info("dlsym_dup: before dup...\n");

    if (real_dup == NULL){
        real_dup = (int (*)(int)) dlsym(RTLD_NEXT,"dup");
    }
    
    return real_dup(fd);
  }

  int dlsym_dup2(int fd, int fd2)
  {
    debug_info("dlsym_dup2: before dup2...\n");

    if (real_dup2 == NULL){
        real_dup2 = (int (*)(int, int)) dlsym(RTLD_NEXT,"dup2");
    }
    
    return real_dup2(fd, fd2);
  }

  void dlsym_exit(int status)
  {
    debug_info("dlsym_exit: before exit...\n");

    if (real_exit == NULL){
        real_exit = (void (*)(int)) dlsym(RTLD_NEXT,"exit");
    }
    
    real_exit(status);
  }


  //
  // File/Directory Metadata API
  //

  int dlsym_chdir(char * path)
  {
    debug_info("dlsym_chdir: before chdir...\n");

    if (real_chdir == NULL){
        real_chdir = (int (*)(char *)) dlsym(RTLD_NEXT,"chdir");
    }
    
    return real_chdir((char *)path);
  }

  int dlsym_chmod( char *path, mode_t mode)
  {
    debug_info("dlsym_chmod: before chmod...\n");

    if (real_chmod == NULL){
        real_chmod = (int (*)(char *, mode_t)) dlsym(RTLD_NEXT,"chmod");
    }
    
    return real_chmod((char *)path, mode);
  }

  int dlsym_fchmod(int fd, mode_t mode)
  {
    debug_info("dlsym_fchmod: before fchmod...\n");

    if (real_fchmod == NULL){
        real_fchmod = (int (*)(int, mode_t)) dlsym(RTLD_NEXT,"fchmod");
    }
    
    return real_fchmod(fd,mode);
  }

  int dlsym_chown(char *path, uid_t owner, gid_t group)
  {
    debug_info("dlsym_chown: before chown...\n");

    if (real_chown == NULL){
        real_chown = (int (*)(char *, uid_t, gid_t)) dlsym(RTLD_NEXT,"chown");
    }
    
    return real_chown((char *)path, owner, group);
  }

  int dlsym_fcntl(int fd, int cmd, long arg)
  {
    debug_info("dlsym_fcntl: before fcntl...\n");

    if (real_fcntl == NULL){
        real_fcntl = (int (*)(int, int, long)) dlsym(RTLD_NEXT,"fcntl");
    }
    
    return real_fcntl(fd, cmd, arg);
  }

  int dlsym_access(const char *path, int mode)
  {
    debug_info("dlsym_access: before access...\n");
    debug_info("dlsym_access: Path => %s\n",path);

    if (real_access == NULL) {
        real_access = (int (*)(const char *, int)) dlsym(RTLD_NEXT,"access");
    }
    
    int ret = real_access((char *)path, mode);

    debug_info("dlsym_access: (%s,%d) return %d\n",path,mode,ret);

    return ret;
  }

  char *dlsym_realpath(const char *restrict path, char *restrict resolved_path)
  {
    debug_info("dlsym_realpath: before realpath...\n");
    debug_info("dlsym_realpath: Path => %s\n",path);

    if (real_realpath == NULL) {
        real_realpath = (char* (*)(const char *restrict, char *restrict)) dlsym(RTLD_NEXT,"realpath");
    }
    
    char* ret = real_realpath((char *)path, (char *)resolved_path);

    debug_info("dlsym_access: (%s,%s) return %s\n",path,resolved_path,ret);

    return ret;
  }

  int dlsym_fsync(int fd)
  {
    debug_info("dlsym_fsync: before fsync...\n");

    if (real_fsync == NULL){
        real_fsync = (int (*)(int)) dlsym(RTLD_NEXT,"fsync");
    }
    
    return real_fsync(fd);
  }


  //
  // Memory API
  //

  void *dlsym_mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset)
  {
    debug_info("dlsym_mmap: before mmap...\n");
    debug_info("dlsym_mmap: fd => %d\n",fd);

    if (real_mmap == NULL) {
        real_mmap = (void *(*)(void *, size_t, int, int, int, off_t)) dlsym(RTLD_NEXT,"mmap");
    }
    
    char* ret = real_mmap(addr, length, prot, flags, fd, offset);

    debug_info("dlsym_mmap: (%d) return %p\n",fd,ret);

    return ret;
  } 
 

   /* ................................................................... */

