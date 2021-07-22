#include "nfi/nfi_ftp/ftp_lib.h"
//Hemos comentado los errno=-1 ya que sino no compilaba.¿Posible error por los includes?
extern int errno;

/* this interface must be changed */
void ftp_err(int err){
        fprintf(stderr, "Error en el servicio FTP %d",err);
	switch(err){
		case FTPERR_PARAM:
			//errno = -1;
			break;
		
		case FTPERR_MEMORY:
			//errno = -1;
			break;
			
		case FTPERR_URL:
			//errno = -1;
			break;

		case FTPERR_SOCKETCONNECTION:
			//errno = -1;
			break;

		case FTPERR_MOUNT:
			//errno = -1;
			break;

		case FTPERR_FTPCONNECTION:
			//errno = -1;
			break;
			
		case FTPERR_GETATTR:
			//errno = -1;
			break;

		case FTPERR_LOOKUP:
			//errno = -1;
			break;

		case FTPERR_READ:
			//errno = -1;
			break;
		case FTPERR_WRITE:
			//errno = -1;
			break;
		case FTPERR_CREATE:
			//errno = -1;
			break;
		case FTPERR_REMOVE:
			//errno = -1;
			break;
		case FTPERR_RENAME:
			//errno = -1;
			break;
		case FTPERR_MKDIR:
			//errno = -1;
			break;
		case FTPERR_READDIR:
			//errno = -1;
			break;
		case FTPERR_NOTDIR:
			//errno = -1;
			break;
		case FTPERR_STATFS:
			//errno = -1;
			break;

	}
};
