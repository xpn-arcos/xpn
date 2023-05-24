/*
 *  Copyright 2020-2023 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos
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

/**
 * @file nfi_worker.h
 * @brief Header file to 'TODO'.
 *
 * Header file to 'TODO'.
 *
 * @authors Copyright 2020-2023 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos
 * @date  Jul 22, 2021
 * @bug No known bugs.
 */

#ifndef _NFI_WORKER_H_
#define _NFI_WORKER_H_

/************************************************
 *  ... Includes
 ***********************************************/
#include "nfi_worker_task.h"

#ifdef __cplusplus
extern "C"
{
#endif

   /************************************************
    *  ... Functions
    ***********************************************/

   /**
    * @brief 'TODO'.
    *
    * 'TODO'.
    *
    * @param wrk 'TODO'.
    * @param serv 'TODO'.
    * @param thread 'TODO'.
    * @return 'TODO'.
    */
   int nfi_worker_init(struct nfi_worker *wrk, struct nfi_server *serv, int thread);

   /**
    * @brief 'TODO'.
    *
    * 'TODO'.
    *
    * @param wrk 'TODO'.
    * @param flag 'TODO'.
    * @return 'TODO'.
    */
   int nfi_worker_thread(struct nfi_worker *wrk, int flag);

   /**
    * @brief 'TODO'.
    *
    * 'TODO'.
    *
    * @param wrk 'TODO'.
    * @return 'TODO'.
    */
   ssize_t nfi_worker_wait(struct nfi_worker *wrk);

   /**
    * @brief 'TODO'.
    *
    * 'TODO'.
    *
    * @param wrk 'TODO'.
    * @return 'TODO'.
    */
   int nfi_worker_end(struct nfi_worker *wrk);

   /**
    * @brief 'TODO'.
    *
    * 'TODO'.
    *
    * @par Parameters
    *    None.
    * @return 'TODO'.
    */
   int nfi_worker_destroy();

   // NEW //////////////////////////////////////////

   /**
    * @brief 'TODO'.
    *
    * 'TODO'.
    *
    * @param serv 'TODO'.
    * @return 'TODO'.
    */
   int nfiworker_init(struct nfi_server *serv);

   /**
    * @brief 'TODO'.
    *
    * 'TODO'.
    *
    * @param worker_function 'TODO'.
    * @param wrk 'TODO'.
    * @return 'TODO'.
    */
   int nfiworker_launch(void (*worker_function)(struct st_th), struct nfi_worker *wrk);

   /**
    * @brief 'TODO'.
    *
    * 'TODO'.
    *
    * @param wrk 'TODO'.
    * @return 'TODO'.
    */
   ssize_t nfiworker_wait(struct nfi_worker *wrk);

   /**
    * @brief 'TODO'.
    *
    * 'TODO'.
    *
    * @param serv 'TODO'.
    * @par Returns
    *    Nothing.
    */
   void nfiworker_destroy(struct nfi_server *serv);

#ifdef __cplusplus
}
#endif

#endif
