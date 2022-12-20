#include <stdio.h>
#include <netdb.h>

int main(int argc, char *argv[])
{
     struct hostent *hp;

     hp = gethostbyname(argv[1]);
     if (hp == NULL){
		perror("Errore en gethostbyname");
     } else {
		printf("All Ok\n");
     }

     return 0;
}
