

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "all_system.h"
#include "base/path_misc.h"
#include "base/urlstr.h"
#include "nfi/nfi.h"
#include "nfi/nfi_lib.h"
#include "nfi/nfi_nfs/nfs.h"
#include "nfi/nfi_nfs/nfi_nfs.h"
#include "nfi/nfi_nfs3/nfs3.h"
#include "nfi/nfi_nfs3/nfi_nfs3.h"
#include "nfi/nfi_tcp_server/tcp_server.h"
#include "xpn/xpn_simple/xpn_policy_init.h"


#define	NFI_NOOPT	-1
#define	NFI_EXIT  	 0
#define	NFI_INIT  	 1
#define	NFI_END 	 2
#define	NFI_MKDIR 	 3
#define	NFI_RMDIR 	 4
#define	NFI_CREAT 	 5
#define	NFI_UNLINK 	 6
#define	NFI_OPEN 	 7
#define	NFI_CLOSE 	 8
#define	NFI_READ 	 9
#define	NFI_WRITE 	10
#define	NFI_OPENDIR 	11
#define	NFI_CLOSEDIR 	12
#define	NFI_READDIR 	13

#define	NFI_NOPTR 	-1
#define	NFI_NFS2 	 0
#define	NFI_LOCAL 	 1
#define	NFI_FTP 	 2

#ifndef KB
#define KB		1024
#endif

#define MAXSIZE 	(KB*4)

int protocol = -1;
int fh_open_file = 0;
int fh_open_dir = 0;

char buff[MAXSIZE];

struct nfi_server server;

struct nfi_attr attr;
struct nfi_fhandle  fh;
struct nfi_fhandle  fhdir;
struct nfi_worker_io io;

int read_nfi_cmd()
{
	int  ret ;
	char command[255];
	int  cmd;

	if(EOF == scanf("%s",command)){
		cmd = NFI_EXIT;
		return cmd;
	}

	cmd = NFI_NOOPT;
	if(strcmp("exit",command) == 0){
		cmd = NFI_EXIT;
	}

	if(strcmp("open",command) == 0){
		cmd = NFI_OPEN;
	}

	if(strcmp("close",command) == 0){
		cmd = NFI_CLOSE;
	}

	if(strcmp("creat",command) == 0){
		cmd = NFI_CREAT;
	}

	if(strcmp("rm",command) == 0){
		cmd = NFI_UNLINK;
	}

	if(strcmp("mkdir",command) == 0){
		cmd = NFI_MKDIR;
	}

	if(strcmp("rmdir",command) == 0){
		cmd = NFI_RMDIR;
	}

	if(strcmp("opendir",command) == 0){
		cmd = NFI_OPENDIR;
	}

	if(strcmp("closedir",command) == 0){
		cmd = NFI_CLOSEDIR;
	}

	if(strcmp("readdir",command) == 0){
		cmd = NFI_READDIR;
	}

	if(strcmp("init",command) == 0){
		cmd = NFI_INIT;
	}

	if(strcmp("end",command) == 0){
		cmd = NFI_END;
	}

	return cmd;
}


int read_protocol(char *url)
{
	int prot, ret;
	char str_prot[255];

	ret = ParseURL(url, str_prot, NULL, NULL, NULL, NULL, NULL);
	if(ret <0){
		return -1;
	}

	prot = -1;
	if(strcmp(str_prot,"nfs") == 0){
		prot = NFI_NFS2;
	}

	return prot;
}

