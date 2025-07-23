package org.expand.datasource.text;

import org.apache.spark.sql.connector.read.ScanBuilder;
import org.apache.spark.sql.connector.catalog.SupportsRead;
import org.apache.spark.sql.connector.catalog.SupportsWrite;
import org.apache.spark.sql.connector.write.SupportsOverwrite;
import org.apache.spark.sql.connector.write.SupportsTruncate;
import org.apache.spark.sql.connector.catalog.Table;
import org.apache.spark.sql.connector.expressions.Transform;
import org.apache.spark.sql.connector.write.LogicalWriteInfo;
import org.apache.spark.sql.connector.write.WriteBuilder;
import org.apache.spark.sql.types.*;
import org.apache.spark.sql.util.CaseInsensitiveStringMap;
import org.apache.spark.sql.connector.catalog.TableCapability;
import org.apache.spark.sql.SparkSession;
import org.apache.spark.sql.sources.Filter;

import org.apache.hadoop.conf.Configuration;

import org.expand.datasource.text.read.ExpandTextReadBuilder;
import org.expand.datasource.text.write.ExpandTextWriteBuilder;

import scala.Option;

import java.util.Collections;
import java.util.Set;
import java.util.Arrays;
import java.util.HashSet;

public class ExpandTextTable implements Table, SupportsRead, SupportsWrite, SupportsTruncate, SupportsOverwrite {

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
    public Transform[] partitioning() {
        return new Transform[0];
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

        return new ExpandTextReadBuilder(schema, options, hadoopConf);
    }

    @Override
    public WriteBuilder newWriteBuilder(LogicalWriteInfo info) {
        return new ExpandTextWriteBuilder(options, false);
    }

    @Override
    public WriteBuilder truncate() {
        System.out.println("Truncando tabla antes de escribir (overwrite)");
        return new ExpandTextWriteBuilder(options, true);
    }

    @Override
    public WriteBuilder overwrite(Filter[] filters) {
        System.out.println("Sobrescribiendo tabla completa con filtros (OverwriteByExpression)");
        return new ExpandTextWriteBuilder(options, true);
    }
}
