
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mpi.h>
#include "all_system.h"
#include "xpn.h"
#include <time.h>

// Estructura para pasar parámetros a los hilos
struct ThreadArgs 
{
    int mainPid;
    int threadId;
    int iterations;
    int payloadSize;
};

pthread_mutex_t m;


double get_time(void)
{
    struct timeval tp;
    struct timezone tzp;

    gettimeofday( &tp, &tzp);
    return ((double) tp.tv_sec + .000001 * (double) tp.tv_usec);
}


// Función que ejecutará cada hilo
void *threadFunction(void *args) 
{
    int ret, fd, payload_size, num_its, mainPid;
    double start_time, used_time, avg_time, us_rate;
    struct ThreadArgs *threadArgs = (struct ThreadArgs *)args;

    payload_size        = threadArgs->payloadSize;
    num_its             = threadArgs->iterations;
    mainPid             = threadArgs->mainPid;

    char filename[1024];
    //char payload[payload_size + 1]; // Buffer para el mensaje (ID + ";" + 128B de datos)
    char data[payload_size];

    //bzero(payload, sizeof(payload));
    memset(data, 'A', payload_size);
    data[payload_size-1] = '\n';

    //char temp[7];
    //bzero(temp, sizeof(temp));
    // Crear el nombre del archivo con el pid y el identificador del hilo
    
    sprintf(filename, "/P1/test-%d-%d", mainPid, threadArgs->threadId);
    //printf("%s\n",filename);
    
     // Iniciar el reloj
    avg_time = 0.0;
    start_time = get_time();

    // Abrir el archivo para escritura
    pthread_mutex_lock(&m);
    fd = xpn_open(filename, O_WRONLY);
    pthread_mutex_unlock(&m);
    if (fd < 0)
    {
        printf("[CLIENT] Error: creating file %s\n", filename);
        pthread_exit(NULL);
    }

    

    for (int i = 0; i < num_its; i++)
    {
        // Colocar el guión y el número en el array temporal
        data[0] = '-'; // Colocar el guión
        
        // Asignar cada dígito del número a su respectiva posición
        data[1] = '0' + (i / 1000) % 10; // Primer dígito
        data[2] = '0' + (i / 100) % 10;  // Segundo dígito
        data[3] = '0' + (i / 10) % 10;   // Tercer dígito
        data[4] = '0' + (i % 10);        // Cuarto dígito
        data[5] = ';';

        //sprintf (payload, temp);
        //strcat (payload, data);

        //snprintf(payload, sizeof(payload), "%s;%s", temp, data);
        pthread_mutex_lock(&m);
        ret = xpn_write(fd, data, payload_size);
        pthread_mutex_unlock(&m);
        if (ret < 0)
        {
            printf("[CLIENT] Error: writing file %s\n",filename);
        } 
        usleep(100000);
    }

    pthread_mutex_lock(&m);
    ret = xpn_close(fd);
    pthread_mutex_unlock(&m);
    if (ret < 0) 
    {
        //MPI_Finalize();
        printf("[CLIENT] Error: closing file %s\n",filename);
        pthread_exit(NULL);
    }
    
    pthread_exit(NULL);

    used_time           = (get_time() - start_time) - (0.10 * num_its);
    avg_time            = used_time;
    avg_time            = avg_time / (float) num_its;

    if (avg_time > 0) /* rate is megabytes per second */
        us_rate         = (double)((payload_size) / (avg_time * (double) 1000000));
    else
        us_rate         = 0.0;

    printf("%d;%.8f;%.8f\n", payload_size, avg_time, us_rate);

    

    /*char buf[payload_size + 1];
    memset(buf, 'a', payload_size);
    buf[payload_size] = '\n';

    // Iniciar el reloj
    avg_time = 0.0;
    start_time = get_time();


    char bufStart[payload_size + 1 + 4];
    sprintf(bufStart, "INI;");
    strcat (bufStart, buf);
    
    pthread_mutex_lock(&m);
    ret = xpn_write(fd, bufStart, payload_size + 1 + 4);
    pthread_mutex_unlock(&m);
    nanosleep((const struct timespec[]) {{0,100000000L}}, NULL);

    for (int i = 1; i < num_its - 1; i++) 
    {
        pthread_mutex_lock(&m);
        ret = xpn_write(fd, buf, payload_size + 1);
        pthread_mutex_unlock(&m);
        if (ret < 0)
        {
            printf("[CLIENT] Error: writing file %s\n",filename);
        }
        nanosleep((const struct timespec[]) {{0,100000000L}}, NULL);
    }

    char bufFin[payload_size + 1 + 4];
    sprintf(bufFin, "FIN;");
    strcat (bufFin, buf);

    pthread_mutex_lock(&m);
    ret = xpn_write(fd, bufFin, payload_size + 1 + 4);
    pthread_mutex_unlock(&m);
    nanosleep((const struct timespec[]) {{0,100000000L}}, NULL);
*/
/*
    // Escribir en el archivo
    for (int i = 0; i < num_its; i++) 
    {
        pthread_mutex_lock(&m);
        ret = xpn_write(fd, buf, payload_size + 1);
        pthread_mutex_unlock(&m);
        if (ret < 0)
        {
            printf("[CLIENT] Error: writing file %s\n",filename);
        }
        nanosleep((const struct timespec[]) {{0,10000000L}}, NULL);
    }
*/
    
}






