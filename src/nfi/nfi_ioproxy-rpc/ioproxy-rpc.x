/* . */

typedef int64_t int64 ;
typedef uint64_t uint64 ;
typedef unsigned long uint32;
typedef long int32;
typedef string ioppath<>;
typedef uint32 fileid1;
typedef uint32 uid1;
typedef uint32 gid1;
typedef uint64 size1;
typedef uint64 offset1;
typedef uint32 mode1;
typedef uint32 perm1;
typedef uint32 count1;
typedef uint32 status1;

/* a directory entry */
typedef string nametype<MAXNAMELEN>;

 

/* a link in the listing */
typedef struct namenode *namelist;

 

/*
 * A node in the directory listing
 */
struct namenode {
        nametype name;          /* name of directory entry */
        namelist next;          /* next entry */
};

 

/*
 * The result of a READDIR operation.
 */
union readdir_res switch (status1 errno) {
case 0:
        namelist list;  /* no error: return directory listing */
default:
        void;           /* error occurred: nothing else to return */
};


struct IOPOPEN1args{
	ioppath path;
	mode1 mode;
};
union IOPOPEN1res switch (status1 errno){
case 0:
	fileid1 fd;
default:
        void;           /* error occurred: nothing else to return */
};

struct IOPCLOSE1args{
	fileid1 fd;	
};
union IOPCLOSE1res switch (status1 errno){
case 0:
	status1 errno;
default:
        void;           /* error occurred: nothing else to return */
};

struct IOPCREAT1args{
	ioppath path;
	mode1 mode;
	perm1 perm;
};
union IOPCREAT1res switch (status1 errno){
case 0:
	fileid1 fd;
default:
        void;           /* error occurred: nothing else to return */
};

struct IOPUNLINK1args{
	ioppath path;
};
union  IOPUNLINK1res switch (status1 errno){
case 0:
	status1 errno;
default:
        void;           /* error occurred: nothing else to return */
};

struct IOPREAD1args{
	fileid1 fd;
	offset1 offset;
	count1 size;
};

struct write_res{
	count1 result;
	opaque data<>; 
};
union IOPREAD1res switch (status1 errno){
case 0:
	struct write_res res;
default:
        void;           /* error occurred: nothing else to return */
};

struct IOPWRITE1args{
	fileid1 fd;
	offset1 offset;
	count1 size;
	opaque data<>; 
};
union IOPWRITE1res switch (status1 errno){
case 0:
	count1 result;
default:
        void;           /* error occurred: nothing else to return */
};

struct IOPMKDIR1args{
	ioppath path;
	mode1 mode;
};
union IOPMKDIR1res switch (status1 errno){
case 0:
	status1 errno;
default:
        void;           /* error occurred: nothing else to return */
};


struct IOPOPENDIR1args{
	ioppath path;
};
union IOPOPENDIR1res switch (status1 errno){
case 0:
	status1 errno;
default:
        void;           /* error occurred: nothing else to return */
};

struct IOPRMDIR1args{
	ioppath path;
};
union IOPRMDIR1res switch (status1 errno){
case 0:
	status1 errno;
default:
        void;           /* error occurred: nothing else to return */
};

struct IOPREADDIR1args{
	ioppath path;
};
union IOPREADDIR1res switch (status1 errno){
case 0:
	struct readdir_res res;
default:
        void;           /* error occurred: nothing else to return */
};

struct IOPCLOSEDIR1args{
	fileid1 fd;
};
union IOPCLOSEDIR1res switch (status1 errno){
case 0:
	status1 errno;
default:
        void;           /* error occurred: nothing else to return */
};

struct IOPRENAME1args{
	ioppath pathin;
	ioppath pathout;
};
union IOPRENAME1res switch (status1 errno){
case 0:
	status1 errno;
default:
        void;           /* error occurred: nothing else to return */
};

struct IOPGETATTR1args{
	ioppath path;
};
union IOPGETATTR1res switch (status1 errno){
case 0:
	struct stat buf;
default:
        void;           /* error occurred: nothing else to return */
};

struct IOPSETATTR1args{
	ioppath path;
	struct stat buf;
};
union IOPSETATTR1res switch (status1 errno){
case 0:
	status1 errno;
default:
        void;           /* error occurred: nothing else to return */
};

struct IOPSTATFS1args{
	ioppath path; 
};
union IOPSTATFS1res switch (status1 errno){
case 0:
	struct statfs buf;
default:
        void;           /* error occurred: nothing else to return */
};

struct IOPFLUSH1args{
	ioppath path; 
};
union IOPFLUSH1res switch (status1 errno){
case 0:
	status1 errno;
default:
        void;           /* error occurred: nothing else to return */
};

struct IOPPRELOAD1args{
	ioppath path; 
};
union IOPPRELOAD1res switch (status1 errno){
case 0:
	status1 errno;
default:
        void;           /* error occurred: nothing else to return */
};

struct IOPSTATFSSERVER1args{
	ioppath path; 
};
union IOPSTATFSSERVER1res switch (status1 errno){
case 0:
	opaque data<>;
default:
        void;           /* error occurred: nothing else to return */
};

program IOPROXY_PROGRAM { 
	version IOPROXY_V1 {
	   	void 		IOPPROC1_null(void) 			= 0;
		
		IOPOPEN1res	IOPPROC1_OPEN(IOPOPEN1args)		= 1;
		IOPCLOSE1res 	IOPPROC1_CLOSE(IOPCLOSE1args) 		= 2;
		IOPCREAT1res 	IOPPROC1_CREAT(IOPCREAT1args) 		= 3;
		IOPUNLINK1res 	IOPPROC1_UNLINK(IOPUNLINK1args)		= 4;

		IOPREAD1res 	IOPPROC1_READ(IOPREAD1args) 		= 5;
		IOPWRITE1res 	IOPPROC1_WRITE(IOPWRITE1args) 		= 6;

		IOPMKDIR1res	IOPPROC1_MKDIR(IOPMKDIR1args)		= 7;
		IOPRMDIR1res	IOPPROC1_RMDIR(IOPRMDIR1args)		= 8;
		
		IOPOPENDIR1res	IOPPROC1_OPENDIR(IOPOPENDIR1args)	= 9;
		IOPREADDIR1res	IOPPROC1_READDIR(IOPREADDIR1args)	= 10;
		IOPCLOSEDIR1res	IOPPROC1_CLOSEDIR(IOPCLOSEDIR1args)	= 11;
		
		IOPRENAME1res	IOPPROC1_RENAME(IOPRENAME1args)		= 12;
		IOPGETATTR1res	IOPPROC1_GETATTR(IOPGETATTR1args)	= 13;
		IOPSETATTR1res	IOPPROC1_SETATTR(IOPSETATTR1args)	= 14;

		IOPSTATFS1res	IOPPROC1_STATFS(IOPSTATFS1args)		= 15;
		IOPFLUSH1res	IOPPROC1_FLUSH(IOPFLUSH1args)		= 16;
		IOPPRELOAD1res	IOPPROC1_PRELOAD(IOPPRELOAD1args)	= 17;
		
		
		IOPSTAFSSERVER1res	IOPPROC1_STAFSSERVER(IOPSTAFSSERVER1args)	= 18;

	} = 1; /* versión número = 1 */
} = 200123; /* programa número = 200123 */ 
