package org.expand.hadoop;

import java.net.URI;
import java.net.InetAddress;
import java.io.IOException;
import java.io.FileNotFoundException;
import java.lang.Math;
import java.util.ArrayList;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.fs.FileStatus;
import org.apache.hadoop.fs.permission.FsPermission;
import org.apache.hadoop.fs.FSDataInputStream;
import org.apache.hadoop.fs.FSDataOutputStream;
import org.apache.hadoop.fs.BlockLocation;
import org.apache.hadoop.util.Progressable;
import org.apache.hadoop.fs.PathFilter;

import org.expand.jni.ExpandToPosix;
import org.expand.jni.ExpandFlags;
import org.expand.jni.Stat;

public class Expand extends FileSystem {

	private ExpandToPosix xpn;
	private URI uri;
	private Path workingDirectory;
	public ExpandFlags flags;
	private long blksize = 134217728;
	private int bufsize = 134217728;
	private short xpn_replication = 1;
	private boolean initialized;

	public Expand(){
		this.xpn = new ExpandToPosix();
		this.uri = URI.create("xpn:///");
		this.setWorkingDirectory(new Path("/xpn"));
		this.flags = this.xpn.flags;
		this.initialized = false;
	}

	@Override
	public FSDataOutputStream append(Path f) throws IOException {
		return append(f, this.bufsize, null);
	}

	@Override
	public FSDataOutputStream append(Path f, int bufferSize, Progressable progress){
		f = removeURI(f);
		if (!exists(f)) xpn.jni_xpn_creat(f.toString(), flags.S_IRWXU | flags.S_IRWXG | flags.S_IRWXO);

		return new FSDataOutputStream(new ExpandOutputStream(f.toString(), this.bufsize, this.xpn_replication, this.blksize, true), statistics);
	}

	public void close() throws IOException {

	}

	@Override
	public FSDataOutputStream create(Path f) throws IOException {
		return create(f, FsPermission.getFileDefault(), true, this.bufsize, this.xpn_replication, this.blksize, null);
	}

	@Override
	public FSDataOutputStream create(Path f, FsPermission permission, boolean overwrite, int bufferSize, short replication, long blockSize, Progressable progress) throws IOException {
		f = removeURI(f);
		Path parent = f.getParent();

		if (exists(f)) {
			if (overwrite) delete(f, false);
			else throw new IOException("File already exists: " + f.toString());
		}else{
			if (!exists(parent)) mkdirs(parent, FsPermission.getFileDefault());
		}
		return new FSDataOutputStream(new ExpandOutputStream(f.toString(), bufsize, replication, 
					blksize, false), statistics);
	}

	@Override
	public boolean delete(Path path, boolean recursive){
		path = removeURI(path);
		
		if (!exists(path)) return false;
		if (!isDirectory(path)) return this.xpn.jni_xpn_unlink(path.toString()) == 0;
		if (!recursive) return this.xpn.jni_xpn_rmdir(path.toString()) == 0;

		String [] str = this.xpn.jni_xpn_getDirContent(path.toString());
		String deletePath;
		boolean res;

		for (int i = 0; i < str.length; i++){
			if (str[i].equals(".") || str[i].equals("..")) continue;
			res = delete(new Path(path.toString() + "/" + str[i]), true);
			if (!res) return false;
		}

		return this.xpn.jni_xpn_rmdir(path.toString()) == 0;
	}

	public boolean exists (Path path){
		path = removeURI(path);
		if (this.xpn.jni_xpn_exist(path.toString()) != 0) return false;

		return true;
	}

	@Override
	public BlockLocation[] getFileBlockLocations(FileStatus file,
									long start, long len) throws IOException {
		Path path = removeURI(file.getPath());
		if (file == null) {
			throw new IOException("File does not exist: " + path.toString());
		}

		if (start < 0 || len < 0) {
			throw new IllegalArgumentException("Invalid start or len parameter");
		}

		if (getLength(path) <= start) {
			return new BlockLocation[0];
		}

		int splits = (int) Math.ceil(len / blksize) + 1;

		BlockLocation[] blkloc = new BlockLocation[splits];

		String[][] url_v = new String[splits][xpn_replication]; 
		String[][] host = new String[splits][xpn_replication];

		for (int i = 0; i < splits; i++){
			int res = this.xpn.jni_xpn_get_block_locality(path.toString(), i * blksize, url_v[i]);

			for (int j = 0; j < xpn_replication; j++) {
				InetAddress addr = InetAddress.getByName(url_v[i][j]);
				host[i][j] = addr.getHostAddress();
			}

			long split_len = blksize;
			if (i == splits - 1){
				split_len = len - (blksize * i);
			}

			blkloc[i] = new BlockLocation(url_v[i], host[i], i * blksize, split_len);
		}

		return blkloc;
	}

