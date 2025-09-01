package org.expand.datasource.text.read;

import org.apache.spark.sql.connector.read.*;
import org.apache.spark.sql.types.*;
import org.apache.spark.sql.util.CaseInsensitiveStringMap;

import org.apache.hadoop.conf.Configuration;

public class ExpandTextReadBuilder implements ScanBuilder, Scan {

    private final StructType schema;
    private final CaseInsensitiveStringMap options;
    private final Configuration conf;

    public ExpandTextReadBuilder(StructType schema, CaseInsensitiveStringMap options, Configuration conf) {
        this.schema = schema;
        this.options = options;
        this.conf = conf;
    }

    @Override
    public Scan build() {
        return this;
    }

    @Override
    public StructType readSchema() {
        return schema;
    }

    @Override
    public Batch toBatch() {
        return new ExpandTextBatchReader(options, conf);
    }
}
