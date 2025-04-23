import boto3
import sys, time, os
import json

# Lee los parámetros de conexión a S3 desde un archivo de configuración JSON
def load_s3_config():
    config_file = os.getenv('S3_CONFIG_FILE', 'config.json')
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

def download_part(file_path, key, part_number_start, part_number_end, file_size, block_size):

    # Abre el archivo y coloca el puntero en el byte de inicio
    with open(file_path, 'wb') as f:
        # Lee el fragmento del archivo
            
        for part_number in range(part_number_start, part_number_end + 1):
            start_pos = (part_number - 1) * block_size
            f.seek(start_pos)
            end_pos = (part_number * block_size) - 1

            if part_number == part_number_end:
                response = s3_client.head_object(Bucket=bucket_name, Key=key)
                end_pos = min(end_pos, file_size - 1)

            range_header = f"bytes={start_pos}-{end_pos - 1}"
            response = s3_client.get_object(Bucket=bucket_name, Key=key, Range=range_header)
            data = response['Body'].read()
            f.write(data)            



if __name__ == "__main__":
    if len(sys.argv) != 7:
        print("Uso: python download_file.py <file_path> <key> <part_number_start> <part_number_end> <file_size> <block_size>")
        sys.exit(1)

    file_path = sys.argv[1]
    key = sys.argv[2]
    part_number_start = int(sys.argv[3])
    part_number_end = int(sys.argv[4])
    file_size = int(sys.argv[5])
    block_size = int(sys.argv[6])

    download_part(file_path, key, part_number_start, part_number_end, file_size, block_size)
