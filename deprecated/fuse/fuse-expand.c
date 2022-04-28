/*
    gcc -Wall -Wextra `pkg-config fuse --cflags --libs` fuse-expand.c -o fuse-expand
    gcc -D_FILE_OFFSET_BITS=64 -DFUSE_USE_VERSION=22 -c -o fuse-expand.o fuse-expand.c
    gcc -o fuse-expand fuse-expand.o /usr/lib/libfuse.so.2.4.2 -lpthread -Wl,--rpath -Wl,/usr/lib

    Usage: ./fuse-expand /storage/fuse -d -s -o xpnpart=/<partition_name> -o big_writes -o allow_other -o no_remote_lock -o intr -o uid=1001 -o gid=100
*/

#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stddef.h> // For: offsetof

#include <xpn.h>
#include <xpn_debug.h>


extern int errno;

// Expand partition name
const char *xpn_partition_option = "xpnpart=/";
int xpn_partition_option_len = 0;
//char *xpn_partition_prefix = NULL;

struct myfs_config {
     char *xpn_partition_prefix;
};

struct myfs_config conf;


void print_stat (struct stat *st) {
	if (st == NULL) {
		printf("struct stat is NULL\n");
		return;
	}
	
	printf("struct stat {\n");
	printf("    dev_t     st_dev     = %lu\n", (unsigned long) st->st_dev);     /* ID of device containing file */
	printf("    ino_t     st_ino     = %lu\n", (unsigned long) st->st_ino);     /* inode number */
	printf("    mode_t    st_mode    = %d\n",                  st->st_mode);    /* protection */
	printf("    nlink_t   st_nlink   = %lu\n", (unsigned long) st->st_nlink);   /* number of hard links */
	printf("    uid_t     st_uid     = %d\n",                  st->st_uid);     /* user ID of owner */
	printf("    gid_t     st_gid     = %d\n",                  st->st_gid);     /* group ID of owner */
	printf("    dev_t     st_rdev    = %lu\n", (unsigned long) st->st_rdev);    /* device ID (if special file) */
	printf("    off_t     st_size    = %lld\n",    (long long) st->st_size);    /* total size, in bytes */
	printf("    blksize_t st_blksize = %ld\n",                 st->st_blksize); /* blocksize for file system I/O */
	printf("    blkcnt_t  st_blocks  = %lld\n",    (long long) st->st_blocks);  /* number of 512B blocks allocated */
	printf("    time_t    st_atime   = %lu\n",                 st->st_atime);   /* time of last access */
	printf("    time_t    st_mtime   = %lu\n",                 st->st_mtime);   /* time of last modification */
	printf("    time_t    st_ctime   = %lu\n",                 st->st_ctime);   /* time of last status change */
	printf("};\n");
}

void print_dirent (struct dirent *dp) {
	if (dp == NULL) {
		printf("struct dirent is NULL\n");
		return;
	}
	
	printf("fuse: struct dirent {            %lu-%p\n",  (unsigned long) sizeof(struct dirent), dp);
	printf("    ino_t          d_ino       = %lu+%ld\n", (unsigned long) sizeof(ino_t), (long int)&dp->d_ino-(long int)dp);
	printf("    off_t          d_off       = %lu+%ld\n", (unsigned long) sizeof(off_t), (long int)&dp->d_off-(long int)dp);
	printf("    unsigned short d_reclen    = %lu+%ld\n", (unsigned long) sizeof(unsigned short), (long int)&dp->d_reclen-(long int)dp);
	printf("    unsigned char  d_type      = %lu+%ld\n", (unsigned long) sizeof(unsigned char), (long int)&dp->d_type-(long int)dp);
	printf("    char           d_name[256] = %lu+%ld\n", (unsigned long) sizeof(struct dirent)-sizeof(ino_t)-sizeof(off_t)-sizeof(unsigned short)-sizeof(unsigned char), (long int)&dp->d_name-(long int)dp);
	printf("}\n");
	printf("dp=%p, dp->d_name=%p='%s'\n", dp, dp->d_name, dp->d_name);
	printf("\n");
}

