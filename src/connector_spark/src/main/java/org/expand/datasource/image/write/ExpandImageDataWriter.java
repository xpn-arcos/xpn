package org.expand.datasource.image.write;

import org.apache.spark.sql.connector.write.DataWriter;
import org.apache.spark.sql.connector.write.WriterCommitMessage;
import org.apache.spark.sql.catalyst.InternalRow;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.*;

import java.io.IOException;
import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;

public class ExpandImageDataWriter implements DataWriter {

    public ExpandImageDataWriter() {
    }

    @Override
    public void write(Object record) {

        Configuration conf = new Configuration();
        conf.set("fs.defaultFS", "xpn:///");
        conf.set("fs.xpn.impl", "org.expand.hadoop.Expand");
        InternalRow row = (InternalRow) record;
        String path = row.getString(0);
        byte[] data = row.getBinary(5);

        try {
            Path hPath = new Path(path);
            FileSystem fs = hPath.getFileSystem(conf);

            FSDataOutputStream out = fs.create(hPath, true);
            out.write(data, 0, data.length);
            out.flush();
            out.close();
        } catch (Exception e) {
            System.err.println("IOException: " + e.getMessage());
            System.exit(0);
        }
    }

    @Override
    public WriterCommitMessage commit() {
        return null;
    }

    @Override
    public void abort() {
        // no-op
    }

    @Override
    public void close() {
        // no-op
    }
}
