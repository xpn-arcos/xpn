package org.expand.datasource;


import org.apache.spark.sql.connector.write.LogicalWriteInfo;
import org.apache.spark.sql.connector.write.PhysicalWriteInfo;
import org.apache.spark.sql.connector.write.BatchWrite;
import org.apache.spark.sql.connector.write.DataWriterFactory;
import org.apache.spark.sql.connector.write.WriterCommitMessage;
import org.apache.spark.sql.util.CaseInsensitiveStringMap;

public class ExpandTextBatchWrite implements BatchWrite {
    private final String path;
    private final boolean truncate;

    public ExpandTextBatchWrite(CaseInsensitiveStringMap options, boolean truncate) {
        this.path = options.get("path");;
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
