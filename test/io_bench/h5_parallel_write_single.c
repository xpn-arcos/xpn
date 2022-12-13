/* This file contains the functions that write the data to the HDF5 file
 * The functions accept the PARAMESH data through arguments, since C cannot
 * handle common blocks 
 */

/* if the following flag is defined, status checking on the writes will
   be performed, and the results will be output to stdout */

/* #define DEBUG_IO */


#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <hdf5.h>
#include "hdf5_flash.h"
#include <mpi.h>


  /* eslint-disable no-use-before-define */
  /* eslint-disable no-unused-vars */
  /* eslint-disable block-scoped-var */


/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx */

void h5_write_header_info_sp_(int* MyPE,
			      int* nvar_out,            /* num vars to store */
			      hid_t* file_identifier,   /* file handle */
                              char file_creation_time[],/* time / date stamp */
			      char flash_version[],     /* FLASH version num */
                              char run_comment[],       /* runtime comment */
                              int* total_blocks,        /* total # of blocks */
                              float* time,              /* simulation time */
                              float* timestep,          /* current timestep */
                              int* nsteps,              /* # of timestep */
                              int nzones_block[3],      /* nxb, nyb, nzb */
                              char unk_labels[][5])     /* unknown labels */
{
  hid_t dataspace, dataset;
  herr_t status;

  int rank;
  hsize_t dimens_1d, dimens_2d[2];

  int string_size;

  hid_t string_type, sp_type;

  typedef struct sim_params_t {
    int total_blocks;
    int nsteps;
    int nxb;
    int nyb;
    int nzb;
    float time; 
    float timestep;

  } sim_params_t;

  sim_params_t sim_params;

  /* file creation time */
  rank = 1;
  dimens_1d = 1;

  /* manually set the string size, and make it null terminated */
  string_size = 40;

  /* setup the datatype for this string length */
  string_type = H5Tcopy(H5T_C_S1);
  H5Tset_size(string_type, string_size);
  
  dataspace = H5Screate_simple(rank, &dimens_1d, NULL);
  dataset   = H5Dcreate(*file_identifier, "file creation time", 
                        string_type, dataspace, H5P_DEFAULT);

  if (*MyPE == 0) {
    status    = H5Dwrite(dataset, string_type, H5S_ALL, H5S_ALL, 
			 H5P_DEFAULT, file_creation_time);
#ifdef DEBUG_IO
    printf("MyPE = %d, wrote file creation time, status = %d\n",
           *MyPE, (int) status);
#endif

  }

  H5Tclose(string_type);
  H5Sclose(dataspace);
  H5Dclose(dataset);


  /* FLASH version */

  /* manually set the string size, and make it null terminated */
  string_size = 20;

  /* setup the datatype for this string length */
  string_type = H5Tcopy(H5T_C_S1);
  H5Tset_size(string_type, string_size);
  
  dataspace = H5Screate_simple(rank, &dimens_1d, NULL);
  dataset   = H5Dcreate(*file_identifier, "FLASH version", 
                        string_type, dataspace, H5P_DEFAULT);

  if (*MyPE == 0) {
    status    = H5Dwrite(dataset, string_type, H5S_ALL, H5S_ALL, 
                         H5P_DEFAULT, flash_version);

#ifdef DEBUG_IO
    printf("wrote flash version, status = %d\n", (int) status);
#endif

  }
  H5Tclose(string_type);
  H5Sclose(dataspace);
  H5Dclose(dataset);


  /*-----------------------------------------------------------------------
     create a compound datatype to hold the simulation parameters -- this
     will cut down on the number of writes issued, and should improve
     performance 
   ------------------------------------------------------------------------- */

  sim_params.total_blocks = *total_blocks;
  sim_params.time = *time; 
  sim_params.timestep = *timestep; 
  sim_params.nsteps = *nsteps;

  sim_params.nxb = nzones_block[0];
  sim_params.nyb = nzones_block[1];
  sim_params.nzb = nzones_block[2]; 

  rank = 1;
  dimens_1d = 1;

  rank = 1;
  dimens_1d = 1;

  dataspace = H5Screate_simple(rank, &dimens_1d, NULL);

  sp_type = H5Tcreate(H5T_COMPOUND, sizeof(sim_params_t));

  H5Tinsert(sp_type, 
	    "total blocks", 
	    offsetof(sim_params_t, total_blocks),
	    H5T_NATIVE_INT);
 
  H5Tinsert(sp_type,
	    "time",
	    offsetof(sim_params_t, time),
	    H5T_NATIVE_FLOAT);
  
  H5Tinsert(sp_type,
	    "timestep",
	    offsetof(sim_params_t, timestep),
	    H5T_NATIVE_FLOAT);

  H5Tinsert(sp_type,
	    "number of steps",
	    offsetof(sim_params_t, nsteps),
	    H5T_NATIVE_INT);

  H5Tinsert(sp_type,
	    "nxb",
	    offsetof(sim_params_t, nxb),
	    H5T_NATIVE_INT);

  H5Tinsert(sp_type,
	    "nyb",
	    offsetof(sim_params_t, nyb),
	    H5T_NATIVE_INT);
  
  H5Tinsert(sp_type,
	    "nzb",
	    offsetof(sim_params_t, nzb),
	    H5T_NATIVE_INT);

  dataset = H5Dcreate(*file_identifier, "simulation parameters", sp_type,
		      dataspace, H5P_DEFAULT);

  if (*MyPE == 0) {
    status = H5Dwrite(dataset, sp_type, H5S_ALL, H5S_ALL,
		      H5P_DEFAULT, &sim_params);
  }

  H5Sclose(dataspace);
  H5Dclose(dataset);
  H5Tclose(sp_type);



  /* unknown names */
  rank = 2;
  dimens_2d[0] = (hsize_t) *nvar_out;
  dimens_2d[1] = 1;

  /* manually set the string size */
  string_size = 4;
   
  /* setup the datatype for this string length */
  string_type = H5Tcopy(H5T_C_S1);
  status = H5Tset_size(string_type, string_size);

#ifdef DEBUG_IO
  printf("MyPE = %d, string type = %d, set size status = %d\n",
         *MyPE, (int) string_type, (int) status);
#endif

  dataspace = H5Screate_simple(rank, dimens_2d, NULL);
  dataset   = H5Dcreate(*file_identifier, "unknown names", 
                        string_type, dataspace, H5P_DEFAULT);

  if (*MyPE == 0) {
    status    = H5Dwrite(dataset, string_type, H5S_ALL, H5S_ALL, 
			 H5P_DEFAULT, unk_labels);
  }
  
  H5Tclose(string_type);
  H5Sclose(dataspace);
  H5Dclose(dataset);

}


