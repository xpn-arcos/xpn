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
 * @file xpn_rw.h
 * @brief Header file to 'TODO'.
 *
 * Header file to 'TODO'.
 *
 * @authors Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
 * @date  Jul 22, 2021
 * @bug No known bugs.
 */

#ifndef _XPN_RW_H
#define _XPN_RW_H

/************************************************
 *  ... Includes
 ***********************************************/
#include "xpn.h"
#include "xpn_file.h"
#include "xpn_open.h"
#include "xpn_policy_rw.h"
#include "xpn_err.h"
#include "base/workers.h"

#ifdef __cplusplus
extern "C"
{
#endif

/************************************************
 *  ... Functions
 ***********************************************/

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param fd 'TODO'.
     * @param buffer 'TODO'.
     * @param size 'TODO'.
     * @return 'TODO'.
     */
    ssize_t xpn_simple_read(int fd, void *buffer, size_t size);

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param fd 'TODO'.
     * @param buffer 'TODO'.
     * @param size 'TODO'.
     * @return 'TODO'.
     */
    ssize_t xpn_simple_write(int fd, const void *buffer, size_t size);

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param fd 'TODO'.
     * @param offset 'TODO'.
     * @param flag 'TODO'.
     * @return 'TODO'.
     */
    off_t xpn_simple_lseek(int fd, off_t offset, int flag);

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param filename 'TODO'.
     * @param mode 'TODO'.
     * @return 'TODO'.
     */
    FILE *xpn_simple_fopen(const char *filename, const char *mode);

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param stream 'TODO'.
     * @return 'TODO'.
     */
    int xpn_simple_fclose(FILE *stream);

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param ptr 'TODO'.
     * @param size 'TODO'.
     * @param nmemb 'TODO'.
     * @param stream 'TODO'.
     * @return 'TODO'.
     */
    size_t xpn_simple_fread(void *ptr, size_t size, size_t nmemb, FILE *stream);

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param ptr 'TODO'.
     * @param size 'TODO'.
     * @param nmemb 'TODO'.
     * @param stream 'TODO'.
     * @return 'TODO'.
     */
    size_t xpn_simple_fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param stream 'TODO'.
     * @param offset 'TODO'.
     * @param whence 'TODO'.
     * @return 'TODO'.
     */
    int xpn_simple_fseek(FILE *stream, long offset, int whence);

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param stream 'TODO'.
     * @return 'TODO'.
     */
    long xpn_simple_ftell(FILE *stream);

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param stream 'TODO'.
     * @return 'TODO'.
     */
    int xpn_simple_fflush(FILE *stream);

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param fd 'TODO'.
     * @param buffer 'TODO'.
     * @param size 'TODO'.
     * @param offset 'TODO'.
     * @return 'TODO'.
     */
    ssize_t xpn_sread(int fd, const void *buffer, size_t size, off_t offset);

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param fd 'TODO'.
     * @param buffer 'TODO'.
     * @param size 'TODO'.
     * @param offset 'TODO'.
     * @return 'TODO'.
     */
    ssize_t xpn_pread(int fd, void *buffer, size_t size, off_t offset);

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param fd 'TODO'.
     * @param buffer 'TODO'.
     * @param size 'TODO'.
     * @param offset 'TODO'.
     * @return 'TODO'.
     */
    ssize_t xpn_swrite(int fd, const void *buffer, size_t size, off_t offset);

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param fd 'TODO'.
     * @param buffer 'TODO'.
     * @param size 'TODO'.
     * @param offset 'TODO'.
     * @return 'TODO'.
     */
    ssize_t xpn_pwrite(int fd, const void *buffer, size_t size, off_t offset);

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param cookie 'TODO'.
     * @param buffer 'TODO'.
     * @param size 'TODO'.
     * @return 'TODO'.
     */
    ssize_t xpn_reader(void *cookie, char *buffer, size_t size);

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param cookie 'TODO'.
     * @param buffer 'TODO'.
     * @param size 'TODO'.
     * @return 'TODO'.
     */
    ssize_t xpn_writer(void *cookie, const char *buffer, size_t size);
    // int xpn_seeker (void *cookie, fpos_t *position, int whence);

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param cookie 'TODO'.
     * @param position 'TODO'.
     * @param whence 'TODO'.
     * @return 'TODO'.
     */
    int xpn_seeker(void *cookie, __off64_t *position, int whence);

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param cookie 'TODO'.
     * @return 'TODO'.
     */
    int xpn_cleaner(void *cookie);

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param flujo 'TODO'.
     * @return 'TODO'.
     */
    int xpn_fgetc(FILE *flujo);

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param s 'TODO'.
     * @param tam 'TODO'.
     * @param flujo 'TODO'.
     * @return 'TODO'.
     */
    char *xpn_fgets(char *s, int tam, FILE *flujo);

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param flujo 'TODO'.
     * @return 'TODO'.
     */
    int xpn_fgetc(FILE *flujo);

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param stream 'TODO'.
     * @par Returns
     *    Nothing.
     */
    void xpn_rewind(FILE *stream);

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param stream 'TODO'.
     * @return 'TODO'.
     */
    int xpn_ferror(FILE *stream);

#ifdef __cplusplus
}
#endif

#endif
