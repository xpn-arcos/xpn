import boto3
import os
import sys
import math
import paramiko
import json
import time
from collections import defaultdict
from concurrent.futures import ThreadPoolExecutor

# Obtiene el directorio actual del script
current_dir = os.path.dirname(os.path.abspath(__file__))

# Lee los parámetros de conexión a S3 desde un archivo de configuración JSON
def load_s3_config():
    config_file = os.getenv('S3_CONFIG_FILE', 'configS3.json')
    with open(config_file, 'r') as f:
        config = json.load(f)
    return config

# Carga la configuración de S3
s3_config = load_s3_config()

bucket_name = s3_config['bucket_name']
aws_access_key_id = s3_config['aws_access_key_id']
aws_secret_access_key = s3_config['aws_secret_access_key']
region_name = s3_config['region_name']

# Crea una instancia del cliente de S3
s3_client = boto3.client('s3', aws_access_key_id=aws_access_key_id, aws_secret_access_key=aws_secret_access_key, region_name=region_name)

def create_ssh_client(node_address, timeout=60):
    """Crea y devuelve una conexión SSH a un nodo remoto."""
    ssh_client = paramiko.SSHClient()
    ssh_client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
    #ssh_client.connect(node_address, password="*****", timeout=timeout)
    ssh_client.connect(node_address, timeout=timeout)
    return ssh_client

def execute_remote_command(ssh_client, command):
    """Ejecuta un comando en un nodo remoto y devuelve la salida."""
    stdin, stdout, stderr = ssh_client.exec_command(command)
    stdout_str = stdout.read().decode()
    stderr_str = stderr.read().decode()
    if stderr_str:
        print(f"Error: {stderr_str}")
    return stdout_str

def get_environment_variables():
    """Obtiene las variables de entorno necesarias."""
    env_vars = {
        'XPN_CONF': os.getenv('XPN_CONF', ''),
        'XPN_LOCALITY': os.getenv('XPN_LOCALITY', ''),
        'XPN_SESSION_FILE': os.getenv('XPN_SESSION_FILE', ''),
        'XPN_MQTT': os.getenv('XPN_MQTT', '0'),
        'XPN_CONNECTED': os.getenv('XPN_CONNECTED', ''),
        'LD_PRELOAD': os.getenv('LD_PRELOAD', ''),
        'S3_CONFIG_FILE': os.getenv('S3_CONFIG_FILE', './configS3.json')
    }
    return env_vars

def build_preamble(env_vars):
    """Construye el preámbulo para exportar las variables de entorno."""
    preamble = ' '.join([f'export {key}={value};' for key, value in env_vars.items()])
    return preamble

# Define una función para descargar una parte del archivo desde S3 en un nodo remoto
def download_part_on_remote_node(file_path, key, part_number_start, part_number_end, file_size, node_address, block_size):
    """Descarga una parte del archivo desde S3 en un nodo remoto."""
    try:
        ssh_client = create_ssh_client(node_address)
        env_vars = get_environment_variables()
        preamble = build_preamble(env_vars)
        command = f"{preamble} /usr/bin/python3 {current_dir}/S3_to_xpn.py {file_path} {key} {part_number_start} {part_number_end} {file_size} {block_size}"
        execute_remote_command(ssh_client, command)
        #execute_remote_command(ssh_client, command)
    finally:
        ssh_client.close()
      

# Define una función para cargar una parte del archivo a S3 en un nodo remoto
def upload_part_on_remote_node(file_path, key, part_number_start, part_number_end, upload_id, node_address, block_size):
    """Carga una parte del archivo a S3 en un nodo remoto."""
    try:
        ssh_client = create_ssh_client(node_address)
        env_vars = get_environment_variables()
        preamble = build_preamble(env_vars)
        command = f"{preamble} /usr/bin/python3 {current_dir}/xpn_to_S3.py {file_path} {key} {part_number_start} {part_number_end} {upload_id} {block_size}"
        stdout_str = execute_remote_command(ssh_client, command)
        return json.loads(stdout_str)
    finally:
        ssh_client.close()


# Define una función para completar la carga multipartes
def complete_multipart_upload(key, upload_id, parts):
    # Ordena las partes según el número de parte
    parts.sort(key=lambda x: x['PartNumber'])
    # Completa la carga multipartes
    s3_client.complete_multipart_upload(Bucket=bucket_name, Key=key, UploadId=upload_id, MultipartUpload={'Parts': parts})
    print("Carga multipartes completada con éxito.")

# Define una función para calcular la cantidad de nodos necesarios y las posiciones de inicio y fin de cada nodo
def calculate_nodes_and_positions(file_size, num_nodes, part_size):
    """Calcula la cantidad de nodos necesarios y las posiciones de inicio y fin de cada nodo."""
    total_parts = math.ceil(file_size / part_size)
    parts_per_node = total_parts // num_nodes
    extra_parts = total_parts % num_nodes

    node_positions = []
    part_number = 1

    for i in range(num_nodes):
        part_number_start = part_number
        part_number_end = part_number_start + parts_per_node - 1

        if i < extra_parts:
            part_number_end += 1

        node_positions.append((part_number_start, part_number_end))
        part_number = part_number_end + 1

    return node_positions

