/* This file contains the routines that open and close the HDF5 files */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hdf5.h>
#include "hdf5_flash.h"
#include <mpi.h>


/* define an info object to store MPI-IO information */
static MPI_Info FILE_INFO_TEMPLATE;


/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx */

void h5_initialize_file_(hid_t* file_identifier, char filename[81])
{

  int ierr;

  hid_t acc_template;

  /* make the filename pretty -- cutoff any trailing white space */
  int len = 0;
  char* string_index; 

  /* operate on a copy of the filename -- adding the null character for
     C messes with FORTRAN */

  char local_filename[81];


  string_index = filename;
  
  while (*string_index != ' ') {
    local_filename[len] = filename[len];
    len++;
    string_index++;
  }

  *(local_filename+len) = '\0';

  /* set the file access template for parallel IO access */
  acc_template = H5Pcreate(H5P_FILE_ACCESS);

  /* ---------------------------------------------------------------------
      platform dependent code goes here -- the access template must be
      tuned for a particular filesystem blocksize.  some of these 
      numbers are guesses / experiments, others come from the file system
      documentation.

      The sieve_buf_size should be equal a multiple of the disk block size
     ---------------------------------------------------------------------- */

  /* create an MPI_INFO object -- on some platforms it is useful to
     pass some information onto the underlying MPI_File_open call */
  ierr = MPI_Info_create(&FILE_INFO_TEMPLATE);

#ifdef IBM
  ierr = H5Pset_sieve_buf_size(acc_template, 262144); 
  ierr = H5Pset_alignment(acc_template, 524288, 262144);
#endif

#ifdef TFLOPS
  ierr = H5Pset_sieve_buf_size(acc_template, 524288); 
  ierr = H5Pset_alignment(acc_template, 524288, 262144);

  ierr = MPI_Info_set(FILE_INFO_TEMPLATE, "access_style", "write_once");
  ierr = MPI_Info_set(FILE_INFO_TEMPLATE, "collective_buffering", "true");
  ierr = MPI_Info_set(FILE_INFO_TEMPLATE, "cb_block_size", "1048576");
  ierr = MPI_Info_set(FILE_INFO_TEMPLATE, "cb_buffer_size", "4194304");
#endif

#ifdef CHIBA
  ierr = H5Pset_sieve_buf_size(acc_template, 524288); 
  ierr = H5Pset_alignment(acc_template, 524288, 262144);

  ierr = MPI_Info_set(FILE_INFO_TEMPLATE, "access_style", "write_once");
  ierr = MPI_Info_set(FILE_INFO_TEMPLATE, "collective_buffering", "true");
  ierr = MPI_Info_set(FILE_INFO_TEMPLATE, "cb_block_size", "1048576");
  ierr = MPI_Info_set(FILE_INFO_TEMPLATE, "cb_buffer_size", "4194304");
#endif

  ierr = H5Pset_fapl_mpio(acc_template, MPI_COMM_WORLD, FILE_INFO_TEMPLATE);

#ifdef DEBUG_IO
  printf("set fapl to use MPI-IO, ierr = %d\n", (int) ierr);
#endif

  /* ----------------------------------------------------------------------
      end of platform dependent properties
     ---------------------------------------------------------------------- */


  /* ----------------------------------------------------------------------
      the jury is still out on dataset chunking, so group all of the 
      chunking settings under the CHUNK preprocessor variable
     ---------------------------------------------------------------------- */
#ifdef CHUNK
  ierr = H5Pset_cache(acc_template, 128, 512, (size_t) 1048576, 1.0);
#endif


  
  /* create the file collectively */
  *file_identifier = H5Fcreate(local_filename, H5F_ACC_TRUNC, 
			       H5P_DEFAULT, acc_template);

#ifdef DEBUG_IO
  printf("openned the file, identifier = %d\n", (int) *file_identifier);
#endif

  /* release the file access template */
  ierr = H5Pclose(acc_template);

    
}



/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx */

void h5_close_file_(hid_t* file_identifier)
{
  int ierr;


  /* close the file */
  H5Fclose(*file_identifier);
  
  ierr = MPI_Info_free(&FILE_INFO_TEMPLATE);
  
}









