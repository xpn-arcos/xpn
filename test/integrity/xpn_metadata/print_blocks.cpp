#define _LARGEFILE64_SOURCE
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "xpn/xpn_file.hpp"
#include "xpn/xpn_metadata.hpp"
#include "xpn/xpn_partition.hpp"

using namespace XPN;

int main ( int argc, char *argv[] )
{
    if (argc < 5)
	{
	    printf("\n") ;
	    printf(" Usage: %s <num blocks> <num serv ex: 3 3;4  4;5;-6> <num offsets ex: 3 3;4  4;5;-6> <replication level> \n", argv[0]) ;
	    printf("\n") ;
	    return -1 ;
	}	
    xpn_partition part("xpn", 0, 512*KB);
    std::string aux_path = "test_file";
    xpn_file file(aux_path, part);
    int blocks = atoi(argv[1]);
    const char *delim = ";";
    printf("Print %d blocks in %s servers with %s offset and %s replication level\n", blocks, argv[2], argv[3], argv[4]);
    if (strchr(argv[2], delim[0])==NULL){
        file.m_mdata.m_data.data_nserv[0] = atoi(argv[2]);
    }else{
        char *token = strtok(argv[2], delim);
        int index = 0;
        while (token != NULL) {
            if (index >= xpn_metadata::MAX_RECONSTURCTIONS){
                printf("Too much num serv %s\n", argv[2]);
                exit(EXIT_FAILURE);
            }
            file.m_mdata.m_data.data_nserv[index++] = atoi(token);
            token = strtok(NULL, delim);
        }
    }

    if (strchr(argv[3], delim[0])==NULL){
        file.m_mdata.m_data.offsets[0] = atoi(argv[3]);
    }else{
        char *token = strtok(argv[3], delim);
        int index = 0;
        while (token != NULL) {
            if (index >= xpn_metadata::MAX_RECONSTURCTIONS){
                printf("Too much num serv %s\n", argv[3]);
                exit(EXIT_FAILURE);
            }
            file.m_mdata.m_data.offsets[index++] = atoi(token);
            token = strtok(NULL, delim);
        }
    }
    int replication_level = atoi(argv[4]);

    part.m_replication_level = replication_level;
    part.m_data_serv.resize(file.m_mdata.m_data.data_nserv[0]);
    memcpy(&file.m_mdata.m_data.magic_number, xpn_metadata::MAGIC_NUMBER, 3);

    file.m_mdata.m_data.fill(file.m_mdata);
    file.m_mdata.m_data.first_node = 0;

    std::cout << file.m_mdata.to_string() << std::endl;

    std::cout << file.m_mdata.to_string_blocks(blocks) << std::endl;

    return 0;
}