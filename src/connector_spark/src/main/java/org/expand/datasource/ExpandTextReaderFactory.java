package org.expand.datasource;

import org.apache.spark.sql.catalyst.InternalRow;
import org.apache.spark.sql.connector.read.*;
import org.apache.spark.sql.types.StructType;
import org.apache.spark.sql.util.CaseInsensitiveStringMap;
import org.apache.spark.unsafe.types.UTF8String;
import org.apache.spark.sql.catalyst.expressions.GenericInternalRow;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.*;
import org.expand.hadoop.ExpandInputStream;

import java.io.*;
import java.nio.charset.StandardCharsets;
import java.util.*;

public class ExpandTextReaderFactory implements PartitionReaderFactory {

    private final StructType schema;
    private final String pathStr;

    public ExpandTextReaderFactory(StructType schema, String path) {
        this.schema = schema;
        this.pathStr = path;
    }

    @Override
    public PartitionReader<InternalRow> createReader(InputPartition partition) {
        return new PartitionReader<InternalRow>() {
            private final ExpandTextInputPartition p = (ExpandTextInputPartition) partition;

            private final Iterator<String> lineIterator = readLinesFromPath(p.getPath(), p.getStart(), p.getLength()).iterator();
            private String current;

            private List<String> readLinesFromPath(String pathStr, long start, long length) {
                List<String> lines = new ArrayList<>();
                FSDataInputStream inputStream = null;

                try {
                    Configuration conf = new Configuration();
                    conf.set("fs.defaultFS", "xpn:///");
                    conf.set("fs.xpn.impl", "org.expand.hadoop.Expand");

                    Path path = new Path(pathStr);
                    FileSystem fs = path.getFileSystem(conf);

                    inputStream = fs.open(path);
                    inputStream.seek(start);

                    byte[] buffer = new byte[(int) length];
                    int bytesRead = inputStream.read(start, buffer, 0, buffer.length);

                    if (bytesRead > 0) {
                        String textChunk = new String(buffer, 0, bytesRead, StandardCharsets.UTF_8);

                        String[] splitLines = textChunk.split("\n");
                        for (String line : splitLines) {
                            lines.add(line.trim());
                        }
                    }

                } catch (IOException e) {
                    throw new RuntimeException("Error leyendo archivo desde HCFS", e);
                } finally {
                    if (inputStream != null) {
                        try {
                            inputStream.close();
                        } catch (IOException ignored) {}
                    }
                }
                return lines;
            }

            @Override
            public boolean next() {
                if (lineIterator.hasNext()) {
                    current = lineIterator.next();
                    return true;
                }
                return false;
            }

            @Override
            public InternalRow get() {
                return new GenericInternalRow(new Object[] { UTF8String.fromString(current) });
            }

            @Override
            public void close() {

            }
        };
    }
}
