
#include "both.hpp"


int main() {
    struct sockaddr_in servaddr;

    int servfd = socket(AF_INET, SOCK_STREAM, 0);
    if (servfd < 0) {
        perror("Error creating socket");
        return NITS_SOCKET_ERROR;
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(MY_PORT);

    if (bind(servfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        perror("Error binding socket");
        close(servfd);
        return NITS_SOCKET_ERROR;
    }

    if (listen(servfd, 5) < 0) {
        perror("Error listening on socket");
        close(servfd);
        return NITS_SOCKET_ERROR;
    }

    printf("Server listening on port %d...\n", MY_PORT);

    struct sockaddr_in cliaddr;
    socklen_t clilen = sizeof(cliaddr);
    int connfd = accept(servfd, (struct sockaddr*) &cliaddr, &clilen);

    if (connfd < 0) {
        perror("Error accepting connection");
        close(servfd);
        return NITS_SOCKET_ERROR;
    }

    printf("Client connected! Start chatting...\n");

    char buffer[MAXLINE];
    while (1) {
        bzero(buffer, MAXLINE);
        int bytes_received = read(connfd, buffer, MAXLINE - 1);

        if (bytes_received <= 0) {
            printf("Client disconnected or error occurred.\n");
            break;
        }

        printf("Client: %s", buffer);

        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Client ended the chat.\n");
            break;
        }

        write(connfd, buffer, strlen(buffer));
    }

    close(connfd);
    close(servfd);
    return 0;
}