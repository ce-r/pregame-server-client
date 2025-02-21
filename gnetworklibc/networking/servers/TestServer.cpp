
#include "TestServer.hpp"
#include <string.h>


gnetwork::TestServer::TestServer() : BasicServer(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, 10) {}

void gnetwork::TestServer::acceptance() {
    struct sockaddr_in address = get_serv_socket()->get_address();
    int addrlen = sizeof(address);
    new_socket = accept(get_serv_socket()->get_sock(), (struct sockaddr*) &address, (socklen_t*) &addrlen);
    if (new_socket < 0) {
        throw std::runtime_error("Failed to accept connection");
    }
    read(new_socket, buffer, 30000);
}

void gnetwork::TestServer::print_buffer() {
    std::cout << buffer << std::endl;
}

void gnetwork::TestServer::writer() {
    const char* hello = "Hello from server";
    if (write(new_socket, hello, strlen(hello)) < 0) {
        throw std::runtime_error("Failed to write to socket");
    }
    close(new_socket);
}

void gnetwork::TestServer::slaunch() {
    while (true) {
        std::cout << "Waiting for connections..." << std::endl;
        acceptance();
        writer();
        print_buffer();
        std::cout << "Done..." << std::endl;
    }
}

void gnetwork::TestServer::launch() {
    std::cout << "Launching TestServer...\n";
    slaunch();
}