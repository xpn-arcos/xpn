#------------------------------------------------------------------------------------------------------------------------------------------------------
# This folder contains the following files:
#     Config.py: This file shall be modified with the values desired by users
#     nodes: This file shall contain the nodes where Spark workers and Expand server will be deployed. Only needed in interactive mode
#     interactive.py: This script will launch the application in interactive mode
#     sbatch.py: This script will generate a file to launch slurm jobs
# Usage: 
#     Fill this file and run: python3 launch.py
#------------------------------------------------------------------------------------------------------------------------------------------------------

#------------------------------------------------------------------------------------------------------------------------------------------------------
# SCRIPT TYPE
#     DEPLOY_MODE: Defines the deployment mode. This field can be "interactive" for interactive mode or "sbatch" to sbatch mode
#------------------------------------------------------------------------------------------------------------------------------------------------------

DEPLOY_MODE="interactive"

#------------------------------------------------------------------------------------------------------------------------------------------------------
# SLURM CONFIGURATION - ONLY FOR SBATCH MODE
#     NUM_NODES: Number of nodes desired for the job
#     TIME: Job maximum duration
#     ACCOUNT: Slurm job account
#     PARTITION: Slurm job partition
# In this mode, the nodes file will be automatically generated using the nodes asigned by slurm.
# The first node of the list will be the SPARK_MASTER and the rest will be used to allocate Expand servers and Spark workers
# This mode will generate a file to specify to sbatch command. This file will be generated in CONNECTOR_HOME/script_generator
# and will be named: xpn_<num_nodes>_<app_type>_<app_name>.job
#------------------------------------------------------------------------------------------------------------------------------------------------------

NUM_NODES=5
TIME="30:00"
ACCOUNT="pawsey0854"
PARTITION="work"

#------------------------------------------------------------------------------------------------------------------------------------------------------
# ENVIRONMENT CONFIGURATION
#     SPARK_HOME: Spark home directory
#     XPN_HOME: Expand home directory
#     XPN_BIN: Expand bin directory
#     MPICH_BIN: MPICH bin directory
#     CONNECTOR_HOME: Connector Expand-Spark home directory
#------------------------------------------------------------------------------------------------------------------------------------------------------

XPN_HOME="/home/lab/src/xpn"
XPN_BIN="/home/lab/bin/xpn"
SPARK_HOME="/home/lab/spark"
MPICH_BIN="/home/lab/bin/mpich"
CONNECTOR_HOME="/home/lab/connector_spark2"

#------------------------------------------------------------------------------------------------------------------------------------------------------
# EXPAND CONFIGURATION
#     XPN_CONF: Path to Expand configuration file
#     XPN_BLOCKSIZE: Expand block size used in the application
#     XPN_REPLICATION: Expand replication used in the application
#     XPN_MOUNT_PATH: Path where Expand will be mounted
#     XPN_WORK_DIR: Path used to preload directories in Expand
#     XPN_WORK_DIR: Path used to generate data by the application in Expand
#------------------------------------------------------------------------------------------------------------------------------------------------------

XPN_CONF="/home/lab/conf/config.txt"
XPN_BLOCKSIZE="512k"
XPN_REPLICATION=1
XPN_MOUNT_PATH="/temp"
XPN_WORKING_DIR="/xpn/input"
XPN_OUTPUT_DIR="/xpn/output"

#------------------------------------------------------------------------------------------------------------------------------------------------------
# BACKEND FILE SYSTEM CONFIGURATION
#     LOCAL_WORKING_DIR: Path that contains data preloaded to Expand
#     LOCAL_OUTPUT_DIR: Path that will contain data flushed from Expand
#------------------------------------------------------------------------------------------------------------------------------------------------------

LOCAL_WORKING_DIR="/home/lab/data/libsvm"
LOCAL_OUTPUT_DIR="/home/lab/data/output"

#------------------------------------------------------------------------------------------------------------------------------------------------------
# SPARK CONFIGURATION
#     SPARK_MASTER: Spark master node
#     SPARK_DRIVER_MEMORY: Memory located for Spark driver
#     SPARK_EXECUTOR_MEMORY: Memory located for Spark executor
#     SPARK_APP_TYPE: Application type launched in Spark. This field can be "python" or "java"
#     SPARK_APPLICATION: Application launched in Spark. The format of this field shall be as follows
#         If the SPARK_APP_TYPE="python": "<path to .py> <args of the application>"
#              Example: SPARK_APPLICATION="./spark/examples/src/main/python/wordcount.py xpn:///xpn/input/quixote"
#         If the SPARK_APP_TYPE="java": "<path to .jar> <.class application> <args of the application>"
#              Example: SPARK_APPLICATION="./spark/examples/jars/spark-examples_2.12-3.5.2.jar org.apache.spark.examples.JavaWordCount xpn:///xpn/input/quixote"
#------------------------------------------------------------------------------------------------------------------------------------------------------

SPARK_MASTER="nodo1"
SPARK_DRIVER_MEMORY="3G"
SPARK_EXECUTOR_MEMORY="2G"
SPARK_APP_TYPE="python"
SPARK_APPLICATION="/home/lab/data/ml/dt.py xpn:///xpn/input/sample_libsvm_data_unique_variable.txt xpn:///xpn/output"
