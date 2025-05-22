import os
import sys
import Config

def generate_header():
    return f"""#!/bin/bash
#SBATCH --job-name=xpn_job
#SBATCH --output=xpn_job_%j.log
#SBATCH --error=xpn_job_%j.err
#SBATCH --nodes={Config.NUM_NODES}
#SBATCH --time={Config.TIME}
#SBATCH --partition={Config.PARTITION}
#SBATCH --account={Config.ACCOUNT}
#SBATCH --exclusive
"""

def preload_xpn():
    if Config.LOCAL_WORKING_DIR == None:
        print("Local working directory needed", file=sys.stderr)
        sys.exit(-1)

    if Config.XPN_WORKING_DIR == None:
        print("Expand working directory needed", file=sys.stderr)
        sys.exit(-1)

    blk_size = 1
    if (Config.XPN_BLOCKSIZE[-1].lower() == "k"):
        blk_size = int(Config.XPN_BLOCKSIZE[:-1]) * 1024
    elif (Config.XPN_BLOCKSIZE[-1].lower() == "m"):
        blk_size = int(Config.XPN_BLOCKSIZE[:-1]) * 1024 * 1024
    elif (Config.XPN_BLOCKSIZE[-1].lower() == "g"):
        blk_size = int(Config.XPN_BLOCKSIZE[:-1]) * 1024 * 1024 * 1024

    working_dir = Config.XPN_WORKING_DIR.removeprefix("/xpn/")
    num_nodes = int(Config.NUM_NODES) - 1

    return f"""
mpiexec -n {num_nodes} -f {Config.SPARK_HOME}/conf/workers -envall mkdir -p {Config.XPN_MOUNT_PATH}/{working_dir}
mpiexec -n {num_nodes} -f {Config.SPARK_HOME}/conf/workers -envall {Config.XPN_HOME}/src/utils/xpn_preload {Config.LOCAL_WORKING_DIR} {Config.XPN_MOUNT_PATH}/{working_dir} {blk_size}
"""

def start_xpn():
    xpn_conf_dir = os.path.abspath(os.path.join(os.getenv("XPN_CONF"), os.pardir))
    num_nodes = int(Config.NUM_NODES) - 1
    
    return f"""
{Config.XPN_HOME}/scripts/execute/xpn.sh -w {xpn_conf_dir} -l {Config.SPARK_HOME}/conf/workers -x {Config.XPN_MOUNT_PATH} -n {num_nodes} -q {Config.XPN_BLOCKSIZE} start
"""

def start_spark():
    return f"""
{Config.SPARK_HOME}/sbin/start-all.sh
"""

def launch_python_app():
    blk_size = 1
    if (Config.XPN_BLOCKSIZE[-1].lower() == "k"):
        blk_size = int(Config.XPN_BLOCKSIZE[:-1]) * 1024
    elif (Config.XPN_BLOCKSIZE[-1].lower() == "m"):
        blk_size = int(Config.XPN_BLOCKSIZE[:-1]) * 1024 * 1024
    elif (Config.XPN_BLOCKSIZE[-1].lower() == "g"):
        blk_size = int(Config.XPN_BLOCKSIZE[:-1]) * 1024 * 1024 * 1024

    return f"""
mpiexec -n 1 -hosts $MASTER_NODE -envall {Config.SPARK_HOME}/bin/spark-submit --master spark://$MASTER_NODE:7077 \\
    --driver-memory {Config.SPARK_DRIVER_MEMORY} --executor-memory {Config.SPARK_EXECUTOR_MEMORY} \\
    --conf "spark.executorEnv.XPN_CONF=$XPN_CONF" \\
    --conf "spark.executorEnv.PATH=$PATH" \\
    --conf "spark.executorEnv.LD_LIBRARY_PATH=$LD_LIBRARY_PATH" \\
    --conf "spark.hadoop.fs.defaultFS=xpn:///" \\
    --conf "spark.hadoop.fs.xpn.impl=org.expand.hadoop.Expand" \\
    --conf "spark.hadoop.mapreduce.input.fileinputformat.split.minsize={blk_size}" \\
    --conf "spark.hadoop.mapreduce.input.fileinputformat.split.maxsize={blk_size}" \\
    --conf "spark.hadoop.xpn.file.buffer.size={blk_size}" \\
    --conf "spark.hadoop.xpn.block.replication={Config.XPN_REPLICATION}" \\
    --conf \"spark.hadoop.xpn.block.size={blk_size}\" \
    --files {Config.CONNECTOR_HOME}/src/libexpandtoposix.so \\
    --jars {Config.CONNECTOR_HOME}/target/test-xpn-1.0-jar-with-dependencies.jar \\
    {Config.SPARK_APPLICATION}
"""

