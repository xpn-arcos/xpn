import os
import shutil
import sys
import Config
import time

global num_nodes

def preload_xpn():
    global num_nodes

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
    os.system(f"mpiexec -n {num_nodes} -f {Config.SPARK_HOME}/conf/workers -envall mkdir -p {Config.XPN_MOUNT_PATH}/{working_dir}")
    os.system(f"mpiexec -n {num_nodes} -f {Config.SPARK_HOME}/conf/workers -envall {Config.XPN_HOME}/src/utils/xpn_preload {Config.LOCAL_WORKING_DIR} {Config.XPN_MOUNT_PATH}/{working_dir} {blk_size}")

def start_xpn():
    global num_nodes
    xpn_conf_dir = os.path.abspath(os.path.join(Config.XPN_CONF, os.pardir))
    os.system(f"{Config.XPN_HOME}/scripts/execute/xpn.sh -w {xpn_conf_dir} -l {Config.SPARK_HOME}/conf/workers -x {Config.XPN_MOUNT_PATH} -n {num_nodes} -q {Config.XPN_BLOCKSIZE} start")
    time.sleep(5)
    os.system("scp /home/lab/conf/config.txt nodo2:/home/lab/conf/")
    os.system("scp /home/lab/conf/config.txt nodo3:/home/lab/conf/")
    
def start_spark():
    os.system(f"{Config.SPARK_HOME}/sbin/start-all.sh")

def launch_python_app():
    blk_size = 1
    if (Config.XPN_BLOCKSIZE[-1].lower() == "k"):
        blk_size = int(Config.XPN_BLOCKSIZE[:-1]) * 1024
    elif (Config.XPN_BLOCKSIZE[-1].lower() == "m"):
        blk_size = int(Config.XPN_BLOCKSIZE[:-1]) * 1024 * 1024
    elif (Config.XPN_BLOCKSIZE[-1].lower() == "g"):
        blk_size = int(Config.XPN_BLOCKSIZE[:-1]) * 1024 * 1024 * 1024

    print(f"{Config.SPARK_HOME}/bin/spark-submit --master spark://{Config.SPARK_MASTER}:7077 \
        --driver-memory {Config.SPARK_DRIVER_MEMORY} --executor-memory {Config.SPARK_EXECUTOR_MEMORY} \
        --conf \"spark.executorEnv.XPN_CONF={Config.XPN_CONF}\" \
        --conf \"spark.executorEnv.PATH=$PATH\" \
        --conf \"spark.executorEnv.LD_LIBRARY_PATH=$LD_LIBRARY_PATH\" \
        --conf \"spark.hadoop.fs.defaultFS=xpn:///\" \
        --conf \"spark.hadoop.fs.xpn.impl=org.expand.hadoop.Expand\" \
        --conf \"spark.hadoop.mapreduce.input.fileinputformat.split.minsize={blk_size}\" \
        --conf \"spark.hadoop.mapreduce.input.fileinputformat.split.maxsize={blk_size}\" \
        --conf \"spark.hadoop.xpn.file.buffer.size={blk_size}\" \
        --conf \"spark.hadoop.xpn.block.replication={Config.XPN_REPLICATION}\" \
        --conf \"spark.hadoop.xpn.block.size={blk_size}\" \
        --files {Config.CONNECTOR_HOME}/src/libexpandtoposix.so \
        --jars {Config.CONNECTOR_HOME}/target/test-xpn-1.0-jar-with-dependencies.jar \
        {Config.SPARK_APPLICATION}")
    
    os.system(f"{Config.SPARK_HOME}/bin/spark-submit --master spark://{Config.SPARK_MASTER}:7077 \
        --driver-memory {Config.SPARK_DRIVER_MEMORY} --executor-memory {Config.SPARK_EXECUTOR_MEMORY} \
        --conf \"spark.executorEnv.XPN_CONF={Config.XPN_CONF}\" \
        --conf \"spark.executorEnv.PATH=$PATH\" \
        --conf \"spark.executorEnv.LD_LIBRARY_PATH=$LD_LIBRARY_PATH\" \
        --conf \"spark.hadoop.fs.defaultFS=xpn:///\" \
        --conf \"spark.hadoop.fs.xpn.impl=org.expand.hadoop.Expand\" \
        --conf \"spark.hadoop.mapreduce.input.fileinputformat.split.minsize={blk_size}\" \
        --conf \"spark.hadoop.mapreduce.input.fileinputformat.split.maxsize={blk_size}\" \
        --conf \"spark.hadoop.xpn.file.buffer.size={blk_size}\" \
        --conf \"spark.hadoop.xpn.block.replication={Config.XPN_REPLICATION}\" \
        --files {Config.CONNECTOR_HOME}/src/libexpandtoposix.so \
        --jars {Config.CONNECTOR_HOME}/target/test-xpn-1.0-jar-with-dependencies.jar \
        {Config.SPARK_APPLICATION}")

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
    os.system(f"{Config.SPARK_HOME}/bin/spark-submit --master spark://{Config.SPARK_MASTER}:7077 \
        --driver-memory {Config.SPARK_DRIVER_MEMORY} --executor-memory {Config.SPARK_EXECUTOR_MEMORY} \
        --conf \"spark.executorEnv.XPN_CONF={Config.XPN_CONF}\" \
        --conf \"spark.executorEnv.PATH=$PATH\" \
        --conf \"spark.executorEnv.LD_LIBRARY_PATH=$LD_LIBRARY_PATH\" \
        --conf \"spark.hadoop.fs.defaultFS=xpn:///\" \
        --conf \"spark.hadoop.fs.xpn.impl=org.expand.hadoop.Expand\" \
        --conf \"spark.hadoop.mapreduce.input.fileinputformat.split.minsize={blk_size}\" \
        --conf \"spark.hadoop.mapreduce.input.fileinputformat.split.maxsize={blk_size}\" \
        --conf \"spark.hadoop.xpn.file.buffer.size={blk_size}\" \
        --conf \"spark.hadoop.xpn.block.replication={Config.XPN_REPLICATION}\" \
        --conf \"spark.hadoop.xpn.block.size={blk_size}\" \
        --class {java_class} {jar} {params}")

