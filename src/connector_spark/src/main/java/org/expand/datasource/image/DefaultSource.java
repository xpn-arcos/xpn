package org.expand.datasource.image;

import org.apache.spark.sql.connector.catalog.Table;
import org.apache.spark.sql.connector.catalog.TableProvider;
import org.apache.spark.sql.connector.expressions.Transform;
import org.apache.spark.sql.types.StructType;
import org.apache.spark.sql.types.*;
import org.apache.spark.sql.util.CaseInsensitiveStringMap;
import org.apache.spark.sql.sources.DataSourceRegister;

import java.util.Map;

public class DefaultSource implements TableProvider, DataSourceRegister {

    @Override
    public String shortName() {
        return "xpn_image";
    }

    @Override
    public StructType inferSchema(CaseInsensitiveStringMap options) {
        return ImageSchema.SCHEMA;
    }

    @Override
    public Table getTable(StructType schema, Transform[] partitions, Map<String, String> options) {
        return new ExpandImageTable(options);
    }
}
