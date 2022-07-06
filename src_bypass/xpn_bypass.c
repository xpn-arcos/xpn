
  /*
   *  Copyright 2020-2022 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
   *
   *  This file is part of mpiServer.
   *
   *  mpiServer is free software: you can redistribute it and/or modify
   *  it under the terms of the GNU Lesser General Public License as published by
   *  the Free Software Foundation, either version 3 of the License, or
   *  (at your option) any later version.
   *
   *  mpiServer is distributed in the hope that it will be useful,
   *  but WITHOUT ANY WARRANTY; without even the implied warranty of
   *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   *  GNU Lesser General Public License for more details.
   *
   *  You should have received a copy of the GNU Lesser General Public License
   *  along with mpiServer.  If not, see <http://www.gnu.org/licenses/>.
   *
   */ 


  // some definitions
  #include "xpn_bypass.h"

  /**
   * This variable indicates if expand has already been initialized or not.
   * 0 indicates that expand has NOT been initialized yet.
   * 1 indicates that expand has already been initialized.
   */
  static int xpn_adaptor_initCalled = 0;
  static int xpn_adaptor_initCalled_getenv = 0; //env variable obtained

  /**
   * This variable contains the prefix which will be considerated as expand partition.
   */
  char *xpn_adaptor_partition_prefix = "xpn://"; //Original
  //char *xpn_adaptor_partition_prefix = "/xpn/";
    
  /*
  char *xpn_adaptor_flog_name  = "/tmp/EXPAND.LOG" ;
  void xpn_adaptor_log ( char * msg, ... )
  {
      FILE *flog;
      va_list vargs;

      va_start(vargs,msg);
      flog = fopen(xpn_adaptor_flog_name,"a+");
      if (NULL != flog) {
          vfdebug_info(flog,msg,vargs);
          fclose(flog);
      }
      va_end(vargs);
  }
  */



  // fd table management

  void fdstable_init ( void ){
    for (int i = 0; i < MAX_FDS; ++i)
    {
      fdstable[i].type = FD_FREE;
    }
  }

  struct generic_fd fdstable_get ( int fd ){
    //debug_info("GET FSTABLE %d  %d  %d\n", fd, fdstable[fd].type, fdstable[fd].real_fd);
    return fdstable[fd];
  }

  int fdstable_put ( struct generic_fd fd ){
    for (int i = 0; i < MAX_FDS; ++i)
    {
      if ( fdstable[i].type == FD_FREE ){
        fdstable[i] = fd;
        //debug_info("PUT FSTABLE %d  %d  %d\n", i, fdstable[i].type, fdstable[i].real_fd);
        return i;
      }
    }

    return -1;
  }

  int fdstable_remove ( int fd ){
    fdstable[fd].type = FD_FREE;
    return 0;
  }

  // Dir table management

  void fdsdirtable_init ( void ){
    for (int i = 0; i < MAX_DIRS; ++i)
    {
      fdsdirtable[i] = NULL;
    }
  }

  int fdsdirtable_search ( DIR * dir ) {
    for (int i = 0; i < MAX_DIRS; ++i)
    {
      if ( fdsdirtable[i] == dir ){
        return i;
      }
    }

    return -1;
  }

  int fdsdirtable_put ( DIR * dir ){
    for (int i = 0; i < MAX_DIRS; ++i)
    {
      if ( fdsdirtable[i] == NULL ){
        fdsdirtable[i] = dir;
        return 0;
      }
    }

    return -1;
  }

  int fdsdirtable_remove ( DIR * dir ){
    for (int i = 0; i < MAX_DIRS; ++i)
    {
      if ( fdsdirtable[i] == dir ){
        fdsdirtable[i] = NULL;
        return 0;
      }
    }

    return -1;
  }



  /**
   * This function checks if expand has already been initialized.
   * If not, it initialize it.
   */
  void xpn_adaptor_keepInit ( void )
  {
    int ret;

    debug_info("Before xpn_adaptor_keepInit\n");

    if (xpn_adaptor_initCalled_getenv == 0)
    {
      char * xpn_adaptor_initCalled_env = getenv("INITCALLED");
      xpn_adaptor_initCalled = 0;
      if (xpn_adaptor_initCalled_env != NULL)
      {
        xpn_adaptor_initCalled = atoi(xpn_adaptor_initCalled_env);
      }
      xpn_adaptor_initCalled_getenv = 1;
    }
    
    if (0 == xpn_adaptor_initCalled)
    {
      // If expand has not been initialized, then initialize it.
      debug_info("Before xpn_init()\n");

      xpn_adaptor_initCalled = 1; //TODO: Delete
      setenv("INITCALLED", "1", 1);

      fdstable_init ();
      fdsdirtable_init ();

      ret = xpn_init();

      debug_info("After xpn_init()\n");

      if (ret < 0)
      {
        debug_info("xpn_init: Expand couldn't be initialized\n");
        //xpn_adaptor_log("xpn_init: Expand couldn't be initialized\n");
        xpn_adaptor_initCalled = 0;
        setenv("INITCALLED", "0", 1);
      }
      else
      {
        xpn_adaptor_initCalled = 1;
        setenv("INITCALLED", "1", 1);
      }
    }
    debug_info("End xpn_adaptor_keepInit\n");
  }



  // File API

  //int open(const char *path, int flags, [mode_t mode])
  int open(const char *path, int flags, mode_t mode)
  {
    int ret, fd;

    debug_info("Before open(%s,%o,%o)...\n",path,flags,mode);
    debug_info("Path => %s\n",path);

    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    // This if checks if variable path passed as argument starts with the expand prefix.
    if(!strncmp(xpn_adaptor_partition_prefix,path,strlen(xpn_adaptor_partition_prefix)))
    {
      // It is an XPN partition, so we redirect the syscall to expand syscall
      debug_info("Path => %s\n",path + strlen(xpn_adaptor_partition_prefix));

      fd = xpn_open((char *)(path + strlen(xpn_adaptor_partition_prefix)), flags, mode);

      debug_info("xpn.bypass: xpn_open(%s,%o) return %d\n",path+strlen(xpn_adaptor_partition_prefix),flags,fd);

      if(fd<0)
      {
        ret = fd;
      } 
      else{
        struct generic_fd virtual_fd;

        virtual_fd.type    = FD_XPN;
        virtual_fd.real_fd = fd;

        ret = fdstable_put ( virtual_fd );
      }
    }
    // Not an XPN partition. We must link with the standard library.
    else 
    {
      fd = dlsym_open((char *)path, flags, mode);

      if(fd<0)
      {
        ret = fd;
      } 
      else{
        struct generic_fd virtual_fd;

        virtual_fd.type    = FD_SYS;
        virtual_fd.real_fd = fd;

        ret = fdstable_put ( virtual_fd );
      }
    }

    return ret;
  }


  int open64(const char *path, int flags, mode_t mode)
  {
    int fd, ret;

    debug_info("Before open64.... %s\n",path);
    debug_info("1) Path => %s\n",path+strlen(xpn_adaptor_partition_prefix));
    debug_info("2) flags => %d\n",flags);

    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    if(!strncmp(xpn_adaptor_partition_prefix,path,strlen(xpn_adaptor_partition_prefix)))
    {
      debug_info("Path => %s\n",path+strlen(xpn_adaptor_partition_prefix));

      fd=xpn_open((char *)(path+strlen(xpn_adaptor_partition_prefix)),flags);

      debug_info("xpn.bypass: xpn_open(%s,%o) return %d\n",path+strlen(xpn_adaptor_partition_prefix),flags,fd);

      if(fd<0)
      {
        ret = fd;
      } 
      else{
        struct generic_fd virtual_fd;

        virtual_fd.type    = FD_XPN;
        virtual_fd.real_fd = fd;

        ret = fdstable_put ( virtual_fd );
      }
    }
    // Not an XPN partition. We must link with the standard library
    else
    {
      fd = dlsym_open64((char *)path, flags, mode);

      if(fd<0)
      {
        ret = fd;
      } 
      else{
        struct generic_fd virtual_fd;

        virtual_fd.type    = FD_SYS;
        virtual_fd.real_fd = fd;

        ret = fdstable_put ( virtual_fd );
      }
    }

    debug_info("OPEN64 %d\n", ret);

    return ret;
  }

  int creat(const char *path, mode_t mode)
  {
    int fd,ret;

    debug_info("Before creat....\n");

    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    if(!strncmp(xpn_adaptor_partition_prefix,path,strlen(xpn_adaptor_partition_prefix)))
    {
      fd = xpn_creat((char *)(path+strlen(xpn_adaptor_partition_prefix)),mode);

      debug_info("The file is %s",(char *)(path+strlen(xpn_adaptor_partition_prefix)));

      if(fd<0)
      {
        ret = fd;
      } 
      else{
        struct generic_fd virtual_fd;

        virtual_fd.type    = FD_XPN;
        virtual_fd.real_fd = fd;

        ret = fdstable_put ( virtual_fd );
      }
    }
    // Not an XPN partition. We must link with the standard library
    else
    {
      fd = dlsym_creat(path,mode);

      if(fd<0)
      {
        ret = fd;
      } 
      else{
        struct generic_fd virtual_fd;

        virtual_fd.type    = FD_SYS;
        virtual_fd.real_fd = fd;

        ret = fdstable_put ( virtual_fd );
      }
    }

    return ret;
  }

  int ftruncate(int fd, off_t length)
  {
    debug_info("Before ftruncate...\n");

    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    struct generic_fd virtual_fd = fdstable_get ( fd );

    if(virtual_fd.type == FD_XPN)
    {
      return xpn_ftruncate(virtual_fd.real_fd, length);
    }
    // Not an XPN partition. We must link with the standard library
    else if (virtual_fd.type == FD_SYS)
    {
      return dlsym_ftruncate(virtual_fd.real_fd, length);
    }
    else{
      return dlsym_ftruncate(fd, length);
    }

    return -1;
  }

  ssize_t read(int fd, void *buf, size_t nbyte)
  {         
    debug_info("Before read...\n");
    debug_info("read(fd=%d,*buf,nbyte=%ld)\n",fd,nbyte);
    debug_info("buffer= %s\n",buf);

    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    struct generic_fd virtual_fd = fdstable_get ( fd );

    if(virtual_fd.type == FD_XPN)
    { 
      return xpn_read(virtual_fd.real_fd, buf, nbyte);
    }
    // Not an XPN partition. We must link with the standard library
    else if (virtual_fd.type == FD_SYS)
    {
      return dlsym_read(virtual_fd.real_fd, buf, nbyte);
    }
    else{
      return dlsym_read(fd, buf, nbyte);
    }

    return -1;
  }

  ssize_t write(int fd, const void *buf, size_t nbyte)
  {
    debug_info("Before write...\n");
    debug_info("write(fd=%d,*buf=%s,nbyte=%ld)\n", fd, buf, nbyte);

    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    struct generic_fd virtual_fd = fdstable_get ( fd );

    if(virtual_fd.type == FD_XPN)
    {
      return xpn_write(virtual_fd.real_fd, (void *)buf, nbyte);
    }
    // Not an XPN partition. We must link with the standard library
    else if (virtual_fd.type == FD_SYS)
    {
      return dlsym_write(virtual_fd.real_fd, (void *)buf, nbyte);
    }
    else{
      return dlsym_write(fd, (void *)buf, nbyte);
    }

    return -1;
  }

  off_t lseek(int fd, off_t offset, int whence)
  {
    debug_info("Before lseek...\n");

    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    struct generic_fd virtual_fd = fdstable_get ( fd );

    if(virtual_fd.type == FD_XPN)
    {
      return xpn_lseek(virtual_fd.real_fd, offset, whence);
    }
    // Not an XPN partition. We must link with the standard library
    else if (virtual_fd.type == FD_SYS)
    {
      return dlsym_lseek(virtual_fd.real_fd, offset, whence);
    }
    else{
      return dlsym_lseek(fd, offset, whence);
    }

    return -1;
  }

  // For the moment we intercept __*stat64

  //int lstat64(const char *path, struct stat64 *buf)
  int __lxstat64(int ver, const char *path, struct stat64 *buf)
  {
    int ret;
    struct stat st;

    debug_info("Before lstat64... %s\n",path);
    debug_info("lstat64...path = %s\n",path+strlen(xpn_adaptor_partition_prefix));

    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    if(!strncmp(xpn_adaptor_partition_prefix,path,strlen(xpn_adaptor_partition_prefix)))
    {
      ret = xpn_stat((char *)(path+strlen(xpn_adaptor_partition_prefix)), &st);

      if(ret > 0){
        buf->st_dev = (__dev_t)st.st_dev;
        //buf->__st_ino   = (__ino_t)st.st_ino;
        buf->st_mode    = (__mode_t)st.st_mode;
        buf->st_nlink   = (__nlink_t)st.st_nlink;
        buf->st_uid = (__uid_t)st.st_uid;
        buf->st_gid = (__gid_t)st.st_gid;
        buf->st_rdev    = (__dev_t)st.st_rdev;
        //buf->__pad2 = st.st_;
        buf->st_size    = (__off64_t)st.st_size;
        buf->st_blksize = (__blksize_t)st.st_blksize;
        buf->st_blocks  = (__blkcnt64_t)st.st_blocks;
        //buf->st_atime = (__time_t)st.st_atime;
        //buf->__unused1;
        //buf->st_mtime = (__time_t)st.st_mtime;
        //buf->__unused2;
        //buf->st_ctime = (__time_t)st.st_ctime;
        //buf->__unused3 =
        buf->st_ino = (__ino64_t)st.st_ino;

        //ret = 0;
      }

      return ret;
    }
    // Not an XPN partition. We must link with the standard library
    else
    {
      return dlsym_lxstat64(ver,(char *)path, buf);
    }
  }

  //int stat64(const char *path, struct stat64 *buf)
  int __xstat64(int ver, const char *path, struct stat64 *buf)
  {
    int ret;
    struct stat st;

    debug_info("Before stat64... %s\n",path);
    debug_info("stat64...path = %s\n",path);

    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    if(!strncmp(xpn_adaptor_partition_prefix,path,strlen(xpn_adaptor_partition_prefix)))
    {
      ret = xpn_stat((char *)(path+strlen(xpn_adaptor_partition_prefix)), &st);

      if(ret > 0){
        buf->st_dev     = (__dev_t)st.st_dev;
        //buf->__st_ino   = (__ino_t)st.st_ino;
        buf->st_mode    = (__mode_t)st.st_mode;
        buf->st_nlink   = (__nlink_t)st.st_nlink;
        buf->st_uid = (__uid_t)st.st_uid;
        buf->st_gid = (__gid_t)st.st_gid;
        buf->st_rdev    = (__dev_t)st.st_rdev;
        //buf->__pad2 = st.st_;
        buf->st_size    = (__off64_t)st.st_size;
        buf->st_blksize     = (__blksize_t)st.st_blksize;
        buf->st_blocks  = (__blkcnt64_t)st.st_blocks;
        //buf->st_atime     = (__time_t)st.st_atime;
        //buf->__unused1;
        //buf->st_mtime     = (__time_t)st.st_mtime;
        //buf->__unused2;
        //buf->st_ctime     = (__time_t)st.st_ctime;
        //buf->__unused3 =
        buf->st_ino     = (__ino64_t)st.st_ino;

        //ret = 0;
      }

      return ret;
    }
    // Not an XPN partition. We must link with the standard library
    else
    {    
      return dlsym_xstat64(ver,(char *)path, buf);
    }    
  }

  //int fstat64(int fd, struct stat64 *buf)
  int __fxstat64(int ver, int fd, struct stat64 *buf)
  {
    int ret;
    struct stat st;

    debug_info(" Before fstat64... %d\n",fd);

    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    struct generic_fd virtual_fd = fdstable_get ( fd );

    if(virtual_fd.type == FD_XPN)
    {
      ret = xpn_fstat(virtual_fd.real_fd, &st);

      if(ret > 0){
        buf->st_dev     = (__dev_t)st.st_dev;
        //buf->__st_ino   = (__ino_t)st.st_ino;
        buf->st_mode    = (__mode_t)st.st_mode;
        buf->st_nlink   = (__nlink_t)st.st_nlink;
        buf->st_uid = (__uid_t)st.st_uid;
        buf->st_gid = (__gid_t)st.st_gid;
        buf->st_rdev    = (__dev_t)st.st_rdev;
        //buf->__pad2 = st.st_;
        buf->st_size    = (__off64_t)st.st_size;
        buf->st_blksize     = (__blksize_t)st.st_blksize;
        buf->st_blocks  = (__blkcnt64_t)st.st_blocks;
        //buf->st_atime     = (__time_t)st.st_atime;
        //buf->__unused1;
        //buf->st_mtime     = (__time_t)st.st_mtime;
        //buf->__unused2;
        //buf->st_ctime     = (__time_t)st.st_ctime;
        //buf->__unused3 = ;
        buf->st_ino     = (__ino64_t)st.st_ino;

        //ret = 0;
      }
    }
    // Not an XPN partition. We must link with the standard library
    else if (virtual_fd.type == FD_SYS)
    {
      ret = dlsym_fxstat64(ver,virtual_fd.real_fd, buf);
    }
    else{
      return dlsym_fxstat64(ver,fd, buf);
    }

    return ret;
  }

  //int lstat(const char *path, struct stat *buf) //old
  int __lxstat(int ver, const char *path, struct stat *buf)
  {
    int ret;

    debug_info("Before lstat... %s\n",path);

    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    if(!strncmp(xpn_adaptor_partition_prefix,path,strlen(xpn_adaptor_partition_prefix)))
    {
      debug_info("XPN:lstat:path = %s\n",path+strlen(xpn_adaptor_partition_prefix));
      ret = xpn_stat((char *)(path+strlen(xpn_adaptor_partition_prefix)), buf);
    }
    // Not an XPN partition. We must link with the standard library
    else
    {
      ret = dlsym_lstat(ver,(char *)path, buf);
    }

    return ret;
  }

  //int stat(const char *path, struct stat *buf) //old
  int __xstat(int ver, const char *path, struct stat *buf) // TODO
  {
    //char path2[1024];

    debug_info("Before stat...\n");
    debug_info("stat...path =>%s\n",path);

    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    if(!strncmp(xpn_adaptor_partition_prefix,path,strlen(xpn_adaptor_partition_prefix)))
    {
      if (0 == strncmp(path,"/xpn/htdocs",11)) {
    // TODO
          debug_info("into TODO\n");
          return stat(path, buf);
      } else {
          return xpn_stat((char *)(path+strlen(xpn_adaptor_partition_prefix)), buf);
      }
    }
    // Not an XPN partition. We must link with the standard library
    else
    {
      return dlsym_stat(ver,(char *)path, buf);
    } 
  }

  //int fstat(int fd, struct stat *buf) //old
  int __fxstat(int ver, int fd, struct stat *buf)
  {
    debug_info("Before fstat...\n");

    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    struct generic_fd virtual_fd = fdstable_get ( fd );

    if(virtual_fd.type == FD_XPN)
    {
      return xpn_fstat(virtual_fd.real_fd,buf);
    }
    // Not an XPN partition. We must link with the standard library
    else if (virtual_fd.type == FD_SYS)
    {
      return dlsym_fstat(ver,virtual_fd.real_fd,buf);
    }
    else{
      return dlsym_fstat(ver,fd,buf);
    }

    return -1;
  }

  int close(int fd)
  {
    int ret;
    debug_info("Before close....\n");
    debug_info("FD = %d\n", fd);

    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    struct generic_fd virtual_fd = fdstable_get ( fd );

    if(virtual_fd.type == FD_XPN)
    {
      ret = xpn_close(virtual_fd.real_fd);
      fdstable_remove ( fd );
      return ret;
    }
    // Not an XPN partition. We must link with the standard library
    else if (virtual_fd.type == FD_SYS)
    {
      ret = dlsym_close(virtual_fd.real_fd);
      fdstable_remove ( fd );
      return ret;
    }
    else{
      return dlsym_close(fd);
    }

    return -1;
  }

  int rename(const char *old_path, const char *new_path)
  {
    debug_info("Before rename....\n");

    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    if(!strncmp(xpn_adaptor_partition_prefix,old_path,strlen(xpn_adaptor_partition_prefix)) && !strncmp(xpn_adaptor_partition_prefix,new_path,strlen(xpn_adaptor_partition_prefix)))
    {
      debug_info("Old Path => %s\n",old_path+strlen(xpn_adaptor_partition_prefix));
      debug_info("New Path => %s\n",new_path+strlen(xpn_adaptor_partition_prefix));

      return xpn_rename((char *)(old_path+strlen(xpn_adaptor_partition_prefix)), (char *)(new_path+strlen(xpn_adaptor_partition_prefix)));
    }
    // Not an XPN partition. We must link with the standard library
    else 
    {
      return dlsym_rename(old_path, new_path);
    }
  }

  int unlink(const char *path)
  {
    debug_info("Before unlink...\n");
    debug_info("PATH %s\n", path);

    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    if(!strncmp(xpn_adaptor_partition_prefix,path,strlen(xpn_adaptor_partition_prefix)))
    {
      return(xpn_unlink((char *)(path+strlen(xpn_adaptor_partition_prefix))));
    }
    // Not an XPN partition. We must link with the standard library
    else
    {
      return dlsym_unlink((char *)path);
    }
  }



  // Directory API

  int mkdir(const char *path, mode_t mode)
  {
    debug_info("Before mkdir...\n");
    debug_info("PATH %s\n", path);

    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    if(!strncmp(xpn_adaptor_partition_prefix,path,strlen(xpn_adaptor_partition_prefix)))
    {
      debug_info("Before xpn_mkdir(%s)...\n",((char *)(path+strlen(xpn_adaptor_partition_prefix))));
      return xpn_mkdir( ((char *)(path+strlen(xpn_adaptor_partition_prefix))) ,mode );
    }
    // Not an XPN partition. We must link with the standard library
    else
    {
      return dlsym_mkdir((char *)path,mode);
    }
  }

  DIR *opendir(const char *dirname)
  {
    debug_info("Before opendir(%s)...\n", dirname);

    DIR * ret;

    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    if(!strncmp(xpn_adaptor_partition_prefix,dirname,strlen(xpn_adaptor_partition_prefix))) //TODO:Aqui falla
    {
      ret = xpn_opendir((char *)(dirname+strlen(xpn_adaptor_partition_prefix)));

      if ( ret != NULL ){
        fdsdirtable_put ( ret );
      }

      /*for (int i = 0; i < MAX_DIRS; ++i)
      {
        debug_info("%p\n", fdsdirtable[i]);
      }*/

      return ret;
    }
    // Not an XPN partition. We must link with the standard library
    else
    {
      return dlsym_opendir((char *)dirname);
    }
  }

  struct dirent *readdir(DIR *dirp)
  {
    struct dirent *ret;

    debug_info("Before readdir...\n");

    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    if( fdsdirtable_search( dirp ) != -1 )
    {
      ret=xpn_readdir(dirp);

      debug_info("After xpn_readdir()...\n");

      return ret;
    }
    // Not an XPN partition. We must link with the standard library
    else
    {
      return dlsym_readdir(dirp);
    }
  }

  struct dirent64 *readdir64(DIR *dirp)
  {
    struct dirent *aux;
    struct dirent64 *ret = NULL;

    debug_info("Before readdir64...\n");

    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    //memcpy(&fd, dirp,sizeof(int));

    if( fdsdirtable_search( dirp ) != -1 )
    {
      aux=xpn_readdir(dirp);

      if (aux != NULL){
        ret = (struct dirent64 *)malloc(sizeof(struct dirent64));
        ret->d_ino = (__ino64_t)aux->d_ino;
        ret->d_off = (__off64_t)aux->d_off;
        ret->d_reclen = aux->d_reclen;
        ret->d_type = aux->d_type;
        //ret->d_name = aux->d_name;
      }

      debug_info("After xpn_readdir()...\n");

      return ret;
    }
    // Not an XPN partition. We must link with the standard library
    else
    {
      return dlsym_readdir64(dirp);
    } 
  }

  int closedir(DIR *dirp)
  {
    int ret;

    debug_info("Before closedir...\n");

    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    if( fdsdirtable_search( dirp ) != -1 )
    {

      ret=xpn_closedir( dirp );

      fdsdirtable_remove( dirp );

      /*for (int i = 0; i < MAX_DIRS; ++i)
      {
        debug_info("%p\n", fdsdirtable[i]);
      }*/

      debug_info("closedir return %d\n",ret);
      return ret;
    }
    // Not an XPN partition. We must link with the standard library
    else
    {
      return dlsym_closedir(dirp);
    }
  }

  int rmdir(const char *path)
  {
    debug_info("Before rmdir...\n");
    debug_info("PATH %s\n", path);

    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    if(!strncmp(xpn_adaptor_partition_prefix,path,strlen(xpn_adaptor_partition_prefix)))
    {
      return xpn_rmdir( ((char *)(path+strlen(xpn_adaptor_partition_prefix))) );
    }
    // Not an XPN partition. We must link with the standard library
    else
    {
      return dlsym_rmdir((char *)path);
    }
  }



  // Proccess API

  int fork()
  {
    debug_info("Before fork()\n");
    int ret = dlsym_fork();
    if(0 == ret){
      // We want the children to be initialized
      xpn_adaptor_initCalled = 0;
    }
    return ret;
  }

  int pipe(int pipefd[2])
  {
    debug_info("Before pipe()\n");
    int ret = dlsym_pipe(pipefd);
    
    if(ret > 0)
    {
      struct generic_fd virtual_fd;
      struct generic_fd virtual_fd2;

      virtual_fd.type    = FD_SYS;
      virtual_fd.real_fd = pipefd[0];

      virtual_fd2.type    = FD_SYS;
      virtual_fd2.real_fd = pipefd[1];

      debug_info("PIPE FD1 SYS %d\n", pipefd[0]);
      debug_info("PIPE FD2 SYS %d\n", pipefd[1]);

      pipefd[0] = fdstable_put ( virtual_fd );
      pipefd[1] = fdstable_put ( virtual_fd );
    }

    return ret;
  }

  /*
  int execve(const char *filename, char *const *argv, char *const *envp)

  {
      int ret, num_filas, i;
      char **aux;
      char *envpAux[2];
  #ifdef DEBUG_BYPASS_EXECVE
      debug_info("antes de execve...\n");
  #endif
      if (NULL != envp){
          
          num_filas = 0;
          while (envp[num_filas] != NULL){
  #ifdef DEBUG_BYPASS_EXECVE
              debug_info("%s\n",envp[num_filas]);
  #endif
              num_filas++;
          }
          aux = (char**)malloc((num_filas+1)*sizeof(char*));  
          aux[0] = preload;
      }
      if (NULL != envp){
          i = 0;
              while (num_filas>i){
              aux[i+1] = envp[i];
                  i++;
          }
      } else {
          aux = envpAux;
          envpAux[0] = preload;
          envpAux[1] = NULL;
      }
      ret = execve(filename,argv,aux);
      return(ret);
  }
  */

  int dup(int fd)
  {
    debug_info("Before dup...\n");

    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    struct generic_fd virtual_fd = fdstable_get ( fd );

    debug_info("DUP %d %d\n", fd, virtual_fd.real_fd);

    if(virtual_fd.type == FD_XPN)
    {
      return xpn_dup(virtual_fd.real_fd);
    }
    // Not an XPN partition. We must link with the standard library
    else if (virtual_fd.type == FD_SYS)
    {
      return dlsym_dup(virtual_fd.real_fd);
    }
    else{
      return dlsym_dup(fd);
    }

    return -1;
  }

  int dup2(int fd, int fd2)
  {
    debug_info("Before dup2...\n");

    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    struct generic_fd virtual_fd  = fdstable_get ( fd );
    struct generic_fd virtual_fd2 = fdstable_get ( fd2 );

    debug_info("DUP2 %d %d\n", fd, virtual_fd.real_fd);
    debug_info("DUP2 %d %d\n", fd2, virtual_fd2.real_fd);

    if(virtual_fd.type == FD_XPN)
    {
      return xpn_dup2(virtual_fd.real_fd, virtual_fd2.real_fd);
    }
    // Not an XPN partition. We must link with the standard library
    else if (virtual_fd.type == FD_SYS)
    {
      return dlsym_dup2(virtual_fd.real_fd, virtual_fd2.real_fd);
    }
    else{
      return dlsym_dup2(fd, fd2);
    }

    return -1;
  }

  void exit(int status)
  {
    if (xpn_adaptor_initCalled == 1)
    {
      xpn_destroy();
    }
    
    dlsym_exit(status);
  }



  // Manager API

  int chdir(const char *path)
  {
    debug_info("antes de chdir....\n");

    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    if(!strncmp(xpn_adaptor_partition_prefix,path,strlen(xpn_adaptor_partition_prefix)))
    {
      return(xpn_chdir((char *)(path+strlen(xpn_adaptor_partition_prefix))));
    }
    // Not an XPN partition. We must link with the standard library
    else 
    {
      return dlsym_chdir((char *)path);
    }
  }

  int chmod(const char *path, mode_t mode)
  {
    debug_info("Before chmod...\n");

    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    if(!strncmp(xpn_adaptor_partition_prefix,path,strlen(xpn_adaptor_partition_prefix)))
    {
      return(xpn_chmod((char *)(path+strlen(xpn_adaptor_partition_prefix)), mode));
    }
    // Not an XPN partition. We must link with the standard library
    else
    {
      return dlsym_chmod((char *)path, mode);
    }
  }

  int fchmod(int fd, mode_t mode)
  {
    debug_info("Before fchmod...\n");

    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    struct generic_fd virtual_fd = fdstable_get ( fd );

    if(virtual_fd.type == FD_XPN)
    {
      return xpn_fchmod(fd,mode);
    }
    // Not an XPN partition. We must link with the standard library
    else if (virtual_fd.type == FD_SYS)
    {
      return dlsym_fchmod(virtual_fd.real_fd,mode);
    }
    else{
      return dlsym_fchmod(fd,mode);
    }

    return -1;
  }

  int chown(const char *path, uid_t owner, gid_t group)
  {
    debug_info("Before chown...\n");

    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    if(!strncmp(xpn_adaptor_partition_prefix,path,strlen(xpn_adaptor_partition_prefix)))
    {
      return(xpn_chown((char *)(path+strlen(xpn_adaptor_partition_prefix)), owner, group));
    }
    // Not an XPN partition. We must link with the standard library
    else
    {
      return dlsym_chown((char *)path, owner, group);
    }
  }

  int fcntl(int fd, int cmd, long arg) //TODO
  {
    struct generic_fd virtual_fd = fdstable_get ( fd );

    if(virtual_fd.type == FD_XPN)
    {
      //TODO
      return 0;
    } 
    else if (virtual_fd.type == FD_SYS)
    {
      return dlsym_fcntl(virtual_fd.real_fd, cmd, arg);
    }
    else{
      return dlsym_fcntl(fd, cmd, arg);
    }

    return -1;
  }

  int access(const char *path, int mode){
    debug_info("Before access...\n");

    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    if(!strncmp(xpn_adaptor_partition_prefix,path,strlen(xpn_adaptor_partition_prefix)))
    {
      struct stat64 stats;
      if (__lxstat64(_STAT_VER, path, &stats)){
        return -1;
      }

      if (mode == F_OK){
        return 0;     /* The file exists. */
      }

      if ((mode & X_OK) == 0 || (stats.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)))
      {
        return 0;
      }

      return -1;
    }
    // Not an XPN partition. We must link with the standard library
    else
    {
      return dlsym_access(path, mode);
    }
  }

  /**************************************************
  GETCWD TIENE MUCHA CHICHA...PA LUEGO
  ***************************************************
  char *getcwd(char *path, size_t size)
  {

  #ifdef DEBUG_BYPASS_GETCWD
      debug_info("antes de getcwd...\n");
  #endif

      // We must initialize expand if it has not been initialized yet.
      xpn_adaptor_keepInit ();

      if(!strncmp(xpn_adaptor_partition_prefix,path,strlen(xpn_adaptor_partition_prefix)))
      {
          // If xpn
          return(xpn_chdir(path+strlen(xpn_adaptor_partition_prefix)));
      }
      else // Not an XPN partition. We must link with the standard library
      {
          return(chdir(path));
      }
  }
  *******************************************************/

  /*
  int utime(char *path, struct utimbuf *times)
  {

  #ifdef DEBUG_BYPASS_UTIME
      debug_info("antes de utime...\n");
  #endif

      // We must initialize expand if it has not been initialized yet.
      xpn_adaptor_keepInit ();

      if(!strncmp(xpn_adaptor_partition_prefix,path,strlen(xpn_adaptor_partition_prefix)))
      {
          return(xpn_utime(path+strlen(xpn_adaptor_partition_prefix), times));
      }// If xpn
      else // Not an XPN partition. We must link with the standard library
      {
          return(utime(path, times));
      } // Else
  }
  */



  // MPI API

  int MPI_Init (int *argc, char ***argv)
  {
    char *value;

    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();
    value = getenv("IS_MPI_SERVER") ;
    if (NULL == value){
      return PMPI_Init(argc, argv);
    }
    return MPI_SUCCESS;
  }

  int MPI_Finalize (void)
  {
    char *value;

    value = getenv("IS_MPI_SERVER") ;
    if (NULL != value && xpn_adaptor_initCalled == 1){
      xpn_destroy();
    }

    return PMPI_Finalize();
  }
