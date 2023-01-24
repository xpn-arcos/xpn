#ifndef _NFI_WORKER_H_
#define _NFI_WORKER_H_


#include "nfi_worker_task.h"


 #ifdef  __cplusplus
    extern "C" {
 #endif


    int     nfi_worker_init    ( struct nfi_worker *wrk, struct nfi_server *serv, int thread );
    int     nfi_worker_thread  ( struct nfi_worker *wrk, int flag );
    ssize_t nfi_worker_wait    ( struct nfi_worker *wrk );
    int     nfi_worker_end     ( struct nfi_worker *wrk );
    int     nfi_worker_destroy ( );


    // NEW //////////////////////////////////////////
    int     nfiworker_launch ( void (*worker_function)(struct st_th), struct nfi_worker *wrk ) ;
    ssize_t nfiworker_wait   ( struct nfi_worker *wrk ) ;


 #ifdef  __cplusplus
    }
 #endif


#endif

