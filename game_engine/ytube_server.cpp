#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 8080


int main(int argc, char const *argv[]) {
    int serv_fd, cli_fd; // conn_fd; 
    long valread;
    struct sockaddr_in serv_address;
    int addrlen = sizeof(serv_address);
    
    const char* hello = "hello from server";
    
    if ((serv_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("\n socket creation failed \n");
        exit(EXIT_FAILURE);
    }

    serv_address.sin_family = AF_INET;
    serv_address.sin_addr.s_addr = INADDR_ANY;
    serv_address.sin_port = htons(PORT);
    
    memset(serv_address.sin_zero, '\0', sizeof serv_address.sin_zero);
    if (bind(serv_fd, (struct sockaddr*) &serv_address, sizeof(serv_address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(serv_fd, 10) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
    
    while(1) {
        printf("\n +++++++ waiting for new connection ++++++++ \n\n");
        if ((cli_fd = accept(serv_fd, (struct sockaddr*) &serv_address, (socklen_t*) &addrlen)) < 0) {
            perror("\n accept failed \n");
            exit(EXIT_FAILURE);
        }
        
        char buffer[30000] = {0};
        valread = read(cli_fd, buffer, 30000);
        printf("%s\n", buffer);
        write(cli_fd, hello, strlen(hello));
        printf("\n ------------------ hello message sent ------------------ \n");
        close(cli_fd);
    }

    return 0;
}