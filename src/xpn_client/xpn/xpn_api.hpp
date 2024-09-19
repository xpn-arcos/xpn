
/*
 *  Copyright 2020-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Dario Muñoz Muñoz
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

#pragma once

#include <string>
#include <unordered_map>
#include <mutex>
#include <sys/stat.h>

#include <xpn/xpn_partition.hpp>
#include <xpn/xpn_file.hpp>
#include <base_cpp/debug.hpp>
#include <base_cpp/workers.hpp>

#ifdef _REENTRANT

    #define XPN_API_LOCK()    std::unique_lock<std::mutex> lock(XPN::xpn_api::get_instance().m_api_mutex);
    #define XPN_API_UNLOCK()  lock.unlock();

#else

    #define XPN_API_LOCK()    
    #define XPN_API_UNLOCK()  

#endif

namespace XPN
{
    class xpn_api
    {
    public:
        xpn_api() { init(); }
    public:
        static xpn_api& get_instance()
        {
            static xpn_api instance;
            return instance;
        }
    private:
        std::unordered_map<std::string, xpn_partition> m_partitions;
        std::unordered_map<int, xpn_file> m_files;
        std::unique_ptr<workers> m_worker;

        std::mutex m_init_mutex;
        bool m_initialized = false;


    public:
        std::mutex m_api_mutex;

    public:
        // XPN api
        int init();
        int destroy();
        int mark_error_server(int index);
        int get_block_locality(char *path, off_t offset, int *url_c, char **url_v[]);
        int free_block_locality(int *url_c, char **url_v[]);

        // File api
        int   open      (const char *path, int flags, mode_t mode);

        int   creat     (const char *path, mode_t perm);
        int   close     (int fd);
        int   unlink    (const char *path);
        int   rename    (const char *path, const char *newpath);

        // Stat api
        int   fstat     (int fd, struct stat *sb);
        int   stat      (const char *path, struct stat *sb);
        int   chown     (const char *path,  uid_t owner,  gid_t group);
        int   fchown    (int  fd,  uid_t owner,  gid_t group);
        int   chmod     (const char *path,  mode_t mode);
        int   fchmod    (int fd,  mode_t mode);
        int   truncate  (const char *path,  off_t length);
        int   ftruncate (int fd, off_t length);
        int   dup       (int fd);
        int   dup2      (int fd, int fd2);

        // RW api
        ssize_t read    (int fd, void *buffer, size_t size);
        ssize_t write   (int fd, const void *buffer, size_t size);
        off_t   lseek   (int fd, off_t offset, int flag);

        // f_file api
        FILE   *fopen   (const char *filename, const char *mode);
        int     fclose  (FILE *stream);
        size_t  fread   (void *ptr, size_t size, size_t nmemb, FILE *stream);
        size_t  fwrite  (const void *ptr, size_t size, size_t nmemb, FILE *stream);
        int     fseek   (FILE *stream, long offset, int whence);
        long    ftell   (FILE *stream);
        int     fflush  (FILE *stream);
        int     fgetc   (FILE *stream);
        char   *fgets   (char *s, int tam, FILE *stream);
        int     getc    (FILE *stream);
        void    rewind  (FILE *stream);
        int     fileno  (FILE *stream);
        int     ferror  (FILE *stream);

        // DIR api
        DIR *           opendir(const char *path);
        int             closedir(DIR *dirp);
        struct:: dirent*readdir(DIR *dirp);
        void            rewinddir(DIR *dirp);  
        int             mkdir(const char *path, mode_t perm);
        int             rmdir(const char *path);
        
        // cwd api
        char* getcwd(char *path, size_t size);
        int   chdir(char *path);
    };
} // namespace XPN