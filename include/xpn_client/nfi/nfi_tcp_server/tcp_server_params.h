#ifndef _TCP_SERVER_PARAMS_H_
#define _TCP_SERVER_PARAMS_H_

  #include <stdlib.h>
  #include <stdio.h>
  #include <string.h>
  //#include <semaphore.h>


  /*
   * Datatype
   */

  #define TCP_MAX_PORT_NAME 1024

  typedef struct
  {
    // Execution configuration
    int xpn_thread ;
    int xpn_session ;
    int xpn_locality ;


    //MQTT usage
    int xpn_mosquitto_mode;
    int xpn_mosquitto_qos;

    // client identification
    int  size ;
    int  rank ;

    // server port and comm
    // int   port_number;
    char  srv_name   [TCP_MAX_PORT_NAME] ;
    char  server_name[TCP_MAX_PORT_NAME];
    char  port_number[TCP_MAX_PORT_NAME];
    int   server ;

    int     locality ;
    char dirbase[PATH_MAX] ;
    //char    sem_name_server [PATH_MAX];
    //sem_t * sem_server ;

    // server arguments
    int    argc ;
    char **argv ;

  } tcpClient_param_st ;


#endif

