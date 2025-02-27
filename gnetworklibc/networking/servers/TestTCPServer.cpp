
#include "TestTCPServer.hpp"
#include <string.h>


gnetwork::TestTCPServer::TestTCPServer() : TCPServer(AF_INET, SOCK_STREAM, 0, 8080, INADDR_ANY, 10) {}

void gnetwork::TestTCPServer::reader() {
    struct sockaddr_in server_address = get_serv_socket()->get_address();
    int server_sock = get_serv_socket()->get_sock();
    std::cout << "Waiting for a new connection on socket: " << server_sock << std::endl;

    new_socket = get_serv_socket()->accnetw(server_sock, server_address);
    if (new_socket < 0) {
        perror("Accept failed");
        throw std::runtime_error("Failed to accept connection");
    }

    std::cout << "Accepted connection from client on socket: " << new_socket << std::endl;

    // Read data from the accepted socket
    ssize_t bytes_read = read(new_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read <= 0) {
        perror("Read failed or client closed connection");
        close(new_socket);
        return;
    }

    buffer[bytes_read] = '\0';
    std::cout << "Received from client: " << buffer << std::endl;

    const char* response = "Hello from server!";
    ssize_t bytes_sent = send(new_socket, response, strlen(response), 0);
    if (bytes_sent < 0) {
        perror("Send to client failed");
    } else {
        std::cout << "Response sent to client." << std::endl;
    }

    // Close the socket after sending response
    if (close(new_socket) < 0) {
        perror("Failed to close server socket");
    } else {
        std::cout << "Server socket closed successfully." << std::endl;
    }
}

void gnetwork::TestTCPServer::print_buffer() {
    std::cout << buffer << std::endl;
}

void gnetwork::TestTCPServer::writer() {
    const char* hello = "Hello from server";
    if (write(new_socket, hello, strlen(hello)) < 0) {
        throw std::runtime_error("Failed to write to socket");
    }
    close(new_socket);
}

// void gnetwork::TestTCPServer::slaunch() {
//     while (true) {
//         std::cout << "Waiting for connections..." << std::endl;
//         reader();
//         std::cout << "Done..." << std::endl;
//     }
// }

void gnetwork::TestTCPServer::launch() {
    std::cout << "Launching TestTCPServer...\n";
    // slaunch();
}

// 50