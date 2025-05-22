package org.expand.jni;

import java.nio.ByteBuffer;

public class ExpandToPosix {

	static{
		System.loadLibrary("xpn");
		System.loadLibrary("expandtoposix");
	}

	public ExpandFlags flags;

	public ExpandToPosix(){
		flags = jni_xpn_createFlags();
	}

	public native int jni_xpn_chdir(String path);

	public native int jni_xpn_chmod(String path, short mode);

	public native int jni_xpn_close(int fd);

	public native int jni_xpn_creat(String path, long mode);

	public native ExpandFlags jni_xpn_createFlags();

	public native int jni_xpn_destroy();

	public native int jni_xpn_exist(String path);

	public native Stat jni_xpn_fstat(int fd);

	public native int jni_xpn_get_block_locality(String path, long offset, String[] url_v);

	public native String jni_xpn_getcwd(String path, long size);

	public native String [] jni_xpn_getDirContent(String path);

	public native String jni_xpn_getGroupname(int id);

	public native String jni_xpn_getUsername(int id);

	public native int jni_xpn_init();

	public native int jni_xpn_isDir(int mode);

	public native long jni_xpn_lseek(int fd, long offset, long whence);

	public native int jni_xpn_mkdir(String path, short mode);

	public native int jni_xpn_open(String path, long flags);

	public native long jni_xpn_read(int fd, ByteBuffer buf, long size);

	public native int jni_xpn_rename(String src, String dst);

	public native int jni_xpn_rmdir(String path);

	public native Stat jni_xpn_stat(String path);

	public native int jni_xpn_unlink(String path);

	public native long jni_xpn_write(int fd, ByteBuffer buf, long count);

}