	@Override
	public FileStatus getFileStatus (Path path) throws FileNotFoundException, IOException {
		path = removeURI(path);

		if (!exists(path)) {
			throw new FileNotFoundException("File does not exist: " + path.toString());
		};

		Stat stats = this.xpn.jni_xpn_stat(path.toString());
		boolean isdir = this.xpn.jni_xpn_isDir(stats.st_mode) != 0;
		return new FileStatus(stats.st_size, isdir, 0, stats.st_blksize,
					stats.st_mtime * 1000, path);
	}

	public long getLength (Path path) throws IOException {
		path = removeURI(path);

		if (!exists(path)) {
			throw new IOException("File does not exist: " + path.toString());
		};

		return this.xpn.jni_xpn_stat(path.toString()).st_size;
	}

	@Override
	public URI getUri() {
		return this.uri;
	}

	@Override
	public Path getWorkingDirectory() {
		return this.workingDirectory;
	}

	public void initialize(URI uri, Configuration conf) throws IOException {
		try{
			super.initialize(getUri(), conf);
			this.xpn.jni_xpn_init();
			this.blksize = conf.getLong("xpn.block.size", 134217728);
			this.bufsize = conf.getInt("xpn.file.buffer.size", 134217728);
			this.xpn_replication = (short) conf.getInt("xpn.block.replication", 1);
			this.initialized = true;
		}catch(Exception e){
			System.out.println("Excepcion en INITIALIZE: " + e);
			return;
		}
	}

	@Override
	public boolean isDirectory (Path path) {
		path = removeURI(path);
		try {
			Stat stats = this.xpn.jni_xpn_stat(path.toString());
			return this.xpn.jni_xpn_isDir(stats.st_mode) != 0;
		} catch (Exception e) {
			return false;
		}
	}

	@Override
	public FileStatus[] listStatus(Path f, PathFilter filter)
									throws FileNotFoundException, IOException {
		ArrayList<FileStatus> results = new ArrayList<>();
		return results.toArray(new FileStatus[results.size()]);
	}

	@Override
	public FileStatus[] listStatus(Path f) throws FileNotFoundException, IOException {
		f = removeURI(f);
		if (!exists(f))
			throw new FileNotFoundException("File does not exist: " + f.toString());

		if (!isDirectory(f)){
			FileStatus [] list = new FileStatus[1];
			list[0] = getFileStatus(f);
			return list;
		}

		String str [] = this.xpn.jni_xpn_getDirContent(f.toString());
		FileStatus list [] = new FileStatus [str.length - 2];
		int pos = 0;
		for (int i = 0; i < str.length; i++){
			if (!(str[i].equals(".") || str[i].equals(".."))) {
				list[pos] = getFileStatus(new Path(f.toString() + "/" + str[i]));
				pos++;
			}
		}

		return list;
	}

	private Path makeAbsolute (Path path) {
		String fullPath = this.workingDirectory.toString() + path.toString();

		return new Path (fullPath);
	}

	@Override
	public boolean mkdirs(Path path, FsPermission permission) throws IOException {
		path = removeURI(path);
		String relPath = "/xpn";
		String absPath;
		String [] dirs = path.toString().split("/");

		for (int i = 1; i < dirs.length; i++){
			if (dirs[i].equals("xpn") && i == 1) continue;
			relPath += "/" + dirs[i];
			if (exists(new Path(relPath))) continue;
			int res = this.xpn.jni_xpn_mkdir(relPath , permission.toShort());
			if (res != 0) return false;
		}

		return true;
	}

	@Override
	public FSDataInputStream open(Path f){
		return open(f, this.bufsize);
	}

	@Override
	public FSDataInputStream open(Path f, int bufferSize){
		f = removeURI(f);
		return new FSDataInputStream(new ExpandFSInputStream(f.toString(), bufsize, statistics));
	}

	private Path removeURI (Path path){
		String str [] = path.toString().split(":");
		if (str.length == 1) return path;
		else return new Path (str[1]);
	}

	@Override
	public boolean rename(Path src, Path dst) throws IOException {
		src = removeURI(src);
		dst = removeURI(dst);

		if (!exists(src)) return false;
		if (exists(dst)) return false;

		int res = xpn.jni_xpn_rename(src.toString(), dst.toString());

		return res == 0;
	}

	@Override
	public void setWorkingDirectory(Path new_dir) {
		new_dir = removeURI(new_dir);
		this.workingDirectory = new_dir;
	}

	@Override
	public void setPermission(Path path, FsPermission perm) throws IOException {

	}
}
