package org.expand.datasource;

import org.apache.spark.sql.connector.write.DataWriterFactory;
import org.apache.spark.sql.connector.write.DataWriter;
import org.apache.spark.sql.util.CaseInsensitiveStringMap;


import java.io.Serializable;

public class ExpandTextDataWriterFactory implements DataWriterFactory, Serializable {
    private final String path;
    private final boolean truncate;

    public ExpandTextDataWriterFactory(String path, boolean truncate) {
        this.path = path;
        this.truncate = truncate;
    }

    @Override
    public DataWriter createWriter(int partitionId, long taskId) {
        return new ExpandTextDataWriter(path, partitionId, taskId, truncate);
    }
}
