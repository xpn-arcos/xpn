package org.expand.datasource.image.read;

import org.apache.spark.sql.connector.read.InputPartition;

import java.io.Serializable;

public class ExpandImageInputPartition implements InputPartition, Serializable {

    private final String path;
    private final long start;
    private final long length;
    private final String[] preferredLocations;

    public ExpandImageInputPartition(String path, long start, long length, String[] preferredLocations) {
        this.path = path;
        this.start = start;
        this.length = length;
        this.preferredLocations = preferredLocations;
    }

    public String[] preferredLocations() {
        return preferredLocations;
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
}
