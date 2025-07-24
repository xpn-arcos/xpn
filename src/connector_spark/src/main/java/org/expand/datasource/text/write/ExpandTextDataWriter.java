package org.expand.datasource.text.write;

import org.apache.spark.sql.connector.write.DataWriter;
import org.apache.spark.sql.connector.write.WriterCommitMessage;
import org.apache.spark.sql.util.CaseInsensitiveStringMap;
import org.apache.spark.sql.catalyst.InternalRow;
import org.apache.spark.unsafe.types.UTF8String;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.*;
import org.apache.hadoop.fs.FSDataOutputStream;

import java.io.IOException;
import java.nio.charset.StandardCharsets;

public class ExpandTextDataWriter implements DataWriter {
    private final FSDataOutputStream out;
    private final byte newLine = '\n';

    public ExpandTextDataWriter(String pathStr, int partitionId, long taskId, boolean truncate) {
        Configuration conf = new Configuration();
        conf.set("fs.defaultFS", "xpn:///");
        conf.set("fs.xpn.impl", "org.expand.hadoop.Expand");
        Path path = new Path(pathStr);
        try {
            FileSystem fs = path.getFileSystem(conf);
            Path outputPath = new Path(path + "/part-" + partitionId + "-" + taskId);
            this.out = fs.create(outputPath, truncate);
        } catch (IOException e) {
            throw new RuntimeException("File already exists: " + pathStr);
        }
    }

    @Override
    public void write(Object record) throws IOException {
        InternalRow row = (InternalRow) record;
        byte[] bytes = row.getString(0).getBytes(StandardCharsets.UTF_8);
        byte[] finalBytes = new byte[bytes.length + 1];
        System.arraycopy(bytes, 0, finalBytes, 0, bytes.length);
        finalBytes[finalBytes.length - 1] = newLine;
        out.write(finalBytes);
    }

    @Override
    public WriterCommitMessage commit() throws IOException {
        out.flush();
        out.close();
        return null;
    }

    @Override
    public void abort() throws IOException {
        out.flush();
        out.close();
    }

    @Override
    public void close() throws IOException {
        out.flush();
        out.close();
    }
}
