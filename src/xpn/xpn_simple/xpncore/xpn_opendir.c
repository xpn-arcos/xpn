#include "xpn.h"
#include "xpn/xpn_simple/xpn_open.h"
#include "xpn/xpn_simple/xpn_opendir.h"
#include "xpn/xpn_simple/xpn_policy_opendir.h"

DIR *xpn_opendir(const char *path)
{
	DIR *dirp = NULL;
	int res = -1;
	//char path_aux[255];
	char path_aux[MAX_PATH_LEN];
	
	XPN_DEBUG_BEGIN_ARGS1(path)
	
	if ((path == NULL)||(strlen(path)==0)||(strlen(path)>MAX_PATH_LEN)) {
		errno = ENOENT;
		XPN_DEBUG_END_ARGS1(path)
		return NULL;
	}
	
	strcpy(path_aux,path);
	
	//if(path_aux[strlen(path_aux)-1] != '/')
	//	  path_aux[strlen(path_aux)-1] = '/';
	
	if (path_aux[strlen(path)-1] != '/') {
		path_aux[strlen(path)] = '/';
		path_aux[strlen(path)+1] = '\0';
	}
	
	res = xpn_open(path_aux, O_RDONLY);
	if (res < 0) {
		errno = ENOENT;
		XPN_DEBUG_END_ARGS1(path)
		return NULL;
	}
	
	dirp = (DIR *)malloc(sizeof(DIR));
	if (dirp == NULL) {
		XPN_DEBUG_END_ARGS1(path)
		return NULL;
	}
	dirp->fd = res;
	
	XPN_DEBUG_END_ARGS1(path)
	
	return dirp;
}

int xpn_closedir(DIR *dirp)
{
  int i;
  
  if((NULL == dirp)||(dirp->fd<0)||(dirp->fd>XPN_MAX_FILE-1)){
	  // set errno
	  return -1;
  }
  

	if(xpn_file_table[dirp->fd] == NULL){
		// xpn_err
		return -1;
	}

	xpn_file_table[dirp->fd]->links--;
	if(xpn_file_table[dirp->fd]->links == 0){
        	for(i=0;i<xpn_file_table[dirp->fd]->data_vfh->n_nfih;i++){
                	if(xpn_file_table[dirp->fd]->data_vfh->nfih[i] != NULL){
				if(xpn_file_table[dirp->fd]->data_vfh->nfih[i]->priv_fh != NULL){
					xpn_file_table[dirp->fd]->data_vfh->nfih[i]->server->ops->nfi_closedir(
							xpn_file_table[dirp->fd]->data_vfh->nfih[i]->server,
							xpn_file_table[dirp->fd]->data_vfh->nfih[i]);	
				}
				free(xpn_file_table[dirp->fd]->data_vfh->nfih[i]);
			}
		}

		free(xpn_file_table[dirp->fd]->data_vfh->nfih);
		free(xpn_file_table[dirp->fd]->data_vfh);

		free(xpn_file_table[dirp->fd]->mdata->policy);
		free(xpn_file_table[dirp->fd]->mdata);

		free(xpn_file_table[dirp->fd]);	
		xpn_file_table[dirp->fd] = NULL;
	}
	

  free(dirp);

  // set errno
  xpn_err(XPN_OK);
  return 0;
  //return -1;
}

struct dirent* xpn_readdir(DIR *dirp)
{
	int res;
	struct dirent *dirnt = NULL;

	XPN_DEBUG_BEGIN

    if((NULL == dirp)||(dirp->fd<0)||(dirp->fd>XPN_MAX_FILE-1)){
    	// set errno
	  	return NULL;
    }
  
	if(xpn_file_table[dirp->fd] == NULL){
		// xpn_err
		return NULL;
	}


	dirnt = (struct dirent *)malloc(sizeof(struct dirent));
	memset(dirnt, 0, sizeof(struct dirent));
	
	/*
	printf("xpn: struct dirent {             %d-%p\n",sizeof(struct dirent),dirnt);
	printf("    ino_t          d_ino;        %d+%d\n",sizeof(ino_t),(int)&dirnt->d_ino-(int)dirnt);
	printf("    off_t          d_off;        %d+%d\n",sizeof(off_t),(int)&dirnt->d_off-(int)dirnt);
	printf("    unsigned short d_reclen;     %d+%d\n",sizeof(unsigned short),(int)&dirnt->d_reclen-(int)dirnt);
	printf("    unsigned char  d_type;       %d+%d\n",sizeof(unsigned char),(int)&dirnt->d_type-(int)dirnt);
	printf("    char           d_name[256];  %d+%d\n",sizeof(struct dirent)-sizeof(ino_t)-sizeof(off_t)-sizeof(unsigned short)-sizeof(unsigned char),(int)&dirnt->d_name-(int)dirnt);
	printf("}\n");
	*/
	
	res = XpnGetEntry(dirp->fd, dirnt->d_name, &(dirnt->d_type));
	if(res != 0){
		free(dirnt);
		XPN_DEBUG_END
		return NULL;
	}

	XPN_DEBUG_END
	return dirnt;	
}

void xpn_rewinddir(__attribute__((__unused__)) DIR *dirp)
{
}
