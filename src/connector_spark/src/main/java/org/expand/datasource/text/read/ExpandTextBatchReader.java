package org.expand.datasource.text.read;

import org.apache.spark.sql.connector.read.*;
import org.apache.spark.sql.types.*;
import org.apache.spark.sql.util.CaseInsensitiveStringMap;

import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.fs.FileStatus;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.BlockLocation;

import java.util.ArrayList;
import java.util.List;
import java.io.IOException;
import java.lang.Math;

public class ExpandTextBatchReader implements Batch {

    private final String path;
    private final Configuration conf;

    public ExpandTextBatchReader(CaseInsensitiveStringMap options, Configuration conf) {
        this.path = options.get("path");
        this.conf = conf;
    }

    @Override
    public InputPartition[] planInputPartitions() {
        List<InputPartition> partitions = new ArrayList<>();
        try {
            Path hPath = new Path(path);
            FileSystem fs = hPath.getFileSystem(conf);
            long length = fs.getBlockSize(hPath);
            FileStatus [] fileStatus = fs.listStatus(hPath);
            for (FileStatus status : fileStatus) {
                if (status.isDirectory()) {
                    continue;
                }
                
                String fileName = status.getPath().toString();
                BlockLocation[] locations = fs.getFileBlockLocations(status, 0, status.getLen());

                for (BlockLocation location : locations) {
                    partitions.add(new ExpandTextInputPartition(fileName, location.getOffset(), length, location.getHosts()));
                }
            }
        } catch (IOException e) {
            System.err.println("IOException: " + e.getMessage());
            System.exit(0);
            return null;
        }

        return partitions.toArray(new InputPartition[0]);
    }

    @Override
    public PartitionReaderFactory createReaderFactory() {
        return new ExpandTextReaderFactory();
    }
}
