#include <iostream>
#include <thread>
#include <vector>

#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 8080


int main(int argc, char const *argv[]) {
    int client_socket = 0;
    long bytes_recvd;
    struct sockaddr_in serv_addr;
    const char* hello = "hello from client";
    char buffer[1024] = {0};
    
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n socket creation failed \n");
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\n address not supported \n");
        return -1;
    }

    if(connect(client_socket, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
        printf("\n connection failed \n");
        return -1;
    }

    send(client_socket, hello, strlen(hello), 0);
    printf("\n hello message sent \n");
    bytes_recvd = read(client_socket, buffer, 1024);
    printf("%s\n", buffer);
    
    return 0;
}