void read_params(int cmd)
{
	char url[255] ;
	struct dirent entry ;
	int ret, offset, size;

	switch(cmd)
	{
		case NFI_EXIT:
			switch(protocol){
				case NFI_NFS2:
					ret = nfi_nfs_destroy(&server);
					if (ret<0) {
					  printf("rnfi> Error on nfi_nfs_destroy\n");
					}
					protocol = -1;
					break;
				default:
					break;
			}
			break;

		case NFI_END:
			switch(protocol){
				case NFI_NFS2:
					ret = nfi_nfs_destroy(&server);
					if (ret<0) {
					  printf("rnfi> Error on nfi_nfs_destroy\n");
					}
					protocol = -1;
					break;
				default:
					break;
			}
			break;

		case NFI_INIT:
			ret = scanf("%250s",url);

                        switch(protocol){
                            case NFI_NFS2:
                                 ret = nfi_nfs_destroy(&server);
				 if (ret<0) {
				   printf("rnfi> Error on nfi_nfs_destroy\n");
				 }
                                 protocol = -1;
                                 break;
                            default:
                                 break;
                        }

			protocol = read_protocol(url);
			switch(protocol){
				case NFI_NFS2:
					ret = nfi_nfs_init(url, &server);
					if(ret<0){
						printf("rnfi> Error operation INIT\n");
						protocol = -1;
					}else{

						printf("rnfi> Successfully operation INIT\n");
					}
					break;
				default:
					printf("rnfi> URL %s no valid\n",url);
					break;
			}
			break;

		case NFI_OPEN:
			ret = scanf("%250s",url);

			if(protocol < 0){
				printf("rnfi> Error protocol no defined\n");
                                break;
			}

			if(fh_open_file == 1){
				/* close */
				nfi_worker_do_close(server.wrk, &fh);
				ret = nfi_worker_wait(server.wrk);
				fh_open_file = 0;
			}

			nfi_worker_do_open(server.wrk, url, &fh);
			ret = nfi_worker_wait(server.wrk);

			if (ret<0){
				printf("rnfi> Error operation OPEN\n");
			}else{
				printf("rnfi> Successfully operation OPEN\n");
				fh_open_file = 1;
			}

			break;

		case NFI_CLOSE:
			if(protocol < 0){
				printf("rnfi> Error protocol no defined\n");
                                break;
			}

			if(fh_open_file == 0){
				printf("rnfi> Error file not open\n");
                                break;
			}

			nfi_worker_do_close(server.wrk, &fh);
			ret = nfi_worker_wait(server.wrk);

			if (ret<0){
				printf("rnfi> Error operation CLOSE\n");
			}else{
				printf("rnfi> Successfully operation CLOSE\n");
			}

		  	fh_open_file = 0;
			break;

		case NFI_RMDIR:
			ret = scanf("%250s",url);

			if(protocol < 0){
				printf("rnfi> Error protocol no defined\n");
				break;
			}
			nfi_worker_do_rmdir(server.wrk,
		                                 url);
  			ret = nfi_worker_wait(server.wrk);
			if (ret<0){
				printf("rnfi> Error operation RMDIR\n");
			}else{
				printf("rnfi> Successfully operation RMDIR\n");
			}
			break;

		case NFI_MKDIR:
			ret = scanf("%250s",url);
			bzero(&attr, sizeof(struct nfi_attr));
			attr.at_mode = 0777;
			nfi_worker_do_mkdir(server.wrk, url, &attr,&fh);

  			ret = nfi_worker_wait(server.wrk);
			if (ret<0){
				printf("rnfi> Error operation MKDIR\n");
			}else{
				printf("rnfi> Successfully operation MKDIR\n");
			}
			break;

		case NFI_CREAT:
			ret = scanf("%250s",url);

		 	if(protocol < 0){
				printf("rnfi> Error protocol no defined\n");
				break;
			}
			bzero(&attr, sizeof(struct nfi_attr));
			attr.at_mode = 0777;
			nfi_worker_do_create(server.wrk,
		                                 url,
		                        	 &attr,&fh);
  			ret = nfi_worker_wait(server.wrk);
			if (ret<0){
				printf("rnfi> Error operation CREAT\n");
			}else{
				printf("rnfi> Successfully operation CREAT\n");
			}
			break;

		case NFI_UNLINK:
                        ret = scanf("%250s",url);

			if(protocol < 0){
				printf("rnfi> Error protocol no defined\n");
				break;
			}
			nfi_worker_do_remove(server.wrk,
			                     url);
			ret = nfi_worker_wait(server.wrk);
			if (ret<0){
			      printf("rnfi> Error operation UNLINK\n");
			}else{
			      printf("rnfi> Successfully operation UNLINK\n");
			}
			break;

		case NFI_WRITE:
			ret = scanf("%d%d", &offset, &size);
			if(protocol < 0){
				printf("rnfi> Error protocol no defined\n");
                                break;
			}

			if(fh_open_file == 0){
				printf("rnfi> Error file not open\n");
                                break;
			}

			io.buffer = (void *)buff;
			io.offset = offset;
			io.size   = size;
			if(size > MAXSIZE){
				io.size = MAXSIZE;
			}

			nfi_worker_do_write(server.wrk, &io, 1);
			ret = nfi_worker_wait(server.wrk);

                        if (ret<0){
                        	printf("rnfi> Error operation WRITE %d\n",ret);
			}else{
			        printf("rnfi> Successfully operation WRITE\n");
			}

			break;

		case NFI_READ:
			ret = scanf("%d%d", &offset, &size);
			if(protocol < 0){
				printf("rnfi> Error protocol no defined\n");
                                break;
			}

			if(fh_open_file == 0){
				printf("rnfi> Error file not open\n");
                                break;
			}

			io.buffer = (void *)buff;
			io.offset = offset;
			io.size   = size;
			if(size > MAXSIZE){
				io.size = MAXSIZE;
			}

			nfi_worker_do_read(server.wrk, &io, 1);
			ret = nfi_worker_wait(server.wrk);

                        if (ret<0){
                        	printf("rnfi> Error operation READ %d\n",ret);
			}else{
			        printf("rnfi> Successfully operation READ\n");
			}

			break;

		case NFI_OPENDIR:
			ret = scanf("%250s",url);
			if(protocol < 0){
				printf("rnfi> Error protocol no defined\n");
                                break;
			}

			if(fh_open_dir == 1){

				nfi_worker_do_closedir(server.wrk, &fhdir);
				ret = nfi_worker_wait(server.wrk);
                                fh_open_dir = 0;
			}

			nfi_worker_do_opendir(server.wrk, url, &fhdir);
			ret = nfi_worker_wait(server.wrk);

                        if (ret<0){
                        	printf("rnfi> Error operation OPENDIR\n");
			}else{
			        printf("rnfi> Successfully operation OPENDIR\n");
				fh_open_dir = 1;
			}
			break;

		case NFI_CLOSEDIR:
			if(protocol < 0){
				printf("rnfi> Error protocol no defined\n");
                                break;
			}

			if(fh_open_dir == 0){
				printf("rnfi> Error dir not open\n");
                                break;
			}

			nfi_worker_do_closedir(server.wrk, &fhdir);
			ret = nfi_worker_wait(server.wrk);

                        if (ret<0){
                        	printf("rnfi> Error operation CLOSEDIR\n");
			}else{
			        printf("rnfi> Successfully operation CLOSEDIR\n");
			}
			fh_open_dir = 0;
			break;

		case NFI_READDIR:
			if(protocol < 0){
				printf("rnfi> Error protocol no defined\n");
                                break;
			}

			if(fh_open_dir == 0){
				printf("rnfi> Error dir not open\n");
                                break;
			}
			nfi_worker_do_readdir(server.wrk, &fhdir, &entry);
			ret = nfi_worker_wait(server.wrk);

                        if (ret<0){
                        	printf("rnfi> Error operation READDIR\n");
			}else{
			        printf("rnfi> Successfully operation READDIR %s\n", entry.d_name);
			}
			break;

		default:
			printf("rnfi> Operation not implemented\n");
			break;

	}
}

int main(int argc, char *argv[])
{
	int cmd;

	do{
		printf("nfi> ");
		cmd = read_nfi_cmd();
		read_params(cmd);

	}while(cmd != NFI_EXIT);

	return 0;
}