char *create_xpn_path (const char *path)
{
	char *xpn_path;
	
	xpn_path=(char *)malloc(strlen(conf.xpn_partition_prefix)+strlen(path)+1);
	memset(xpn_path, 0, strlen(conf.xpn_partition_prefix)+strlen(path)+1);
	xpn_path=strcat(xpn_path, conf.xpn_partition_prefix);
	xpn_path=strcat(xpn_path, path);
	
	//printf("Antes: path=%s\nDespues: xpn_path=%s\n", path, xpn_path);
	
	return xpn_path;
}
/***********************************************************************************/
static int expand_fgetattr(const char *path, struct stat *stbuf,
			   __attribute__((__unused__)) struct fuse_file_info *fi)
{
	int res = 0;
	char *xpn_path;
	
	XPN_DEBUG_BEGIN_ARGS1(path)
	
	memset(stbuf, 0, sizeof(struct stat));
	xpn_path = create_xpn_path(path);
	res = xpn_stat(xpn_path, stbuf);
	free(xpn_path);
	if (res < 0) {
		//printf("expand_fgetattr: xpn_stat(%s)=%d\n", xpn_path, res);
		//print_stat(stbuf);
		//printf("FUSE: sizeof(struct stat)=%d\n", sizeof(struct stat));
		res = -errno;
		XPN_DEBUG_END
		return res;
	}
	
	res = 0;
	XPN_DEBUG_END
	return res;
}

static int expand_getattr (const char *path, struct stat *stbuf)
{
	int res = 0;
	char *xpn_path;
	
	XPN_DEBUG_BEGIN_ARGS1(path)
	
	memset(stbuf, 0, sizeof(struct stat));
	
	stat(".", stbuf);
	//print_stat(stbuf);

	xpn_path = create_xpn_path(path);
	res = xpn_stat(xpn_path, stbuf);
	free(xpn_path);
	if (res < 0) {
		//printf("expand_getattr: xpn_stat(%s)=%d\n", xpn_path, res);
		//print_stat(stbuf);
		//printf("FUSE: sizeof(struct stat)=%d\n", sizeof(struct stat));
		res = -errno;
		XPN_DEBUG_END
		return res;
	}
	
	if (S_ISDIR(stbuf->st_mode))
		stbuf->st_size = 4096;
	stbuf->st_mtime = stbuf->st_atime;
	stbuf->st_ctime = stbuf->st_atime;
	//print_stat(stbuf);

	res = 0;
	XPN_DEBUG_END
	return res;
}

static int expand_chmod(__attribute__((__unused__)) const char *path, __attribute__((__unused__)) mode_t mode)
{
	return -ENOSYS;
}

static int expand_chown(__attribute__((__unused__)) const char *path, __attribute__((__unused__)) uid_t uid, __attribute__((__unused__)) gid_t gid)
{
	return -ENOSYS;
}

static int expand_truncate(const char *path, __attribute__((__unused__)) off_t size)
{
        char *xpn_path;
        int res;
	int mode = 0777;

        XPN_DEBUG_BEGIN_ARGS1(path)

        xpn_path = create_xpn_path(path);
        res = xpn_unlink(xpn_path);
        res = xpn_creat(xpn_path, mode);
        free(xpn_path);

        if (res < 0) {
                res = -errno;
                XPN_DEBUG_END
                return res;
        }

        res = 0;
        XPN_DEBUG_END
        return res;
}

static int expand_ftruncate(__attribute__((__unused__)) const char *path, __attribute__((__unused__)) off_t size,
			    __attribute__((__unused__)) struct fuse_file_info *fi)
{
	return -ENOSYS;
}

