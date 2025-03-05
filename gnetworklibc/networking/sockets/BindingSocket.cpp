
#include "BindingSocket.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>

gnetwork::BindingSocket::BindingSocket(int domain, int service, int protocol, int port, uint32_t interface) 
                                        : Socket(domain, service, protocol, port, interface), socket_type(service) {
    socket_type = service;
    bind2self(get_sock(), get_address());
    // test_sock(binding);
};

gnetwork::BindingSocket::~BindingSocket() { 
    close(get_sock()); 
}

int gnetwork::BindingSocket::bind2self(int sock, const struct sockaddr_in &address) {
    std::cout << "[BindingSocket] Binding socket on port " << ntohs(address.sin_port) << std::endl;
    binding = bind(sock, (struct sockaddr*) &address, sizeof(address));
    if (binding < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    return binding;
}

int gnetwork::BindingSocket::get_binding() const {
    return binding;
}

// 20