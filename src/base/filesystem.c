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

     //pthread_attr_t filesystem_attr;

  /* ... Functions / Funciones ......................................... */

     /*
      * Internal
      */

int aux_clear_dirs(char * path)
{
    int j;
    char ant = '\0', s[255];

    if (NULL == path) {
        return -1;
    }

    j = 0;
    for (unsigned i = 0; i < strlen(path); i++) {
        switch (path[i]) {
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

/*int  filesystem_init ( void )
{
  pthread_attr_init(&filesystem_attr);
  int ret = pthread_attr_setdetachstate(&filesystem_attr, PTHREAD_CREATE_DETACHED);
  if (ret !=0 ) {
    //perror("pthread_attr_setdetachstate: ");
    return ret;
  }

  return 0;
}*/

void * filesystem_async_close(void * arg)
{
    // Try to close file
    int ret = real_posix_close((int)(long) arg);
    if (ret < 0) {
        debug_warning("[FILE_POSIX]: async_close(fd:%d) -> %d\n", (int)(long) arg, ret);
        //perror("async_close: ") ;
    }

    pthread_exit(NULL);
}

/*int  filesystem_destroy ( void )
{
  int ret = pthread_attr_destroy(&filesystem_attr);
  if (ret !=0 ) {
    //perror("pthread_attr_destroy: ");
    return ret;
  }

  return 0;
}*/

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
    ret = real_posix_creat(pathname, mode);
    if (ret < 0) {
        debug_warning("[FILE_POSIX]: open(pathname:%s, flags:%d, mode:%d) -> %d\n", pathname, flags, mode, ret);
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
    ret = real_posix_open(pathname, flags);
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
    ret = real_posix_open2(pathname, flags, mode);
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
        ret = real_posix_close(fd);
        if (ret < 0) {
            debug_warning("[FILE_POSIX]: close(fd:%d) -> %d\n", fd, ret);
            //perror("close: ") ;
        }
    }

    #else
    ret = real_posix_close(fd);
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
        read_num_bytes = real_posix_read(read_fd2, read_buffer, read_remaining_bytes);

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
        write_num_bytes = real_posix_write(write_fd2, write_buffer, write_remaining_bytes);

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
    ret = real_posix_rename(old_pathname, new_pathname);
    if (ret < 0) {
        debug_warning("[FILE_POSIX]: rename(old_pathname:%s, new_pathname:%s)\n", old_pathname, new_pathname);
        //perror("rename: ") ;
    }

    DEBUG_END();

    // Return OK/KO
    return ret;
}

int filesystem_mkpath(char * pathname)
{
    int ret;
    char dir[PATH_MAX];

    DEBUG_BEGIN();

    for (int i = 0; aux_get_dirs(pathname, i, dir) != 0; i++) {
        ret = real_posix_mkdir(dir, 0770);
        if (ret < 0) {
            debug_warning("[FILE_POSIX]: cannot mkdir(%s)\n", dir);
            //perror("mkdir: ") ;
        }
    }

    DEBUG_END();

    // Return OK
    return 1;
}

int filesystem_mkdir(char * pathname, mode_t mode)
{
    int ret;

    DEBUG_BEGIN();

    // Check params
    if (NULL == pathname) {
        debug_warning("[FILE_POSIX]: pathname is NULL\n");
    }

    // Try to mkdir
    ret = real_posix_mkdir(pathname, mode);
    if (ret < 0) {
        debug_warning("[FILE_POSIX]: mkdir(pathname:%s, mode:%d) -> %d\n", pathname, mode, ret);
        //perror("mkdir: ") ;
    }

    DEBUG_END();

    // Return OK/KO
    return ret;
}

int filesystem_rmdir(char * pathname)
{
    int ret;

    DEBUG_BEGIN();

    // Check params
    if (NULL == pathname) {
        debug_warning("[FILE_POSIX]: pathname is NULL\n");
    }

    // Try to rmdir
    ret = real_posix_rmdir(pathname);
    if (ret < 0) {
        debug_warning("[FILE_POSIX]: rmdir(pathname:%s) -> %d\n", pathname, ret);
        //perror("rmdir: ") ;
    }

    DEBUG_END();

    // Return OK/KO
    return ret;
}

DIR * filesystem_opendir(char * pathname)
{
    DIR * ret;

    DEBUG_BEGIN();

    // Check params
    if (NULL == pathname) {
        debug_warning("[FILE_POSIX]: pathname is NULL\n");
    }

    // Try to open the directory
    ret = real_posix_opendir(pathname);
    if (NULL == ret) {
        debug_warning("[FILE_POSIX]: opendir(pathname:%s) -> %p\n", pathname, ret);
        //perror("opendir: ") ;
    }

    DEBUG_END();

    // Return DIR*
    return ret;
}

struct dirent * filesystem_readdir(DIR * dirp)
{
    struct dirent * ret;

    DEBUG_BEGIN();

    // Check params
    if (NULL == dirp) {
        debug_warning("[FILE_POSIX]: dirp is NULL\n");
    }

    // Try to read next entry of the directory
    ret = real_posix_readdir(dirp);
    if (NULL == ret) {
        debug_warning("[FILE_POSIX]: readdir(dirp:%p) -> %p\n", dirp, ret);
        //perror("readdir: ") ;
    }

    DEBUG_END();

    // Return DIR*
    return ret;
}

int filesystem_closedir(DIR * dirp)
{
    int ret;

    DEBUG_BEGIN();

    // Check params
    if (NULL == dirp) {
        debug_warning("[FILE_POSIX]: dirp is NULL\n");
    }

    // Try to close the directory session
    ret = real_posix_closedir(dirp);
    if (ret < 0) {
        debug_warning("[FILE_POSIX]: closedir(dirp:%p) -> %p\n", dirp, ret);
        //perror("closedir: ") ;
    }

    DEBUG_END();

    // Return DIR*
    return ret;
}

off_t filesystem_lseek(int fd, off_t offset, int whence)
{
    off_t ret;

    DEBUG_BEGIN();

    // Check params
    if (fd < 0) {
        debug_warning("[FILE_POSIX]: fd is negative\n");
    }

    // Try to lseek the file
    ret = real_posix_lseek(fd, offset, whence);
    if (ret == (off_t) -1) {
        debug_warning("[FILE_POSIX]: lseek(fd:%s, offset:%ld, whence:%d) -> %d\n", fd, offset, whence, ret);
        //perror("lseek: ") ;
    }

    DEBUG_END();

    // Return OK/KO
    return ret;
}

int filesystem_unlink(char * pathname)
{
    int ret;

    DEBUG_BEGIN();

    // Check params
    if (NULL == pathname) {
        debug_warning("[FILE_POSIX]: pathname is NULL\n");
    }

    // Try to unlink a file
    ret = real_posix_unlink(pathname);
    if (ret < 0) {
        debug_warning("[FILE_POSIX]: unlink(pathname:%s) -> %d\n", pathname, ret);
        //perror("unlink: ") ;
    }

    DEBUG_END();

    // Return OK/KO
    return ret;
}

int filesystem_stat(char * pathname, struct stat * sinfo)
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
    ret = real_posix_stat(pathname, sinfo);
    if (ret < 0) {
        debug_warning("[FILE_POSIX]: stat(pathname:%s, sinfo:%p) -> %d\n", pathname, sinfo, ret);
        //perror("stat: ") ;
    }

    DEBUG_END();

    // Return OK/KO
    return ret;
}


  /* ................................................................... */