def flush_xpn():
    global num_nodes

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

    output_dir = Config.XPN_OUTPUT_DIR.removeprefix("/xpn/")
    os.system(f"mpiexec -n {num_nodes} -f {Config.SPARK_HOME}/conf/workers -envall {Config.XPN_HOME}/src/utils/xpn_flush {Config.XPN_MOUNT_PATH}/{output_dir} {Config.LOCAL_OUTPUT_DIR} {blk_size}")

def stop_xpn():
    os.system("pkill mpiexec")

def stop_spark():
    os.system(f"{Config.SPARK_HOME}/sbin/stop-all.sh")

def clean():
    working_dir = Config.XPN_WORKING_DIR.removeprefix("/xpn/")
    output_dir = Config.XPN_OUTPUT_DIR.removeprefix("/xpn/")
    os.system(f"mpiexec -n {num_nodes} -f {Config.SPARK_HOME}/conf/workers -envall rm -r {Config.XPN_MOUNT_PATH}/{working_dir}")
    os.system(f"mpiexec -n {num_nodes} -f {Config.SPARK_HOME}/conf/workers -envall rm -r {Config.XPN_MOUNT_PATH}/{output_dir}")

def interactive(conf_dir):

    curr_env_path = os.getenv("PATH")
    curr_env_ldlp = os.getenv("PATH")
    os.environ["PATH"] = f"{Config.MPICH_BIN}/bin:{Config.XPN_BIN}/bin:{curr_env_path}"
    os.environ["LD_LIBRARY_PATH"] = f"{Config.MPICH_BIN}/lib:{Config.XPN_BIN}/lib:{Config.CONNECTOR_HOME}/src:{curr_env_ldlp}"
    os.environ["XPN_CONF"] = Config.XPN_CONF

    global num_nodes
    num_nodes = len([l for l in open(f'{conf_dir}/nodes', 'r').readlines() if l.strip(' \n') != ''])

    os.system(f"cat {conf_dir}/nodes > {Config.SPARK_HOME}/conf/workers")

    print("---------------------------------")
    print("    SPARK MASTER NODE: " + Config.SPARK_MASTER)
    print("---------------------------------")

    preload_xpn()
    start_xpn()
    start_spark()
    if Config.SPARK_APP_TYPE.lower() == "python":
        launch_python_app()
    elif Config.SPARK_APP_TYPE.lower() == "java":
        launch_java_app()
    else:
        print("Application type not supported", file=sys.stderr)
        clean()
        sys.exit(-1)
    stop_xpn()
    stop_spark()
    flush_xpn()
    clean()
