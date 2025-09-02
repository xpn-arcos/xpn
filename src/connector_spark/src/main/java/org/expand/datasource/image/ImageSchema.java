package org.expand.datasource.image;

import org.apache.spark.sql.types.*;


public class ImageSchema {
    public static final StructType SCHEMA = new StructType()
        .add("origin", DataTypes.StringType, false)
        .add("height", DataTypes.IntegerType, false)
        .add("width", DataTypes.IntegerType, false)
        .add("nChannels", DataTypes.IntegerType, false)
        .add("mode", DataTypes.StringType, false)
        .add("data", DataTypes.BinaryType, false);
}