/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx */

void h5_write_coord_sp_(hid_t* file_identifier,
		        int* maximum_blocks,
		        int* coordinates,
		        int* local_blocks,
		        int* total_blocks,
		        int* global_offset)
{
  hid_t dataspace, dataset, memspace;
  herr_t status;

  int rank;
  hsize_t dimens_2d[2];

  hssize_t start_2d[2];
  hsize_t stride_2d[2], count_2d[2];

  int ierr;

  /* set the dimensions of the dataset */
  rank = 2;
  dimens_2d[0] = *total_blocks;
  dimens_2d[1] = NDIM;
  
  dataspace = H5Screate_simple(rank, dimens_2d, NULL);

  /* create the dataset */
  dataset = H5Dcreate(*file_identifier, "coordinates", H5T_NATIVE_FLOAT,
		      dataspace, H5P_DEFAULT);

  /* create the hyperslab -- this will differ on the different processors */
  start_2d[0] = (hssize_t) (*global_offset);
  start_2d[1] = 0;

  stride_2d[0] = 1;
  stride_2d[1] = 1;

  count_2d[0] = (hssize_t) (*local_blocks);
  count_2d[1] = NDIM;

  status = H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, start_2d, 
			      stride_2d, count_2d, NULL);

  /* create the memory space */
  rank = 2;
  dimens_2d[0] = *maximum_blocks;
  dimens_2d[1] = MDIM;

  memspace = H5Screate_simple(rank, dimens_2d, NULL);

  start_2d[0] = 0;
  start_2d[1] = 0;
  
  stride_2d[0] = 1;
  stride_2d[1] = 1;
  
  count_2d[0] = *local_blocks;
  count_2d[1] = NDIM;

  ierr = H5Sselect_hyperslab(memspace, H5S_SELECT_SET,
                             start_2d, stride_2d, count_2d, NULL);



  /* write the data */
  status = H5Dwrite(dataset, H5T_NATIVE_FLOAT, memspace, dataspace, 
		    H5P_DEFAULT, coordinates);

  H5Sclose(memspace); 
  H5Sclose(dataspace);
  H5Dclose(dataset);

}