static int expand_utimens(__attribute__((__unused__)) const char *path, __attribute__((__unused__)) const struct timespec ts[2])
{
	return -ENOSYS;
}

/***********************************************************************************/

static int expand_open (const char *path, struct fuse_file_info *fi)
{
	char *xpn_path;
	int res;
	
	XPN_DEBUG_BEGIN_ARGS1(path)
	
	xpn_path = create_xpn_path(path);
	res = xpn_open(xpn_path, fi->flags);
	fi->fh = res;
	free(xpn_path);
	
	if (res < 0) {
		res = -errno;
		XPN_DEBUG_END
		return res;
	}
	
	res = 0;
	XPN_DEBUG_END
	return res;
}

static int expand_create (const char *path, mode_t mode, struct fuse_file_info *fi)
{
	char *xpn_path;
	int res;

	XPN_DEBUG_BEGIN_ARGS1(path)
	
	xpn_path = create_xpn_path(path);
	res = xpn_creat(xpn_path, mode);
	fi->fh = res;
	free(xpn_path);
	
	if (res < 0) {
		res = -errno;
		XPN_DEBUG_END
		return res;
	}
	
	XPN_DEBUG_END
	return res;
}

static int expand_release(const char *path, struct fuse_file_info *fi)
{
	int res;

	XPN_DEBUG_BEGIN_ARGS1(path)
	
	res = xpn_close(fi->fh);
	if (res < 0){
		res = -errno;
		XPN_DEBUG_END
		return res;
	}
	
	XPN_DEBUG_END
	return res;
}

static int expand_unlink(const char *path)
{
	char *xpn_path;
	int res;

	XPN_DEBUG_BEGIN_ARGS1(path)
	
	xpn_path = create_xpn_path(path);
	res = xpn_unlink(xpn_path);
	free(xpn_path);

	if (res < 0) {
		res = -errno;
		XPN_DEBUG_END
		return res;
	}
	
	XPN_DEBUG_END
	return res;
}

static int expand_rename(__attribute__((__unused__)) const char *from, __attribute__((__unused__)) const char *to)
{
	return -ENOSYS;
}

static int expand_fsync(__attribute__((__unused__)) const char *path, __attribute__((__unused__)) int isdatasync,
			__attribute__((__unused__)) struct fuse_file_info *fi)
{
	return -ENOSYS;
}

static int expand_flush(__attribute__((__unused__)) const char *path, __attribute__((__unused__)) struct fuse_file_info *fi)
{
	return -ENOSYS;
}

static int expand_statfs(__attribute__((__unused__)) const char *path, __attribute__((__unused__)) struct statvfs *stbuf)
{
	return -ENOSYS;
}

static int expand_access(__attribute__((__unused__)) const char *path, __attribute__((__unused__)) int mask)
{
	return -ENOSYS;
}

static int expand_mknod(const char *path, mode_t mode, __attribute__((__unused__)) dev_t rdev)
{
	int res;
	char *xpn_path;
	
	XPN_DEBUG_BEGIN_ARGS1(path)
	
	if (S_ISREG(mode)) {
		xpn_path = create_xpn_path(path);
		res = xpn_creat(xpn_path, mode);
		free(xpn_path);
		if (res>=0) {
			xpn_close(res);
		} else {
			res = -errno;
			XPN_DEBUG_END
			return res;
		}
		return 0;
	}
	
	res = -ENOSYS;
	XPN_DEBUG_END
	return res;
}
/***********************************************************************************/

static int expand_read (const char *path, char *buf, size_t size, off_t offset,
                       struct fuse_file_info *fi)
{
	int res;
	char *xpn_path;
	
	XPN_DEBUG_BEGIN_ARGS1(path)
	
	xpn_path = create_xpn_path(path);
	//fi->fh = xpn_open(xpn_path, fi->flags);
	res = xpn_pread(fi->fh, buf, size, offset);
	//xpn_close(fi->fh);
	free(xpn_path);
	
