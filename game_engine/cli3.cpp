
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
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(clifd, &readfds); // adds a file descriptor to be monitored
        FD_SET(STDIN_FILENO, &readfds); // Allow user to type messages

        int maxfd = clifd > STDIN_FILENO ? clifd : STDIN_FILENO;
        
        // select() is used to monitor STDIN_FILENO/keyboard input 
        // and the socket descriptor
        // allows bidirectional communication at any time
        // prevents  read() or write() from
        // blocking indefinitely, 
        // select() efficiently waits for input
        // select() blocks until data is ready to be read or written
        // waits until a file descriptor is ready for reading.
        if (select(maxfd + 1, &readfds, NULL, NULL, NULL) < 0) {
            perror("Error in select()");
            break;
        }
        // checks if specific fd is ready for use
        if (FD_ISSET(STDIN_FILENO, &readfds)) {
            // Read from user input
            bzero(buffer, MAXLINE);
            fgets(buffer, MAXLINE, stdin);
            write(clifd, buffer, strlen(buffer));

            if (strncmp(buffer, "exit", 4) == 0) {
                printf("Exiting chat...\n");
                break;
            }
        }

        if (FD_ISSET(clifd, &readfds)) {
            // Read from server
            bzero(buffer, MAXLINE);
            int bytes_received = read(clifd, buffer, MAXLINE - 1);
            if (bytes_received <= 0) {
                printf("Server disconnected.\n");
                break;
            }
            printf("Server: %s", buffer);
        }
    }

    close(clifd);
    return 0;
}