/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx */

void h5_write_size_sp_(hid_t* file_identifier,
		       int* maximum_blocks,
		       int* size,
		       int* local_blocks,
		       int* total_blocks,
		       int* global_offset)
{
  hid_t dataspace, dataset, memspace;
  herr_t status;

  int rank;
  hsize_t dimens_2d[2];

  hssize_t start_2d[2];
  hsize_t stride_2d[2], count_2d[2];

  int ierr;

  /* set the dimensions of the dataset */
  rank = 2;
  dimens_2d[0] = *total_blocks;
  dimens_2d[1] = NDIM;
  
  dataspace = H5Screate_simple(rank, dimens_2d, NULL);

  /* create the dataset */
  dataset = H5Dcreate(*file_identifier, "block size", H5T_NATIVE_FLOAT,
		      dataspace, H5P_DEFAULT);

  /* create the hyperslab -- this will differ on the different processors */
  start_2d[0] = (hssize_t) (*global_offset);
  start_2d[1] = 0;

  stride_2d[0] = 1;
  stride_2d[1] = 1;

  count_2d[0] = (hssize_t) (*local_blocks);
  count_2d[1] = NDIM;

  status = H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, start_2d, 
			      stride_2d, count_2d, NULL);


  /* create the memory space */
  rank = 2;
  dimens_2d[0] = *maximum_blocks;
  dimens_2d[1] = MDIM;

  memspace = H5Screate_simple(rank, dimens_2d, NULL);

  start_2d[0] = 0;
  start_2d[1] = 0;
  
  stride_2d[0] = 1;
  stride_2d[1] = 1;
  
  count_2d[0] = *local_blocks;
  count_2d[1] = NDIM;

  ierr = H5Sselect_hyperslab(memspace, H5S_SELECT_SET,
                             start_2d, stride_2d, count_2d, NULL);

  /* write the data */
  status = H5Dwrite(dataset, H5T_NATIVE_FLOAT, memspace, dataspace, 
		    H5P_DEFAULT, size);

  H5Sclose(memspace); 
  H5Sclose(dataspace);
  H5Dclose(dataset);

}


/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx */

void h5_write_bnd_box_sp_(hid_t* file_identifier,
			  int* maximum_blocks,
			  int* bnd_box,
			  int* local_blocks,
			  int* total_blocks,
			  int* global_offset)
{
  hid_t dataspace, dataset, memspace;
  herr_t status;

  int rank;
  hsize_t dimens_3d[3];

  hssize_t start_3d[3];
  hsize_t stride_3d[3], count_3d[3];

  int ierr;

  /* set the dimensions of the dataset */
  rank = 3;
  dimens_3d[0] = *total_blocks;
  dimens_3d[1] = NDIM;
  dimens_3d[2] = 2;

  dataspace = H5Screate_simple(rank, dimens_3d, NULL);

  /* create the dataset */
  dataset = H5Dcreate(*file_identifier, "bounding box", 
		      H5T_NATIVE_FLOAT, dataspace, H5P_DEFAULT);

  /* create the hyperslab -- this will differ on the different processors */
  start_3d[0] = (hssize_t) (*global_offset);
  start_3d[1] = 0;
  start_3d[2] = 0;

  stride_3d[0] = 1;
  stride_3d[1] = 1;
  stride_3d[2] = 1;

  count_3d[0] = (hssize_t) (*local_blocks);
  count_3d[1] = NDIM;
  count_3d[2] = 2;

  status = H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, start_3d, 
			      stride_3d, count_3d, NULL);

  /* create the memory space */
  rank = 3;
  dimens_3d[0] = *maximum_blocks;
  dimens_3d[1] = MDIM;
  dimens_3d[2] = 2;

  memspace = H5Screate_simple(rank, dimens_3d, NULL);

  start_3d[0] = 0;
  start_3d[1] = 0;
  start_3d[2] = 0;

  stride_3d[0] = 1;
  stride_3d[1] = 1;
  stride_3d[2] = 1;

  count_3d[0] = *local_blocks;
  count_3d[1] = NDIM;
  count_3d[2] = 2;

  ierr = H5Sselect_hyperslab(memspace, H5S_SELECT_SET,
                             start_3d, stride_3d, count_3d, NULL);

  /* write the data */
  status = H5Dwrite(dataset, H5T_NATIVE_FLOAT, memspace, dataspace, 
		    H5P_DEFAULT, bnd_box);

  H5Sclose(memspace); 
  H5Sclose(dataspace);
  H5Dclose(dataset);

}


