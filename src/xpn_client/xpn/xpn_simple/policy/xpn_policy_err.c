#ifndef _XPN_ERR_H_
#define _XPN_ERR_H_

#include "xpn.h"


extern int errno;
extern int xpn_errno;

int xpn_err(int err);

#endif
