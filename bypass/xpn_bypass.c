/****************************************************************************************************
 *                                                                                                  *
 * xpn.bypass                                                                                       *
 *                                                                                                  *
 * This interface definition intercepts system calls (actually, libc wrappers)                      *
 * and redirect them to "expand" file system.                                                       *
 *                                                                                                  *
 *                                                                                                  *
 * Usage:                                                                                           *
 *    sys_call(/xpn/PNFS/<path/to/file>)                                                            *
 *                                                                                                  *
 *                                                                                                  *
 * Grupo de arquitectura de computadores, sistemas y paralelismo. Universidad Carlos III de Madrid. *
 * Computers architecture, systems and paralelism group. Carlos III of Madrid University.           *
 *                                                                                                  *
 * http://arcos.inf.uc3m.es                                                                         *
 *                                                                                                  *
 *                                                                                                  *
 *                                                                                                  *
 * This program is released under the GNU General Public License.                                   *
 * See the file COPYING for details.                                                                *
 *                                                                                                  *
 ****************************************************************************************************/

// next lines are for debugging purposes
#ifdef DEBUG_BYPASS
    #define DEBUG_BYPASS_XPN_ADAPTOR_KEEPINIT
    #define DEBUG_BYPASS_FORK
    //#define DEBUG_BYPASS_EXECVE
    #define DEBUG_BYPASS_OPEN
    #define DEBUG_BYPASS_OPEN64
    #define DEBUG_BYPASS_CLOSE
    #define DEBUG_BYPASS_CREAT
    #define DEBUG_BYPASS_CHDIR
    //#define DEBUG_BYPASS_GETCWD
    #define DEBUG_BYPASS_MKDIR
    #define DEBUG_BYPASS_RMDIR
    #define DEBUG_BYPASS_UNLINK
    #define DEBUG_BYPASS_CHOWN
    #define DEBUG_BYPASS_FTRUNCATE
    #define DEBUG_BYPASS_DUP
    #define DEBUG_BYPASS_DUP2
    #define DEBUG_BYPASS_OPENDIR
    #define DEBUG_BYPASS_READDIR
    #define DEBUG_BYPASS_READDIR64
    #define DEBUG_BYPASS_CLOSEDIR
    #define DEBUG_BYPASS_LSTAT64
    #define DEBUG_BYPASS_STAT64
    #define DEBUG_BYPASS_FSTAT64
    #define DEBUG_BYPASS_LSTAT
    #define DEBUG_BYPASS_STAT
    #define DEBUG_BYPASS_FSTAT
    #define DEBUG_BYPASS_CHMOD
    #define DEBUG_BYPASS_FCHMOD
    #define DEBUG_BYPASS_READ
    #define DEBUG_BYPASS_WRITE
    #define DEBUG_BYPASS_LSEEK
    //#define DEBUG_BYPASS_UTIME