/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx */

void h5_write_bnd_box_min_sp_(hid_t* file_identifier,
			      int* maximum_blocks,
			      int* bnd_box,
			      int* local_blocks,
			      int* total_blocks,
			      int* global_offset)
{
  hid_t dataspace, dataset, memspace;
  herr_t status;

  int rank;
  hsize_t dimens_2d[2], dimens_3d[3];

  hssize_t start_2d[2], start_3d[3];
  hsize_t stride_2d[2], stride_3d[3], count_2d[2], count_3d[3];

  int ierr;

  /* set the dimensions of the dataset */
  rank = 2;
  dimens_2d[0] = *total_blocks;
  dimens_2d[1] = NDIM;
  
  dataspace = H5Screate_simple(rank, dimens_2d, NULL);

  /* create the dataset */
  dataset = H5Dcreate(*file_identifier, "bounding box minimum", 
		      H5T_NATIVE_FLOAT, dataspace, H5P_DEFAULT);

  /* create the hyperslab -- this will differ on the different processors */
  start_2d[0] = (hssize_t) (*global_offset);
  start_2d[1] = 0;

  stride_2d[0] = 1;
  stride_2d[1] = 1;

  count_2d[0] = (hssize_t) (*local_blocks);
  count_2d[1] = NDIM;

  status = H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, start_2d, 
			      stride_2d, count_2d, NULL);

  /* create the memory space */
  rank = 3;
  dimens_3d[0] = *maximum_blocks;
  dimens_3d[1] = MDIM;
  dimens_3d[2] = 2;

  memspace = H5Screate_simple(rank, dimens_3d, NULL);

  start_3d[0] = 0;
  start_3d[1] = 0;
  start_3d[2] = 0;

  stride_3d[0] = 1;
  stride_3d[1] = 1;
  stride_3d[2] = 1;

  count_3d[0] = *local_blocks;
  count_3d[1] = NDIM;
  count_3d[2] = 1;

  ierr = H5Sselect_hyperslab(memspace, H5S_SELECT_SET,
                             start_3d, stride_3d, count_3d, NULL);

  /* write the data */
  status = H5Dwrite(dataset, H5T_NATIVE_FLOAT, memspace, dataspace, 
		    H5P_DEFAULT, bnd_box);

  H5Sclose(memspace); 
  H5Sclose(dataspace);
  H5Dclose(dataset);

}


/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx */

