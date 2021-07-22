/* File : xpn.i */
%module xpn
/* %import "sys/types.h" */
%{
#include <sys/types.h>
#include <dirent.h>
#include "xpn.h"
%}

/* SWIG needs this information to convert to/from Java and Python ints
   http://www.cs.utah.edu/dept/old/texinfo/glibc-manual-0.02/library_33.html#SEC506 */
typedef unsigned int gid_t;
typedef unsigned int mode_t;
typedef long int     off_t;
typedef signed int   ssize_t;
typedef unsigned int uid_t;

typedef long int __time_t;

struct timespec
  {
    __time_t tv_sec;
    long int tv_nsec;
  };

typedef unsigned long long int __dev_t;
typedef unsigned long int __ino_t;
typedef unsigned int __mode_t;
typedef unsigned int __nlink_t;
typedef unsigned int __uid_t;
typedef unsigned int __gid_t;
typedef long int __off_t;
typedef long int __blksize_t;
typedef long int __blkcnt_t;

struct stat
  {
    __dev_t st_dev;
    unsigned short int __pad1;
    __ino_t st_ino;
    __mode_t st_mode;
    __nlink_t st_nlink;
    __uid_t st_uid;
    __gid_t st_gid;
    __dev_t st_rdev;
    unsigned short int __pad2;
    __off_t st_size;
    __blksize_t st_blksize;
    __blkcnt_t st_blocks;
    struct timespec st_atim;
    struct timespec st_mtim;
    struct timespec st_ctim;
    /*unsigned long int __unused4;*/
    /*unsigned long int __unused5;*/
  };

struct dirent
  {
    __ino_t d_ino;
    __off_t d_off;
    unsigned short int d_reclen;
    unsigned char d_type;
    char d_name[256];
  };

%include "xpn.h"
