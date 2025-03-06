
#include "both.hpp"


int main() {
    int serv_port = MY_PORT;
    struct sockaddr_in servaddr;

    int clifd = socket(AF_INET, SOCK_STREAM, 0);
    if (clifd < 0) {
        perror("Error creating socket");
        return NITS_SOCKET_ERROR;
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(serv_port);

    if (inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr) <= 0) {
        perror("Invalid address / Address not supported");
        close(clifd);
        return NITS_SOCKET_ERROR;
    }

    if (connect(clifd, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0) {
        perror("Error connecting to server");
        close(clifd);
        return NITS_SOCKET_ERROR;
    }

    printf("Connected to server! Start chatting...\nType 'exit' to quit.\n");

    char buffer[MAXLINE];
    while (1) {
        printf("You: ");
        bzero(buffer, MAXLINE);
        fgets(buffer, MAXLINE, stdin);

        write(clifd, buffer, strlen(buffer));

        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Exiting chat...\n");
            break;
        }

        bzero(buffer, MAXLINE);
        int bytes_received = read(clifd, buffer, MAXLINE - 1);
        if (bytes_received <= 0) {
            printf("Server disconnected.\n");
            break;
        }

        printf("Server: %s", buffer);
    }

    close(clifd);
    return 0;
}