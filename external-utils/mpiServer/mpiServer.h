/** 
Only include the necessary data to realize the comunication between client and server
*/
#ifndef _MPISERVER_CLIENT_H_
#define _MPISERVER_CLIENT_H_


#include "all_system.h"
#include "mpiServer_conf.h"
#include "mpiServer_comm.h"
#include "mpiServer_utils.h"

#define _LARGEFILE64_ 1

/* External CONST & STRUCTS */
extern int errno;

/* Operation */
#define MPISERVER_OPEN_FILE	0
#define MPISERVER_CREAT_FILE	1
#define MPISERVER_READ_FILE	2
#define MPISERVER_WRITE_FILE	3
#define MPISERVER_CLOSE_FILE	4
#define MPISERVER_RM_FILE	5
#define MPISERVER_GETATTR_FILE	6
#define MPISERVER_SETATTR_FILE	7

#define MPISERVER_MKDIR_DIR	8
#define MPISERVER_RMDIR_DIR	9
#define MPISERVER_OPENDIR_DIR	10
#define MPISERVER_READDIR_DIR	11
#define MPISERVER_CLOSEDIR_DIR	12

#define MPISERVER_FLUSH_FILE	13
#define MPISERVER_PRELOAD_FILE	14

#define MPISERVER_STATFS_DIR	15

#define MPISERVER_FINALIZE   	16
#define MPISERVER_GETID          17
#define MPISERVER_END 		-1



#ifndef KB
	#define KB (1024)
#endif

#ifndef MB
	#define MB (KB*KB)
#endif

#ifndef GB
	#define GB (MB*MB)
#endif

#ifndef TRUE
	#define TRUE 1
#endif
#ifndef FALSE
	#define FALSE 0
#endif


#endif
