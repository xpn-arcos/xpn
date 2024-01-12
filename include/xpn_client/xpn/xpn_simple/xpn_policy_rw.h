#ifndef _XPN_POLICY_RW_H
#define _XPN_POLICY_RW_H

#include "xpn_file.h"
#include "xpn_policy_open.h"


 #ifdef  __cplusplus
    extern "C" {
 #endif

int XpnGetBlock(int fd, off_t offset, int replication, off_t *local_offset, int *serv);

void *XpnReadBlocks      (int fd, const void *buffer, size_t size, off_t offset, struct nfi_worker_io ***io_out, int **ion_out, int num_servers);

void *XpnWriteBlocks      (int fd, const void *buffer, size_t size, off_t offset, struct nfi_worker_io ***io_out, int **ion_out, int num_servers);

ssize_t XpnRWGetTotalBytes (int fd, ssize_t *res_v, int num_servers);

 #ifdef  __cplusplus
     }
 #endif


#endif
