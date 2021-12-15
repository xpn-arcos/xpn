#ifndef _MYSERVER_PARAMS_H_
#define _MYSERVER_PARAMS_H_

struct tcpServer_param_st{
        int port;
        int IOsize;
        char name[255];
        char file[255];
        char dirbase[255];
};

#endif// _MYSERVER_PARAMS_H_
