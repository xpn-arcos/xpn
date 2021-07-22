#ifndef _LIBIOSERVICESERVER_H_
#define _LIBIOSERVICESERVER_H_

#include "project/soapStub.h"

#ifdef __cplusplus
extern "C" {
#endif

int ns1__open(struct soap *soap, char *pathname, int flags, unsigned int mode, int *result);

int ns1__close(struct soap *soap, int fd, int *result);

int ns1__read(struct soap *soap, int fd, unsigned int count, struct ns1__readResponse *readRes);

int ns1__write(struct soap *soap, int fd, /*struct xsd__base64Binary*/ns__t_buf_param buf, unsigned int count, int *result);

int ns1__lseek(struct soap *soap, int fildes, LONG64 offset, int whence, LONG64 *result);

int ns1__stat(struct soap *soap, char *path, struct ns1__statResponse *statRes);

#ifdef __cplusplus
}
#endif

#endif // _LIBIOSERVICESERVER_H_
