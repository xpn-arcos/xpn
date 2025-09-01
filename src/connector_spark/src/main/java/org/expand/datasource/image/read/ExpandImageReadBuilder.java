package org.expand.datasource.image.read;

import org.apache.spark.sql.connector.read.*;
import org.apache.spark.sql.util.CaseInsensitiveStringMap;
import org.apache.spark.sql.types.StructType;

import org.expand.datasource.image.ImageSchema;

import org.apache.hadoop.conf.Configuration;

public class ExpandImageReadBuilder implements ScanBuilder, Scan {

    private final CaseInsensitiveStringMap options;
    private final Configuration conf;

    public ExpandImageReadBuilder(CaseInsensitiveStringMap options, Configuration conf) {
        this.options = options;
        this.conf = conf;
    }

    @Override
    public Scan build() {
        return this;
    }

    @Override
    public StructType readSchema() {
        return ImageSchema.SCHEMA;
    }

    @Override
    public Batch toBatch() {
        return new ExpandImageBatchReader(options.get("path"), conf);
    }
}
