/*
 *  Copyright 2020-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
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

     #include "filesystem.h"


  /* ... Varibles ........................................... */

     void * DLSYM_RTLD = RTLD_DEFAULT ;

     int     (*fs_low_creat   )(const char *, mode_t)             = creat ;
     int     (*fs_low_open    )(const char *, int, mode_t)        = (int (*)(const char *, int, mode_t))open ;
     int     (*fs_low_close   )(int)                              = close ;
     ssize_t (*fs_low_read    )(int, void*, size_t)               = read ;
     ssize_t (*fs_low_write   )(int, const void*, size_t)         = write ;
     off_t   (*fs_low_lseek   )(int, off_t, int)                  = lseek ;
     off64_t (*fs_low_lseek64 )(int, off64_t, int)                = lseek64 ;
     int     (*fs_low_stat    )(const char *, struct stat *)      = stat ;

     int     (*fs_low_mkdir   )(const char *, mode_t)             = mkdir ;
     int     (*fs_low_rmdir   )(const char *)                     = rmdir ;
     int     (*fs_low_unlink  )(const char *)                     = unlink ;
     int     (*fs_low_rename  )(const char *, const  char *)      = rename ;

     DIR*            (*fs_low_opendir  )(const char *) = opendir ;
     long            (*fs_low_telldir  )(DIR *)        = telldir ;
     void            (*fs_low_seekdir  )(DIR *, long)  = seekdir ;
     struct dirent * (*fs_low_readdir  )(DIR *)        = readdir ;
     int             (*fs_low_closedir )(DIR *)        = closedir ;

     //pthread_attr_t filesystem_attr;


  /* ... Functions / Funciones ......................................... */


     /*
      * Internal
      */

int aux_clear_dirs(char * path)
{
    char  s[PATH_MAX];
    char  ant = '\0' ;
    int      j = 0;
    unsigned i = 0 ;
    unsigned sl = 0 ;

    if (NULL == path) {
        return -1;
    }

    sl = strlen(path) ;
    for (i = 0; i < sl; i++)
    {
        switch (path[i])
	{
           case '/':
               if (ant != '/') {
                   ant = s[j] = '/';
                   j++;
               }
               break;

           default:
               ant = s[j] = path[i];
               j++;
        }

        s[j] = '\0';
    }

    strcpy(path, s);
    return 0;
}


int aux_get_dirs(char * path, int n, char * s)
{
    long unsigned i = 0;
    long j = 0, ant = -1, pos = -1;
    int cont = -1;
    char new_path[PATH_MAX];

    if (path == NULL) {
        return 0;
    }

    strcpy(new_path, path);
    path = new_path;

    aux_clear_dirs(path);
    s[0] = '\0';

    for (i = 0; i < strlen(path) && cont < (n + 1); i++) {
        if (path[i] == '/') {
            if (ant < 0) {
                ant = pos = i;
            }
            pos = i;
            cont++;
        }
    }

    if (cont < (n + 1)) {
        return 0;
    }

    for (j = ant; j < pos; j++) {
        s[j] = path[j];
    }

    s[j] = '\0';

    return strlen(s);
}


/*
 * API
 */

int  filesystem_low_set ( void * new_rtld )
{
     void * old_rtld = DLSYM_RTLD ;

     DEBUG_BEGIN();

     if (RTLD_NEXT    == new_rtld) DLSYM_RTLD = RTLD_NEXT ;
     if (RTLD_DEFAULT == new_rtld) DLSYM_RTLD = RTLD_DEFAULT ;
     if (old_rtld == new_rtld) return 1 ;

     fs_low_creat   = (int     (*)(const char *, mode_t))              dlsym(DLSYM_RTLD, "creat") ;
     fs_low_open    = (int     (*)(const char *, int, mode_t))         dlsym(DLSYM_RTLD, "open") ;
     fs_low_close   = (int     (*)(int))                               dlsym(DLSYM_RTLD, "close") ;
     fs_low_read    = (ssize_t (*)(int, void*, size_t))                dlsym(DLSYM_RTLD, "read") ;
     fs_low_write   = (ssize_t (*)(int, const void*, size_t))          dlsym(DLSYM_RTLD, "write") ;
     fs_low_lseek   = (off_t   (*)(int, off_t, int))                   dlsym(DLSYM_RTLD, "lseek");
     fs_low_lseek64 = (off64_t (*)(int, off64_t, int))                 dlsym(DLSYM_RTLD, "lseek64") ;
     fs_low_stat    = (int     (*)(const char *, struct stat *))       dlsym(DLSYM_RTLD, "stat") ;

     fs_low_mkdir   = (int     (*)(const char *, mode_t))              dlsym(DLSYM_RTLD, "mkdir") ;
     fs_low_rmdir   = (int     (*)(const char *))                      dlsym(DLSYM_RTLD, "rmdir") ;
     fs_low_unlink  = (int     (*)(const char *))                      dlsym(DLSYM_RTLD, "unlink") ;
     fs_low_rename  = (int     (*)(const char *, const char *))        dlsym(DLSYM_RTLD, "rename") ;

     fs_low_opendir  = (DIR *           (*)(const char *))             dlsym(DLSYM_RTLD, "opendir") ;
     fs_low_telldir  = (long            (*)(DIR *))                    dlsym(DLSYM_RTLD, "telldir") ;
     fs_low_seekdir  = (void            (*)(DIR *, long))              dlsym(DLSYM_RTLD, "seekdir") ;
     fs_low_readdir  = (struct dirent * (*)(DIR *))                    dlsym(DLSYM_RTLD, "readdir") ;
     fs_low_closedir = (int             (*)(DIR *))                    dlsym(DLSYM_RTLD, "closedir") ;

     DEBUG_END();

     // Return OK
     return 1 ;
}

