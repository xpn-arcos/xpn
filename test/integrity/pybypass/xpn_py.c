
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>


int open2 ( char *name, char *flags )
{
	int ret ;
	int flags_c = 0 ;

	if (strchr(flags, 'a') != NULL)
	    flags_c = flags_c | O_APPEND ;
	if (strchr(flags, 'w') != NULL)
	    flags_c = flags_c | O_CREAT | O_WRONLY ;
	if (strchr(flags, 'r') != NULL)
	    flags_c = flags_c | O_RDONLY ;
	if (strchr(flags, '+') != NULL)
	    flags_c = flags_c | O_RDWR ;

	ret = open(name, flags_c, S_IRWXU) ;
	printf("%d = xpn_open(name:'%s', flags:%d, mode:%d)\n", ret, name, flags_c, S_IRWXU) ;

	return ret ;
}

int write2 ( int fd, char *buffer, int buffer_size )
{
	int ret ;

	ret = write(fd, buffer, buffer_size) ;
	printf("%d = xpn_write(fd:%d, buffer:'%.10s...', buffer_size:%d)\n", ret, fd, buffer, buffer_size) ;

	return ret ;
}


char * read2 ( int fd )
{
	int     ret ;
        struct  stat st;
        char   *buffer ;

        fstat(fd, &st);
	buffer = malloc(st.st_size) ;

	ret = read(fd, buffer, st.st_size) ;
	printf("%d = xpn_read(fd:%d, buffer:'%.10s...', buffer_size:%d)\n", ret, fd, buffer, 1024) ;

	return buffer ;  // TODO: free buffer... how?
}

int close2 ( int fd )
{
	int ret ;

	ret = close(fd) ;
	printf("xpn_close(fd: %d)\n", fd) ;

	return ret ;
}

