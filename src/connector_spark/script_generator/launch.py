import sys
import Config
import interactive
import sbatch

if __name__ == "__main__":

    if Config.DEPLOY_MODE.lower() == "sbatch":
        if Config.SPARK_APP_TYPE.lower() == "python":
            app = Config.SPARK_APPLICATION.split(" ")[0].split("/")[-1].split(".")[0]
            sbatch.sbatch(Config.CONNECTOR_HOME + f"/script_generator/xpn_{Config.NUM_NODES}_python_{app}.job")
        elif Config.SPARK_APP_TYPE.lower() == "java":
            app = Config.SPARK_APPLICATION.split(" ")[1].split(".")[-1]
            sbatch.sbatch(Config.CONNECTOR_HOME + f"/script_generator/xpn_{Config.NUM_NODES}_java_{app}.job")
        else:
            print("Application type not supported", file=sys.stderr)
            sys.exit(-1)
    elif Config.DEPLOY_MODE.lower() == "interactive":
        interactive.interactive(Config.CONNECTOR_HOME + f"/script_generator")
    else:
        print("Deploy mode not supported", file=sys.stderr)
        sys.exit(-1)
