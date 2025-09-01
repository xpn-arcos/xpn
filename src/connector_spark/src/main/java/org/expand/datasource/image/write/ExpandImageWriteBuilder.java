package org.expand.datasource.image.write;

import org.apache.spark.sql.connector.write.*;

public class ExpandImageWriteBuilder implements WriteBuilder {
    private final String path;

    public ExpandImageWriteBuilder(String path) {
        this.path = path;
    }

    @Override
    public Write build() {
        return new Write() {
            @Override
            public BatchWrite toBatch() {
                return new BatchWrite() {
                    @Override
                    public DataWriterFactory createBatchWriterFactory(PhysicalWriteInfo info) {
                        return new ExpandImageDataWriterFactory(path);
                    }

                    @Override
                    public void commit(WriterCommitMessage[] messages) {}

                    @Override
                    public void abort(WriterCommitMessage[] messages) {}
                };
            }
        };
    }
}
