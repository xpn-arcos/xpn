
/*
 *  Copyright 2000-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
 *
 *  This file is part of Expand.
 *
 *  Expand is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Expand is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with Expand.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef _NFS_H_
#define _NFS_H_

  #include "all_system.h"
  #include "base/path_misc.h"

  #define NFS_TCP 1
  #define NFS_UDP 0

  /* tamaño del buffer de envio de datos */ 
  #define SENDSZ (unsigned int) (128*1024)  
  /* tamaño del buffer de recepción de datos */ 
  #define RECVSZ (unsigned int) (128*1024) 

  #define MNTPATHLEN 1024
  #define MNTNAMLEN 255

  #define NFSPATHLEN MNTPATHLEN

  /* tamaño de los manejadores en bytes */
  #define FHSIZE 32

  /* numero de programa del servicio mount */
  #define MOUNT1_PROGRAM ((u_long)100005)
  /* version de programa del servicio mount */
  #define MOUNT1_VERSION ((u_long)1)


  /* numero de programa del servicio nfs */
  #define NFS2_PROGRAM ((u_long)100003)
  /* version de programa del servicio nfs */
  #define NFS2_VERSION ((u_long)2)


  /************************************************************************************************************/
  /************************************************************************************************************/


  #ifdef __cplusplus
  extern "C" {
  #endif



  typedef char fhandle[FHSIZE];

  typedef char *nfs_dirpath;

  typedef char *nfs_name;

  struct fhd {
    char fh[FHSIZE];
  };
  typedef struct fhd fhd;

  struct fhstatus {
    u_long status;
    union {
      fhandle directory;
    } fhstatus_u;
  };
  typedef struct fhstatus fhstatus;

  typedef struct mountbody *mountlist;

  struct mountbody {
    nfs_name ml_hostname;
    nfs_dirpath ml_directory;
    mountlist ml_next;
  };
  typedef struct mountbody mountbody;

  typedef struct groupnode *groups;

  struct groupnode {
    nfs_name gr_name;
    groups gr_next;
  };
  typedef struct groupnode groupnode;

  typedef struct exportnode *exports;

  struct exportnode {
    nfs_dirpath ex_dir;
    groups ex_groups;
    exports ex_next;
  };
  typedef struct exportnode exportnode;
  #define MAXDATA 8192
  #define NFSMAXPATHLEN 1024
  #define NFSMAXNAMLEN 255
  #define COOKIESIZE 4

  typedef char *filename;

  typedef char *path;

  typedef char nfscookie[COOKIESIZE];

  typedef struct {
    u_long nfsdata_len;
    char *nfsdata_val;
  } nfsdata;

  enum nfs_stat {
    /* OK */
    NFS_OK = 0, 
    /* errores de nfs */ 
    NFSERR_PERM = 1,
    NFSERR_NOENT = 2,
    NFSERR_IO = 5,
    NFSERR_NXIO = 6,
    NFSERR_ACCES = 13,
    NFSERR_EXIST = 17,
    NFSERR_NODEV = 19,
    NFSERR_NOTDIR = 20,
    NFSERR_ISDIR = 21,
    NFSERR_FBIG = 27,
    NFSERR_NOSPC = 28,
    NFSERR_ROFS = 30,
    NFSERR_NAMETOOLONG = 63,
    NFSERR_NOTEMPTY = 66,
    NFSERR_DQUOT = 69,
    NFSERR_STALE = 70,
    NFSERR_WFLUSH = 99,
    /* */
    NFSERR_NULL = -3,
    NFSERR_CONNECT = -7,
    NFSERR_EOFDIR = -47,
    /*  */
    /*
    XPNERR_NULL = -3,
    XPNERR_FILESERVER = -4,
    XPNERR_CONNECT = -7,
    XPNERR_FILEOPEN = -8,
    XPNERR_FILEMODE = -9,
    XPNERR_FORMAT = -10,
    XPNERR_DISTSERVER = -11,
    XPNERR_DIRMNT = -12,
    XPNERR_SVRMOUNT = -14,
    XPNERR_DIRMOUNT = -15,
    XPNERR_FULLTABLEFILE = -18,
    XPNERR_NOTENTRY = -24,
    XPNERR_FULLTABLEINIT = -25,
    XPNERR_FULLTABLEDIR = -26,
    XPNERR_NODEFINED = -29,
    XPNERR_NORESOLVHOST = -31,
    XPNERR_OFFSET = -32,
    XPNERR_ISDIR = -34,
    XPNERR_ISFILE = -42,
    XPNERR_DIRMOUNTPART = -37,
    XPNERR_INTERNAL = -38,
    XPNERR_DISTPART = -39,
    XPNERR_FULLTABLEHEADER = -41,
    XPNERR_EMPTYTABLE = -43,
    XPNERR_INVALIDFILE = -45,
    XPNERR_INVALIDCAB = -46,
    XPNERR_EOFDIR = -47,
    XPNERR_FILEXIST = -48,
    XPNERR_NOUNLOCK = -49,
    XPNERR_NOLOCK = -50,
    XPNERR_PATHFORMAT = -51,
    XPNERR_INVALARG = -52,
    XPNERR_NOIMPLEMENTED = -53,
    XPNERR_USED = -54,
    */
  };
  typedef enum nfs_stat nfs_stat;

  enum ftype {
    NFNON = 0,
    NFREG = 1,
    NFDIR = 2,
    NFBLK = 3,
    NFCHR = 4,
    NFLNK = 5,
  };
  typedef enum ftype ftype;

  struct timevalNfs {
    u_long seconds;
    u_long useconds;
  };
  typedef struct timevalNfs timevalNfs;

  struct fattr {
    ftype type;
    u_long mode;
    u_long nlink;
    u_long uid;
    u_long gid;
    u_long size;
    u_long blocksize;
    u_long rdev;
    u_long blocks;
    u_long fsid;
    u_long fileid;
    timevalNfs atime;
    timevalNfs mtime;
    timevalNfs ctime;
  };
  typedef struct fattr fattr;

  struct sattr {
    u_long mode;
    u_long uid;
    u_long gid;
    u_long size;
    timevalNfs atime;
    timevalNfs mtime;
  };
  typedef struct sattr sattr;

  struct attrstat {
    nfs_stat status;
    union {
      fattr attributes;
    } attrstat_u;
  };
  typedef struct attrstat attrstat;

  struct diropargs {
    fhandle dir;
    filename name;
  };
  typedef struct diropargs diropargs;

  struct diropok {
    fhandle file;
    fattr attributes;
  };
  typedef struct diropok diropok;

  struct diropres {
    nfs_stat status;
    union {
      diropok fhand_attr;
    } diropres_u;
  };
  typedef struct diropres diropres;

  struct sattrargs {
    fhandle file;
    sattr attributes;
  };
  typedef struct sattrargs sattrargs;

  struct readlinkres {
    nfs_stat status;
    union {
      path data;
    } readlinkres_u;
  };
  typedef struct readlinkres readlinkres;

  struct readargs {
    fhandle file;
    u_long offset;
    u_long count;
    u_long totalcount;
  };
  typedef struct readargs readargs;

  struct datosRes {
    fattr attributes;
    nfsdata data;
  };
  typedef struct datosRes datosRes;

  struct readres {
    nfs_stat status;
    union {
      datosRes fich_read;
    } readres_u;
  };
  typedef struct readres readres;

  struct writeargs {
    fhandle file;
    u_long beginoffset;
    u_long offset;
    u_long totalcount;
    nfsdata data;
  };
  typedef struct writeargs writeargs;

  struct createargs {
    diropargs where;
    sattr attributes;
  };
  typedef struct createargs createargs;

  struct renameargs {
    diropargs from;
    diropargs to;
  };
  typedef struct renameargs renameargs;

  struct linkargs {
    fhandle from;
    diropargs to;
  };
  typedef struct linkargs linkargs;

  struct symlinkargs {
    diropargs from;
    path to;
    sattr attributes;
  };
  typedef struct symlinkargs symlinkargs;

  struct readdirargs {
    fhandle dir;
    nfscookie cookie;
    u_long count;
  };
  typedef struct readdirargs readdirargs;

  struct entry {
    u_long fileid;
    filename name;
    nfscookie cookie;
    struct entry *nextentry;
  };
  typedef struct entry entry;

  struct readdirok {
    entry *entries;
    bool_t eof;
  };
  typedef struct readdirok readdirok;

  struct readdirres {
    nfs_stat status;
    union {
      readdirok entradasDir;
    } readdirres_u;
  };
  typedef struct readdirres readdirres;

  struct nfs_info {
    u_long tsize;
    u_long bsize;
    u_long blocks;
    u_long bfree;
    u_long bavail;
  };
  typedef struct nfs_info nfs_info;

  struct statfsres {
    nfs_stat status;
    union {
      struct nfs_info inf;
    } statfsres_u;
  };
  typedef struct statfsres statfsres;





  #if defined(__STDC__) || defined(__cplusplus)
  #define MOUNTPROC_NULL 0
  extern  enum clnt_stat mountproc_null_1(void *, void *, CLIENT *);
  extern  bool_t mountproc_null_1_svc(void *, void *, struct svc_req *);
  #define MOUNTPROC_MNT 1
  extern  enum clnt_stat mountproc_mnt_1(nfs_dirpath *, fhstatus *, CLIENT *);
  extern  bool_t mountproc_mnt_1_svc(nfs_dirpath *, fhstatus *, struct svc_req *);
  #define MOUNTPROC_DUMP 2
  extern  enum clnt_stat mountproc_dump_1(void *, mountlist *, CLIENT *);
  extern  bool_t mountproc_dump_1_svc(void *, mountlist *, struct svc_req *);
  #define MOUNTPROC_UMNT 3
  extern  enum clnt_stat mountproc_umnt_1(nfs_dirpath *, void *, CLIENT *);
  extern  bool_t mountproc_umnt_1_svc(nfs_dirpath *, void *, struct svc_req *);
  #define MOUNTPROC_UMNTALL 4
  extern  enum clnt_stat mountproc_umntall_1(void *, void *, CLIENT *);
  extern  bool_t mountproc_umntall_1_svc(void *, void *, struct svc_req *);
  #define MOUNTPROC_EXPORT 5
  extern  enum clnt_stat mountproc_export_1(void *, exports *, CLIENT *);
  extern  bool_t mountproc_export_1_svc(void *, exports *, struct svc_req *);
  extern int mountprog_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

  #else /* K&R C */
  #define MOUNTPROC_NULL 0
  extern  enum clnt_stat mountproc_null_1();
  extern  bool_t mountproc_null_1_svc();
  #define MOUNTPROC_MNT 1
  extern  enum clnt_stat mountproc_mnt_1();
  extern  bool_t mountproc_mnt_1_svc();
  #define MOUNTPROC_DUMP 2
  extern  enum clnt_stat mountproc_dump_1();
  extern  bool_t mountproc_dump_1_svc();
  #define MOUNTPROC_UMNT 3
  extern  enum clnt_stat mountproc_umnt_1();
  extern  bool_t mountproc_umnt_1_svc();
  #define MOUNTPROC_UMNTALL 4
  extern  enum clnt_stat mountproc_umntall_1();
  extern  bool_t mountproc_umntall_1_svc();
  #define MOUNTPROC_EXPORT 5
  extern  enum clnt_stat mountproc_export_1();
  extern  bool_t mountproc_export_1_svc();
  extern int mountprog_1_freeresult ();
  #endif /* K&R C */




  #if defined(__STDC__) || defined(__cplusplus)
  #define NFSPROC_NULL 0
  extern  enum clnt_stat nfsproc_null_2(void *, void *, CLIENT *);
  extern  bool_t nfsproc_null_2_svc(void *, void *, struct svc_req *);
  #define NFSPROC_GETATTR 1
  extern  enum clnt_stat nfsproc_getattr_2(char *, attrstat *, CLIENT *);
  extern  bool_t nfsproc_getattr_2_svc(char *, attrstat *, struct svc_req *);
  #define NFSPROC_SETATTR 2
  extern  enum clnt_stat nfsproc_setattr_2(sattrargs *, attrstat *, CLIENT *);
  extern  bool_t nfsproc_setattr_2_svc(sattrargs *, attrstat *, struct svc_req *);
  #define NFSPROC_ROOT 3
  extern  enum clnt_stat nfsproc_root_2(void *, void *, CLIENT *);
  extern  bool_t nfsproc_root_2_svc(void *, void *, struct svc_req *);
  #define NFSPROC_LOOKUP 4
  extern  enum clnt_stat nfsproc_lookup_2(diropargs *, diropres *, CLIENT *);
  extern  bool_t nfsproc_lookup_2_svc(diropargs *, diropres *, struct svc_req *);
  #define NFSPROC_READLINK 5
  extern  enum clnt_stat nfsproc_readlink_2(char *, readlinkres *, CLIENT *);
  extern  bool_t nfsproc_readlink_2_svc(char *, readlinkres *, struct svc_req *);
  #define NFSPROC_READ 6
  extern  enum clnt_stat nfsproc_read_2(readargs *, readres *, CLIENT *);
  extern  bool_t nfsproc_read_2_svc(readargs *, readres *, struct svc_req *);
  #define NFSPROC_WRITECACHE 7
  extern  enum clnt_stat nfsproc_writecache_2(void *, void *, CLIENT *);
  extern  bool_t nfsproc_writecache_2_svc(void *, void *, struct svc_req *);
  #define NFSPROC_WRITE 8
  extern  enum clnt_stat nfsproc_write_2(writeargs *, attrstat *, CLIENT *);
  extern  bool_t nfsproc_write_2_svc(writeargs *, attrstat *, struct svc_req *);
  #define NFSPROC_CREATE 9
  extern  enum clnt_stat nfsproc_create_2(createargs *, diropres *, CLIENT *);
  extern  bool_t nfsproc_create_2_svc(createargs *, diropres *, struct svc_req *);
  #define NFSPROC_REMOVE 10
  extern  enum clnt_stat nfsproc_remove_2(diropargs *, nfs_stat *, CLIENT *);
  extern  bool_t nfsproc_remove_2_svc(diropargs *, nfs_stat *, struct svc_req *);
  #define NFSPROC_RENAME 11
  extern  enum clnt_stat nfsproc_rename_2(renameargs *, nfs_stat *, CLIENT *);
  extern  bool_t nfsproc_rename_2_svc(renameargs *, nfs_stat *, struct svc_req *);
  #define NFSPROC_LINK 12
  extern  enum clnt_stat nfsproc_link_2(linkargs *, nfs_stat *, CLIENT *);
  extern  bool_t nfsproc_link_2_svc(linkargs *, nfs_stat *, struct svc_req *);
  #define NFSPROC_SYMLINK 13
  extern  enum clnt_stat nfsproc_symlink_2(symlinkargs *, nfs_stat *, CLIENT *);
  extern  bool_t nfsproc_symlink_2_svc(symlinkargs *, nfs_stat *, struct svc_req *);
  #define NFSPROC_MKDIR 14
  extern  enum clnt_stat nfsproc_mkdir_2(createargs *, diropres *, CLIENT *);
  extern  bool_t nfsproc_mkdir_2_svc(createargs *, diropres *, struct svc_req *);
  #define NFSPROC_RMDIR 15
  extern  enum clnt_stat nfsproc_rmdir_2(diropargs *, nfs_stat *, CLIENT *);
  extern  bool_t nfsproc_rmdir_2_svc(diropargs *, nfs_stat *, struct svc_req *);
  #define NFSPROC_READDIR 16
  extern  enum clnt_stat nfsproc_readdir_2(readdirargs *, readdirres *, CLIENT *);
  extern  bool_t nfsproc_readdir_2_svc(readdirargs *, readdirres *, struct svc_req *);
  extern int nfs_program_2_freeresult (SVCXPRT *, xdrproc_t, caddr_t);
  #define NFSPROC_STATFS 17
  extern  enum clnt_stat nfsproc_statfs_2(char *, statfsres *, CLIENT *);
  extern  bool_t nfsproc_statfs_2_svc(char *, statfsres *, struct svc_req *);
  extern int nfs_program_2_freeresult (SVCXPRT *, xdrproc_t, caddr_t);


  #else /* K&R C */
  #define NFSPROC_NULL 0
  extern  enum clnt_stat nfsproc_null_2();
  extern  bool_t nfsproc_null_2_svc();
  #define NFSPROC_GETATTR 1
  extern  enum clnt_stat nfsproc_getattr_2();
  extern  bool_t nfsproc_getattr_2_svc();
  #define NFSPROC_SETATTR 2
  extern  enum clnt_stat nfsproc_setattr_2();
  extern  bool_t nfsproc_setattr_2_svc();
  #define NFSPROC_ROOT 3
  extern  enum clnt_stat nfsproc_root_2();
  extern  bool_t nfsproc_root_2_svc();
  #define NFSPROC_LOOKUP 4
  extern  enum clnt_stat nfsproc_lookup_2();
  extern  bool_t nfsproc_lookup_2_svc();
  #define NFSPROC_READLINK 5
  extern  enum clnt_stat nfsproc_readlink_2();
  extern  bool_t nfsproc_readlink_2_svc();
  #define NFSPROC_READ 6
  extern  enum clnt_stat nfsproc_read_2();
  extern  bool_t nfsproc_read_2_svc();
  #define NFSPROC_WRITECACHE 7
  extern  enum clnt_stat nfsproc_writecache_2();
  extern  bool_t nfsproc_writecache_2_svc();
  #define NFSPROC_WRITE 8
  extern  enum clnt_stat nfsproc_write_2();
  extern  bool_t nfsproc_write_2_svc();
  #define NFSPROC_CREATE 9
  extern  enum clnt_stat nfsproc_create_2();
  extern  bool_t nfsproc_create_2_svc();
  #define NFSPROC_REMOVE 10
  extern  enum clnt_stat nfsproc_remove_2();
  extern  bool_t nfsproc_remove_2_svc();
  #define NFSPROC_RENAME 11
  extern  enum clnt_stat nfsproc_rename_2();
  extern  bool_t nfsproc_rename_2_svc();
  #define NFSPROC_LINK 12
  extern  enum clnt_stat nfsproc_link_2();
  extern  bool_t nfsproc_link_2_svc();
  #define NFSPROC_SYMLINK 13
  extern  enum clnt_stat nfsproc_symlink_2();
  extern  bool_t nfsproc_symlink_2_svc();
  #define NFSPROC_MKDIR 14
  extern  enum clnt_stat nfsproc_mkdir_2();
  extern  bool_t nfsproc_mkdir_2_svc();
  #define NFSPROC_RMDIR 15
  extern  enum clnt_stat nfsproc_rmdir_2();
  extern  bool_t nfsproc_rmdir_2_svc();
  #define NFSPROC_READDIR 16
  extern  enum clnt_stat nfsproc_readdir_2();
  extern  bool_t nfsproc_readdir_2_svc();
  extern int nfs_program_2_freeresult ();
  #define NFSPROC_STATFS 17
  extern  enum clnt_stat nfsproc_statfs_2();
  extern  bool_t nfsproc_statfs_2_svc();
  extern int nfs_program_2_freeresult ();
  #endif /* K&R C */

  /* the xdr functions */

  #if defined(__STDC__) || defined(__cplusplus)
  extern  bool_t xdr_fhandle (XDR *, fhandle);
  extern  bool_t xdr_dirpath (XDR *, nfs_dirpath*);
  extern  bool_t xdr_name (XDR *, nfs_name*);
  extern  bool_t xdr_fhd (XDR *, fhd*);
  extern  bool_t xdr_fhstatus (XDR *, fhstatus*);
  extern  bool_t xdr_mountlist (XDR *, mountlist*);
  extern  bool_t xdr_mountbody (XDR *, mountbody*);
  extern  bool_t xdr_groups (XDR *, groups*);
  extern  bool_t xdr_groupnode (XDR *, groupnode*);
  extern  bool_t xdr_exports (XDR *, exports*);
  extern  bool_t xdr_exportnode (XDR *, exportnode*);
  extern  bool_t xdr_filename (XDR *, filename*);
  extern  bool_t xdr_path (XDR *, path*);
  extern  bool_t xdr_nfscookie (XDR *, nfscookie);
  extern  bool_t xdr_nfsdata (XDR *, nfsdata*);
  extern  bool_t xdr_nfs_stat (XDR *, nfs_stat*);
  extern  bool_t xdr_ftype (XDR *, ftype*);
  extern  bool_t xdr_timevalNfs (XDR *, timevalNfs*);
  extern  bool_t xdr_fattr (XDR *, fattr*);
  extern  bool_t xdr_sattr (XDR *, sattr*);
  extern  bool_t xdr_attrstat (XDR *, attrstat*);
  extern  bool_t xdr_diropargs (XDR *, diropargs*);
  extern  bool_t xdr_diropok (XDR *, diropok*);
  extern  bool_t xdr_diropres (XDR *, diropres*);
  extern  bool_t xdr_sattrargs (XDR *, sattrargs*);
  extern  bool_t xdr_readlinkres (XDR *, readlinkres*);
  extern  bool_t xdr_readargs (XDR *, readargs*);
  extern  bool_t xdr_datosRes (XDR *, datosRes*);
  extern  bool_t xdr_readres (XDR *, readres*);
  extern  bool_t xdr_writeargs (XDR *, writeargs*);
  extern  bool_t xdr_createargs (XDR *, createargs*);
  extern  bool_t xdr_renameargs (XDR *, renameargs*);
  extern  bool_t xdr_linkargs (XDR *, linkargs*);
  extern  bool_t xdr_symlinkargs (XDR *, symlinkargs*);
  extern  bool_t xdr_readdirargs (XDR *, readdirargs*);
  extern  bool_t xdr_entry (XDR *, entry*);
  extern  bool_t xdr_readdirok (XDR *, readdirok*);
  extern  bool_t xdr_readdirres (XDR *, readdirres*);
  extern  bool_t xdr_nfs_info (XDR *, nfs_info*);
  extern  bool_t xdr_statfsres (XDR *, statfsres*);

  #else /* K&R C */
  extern bool_t xdr_fhandle ();
  extern bool_t xdr_dirpath ();
  extern bool_t xdr_name ();
  extern bool_t xdr_fhd ();
  extern bool_t xdr_fhstatus ();
  extern bool_t xdr_mountlist ();
  extern bool_t xdr_mountbody ();
  extern bool_t xdr_groups ();
  extern bool_t xdr_groupnode ();
  extern bool_t xdr_exports ();
  extern bool_t xdr_exportnode ();
  extern bool_t xdr_filename ();
  extern bool_t xdr_path ();
  extern bool_t xdr_nfscookie ();
  extern bool_t xdr_nfsdata ();
  extern bool_t xdr_nfs_stat ();
  extern bool_t xdr_ftype ();
  extern bool_t xdr_timevalNfs ();
  extern bool_t xdr_fattr ();
  extern bool_t xdr_sattr ();
  extern bool_t xdr_attrstat ();
  extern bool_t xdr_diropargs ();
  extern bool_t xdr_diropok ();
  extern bool_t xdr_diropres ();
  extern bool_t xdr_sattrargs ();
  extern bool_t xdr_readlinkres ();
  extern bool_t xdr_readargs ();
  extern bool_t xdr_datosRes ();
  extern bool_t xdr_readres ();
  extern bool_t xdr_writeargs ();
  extern bool_t xdr_createargs ();
  extern bool_t xdr_renameargs ();
  extern bool_t xdr_linkargs ();
  extern bool_t xdr_symlinkargs ();
  extern bool_t xdr_readdirargs ();
  extern bool_t xdr_entry ();
  extern bool_t xdr_readdirok ();
  extern bool_t xdr_readdirres ();

  #endif /* K&R C */

  #ifdef __cplusplus
  }
  #endif


  CLIENT* create_connection_mount(char *name, int type); 

  void close_connection_mount(CLIENT *cl);
   
  int nfs_mount(char *dir, fhandle fhand, CLIENT *cl );

  int nfs_umount(char *path, CLIENT *cl);

  int nfs_export(exports* ,CLIENT *cl);




  CLIENT* create_connection_nfs(char *name, int type);

  void close_connection_nfs(CLIENT *cl);

  int nfs_getattr(fhandle fh, fattr *fatt, CLIENT *cl);

  int nfs_setattr(fhandle fh, fattr *fatt, CLIENT *cl);

  int nfs_lookup(fhandle fhin, char *path , fhandle fhout, fattr *att, CLIENT *cl);

  ssize_t nfs_read(fhandle fh, void *data, off_t offset, size_t size, CLIENT *cl);

  ssize_t nfs_write(fhandle fh, void *data, off_t offset, size_t size, CLIENT *cl);

  int nfs_create(fhandle fhin, char *file, mode_t mode, fhandle fhout, fattr *at, CLIENT *cl);

  int nfs_remove(fhandle fh, char *file, CLIENT *cl);

  int nfs_rename(fhandle fh, char *name, fhandle fhR, char *nameR, CLIENT *cl);

  int nfs_mkdir(fhandle fhin, char *dir, mode_t mode, fhandle fhout, fattr *att, CLIENT *cl);

  int nfs_rmdir(fhandle fh, char *dir, CLIENT *cl);

  int nfs_readdir(fhandle fh, nfscookie cookie, char *entry, CLIENT *cl);
    
  int nfs_statfs(fhandle arg, struct nfs_info *inf, CLIENT *cl);


#endif /* _NFS_H */
