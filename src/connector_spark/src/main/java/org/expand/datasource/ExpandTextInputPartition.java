package org.expand.datasource;

import org.apache.spark.sql.connector.read.InputPartition;
import org.apache.spark.sql.util.CaseInsensitiveStringMap;

import org.apache.hadoop.conf.Configuration;

import java.io.Serializable;

public class ExpandTextInputPartition implements InputPartition, Serializable {

    private final String path;
    private final long start;
    private final long length;
    // private final Configuration conf;

    public ExpandTextInputPartition(String path, long start, long length) {
        this.path = path;
        this.start = start;
        this.length = length;
        // this.conf = new Configuration(conf);
    }

    public String getPath() {
        return path;
    }

    public long getStart() {
        return start;
    }

    public long getLength() {
        return length;
    }

    // public Configuration getConf() {
    //     return conf;
    // }
}
