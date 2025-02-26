
#include "all_system.h"
#include "xpn.h"
#include "xpn_conf_reader.h"


struct conf_file_data conf_data ;


int main ( int argc, char *argv[] )
{
    int   ret ;
    int   first, last ;
    char *fname ;
    char  conf[1024];

    // initial limits
    first = 1 ;
    last  = 5 ;
    if (argc > 2) {
        last = atoi(argv[1]) ;
    }

    // get file name
    fname = getenv("XPN_CONF") ;
    if (fname != NULL)
         strcpy(conf, fname);
    else strcpy(conf, XPN_CONF_DEFAULT);

    // test loop
    for (int i=first; i<last; i++)
    {
        sprintf(conf, "xpn-%d.conf", i) ;

	printf(">>> load xpn-%d.conf...\n", i) ;
        ret = xpn_conf_reader_load(&conf_data, conf) ;
	if (ret < 0)
	{
	    printf("\n") ;
	    continue ;
	}

	printf(">>> show xpn-%d.conf ...\n", i) ;
        ret = xpn_conf_reader_show(stdout, &conf_data) ;
	if (ret < 0)
	{
	    printf("\n") ;
	    continue ;
	}

	printf(">>> free xpn-%d.conf ...\n", i) ;
        ret = xpn_conf_reader_free(&conf_data) ;

	printf("\n") ;
    }

    return 0 ;
}