#endif

    // some definitions
    #include "xpn_bypass.h"
    #include "mpi.h"

    /**
     * This variable indicates if expand has already been initialized or not.
     * 0 indicates that expand has NOT been initialized yet.
     * 1 indicates that expand has already been initialized.
     */
    static int xpn_adaptor_initCalled = 0;

    /**
     * This variable contains the prefix which will be considerated as expand partition.
     */
    char *xpn_adaptor_partition_prefix = "xpn://"; //Original
    //char *xpn_adaptor_partition_prefix = ""; //New
    
    //char *preload = "LD_PRELOAD=/export/home/pato11-1/users/bbergua/intercept/bypass/xpn_agent.so";
    
    /*
    char *xpn_adaptor_flog_name  = "/tmp/EXPAND.LOG" ;
    void xpn_adaptor_log ( char * msg, ... )
    {
        FILE *flog;
        va_list vargs;

        va_start(vargs,msg);
        flog = fopen(xpn_adaptor_flog_name,"a+");
        if (NULL != flog) {
            vfprintf(flog,msg,vargs);
            fclose(flog);
        }
        va_end(vargs);
    }
    */
    
    /**
     * This function checks if expand has already been initialized.
     * If not, it initialize it.
     */
    void xpn_adaptor_keepInit ( void )
    {
        int ret;

    #ifdef DEBUG_BYPASS_XPN_ADAPTOR_KEEPINIT
        printf("antes de xpn_adaptor_keepInit\n");
    #endif
        if (0 == xpn_adaptor_initCalled)
        {
            // If expand has not been initialized, then initialize it.
        
            //setenv("XPN_CONF", "/export/home/pato11-1/users/bbergua/expand-1.0/", 0);
    #ifdef DEBUG_BYPASS_XPN_ADAPTOR_KEEPINIT
            printf("Antes de xpn_init()\n");
    #endif
            xpn_adaptor_initCalled = 1; //TODO: quitar
            ret = xpn_init();
    #ifdef DEBUG_BYPASS_XPN_ADAPTOR_KEEPINIT
            printf("Despues de xpn_init()\n");
    #endif
            if (ret < 0)
            {
                printf("xpn_init: Expand couldn't be initialized\n");
                //xpn_adaptor_log("xpn_init: Expand couldn't be initialized\n");
                xpn_adaptor_initCalled = 0;
            }
            else
            {
                xpn_adaptor_initCalled = 1;
            }
        }
    }


    int fork()
    {
        int ret = dlsym_fork();
        if(0 == ret){
            // We want the children to be initialized
            xpn_adaptor_initCalled = 0;
        }
        return ret;
    }
    /*
    int execve(const char *filename, char *const *argv, char *const *envp)

    {
        int ret, num_filas, i;
        char **aux;
        char *envpAux[2];
    #ifdef DEBUG_BYPASS_EXECVE
        printf("antes de execve...\n");
    #endif
        if (NULL != envp){
            
            num_filas = 0;
            while (envp[num_filas] != NULL){
    #ifdef DEBUG_BYPASS_EXECVE
                printf("%s\n",envp[num_filas]);
    #endif
                num_filas++;
            }
            aux = (char**)malloc((num_filas+1)*sizeof(char*));  
            aux[0] = preload;
        }
        if (NULL != envp){
            i = 0;
                while (num_filas>i){
                aux[i+1] = envp[i];
                    i++;
            }
        } else {
            aux = envpAux;
            envpAux[0] = preload;
            envpAux[1] = NULL;
        }
        ret = execve(filename,argv,aux);
        return(ret);
    }
    */
    //int open(const char *path, int flags, [mode_t mode])
    int open(const char *path, int flags, mode_t mode)
    {
        int ret, fd, fdret;

    #ifdef DEBUG_BYPASS_OPEN
        //printf("%d: antes de open.... %s\n",getpid(),path);
        printf("antes de open(%s,%o,%o)...\n",path,flags,mode);
    #endif

        // We must initialize expand if it has not been initialized yet.
        xpn_adaptor_keepInit ();

    #ifdef DEBUG_BYPASS_OPEN
        printf("El path es => %s\n",path);
    #endif
        // This if checks if variable path passed as argument starts with the expand prefix.
        if(!strncmp(xpn_adaptor_partition_prefix,path,strlen(xpn_adaptor_partition_prefix)))
        {
            // It is an XPN partition, so we redirect the syscall to expand syscall
    #ifdef DEBUG_BYPASS_OPEN
            printf("El path es => %s\n",path+strlen(xpn_adaptor_partition_prefix));
    #endif
            fd=xpn_open((char *)(path+strlen(xpn_adaptor_partition_prefix)),flags,mode);
    #ifdef DEBUG_BYPASS_OPEN
            printf("xpn.bypass: xpn_open(%s,%o) devuelve %d\n",path+strlen(xpn_adaptor_partition_prefix),flags,fd);
    #endif
            if(fd<0)
            {
                ret = fd;
            } 
            else{
                fdret=fd+PLUSXPN;
                ret = fdret;
            }
        }// If xpn
        else // Not an XPN partition. We must link with the standard library.
        {   
            return dlsym_open((char *)path, flags, mode);
        }// Else
    #ifdef DEBUG_BYPASS_OPEN
        printf("xpn.bypass: bypass->open(%s,%o,%o) devuelve %d\n\n",path,flags,mode,ret);
    #endif
        return ret;
    }

    
    int open64(const char *path, int flags, mode_t mode)
    {
        int fd, fdret;

    #ifdef DEBUG_BYPASS_OPEN64
        printf("antes de open64.... %s\n",path);
    #endif

        xpn_adaptor_keepInit ();

    #ifdef DEBUG_BYPASS_OPEN64
        printf("1)El path es => %s\n",path+strlen(xpn_adaptor_partition_prefix));
    #endif
        if(!strncmp(xpn_adaptor_partition_prefix,path,strlen(xpn_adaptor_partition_prefix)))
        {
    #ifdef DEBUG_BYPASS_OPEN64
        printf("El path es => %s\n",path+strlen(xpn_adaptor_partition_prefix));
    #endif
            fd=xpn_open((char *)(path+strlen(xpn_adaptor_partition_prefix)),flags);
            if(fd<0)
            {
                return(fd);
            } else{
                fdret=fd+PLUSXPN;
            }

            return fdret;
        }// If xpn
        else // Not an XPN partition. We must link with the standard library
        {
            return dlsym_open64((char *)path, flags, mode);
        }// Else
    }
    

    int close(int fd)
    {
        int ret;

    #ifdef DEBUG_BYPASS_CLOSE
        printf("antes de close....\n");
    #endif

        xpn_adaptor_keepInit ();

        if(fd>=PLUSXPN)
        {
            ret=xpn_close(fd-PLUSXPN);
            return(ret);
        }
        else // Not an XPN partition. We must link with the standard library
        {
            return dlsym_close(fd);
        }// Else
    }

    int creat(const char *path, mode_t mode)
    {
        int fd,fdret;

    #ifdef DEBUG_BYPASS_CREAT
        printf("antes de creat....\n");
    #endif

        xpn_adaptor_keepInit ();

        if(!strncmp(xpn_adaptor_partition_prefix,path,strlen(xpn_adaptor_partition_prefix)))
        {
    #ifdef DEBUG_BYPASS_CREAT
            printf("ES EXPAND!\n");
    #endif
            fd=xpn_creat((char *)(path+strlen(xpn_adaptor_partition_prefix)),mode);
    #ifdef DEBUG_BYPASS_CREAT
            printf("El archivo es %s",(char *)(path+strlen(xpn_adaptor_partition_prefix)));
    #endif
            if(fd<0){
    #ifdef DEBUG_BYPASS_CREAT
                printf("xpn_creat devuelve fd<0: %d\n",fd);
    #endif
                return(fd);
            } 
            else {
                fdret=fd+PLUSXPN;
    //#ifdef DEBUG_BYPASS_CREAT
            //  printf("(Es xpn) fdret=-1\n");
    //#endif
            }
            return(fdret);
        }// If xpn
        else // Not an XPN partition. We must link with the standard library
        {
            return dlsym_creat(path,mode);
        } // Else
    }

    int chdir(const char *path)
    {

    #ifdef DEBUG_BYPASS_CHDIR
        printf("antes de chdir....\n");
    #endif

        xpn_adaptor_keepInit ();

        if(!strncmp(xpn_adaptor_partition_prefix,path,strlen(xpn_adaptor_partition_prefix)))
        {
            return(xpn_chdir((char *)(path+strlen(xpn_adaptor_partition_prefix))));
        }// If xpn
        else // Not an XPN partition. We must link with the standard library
        {
            return dlsym_chdir((char *)path);
        } // Else
    }

    /**************************************************
    GETCWD TIENE MUCHA CHICHA...PA LUEGO
    ***************************************************
    char *getcwd(char *path, size_t size)
    {

    #ifdef DEBUG_BYPASS_GETCWD
        printf("antes de getcwd...\n");
    #endif

        xpn_adaptor_keepInit ();

        if(!strncmp(xpn_adaptor_partition_prefix,path,strlen(xpn_adaptor_partition_prefix)))
        {
            // If xpn
            return(xpn_chdir(path+strlen(xpn_adaptor_partition_prefix)));
        }
        else // Not an XPN partition. We must link with the standard library
        {
            return(chdir(path));
        }
    }
    *******************************************************/

    int mkdir(const char *path, mode_t mode)
    {
        int ret;

    #ifdef DEBUG_BYPASS_MKDIR
        printf("antes de mkdir...\n");
    #endif

        xpn_adaptor_keepInit ();

        if(!strncmp(xpn_adaptor_partition_prefix,path,strlen(xpn_adaptor_partition_prefix)))
        {
    #ifdef DEBUG_BYPASS_MKDIR
            printf("antes de xpn_mkdir(%s)...\n",((char *)(path+strlen(xpn_adaptor_partition_prefix))));
    #endif
            ret =xpn_mkdir( ((char *)(path+strlen(xpn_adaptor_partition_prefix))) ,mode );
    #ifdef DEBUG_BYPASS_MKDIR
            printf("xpn_mkdir ha devuelto %d\n",ret);
    #endif
            return(ret);
            //return(xpn_mkdir( ((char *)(path+strlen(xpn_adaptor_partition_prefix))) ,mode ));
        }// If xpn
        else // Not an XPN partition. We must link with the standard library
        {
            return dlsym_mkdir((char *)path,mode);
        } // Else
    }

    int rmdir(const char *path)
    {
        int ret;

    #ifdef DEBUG_BYPASS_RMDIR
        printf("antes de rmdir...\n");
    #endif

        xpn_adaptor_keepInit ();

        if(!strncmp(xpn_adaptor_partition_prefix,path,strlen(xpn_adaptor_partition_prefix)))
        {
            ret = xpn_rmdir( ((char *)(path+strlen(xpn_adaptor_partition_prefix))) );
    #ifdef DEBUG_BYPASS_RMDIR
            printf("xpn_rmdir(%s) devuelve %d\n",path+strlen(xpn_adaptor_partition_prefix),ret);
    #endif
            return(ret);
        }// If xpn
        else // Not an XPN partition. We must link with the standard library
        {
            return dlsym_rmdir((char *)path);
        } // Else
    }

    int unlink(const char *path)
    {

    #ifdef DEBUG_BYPASS_UNLINK
        printf("antes de unlink...\n");
    #endif

        xpn_adaptor_keepInit ();

        if(!strncmp(xpn_adaptor_partition_prefix,path,strlen(xpn_adaptor_partition_prefix)))
        {
            return(xpn_unlink((char *)(path+strlen(xpn_adaptor_partition_prefix))));
        }// If xpn
        else // Not an XPN partition. We must link with the standard library
        {
            return dlsym_unlink((char *)path);
        } // Else
    }

    int chown(const char *path, uid_t owner, gid_t group)
    {

    #ifdef DEBUG_BYPASS_CHOWN
        printf("antes de chown...\n");
    #endif

        xpn_adaptor_keepInit ();

        if(!strncmp(xpn_adaptor_partition_prefix,path,strlen(xpn_adaptor_partition_prefix)))
        {
            return(xpn_chown((char *)(path+strlen(xpn_adaptor_partition_prefix)), owner, group));
        }// If xpn
        else // Not an XPN partition. We must link with the standard library
        {
            return dlsym_chown((char *)path, owner, group);
        } // Else
    }

    int ftruncate(int fd, off_t length)
    {

    #ifdef DEBUG_BYPASS_FTRUNCATE
        printf("antes de ftruncate...\n");
    #endif

        xpn_adaptor_keepInit ();

        if(fd>=PLUSXPN)
        {
            return xpn_ftruncate(fd-PLUSXPN,length);
        }// If xpn
        else // Not an XPN partition. We must link with the standard library
        {
            return dlsym_ftruncate(fd, length);
        } // Else
    }

    int dup(int fd)
    {

    #ifdef DEBUG_BYPASS_DUP
        printf("antes de dup...\n");
    #endif

        xpn_adaptor_keepInit ();

        if(fd>=PLUSXPN)
        {
            return(xpn_dup(fd-PLUSXPN));
        }// If xpn
        else // Not an XPN partition. We must link with the standard library
        {
            return dlsym_dup(fd);
        } // Else
    }

    int dup2(int fd, int fd2)
    {

    #ifdef DEBUG_BYPASS_DUP2
        printf("antes de dup2...\n");
    #endif

        xpn_adaptor_keepInit ();

        if(fd>=PLUSXPN)
        {
            return(xpn_dup2(fd-PLUSXPN, fd2));
        }// If xpn
        else // Not an XPN partition. We must link with the standard library
        {
            return dlsym_dup2(fd, fd2);
        } // Else
    }

    DIR *opendir(const char *dirname)
    {
        DIR *ret;
        int fd,fdret;

    #ifdef DEBUG_BYPASS_OPENDIR
        printf("antes de opendir(%s)...\n", dirname);
    #endif

        xpn_adaptor_keepInit ();

        if(!strncmp(xpn_adaptor_partition_prefix,dirname,strlen(xpn_adaptor_partition_prefix))) //TODO:Aqui falla
        {
    #ifdef DEBUG_BYPASS_OPENDIR
        printf("xpn.bypass: opendir...\n");
    #endif
            ret=xpn_opendir((char *)(dirname+strlen(xpn_adaptor_partition_prefix)));
    #ifdef DEBUG_BYPASS_OPENDIR
            //printf("xpn.bypass: opendir...ret=%d\n",&ret);
    #endif
    //printf("Antes fd = %d\n", fd);
            memcpy(&fd, ret,sizeof(int)); // TODO: 真真真真Y esto para que???????
    //printf("Despues fd = %d\n", fd);
            if(fd<-1)
            {
                return((DIR *)fd); // TODO: Devolver ret?
            } 
            else {
    //printf("Antes fdret = %d\n", fdret);
                fdret=fd+PLUSXPN;
    //printf("Despues fdret = %d\n", fdret);
                memcpy(ret,&fdret,sizeof(int));
    #ifdef DEBUG_BYPASS_OPENDIR
                //printf("xpn.bypass: opendir...ret=%d\n",*ret);
    #endif
                return(ret);
            }
        }// If xpn
        else // Not an XPN partition. We must link with the standard library
        {
            return dlsym_opendir((char *)dirname);
        } // Else
    }

    
    struct dirent64 *readdir64(DIR *dirp)
    {
        int fd,fdaux;
        struct dirent *aux;
        struct dirent64 *ret = NULL;

    #ifdef DEBUG_BYPASS_READDIR64
        printf("antes de readdir64...\n");
    #endif

        xpn_adaptor_keepInit ();
    #ifdef DEBUG_BYPASS_READDIR64
        printf("despues de xpn_adaptor_keepInit()...\n");
    #endif

        memcpy(&fd, dirp,sizeof(int));
    #ifdef DEBUG_BYPASS_READDIR64
        printf("despues de memcpy()...\n");
    #endif

        if(fd >= PLUSXPN)
        {
            fdaux=fd-PLUSXPN;
            memcpy(dirp,&(fdaux),sizeof(int));
            aux=xpn_readdir(dirp);
            if (aux > 0){
                ret = (struct dirent64 *)malloc(sizeof(struct dirent64));
                ret->d_ino = (__ino64_t)aux->d_ino;
                ret->d_off = (__off64_t)aux->d_off;
                ret->d_reclen = aux->d_reclen;
                ret->d_type = aux->d_type;
                //ret->d_name = aux->d_name;
            }
    #ifdef DEBUG_BYPASS_READDIR64
            printf("despues de xpn_readdir()...\n");
    #endif
            memcpy(dirp,&fd,sizeof(int));
    #ifdef DEBUG_BYPASS_READDIR64
            printf("Es xpn: readdir devuelve %s\n",ret->d_name);
    #endif
            return(ret);
        }// If xpn
        else // Not an XPN partition. We must link with the standard library
        {
            return dlsym_readdir64(dirp);
        } // Else
        return(ret);
    }

    struct dirent *readdir(DIR *dirp)
    {
        int fd,fdaux;
        struct dirent *ret;

    #ifdef DEBUG_BYPASS_READDIR
        printf("antes de readdir...\n");
    #endif

        xpn_adaptor_keepInit ();
    #ifdef DEBUG_BYPASS_READDIR
        printf("despues de xpn_adaptor_keepInit()...\n");
    #endif

        memcpy(&fd, dirp,sizeof(int));
    #ifdef DEBUG_BYPASS_READDIR
        printf("despues de memcpy() 1...\n");
    #endif

        if(fd >= PLUSXPN)
        {
            fdaux=fd-PLUSXPN;
            memcpy(dirp,&(fdaux),sizeof(int));
            ret=xpn_readdir(dirp);
    #ifdef DEBUG_BYPASS_READDIR
            printf("despues de xpn_readdir()...\n");
    #endif
            memcpy(dirp,&fd,sizeof(int));
    #ifdef DEBUG_BYPASS_READDIR
            printf("Es xpn: readdir devuelve %s\n",ret->d_name);
    #endif
            return(ret);
        }// If xpn
        else // Not an XPN partition. We must link with the standard library
        {
            return dlsym_readdir(dirp);
        } // Else
        return ret;
    }

    int closedir(DIR *dirp)
    {
        int fd,ret,temp;

    #ifdef DEBUG_BYPASS_CLOSEDIR
        printf("antes de closedir...\n");
    #endif

        xpn_adaptor_keepInit ();

        memcpy(&fd, dirp,sizeof(int));

        //if(fdstable[fd]>=PLUSXPN)
        if (fd >= PLUSXPN)
        {
            //temp = fdstable[fd]-PLUSXPN;
            temp = fd-PLUSXPN;
            memcpy(dirp, &temp,sizeof(int));
            ret=xpn_closedir(dirp);
            /*
            if (ret==0)
            {
                fdstable[fd]=-1;
            }
            */
    #ifdef DEBUG_BYPASS_CLOSEDIR
            printf("closedir devuelve %d\n",ret);
    #endif
            return(ret);
        }// If xpn
        else // Not an XPN partition. We must link with the standard library
        {
            return dlsym_closedir(dirp);
        } // Else
    }

    // For the moment we intercept __*stat64

    //int lstat64(const char *path, struct stat64 *buf)
    int __lxstat64(int ver, const char *path, struct stat64 *buf)
    {
        int ret;
        struct stat st;

    #ifdef DEBUG_BYPASS_LSTAT64
        printf("antes de lstat64... %s\n",path);
    #endif

        xpn_adaptor_keepInit ();

    #ifdef DEBUG_BYPASS_LSTAT64
        printf("lstat64                  (1)... %s\n",path);
        printf("lstat64...path = %s\n",path+strlen(xpn_adaptor_partition_prefix));
    #endif
        if(!strncmp(xpn_adaptor_partition_prefix,path,strlen(xpn_adaptor_partition_prefix)))
        {
            ret = xpn_stat((char *)(path+strlen(xpn_adaptor_partition_prefix)), &st);
    #ifdef DEBUG_BYPASS_LSTAT64
        printf("lstat64...xpn_stat(%s)=%d\n",path+strlen(xpn_adaptor_partition_prefix),ret);
    #endif
            if(ret<0){
                ret = -1;
            } else{
                //////////////////////////////////////////////
                buf->st_dev = (__dev_t)st.st_dev;
    //          buf->__st_ino   = (__ino_t)st.st_ino;
                buf->st_mode    = (__mode_t)st.st_mode;
                buf->st_nlink   = (__nlink_t)st.st_nlink;
                buf->st_uid = (__uid_t)st.st_uid;
                buf->st_gid = (__gid_t)st.st_gid;
                buf->st_rdev    = (__dev_t)st.st_rdev;
    //          buf->__pad2 = st.st_;
                buf->st_size    = (__off64_t)st.st_size;
                buf->st_blksize = (__blksize_t)st.st_blksize;
                buf->st_blocks  = (__blkcnt64_t)st.st_blocks;
                //buf->st_atime = (__time_t)st.st_atime;
    //          buf->__unused1;
                //buf->st_mtime = (__time_t)st.st_mtime;
    //          buf->__unused2;
                //buf->st_ctime = (__time_t)st.st_ctime;
    //          buf->__unused3 =
                buf->st_ino = (__ino64_t)st.st_ino;

    //          ret = 0;
            }
        }// If xpn
        else // Not an XPN partition. We must link with the standard library
        {
            return dlsym_lxstat64(ver,(char *)path, buf);
        } // Else
    #ifdef DEBUG_BYPASS_LSTAT64
        printf("lstat64...devuelve %d\n",ret);
    #endif
        return(ret);
    }

    //int stat64(const char *path, struct stat64 *buf)
    int __xstat64(int ver, const char *path, struct stat64 *buf)
    {
        int ret;
        struct stat st;

    #ifdef DEBUG_BYPASS_STAT64
        printf("antes de stat64... %s\n",path);
    #endif

        xpn_adaptor_keepInit ();

    #ifdef DEBUG_BYPASS_STAT64
        printf("stat64...path = %s\n",path);
    #endif
        if(!strncmp(xpn_adaptor_partition_prefix,path,strlen(xpn_adaptor_partition_prefix)))
        {
            ret = xpn_stat((char *)(path+strlen(xpn_adaptor_partition_prefix)), &st);
    #ifdef DEBUG_BYPASS_STAT64
        printf("stat64...xpn_stat(%s)=%d\n",path+strlen(xpn_adaptor_partition_prefix),ret);
    #endif
            if(ret<0){
                ret = -1;
            } else {
                //////////////////////////////////////////////
                buf->st_dev     = (__dev_t)st.st_dev;
    //          buf->__st_ino   = (__ino_t)st.st_ino;
                buf->st_mode    = (__mode_t)st.st_mode;
                buf->st_nlink   = (__nlink_t)st.st_nlink;
                buf->st_uid = (__uid_t)st.st_uid;
                buf->st_gid = (__gid_t)st.st_gid;
                buf->st_rdev    = (__dev_t)st.st_rdev;
    //          buf->__pad2 = st.st_;
                buf->st_size    = (__off64_t)st.st_size;
                buf->st_blksize     = (__blksize_t)st.st_blksize;
                buf->st_blocks  = (__blkcnt64_t)st.st_blocks;
                //buf->st_atime     = (__time_t)st.st_atime;
    //          buf->__unused1;
                //buf->st_mtime     = (__time_t)st.st_mtime;
    //          buf->__unused2;
                //buf->st_ctime     = (__time_t)st.st_ctime;
    //          buf->__unused3 =
                buf->st_ino     = (__ino64_t)st.st_ino;

    //          ret = 0;
            }
        }// If xpn
        else // Not an XPN partition. We must link with the standard library
        {    
            return dlsym_xstat64(ver,(char *)path, buf);
        } // Else
    #ifdef DEBUG_BYPASS_LSTAT64
        printf("stat64...devuelve %d\n",ret);
    #endif
        return(ret);
    }

    //int fstat64(int fd, struct stat64 *buf)
    int __fxstat64(int ver, int fd, struct stat64 *buf)
    {
        int ret;
        struct stat st;

    #ifdef DEBUG_BYPASS_FSTAT64
        printf("antes de fstat64... %d\n",fd);
    #endif

        xpn_adaptor_keepInit ();

        if(fd>=PLUSXPN)
        {
    #ifdef DEBUG_BYPASS_FSTAT64
            printf("antes de xpn_stat(%d)... \n",fd-PLUSXPN);
    #endif
            ret = xpn_fstat(fd-PLUSXPN, &st);
    #ifdef DEBUG_BYPASS_FSTAT64
            printf("despues de xpn_stat(%d)=%d... \n",fd-PLUSXPN,ret);
    #endif
            if(ret<0){
                ret = -1;
            } else {
                //////////////////////////////////////////////
                buf->st_dev     = (__dev_t)st.st_dev;
    //          buf->__st_ino   = (__ino_t)st.st_ino;
                buf->st_mode    = (__mode_t)st.st_mode;
                buf->st_nlink   = (__nlink_t)st.st_nlink;
                buf->st_uid = (__uid_t)st.st_uid;
                buf->st_gid = (__gid_t)st.st_gid;
                buf->st_rdev    = (__dev_t)st.st_rdev;
    //          buf->__pad2 = st.st_;
                buf->st_size    = (__off64_t)st.st_size;
                buf->st_blksize     = (__blksize_t)st.st_blksize;
                buf->st_blocks  = (__blkcnt64_t)st.st_blocks;
                //buf->st_atime     = (__time_t)st.st_atime;
    //          buf->__unused1;
                //buf->st_mtime     = (__time_t)st.st_mtime;
    //          buf->__unused2;
                //buf->st_ctime     = (__time_t)st.st_ctime;
    //          buf->__unused3 = ;
                buf->st_ino     = (__ino64_t)st.st_ino;

    //          ret = 0;
            }
        }// If xpn
        else // Not an XPN partition. We must link with the standard library
        {
            return dlsym_fxstat64(ver,fd, buf);
        } // Else
    #ifdef DEBUG_BYPASS_FSTAT64
        printf("antes de devolver %d... \n",ret);
    #endif
        return(ret);
    }

    //int lstat(const char *path, struct stat *buf) //old
    int __lxstat(int ver, const char *path, struct stat *buf)
    {
        int ret;

    #ifdef DEBUG_BYPASS_LSTAT
        printf("antes de lstat... %s\n",path);
    #endif

        xpn_adaptor_keepInit ();

        if(!strncmp(xpn_adaptor_partition_prefix,path,strlen(xpn_adaptor_partition_prefix)))
        {
            ret = xpn_stat((char *)(path+strlen(xpn_adaptor_partition_prefix)), buf);
    #ifdef DEBUG_BYPASS_LSTAT
            printf("XPN:lstat:path = %s\n",path+strlen(xpn_adaptor_partition_prefix));
            printf("XPN:lstat:xpn_stat devuelve %d\n",ret);
    #endif
        }// If xpn
        else // Not an XPN partition. We must link with the standard library
        {
            return dlsym_lstat(ver,(char *)path, buf);
        } // Else
        return ret;
    }

    //int stat(const char *path, struct stat *buf) //old
    int __xstat(int ver, const char *path, struct stat *buf)
    {

        int ret;
        char path2[1024];

    #ifdef DEBUG_BYPASS_STAT
        printf("antes de stat...\n");
        printf("stat...path total=>%s\n",path);
        printf("stat...prefix=>%ld\n",strlen(xpn_adaptor_partition_prefix));
        printf("stat...path=>%s\n",path+strlen(xpn_adaptor_partition_prefix));
    #endif

        xpn_adaptor_keepInit ();

        if(!strncmp(xpn_adaptor_partition_prefix,path,strlen(xpn_adaptor_partition_prefix)))
        {
    #ifdef DEBUG_BYPASS_STAT
            printf("intercept.stat Es XPN...");
            printf("stat...path=>%s\n",path+strlen(xpn_adaptor_partition_prefix));
    #endif


            if (0 == strncmp(path,"/xpn/htdocs",11)){ //TODO
                printf("into TODO\n");
                sprintf(path2,"/export/home/pato11-1/users/bbergua/httpd-2.0.48/htdocs%s",path+11);
                ret = stat(path2, buf);
            } else {
                ret = xpn_stat((char *)(path+strlen(xpn_adaptor_partition_prefix)), buf);
            }
        }// If xpn
        else // Not an XPN partition. We must link with the standard library
        {
            return dlsym_stat(ver,(char *)path, buf);
        } // Else
        return ret;
    }


    //int fstat(int fd, struct stat *buf) //old
    int __fxstat(int ver, int fd, struct stat *buf)
    {
        int ret;

    #ifdef DEBUG_BYPASS_FSTAT
        printf("antes de fstat...\n");
    #endif

        xpn_adaptor_keepInit ();

        if(fd>=PLUSXPN)
        {
            ret = xpn_fstat(fd-PLUSXPN,buf);
        }// If xpn
        else // Not an XPN partition. We must link with the standard library
        {
            return dlsym_fstat(ver,fd,buf);
        } // Else

        return ret;
    }

    int chmod(const char *path, mode_t mode)
    {

    #ifdef DEBUG_BYPASS_CHMOD
        printf("antes de chmod...\n");
    #endif

        xpn_adaptor_keepInit ();

        if(!strncmp(xpn_adaptor_partition_prefix,path,strlen(xpn_adaptor_partition_prefix)))
        {
            return(xpn_chmod((char *)(path+strlen(xpn_adaptor_partition_prefix)), mode));
        }// If xpn
        else // Not an XPN partition. We must link with the standard library
        {
            return dlsym_chmod((char *)path, mode);
        } // Else
    }

    int fchmod(int fd, mode_t mode)
    {

    #ifdef DEBUG_BYPASS_FCHMOD
        printf("antes de fchmod...\n");
    #endif

        xpn_adaptor_keepInit ();

        if(fd>=PLUSXPN)
        {
            return(xpn_fchmod(fd-PLUSXPN,mode));
        }// If xpn
        else // Not an XPN partition. We must link with the standard library
        {
            return dlsym_fchmod(fd,mode);
        } // Else
    }



    ssize_t read(int fd, void *buf, size_t nbyte)
    {

        int ret;
                    
    #ifdef DEBUG_BYPASS_READ
        printf("antes de read...\n");
        printf("read(fd=%d,*buf,nbyte=%ld)\n",fd,nbyte);
        printf("buffer= %s\n",buf);
    #endif

        xpn_adaptor_keepInit ();

        if(fd>=PLUSXPN)
        {
            ret = xpn_read(fd-PLUSXPN, buf, nbyte);
        }// If xpn
        else // Not an XPN partition. We must link with the standard library
        {
            return dlsym_read(fd,buf, nbyte);
        } // Else
        return ret;
    }

    ssize_t write(int fd, const void *buf, size_t nbyte)
    {
        int ret;

    #ifdef DEBUG_BYPASS_WRITE
        printf("antes de write...\n");
    #endif

        xpn_adaptor_keepInit ();


        if(fd>=PLUSXPN)
        {
            ret =(xpn_write(fd-PLUSXPN, (void *)buf, nbyte));
        }// If xpn
        else // Not an XPN partition. We must link with the standard library
        {
            return dlsym_write(fd, (char *) buf, nbyte);
        } // Else

        return ret;
    }

    off_t lseek(int fd, off_t offset, int whence)
    {

    #ifdef DEBUG_BYPASS_LSEEK
        printf("antes de lseek...\n");
    #endif

        xpn_adaptor_keepInit ();

        if(fd>=PLUSXPN)
        {
            return(xpn_lseek(fd-PLUSXPN, offset, whence));
        }// If xpn
        else // Not an XPN partition. We must link with the standard library
        {
            return dlsym_lseek(fd,offset, whence);
        } // Else
    }

    /*
    int utime(char *path, struct utimbuf *times)
    {

    #ifdef DEBUG_BYPASS_UTIME
        printf("antes de utime...\n");
    #endif

        xpn_adaptor_keepInit ();

        if(!strncmp(xpn_adaptor_partition_prefix,path,strlen(xpn_adaptor_partition_prefix)))
        {
            return(xpn_utime(path+strlen(xpn_adaptor_partition_prefix), times));
        }// If xpn
        else // Not an XPN partition. We must link with the standard library
        {
            return(utime(path, times));
        } // Else
    }
    */

    int fcntl(int fd, int cmd, long arg){
        //printf("Antes de pachaaa\n");
        if(fd >= PLUSXPN){
            //TODO
            return 0;
        } else {
            return dlsym_fcntl(fd, cmd, arg);
        }
    }

    void exit(int status)
    {
        //printf("EXIT BYPASS\n");

        if (xpn_adaptor_initCalled == 1)
        {
            xpn_destroy();
        }
        
        dlsym_exit(status);
    }








    int MPI_Init (int *argc, char ***argv)
    {
        char *value;

        //printf("DENTRO \n");

        // We must initialize expand if it has not been initialized yet.
        xpn_adaptor_keepInit ();

        value = getenv("IS_MPI_SERVER") ;
        if (NULL == value){
            return PMPI_Init(argc, argv);
        }

        //printf("mpiServer end\n");
        return MPI_SUCCESS;
    }

    int MPI_Finalize (void)
    {
        char *value;

        //printf("MPI_Finalize BYPASS\n");

        value = getenv("IS_MPI_SERVER") ;
        if (NULL != value && xpn_adaptor_initCalled == 1){
            xpn_destroy();
        }

        return PMPI_Finalize();
    }