	if (res < 0){
		res = -errno;
		XPN_DEBUG_END
		return res;
	}
	
	res = size;
	XPN_DEBUG_END
	return res;
}

static int expand_write(const char *path, const char *buf, size_t size,
		     off_t offset, struct fuse_file_info *fi)
{
	int res;
	char *xpn_path;
	
	XPN_DEBUG_BEGIN_ARGS1(path)
	
	xpn_path = create_xpn_path(path);
	//fi->fh = xpn_open(xpn_path, fi->flags);
	res = xpn_pwrite(fi->fh, buf, size, offset);
	//xpn_close(fi->fh);
	free(xpn_path);
	
	if (res < 0){
		res = -errno;
		XPN_DEBUG_END
		return res;
	}
	
	res = size;
	XPN_DEBUG_END
	return res;
}

/***********************************************************************************/
static int expand_opendir (const char *path, struct fuse_file_info *fi)
{
	int res;
	char *xpn_path;
	DIR *dirp;
	
	XPN_DEBUG_BEGIN_ARGS1(path)
	
	xpn_path = create_xpn_path(path);
	dirp = xpn_opendir(xpn_path);
	//fi->fh = (uint64_t)dirp;
	fi->fh = (unsigned long)dirp; // /usr/include/fuse/fuse_common_compat.h: unsigned long fh;
	free(xpn_path);
	
	if (dirp == NULL) {
		res = -errno;
		XPN_DEBUG_END
		return res;
	}
	
	res = 0;
	XPN_DEBUG_END
	return res;
}

static int expand_readdir (const char *path, void *buf, fuse_fill_dir_t filler,
			   __attribute__((__unused__)) off_t offset, struct fuse_file_info *fi)
{
	int res;
	DIR *dirp;
	struct dirent *dp;
	
	XPN_DEBUG_BEGIN_ARGS1(path)
	
	dirp = (DIR *) (uintptr_t) fi->fh;	// I think I could finally get rid of this warning!!!
										// /usr/include/fuse/fuse_common.h: uint64_t fh;
										// http://www.cs.nmsu.edu/~pfeiffer/fuse-tutorial/unclear.html
										// http://www.koders.com/c/fid3D9071A84BBCD4179231F161819AAAEF37D4E3E6.aspx?s=cdefs
	while ((dp = xpn_readdir(dirp))) {
		filler(buf, dp->d_name, NULL, 0);
		//printf("dp->d_name = %s\n", dp->d_name);
		free(dp);
	}
	
	res = 0;
	XPN_DEBUG_END
	return res;
}

static int expand_releasedir(const char *path, struct fuse_file_info *fi)
{
	int res;
	DIR *dirp;

	XPN_DEBUG_BEGIN_ARGS1(path)

	dirp = (DIR *)((unsigned long)fi->fh); // I think I could finally get rid of this warning!!! /usr/include/fuse/fuse_common.h: uint64_t fh http://www.koders.com/c/fid3D9071A84BBCD4179231F161819AAAEF37D4E3E6.aspx?s=cdefs
	res = xpn_closedir(dirp);

	if (res < 0) {
		res = -errno;
		XPN_DEBUG_END
		return res;
	}

	XPN_DEBUG_END
	return res;
}

static int expand_mkdir (const char *path, mode_t mode)
{
	int res;
	char *xpn_path;
	
	XPN_DEBUG_BEGIN_ARGS1(path)
	
	xpn_path = create_xpn_path(path);
	res = xpn_mkdir(xpn_path, mode);
	free(xpn_path);
	
	if (res < 0) {
		res = -errno;
		XPN_DEBUG_END
		return res;
	}
	
	XPN_DEBUG_END
	return res;
}

