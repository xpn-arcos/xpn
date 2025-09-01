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
        List<InputPartition> partitions = new ArrayList<>();
        try {
            Path hPath = new Path(path);
            FileSystem fs = hPath.getFileSystem(conf);
            FileStatus[] statuses = fs.listStatus(hPath);
            long length = fs.getBlockSize(hPath);
            for (FileStatus status : statuses) {
                if (status.isDirectory()) {
                    continue;
                }

                String fileName = status.getPath().toString();
                if ((fileName.toLowerCase().endsWith(".png") || fileName.toLowerCase().endsWith(".jpg") || fileName.toLowerCase().endsWith(".jpeg"))) {
                    BlockLocation[] locations = fs.getFileBlockLocations(status, 0, status.getLen());
                    for (BlockLocation location : locations) {
                        partitions.add(new ExpandImageInputPartition(fileName, location.getOffset(), length, location.getHosts()));
                    }
                }
            }
            return partitions.toArray(new InputPartition[0]);
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    @Override
    public PartitionReaderFactory createReaderFactory() {
        return new ExpandImagePartitionReaderFactory();
    }
}
