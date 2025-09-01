package org.expand.datasource.image.read;

import org.apache.spark.sql.catalyst.InternalRow;
import org.apache.spark.sql.catalyst.expressions.GenericInternalRow;
import org.apache.spark.sql.connector.read.PartitionReader;
import org.apache.spark.unsafe.types.UTF8String;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.*;

import java.io.IOException;
import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import java.io.ByteArrayInputStream;
import java.io.InputStream;

public class ExpandImagePartitionReader implements PartitionReader<InternalRow> {
    private final String path;
    private boolean read = false;
    private InternalRow row;
    private final long length;
    private final long start;

    public ExpandImagePartitionReader(String path, long length, long start) {
        this.path = path;
        this.length = length;
        this.start = start;
    }

    @Override
    public boolean next() {
        if (read) return false;
        try {
            Path p = new Path(path);
            byte[] buffer = new byte[(int) this.length];

            Configuration conf = new Configuration();
            conf.set("fs.defaultFS", "xpn:///");
            conf.set("fs.xpn.impl", "org.expand.hadoop.Expand");

            FileSystem fs = p.getFileSystem(conf);
            FSDataInputStream is = fs.open(p);
            is.read(this.start, buffer, 0, buffer.length);
            is.close();

            ByteArrayInputStream bais = new ByteArrayInputStream(buffer);
            BufferedImage img = ImageIO.read(bais);
            // String format = path.endsWith(".png") ? "png" : "jpeg";

            row = new GenericInternalRow(new Object[]{
                    UTF8String.fromString(path),
                    img.getHeight(),
                    img.getWidth(),
                    UTF8String.fromString(img.getColorModel().toString()),
                    buffer
            });
            read = true;
            return true;
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    @Override
    public InternalRow get() {
        return row;
    }

    @Override
    public void close() {
        // nothing
    }
}
