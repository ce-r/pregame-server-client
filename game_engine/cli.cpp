
#include "both.hpp"


int main() {
    int serv_port = MY_PORT;
    struct sockaddr_in cliaddr;

    for (int i = 0; i < MAX_ATTEMPTS; i++) {
        int clifd = socket(AF_INET, SOCK_STREAM, 0);
        if (clifd < 0) {
            perror("Error creating socket");
            continue; // Retry next iteration
        }

        bzero(&cliaddr, sizeof(cliaddr));
        cliaddr.sin_family = AF_INET;
        cliaddr.sin_port = htons(serv_port);

        // Convert "127.0.0.1" to binary and check for errors
        if (inet_pton(AF_INET, "127.0.0.1", &cliaddr.sin_addr) <= 0) {
            perror("Invalid address / Address not supported");
            close(clifd);
            return NITS_SOCKET_ERROR;
        }

        if (connect(clifd, (struct sockaddr*) &cliaddr, sizeof(cliaddr)) == 0) {
            printf("Connected to server on attempt %d!\n", i + 1);
            close(clifd);
            return NITS_SOCKET_OK;
        }

        perror("Error connecting to server");
        close(clifd);

        if (i < MAX_ATTEMPTS - 1) {
            sleep(2); // Delay before retrying
        }
    }

    // printf("Failed to connect after %d attempts\n", MAX_ATTEMPTS);
    return NITS_SOCKET_ERROR;
}


// SEGMENTATION FAULT
// int main() {

//     // struct in_addr *serv_host;
//     int serv_port = MY_PORT;

//     // int clifd, i;
//     struct sockaddr_in cliaddr;
//     for (int i = 0; i < MAX_ATTEMPTS; i++) {
//         int clifd = socket(AF_INET, SOCK_STREAM, 0);
//         if (clifd < 0) {
//             perror("error in creating socket");
//             continue; // retry next iteration
//         }

//         bzero(&cliaddr, sizeof(cliaddr));
//         cliaddr.sin_family = AF_INET;
//         cliaddr.sin_port = htons(serv_port);
//         // cliaddr.sin_addr = *serv_host;
//         if (inet_pton(AF_INET, "127.0.0.1", &cliaddr.sin_addr) <= 0) {
//             perror("Invalid address / Address not supported");
//             close(clifd);
//             return NITS_SOCKET_ERROR;
//         }

//         if (connect(clifd, (struct sockaddr*) &cliaddr, sizeof(cliaddr)) == 0) {
//             // return clifd;
//             close(clifd);
//             return NITS_SOCKET_OK;
//         }

//         close(clifd);

//         if (i < MAX_ATTEMPTS - 1) {
//             sleep(2);// delay for 2 seconds before the next attempt
//         }
//     }

//     return 0;
// }