static int expand_rmdir(const char *path)
{
	int res;
	char *xpn_path;
	
	XPN_DEBUG_BEGIN_ARGS1(path)
	
	xpn_path = create_xpn_path(path);
	res = xpn_rmdir(xpn_path);
	free(xpn_path);
	
	if (res < 0) {
		res = -errno;
		XPN_DEBUG_END
		return res;
	}
	
	XPN_DEBUG_END
	return res;
}

/***********************************************************************************/
static void *expand_init (__attribute__((__unused__)) struct fuse_conn_info *conn)
{
	int res;

	xpn_debug=1;
	XPN_DEBUG_BEGIN
	
	if (conf.xpn_partition_prefix == NULL) {
		exit(1);
	}

	res = xpn_init();
	
	XPN_DEBUG_END
	xpn_debug=0;
	return (void *)NULL;
}

static void expand_destroy (__attribute__((__unused__)) void *private_data)
{
	int res;
	
	XPN_DEBUG_BEGIN
	
	res = xpn_destroy();
	
	XPN_DEBUG_END
}

/***********************************************************************************/
static struct fuse_operations expand_oper = {
	.getattr	= expand_getattr,
	.fgetattr	= expand_fgetattr,
	
	.open		= expand_open,
	.create		= expand_create,
	.read		= expand_read,
	.write		= expand_write,
	.release	= expand_release,
	.unlink		= expand_unlink,
	.truncate	= expand_truncate,
	.ftruncate	= expand_ftruncate,
	
	.mknod		= expand_mknod,
	.access		= expand_access,
	.rename		= expand_rename,
	.chmod		= expand_chmod,
	.chown		= expand_chown,
	.utimens	= expand_utimens,
	
	.statfs		= expand_statfs,
	.flush		= expand_flush,
	.fsync		= expand_fsync,
	
	.opendir	= expand_opendir,
	.readdir	= expand_readdir,
	.releasedir	= expand_releasedir,
	.mkdir		= expand_mkdir,
	.rmdir		= expand_rmdir,
	
	.init		= expand_init,
	.destroy	= expand_destroy,
};

#define MYFS_OPT(t, p, v) { t, offsetof(struct myfs_config, p), v }

static struct fuse_opt expand_opts[] = {
		MYFS_OPT("xpnpart=%s", xpn_partition_prefix, 0),
		FUSE_OPT_END
};

static int expand_opt_proc(__attribute__((__unused__)) void *data, const char *arg, __attribute__((__unused__)) int key, __attribute__((__unused__)) struct fuse_args *outargs)
{
	//printf("expand_opt_proc: arg='%s' key='%d'\n", arg, key);
	if (strncmp(arg, xpn_partition_option, xpn_partition_option_len) == 0) {
		int xpn_partition_value_len = strlen(&arg[xpn_partition_option_len-1]);

		if (xpn_partition_value_len > 1) {
			if (conf.xpn_partition_prefix != NULL)
				free(conf.xpn_partition_prefix);
			conf.xpn_partition_prefix = malloc(xpn_partition_value_len+2);

			strcpy(conf.xpn_partition_prefix, &arg[xpn_partition_option_len-1]);
			//printf("expand_opt_proc: xpn_partition_prefix='%s'\n", xpn_partition_prefix);
		}
	}

	return 1;
}

int main (int argc, char *argv[])
{
    struct fuse_args args = FUSE_ARGS_INIT(argc, argv);

    xpn_partition_option_len = strlen(xpn_partition_option);

	fuse_opt_parse(&args, &conf, expand_opts, expand_opt_proc);

	if (conf.xpn_partition_prefix == NULL) {
		printf("Error: you must provide an Expand partition name using '-o %s<partition_name>'\n", xpn_partition_option);
		printf("usage: %s mountpoint -o %sXPN\n", argv[0], xpn_partition_option);
	}

	printf("[fuse-expand.c][main] xpn_partition_prefix=%s\n", conf.xpn_partition_prefix);

    return fuse_main(args.argc, args.argv, &expand_oper, NULL);
}
