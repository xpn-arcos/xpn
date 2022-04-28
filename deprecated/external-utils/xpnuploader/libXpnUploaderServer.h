#ifndef _LIBXPNUPLOADERERVER_H_
#define _LIBXPNUPLOADERSERVER_H_

#include "soapStub.h"

#ifdef __cplusplus
extern "C" {
#endif

int ns1__upload(struct soap *soap, char *source, char *dest, char *partition, int *result);

#ifdef __cplusplus
}
#endif

#endif // _LIBXPNUPLOADERSERVER_H_
