#include <sys/stat.h>

#include "project/posix.nsmap"

#include "libIOServiceServer.h"

int debug;

int ns1__open(struct soap *soap, char *pathname, int flags, unsigned int mode, int *result) {
	if (debug)
		printf("ns1__open(%s, %o, %o)", pathname, flags, mode);
	
	if ((pathname == NULL) || (strlen(pathname) <= 0))
		return soap_sender_fault(soap, "pathname not valid", "pathname is NULL or has non-positive length");
	
	if (flags < 0)
		return soap_sender_fault(soap, "flags not valid", "flags are negative");
	
	*result = open(pathname, flags, mode);
	
	if (debug)
		printf("=%d\n", *result);
	
	if (*result < 0)
		*result = -errno;
	
	return SOAP_OK;
}

int ns1__close(struct soap *soap, int fd, int *result) {
	if (fd < 0)
		return soap_sender_fault(soap, "file descriptor not valid", "file descriptor is negative");
	
	*result = close(fd);
	
	return SOAP_OK;
}

int ns1__read(struct soap *soap, int fd, unsigned int count, struct ns1__readResponse *readRes) {
	if (debug)
		printf("ns1__read(%d, %d, readRes)", fd, count);
	
	if (fd < 0)
		return soap_sender_fault(soap, "file descriptor not valid", "file descriptor is negative");
	
	if (count < 0)
		return soap_sender_fault(soap, "count not valid", "count is negative");
	
	memset(&(readRes->buf), 0, sizeof(/*struct xsd__base64Binary*/ns__t_buf_param));
	
	if (count == 0) {
		readRes->result = 0;
		return SOAP_OK;
	}
	
	readRes->buf.xop__Include.__ptr = (unsigned char *)soap_malloc(soap, count);
	memset(readRes->buf.xop__Include.__ptr, 0, count);
	readRes->result = read(fd, readRes->buf.xop__Include.__ptr, count);
	readRes->buf.xop__Include.__size = count;
	readRes->buf.xop__Include.id = NULL;
	readRes->buf.xop__Include.type = "application/octet-stream"; // MIME type
	readRes->buf.xop__Include.options = NULL;
	readRes->buf.xmlmime__contentType = "application/octet-stream"; // MIME type
	
	if (debug)
		printf("=%d\n", readRes->result);
	
	return SOAP_OK;
}

int ns1__write(struct soap *soap, int fd, /*struct xsd__base64Binary*/ns__t_buf_param buf, unsigned int count, int *result) {
	if (debug)
		printf("ns1__write(%d, buf, %d)", fd, count);
	
	if (fd < 0)
		return soap_sender_fault(soap, "file descriptor not valid", "file descriptor is negative");
	
	if (buf.xop__Include.__ptr == NULL)
		return soap_sender_fault(soap, "buf not valid", "buf is NULL");
	
	if (count < 0)
		return soap_sender_fault(soap, "count not valid", "count is negative");
	
	if (count != buf.xop__Include.__size)
		return soap_sender_fault(soap, "count or buf not valid", "count and buf.__size mismatch");
	
	*result = write(fd, buf.xop__Include.__ptr, count);
	
	if (debug)
		printf("=%d\n", *result);
	
	return SOAP_OK;
}

int ns1__lseek(struct soap *soap, int fildes, LONG64 offset, int whence, LONG64 *result) {
	if (debug)
		printf("ns1__lseek(%d, %lld, %d)", fildes, offset, whence);
	
	if (fildes < 0)
		return soap_sender_fault(soap, "file descriptor not valid", "file descriptor is negative");
	
	*result = lseek(fildes, offset, whence);
	
	if (debug)
		printf("=%lld\n", *result);
	
	return SOAP_OK;
}

int ns1__stat(struct soap *soap, char *path, struct ns1__statResponse *statRes) {
	struct stat buf;
	
	if (debug)
		printf("ns1__stat(%s, statRes)", path);
	
	if ((path == NULL) || (strlen(path) <= 0))
		return soap_sender_fault(soap, "path not valid", "path is NULL or has non-positive length");
	
	memset(&buf, 0, sizeof(struct stat));
	statRes->result = stat(path, &buf);
	
	if (debug)
		printf("=%d\n", statRes->result);
	
	statRes->buf = (struct ns1__structStat *)soap_malloc(soap, sizeof(struct ns1__structStat));
	memset(statRes->buf, 0, sizeof(struct ns1__structStat));
	
	statRes->buf->st_dev	= buf.st_dev;
	statRes->buf->st_ino	= buf.st_ino;
	statRes->buf->st_mode	= buf.st_mode;
	statRes->buf->st_nlink	= buf.st_nlink;
	statRes->buf->st_uid	= buf.st_uid;
	statRes->buf->st_gid	= buf.st_gid;
	statRes->buf->st_rdev	= buf.st_rdev;
	statRes->buf->st_size	= buf.st_size;
	statRes->buf->st_blksize= buf.st_blksize;
	statRes->buf->st_blocks	= buf.st_blocks;
	statRes->buf->st_atim	= buf.st_atime;
	statRes->buf->st_mtim	= buf.st_mtime;
	statRes->buf->st_ctim	= buf.st_ctime;
	
	return SOAP_OK;
}
