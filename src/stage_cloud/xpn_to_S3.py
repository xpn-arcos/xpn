import boto3
from boto3.s3.transfer import TransferConfig
import os
import sys
import json

# Lee los parámetros de conexión a S3 desde un archivo de configuración JSON
def load_s3_config():
    config_file = os.getenv('S3_CONFIG_FILE', './configS3.json')
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

# Define una función para cargar una parte del archivo a S3
def upload_part(file_path, key, part_number_start, part_number_end, upload_id, block_s):
    # Configura la configuración de transferencia multipart

    parts_info = []

    # Calcula el tamaño del fragmento
    part_size = block_s
    
    # Abre el archivo y coloca el puntero en el byte de inicio
    try:
        with open(file_path, 'rb') as f:
            for part_number in range(part_number_start, part_number_end + 1):
                start_pos = (part_number - 1) * part_size
                f.seek(start_pos)
                # Lee el fragmento del archivo
                chunk = f.read(part_size)

                # Carga el fragmento al bucket de S3
                response_upload = s3_client.upload_part(
                    Body=chunk,
                    Bucket=bucket_name,
                    Key=key,
                    PartNumber=part_number,
                    UploadId=upload_id,
                )

                parts_info.append({
                    'PartNumber': part_number,
                    'ETag': response_upload['ETag']
                })
    except Exception as e:
        print(f"Error al subir la parte del archivo: {e}")
        sys.exit(1)

    return parts_info

# Ejemplo de uso
if __name__ == "__main__":
    if len(sys.argv) != 7:
        print("Uso: python upload_file.py <file_path> <key> <part_number_start> <part_number_end> <upload_id> <block_size>")
        sys.exit(1)
        
    file_path = sys.argv[1]
    key = sys.argv[2]
    part_number_start = int(sys.argv[3])
    part_number_end = int(sys.argv[4])
    upload_id = sys.argv[5]
    block_s = int(sys.argv[6])

    #if os.path.exists(file_path):
    part_info = upload_part(file_path, key, part_number_start, part_number_end, upload_id, block_s)
    
    # Devuelve la información de la parte subida como JSON
    print(json.dumps(part_info))
