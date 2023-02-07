
  /*
   *  Copyright 2000-2023 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
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
  //char *xpn_adaptor_partition_prefix = "xpn://"; //Original
  char *xpn_adaptor_partition_prefix = "/tmp/expand/";

  int is_prefix(const char * prefix, const char * path){
    return ( !strncmp(prefix,path,strlen(prefix)) && strlen(path) > strlen(prefix) );
  }



  // fd table management

  struct generic_fd * fdstable = NULL;
  long fdstable_size = 0L;
  long fdstable_first_free = 0L;

  //pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

  void fdstable_realloc ( void )
  {
    long old_size = fdstable_size;
    struct generic_fd * fdstable_aux = fdstable;

    if ( NULL == fdstable )
    {
      fdstable_size = (long)MAX_FDS;
      fdstable = (struct generic_fd *) malloc(fdstable_size * sizeof(struct generic_fd));
    }
    else
    {
      fdstable_size = fdstable_size * 2;
      fdstable = (struct generic_fd *) realloc((struct generic_fd *)fdstable, fdstable_size * sizeof(struct generic_fd));
    }

    if ( NULL == fdstable )
    {
      printf("[bypass:%s:%d] Error: out of memory\n", __FILE__, __LINE__);
      if (fdstable_aux != NULL){
	      free(fdstable_aux) ;
      }
      exit(-1);
    }
    
    //pthread_mutex_lock(&mutex);
    for (int i = old_size; i < fdstable_size; ++i)
    {
      fdstable[i].type = FD_FREE;
      fdstable[i].real_fd = -1;
    }
    //pthread_mutex_unlock(&mutex);
  }

  void fdstable_init ( void )
  {
    fdstable_realloc();
  }

  struct generic_fd fdstable_get ( int fd )
  {
    //debug_info("[bypass] GET FSTABLE %d  %d  %d\n", fd, fdstable[fd].type, fdstable[fd].real_fd);
    //pthread_mutex_lock(&mutex);

    struct generic_fd ret;

    if (fd >= PLUSXPN)
    {
      fd = fd - PLUSXPN;
      ret = fdstable[fd];
    }
    else
    {
      ret.type = FD_SYS;
      ret.real_fd = fd;
    }

    //pthread_mutex_unlock(&mutex);

    return ret;
  }

  int fdstable_put ( struct generic_fd fd )
  {
    for (int i = fdstable_first_free; i < fdstable_size; ++i)
    {
      //pthread_mutex_lock(&mutex);
      if ( fdstable[i].type == FD_FREE ){
        fdstable[i] = fd;
        fdstable_first_free = (long)(i + 1);
        //debug_info("[bypass] PUT FSTABLE %d  %d  %d\n", i, fdstable[i].type, fdstable[i].real_fd);
        //pthread_mutex_unlock(&mutex);
        return i + PLUSXPN;
      }
      //pthread_mutex_unlock(&mutex);
    }

    long old_size = fdstable_size;

    fdstable_realloc();

    //pthread_mutex_lock(&mutex);
    if ( fdstable[old_size].type == FD_FREE ){
      fdstable[old_size] = fd;
      //debug_info("[bypass] PUT FSTABLE %d  %d  %d\n", i, fdstable[i].type, fdstable[i].real_fd);
      //pthread_mutex_unlock(&mutex);
      return old_size + PLUSXPN;
    }
    //pthread_mutex_unlock(&mutex);

    return -1;
  }

  int fdstable_remove ( int fd )
  {
    //pthread_mutex_lock(&mutex);
    if (fd < PLUSXPN)
    {
      return 0;
    }
    fd = fd - PLUSXPN;
    fdstable[fd].type = FD_FREE;
    fdstable[fd].real_fd = -1;

    if ( fd < fdstable_first_free )
    {
      fdstable_first_free = fd;
    }
    //pthread_mutex_unlock(&mutex);

    return 0;
  }



  // Dir table management

  DIR ** fdsdirtable = NULL;
  long fdsdirtable_size = 0L;
  long fdsdirtable_first_free = 0L;

  void fdsdirtable_realloc ( void )
  {
    long          old_size = fdsdirtable_size;
    DIR ** fdsdirtable_aux = fdsdirtable;
    
    if ( NULL == fdsdirtable ){
      fdsdirtable_size = (long)MAX_DIRS;
      fdsdirtable = (DIR **) malloc(MAX_DIRS * sizeof(DIR *));
    }
    else{
      fdsdirtable_size = fdsdirtable_size * 2;
      fdsdirtable = (DIR **) realloc((DIR **)fdsdirtable, fdsdirtable_size * sizeof(DIR *));
    }

    if ( NULL == fdsdirtable )
    {
      printf("[bypass:%s:%d] Error: out of memory\n", __FILE__, __LINE__);
      if (NULL != fdsdirtable_aux){
	      free(fdsdirtable_aux) ;
      }
      exit(-1);
    }
    
    //pthread_mutex_lock(&mutex);
    for (int i = old_size; i < fdsdirtable_size; ++i) {
      fdsdirtable[i] = NULL;
    }
    //pthread_mutex_unlock(&mutex);
  }

  void fdsdirtable_init ( void )
  {
    fdsdirtable_realloc();
  }

  int fdsdirtable_get ( DIR * dir )
  {
    for (int i = 0; i < fdsdirtable_size; ++i)
    {
      if ( fdsdirtable[i] == dir ){
        return i;
      }
    }

    return -1;
  }

  int fdsdirtable_put ( DIR * dir )
  {
    for (int i = fdsdirtable_first_free; i < fdsdirtable_size; ++i)
    {
      if ( fdsdirtable[i] == NULL ){
        fdsdirtable[i] = dir;
        fdsdirtable_first_free = (long)(i + 1);
        return 0;
      }
    }

    long old_size = fdstable_size;

    fdsdirtable_realloc();

    if ( fdsdirtable[old_size] == NULL ){
      fdsdirtable[old_size] = dir;
      return 0;
    }

    return -1;
  }

  int fdsdirtable_remove ( DIR * dir )
  {
    for (int i = 0; i < fdsdirtable_size; ++i)
    {
      if ( fdsdirtable[i] == dir ){
        fdsdirtable[i] = NULL;

        if ( i < fdsdirtable_first_free )
        {
          fdsdirtable_first_free = i;
        }

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

    debug_info("[bypass] Before xpn_adaptor_keepInit\n");

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
      debug_info("[bypass] Before xpn_init()\n");

      xpn_adaptor_initCalled = 1; //TODO: Delete
      setenv("INITCALLED", "1", 1);

      fdstable_init ();
      fdsdirtable_init ();

      ret = xpn_init();

      debug_info("[bypass] After xpn_init()\n");

      if (ret < 0)
      {
        printf("xpn_init: Expand couldn't be initialized\n");
        xpn_adaptor_initCalled = 0;
        setenv("INITCALLED", "0", 1);
      }
      else
      {
        xpn_adaptor_initCalled = 1;
        setenv("INITCALLED", "1", 1);
      }
    }
    debug_info("[bypass] End xpn_adaptor_keepInit\n");
  }



  // File API

  int open(const char *path, int flags, ...)
  {
    int ret, fd;
    va_list ap;
    mode_t mode = 0;

    va_start(ap, flags);

    mode = va_arg(ap, mode_t);

    debug_info("[bypass] Before open.... %s\n",path);
    debug_info("[bypass] 1) Path => %s\n",path);
    debug_info("[bypass] 2) flags => %d\n",flags);
    debug_info("[bypass] 3) mode => %d\n",mode);

    // This if checks if variable path passed as argument starts with the expand prefix.
    if(is_prefix(xpn_adaptor_partition_prefix, path))
    {
      // We must initialize expand if it has not been initialized yet.
      xpn_adaptor_keepInit ();

      // It is an XPN partition, so we redirect the syscall to expand syscall
      debug_info("[bypass] xpn_open\n");
      debug_info("[bypass] Path => %s\n",path + strlen(xpn_adaptor_partition_prefix));

      if (mode != 0){
        fd=xpn_open((const char *)(path+strlen(xpn_adaptor_partition_prefix)),flags, mode);
      }
      else{
        fd=xpn_open((const char *)(path+strlen(xpn_adaptor_partition_prefix)),flags);
      }

      debug_info("[bypass] xpn.bypass: xpn_open(%s,%o) return %d\n",path+strlen(xpn_adaptor_partition_prefix),flags,fd);

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
      debug_info("[bypass] dlsym_open\n");

      ret = dlsym_open2((char *)path, flags, mode);
    }
    va_end(ap);

    return ret;
  }

  int open64(const char *path, int flags, ...)
  {
    int fd, ret;
    va_list ap;
    mode_t mode = 0;

    va_start(ap, flags);

    mode = va_arg(ap, mode_t);

    debug_info("[bypass] Before open64.... %s\n",path);
    debug_info("[bypass] 1) Path => %s\n",path);
    debug_info("[bypass] 2) flags => %d\n",flags);
    debug_info("[bypass] 3) mode => %d\n",mode);

    if(is_prefix(xpn_adaptor_partition_prefix, path))
    {
      // We must initialize expand if it has not been initialized yet.
      xpn_adaptor_keepInit ();

      debug_info("[bypass] xpn_open\n");
      debug_info("[bypass] Path => %s\n",path+strlen(xpn_adaptor_partition_prefix));

      if (mode != 0){
        fd=xpn_open((const char *)(path+strlen(xpn_adaptor_partition_prefix)),flags, mode);
      }
      else{
        fd=xpn_open((const char *)(path+strlen(xpn_adaptor_partition_prefix)),flags);
      }

      debug_info("[bypass] xpn.bypass: xpn_open(%s,%o) return %d\n",path+strlen(xpn_adaptor_partition_prefix),flags,fd);

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
      debug_info("[bypass] dlsym_open64\n");

      ret = dlsym_open64((char *)path, flags, mode);
    }

    va_end(ap);

    return ret;
  }

  /*int __open_2(const char *path, int flags, ...)
  {
    int fd, ret;
    va_list ap;
    mode_t mode = 0;

    va_start(ap, flags);

    mode = va_arg(ap, mode_t);

    debug_info("[bypass] Before __open_2.... %s\n",path);
    debug_info("[bypass] 1) Path => %s\n",path);
    debug_info("[bypass] 2) flags => %d\n",flags);
    debug_info("[bypass] 3) mode => %d\n",mode);

    if(is_prefix(xpn_adaptor_partition_prefix, path))
    {
      // We must initialize expand if it has not been initialized yet.
      xpn_adaptor_keepInit ();

      debug_info("[bypass] xpn_open\n");
      debug_info("[bypass] Path => %s\n",path+strlen(xpn_adaptor_partition_prefix));

      if (mode != 0){
        fd=xpn_open((const char *)(path+strlen(xpn_adaptor_partition_prefix)),flags, mode);
      }
      else{
        fd=xpn_open((const char *)(path+strlen(xpn_adaptor_partition_prefix)),flags);
      }

      debug_info("[bypass] xpn.bypass: xpn_open(%s,%o) return %d\n",path+strlen(xpn_adaptor_partition_prefix),flags,fd);

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
      debug_info("[bypass] dlsym___open_2\n");

      ret = dlsym___open_2((char *)path, flags);
    }

    va_end(ap);

    return ret;
  }*/


  int creat(const char *path, mode_t mode)
  {
    int fd,ret;

    debug_info("[bypass] Before creat....\n");

    if(is_prefix(xpn_adaptor_partition_prefix, path))
    {
      // We must initialize expand if it has not been initialized yet.
      xpn_adaptor_keepInit ();

      debug_info("[bypass] xpn_creat\n");

      fd = xpn_creat((const char *)(path+strlen(xpn_adaptor_partition_prefix)),mode);

      debug_info("[bypass] The file is %s",(const char *)(path+strlen(xpn_adaptor_partition_prefix)));

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
      debug_info("[bypass] dlsym_creat\n");

      ret = dlsym_creat(path,mode);
    }

    return ret;
  }

  int ftruncate(int fd, off_t length)
  {
    debug_info("[bypass] Before ftruncate...\n");

    int ret = -1;

    struct generic_fd virtual_fd = fdstable_get ( fd );

    if(virtual_fd.type == FD_XPN)
    {
      // We must initialize expand if it has not been initialized yet.
      xpn_adaptor_keepInit ();

      debug_info("[bypass] xpn_ftruncate\n");
      ret = xpn_ftruncate(virtual_fd.real_fd, length);
    }
    // Not an XPN partition. We must link with the standard library
    else{
      debug_info("[bypass] dlsym_ftruncate\n");
      ret = dlsym_ftruncate(fd, length);
    }

    return ret;
  }

  ssize_t read(int fd, void *buf, size_t nbyte)
  {         
    debug_info("[bypass] Before read...\n");
    debug_info("[bypass] read(fd=%d,*buf,nbyte=%ld)\n",fd,nbyte);
    debug_info("[bypass] buffer= %s\n",buf);

    int ret = -1;

    struct generic_fd virtual_fd = fdstable_get ( fd );

    if(virtual_fd.type == FD_XPN)
    {
      // We must initialize expand if it has not been initialized yet.
      xpn_adaptor_keepInit ();

      debug_info("[bypass] xpn_read\n");
      ret = xpn_read(virtual_fd.real_fd, buf, nbyte);
    }
    // Not an XPN partition. We must link with the standard library
    else{
      debug_info("[bypass] dlsym_read\n");
      ret = dlsym_read(fd, buf, nbyte);
    }

    return ret;
  }

  ssize_t write(int fd, const void *buf, size_t nbyte)
  {
    debug_info("[bypass] Before write...\n");
    debug_info("[bypass] write(fd=%d,*buf=%s,nbyte=%ld)\n", fd, buf, nbyte);

    int ret = -1;

    struct generic_fd virtual_fd = fdstable_get ( fd );

    if(virtual_fd.type == FD_XPN)
    {
      // We must initialize expand if it has not been initialized yet.
      xpn_adaptor_keepInit ();

      debug_info("[bypass] xpn_write\n");
      ret = xpn_write(virtual_fd.real_fd, (void *)buf, nbyte);
    }
    // Not an XPN partition. We must link with the standard library
    else{
      debug_info("[bypass] dlsym_write\n");
      ret = dlsym_write(fd, (void *)buf, nbyte);
    }

    return ret;
  }

  off_t lseek(int fd, off_t offset, int whence)
  {
    debug_info("[bypass] Before lseek...\n");

    int ret = -1;

    struct generic_fd virtual_fd = fdstable_get ( fd );

    if(virtual_fd.type == FD_XPN)
    {
      // We must initialize expand if it has not been initialized yet.
      xpn_adaptor_keepInit ();

      debug_info("[bypass] xpn_lseek\n");
      ret = xpn_lseek(virtual_fd.real_fd, offset, whence);
    }
    // Not an XPN partition. We must link with the standard library
    else{
      debug_info("[bypass] dlsym_lseek\n");
      ret = dlsym_lseek(fd, offset, whence);
    }

    return ret;
  }

  int __lxstat64(int ver, const char *path, struct stat64 *buf)
  {
    int ret;
    struct stat st;

    debug_info("[bypass] Before lstat64... %s\n",path);
    debug_info("[bypass] lstat64...path = %s\n",path+strlen(xpn_adaptor_partition_prefix));

    if(is_prefix(xpn_adaptor_partition_prefix, path))
    {
      // We must initialize expand if it has not been initialized yet.
      xpn_adaptor_keepInit ();

      debug_info("[bypass] xpn_stat\n");

      ret = xpn_stat((const char *)(path+strlen(xpn_adaptor_partition_prefix)), &st);

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
      debug_info("[bypass] dlsym_lxstat64\n");
      return dlsym_lxstat64(ver,(const char *)path, buf);
    }
  }

  int __xstat64(int ver, const char *path, struct stat64 *buf)
  {
    int ret;
    struct stat st;

    debug_info("[bypass] Before stat64... %s\n",path);
    debug_info("[bypass] stat64...path = %s\n",path);

    if(is_prefix(xpn_adaptor_partition_prefix, path))
    {
      // We must initialize expand if it has not been initialized yet.
      xpn_adaptor_keepInit ();

      debug_info("[bypass] xpn_stat\n");

      ret = xpn_stat((const char *)(path+strlen(xpn_adaptor_partition_prefix)), &st);

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
      debug_info("[bypass] dlsym_xstat64\n");
      return dlsym_xstat64(ver,(const char *)path, buf);
    }    
  }

  int __fxstat64(int ver, int fd, struct stat64 *buf)
  {
    int ret;
    struct stat st;

    debug_info("[bypass]  Before fstat64... %d\n",fd);

    struct generic_fd virtual_fd = fdstable_get ( fd );

    if(virtual_fd.type == FD_XPN)
    {
      // We must initialize expand if it has not been initialized yet.
      xpn_adaptor_keepInit ();

      debug_info("[bypass] xpn_fstat\n");

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
    else{
      debug_info("[bypass] dlsym_fxstat64\n");
      return dlsym_fxstat64(ver,fd, buf);
    }

    return ret;
  }

  int __lxstat(int ver, const char *path, struct stat *buf)
  {
    int ret;

    debug_info("[bypass] Before lstat... %s\n",path);

    if(is_prefix(xpn_adaptor_partition_prefix, path))
    {
      // We must initialize expand if it has not been initialized yet.
      xpn_adaptor_keepInit ();

      debug_info("[bypass] xpn_stat\n");
      debug_info("[bypass] XPN:lstat:path = %s\n",path+strlen(xpn_adaptor_partition_prefix));

      ret = xpn_stat((const char *)(path+strlen(xpn_adaptor_partition_prefix)), buf);
    }
    // Not an XPN partition. We must link with the standard library
    else
    {
      debug_info("[bypass] dlsym_lstat\n");
      ret = dlsym_lstat(ver,(const char *)path, buf);
    }

    return ret;
  }

  int __xstat(int ver, const char *path, struct stat *buf) // TODO
  {
    //char path2[1024];

    debug_info("[bypass] Before stat...\n");
    debug_info("[bypass] stat...path =>%s\n",path);

    if(is_prefix(xpn_adaptor_partition_prefix, path))
    {
      // We must initialize expand if it has not been initialized yet.
      xpn_adaptor_keepInit ();

      debug_info("[bypass] xpn_stat\n");

      return xpn_stat((const char *)(path+strlen(xpn_adaptor_partition_prefix)), buf);
      
    }
    // Not an XPN partition. We must link with the standard library
    else
    {
      debug_info("[bypass] dlsym_stat\n");
      return dlsym_stat(ver,(const char *)path, buf);
    } 
  }

  int __fxstat(int ver, int fd, struct stat *buf)
  {
    debug_info("[bypass] Before fstat...\n");

    int ret = -1;

    struct generic_fd virtual_fd = fdstable_get ( fd );

    if(virtual_fd.type == FD_XPN)
    {
      // We must initialize expand if it has not been initialized yet.
      xpn_adaptor_keepInit ();

      debug_info("[bypass] xpn_fstat\n");
      ret = xpn_fstat(virtual_fd.real_fd,buf);
    }
    // Not an XPN partition. We must link with the standard library
    else{
      debug_info("[bypass] dlsym_fstat\n");
      ret = dlsym_fstat(ver,fd,buf);
    }
    debug_info("[bypass] fstat res: %d\n", ret);
    return ret;
  }

  int close(int fd)
  {
    debug_info("[bypass] Before close....\n");
    debug_info("[bypass] FD = %d\n", fd);

    int ret = -1;

    struct generic_fd virtual_fd = fdstable_get ( fd );

    if(virtual_fd.type == FD_XPN)
    {
      // We must initialize expand if it has not been initialized yet.
      xpn_adaptor_keepInit ();

      debug_info("[bypass] xpn_close\n");

      ret = xpn_close(virtual_fd.real_fd);
      fdstable_remove ( fd );
    }
    // Not an XPN partition. We must link with the standard library
    else{
      debug_info("[bypass] dlsym_close\n");
      ret = dlsym_close(fd);
    }

    return ret;
  }

  int rename(const char *old_path, const char *new_path)
  {
    debug_info("[bypass] Before rename....\n");

    if(is_prefix(xpn_adaptor_partition_prefix, old_path) && is_prefix(xpn_adaptor_partition_prefix, new_path))
    {
      // We must initialize expand if it has not been initialized yet.
      xpn_adaptor_keepInit ();

      debug_info("[bypass] xpn_rename\n");
      debug_info("[bypass] Old Path => %s\n",old_path+strlen(xpn_adaptor_partition_prefix));
      debug_info("[bypass] New Path => %s\n",new_path+strlen(xpn_adaptor_partition_prefix));

      return xpn_rename((const char *)(old_path+strlen(xpn_adaptor_partition_prefix)), (const char *)(new_path+strlen(xpn_adaptor_partition_prefix)));
    }
    // Not an XPN partition. We must link with the standard library
    else 
    {
      debug_info("[bypass] dlsym_rename\n");
      return dlsym_rename(old_path, new_path);
    }
  }

  int unlink(const char *path)
  {
    debug_info("[bypass] Before unlink...\n");
    debug_info("[bypass] PATH %s\n", path);

    if(is_prefix(xpn_adaptor_partition_prefix, path))
    {
      // We must initialize expand if it has not been initialized yet.
      xpn_adaptor_keepInit ();

      debug_info("[bypass] xpn_unlink\n");
      return(xpn_unlink((const char *)(path+strlen(xpn_adaptor_partition_prefix))));
    }
    // Not an XPN partition. We must link with the standard library
    else
    {
      debug_info("[bypass] dlsym_unlink\n");
      return dlsym_unlink((char *)path);
    }
  }




  // Directory API

  int mkdir(const char *path, mode_t mode)
  {
    debug_info("[bypass] Before mkdir...\n");
    debug_info("[bypass] PATH %s\n", path);

    if(is_prefix(xpn_adaptor_partition_prefix, path))
    {
      // We must initialize expand if it has not been initialized yet.
      xpn_adaptor_keepInit ();

      debug_info("[bypass] xpn_mkdir\n");
      debug_info("[bypass] Before xpn_mkdir(%s)...\n",((const char *)(path+strlen(xpn_adaptor_partition_prefix))));

      return xpn_mkdir( ((const char *)(path+strlen(xpn_adaptor_partition_prefix))) ,mode );
    }
    // Not an XPN partition. We must link with the standard library
    else
    {
      debug_info("[bypass] dlsym_mkdir\n");
      return dlsym_mkdir((char *)path,mode);
    }
  }

  DIR *opendir(const char *dirname)
  {
    debug_info("[bypass] Before opendir(%s)...\n", dirname);

    DIR * ret;

    if(is_prefix(xpn_adaptor_partition_prefix, dirname))
    {
      // We must initialize expand if it has not been initialized yet.
      xpn_adaptor_keepInit ();

      debug_info("[bypass] xpn_opendir\n");

      ret = xpn_opendir((const char *)(dirname+strlen(xpn_adaptor_partition_prefix)));
      if ( ret != NULL ){
        fdsdirtable_put ( ret );
      }

      return ret;
    }
    // Not an XPN partition. We must link with the standard library
    else
    {
      debug_info("[bypass] dlsym_opendir\n");
      return dlsym_opendir((char *)dirname);
    }
  }

  struct dirent *readdir(DIR *dirp)
  {
    struct dirent *ret;

    debug_info("[bypass] Before readdir...\n");

    if( fdsdirtable_get( dirp ) != -1 )
    {
      // We must initialize expand if it has not been initialized yet.
      xpn_adaptor_keepInit ();

      debug_info("[bypass] xpn_readdir\n");

      ret = xpn_readdir(dirp);

      debug_info("[bypass] After xpn_readdir()...\n");

      return ret;
    }
    // Not an XPN partition. We must link with the standard library
    else
    {
      debug_info("[bypass] dlsym_readdir\n");
      return dlsym_readdir(dirp);
    }
  }

  struct dirent64 *readdir64(DIR *dirp)
  {
    struct dirent *aux;
    struct dirent64 *ret = NULL;

    debug_info("[bypass] Before readdir64...\n");

    //memcpy(&fd, dirp,sizeof(int));

    if( fdsdirtable_get( dirp ) != -1 )
    {
      // We must initialize expand if it has not been initialized yet.
      xpn_adaptor_keepInit ();

      debug_info("[bypass] xpn_readdir\n");

      aux=xpn_readdir(dirp);

      if (aux != NULL){
        ret = (struct dirent64 *)malloc(sizeof(struct dirent64));
        ret->d_ino = (__ino64_t)aux->d_ino;
        ret->d_off = (__off64_t)aux->d_off;
        ret->d_reclen = aux->d_reclen;
        ret->d_type = aux->d_type;
        //ret->d_name = aux->d_name;
      }

      debug_info("[bypass] After xpn_readdir()...\n");

      return ret;
    }
    // Not an XPN partition. We must link with the standard library
    else
    {
      debug_info("[bypass] dlsym_readdir64\n");
      return dlsym_readdir64(dirp);
    } 
  }

  int closedir(DIR *dirp)
  {
    int ret;

    debug_info("[bypass] Before closedir...\n");

    if( fdsdirtable_get( dirp ) != -1 )
    {
      // We must initialize expand if it has not been initialized yet.
      xpn_adaptor_keepInit ();

      debug_info("[bypass] xpn_closedir\n");

      ret = xpn_closedir( dirp );

      fdsdirtable_remove( dirp );

      debug_info("[bypass] closedir return %d\n",ret);
      return ret;
    }
    // Not an XPN partition. We must link with the standard library
    else
    {
      debug_info("[bypass] dlsym_closedir\n");
      return dlsym_closedir(dirp);
    }
  }

  int rmdir(const char *path)
  {
    debug_info("[bypass] Before rmdir...\n");
    debug_info("[bypass] PATH %s\n", path);

    if(is_prefix(xpn_adaptor_partition_prefix, path))
    {
      // We must initialize expand if it has not been initialized yet.
      xpn_adaptor_keepInit ();

      debug_info("[bypass] xpn_rmdir\n");
      return xpn_rmdir( ((const char *)(path+strlen(xpn_adaptor_partition_prefix))) );
    }
    // Not an XPN partition. We must link with the standard library
    else
    {
      debug_info("[bypass] dlsym_rmdir\n");
      return dlsym_rmdir((char *)path);
    }
  }



  // Proccess API

  int fork()
  {
    debug_info("[bypass] Before fork()\n");
    int ret = dlsym_fork();
    if(0 == ret){
      // We want the children to be initialized
      xpn_adaptor_initCalled = 0;
    }
    return ret;
  }

  int pipe(int pipefd[2])
  {
    debug_info("[bypass] Before pipe()\n");
    int ret = dlsym_pipe(pipefd);

    return ret;
  }


  int dup(int fd)
  {
    debug_info("[bypass] Before dup...\n");

    int ret = -1;

    struct generic_fd virtual_fd = fdstable_get ( fd );

    debug_info("[bypass] DUP %d %d\n", fd, virtual_fd.real_fd);

    if(virtual_fd.type == FD_XPN)
    {
      // We must initialize expand if it has not been initialized yet.
      xpn_adaptor_keepInit ();

      debug_info("[bypass] xpn_dup\n");
      ret = xpn_dup(virtual_fd.real_fd);
    }
    // Not an XPN partition. We must link with the standard library
    else{
      debug_info("[bypass] dlsym_dup\n");
      ret = dlsym_dup(fd);
    }

    return ret;
  }

  int dup2(int fd, int fd2)
  {
    debug_info("[bypass] Before dup2...\n");

    int ret = -1;

    struct generic_fd virtual_fd  = fdstable_get ( fd );
    struct generic_fd virtual_fd2 = fdstable_get ( fd2 );

    debug_info("[bypass] DUP2 %d %d\n", fd, virtual_fd.real_fd);
    debug_info("[bypass] DUP2 %d %d\n", fd2, virtual_fd2.real_fd);

    if(virtual_fd.type == FD_XPN)
    {
      // We must initialize expand if it has not been initialized yet.
      xpn_adaptor_keepInit ();

      debug_info("[bypass] xpn_dup2\n");
      ret = xpn_dup2(virtual_fd.real_fd, virtual_fd2.real_fd);
    }
    // Not an XPN partition. We must link with the standard library
    else{
      debug_info("[bypass] dlsym_dup2\n");
      ret = dlsym_dup2(fd, fd2);
    }

    return ret;
  }

  void exit ( int status )
  {
    debug_info("[bypass] Before exit...\n");

    if (xpn_adaptor_initCalled == 1)
    {
      xpn_destroy();
    }

    dlsym_exit(status) ;
    __builtin_unreachable() ;
  }



  // Manager API

  int chdir(const char *path)
  {
    debug_info("[bypass] antes de chdir....\n");

    if(is_prefix(xpn_adaptor_partition_prefix, path))
    {
      // We must initialize expand if it has not been initialized yet.
      xpn_adaptor_keepInit ();

      debug_info("[bypass] xpn_chdir\n");
      return(xpn_chdir((char *)(path+strlen(xpn_adaptor_partition_prefix))));
    }
    // Not an XPN partition. We must link with the standard library
    else 
    {
      debug_info("[bypass] dlsym_chdir\n");
      return dlsym_chdir((char *)path);
    }
  }

  int chmod(const char *path, mode_t mode)
  {
    debug_info("[bypass] Before chmod...\n");

    if(is_prefix(xpn_adaptor_partition_prefix, path))
    {
      // We must initialize expand if it has not been initialized yet.
      xpn_adaptor_keepInit ();

      debug_info("[bypass] xpn_chmod\n");
      return(xpn_chmod((const char *)(path+strlen(xpn_adaptor_partition_prefix)), mode));
    }
    // Not an XPN partition. We must link with the standard library
    else
    {
      debug_info("[bypass] dlsym_chmod\n");
      return dlsym_chmod((char *)path, mode);
    }
  }

  int fchmod(int fd, mode_t mode)
  {
    debug_info("[bypass] Before fchmod...\n");

    int ret = -1;

    struct generic_fd virtual_fd = fdstable_get ( fd );

    if(virtual_fd.type == FD_XPN)
    {
      // We must initialize expand if it has not been initialized yet.
      xpn_adaptor_keepInit ();

      debug_info("[bypass] xpn_fchmod\n");
      ret = xpn_fchmod(fd,mode);
    }
    // Not an XPN partition. We must link with the standard library
    else{
      debug_info("[bypass] dlsym_fchmod\n");
      ret = dlsym_fchmod(fd,mode);
    }

    return ret;
  }

  int chown(const char *path, uid_t owner, gid_t group)
  {
    debug_info("[bypass] Before chown...\n");

    if(is_prefix(xpn_adaptor_partition_prefix, path))
    {
      // We must initialize expand if it has not been initialized yet.
      xpn_adaptor_keepInit ();

      debug_info("[bypass] xpn_chown\n");
      return(xpn_chown((const char *)(path+strlen(xpn_adaptor_partition_prefix)), owner, group));
    }
    // Not an XPN partition. We must link with the standard library
    else
    {
      debug_info("[bypass] dlsym_chown\n");
      return dlsym_chown((char *)path, owner, group);
    }
  }

  int fcntl(int fd, int cmd, long arg) //TODO
  {
    int ret = -1;

    struct generic_fd virtual_fd = fdstable_get ( fd );

    if(virtual_fd.type == FD_XPN)
    {
      debug_info("[bypass] xpn_fcntl\n");
      //TODO
      ret = 0;
    }
    else{
      debug_info("[bypass] dlsym_fcntl\n");
      ret = dlsym_fcntl(fd, cmd, arg);
    }

    return ret;
  }

  int access(const char *path, int mode){
    debug_info("[bypass] Before access...\n");

    if(is_prefix(xpn_adaptor_partition_prefix, path))
    {
      // We must initialize expand if it has not been initialized yet.
      xpn_adaptor_keepInit ();

      debug_info("[bypass] xpn_access\n");

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
      debug_info("[bypass] dlsym_access\n");
      return dlsym_access(path, mode);
    }
  }

  char *realpath(const char *restrict path, char *restrict resolved_path)
  {
    debug_info("[bypass] Before realpath...\n");
    debug_info("[bypass] PATH %s\n", path);

    if(is_prefix(xpn_adaptor_partition_prefix, path))
    {
      // We must initialize expand if it has not been initialized yet.
      xpn_adaptor_keepInit ();

      debug_info("[bypass] Before realpath...\n");
      strcpy(resolved_path, path);
      return resolved_path;
    }
    // Not an XPN partition. We must link with the standard library
    else
    {
      debug_info("[bypass] Before dlsym_realpath...\n");
      return dlsym_realpath(path, resolved_path);
    }
  }

  int fsync(int fd) //TODO
  {
    int ret = -1;

    struct generic_fd virtual_fd = fdstable_get ( fd );

    if(virtual_fd.type == FD_XPN)
    {
      debug_info("[bypass] xpn_fsync\n");
      //TODO
      ret = 0;
    }
    else{
      debug_info("[bypass] dlsym_fsync\n");
      ret = dlsym_fsync(fd);
    }

    return ret;
  }




  // MPI API

  int MPI_Init (int *argc, char ***argv)
  {
    char *value;

    debug_info("[bypass] Before MPI_Init\n");

    // We must initialize expand if it has not been initialized yet.
    xpn_adaptor_keepInit ();

    value = getenv("XPN_IS_MPI_SERVER") ;
    if (NULL == value){
      return PMPI_Init(argc, argv);
    }
    return MPI_SUCCESS;
  }

  int MPI_Finalize (void)
  {
    char *value;

    value = getenv("XPN_IS_MPI_SERVER") ;
    if (NULL != value && xpn_adaptor_initCalled == 1){
      xpn_destroy();
    }

    return PMPI_Finalize();
  }