/*
int  filesystem_init ( void )
{
  pthread_attr_init(&filesystem_attr);
  int ret = pthread_attr_setdetachstate(&filesystem_attr, PTHREAD_CREATE_DETACHED);
  if (ret !=0 ) {
    //perror("pthread_attr_setdetachstate: ");
    return ret;
  }

  return 0;
}

int  filesystem_destroy ( void )
{
  int ret = pthread_attr_destroy(&filesystem_attr);
  if (ret !=0 ) {
    //perror("pthread_attr_destroy: ");
    return ret;
  }

  return 0;
}
*/

void * filesystem_async_close(void * arg)
{
    // Try to close file
    int ret = fs_low_close((int)(long) arg);
    if (ret < 0) {
        debug_warning("[FILE_POSIX]: async_close(fd:%d) -> %d\n", (int)(long) arg, ret);
        //perror("async_close: ") ;
    }

    pthread_exit(NULL);
}

int filesystem_creat(char * pathname, mode_t mode)
{
    int ret;

    DEBUG_BEGIN();

    // Check params
    if (NULL == pathname) {
        debug_warning("[FILE_POSIX]: pathname is NULL\n");
    }
    if (0 == mode) {
        debug_warning("[FILE_POSIX]: mode is zero\n");
    }

    // Try to creat the file
    ret = fs_low_creat(pathname, mode);
    if (ret < 0) {
        debug_warning("[FILE_POSIX]: open(pathname:%s, mode:%d) -> %d\n", pathname, mode, ret);
        //perror("open: ") ;
    }

    DEBUG_END();

    // Return OK/KO
    return ret;
}

int filesystem_open(char * pathname, int flags)
{
    int ret;

    DEBUG_BEGIN();

    // Check params
    if (NULL == pathname) {
        debug_warning("[FILE_POSIX]: pathname is NULL\n");
    }

    // Try to open the file
    ret = fs_low_open(pathname, flags, 0);
    if (ret < 0) {
        debug_warning("[FILE_POSIX]: open(pathname:%s, flags:%d) -> %d\n", pathname, flags, ret);
        //perror("open: ") ;
    }

    DEBUG_END();

    // Return OK/KO
    return ret;
}

int filesystem_open2(char * pathname, int flags, mode_t mode)
{
    int ret;

    DEBUG_BEGIN();

    // Check params
    if (NULL == pathname) {
        debug_warning("[FILE_POSIX]: pathname is NULL\n");
    }

    // Try to open the file
    ret = fs_low_open(pathname, flags, mode);
    if (ret < 0) {
        debug_warning("[FILE_POSIX]: open2(pathname:%s, flags:%d, mode:%d) -> %d\n", pathname, flags, mode, ret);
        //perror("open: ") ;
    }

    DEBUG_END();

    // Return OK/KO
    return ret;
}

int filesystem_close(int fd)
{
    int ret;

    DEBUG_BEGIN();

    // Check params
    if (fd < 0) {
        debug_warning("[FILE_POSIX]: close file with fd < 0\n");
    }

    // Try to close file
    #ifdef ASYNC_CLOSE
    pthread_t thid;

    ret = pthread_create( & thid, NULL, filesystem_async_close, (void * )(long) fd);
    ret = pthread_detach(thid);

    if (ret < 0) {
        ret = fs_low_close(fd);
        if (ret < 0) {
            debug_warning("[FILE_POSIX]: close(fd:%d) -> %d\n", fd, ret);
            //perror("close: ") ;
        }
    }

    #else
    ret = fs_low_close(fd);
    if (ret < 0) {
        debug_warning("[FILE_POSIX]: close(fd:%d) -> %d\n", fd, ret);
        //perror("close: ") ;
    }
    #endif

    DEBUG_END();

    // Return OK/KO
    return ret;
}

