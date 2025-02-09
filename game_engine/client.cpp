#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include "network.hpp"


int main() {
    int cli_fd;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    // char buffer[BUFFER_SIZE];

    // Create socket
    cli_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (cli_fd == -1) {
        perror("Socket creation failed");
        return EXIT_FAILURE;
    }

    // Set up server address structure
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        return EXIT_FAILURE;
    }

    // Connect to server
    if (connect(cli_fd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        return EXIT_FAILURE;
    }

    std::cout << "Connected to the server.\n";

    // Send HTTP GET request
    std::string request = "GET / HTTP/1.1\r\nHost: localhost\r\nConnection: close\r\n\r\n";
    send(cli_fd, request.c_str(), request.length(), 0);

    // Receive response
    int bytes_received = read(cli_fd, buffer, BUFFER_SIZE);
    if (bytes_received > 0) {
        std::cout << "Server response:\n" << buffer << std::endl;
    }

    close(cli_fd);
    return 0;
}