
#include "TestServer.hpp"
#include <string.h>


gnetwork::TestServer::TestServer() : BasicServer(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, 10) {
    launch();
}

void gnetwork::TestServer::acceptance() {
    struct sockaddr_in address = get_socket()->get_address();
    int addrlen = sizeof(address);
    new_socket = accept(get_socket()->get_sock(), (struct sockaddr*) &address, (socklen_t*) &addrlen);
    read(new_socket, buffer, 30000);
}

void gnetwork::TestServer::print_buffer() {
    std::cout << buffer << std::endl;
}

void gnetwork::TestServer::writer() {
    const char* hello = "hello from server";
    write(new_socket, hello, strlen(hello));
    close(new_socket);
}

void gnetwork::TestServer::launch() {
    while (true) {
        std::cout << "==== waiting ====" << std::endl;
        acceptance();
        print_buffer();
        writer();
        std::cout << "==== done ====" << std::endl;
    }
}