#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <sys/wait.h>
#define PORT 80
int main(int argc, char const *argv[]) 
{
	printf("In exec'd child process\n");

	int passed_socket_descriptor;
    const char* hello_msg;

	if(argc > 1){ //if args are present
        passed_socket_descriptor=*argv[1];
		hello_msg=argv[2];
	}
	else
	{
		printf("Error...\n");
		exit(0);
	}
	char buffer[1024] = {0};
	int valread = read( passed_socket_descriptor ,buffer, 1024); 
	printf("%s\n",buffer ); 
	send(passed_socket_descriptor , hello_msg , strlen(hello_msg) , 0 ); 
	printf("Hello message sent from exec'd child\n"); 
	return 0;
}
