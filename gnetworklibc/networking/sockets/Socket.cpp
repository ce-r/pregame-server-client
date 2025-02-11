
#include "Socket.hpp"


gnetwork::Socket::Socket(int domain, int service, int protocol, int port, uint32_t interface) {    
    address.sin_family = domain;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(interface);
    sock = socket(domain, service, protocol);
    
    test_conn(sock);
}

gnetwork::Socket::~Socket() {
    close(sock);
}

void gnetwork::Socket::test_conn(int item_to_test) {

    // checks if socket connection has been successully established 
    if (item_to_test < 0) {
        throw std::runtime_error("Failed to create socket");
    }
}

struct sockaddr_in gnetwork::Socket::get_address() const {
    return address;
}

int gnetwork::Socket::get_sock() const {
    return sock;
}
