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
import java.awt.image.WritableRaster;
import java.io.ByteArrayInputStream;
import java.io.InputStream;
import java.util.*;

public class ExpandImagePartitionReader implements PartitionReader<InternalRow> {
    private InternalRow row;
    private final Iterator<String> iterator;
    private final Iterator<Long> iteratorLength;
    private FileSystem fs;

    public ExpandImagePartitionReader(List<String> path, List<Long> length) {
        this.iterator = path.iterator();
        this.iteratorLength = length.iterator();

        try {
            Configuration conf = new Configuration();
            conf.set("fs.defaultFS", "xpn:///");
            conf.set("fs.xpn.impl", "org.expand.hadoop.Expand");
            Path iniPath = new Path(path.get(0));
            this.fs = iniPath.getFileSystem(conf);
        } catch (IOException e) {
            System.err.println("IOException: " + e.getMessage());
            System.exit(0);
        }
    }

    @Override
    public boolean next() {
        if (!iterator.hasNext()) return false;
        try {
            String currentPath = iterator.next();
            long currentLength = iteratorLength.next();
            Path p = new Path(currentPath);
            byte[] buffer = new byte[(int) currentLength];

            FSDataInputStream is = fs.open(p);
            is.read(0, buffer, 0, buffer.length);
            is.close();

            ByteArrayInputStream bais = new ByteArrayInputStream(buffer);
            BufferedImage img = ImageIO.read(bais);

            WritableRaster raster = img.getRaster();
            int[] pixels = raster.getPixels(0, 0, img.getWidth(), img.getHeight(), (int[]) null);
            byte[] rawData = new byte[pixels.length];
            for (int i = 0; i < pixels.length; i++) {
                rawData[i] = (byte) pixels[i];
            }

            row = new GenericInternalRow(new Object[]{
                    UTF8String.fromString(currentPath),
                    img.getHeight(),
                    img.getWidth(),
                    img.getColorModel().getNumComponents(),
                    UTF8String.fromString(img.getColorModel().toString()),
                    rawData
            });
            return true;
        } catch (IOException e) {
            System.err.println("IOException: " + e.getMessage());
            System.exit(0);
            return false;
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
