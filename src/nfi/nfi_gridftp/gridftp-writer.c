//gcc -I /usr/local/globus/include/gcc32 -L/usr/local/globus/lib -o gridftpclient2 gridftpclient1.c -lglobus_ftp_client_gcc32
//gcc -I /usr/local/globus/include/gcc32dbg -c gridftp-cp.c -o gridftp-cp.o
//gcc -L /usr/local/globus/lib -lglobus_ftp_client_gcc32dbg gridftp-cp.o -o gridftp-cp

/***************************************************************************
* Globus Developers Tutorial: GridFTP Example - Simple Authenticated Put
*
* There are no handle or operation attributes used in this example.
* This means the transfer runs using all defaults, which implies standard
* FTP stream mode. Note that while this program shows proper usage of
* the Globus GridFTP client library functions, it is not an example of
* proper coding style. Much error checking has been left out and other
* simplifications made to keep the program simple.
***************************************************************************/
#include <stdio.h>
#include "globus_ftp_client.h"
static globus_mutex_t lock;
static globus_cond_t cond;
static globus_bool_t done;
#define MAX_BUFFER_SIZE 2048
#define ERROR -1
#define SUCCESS 0
/********************************************************************
* done_cb: A pointer to this function is passed to the call to
* globus_ftp_client_put (and all the other high level transfer
* operations). It is called when the transfer is completely
* finished, i.e. both the data channel and control channel exchange.
* Here it simply sets a global variable (done) to true so the main
* program will exit the while loop.
********************************************************************/
static void
done_cb (void *user_arg,
	 globus_ftp_client_handle_t * handle, globus_object_t * err)
{
  char *tmpstr;
  if (err)
    {
      fprintf (stderr, "%s", globus_object_printable_to_string (err));
    }
  globus_mutex_lock (&lock);
  done = GLOBUS_TRUE;
  globus_cond_signal (&cond);
  globus_mutex_unlock (&lock);
  return;
}

/*************************************************************************
* data_cb: A pointer to this function is passed to the call to
* globus_ftp_client_register_write. It is called when the user supplied
* buffer has been successfully transferred to the kernel. Note that does
* not mean it has been successfully transmitted. In this simple version,
* it justs reads the next block of data and calls register_write again.
*************************************************************************/
static void
data_cb (void *user_arg,
	 globus_ftp_client_handle_t * handle,
	 globus_object_t * err,
	 globus_byte_t * buffer,
	 globus_size_t length, globus_off_t offset, globus_bool_t eof)
{
  if (err)
    {
      fprintf (stderr, "%s", globus_object_printable_to_string (err));
    }
  else
    {
      if (!eof)
	{
	  FILE *fd = (FILE *) user_arg;
	  int rc;
	  rc = fread (buffer, 1, MAX_BUFFER_SIZE, fd);
	  if (ferror (fd) != SUCCESS)
	    {
	      printf ("Read error in function data_cb; errno = %d\n", errno);
	      return;
	    }
	  globus_ftp_client_register_write (handle,
					    buffer,
					    rc,
					    offset + length,
					    feof (fd) != SUCCESS,
					    data_cb, (void *) fd);
	}			/* if(!eof) */
    }				/* else */
  return;
}				/* data_cb */

/**************************
* Main Program
*************************/
int
main (int argc, char **argv)
{
  globus_ftp_client_handle_t handle;
  globus_byte_t buffer[MAX_BUFFER_SIZE];
  globus_size_t buffer_length = MAX_BUFFER_SIZE;
  globus_result_t result;
  char *src;
  char *dst;
  FILE *fd;
/*************************************
* Process the command line arguments
*************************************/
  if (argc != 3)
    {
      printf ("Usage: put local_file DST_URL\n");
      return (ERROR);
    }
  else
    {
      src = argv[1];
      dst = argv[2];
    }
/*********************************
* Open the local source file
*********************************/
  fd = fopen (src, "r");
  if (fd == NULL)
    {
      printf ("Error opening local file: %s\n", src);
      return (ERROR);
    }
/*********************************************************************
* Initialize the module, and client handle
* This has to be done EVERY time you use the client library
* The mutex and cond are theoretically optional, but highly recommended
* because they will make the code work correctly in a threaded build.
*
* NOTE: It is possible for each of the initialization calls below to
* fail and we should be checking for errors. To keep the code simple
* and clean we are not. See the error checking after the call to
* globus_ftp_client_put for an example of how to handle errors in
* the client library.
*********************************************************************/
  globus_module_activate (GLOBUS_FTP_CLIENT_MODULE);
  globus_mutex_init (&lock, GLOBUS_NULL);
  globus_cond_init (&cond, GLOBUS_NULL);
  globus_ftp_client_handle_init (&handle, GLOBUS_NULL);
/********************************************************************
* globus_ftp_client_put starts the protocol exchange on the control
* channel. Note that this does NOT start moving data over the data
* channel
*******************************************************************/
  done = GLOBUS_FALSE;
  result = globus_ftp_client_put (&handle,
				  dst, GLOBUS_NULL, GLOBUS_NULL, done_cb, 0);
  if (result != GLOBUS_SUCCESS)
    {
      globus_object_t *err;
      err = globus_error_get (result);
      fprintf (stderr, "%s", globus_object_printable_to_string (err));
      done = GLOBUS_TRUE;
    }
  else
    {
      int rc;
/**************************************************************
* This is where the data movement over the data channel is initiated.
* You read a buffer, and call register_write. This is an asynch
* call which returns immediately. When it is finished writing
* the buffer, it calls the data callback (defined above) which
* reads another buffer and calls register_write again.
* The data callback will also indicate when you have hit eof
* Note that eof on the data channel does not mean the control
* channel protocol exchange is complete. This is indicated by
* the done callback being called.
*************************************************************/
      rc = fread (buffer, 1, MAX_BUFFER_SIZE, fd);
      globus_ftp_client_register_write (&handle,
					buffer,
					rc,
					0,
					feof (fd) != SUCCESS,
					data_cb, (void *) fd);
    }
/*********************************************************************
* The following is a standard thread construct. The while loop is
* required because pthreads may wake up arbitrarily. In non-threaded
* code, cond_wait becomes globus_poll and it sits in a loop using
* CPU to wait for the callback. In a threaded build, cond_wait would
* put the thread to sleep
*********************************************************************/
  globus_mutex_lock (&lock);
  while (!done)
    {
      globus_cond_wait (&cond, &lock);
    }
  globus_mutex_unlock (&lock);
/**********************************************************************
* Since done has been set to true, the done callback has been called.
* The transfer is now completely finished (both control channel and
* data channel). Now, Clean up and go home
**********************************************************************/
  globus_ftp_client_handle_destroy (&handle);
  globus_module_deactivate_all ();
  return 0;
}
