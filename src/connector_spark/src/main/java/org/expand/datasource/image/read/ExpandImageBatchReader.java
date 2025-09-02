package org.expand.datasource.image.read;

import org.apache.spark.sql.connector.read.*;
import org.apache.spark.sql.types.StructType;
import org.apache.spark.sql.util.CaseInsensitiveStringMap;

import org.apache.hadoop.fs.*;
import org.apache.hadoop.conf.Configuration;

import org.expand.datasource.image.ImageSchema;

import java.io.IOException;
import java.net.URI;
import java.util.*;

public class ExpandImageBatchReader implements Batch {
    private final String path;
    private final Configuration conf;

    public ExpandImageBatchReader(String path, Configuration conf) {
        this.path = path;
        this.conf = conf;
    }

    @Override
    public InputPartition[] planInputPartitions() {
        try {
            Path hPath = new Path(path);
            FileSystem fs = hPath.getFileSystem(conf);
            FileStatus[] statuses = fs.listStatus(hPath);
            long length = fs.getBlockSize(hPath);
            InputPartition partition [] = new InputPartition[statuses.length];
            int i = 0;
            for (FileStatus status : statuses) {
                if (status.isDirectory()) {
                    continue;
                }

                String fileName = status.getPath().toString();
                BlockLocation location = fs.getFileBlockLocations(status, 0, status.getLen())[0];
                partition[i] = new ExpandImageInputPartition(fileName, location.getOffset(), status.getLen(), location.getHosts());
                i++;
            }
            return partition;
        } catch (IOException e) {
            System.err.println("IOException: " + e.getMessage());
            System.exit(0);
            return null;
        }
    }

    @Override
    public PartitionReaderFactory createReaderFactory() {
        return new ExpandImagePartitionReaderFactory();
    }
}
