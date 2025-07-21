package org.expand.datasource;

import org.apache.spark.sql.connector.catalog.Table;
import org.apache.spark.sql.connector.catalog.TableProvider;
import org.apache.spark.sql.connector.expressions.Transform;
import org.apache.spark.sql.types.*;
import org.apache.spark.sql.util.CaseInsensitiveStringMap;
import org.apache.spark.sql.sources.DataSourceRegister;

import java.util.Map;

public class ExpandTextDataSource implements TableProvider, DataSourceRegister {

    @Override
    public String shortName() {
        return "xpn";
    }

    @Override
    public StructType inferSchema(CaseInsensitiveStringMap options) {
        return new StructType().add("value", DataTypes.StringType, false);
    }

    @Override
    public Table getTable(StructType schema, Transform[] transforms, Map<String, String> properties) {
        CaseInsensitiveStringMap options = new CaseInsensitiveStringMap(properties);
        return new ExpandTextTable(schema, options);
    }

    @Override
    public boolean supportsExternalMetadata() {
        return true;
    }
}
