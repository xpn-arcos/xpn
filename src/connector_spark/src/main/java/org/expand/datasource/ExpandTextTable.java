package org.expand.datasource;

import org.apache.spark.sql.connector.read.ScanBuilder;
import org.apache.spark.sql.connector.catalog.SupportsRead;
import org.apache.spark.sql.connector.catalog.Table;
import org.apache.spark.sql.connector.expressions.Transform;
import org.apache.spark.sql.types.*;
import org.apache.spark.sql.util.CaseInsensitiveStringMap;
import org.apache.spark.sql.connector.catalog.TableCapability;
import org.apache.spark.sql.SparkSession;

import org.apache.hadoop.conf.Configuration;

import scala.Option;

import java.util.Collections;
import java.util.Set;

public class ExpandTextTable implements Table, SupportsRead {

    private final StructType schema;
    private final CaseInsensitiveStringMap options;

    public ExpandTextTable(StructType schema, CaseInsensitiveStringMap options) {
        this.schema = schema;
        this.options = options;
    }

    @Override
    public String name() {
        return "ExpandTextTable";
    }

    @Override
    public StructType schema() {
        return schema;
    }

    @Override
    public ScanBuilder newScanBuilder(CaseInsensitiveStringMap options) {
        Configuration hadoopConf = SparkSession.getActiveSession().get().sparkContext().hadoopConfiguration();

        return new ExpandTextScanBuilder(schema, options, hadoopConf);
    }

    @Override
    public Transform[] partitioning() {
        return new Transform[0];
    }

    @Override
    public Set<TableCapability> capabilities() {
        return Collections.singleton(TableCapability.BATCH_READ);
    }
}
