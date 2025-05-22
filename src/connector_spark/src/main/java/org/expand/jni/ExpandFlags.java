package org.expand.jni;

public class ExpandFlags {

	//----------------------------------------------
	//CREAT FLAGS
	//----------------------------------------------
	public long S_IRWXU;
	public long S_IRUSR;
	public long S_IWUSR;
	public long S_IXUSR;
	public long S_IRWXG;
	public long S_IRGRP;
	public long S_IWGRP;
	public long S_IXGRP;
	public long S_IRWXO;
	public long S_IROTH;
	public long S_IWOTH;
	public long S_IXOTH;

	//----------------------------------------------
	//OPEN FLAGS
	//----------------------------------------------
	public long O_WRONLY;
	public long O_RDONLY;
	public long O_RDWR;
	public long O_APPEND;
	public long O_ASYNC;
	public long O_CLOEXEC;
	public long FD_CLOEXEC;
	public long O_CREAT;
	public long O_DIRECTORY;
	public long O_EXCL;
	public long O_NOCTTY;
	public long O_NOFOLLOW;
	public long O_NONBLOCK;
	public long O_TRUNC;

	//----------------------------------------------
	//SEEK FLAGS
	//----------------------------------------------
	public long SEEK_SET;
	public long SEEK_CUR;
	public long SEEK_END;

	ExpandFlags(){}
}
