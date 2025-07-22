package org.expand.datasource;

import org.apache.spark.sql.connector.write.WriteBuilder;
import org.apache.spark.sql.connector.write.BatchWrite;
import org.apache.spark.sql.connector.write.LogicalWriteInfo;
import org.apache.spark.sql.util.CaseInsensitiveStringMap;

public class ExpandTextWriteBuilder implements WriteBuilder {
    private final CaseInsensitiveStringMap options;
    private final boolean truncate;

    public ExpandTextWriteBuilder(CaseInsensitiveStringMap options, boolean truncate) {
        this.options = options;
        this.truncate = truncate;
    }

    @Override
    public BatchWrite buildForBatch() {
        return new ExpandTextBatchWrite(options, truncate);
    }
}
