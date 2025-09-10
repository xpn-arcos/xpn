package org.expand.datasource.image.read;

import org.apache.spark.sql.connector.read.*;
import org.apache.spark.sql.types.StructType;
import org.apache.spark.sql.util.CaseInsensitiveStringMap;
import org.apache.spark.sql.SparkSession;
import org.apache.spark.SparkContext;

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

            List<FileStatus> files = new ArrayList<>();

            for (FileStatus status : statuses) {
                if (!status.isDirectory()) {
                    files.add(status);
                }
            }

            int numFiles = files.size();

            SparkSession spark = SparkSession.active();
            SparkContext sc = spark.sparkContext();

            int parallelism = sc.defaultParallelism();

            int numGroups = Math.min(numFiles, parallelism);

            Map<String, List<FileStatus>> locationGroups = new HashMap<>();

            for (FileStatus status : statuses) {
                BlockLocation[] locations = fs.getFileBlockLocations(status, 0, status.getLen());
                String key = String.join(",", locations[0].getHosts());

                locationGroups.computeIfAbsent(key, k -> new ArrayList<>()).add(status);
            }

            List<List<FileStatus>> groups = new ArrayList<>(locationGroups.values());

            for (int i = 0; i < numGroups; i++) {
                groups.add(new ArrayList<>());
            }

            for (int i = 0; i < numFiles; i++) {
                groups.get(i % numGroups).add(statuses[i]);
            }

            if (groups.size() > numGroups) {
                List<List<FileStatus>> merged = new ArrayList<>();
                for (int i = 0; i < numGroups; i++) {
                    merged.add(new ArrayList<>());
                }
                int idx = 0;
                for (List<FileStatus> g : groups) {
                    merged.get(idx % numGroups).addAll(g);
                    idx++;
                }
                groups = merged;
            }

            List<InputPartition> partitions = new ArrayList<>();

            for (List<FileStatus> group : groups) {
                if (group.isEmpty()) continue;

                BlockLocation[] locs = fs.getFileBlockLocations(group.get(0), 0, group.get(0).getLen());
                String[] hosts = locs[0].getHosts();

                List<String> filePaths = new ArrayList<>();
                List<Long> fileLength = new ArrayList<>();
                for (FileStatus status : group) {
                    filePaths.add(status.getPath().toString());
                    fileLength.add(status.getLen());
                }

                partitions.add(new ExpandImageInputPartition(filePaths, fileLength, hosts));
            }
            return partitions.toArray(new InputPartition[0]);

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