ssize_t filesystem_read(int read_fd2, void * buffer, size_t buffer_size)
{
    ssize_t read_num_bytes = -1;
    ssize_t read_remaining_bytes = buffer_size;
    void * read_buffer = buffer;

    // check arguments...
    if (NULL == buffer) {
        debug_warning("[FILE_POSIX]: read_buffer with NULL buffer\n");
    }

    while (read_remaining_bytes > 0) {
        /* Read from local file... */
        read_num_bytes = fs_low_read(read_fd2, read_buffer, read_remaining_bytes);

        /* Check errors */
        if (read_num_bytes < 0) {
            //perror("read: ") ;
            debug_error("[FILE_POSIX]: read fails to read data.\n");
            return -1;
        }

        /* Check end of file */
        if (read_num_bytes == 0) {
            debug_error("[FILE_POSIX]: end of file, readed %ld.\n", (buffer_size - read_remaining_bytes));
            return (buffer_size - read_remaining_bytes);
        }

        read_remaining_bytes = read_remaining_bytes - read_num_bytes;
        read_buffer = (void * )((char * ) read_buffer + read_num_bytes);
    }

    return buffer_size;
}

ssize_t filesystem_write(int write_fd2, void * buffer, size_t num_bytes_to_write)
{
    ssize_t write_num_bytes = -1;
    ssize_t write_remaining_bytes = num_bytes_to_write;
    void * write_buffer = buffer;

    // check arguments...
    if (NULL == buffer) {
        debug_warning("[FILE_POSIX]: read_buffer with NULL buffer\n");
    }

    while (write_remaining_bytes > 0)
    {
        /* Write into local file (write_fd2)... */
        write_num_bytes = fs_low_write(write_fd2, write_buffer, write_remaining_bytes);

        /* Check errors */
        if (write_num_bytes < 0) {
            //perror("write: ") ;
            debug_error("[FILE_POSIX]: write fails to write data.\n");
            return -1;
        }

        write_remaining_bytes = write_remaining_bytes - write_num_bytes;
        write_buffer = (void * )((char * ) write_buffer + write_num_bytes);
    }

    return num_bytes_to_write;
}

int filesystem_rename(char * old_pathname, char * new_pathname)
{
    int ret;

    DEBUG_BEGIN();

    // Check params
    if (NULL == old_pathname) {
        debug_warning("[FILE_POSIX]: old_pathname is NULL\n");
    }
    if (NULL == new_pathname) {
        debug_warning("[FILE_POSIX]: new_pathname is NULL\n");
    }

    // Try to open the file
    ret = fs_low_rename(old_pathname, new_pathname);
    if (ret < 0) {
        debug_warning("[FILE_POSIX]: rename(old_pathname:%s, new_pathname:%s)\n", old_pathname, new_pathname);
        //perror("rename: ") ;
    }

    DEBUG_END();

    // Return OK/KO
    return ret;
}

int filesystem_mkpath ( char * pathname )
{
    int ret;
    char dir[PATH_MAX];

    DEBUG_BEGIN();

    for (int i = 0; aux_get_dirs(pathname, i, dir) != 0; i++) {
        ret = fs_low_mkdir(dir, 0770);
        if (ret < 0) {
            debug_warning("[FILE_POSIX]: cannot mkdir(%s)\n", dir);
            //perror("mkdir: ") ;
        }
    }

    DEBUG_END();

    // Return OK
    return 1;
}

int filesystem_mkdir ( char * pathname, mode_t mode )
{
    int ret;

    DEBUG_BEGIN();

    // Check params
    if (NULL == pathname) {
        debug_warning("[FILE_POSIX]: pathname is NULL\n");
    }

    // Try to mkdir
    ret = fs_low_mkdir(pathname, mode);
    if (ret < 0) {
        debug_warning("[FILE_POSIX]: mkdir(pathname:%s, mode:%d) -> %d\n", pathname, mode, ret);
        //perror("mkdir: ") ;
    }

    DEBUG_END();

    // Return OK/KO
    return ret;
}

int filesystem_rmdir ( char * pathname )
{
    int ret;

    DEBUG_BEGIN();

    // Check params
    if (NULL == pathname) {
        debug_warning("[FILE_POSIX]: pathname is NULL\n");
    }

    // Try to rmdir
    ret = fs_low_rmdir(pathname);
    if (ret < 0) {
        debug_warning("[FILE_POSIX]: rmdir(pathname:%s) -> %d\n", pathname, ret);
        //perror("rmdir: ") ;
    }

    DEBUG_END();

    // Return OK/KO
    return ret;
}

