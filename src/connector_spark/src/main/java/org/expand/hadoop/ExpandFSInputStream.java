package org.expand.hadoop;

import java.io.Closeable;

import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.PositionedReadable;
import org.apache.hadoop.fs.Seekable;

public class ExpandFSInputStream extends ExpandInputStream
        implements Closeable, Seekable, PositionedReadable {
    private FileSystem.Statistics statistics;
    public ExpandFSInputStream(String path, int bufferSize,
            FileSystem.Statistics statistics) {
        super(path, bufferSize);
    }

    @Override
    public long getPos(){
        return super.tell();
    }

    @Override
    public synchronized int read() {
        return super.read();
    }

    @Override
    public synchronized int read(byte[] b) {
        return super.read(b);
    }

    @Override
    public synchronized int read(byte[] b, int off, int len) {
        int ret = super.read(b, off, len);
        return ret;
    }

    @Override
    public int read(long position, byte[] buffer, int offset, int length) {
        long oldPos = getPos();
        seek(position);
        int ret = read(buffer, offset, length);
        seek(oldPos);
        return ret;
    }

    @Override
    public void readFully(long position, byte[] buffer) {
        long oldPos = getPos();
        seek(position);
        int ret = read(buffer);
        seek(oldPos);
    }

    @Override
    public void readFully(long position, byte[] buffer, int offset, int length) {
        long oldPos = getPos();
        seek(position);
        int ret = read(buffer, offset, length);
        seek(oldPos);
    }

    @Override
    public synchronized void seek(long pos) {
        super.seek(pos);
    }

    @Override
    public synchronized boolean seekToNewSource(long targetPos) {
        return false;
    }
}