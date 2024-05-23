/*
 *  Copyright 2020-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos,
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

     #include "filesystem_low.h"


  /* ... Varibles ........................................... */

     void * DLSYM_RTLD = RTLD_DEFAULT ;

     int     (*fslow_creat   )(const char *, mode_t)             = creat ;
     int     (*fslow_open    )(const char *, int, mode_t)        = (int (*)(const char *, int, mode_t))open ;
     int     (*fslow_close   )(int)                              = close ;
     ssize_t (*fslow_read    )(int, void*, size_t)               = read ;
     ssize_t (*fslow_write   )(int, const void*, size_t)         = write ;
     off_t   (*fslow_lseek   )(int, off_t, int)                  = lseek ;
     off64_t (*fslow_lseek64 )(int, off64_t, int)                = lseek64 ;
     int     (*fslow_stat    )(int, const char *, struct stat *) = __xstat ;

     int     (*fslow_mkdir   )(const char *, mode_t)             = mkdir ;
     int     (*fslow_rmdir   )(const char *)                     = rmdir ;
     int     (*fslow_unlink  )(const char *)                     = unlink ;
     int     (*fslow_rename  )(const char *, const  char *)      = rename ;

     DIR*            (*fslow_opendir  )(const char *) = opendir ;
     long            (*fslow_telldir  )(DIR *)        = telldir ;
     void            (*fslow_seekdir  )(DIR *, long)  = seekdir ;
     struct dirent * (*fslow_readdir  )(DIR *)        = readdir ;
     int             (*fslow_closedir )(DIR *)        = closedir ;


  /* ... Functions / Funciones ......................................... */

int  fs_low_set ( void * new_rtld )
{
     void * old_rtld = DLSYM_RTLD ;
     if (RTLD_NEXT    == new_rtld) DLSYM_RTLD = RTLD_NEXT ;
     if (RTLD_DEFAULT == new_rtld) DLSYM_RTLD = RTLD_DEFAULT ;
     if (old_rtld == new_rtld) return 1 ;

     fslow_creat   = (int     (*)(const char *, mode_t))              dlsym(DLSYM_RTLD, "creat") ;
     fslow_open    = (int     (*)(const char *, int, mode_t))         dlsym(DLSYM_RTLD, "open") ;
     fslow_close   = (int     (*)(int))                               dlsym(DLSYM_RTLD, "close") ;
     fslow_read    = (ssize_t (*)(int, void*, size_t))                dlsym(DLSYM_RTLD, "read") ;
     fslow_write   = (ssize_t (*)(int, const void*, size_t))          dlsym(DLSYM_RTLD, "write") ;
     fslow_lseek   = (off_t   (*)(int, off_t, int))                   dlsym(DLSYM_RTLD, "lseek");
     fslow_lseek64 = (off64_t (*)(int, off64_t, int))                 dlsym(DLSYM_RTLD, "lseek64") ;
     fslow_stat    = (int     (*)(int, const char *, struct stat *))  dlsym(DLSYM_RTLD, "__xstat") ;

     fslow_mkdir   = (int     (*)(const char *, mode_t))              dlsym(DLSYM_RTLD, "mkdir") ;
     fslow_rmdir   = (int     (*)(const char *))                      dlsym(DLSYM_RTLD, "rmdir") ;
     fslow_unlink  = (int     (*)(const char *))                      dlsym(DLSYM_RTLD, "unlink") ;
     fslow_rename  = (int     (*)(const char *, const char *))        dlsym(DLSYM_RTLD, "rename") ;

     fslow_opendir  = (DIR *           (*)(const char *))             dlsym(DLSYM_RTLD, "opendir") ;
     fslow_telldir  = (long            (*)(DIR *))                    dlsym(DLSYM_RTLD, "telldir") ;
     fslow_seekdir  = (void            (*)(DIR *, long))              dlsym(DLSYM_RTLD, "seekdir") ;
     fslow_readdir  = (struct dirent * (*)(DIR *))                    dlsym(DLSYM_RTLD, "readdir") ;
     fslow_closedir = (int             (*)(DIR *))                    dlsym(DLSYM_RTLD, "closedir") ;

     return 1 ;
}

// Files

int  fs_low_creat ( char * path, mode_t mode )
{
    int fd ;
    fd = fslow_creat(path, mode);
    return fd;
}

int  fs_low_open ( char * path, int flags )
{
    int fd ;
    fd = fslow_open(path, flags, 0);
    return fd;
}

int  fs_low_open2 ( char * path, int flags, mode_t mode )
{
    int fd ;
    fd = fslow_open(path, flags, mode);
    return fd;
}

int  fs_low_close ( int fd )
{
    int ret ;
    ret = fslow_close(fd);
    return ret;
}

ssize_t fs_low_read ( int fd, void * buffer, size_t buffer_size )
{
    ssize_t ret ;
    ret = fslow_read(fd, buffer, buffer_size);
    return ret ;
}

ssize_t fs_low_write ( int fd, void * buffer, size_t buffer_size )
{
    ssize_t ret ;
    ret = fslow_write(fd, buffer, buffer_size);
    return ret ;
}

off_t  fs_low_lseek ( int fd, off_t offset, int whence )
{
    off_t ret ;
    ret = fslow_lseek(fd, offset, whence);
    return ret ;
}

off64_t  fs_low_lseek64 ( int fd, off64_t offset, int whence )
{
    off64_t ret ;
    ret = fslow_lseek64(fd, offset, whence);
    return ret ;
}

int    fs_low_stat ( char * pathname, struct stat * info )
{
    int ret ;
    ret = fslow_stat(0, (char *)pathname, info);
    return ret ;
}


// Entries in directory

int  fs_low_mkdir ( char * pathname, mode_t mode )
{
    int ret ;
    ret = fslow_mkdir(pathname, mode) ;
    return ret ;
}

int  fs_low_rmdir ( char * pathname )
{
    int ret ;
    ret = fslow_rmdir(pathname) ;
    return ret ;
}

int  fs_low_unlink ( char * pathname )
{
    int ret ;
    ret = fslow_unlink(pathname) ;
    return ret ;
}

int  fs_low_rename ( char * old_path, char * new_path )
{
    int ret ;
    ret = fslow_rename(old_path, new_path) ;
    return ret ;
}


// Directory listing

DIR *   fs_low_opendir ( char * pathname )
{
    DIR * ret ;
    ret = fs_low_opendir(pathname) ;
    return ret ;
}

long    fs_low_telldir ( DIR * dirptr )
{
    long ret ;
    ret = fs_low_telldir(dirptr) ;
    return ret ;
}

void    fs_low_seekdir ( DIR * dirptr, long loc )
{
    fs_low_seekdir(dirptr, loc) ;
}

struct dirent * fs_low_readdir ( DIR * dirptr )
{
    struct dirent * ret ;
    ret = fs_low_readdir(dirptr) ;
    return ret ;
}

int     fs_low_closedir ( DIR * dirptr )
{
    int ret ;
    ret = fs_low_closedir(dirptr) ;
    return ret ;
}


  /* ................................................................... */


