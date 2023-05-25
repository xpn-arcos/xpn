/*
 *  Copyright 2020-2023 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
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
 * @file math_misc.c
 * @brief File to 'TODO'.
 *
 * File to 'TODO'.
 *
 * @authors Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
 * @date  Jul 22, 2021
 * @bug No known bugs.
 */

/************************************************
 *  ... Includes
 ***********************************************/
#include "base/math_misc.h"

/************************************************
 *  ... Functions
 ***********************************************/

int MATH_MISC_hash(
    char *file,
    int nServ)
{
     int i, max;
     long unsigned num;

     /*
      * Check params...
      */
     if (NULL == file)
          return (-1);

     /*
      * Compute hash...
      */
     num = 0;
     max = strlen(file) - 1;
     for (i = 0; i < max; i++)
     {
          num = (int)file[i] + num;
     }

     /*
      * Return index
      */
     return (int)num % nServ;
}

int MATH_MISC_locateInRAID5withInternalParity(
    int i,   /* block index */
    int m,   /* how many servers */
    int *SP, /* Server for Parity */
    int *IP, /* Index for Parity */
    int *SD, /* Server for Data */
    int *ID  /* Index for Data */
)
{
     int Reorder;
     int aux1, aux2;

     /*
      * Check params...
      */
     if (i < 0)
          return (-1);
     if (m < 2)
          return (-1);

     /*
      * Compute indexes...
      */
     aux1 = i / (m - 1);
     aux2 = i % (m - 1);

     (*SP) = (m - 1) - (aux1 % m);
     (*IP) = aux1;

     Reorder = ((*SP) <= aux2) ? 1 : 0;

     (*SD) = aux2 + Reorder;
     (*ID) = aux1;

     /*
      * Return ok
      */
     return (1);
}

int MATH_MISC_locateInRAID5withExternalParity(
    int i,   /* block index */
    int m,   /* how many servers */
    int *SP, /* Server for Parity */
    int *IP, /* Index for Parity */
    int *SD, /* Server for Data */
    int *ID  /* Index for Data */
)
{
     int aux1;

     /*
      * Check params...
      */
     if (i < 0)
          return (-1);
     if (m < 0)
          return (-1);
     if (0 == m)
          return (-1);

     /*
      * Compute indexes...
      */
     aux1 = i / (m - 1);

     (*SP) = (m - 1) - (aux1 % m);
     (*IP) = aux1 / m;
     (*SD) = i % m;
     (*ID) = i / m;

     /*
      * Return ok
      */
     return (1);
}

int MATH_MISC_Xor(
    char *block_result,
    char *block_1,
    char *block_2,
    int block_size)
{
     int i;

     /*
      * XOR
      */
     for (i = 0; i < block_size; i++)
     {
          block_result[i] = block_1[i] ^ block_2[i];
     }

     /*
      * Return ok
      */
     return (1);
}

int MATH_MISC_Xor3(
    char *block_result,
    char *block_1,
    char *block_2,
    char *block_3,
    int block_size)
{
     int i;

     /*
      * XOR
      */
     for (i = 0; i < block_size; i++)
     {
          block_result[i] = block_1[i] ^ block_2[i] ^ block_3[i];
     }

     /*
      * Return ok
      */
     return (1);
}

int MATH_MISC_XorN(
    char *block_result,
    char **blocks,
    int nblocks,
    int block_size)
{
     int i, j;

     /*
      * XOR
      */
     for (i = 0; i < block_size; i++)
     {
          block_result[i] = blocks[0][i];
          for (j = 1; j < nblocks; j++)
          {
               block_result[i] = block_result[i] ^ blocks[j][i];
          }
     }

     /*
      * Return ok
      */
     return (1);
}
