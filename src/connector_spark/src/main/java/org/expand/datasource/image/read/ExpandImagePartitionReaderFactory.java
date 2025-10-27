package org.expand.datasource.image.read;

import org.apache.spark.sql.connector.read.*;

public class ExpandImagePartitionReaderFactory implements PartitionReaderFactory {
    @Override
    public PartitionReader<org.apache.spark.sql.catalyst.InternalRow> createReader(InputPartition partition) {
        return new ExpandImagePartitionReader(((ExpandImageInputPartition) partition).getPath(), ((ExpandImageInputPartition) partition).getLength());
    }
}