DIR * filesystem_opendir ( char * pathname )
{
    DIR * ret;

    DEBUG_BEGIN();

    // Check params
    if (NULL == pathname) {
        debug_warning("[FILE_POSIX]: pathname is NULL\n");
    }

    // Try to open the directory
    ret = fs_low_opendir(pathname);
    if (NULL == ret) {
        debug_warning("[FILE_POSIX]: opendir(pathname:%s) -> %p\n", pathname, ret);
        //perror("opendir: ") ;
    }

    DEBUG_END();

    // Return DIR*
    return ret;
}

long filesystem_telldir ( DIR * dirp )
{
    long ret;

    DEBUG_BEGIN();

    // Check params
    if (NULL == dirp) {
        debug_warning("[FILE_POSIX]: dirp is NULL\n");
    }

    ret = fs_low_telldir(dirp);
    if (-1 == ret) {
        debug_warning("[FILE_POSIX]: telldir(dirp:%p) -> %ld\n", dirp, ret);
        //perror("telldir: ") ;
    }

    DEBUG_END();

    // Return DIR*
    return ret;
}

void filesystem_seekdir ( DIR * dirp, long loc )
{
    DEBUG_BEGIN();

    // Check params
    if (NULL == dirp) {
        debug_warning("[FILE_POSIX]: dirp is NULL\n");
    }

    fs_low_seekdir(dirp, loc);

    DEBUG_END();
}

struct dirent * filesystem_readdir ( DIR * dirp )
{
    struct dirent * ret;

    DEBUG_BEGIN();

    // Check params
    if (NULL == dirp) {
        debug_warning("[FILE_POSIX]: dirp is NULL\n");
    }

    // Try to read next entry of the directory
    ret = fs_low_readdir(dirp);
    if (NULL == ret) {
        debug_warning("[FILE_POSIX]: readdir(dirp:%p) -> %p\n", dirp, ret);
        //perror("readdir: ") ;
    }

    DEBUG_END();

    // Return DIR*
    return ret;
}

int filesystem_closedir ( DIR * dirp )
{
    int ret;

    DEBUG_BEGIN();

    // Check params
    if (NULL == dirp) {
        debug_warning("[FILE_POSIX]: dirp is NULL\n");
    }

    // Try to close the directory session
    ret = fs_low_closedir(dirp);
    if (ret < 0) {
        debug_warning("[FILE_POSIX]: closedir(dirp:%p) -> %p\n", dirp, ret);
        //perror("closedir: ") ;
    }

    DEBUG_END();

    // Return DIR*
    return ret;
}

off_t filesystem_lseek ( int fd, off_t offset, int whence )
{
    off_t ret;

    DEBUG_BEGIN();

    // Check params
    if (fd < 0) {
        debug_warning("[FILE_POSIX]: fd is negative\n");
    }

    // Try to lseek the file
    ret = fs_low_lseek(fd, offset, whence);
    if (ret == (off_t) -1) {
        debug_warning("[FILE_POSIX]: lseek(fd:%s, offset:%ld, whence:%d) -> %d\n", fd, offset, whence, ret);
        //perror("lseek: ") ;
    }

    DEBUG_END();

    // Return OK/KO
    return ret;
}

int filesystem_unlink ( char * pathname )
{
    int ret;

    DEBUG_BEGIN();

    // Check params
    if (NULL == pathname) {
        debug_warning("[FILE_POSIX]: pathname is NULL\n");
    }

    // Try to unlink a file
    ret = fs_low_unlink(pathname);
    if (ret < 0) {
        debug_warning("[FILE_POSIX]: unlink(pathname:%s) -> %d\n", pathname, ret);
        //perror("unlink: ") ;
    }

    DEBUG_END();

    // Return OK/KO
    return ret;
}

int filesystem_stat ( char * pathname, struct stat * sinfo )
{
    int ret;

    DEBUG_BEGIN();

    // Check params
    if (NULL == pathname) {
        debug_warning("[FILE_POSIX]: pathname is NULL\n");
    }
    if (NULL == sinfo) {
        debug_warning("[FILE_POSIX]: sinfo is NULL\n");
    }

    // Try to stat the file
    ret = fs_low_stat(pathname, sinfo);
    if (ret < 0) {
        debug_warning("[FILE_POSIX]: stat(pathname:%s, sinfo:%p) -> %d\n", pathname, sinfo, ret);
        //perror("stat: ") ;
    }

    DEBUG_END();

    // Return OK/KO
    return ret;
}


  /* ................................................................... */

