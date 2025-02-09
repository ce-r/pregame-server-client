#include <iostream>
#include <thread>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include "network.hpp"


void handle_client(int cli_fd) {
    char buffer[BUFFER_SIZE] = {0};
    // char buffer[BUFFER_SIZE];

    // Read request
    int bytes_read = read(cli_fd, buffer, BUFFER_SIZE);
    if (bytes_read > 0) {
        std::cout << "Received request:\n" << buffer << std::endl;

        // Simple HTTP Response
        std::string response = 
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 13\r\n"
            "Connection: close\r\n"
            "\r\n"
            "Hello, world!";

        // Send response
        send(cli_fd, response.c_str(), response.size(), 0);
    }

    // Close client socket
    close(cli_fd);
}

int main() {
    int serv_fd, cli_fd;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t cli_len = sizeof(cli_addr);

    // Create socket
    serv_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (serv_fd == -1) {
        perror("Socket creation failed");
        return EXIT_FAILURE;
    }

    // Set up server address structure
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    // Bind socket
    if (bind(serv_fd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1) {
        perror("Bind failed");
        return EXIT_FAILURE;
    }

    // Listen for connections
    if (listen(serv_fd, 5) == -1) {
        perror("Listen failed");
        return EXIT_FAILURE;
    }

    std::cout << "Server listening on port " << PORT << "...\n";

    // Handle multiple clients using threads
    std::vector<std::thread> threads;
    while (true) {
        cli_fd = accept(serv_fd, (struct sockaddr*) &cli_addr, &cli_len);
        if (cli_fd == -1) {
            perror("Accept failed");
            continue;
        }

        std::cout << "New connection from " << inet_ntoa(cli_addr.sin_addr) << std::endl;
        threads.push_back(std::thread(handle_client, cli_fd));
        threads.back().detach();  // Detach the thread so it runs independently
    }

    // Close server socket
    close(serv_fd);
    return 0;
}