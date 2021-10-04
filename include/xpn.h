#ifndef _XPN_H_
#define _XPN_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <dirent.h>
//#include <sys/uio.h>

#ifdef  __cplusplus
extern "C" {
#endif

// xpn_cwd.c
char* 		xpn_getcwd	(char *path, size_t size);
int		xpn_chdir	(char *path);

// xpn_dir.c
int 		xpn_mkdir	(const char *path, mode_t perm) ;
int 		xpn_rmdir	(const char *path) ;

// xpn_init.c
int 		xpn_init();
int 		xpn_destroy();

// xpn_open.c
//int 		xpn_flush	(char *virtual_path, char *storage_path); // Auxiliary function
//int 		xpn_preload	(char *virtual_path, char *storage_path); // Auxiliary function
int 		xpn_creat	(const char *path, mode_t perm);
int 		xpn_open	(const char *path, int flags , ...);
int 		xpn_close	(int fd);
int 		xpn_unlink	(const char *path);
int 		xpn_rename	(const char *path, const char *newpath);
int 		xpn_fstat	(int fd, struct stat *sb);
int 		xpn_stat	(const char *path, struct stat *sb);
int 		xpn_chown	(const char *path, uid_t owner, gid_t group);
int 		xpn_fchown	(int fd, uid_t owner, gid_t group);
int 		xpn_chmod	(const char *path, mode_t mode);
int 		xpn_fchmod	(int fd, mode_t mode);
int 		xpn_truncate	(const char *path, off_t length);
int 		xpn_ftruncate	(int fd, off_t length);
int 		xpn_dup		(int fd);
int 		xpn_dup2	(int fd, int fd2);

// xpn_opendir.c
DIR* 		xpn_opendir	(const char *path);
int 		xpn_closedir	(DIR *dirp);
struct dirent* 	xpn_readdir	(DIR *dirp);
void 		xpn_rewinddir	(DIR *dirp);

// xpn_rw.c
ssize_t 	xpn_read	(int fd, void *buffer, size_t size);
ssize_t 	xpn_write	(int fd, const void *buffer, size_t size);
ssize_t 	xpn_pread	(int fd, void *buffer, size_t size, off_t offset);
ssize_t 	xpn_pwrite	(int fd, const void *buffer, size_t size, off_t offset);
off_t 		xpn_lseek	(int fd, off_t offset, int flag);

//ssize_t 	xpn_writev	(int fildes, const struct iovec *iov, int iovcnt);
//ssize_t 	xpn_readv	(int fildes, const struct iovec *iov, int iovcnt);

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
FILE*		xpn_fopen	(const char *filename, const char *mode);

// xpn_fclose.c
int		xpn_fclose	(FILE *stream);

// xpn_fread
//int		xpn_getc	(FILE *stream);
size_t		xpn_fread	(void *ptr, size_t size, size_t nmemb, FILE *stream);
size_t		xpn_fread_prefetch	(void *ptr, size_t size, size_t nmemb, FILE *stream);

// xpn_fwrite.c
//int		xpn_putc	(int c, FILE *stream);
size_t		xpn_fwrite	(const void *ptr, size_t size, size_t nmemb, FILE *stream);

// xpn_fflush.c
int		xpn_fflush	(FILE *stream);

// xpn_fseek.c
int		xpn_fseek	(FILE *stream, long offset, int whence);

// xpn_ftell.c
long		xpn_ftell	(FILE *stream);

// xpn_fileno.c
int		xpn_fileno	(FILE *stream);

// xpn_setbuf.c
void		xpn_setbuf	(FILE *stream, char *buf);
void		xpn_setbuffer	(FILE *stream, char *buf, size_t size);
void		xpn_setlinebuf	(FILE *stream);

// xpn_setvbuf.c
int		xpn_setvbuf	(FILE *stream, char *buf, int mode, size_t size);

#ifdef  __cplusplus
}
#endif

#endif
