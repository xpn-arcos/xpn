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

/**
 * @file xpn.h
 * @brief Header file to 'TODO'.
 *
 * Header file to 'TODO'.
 *
 * @authors Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
 * @date  Jul 22, 2021
 * @bug No known bugs.
 */

#ifndef _XPN_H_
#define _XPN_H_

/************************************************
 *  ... Includes
 ***********************************************/
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <dirent.h>
// #include <sys/uio.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /************************************************
     *  ... Functions
     ***********************************************/

    // xpn_cwd.c
    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param path  'TODO'.
     * @param size  'TODO'.
     * @return 'TODO'.
     */
    char *xpn_getcwd(char *path, size_t size);

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param path  'TODO'.
     * @return 'TODO'.
     */
    int xpn_chdir(char *path);

    // xpn_dir.c
    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param path  'TODO'.
     * @param perm  'TODO'.
     * @return 'TODO'.
     */
    int xpn_mkdir(const char *path, mode_t perm);

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param path  'TODO'.
     * @return 'TODO'.
     */
    int xpn_rmdir(const char *path);

    // xpn_init.c
    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @par Parameters
     *    None.
     * @return 'TODO'.
     */
    int xpn_init(void);

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @par Parameters
     *    None.
     * @return 'TODO'.
     */
    int xpn_destroy(void);

    // xpn_open.c
    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param virtual_path  'TODO'.
     * @param storage_path  'TODO'.
     * @return 'TODO'.
     */
    int xpn_flush(const char *virtual_path, const char *storage_path); // Auxiliary function

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param virtual_path  'TODO'.
     * @param storage_path  'TODO'.
     * @return 'TODO'.
     */
    int xpn_preload(const char *virtual_path, const char *storage_path); // Auxiliary function

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param path  'TODO'.
     * @param perm  'TODO'.
     * @return 'TODO'.
     */
    int xpn_creat(const char *path, mode_t perm);

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param path  'TODO'.
     * @param flags  'TODO'.
     * @return 'TODO'.
     */
    int xpn_open(const char *path, int flags, ...);

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param fd  'TODO'.
     * @return 'TODO'.
     */
    int xpn_close(int fd);

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param path  'TODO'.
     * @return 'TODO'.
     */
    int xpn_unlink(const char *path);

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param path  'TODO'.
     * @param newpath  'TODO'.
     * @return 'TODO'.
     */
    int xpn_rename(const char *path, const char *newpath);

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param fd  'TODO'.
     * @param sb  'TODO'.
     * @return 'TODO'.
     */
    int xpn_fstat(int fd, struct stat *sb);

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param path  'TODO'.
     * @param sb  'TODO'.
     * @return 'TODO'.
     */
    int xpn_stat(const char *path, struct stat *sb);

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param path  'TODO'.
     * @param owner  'TODO'.
     * @param group  'TODO'.
     * @return 'TODO'.
     */
    int xpn_chown(const char *path, uid_t owner, gid_t group);

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param fd  'TODO'.
     * @param owner  'TODO'.
     * @param group  'TODO'.
     * @return 'TODO'.
     */
    int xpn_fchown(int fd, uid_t owner, gid_t group);

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param path  'TODO'.
     * @param mode  'TODO'.
     * @return 'TODO'.
     */
    int xpn_chmod(const char *path, mode_t mode);

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param fd  'TODO'.
     * @param mode  'TODO'.
     * @return 'TODO'.
     */
    int xpn_fchmod(int fd, mode_t mode);

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param path  'TODO'.
     * @param length  'TODO'.
     * @return 'TODO'.
     */
    int xpn_truncate(const char *path, off_t length);

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param fd  'TODO'.
     * @param length  'TODO'.
     * @return 'TODO'.
     */
    int xpn_ftruncate(int fd, off_t length);

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param fd  'TODO'.
     * @return 'TODO'.
     */
    int xpn_dup(int fd);

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param fd  'TODO'.
     * @param fd2  'TODO'.
     * @return 'TODO'.
     */
    int xpn_dup2(int fd, int fd2);

    // xpn_opendir.c
    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param path  'TODO'.
     * @return 'TODO'.
     */
    DIR *xpn_opendir(const char *path);

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param dirp  'TODO'.
     * @return 'TODO'.
     */
    int xpn_closedir(DIR *dirp);

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param dirp  'TODO'.
     * @return 'TODO'.
     */
    struct dirent *xpn_readdir(DIR *dirp);

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param dirp  'TODO'.
     * @par Returns
     *    Nothing.
     */
    void xpn_rewinddir(DIR *dirp);

    // xpn_rw.c
    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param fd  'TODO'.
     * @param buffer  'TODO'.
     * @param size  'TODO'.
     * @return 'TODO'.
     */
    ssize_t xpn_read(int fd, void *buffer, size_t size);

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param fd  'TODO'.
     * @param buffer  'TODO'.
     * @param size  'TODO'.
     * @return 'TODO'.
     */
    ssize_t xpn_write(int fd, const void *buffer, size_t size);

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param fd  'TODO'.
     * @param buffer  'TODO'.
     * @param size  'TODO'.
     * @param offset  'TODO'.
     * @return 'TODO'.
     */
    ssize_t xpn_pread(int fd, void *buffer, size_t size, off_t offset);

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param fd  'TODO'.
     * @param buffer  'TODO'.
     * @param size  'TODO'.
     * @param offset  'TODO'.
     * @return 'TODO'.
     */
    ssize_t xpn_pwrite(int fd, const void *buffer, size_t size, off_t offset);

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param fd  'TODO'.
     * @param offset  'TODO'.
     * @param flag  'TODO'.
     * @return 'TODO'.
     */
    off_t xpn_lseek(int fd, off_t offset, int flag);

    // ssize_t 	xpn_writev	(int fildes, const struct iovec *iov, int iovcnt);
    // ssize_t 	xpn_readv	(int fildes, const struct iovec *iov, int iovcnt);

    /***************/
    /*
    // xpn_f.c
    FILE*		xpn_fopencookie	(const char *path, const char *mode);
    ssize_t		xpn_reader	(void *cookie, char *buffer, size_t size);
    ssize_t		xpn_writer	(void *cookie, const char *buffer, size_t size);
    int		xpn_seeker	(void *cookie, __off64_t *position, int whence);
    int		xpn_cleaner	(void *cookie);
    void		xpn_rewind	(FILE *stream);
    int		xpn_fflush	(FILE *stream);
    int		xpn_ferror	(FILE *stream);
    int		xpn_fgetc	(FILE *flujo);
    char*		xpn_fgets	(char *s, int tam, FILE *flujo);
    */
    /***************/

    // xpn_fopen.c
    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param filename  'TODO'.
     * @param mode  'TODO'.
     * @return 'TODO'.
     */
    FILE *xpn_fopen(const char *filename, const char *mode);

    // xpn_fclose.c
    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param stream  'TODO'.
     * @return 'TODO'.
     */
    int xpn_fclose(FILE *stream);

    // xpn_fread
    // int		xpn_getc	(FILE *stream);

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param ptr  'TODO'.
     * @param size  'TODO'.
     * @param nmemb  'TODO'.
     * @param stream  'TODO'.
     * @return 'TODO'.
     */
    size_t xpn_fread(void *ptr, size_t size, size_t nmemb, FILE *stream);

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param ptr  'TODO'.
     * @param size  'TODO'.
     * @param nmemb  'TODO'.
     * @param stream  'TODO'.
     * @return 'TODO'.
     */
    size_t xpn_fread_prefetch(void *ptr, size_t size, size_t nmemb, FILE *stream);

    // xpn_fwrite.c
    // int		xpn_putc	(int c, FILE *stream);
    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param ptr  'TODO'.
     * @param size  'TODO'.
     * @param nmemb  'TODO'.
     * @param stream  'TODO'.
     * @return 'TODO'.
     */
    size_t xpn_fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);

    // xpn_fflush.c
    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param stream  'TODO'.
     * @return 'TODO'.
     */
    int xpn_fflush(FILE *stream);

    // xpn_fseek.c
    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param stream  'TODO'.
     * @param offset  'TODO'.
     * @param whence  'TODO'.
     * @return 'TODO'.
     */
    int xpn_fseek(FILE *stream, off_t offset, int whence);

    // xpn_ftell.c
    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param stream  'TODO'.
     * @return 'TODO'.
     */
    long xpn_ftell(FILE *stream);

    // xpn_fileno.c
    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param stream  'TODO'.
     * @return 'TODO'.
     */
    int xpn_fileno(FILE *stream);

    // xpn_setbuf.c
    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param stream  'TODO'.
     * @param buf  'TODO'.
     * @par Returns
     *    Nothing.
     */
    void xpn_setbuf(FILE *stream, char *buf);

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param stream  'TODO'.
     * @param buf  'TODO'.
     * @param size  'TODO'.
     * @par Returns
     *    Nothing.
     */
    void xpn_setbuffer(FILE *stream, char *buf, size_t size);

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param stream  'TODO'.
     * @par Returns
     *    Nothing.
     */
    void xpn_setlinebuf(FILE *stream);

    // xpn_setvbuf.c
    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param stream  'TODO'.
     * @param buf  'TODO'.
     * @param mode  'TODO'.
     * @param size  'TODO'.
     * @return 'TODO'.
     */
    int xpn_setvbuf(FILE *stream, char *buf, int mode, size_t size);

#ifdef __cplusplus
}
#endif

#endif
