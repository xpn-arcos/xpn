
#define _LARGEFILE64_SOURCE

#include <stdio.h>
#include <string.h>
#include "xpn/xpn_simple/xpn_policy_rw.h"

int main ( int argc, char *argv[] )
{
    if (argc < 5)
	{
	    printf("\n") ;
	    printf(" Usage: %s <num blocks> <num serv ex: 3 3;4  4;5;-6> <num offsets ex: 3 3;4  4;5;-6> <replication level> \n", argv[0]) ;
	    printf("\n") ;
	    return -1 ;
	}	

    struct xpn_metadata mdata = {0};
    int blocks = atoi(argv[1]);
    const char *delim = ";";
    if (strchr(argv[2], delim[0])==NULL){
        mdata.data_nserv[0] = atoi(argv[2]);
    }else{
        char *token = strtok(argv[2], delim);
        int index = 0;
        while (token != NULL) {
            if (index >= XPN_METADATA_MAX_RECONSTURCTIONS){
                printf("Too much num serv %s\n", argv[2]);
                exit(EXIT_FAILURE);
            }
            mdata.data_nserv[index++] = atoi(token);
            token = strtok(NULL, delim);
        }
    }

    if (strchr(argv[3], delim[0])==NULL){
        mdata.offsets[0] = atoi(argv[3]);
    }else{
        char *token = strtok(argv[3], delim);
        int index = 0;
        while (token != NULL) {
            if (index >= XPN_METADATA_MAX_RECONSTURCTIONS){
                printf("Too much num serv %s\n", argv[3]);
                exit(EXIT_FAILURE);
            }
            mdata.offsets[index++] = atoi(token);
            token = strtok(NULL, delim);
        }
    }
    int replication_level = atoi(argv[4]);

    printf("Print %d blocks in %s servers with %s offset and %d replication level\n",blocks, argv[2], argv[3], replication_level);
    mdata.block_size = 1024*512;
    mdata.replication_level = replication_level;
    mdata.first_node = 0;
    memcpy(mdata.magic_number, XPN_MAGIC_NUMBER, 3);

    XpnPrintMetadata(&mdata);

    XpnPrintBlockDistribution(blocks, &mdata);

    return 0;
}
