# Expand connector for Apache Spark



## Pre-requisites

- In order to use the Expand connector for Apache Spark, the following software shall be installed:
  - Java (tested version: Java 8)
  - Apache Maven (tested version: Maven 3.9.x)

## Compilation

Before compiling the proyect, some of the `./src/makerun.sh` file shall be modified:

```
export JAVA_INC=<PATH WHERE THE jni.h HEADER IS LOCATED>
```

To compile and obtain the necesary libraries for the connector, you just need to execute:

```
./src/makerun.sh
mvn package
```

## Connector usage

To execute Spark applications using Expand, a script generator is offered. With this script generator, you can obtain a executable or a SLURM file to execute using SBATCH. This script will deploy the Expand servers, preload data if needed, deploy Spark cluster, execute Spark application, flush Expand data and stop Spark and Expand features. You just need to modify the `./script_generator/Config.py` and the `./script_generator/nodes` files following the instructions. After that, you just need to execute:
```
python3 script_generator/launch.py
```

Otherwise, after deploying Expand and Spark features, you can execute Spark applications using Expand adding the following configuration options to the spark-submit executable:
1. To specify Spark the use of Expand, the following lines are mandatory:
```
--conf "spark.executorEnv.XPN_CONF=$XPN_CONF"
--conf "spark.executorEnv.PATH=$PATH"
--conf "spark.executorEnv.LD_LIBRARY_PATH=$LD_LIBRARY_PATH"
--conf "spark.hadoop.fs.defaultFS=xpn:///"
--conf "spark.hadoop.fs.xpn.impl=org.expand.hadoop.Expand"
```
- The environment variables shall contain at least:
  - XPN_CONF: the path to the Expand configuration file.
  - PATH: the path to mpiexec.
  - LD_LIBRARY_PATH: the path to the MPI, Expand and the connector libraries; and the path to the connector jar.

2. To customize the Expand features:
```
--conf "spark.hadoop.xpn.block.size=<XPN BLOCKSIZE>"
--conf "spark.hadoop.xpn.file.buffer.size=<XPN BLOCKSIZE>"
--conf "spark.hadoop.xpn.block.replication=<XPN REPLICATION>"
```
This features shall contain the same values specified to the Expand servers. By default, the blocksize is set to 128 MiB and the replication to 1.

3. To ensure the correct Spark tasks generation the following lines are highly recommended:
```
--conf "spark.hadoop.mapreduce.input.fileinputformat.split.minsize=<XPN BLOCKSIZE>"
--conf "spark.hadoop.mapreduce.input.fileinputformat.split.maxsize=<XPN BLOCKSIZE>"
```

4. An example for a Wordcount application is shown below (note as the blocksize is 128 MiB and the replication is 1):
```
$SPARK_HOME/bin/spark-submit --master spark://$MASTER_NODE:7077 \
--conf "spark.executorEnv.XPN_CONF=$XPN_CONF" \
--conf "spark.executorEnv.PATH=$PATH" \
--conf "spark.executorEnv.LD_LIBRARY_PATH=$LD_LIBRARY_PATH" \
--conf "spark.hadoop.mapreduce.input.fileinputformat.split.minsize=134217728" \
--conf "spark.hadoop.mapreduce.input.fileinputformat.split.maxsize=134217728" \
--conf "spark.hadoop.xpn.block.size=134217728"
--conf "spark.hadoop.xpn.file.buffer.size=134217728"
--conf "spark.hadoop.xpn.block.replication=1"
--conf "spark.hadoop.fs.defaultFS=xpn:///" \
--conf "spark.hadoop.fs.xpn.impl=org.expand.hadoop.Expand" \
--class org.expand.tests.JavaWordCount \
$CONNECTOR_HOME/target/test-xpn-1.0.jar \
xpn:///xpn/quixote xpn:///xpn/res/quixote-wc
```