def leer_nodos(fichero):
    """Lee la lista de nodos desde un archivo."""
    nodos = []
    try:
        with open(fichero, 'r') as f:
            for linea in f:
                nodo = linea.strip()  # Elimina cualquier espacio en blanco o salto de línea
                if nodo:  # Solo agrega nodos no vacíos
                    nodos.append(nodo)
    except FileNotFoundError:
        print(f"File {fichero} does not exist.")
    except Exception as e:
        print(f"Error while reading file: {e}")
    return nodos

def merge_contadores(contadores_json):
    """Combina los contadores de caracteres de múltiples nodos."""
    contador_final = defaultdict(int)
    for contador_json in contadores_json:
        if contador_json:
            contador = json.loads(contador_json)
            for caracter, cuenta in contador.items():
                contador_final[caracter] += cuenta
    return contador_final


if __name__ == "__main__":

    if len(sys.argv) < 2:
        print("Indique la operación a realizar: python3 launcher_S3.py download|upload [args]")
        sys.exit(1)


    operation = sys.argv[1]

    if operation == "upload":

        # Comprobar el número de argumentos
        if len(sys.argv) != 6:
            print("Uso: python launcher_S3.py upload <file_path> <key> <block_size> <machine_file>")
            sys.exit(1)

        file_path = sys.argv[2]
        key = sys.argv[3]
        block_size = int (sys.argv[4]) * 1024 * 1024
        machine_file = sys.argv[5]

        destinations = leer_nodos(machine_file)

        #if not os.path.exists(file_path):
        #    print(f"El archivo {file_path} no existe en la ruta especificada.")
        #    sys.exit(1)
        
        #file_size = os.stat(file_path).st_size

        f = open(file_path, 'r')
        file_size = f.seek(0, os.SEEK_END)
        f.close()

        #file_size = os.path.getsize(file_path)
        if block_size < 5 * 1024 * 1024:
            block_size = 5 * 1024 * 1024

        total_parts = math.ceil(file_size / block_size)

        # Inicia la carga multipartes
        response = s3_client.create_multipart_upload(Bucket=bucket_name, Key=key)
        upload_id = response['UploadId']

        node_positions = calculate_nodes_and_positions(file_size, len(destinations), block_size)
        num_nodes = len(destinations)
        parts = []

        # Inicio del tiempo de subida
        start_time = time.time()

        with ThreadPoolExecutor(max_workers=num_nodes) as executor:
            futures = []
            for i, (node_address, (part_number_start, part_number_end)) in enumerate(zip(destinations, node_positions)):
                futures.append(executor.submit(upload_part_on_remote_node, file_path, key, part_number_start, part_number_end, upload_id, node_address, block_size))

            for future in futures:
                part_info = future.result()
                if part_info:
                    parts.extend(part_info)

        # Completa la carga multipartes en S3
        complete_multipart_upload(key, upload_id, parts)      

        end_time = time.time()
        elapsed_time = (end_time - start_time) * 1000  # Convertir a milisegundos
        print(f"Tiempo total de subida: {elapsed_time:.2f} milisegundos")

    elif operation == "download":

        # Comprobar el número de argumentos
        if len(sys.argv) != 6:
            print("Uso: python launcher_S3.py download <keyFile> <downloaded_file> <block_size> <machine_file>")
            sys.exit(1)

        key = sys.argv[2]
        fileName_dest = sys.argv[3]
        block_size = int(sys.argv[4]) * 1024 * 1024  # Tamaño de cada bloque en bytes
        machine_file = sys.argv[5]

            
        if not os.path.exists(machine_file):
            print("El archivo machinefile no existe en la ruta especificada.")
            sys.exit(1)

        destinations = leer_nodos(machine_file)
        
        # Obtener el tamaño del archivo en S3
        response = s3_client.head_object(Bucket=bucket_name, Key=key)
        file_size = response['ContentLength']

        if (block_size < 5 * 1024 * 1024):
            block_size = 5 * 1024 * 1024

        total_parts = math.ceil(file_size / block_size)

        node_positions = calculate_nodes_and_positions(file_size, len(destinations), block_size)
        num_nodes = len(destinations)

        fd = os.open(fileName_dest, os.O_CREAT | os.O_RDWR)
        os.close(fd)

        start_time = time.time()

        with ThreadPoolExecutor(max_workers=num_nodes) as executor:
            futures = []
            for i, (node_address, (part_number_start, part_number_end)) in enumerate(zip(destinations, node_positions)):
                futures.append(executor.submit(download_part_on_remote_node, fileName_dest, key, part_number_start, part_number_end, file_size, node_address, block_size))

            for future in futures:
                future.result()

        end_time = time.time()
        elapsed_time = (end_time - start_time) * 1000  # Convertir a milisegundos
        print(f"Tiempo total de descarga: {elapsed_time:.2f} milisegundos")

    else:
        print("Indique la operación a realizar: python3 scriptS3.py download|upload [args]")
        sys.exit(1)