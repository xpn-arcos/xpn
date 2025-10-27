package org.expand.datasource.image.write;

import org.apache.spark.sql.connector.write.*;

public class ExpandImageWriteBuilder implements WriteBuilder {

    @Override
    public Write build() {
        return new Write() {
            @Override
            public BatchWrite toBatch() {
                return new BatchWrite() {
                    @Override
                    public DataWriterFactory createBatchWriterFactory(PhysicalWriteInfo info) {
                        return new ExpandImageDataWriterFactory();
                    }

                    @Override
                    public void commit(WriterCommitMessage[] messages) {
                        // no-op
                    }

                    @Override
                    public void abort(WriterCommitMessage[] messages) {
                        // no-op
                    }
                };
            }
        };
    }
}
