#include <iostream>
#include <string>
#include <cstring>
#include <hdf5.h>

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <archivo.h5>" << std::endl;
        return 1;
    }

    const char* filename = argv[1];
    const char* mensaje = "Hola mundo";

    // Crear archivo HDF5
    hid_t file_id = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    if (file_id < 0) {
        std::cerr << "Error al crear el archivo HDF5" << std::endl;
        return 1;
    }

    // Crear espacio de datos (1 elemento)
    hsize_t dims[1] = {1};
    hid_t dataspace_id = H5Screate_simple(1, dims, nullptr);

    // Crear tipo de dato (cadena de longitud fija)
    hid_t datatype_id = H5Tcopy(H5T_C_S1);
    H5Tset_size(datatype_id, strlen(mensaje) + 1);  // +1 para '\0'
    H5Tset_strpad(datatype_id, H5T_STR_NULLTERM);   // Terminación nula

    // Crear el dataset
    hid_t dataset_id = H5Dcreate(file_id, "mensaje", datatype_id, dataspace_id,
                                 H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    // Escribir en el dataset
    herr_t status = H5Dwrite(dataset_id, datatype_id, H5S_ALL, H5S_ALL,
                             H5P_DEFAULT, mensaje);

    if (status < 0) {
        std::cerr << "Error al escribir los datos" << std::endl;
    } else {
        std::cout << "Archivo HDF5 creado con mensaje: \"" << mensaje << "\"" << std::endl;
    }

    // Cerrar todos los recursos
    H5Dclose(dataset_id);
    H5Sclose(dataspace_id);
    H5Tclose(datatype_id);
    H5Fclose(file_id);

    return 0;
}

