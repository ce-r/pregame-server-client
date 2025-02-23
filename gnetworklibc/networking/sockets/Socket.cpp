
#include "Socket.hpp"


gnetwork::Socket::Socket(int domain, int service, int protocol, int port, uint32_t interface) {    
    address.sin_family = domain;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(interface);
    sock = socket(domain, service, protocol);
    if (sock < 0) {
        perror("Socket creation failed");
        throw std::runtime_error("Failed to create socket...");
    }


    int opt = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt(SO_REUSEADDR) failed...");
        exit(EXIT_FAILURE);
    }
    
    if (setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt)) < 0) {
        perror("setsockopt(TCP_NODELAY) failed...");
        exit(EXIT_FAILURE);
    }

    test_sock(sock);
}

gnetwork::Socket::~Socket() {
    if (sock >= 0) {
        close(sock);
        sock = -1;
    }
}

struct sockaddr_in gnetwork::Socket::get_address() const {
    return address;
}

int gnetwork::Socket::get_sock() const {
    return sock;
}

void gnetwork::Socket::test_sock(int test_socket) {

    // checks if socket connection has been successully established 
    if (test_socket < 0) {
        throw std::runtime_error("Failed to create socket...");
    }
}

void gnetwork::Socket::set_address(const struct sockaddr_in &addr) {
    address = addr;
}