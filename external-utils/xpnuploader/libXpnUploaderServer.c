#include <sys/stat.h>
#include <sys/wait.h>

#include "project/xpnuploader.nsmap"

#include "libXpnUploaderServer.h"

int debug;

int ns1__upload(struct soap *soap, char *source, char *dest, char *partition, int *result) {
	int fd, status;
	const char *tmp_config_template= "tmp.xpn.conf";
	char *tmp_config_filename;
	pid_t pid;
	char * envp[] = {(char *)NULL, (char *)NULL};
	
	if (debug)
		printf("ns1__upload(%s, %s, %s)", source, dest, partition);
	
	if ((source == NULL) || (strlen(source) <= 0))
		return soap_sender_fault(soap, "source not valid", "source is NULL or has non-positive length");
	
	if ((dest == NULL) || (strlen(dest) <= 0))
		return soap_sender_fault(soap, "dest not valid", "dest is NULL or has non-positive length");
	
	if ((partition == NULL) || (strlen(partition) <= 0))
		return soap_sender_fault(soap, "partition not valid", "partition is NULL or has non-positive length");
	
	//*result = system("xpncp source dest");
	pid = vfork();
	if (pid == 0) {
		//printf("sizeof(pid_t)=%d\n", sizeof(pid_t));
		tmp_config_filename = malloc(strlen(tmp_config_template)+12); // 12 = '.' + pid_t (4 bytes => 10 chars) + '\0'
		sprintf(tmp_config_filename, "%s.%d", tmp_config_template, getpid());
		//printf("tmp_config_filename = %s\n", tmp_config_filename);
		fd = creat(tmp_config_filename, S_IRUSR|S_IWUSR);
		if (fd <= 0) {
			fprintf(stderr, "Could not create %s\n", tmp_config_filename);
			return SOAP_ERR;
		}
		write(fd, partition, strlen(partition));
		close(fd);
		
		//setenv("XPN_CONF", tmp_config_filename, 1);
		envp[0] = malloc(strlen("XPN_CONF=")+strlen(tmp_config_filename)+1);
		sprintf(envp[0], "XPN_CONF=%s", tmp_config_filename);
		
		//*result = execle("xpncp", "xpncp", "-S", source, dest, (char *)NULL, &envp);
		*result = execle("xpncp", "xpncp", source, dest, (char *)NULL, &envp);
		//*result = execl("xpncp", "xpncp", source, dest, (char *)NULL);
		if (*result == -1)
			perror("execle failed");
		
		exit(0);
	} else {
		sprintf(tmp_config_filename, "%s.%d", "tmp.xpn.conf", pid);
	}
	
	waitpid(pid, &status, 0);
	unlink(tmp_config_filename);
	free(tmp_config_filename);
	
	if (debug)
		printf("=%d\n", *result);
	
	return SOAP_OK;
}
