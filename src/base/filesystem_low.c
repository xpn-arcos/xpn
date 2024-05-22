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

     int FILESYSTEM_DLSYM = 1 ;


  /* ... Functions / Funciones ......................................... */

// Files

int  fs_low_creat ( char * path, mode_t mode )
{
    int ret ;

    if (FILESYSTEM_DLSYM)
         ret = dlsym_creat(path, mode) ;
    else ret = creat(path, mode) ;

    return ret ;
}

int  fs_low_open ( char * path, int flags )
{
    int ret ;

    if (FILESYSTEM_DLSYM)
         ret = dlsym_open(path, flags) ;
    else ret = open(path, flags) ;

    return ret ;
}

int  fs_low_open2(char * path, int flags, mode_t mode)
{
    int ret ;

    if (FILESYSTEM_DLSYM)
         ret = dlsym_open2(path, flags, mode) ;
    else ret = open(path,flags, mode) ;

    return ret ;
}

int  fs_low_close(int fd)
{
    int ret ;

    if (FILESYSTEM_DLSYM)
         ret = dlsym_close(fd) ;
    else ret = close(fd) ;

    return ret ;
}

ssize_t fs_low_read(int fd, void * buffer, size_t buffer_size)
{
    ssize_t ret ;

    if (FILESYSTEM_DLSYM)
         ret = dlsym_read(fd, buffer, buffer_size) ;
    else ret = read(fd, buffer, buffer_size) ;

    return ret ;
}

ssize_t fs_low_write(int fd, void * buffer, size_t buffer_size)
{
    ssize_t ret ;

    if (FILESYSTEM_DLSYM)
         ret = dlsym_write(fd, buffer, buffer_size) ;
    else ret = write(fd, buffer, buffer_size) ;

    return ret ;
}

off_t  fs_low_lseek(int fd, off_t offset, int whence)
{
    off_t ret ;

    if (FILESYSTEM_DLSYM)
         ret = dlsym_lseek(fd,offset,whence) ;
    else ret = lseek(fd,offset,whence) ;

    return ret ;
}

off64_t  fs_low_lseek64(int fd, off64_t offset, int whence)
{
    off64_t ret ;

    if (FILESYSTEM_DLSYM)
         ret = dlsym_lseek64(fd,offset,whence) ;
    else ret = lseek64(fd,offset,whence) ;

    return ret ;
}

int    fs_low_stat(char * pathname, struct stat * info)
{
    int ret ;

    if (FILESYSTEM_DLSYM)
         ret = dlsym_stat(0, pathname, info) ;  // ver:0
    else ret = stat(pathname, info) ;

    return ret ;
}


// Entries in directory

int     fs_low_mkdir(char * pathname, mode_t mode)
{
    int ret ;

    if (FILESYSTEM_DLSYM)
         ret = dlsym_mkdir(pathname,mode) ;
    else ret = mkdir(pathname,mode) ;

    return ret ;
}

int     fs_low_rmdir(char * pathname)
{
    int ret ;

    if (FILESYSTEM_DLSYM)
         ret = dlsym_rmdir(pathname) ;
    else ret = rmdir(pathname) ;

    return ret ;
}

int     fs_low_unlink(char * pathname)
{
    int ret ;

    if (FILESYSTEM_DLSYM)
         ret = dlsym_unlink(pathname) ;
    else ret = unlink(pathname) ;

    return ret ;
}

int     fs_low_rename(char * old_path, char * new_path)
{
    int ret ;

    if (FILESYSTEM_DLSYM)
         ret = dlsym_rename(old_path, new_path) ;
    else ret = rename(old_path, new_path) ;

    return ret ;
}


// Directory listing

DIR *   fs_low_opendir(char * pathname)
{
    DIR * ret ;

    if (FILESYSTEM_DLSYM)
         ret = dlsym_opendir(pathname) ;
    else ret = opendir(pathname) ;

    return ret ;
}

long    fs_low_telldir(DIR * dirptr)
{
    long ret ;

    if (FILESYSTEM_DLSYM)
         ret = dlsym_telldir(dirptr) ;
    else ret = telldir(dirptr) ;

    return ret ;
}

void    fs_low_seekdir(DIR * dirptr, long loc)
{
    if (FILESYSTEM_DLSYM)
         dlsym_seekdir(dirptr, loc) ;
    else seekdir(dirptr, loc) ;
}

struct dirent * fs_low_readdir(DIR * dirptr)
{
    struct dirent * ret ;

    if (FILESYSTEM_DLSYM)
         ret = dlsym_readdir(dirptr) ;
    else ret = readdir(dirptr) ;

    return ret ;
}

int     fs_low_closedir(DIR * dirptr)
{
    int ret ;

    if (FILESYSTEM_DLSYM)
         ret = dlsym_closedir(dirptr) ;
    else ret = closedir(dirptr) ;

    return ret ;
}


  /* ................................................................... */

