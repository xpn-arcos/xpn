package org.expand.datasource.image.read;

import org.apache.spark.sql.connector.read.InputPartition;

import java.io.Serializable;
import java.util.*;

public class ExpandImageInputPartition implements InputPartition, Serializable {

    private final List<String> path;
    private final List<Long> length;
    private final String[] preferredLocations;

    public ExpandImageInputPartition(List<String> path, List<Long> length, String[] preferredLocations) {
        this.path = path;
        this.length = length;
        this.preferredLocations = preferredLocations;
    }

    public String[] preferredLocations() {
        return preferredLocations;
    }

    public List<String> getPath() {
        return path;
    }

    public List<Long> getLength() {
        return length;
    }
}