def launch_java_app():
    blk_size = 1
    if (Config.XPN_BLOCKSIZE[-1].lower() == "k"):
        blk_size = int(Config.XPN_BLOCKSIZE[:-1]) * 1024
    elif (Config.XPN_BLOCKSIZE[-1].lower() == "m"):
        blk_size = int(Config.XPN_BLOCKSIZE[:-1]) * 1024 * 1024
    elif (Config.XPN_BLOCKSIZE[-1].lower() == "g"):
        blk_size = int(Config.XPN_BLOCKSIZE[:-1]) * 1024 * 1024 * 1024

    app = Config.SPARK_APPLICATION.split(" ")
    jar = app[0]
    java_class = app[1]
    params = " ".join(map(str, app[2:]))

    return f"""
mpiexec -n 1 -hosts $MASTER_NODE -envall {Config.SPARK_HOME}/bin/spark-submit --master spark://$MASTER_NODE:7077 \\
    --driver-memory {Config.SPARK_DRIVER_MEMORY} --executor-memory {Config.SPARK_EXECUTOR_MEMORY} \\
    --conf "spark.executorEnv.XPN_CONF=$XPN_CONF" \\
    --conf "spark.executorEnv.PATH=$PATH" \\
    --conf "spark.executorEnv.LD_LIBRARY_PATH=$LD_LIBRARY_PATH" \\
    --conf "spark.hadoop.fs.defaultFS=xpn:///" \\
    --conf "spark.hadoop.fs.xpn.impl=org.expand.hadoop.Expand" \\
    --conf "spark.hadoop.mapreduce.input.fileinputformat.split.minsize={blk_size}" \\
    --conf "spark.hadoop.mapreduce.input.fileinputformat.split.maxsize={blk_size}" \\
    --conf "spark.hadoop.xpn.file.buffer.size={blk_size}" \\
    --conf "spark.hadoop.xpn.block.replication={Config.XPN_REPLICATION}" \\
    --conf \"spark.hadoop.xpn.block.size={blk_size}\" \
    --class {java_class} {jar} {params}
"""

def stop_xpn():
    return f"""
pkill mpiexec
"""

def stop_spark():
    return f"""
{Config.SPARK_HOME}/sbin/stop-all.sh
"""

def flush_xpn():
    if Config.LOCAL_OUTPUT_DIR == None:
        print("Local working directory needed", file=sys.stderr)
        sys.exit(-1)

    if Config.XPN_OUTPUT_DIR == None:
        print("Expand working directory needed", file=sys.stderr)
        sys.exit(-1)

    blk_size = 1
    if (Config.XPN_BLOCKSIZE[-1].lower() == "k"):
        blk_size = int(Config.XPN_BLOCKSIZE[:-1]) * 1024
    elif (Config.XPN_BLOCKSIZE[-1].lower() == "m"):
        blk_size = int(Config.XPN_BLOCKSIZE[:-1]) * 1024 * 1024
    elif (Config.XPN_BLOCKSIZE[-1].lower() == "g"):
        blk_size = int(Config.XPN_BLOCKSIZE[:-1]) * 1024 * 1024 * 1024

    num_nodes = int(Config.NUM_NODES) - 1
    output_dir = Config.XPN_OUTPUT_DIR.removeprefix("/xpn/")
    return f"""
mpiexec -n {num_nodes} -f {Config.SPARK_HOME}/conf/workers -envall {Config.XPN_HOME}/src/utils/xpn_flush {Config.XPN_MOUNT_PATH}/{output_dir} {Config.LOCAL_OUTPUT_DIR} {blk_size}
"""

def clean():
    num_nodes = int(Config.NUM_NODES) - 1
    working_dir = Config.XPN_WORKING_DIR.removeprefix("/xpn/")
    output_dir = Config.XPN_OUTPUT_DIR.removeprefix("/xpn/")
    return f"""
mpiexec -n {num_nodes} -f {Config.SPARK_HOME}/conf/workers -envall rm -r {Config.XPN_MOUNT_PATH}/{working_dir}
mpiexec -n {num_nodes} -f {Config.SPARK_HOME}/conf/workers -envall rm -r {Config.XPN_MOUNT_PATH}/{output_dir}
"""

def sbatch(file):
    num_nodes = int(Config.NUM_NODES) - 1

    with open(file, 'w') as f:
        f.write(generate_header())
        f.write(f"""
export PATH={Config.MPICH_BIN}/bin:{Config.XPN_BIN}/bin:$PATH
export LD_LIBRARY_PATH={Config.MPICH_BIN}/lib:{Config.XPN_BIN}/lib:{Config.CONNECTOR_HOME}/src:$LD_LIBRARY_PATH
scontrol show hostnames $SLURM_JOB_NODELIST > {Config.SPARK_HOME}/hosts
tail -n {num_nodes} {Config.SPARK_HOME}/hosts > {Config.SPARK_HOME}/conf/workers

MASTER_NODE=$(head -n 1 {Config.SPARK_HOME}/hosts)

echo "---------------------------------"
echo "    SPARK MASTER NODE: $MASTER_NODE"
echo "---------------------------------"
""")
        f.write(preload_xpn())
        f.write(start_xpn())
        f.write(start_spark())    
        if Config.SPARK_APP_TYPE.lower() == "python":
            f.write(launch_python_app())
        elif Config.SPARK_APP_TYPE.lower() == "java":
            f.write(launch_java_app())
        else:
            print("Application type not supported", file=sys.stderr)
            sys.exit(-1)
        f.write(stop_xpn())
        f.write(stop_spark())
        f.write(flush_xpn())
        f.write(clean())