int main(int argc, char * argv[]) 
{
    int rank, ppn, num_its, mainPid, numThreads, payload_size, ret;
    
    char * slurm_procid     = getenv("SLURM_PROCID");
    rank                    = atoi(slurm_procid);
    ppn                     = atoi(argv[1]); 
    //mainPid                 = (rank / ppn) + 1; 
    mainPid                 = rank + 1;
    num_its                 = 1000;
    //printf("rank: %d; ppn: %d; mainPid: %d\n", rank, ppn, mainPid);
    payload_size            = atoi(argv[2]);
    numThreads              = ppn;

    if (pthread_mutex_init(&m, NULL) != 0)
    {
        printf("Mutex init failed\n");
        return 1;
    }


    struct timespec dur, dur2;

    dur.tv_sec              = 0;
    dur.tv_nsec             = (mainPid % 100) * 1000;
    dur2.tv_sec             = 0;
    dur2.tv_nsec            = (mainPid % 50) * 1000;

    nanosleep(&dur2, NULL);

    // xpn-init
    ret = xpn_init();
    if (ret < 0) 
    {
        pthread_exit(NULL);
    }

    nanosleep(&dur, NULL);


    // Crear un array para almacenar los identificadores de los hilos
    pthread_t threads[numThreads];
    struct ThreadArgs * threadArgsArray        = (struct ThreadArgs *) malloc (numThreads * sizeof(struct ThreadArgs));

    // Inicializar las estructuras ThreadArgs para cada hilo
    for (int i = 0; i < numThreads; i++) 
    {
        threadArgsArray[i].mainPid              = mainPid;
        threadArgsArray[i].threadId             = i;
        threadArgsArray[i].iterations           = num_its; // Puedes ajustar el número de iteraciones según sea necesario
        threadArgsArray[i].payloadSize          = payload_size;
    }

    // Crear e iniciar los hilos
    for (int i = 0; i < numThreads; i++) 
    {
        if (pthread_create(&threads[i], NULL, threadFunction, (void *) &threadArgsArray[i]) != 0) 
        {
            perror("Error al crear el hilo");
            exit(EXIT_FAILURE);
        }
        nanosleep((const struct timespec[]) {{0,10000000L}}, NULL);
    }

    // Esperar a que todos los hilos terminen
    for (int i = 0; i < numThreads; i++) 
    {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&m);

    // Liberar la memoria del array de structs ThreadArgs
    free(threadArgsArray);

    // xpn-destroy

    ret = xpn_destroy();
    

    return 0;
}
