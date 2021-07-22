int main(int argc, char *argv[]) {
	printf("Soy %d y mi LD_PRELOAD es %s\n", getpid(), getenv("LD_PRELOAD"));
	printf("Realizando execve -> ");
	execve("/bin/ls", 0, 0);
	printf("Vuelve de execve => ERROR\n");
	exit(0);
}
