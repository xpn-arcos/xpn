/* general header file for the HDF 5 IO in FLASH */


#ifndef _HDF5_FLASH_H
#define _HDF5_FLASH_H

/* set the dimension and grid variables -- the variable N_DIM is set 
   in the compile line */


/* mdim is the maximum dimension -- this is set in tree.fh */
#define MDIM 3

/* 3-d problem */
#if N_DIM == 3 

#define NDIM  3


#define NGID 15

#define k2d 1
#define k3d 1


/* 2-d problem */
#elif N_DIM == 2

#define NDIM  2


#define NGID 9

#define k2d 1
#define k3d 0


/* 1-d problem */
#else

#define NDIM 1

#define NGID 5

#define k2d 0
#define k3d 0

#endif


#endif

