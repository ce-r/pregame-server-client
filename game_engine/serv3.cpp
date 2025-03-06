
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
    int acceptfd = accept(servfd, (struct sockaddr*) &cliaddr, &clilen);

    if (acceptfd < 0) {
        perror("Error accepting connection");
        close(servfd);
        return NITS_SOCKET_ERROR;
    }

    printf("Client connected! Start chatting...\n");

    char buffer[MAXLINE];

    while (1) {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(acceptfd, &readfds);
        FD_SET(STDIN_FILENO, &readfds); // Allow server to type messages too

        int maxfd = acceptfd > STDIN_FILENO ? acceptfd : STDIN_FILENO;
        
        // STUDY THIS PORTION
        if (select(maxfd + 1, &readfds, NULL, NULL, NULL) < 0) {
            perror("Error in select()");
            break;
        }

        if (FD_ISSET(acceptfd, &readfds)) { 
            bzero(buffer, MAXLINE);
            int bytes_received = read(acceptfd, buffer, MAXLINE - 1);
            if (bytes_received <= 0) {
                printf("Client disconnected.\n");
                break;
            }
            printf("Client: %s", buffer);
            if (strncmp(buffer, "exit", 4) == 0) {
                printf("Client ended the chat.\n");
                break;
            }
        }

        if (FD_ISSET(STDIN_FILENO, &readfds)) {
            bzero(buffer, MAXLINE);
            fgets(buffer, MAXLINE, stdin);
            write(acceptfd, buffer, strlen(buffer));

            if (strncmp(buffer, "exit", 4) == 0) {
                printf("Exiting chat...\n");
                break;
            }
        }
    }

    close(acceptfd);
    close(servfd);
    return 0;
}
