package org.expand.datasource.text.write;

import org.apache.spark.sql.connector.write.WriteBuilder;
import org.apache.spark.sql.connector.write.BatchWrite;
import org.apache.spark.sql.connector.write.LogicalWriteInfo;
import org.apache.spark.sql.util.CaseInsensitiveStringMap;

public class ExpandTextWriteBuilder implements WriteBuilder {
    private final String path;
    private final boolean truncate;

    public ExpandTextWriteBuilder(CaseInsensitiveStringMap options, boolean truncate) {
        this.path = options.get("path");
        this.truncate = truncate;
    }

    @Override
    public BatchWrite buildForBatch() {
        return new ExpandTextBatchWrite(path, truncate);
    }
}