void h5_write_bnd_box_max_sp_(hid_t* file_identifier,
			      int* maximum_blocks,
			      int*  bnd_box,
			      int* local_blocks,
			      int* total_blocks,
			      int* global_offset)
{
  hid_t dataspace, dataset, memspace;
  herr_t status;

  int rank;
  hsize_t dimens_2d[2], dimens_3d[3];

  hssize_t start_2d[2], start_3d[3];
  hsize_t stride_2d[2], stride_3d[3], count_2d[2], count_3d[3];

  int ierr;

  /* set the dimensions of the dataset */
  rank = 2;
  dimens_2d[0] = *total_blocks;
  dimens_2d[1] = NDIM;
  
  dataspace = H5Screate_simple(rank, dimens_2d, NULL);

  /* create the dataset */
  dataset = H5Dcreate(*file_identifier, "bounding box maximum", 
		      H5T_NATIVE_FLOAT, dataspace, H5P_DEFAULT);

  /* create the hyperslab -- this will differ on the different processors */
  start_2d[0] = (hssize_t) (*global_offset);
  start_2d[1] = 0;

  stride_2d[0] = 1;
  stride_2d[1] = 1;

  count_2d[0] = (hssize_t) (*local_blocks);
  count_2d[1] = NDIM;

  status = H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, start_2d, 
			      stride_2d, count_2d, NULL);

  /* create the memory space */
  rank = 3;
  dimens_3d[0] = *maximum_blocks;
  dimens_3d[1] = MDIM;
  dimens_3d[2] = 2;

  memspace = H5Screate_simple(rank, dimens_3d, NULL);

  start_3d[0] = 0;
  start_3d[1] = 0;
  start_3d[2] = 1;  /* we want the maximum */

  stride_3d[0] = 1;
  stride_3d[1] = 1;
  stride_3d[2] = 1;

  count_3d[0] = *local_blocks;
  count_3d[1] = NDIM;
  count_3d[2] = 1;

  ierr = H5Sselect_hyperslab(memspace, H5S_SELECT_SET,
                             start_3d, stride_3d, count_3d, NULL);
 
  /* write the data */
  status = H5Dwrite(dataset, H5T_NATIVE_FLOAT, memspace, dataspace, 
		    H5P_DEFAULT, bnd_box);

  H5Sclose(memspace); 
  H5Sclose(dataspace);
  H5Dclose(dataset);

}


/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx */

/* 
   This function writes out a single unknown (passed from the checkpoint 
   routine), giving the record a label from the varnames or species
   database 
   
   The dimensions of the unknowns array (nvar, nxb, nyb, nzb, maxblocks)
   are passed through as arguments.  The dataspace (what gets written
   to disk) and the memory space (how the unknowns array is accessed in
   local memory) are defined based on these passed values.  This allows
   use to pass an unk array that has all the guardcells + interior cells
   (as in the checkpointing), or a dummy unk array that has just the 
   interior cells (in this case, nguard would be passed as 0).
*/

void h5_write_unknowns_sp_(hid_t* file_identifier,
			   int* index,          /* index of var to write */
			   int* nvar,           /* total number of variables */
			   int* nxb,            /* # of zones to store in x */
			   int* nyb,            /* # of zones to store in y */
			   int* nzb,            /* # of zones to store in z */
			   int* nguard,         /* # of guardcells in pass */ 
			   int* maximum_blocks, /* maximum num of blocks */
			   float* unknowns,   /* [mblk][NZB][NYB][NXB][nvar] */
			   char record_label[5],/* add char-null termination */
			   int* local_blocks,  
			   int* total_blocks,
			   int* global_offset)
{
  hid_t dataspace, dataset, memspace, dxfer_template, dataset_plist;
  herr_t status;

  int rank;
  hsize_t dimens_4d[4], dimens_5d[5];

  hssize_t start_4d[4];
  hsize_t stride_4d[4], count_4d[4];

#ifdef CHUNK
  hsize_t dimens_chunk[4];
#endif

  char record_label_new[5];

  int ierr;

  /* 
     the variable names are 4 characters long -- copy this into 
     record_label_new, the 5th character is for the \0 termination 
  */
  strncpy(record_label_new, record_label,4);
  *(record_label_new + 4) = '\0';

  /* set the dimensions of the dataset */
  rank = 4;
  dimens_4d[0] = *total_blocks;
  dimens_4d[1] = *nzb;
  dimens_4d[2] = *nyb;
  dimens_4d[3] = *nxb;
  
  dataspace = H5Screate_simple(rank, dimens_4d, NULL);

#ifdef DEBUG_IO
  printf("UNKNOWNS: dataspace = %d\n", (int) dataspace);
#endif

  dataset_plist = H5Pcreate(H5P_DATASET_CREATE);

#ifdef CHUNK
  /* set the layout to chunked */

  ierr = H5Pset_layout(dataset_plist, H5D_CHUNKED);
  
  /* create a chunk containing 10 blocks worth of data */
  dimens_chunk[0] = 10;
  dimens_chunk[1] = *nzb;
  dimens_chunk[2] = *nyb;
  dimens_chunk[3] = *nxb;
   
  ierr = H5Pset_chunk(dataset_plist, 4, dimens_chunk);
#endif

  dataset = H5Dcreate(*file_identifier, record_label_new,
                      H5T_NATIVE_FLOAT, dataspace, dataset_plist); 
  

#ifdef DEBUG_IO
  printf("UNKNOWNS: dataset = %d\n", (int) dataset);
#endif

  /* create the hyperslab -- this will differ on the different processors */
  start_4d[0] = (hssize_t) (*global_offset);
  start_4d[1] = 0;
  start_4d[2] = 0;
  start_4d[3] = 0;

  stride_4d[0] = 1;
  stride_4d[1] = 1;
  stride_4d[2] = 1;  
  stride_4d[3] = 1;

  count_4d[0] = (hssize_t) (*local_blocks);
  count_4d[1] = *nzb;
  count_4d[2] = *nyb;
  count_4d[3] = *nxb;

  status = H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, start_4d, 
			      stride_4d, count_4d, NULL);
