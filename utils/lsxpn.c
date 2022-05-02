
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <stdio.h>
#include <sys/file.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/errno.h>
#include <pwd.h>
#include <sys/types.h>
#include "xpn.h"

#define NFSMAXPATHLEN 1024

void get_perm(int attr, char *attrstr)
{
        attrstr[0]= S_ISDIR(attr)  ? 'd' : '-' ;
        attrstr[1]= (attr & S_IRUSR) ? 'r' : '-' ;
        attrstr[2]= (attr & S_IWUSR) ? 'w' : '-' ;
        attrstr[3]= (attr & S_IXUSR) ? 'x' : '-' ;
        attrstr[4]= (attr & S_IRGRP) ? 'r' : '-' ;
        attrstr[5]= (attr & S_IWGRP) ? 'w' : '-' ;
        attrstr[6]= (attr & S_IXGRP) ? 'x' : '-' ;
        attrstr[7]= (attr & S_IROTH) ? 'r' : '-' ;
        attrstr[8]= (attr & S_IWOTH) ? 'w' : '-' ;
        attrstr[9]= (attr & S_IXOTH) ? 'x' : '-' ;
	attrstr[10]= '\0';
}

void del_slash(char *path)
{
	int i,j,k;
	char pathAux[NFSMAXPATHLEN];

	i=0;
	j=0;
	k=0;
	while(path[i]!='\0'){
		if((k==0)&&(path[i]=='/')){
			pathAux[j]=path[i];
			j++;
			k=1;
		}
		else if(path[i]!='/'){
			k=0;
			pathAux[j]=path[i];
			j++;
		}
		i++;
	}
	pathAux[j] = '\0';
	strcpy(path, pathAux);
}


int main(int argc, char *argv[])
{
  char *destino;
  int ret;
  DIR *fdp;
  struct dirent *dr;
  char aux[2*NFSMAXPATHLEN];
  char aux2[NFSMAXPATHLEN];
  
  struct stat st;
  int i;
  char attrstr[11];
  struct passwd *pass;
  
  // Arguments
  if(argc !=2){
    printf("Incorrect number of parameters. Usage \"lsxpn <path>\"\n");
    exit(0);
  }
  destino=argv[1];

  // XPN
  if((ret=xpn_init())<0){
    printf("Error in init %d\n",ret);
    exit(-1);
  }
  strcpy(aux2,destino);
  
 
  del_slash(aux2);
  printf("open %s\n", aux2);
  fdp = xpn_opendir(aux2); 
  if(fdp == NULL){
    printf("error in ls \n");
    exit(-1);
  } 
  i=0;  
  while(( dr = xpn_readdir(fdp)) != NULL)
  {
	  if((strcmp(dr->d_name,".")!=0)&&(strcmp(dr->d_name,"..")!=0))
	  {
		/*printf("%s/%s\n",aux2,dr->d_name);*/
		sprintf(aux,"%s/%s",aux2,dr->d_name);
	  	ret= xpn_stat(aux,&st);
		if(ret<0){
			/*printf("stat ret = %s -> %d\n",aux,ret);*/
			free(dr);
			continue;
		}
		get_perm(st.st_mode, attrstr);
		pass = getpwuid((uid_t)st.st_uid);
		if(pass == NULL){
			printf("Error in lsxpn .. %d\n",ret);
			free(dr);
		        exit(-1);
		}
				
                printf("%s\t%s\t%d\t%s\n",
			attrstr,
			pass->pw_name,
			(int)st.st_size,
			dr->d_name);
		i++;
	  }

 	  /*printf("dr->d_name = %s\n",dr->d_name); */
	  free(dr);
  }
  
  printf("Total %d\n",i);
	 
  xpn_closedir(fdp);

  xpn_destroy();
  exit(0);
}

