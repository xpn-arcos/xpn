package org.expand.datasource.image.write;

import org.apache.spark.sql.connector.write.DataWriter;
import org.apache.spark.sql.connector.write.WriterCommitMessage;
import org.apache.spark.sql.catalyst.InternalRow;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.*;

import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;

public class ExpandImageDataWriter implements DataWriter {
    private final String path;
    private final FileSystem fs;

    public ExpandImageDataWriter(String path) {
        this.path = path;
        Configuration conf = new Configuration();
        conf.set("fs.defaultFS", "xpn:///");
        conf.set("fs.xpn.impl", "org.expand.hadoop.Expand");
        try {
            Path hPath = new Path(path);
            this.fs = hPath.getFileSystem(conf);
        } catch (Exception e) {
            throw new RuntimeException("Error writing image: " + path, e);
        }
    }

    @Override
    public void write(Object record) {
        InternalRow row = (InternalRow) record;
        String outputPath = path + row.getString(0);
        byte[] data = row.getBinary(4);

        try {
            ByteArrayInputStream bais = new ByteArrayInputStream(data);
            BufferedImage img = ImageIO.read(bais);

            ByteArrayOutputStream baos = new ByteArrayOutputStream();
            String format = path.toLowerCase().endsWith(".png") ? "png" : "jpeg";
            ImageIO.write(img, format, baos);
            byte[] imageBytes = baos.toByteArray();
            Path writePath = new Path(outputPath);
            try {
                FSDataOutputStream out = fs.create(writePath, true);
                out.write(imageBytes, 0, imageBytes.length);
                out.flush();
            } catch (Exception e) {
                throw new RuntimeException("Error writing image: " + path, e);
            }
        } catch (Exception e) {
            throw new RuntimeException("Error writing image: " + path, e);
        }
    }

    @Override
    public WriterCommitMessage commit() {
        return null;
    }

    @Override
    public void abort() {}

    @Override
    public void close() {}
}
