package org.expand.datasource.image.write;

import org.apache.spark.sql.connector.write.*;

public class ExpandImageDataWriterFactory implements DataWriterFactory {

    private final String path;

    public ExpandImageDataWriterFactory(String path) {
        this.path = path;
    }

    @Override
    public DataWriter createWriter(int partitionId, long taskId) {
        return new ExpandImageDataWriter(path);
    }

}
