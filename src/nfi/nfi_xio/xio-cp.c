// gcc -I$GLOBUS_LOCATION/include/gcc32dbg/ -c xio.c
// gcc xio.o -L$GLOBUS_LOCATION/lib/ -lglobus_xio_gcc32dbg -o xio
//
// gcc -Wall -Wextra -I$GLOBUS_LOCATION/include/gcc32dbg/ -L$GLOBUS_LOCATION/lib/ -lglobus_xio_gcc32dbg xio-cp.c -o xio-cp

#include "globus_xio.h"
#include "globus_xio_file_driver.h"
#include "globus_xio_gridftp_driver.h"

int
main(
    int                             argc,
    char *                          argv[])
{
    globus_result_t                 res;
    globus_xio_driver_t             driver_s, driver_d;
    globus_xio_stack_t              stack_s, stack_d;
    globus_xio_handle_t             handle_s, handle_d;
    globus_size_t                   nbytes;
    char *                          source = NULL;
    char *                          dest = NULL;
    char                            buf[8*8192];
    int 			    fd_s, fd_d;

    globus_xio_attr_t		    attr_s, attr_d;
    //globus_off_t		    offset;

    char *		            file_prefix = "file:/";
    char *		            gridftp_prefix = "gsiftp:/";
    int				    source_local = 0;
    int				    dest_local = 0;

    struct timeval t_start, t_end;
    long total_time;

    source = argv[1]; //gridftp file
    dest = argv[2]; //gridftp file

    globus_module_activate(GLOBUS_XIO_MODULE);

    //res = globus_xio_driver_load("file", &driver); // Probar con: ./xio file:/export/home/pato11-1/users/bbergua/grid/globus/xio/pruebas-preliminares/pruebaxio pruebaxiodevuelta
    if (strncmp(source, file_prefix, strlen(file_prefix)) == 0){
    	res = globus_xio_driver_load("file", &driver_s); // Probar con: ./xio file:/export/home/pato11-1/users/bbergua/grid/globus/xio/pruebas-preliminares/pruebaxio pruebaxiodevuelta
        assert(res == GLOBUS_SUCCESS);
    } else if (strncmp(source, gridftp_prefix, strlen(gridftp_prefix)) == 0){
        res = globus_xio_driver_load("gridftp", &driver_s); // Probar con: ./xio gsiftp://pato12.arcos.inf.uc3m.es/tmp/pruebaxiogridftp gsiftp://pato12.arcos.inf.uc3m.es/tmp/pruebaxiogridftpdevuelta
        assert(res == GLOBUS_SUCCESS);
    } else {
    	source_local = 1;
    }

    if (strncmp(dest, file_prefix, strlen(file_prefix)) == 0){
    	res = globus_xio_driver_load("file", &driver_d); // Probar con: ./xio file:/export/home/pato11-1/users/bbergua/grid/globus/xio/pruebas-preliminares/pruebaxio pruebaxiodevuelta
        assert(res == GLOBUS_SUCCESS);
    } else if (strncmp(dest, gridftp_prefix, strlen(gridftp_prefix)) == 0){
        res = globus_xio_driver_load("gridftp", &driver_d); // Probar con: ./xio gsiftp://pato12.arcos.inf.uc3m.es/tmp/pruebaxiogridftp gsiftp://pato12.arcos.inf.uc3m.es/tmp/pruebaxiogridftpdevuelta
        assert(res == GLOBUS_SUCCESS);
    } else {
    	dest_local = 1;
    }


    if (!source_local){
	    res = globus_xio_stack_init(&stack_s, NULL);
	    assert(res == GLOBUS_SUCCESS);

	    res = globus_xio_stack_push_driver(stack_s, driver_s);
	    assert(res == GLOBUS_SUCCESS);

	    res = globus_xio_handle_create(&handle_s, stack_s);
	    assert(res == GLOBUS_SUCCESS);

	    globus_xio_attr_init(&attr_s);

	    if (strncmp(source, gridftp_prefix, strlen(gridftp_prefix)) == 0){
	        //res = globus_xio_attr_cntl(attr_s, driver_s, GLOBUS_XIO_GRIDFTP_SET_PARTIAL_TRANSFER, GLOBUS_TRUE);
	        //assert(res == GLOBUS_SUCCESS);

		res = globus_xio_attr_cntl(attr_s, driver_s, GLOBUS_XIO_GRIDFTP_SET_NUM_STREAMS, 4);
	        assert(res == GLOBUS_SUCCESS);

		res = globus_xio_attr_cntl(attr_s, driver_s, GLOBUS_XIO_GRIDFTP_SET_TCP_BUFFER, 64*1024);
	        assert(res == GLOBUS_SUCCESS);
	    }

	    res = globus_xio_open(handle_s, source, attr_s);
	    assert(res == GLOBUS_SUCCESS);
    } else {
            fd_s = open(source, O_RDONLY);
	    assert(fd_s > 0);
    }

    if (!dest_local){
	    res = globus_xio_stack_init(&stack_d, NULL);
	    assert(res == GLOBUS_SUCCESS);

	    res = globus_xio_stack_push_driver(stack_d, driver_d);
	    assert(res == GLOBUS_SUCCESS);

	    res = globus_xio_handle_create(&handle_d, stack_d);
	    assert(res == GLOBUS_SUCCESS);

	    globus_xio_attr_init(&attr_d);

	    if (strncmp(dest, gridftp_prefix, strlen(gridftp_prefix)) == 0){
	        //res = globus_xio_attr_cntl(attr_d, driver_d, GLOBUS_XIO_GRIDFTP_SET_PARTIAL_TRANSFER, GLOBUS_TRUE);
	        //assert(res == GLOBUS_SUCCESS);

		res = globus_xio_attr_cntl(attr_d, driver_d, GLOBUS_XIO_GRIDFTP_SET_NUM_STREAMS, 4);
	        assert(res == GLOBUS_SUCCESS);

		res = globus_xio_attr_cntl(attr_d, driver_d, GLOBUS_XIO_GRIDFTP_SET_TCP_BUFFER, 64*1024);
	        assert(res == GLOBUS_SUCCESS);
	    }

	    res = globus_xio_open(handle_d, dest, attr_d);
	    assert(res == GLOBUS_SUCCESS);
    } else {
	    //fd = creat(dest, 0644);
	    fd_d = open(dest, O_WRONLY|O_CREAT|O_TRUNC);
	    assert(fd_d > 0);
    }

    gettimeofday(&t_start, NULL);
    
    do
    {
        if (!source_local){
            res = globus_xio_read(handle_s, buf, sizeof(buf) - 1, 1, &nbytes, NULL);
            //printf("globus_xio_read = %d, nbytes = %d, buf_s=\"%s\"\n", res, nbytes, buf);
        } else {
            //nbytes = read(fd_s, buf, SSIZE_MAX);
            nbytes = read(fd_s, buf, 8*8192 - 1);
            //printf("read = %d, buf=\"%s\"\n", nbytes, buf);
	}
        if(nbytes > 0)
        {
            if (!dest_local){
                //res = globus_xio_write(handle_d, buf_d, sizeof(buf_d) - 1, 1, &nbytes, NULL);
                res = globus_xio_write(handle_d, buf, nbytes, 1, &nbytes, NULL);
            } else {
	        nbytes = write(fd_d, buf, nbytes);
	    }
        }
    //} while(nbytes == 8*8192 - 1);
    } while(res == GLOBUS_SUCCESS);

   gettimeofday(&t_end, NULL);

/*
    bzero(buf, 8*8192);
    //offset = -10;
    offset = 39;
    //res = globus_xio_handle_cntl(handle, driver, GLOBUS_XIO_FILE_SEEK, &offset, GLOBUS_XIO_FILE_SEEK_SET);
    //res = globus_xio_handle_cntl(handle, driver, GLOBUS_XIO_FILE_SEEK, &offset, GLOBUS_XIO_FILE_SEEK_CUR);
    //res = globus_xio_handle_cntl(handle, driver, GLOBUS_XIO_FILE_SEEK, &offset, GLOBUS_XIO_FILE_SEEK_END);
    res = globus_xio_handle_cntl(handle_d, driver_d, GLOBUS_XIO_GRIDFTP_SEEK, offset);// implicitly use SEEK_SET
    printf("globus_xio_handle_cntl = %d, offset = %d\n", res, offset);
    assert(res == GLOBUS_SUCCESS);

    //res = globus_xio_read(handle_s, buf_s, sizeof(buf_s) - 1, 1, &nbytes, NULL);
    //printf("globus_xio_read = %d, nbytes = %d, buf_s=\"%s\"\n", res, nbytes, buf_s);

    nbytes = 4;
    //write(fd, buf, nbytes);
    //buf_d = malloc(nbytes);
    //memcpy(buf_d, buf_s, nbytes);
    //res = globus_xio_write(handle_d, buf_d, sizeof(buf_d) - 1, 1, &nbytes, NULL);
    res = globus_xio_write(handle_d, buf_d, nbytes, 1, &nbytes, NULL);
*/
    if (!source_local){
        globus_xio_attr_destroy(attr_s);
        globus_xio_close(handle_s, NULL);
    } else {
        close(fd_s);
    }
    if (!dest_local){
        globus_xio_attr_destroy(attr_d);
        globus_xio_close(handle_d, NULL);
    } else {
        close(fd_d);
    }

    globus_module_deactivate(GLOBUS_XIO_MODULE);

    total_time = (t_end.tv_sec - t_start.tv_sec) * 1000000 + t_end.tv_usec - t_start.tv_usec;
    printf("Total Time = %f seconds\n", ((float)total_time)/1000000);    

    return 0;
}
