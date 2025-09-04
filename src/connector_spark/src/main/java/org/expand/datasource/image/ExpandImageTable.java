package org.expand.datasource.image;

import org.apache.spark.sql.connector.catalog.TableCapability;
import org.apache.spark.sql.connector.read.*;
import org.apache.spark.sql.connector.write.*;
import org.apache.spark.sql.connector.catalog.Table;
import org.apache.spark.sql.types.StructType;
import org.apache.spark.sql.connector.catalog.SupportsRead;
import org.apache.spark.sql.connector.catalog.SupportsWrite;
import org.apache.spark.sql.SparkSession;
import org.apache.spark.sql.util.CaseInsensitiveStringMap;

import org.apache.hadoop.conf.Configuration;

import org.expand.datasource.image.ImageSchema;
import org.expand.datasource.image.read.ExpandImageReadBuilder;
import org.expand.datasource.image.write.ExpandImageWriteBuilder;

import java.util.*;

public class ExpandImageTable implements Table, SupportsRead, SupportsWrite {

    private final Map<String, String> options;

    public ExpandImageTable(Map<String, String> options) {
        this.options = options;
    }

    @Override
    public String name() {
        return "ExpandImageTable";
    }

    @Override
    public StructType schema() {
        return ImageSchema.SCHEMA;
    }

    @Override
    public Set<TableCapability> capabilities() {
        return new HashSet<>(Arrays.asList(
            TableCapability.BATCH_READ,
            TableCapability.BATCH_WRITE,
            TableCapability.TRUNCATE,
            TableCapability.OVERWRITE_BY_FILTER
        ));
    }

    @Override
    public ScanBuilder newScanBuilder(CaseInsensitiveStringMap options) {
        Configuration hadoopConf = SparkSession.getActiveSession().get().sparkContext().hadoopConfiguration();
        return new ExpandImageReadBuilder(options, hadoopConf);
    }

    @Override
    public WriteBuilder newWriteBuilder(LogicalWriteInfo info) {
        return new ExpandImageWriteBuilder(options.get("path"));
    }
}
