
#include "Socket.hpp"


gnetwork::Socket::Socket(int domain, int service, int protocol, int port, ulong interface) {}

gnetwork::Socket::Socket(int domain, int service, int protocol, int port, u_long interface) {    
    address.sin_family = domain;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(interface);
    sock = socket(domain, service, protocol);
    
    test_conn(sock);
}

void gnetwork::Socket::test_conn(int item_to_test) {

    // checks if socket connection has been successully established 
    if (item_to_test < 0) {
        perror("failed to connect...");
        exit(EXIT_FAILURE);
    }
}

struct sockaddr_in gnetwork::Socket::get_address() {
    return address;
}

int gnetwork::Socket::get_sock() {
    return sock;
}
