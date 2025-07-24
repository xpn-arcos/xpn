package org.expand.datasource.text.write;


import org.apache.spark.sql.connector.write.LogicalWriteInfo;
import org.apache.spark.sql.connector.write.PhysicalWriteInfo;
import org.apache.spark.sql.connector.write.BatchWrite;
import org.apache.spark.sql.connector.write.DataWriterFactory;
import org.apache.spark.sql.connector.write.WriterCommitMessage;
import org.apache.spark.sql.util.CaseInsensitiveStringMap;

public class ExpandTextBatchWrite implements BatchWrite {
    private final String path;
    private final boolean truncate;

    public ExpandTextBatchWrite(String path, boolean truncate) {
        this.path = path;
        this.truncate = truncate;
    }

    @Override
    public DataWriterFactory createBatchWriterFactory(PhysicalWriteInfo info) {
        return new ExpandTextDataWriterFactory(path, truncate);
    }

    @Override
    public void commit(WriterCommitMessage[] messages) {}

    @Override
    public void abort(WriterCommitMessage[] messages) {}
}
