#ifndef _XPN_RW_H
#define _XPN_RW_H

#include "xpn.h"
#include "xpn_file.h"
#include "xpn_open.h"
#include "xpn_policy_rw.h"
#include "xpn_err.h"


 #ifdef  __cplusplus
    extern "C" {
 #endif


ssize_t xpn_read(int fd, void *buffer, size_t size);

ssize_t xpn_write(int fd, const void *buffer, size_t size);

ssize_t xpn_pread(int fd, void *buffer, size_t size, off_t offset);

ssize_t xpn_pwrite(int fd, const void *buffer, size_t size, off_t offset);

off_t xpn_lseek(int fd, off_t offset, int flag);

ssize_t xpn_reader (void *cookie, char *buffer, size_t size);

ssize_t xpn_writer (void *cookie, const char *buffer, size_t size);

//int xpn_seeker (void *cookie, fpos_t *position, int whence);
int xpn_seeker (void *cookie, __off64_t *position, int whence);

int xpn_cleaner (void *cookie);

int xpn_fgetc(FILE *flujo);

char *xpn_fgets(char *s, int tam, FILE *flujo);

int xpn_fgetc(FILE *flujo);

size_t xpn_fread(void *ptr, size_t size, size_t nmemb, FILE *stream);

size_t xpn_fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);

int xpn_fseek(FILE *stream, long offset, int whence);

long xpn_ftell(FILE *stream);

void xpn_rewind(FILE *stream);

int xpn_fflush(FILE *stream);

int xpn_ferror(FILE *stream);

 #ifdef  __cplusplus
     }
 #endif


#endif

