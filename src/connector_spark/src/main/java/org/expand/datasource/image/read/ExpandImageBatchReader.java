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
            long length = fs.getBlockSize(hPath);
            int numFiles = 0;
            Map<String, List<FileStatus>> locationGroups = new HashMap<>();

            for (FileStatus status : statuses) {
                if (status.isDirectory()) {
                    continue;
                }

                String fileName = status.getPath().toString();
                BlockLocation location = fs.getFileBlockLocations(status, 0, status.getLen())[0];

                locationGroups.computeIfAbsent(location.getHosts()[0], k -> new ArrayList<>()).add(status);
                numFiles++;
            }

            SparkSession spark = SparkSession.active();
            SparkContext sc = spark.sparkContext();

            int parallelism = spark.sparkContext().defaultParallelism();

            int numGroups = Math.min(numFiles, parallelism);
            int cores = parallelism / locationGroups.size();

            InputPartition partition [] = new InputPartition[numGroups];
            List<InputPartition> partitions = new ArrayList<>();

            for (String key : locationGroups.keySet()) {
                List<List<String>> pathList = new ArrayList<>();
                List<List<Long>> lengthList = new ArrayList<>();
                int i = 0;
                for (FileStatus f : locationGroups.get(key)) {
                    if (pathList.size() <= i && pathList.size() < cores) {
                        pathList.add(new ArrayList<>(Arrays.asList(f.getPath().toString())));
                        lengthList.add(new ArrayList<>(Arrays.asList(f.getLen())));
                    }else{
                        pathList.get(i % cores).add(f.getPath().toString());
                        lengthList.get(i % cores).add(f.getLen());
                    }
                    i++;
                }

                for (int j = 0; j < pathList.size(); j++) {
                    partitions.add(new ExpandImageInputPartition(
                            pathList.get(j),
                            lengthList.get(j),
                            new String[]{key}
                    ));
                }
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
