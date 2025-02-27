
#include "UDPServer.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>


gnetwork::UDPServer::UDPServer(int domain, int service, int protocol, int port, uint32_t interface) {
    udp_socket = new BindingSocket(domain, SOCK_DGRAM, IPPROTO_UDP, port, interface);
    running = true;
    udp_thread = std::thread(&UDPServer::udp_listener, this);
}

gnetwork::UDPServer::~UDPServer() {
    stop_udp_listener();
    delete udp_socket;
}

void gnetwork::UDPServer::udp_listener() {
    char buffer[1024];
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);

    while (running) {
        int bytes = recvfrom(udp_socket->get_sock(), buffer, sizeof(buffer), 0, 
                             (struct sockaddr*)&client_addr, &addr_len);
        if (bytes > 0) {
            buffer[bytes] = '\0';
            std::cout << "[UDP Received]: " << buffer << std::endl;
        }
    }
}

void gnetwork::UDPServer::stop_udp_listener() {
    running = false;
    if (udp_thread.joinable()) {
        udp_thread.join();
    }
}

gnetwork::BindingSocket* gnetwork::UDPServer::get_udp_socket() const {
    return udp_socket;
}
