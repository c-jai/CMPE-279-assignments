// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#define PORT 80


int main(int argc, char const *argv[]) 
{ 
    int server_fd, new_socket; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char *hello = "Hello from server"; 
    pid_t pid; 

    // --------------------------- Part 1: Set up socket ---------------------------
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    // Forcefully attaching socket to the port 80
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR , &opt, sizeof(opt)))
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    // Forcefully attaching socket to the port 80
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    if (listen(server_fd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 
    //accept connection from client and process data from client
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
                       (socklen_t*)&addrlen))<0) 
    { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    }

//--------------------------- Part 2: Re-exec the server’s child process after forking ---------------------------

    pid = fork();

    if(pid == 0) //if child process
    {
        printf("In child process\n");
        close(server_fd); 
        int setuidVal = setuid(-1); //drop privileges by setuid to "nobody"

        if(setuidVal != 0) //if drop privileges is not successful
            exit(EXIT_FAILURE); 

        //pass the socket file descriptor between the forked child and the new exec’ed child as command line arguments
        if(execl("exec_child","exec_child",&new_socket,hello,NULL) < 0)
        {
            printf("exec error\n");
            exit(0);
        }

    }

    close(new_socket); 

    if(pid < 0){ // error in creating child process
        perror("accept");
        exit(EXIT_FAILURE);
    }

    return 0;
}