#ifdef DEBUG_IO
  printf("UNKNOWNS: hyperslab selection = %d\n", (int) status);
#endif


  /* create the memory space -- we can get away with a simple memory space
     for the unknowns, since we are passing a contiguous block of memory
     now, and the block count is the last index in FORTRAN */
  rank = 5;
  dimens_5d[0] = *local_blocks;
  dimens_5d[1] = *nzb+(*nguard)*2*k3d;
  dimens_5d[2] = *nyb+(*nguard)*2*k2d;
  dimens_5d[3] = *nxb+(*nguard)*2;
  dimens_5d[4] = *nvar;

  memspace = H5Screate_simple(rank, dimens_5d, NULL);
#ifdef DEBUG_IO
  printf("UNKNOWNS: memspace = %d\n", (int) memspace);
#endif

  /* setting the transfer template */
  dxfer_template = H5Pcreate(H5P_DATASET_XFER);
#ifdef DEBUG_IO
  printf("UNKNOWNS: dxfer_template = %d\n", (int) dxfer_template);
#endif


  /* ---------------------------------------------------------------------
      platform dependent code goes here -- the access template must be
      tuned for a particular filesystem blocksize.  some of these 
      numbers are guesses / experiments, others come from the file system
      documentation.

      The sieve_buf_size should be equal a multiple of the disk block size
     ---------------------------------------------------------------------- */

#ifdef IBM
  ierr = H5Pset_hyper_cache(dxfer_template, 1u, 0u);
  ierr = H5Pset_dxpl_mpio(dxfer_template, H5FD_MPIO_INDEPENDENT);
  ierr = H5Pset_btree_ratios(dxfer_template, 0.334, 0.333, 0.333); 

  /* for best performance, set this to 0 */
  ierr = H5Pset_preserve(dxfer_template, 0u);  
#endif

#ifdef TFLOPS
  ierr = H5Pset_hyper_cache(dxfer_template, 1u, 0u);
  ierr = H5Pset_dxpl_mpio(dxfer_template, H5FD_MPIO_COLLECTIVE);
  ierr = H5Pset_preserve(dxfer_template, 0u);  
#endif

#ifdef CHIBA
  ierr = H5Pset_hyper_cache(dxfer_template, 1u, 0u);
  ierr = H5Pset_dxpl_mpio(dxfer_template, H5FD_MPIO_INDEPENDENT);
  ierr = H5Pset_preserve(dxfer_template, 0u);  
#endif

#ifdef SGI
  ierr = H5Pset_hyper_cache(dxfer_template, 1u, 0u);
  ierr = H5Pset_dxpl_mpio(dxfer_template, H5FD_MPIO_INDEPENDENT);
  ierr = H5Pset_preserve(dxfer_template, 0u);  
#endif

  /* ----------------------------------------------------------------------
      end of platform dependent data-transfer property list settings
     ---------------------------------------------------------------------- */

  /* write the data */
  status = H5Dwrite(dataset, H5T_NATIVE_FLOAT, memspace, dataspace, 
		    dxfer_template, unknowns);

#ifdef DEBUG_IO
  printf("UNKNOWNS: wrote unknowns, status = %d\n", (int) status);
#endif

  H5Pclose(dxfer_template);

  H5Sclose(memspace); 
  H5Sclose(dataspace);
  H5Dclose(dataset);

}






