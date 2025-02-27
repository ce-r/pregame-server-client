
#include "Socket.hpp"


gnetwork::Socket::Socket(int domain, int service, int protocol, int port, uint32_t interface) : socket_type(service) {    
    std::cout << "Creating socket: domain=" << domain 
              << ", service=" << service 
              << ", protocol=" << protocol 
              << ", port=" << port 
              << ", interface=" << interface << std::endl;

    address.sin_family = domain;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(interface);
    sock = socket(domain, service, protocol);

    if (sock < 0) {
        perror("Socket creation failed");
        throw std::runtime_error("Failed to create socket...");
    }

    int opt = 1;

    // SO_REUSEADDR for both TCP and UDP sockets
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt(SO_REUSEADDR) failed...");
        exit(EXIT_FAILURE);
    }

    // apply TCP_NODELAY only for TCP sockets
    if (socket_type == SOCK_STREAM) {
        if (setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt)) < 0) {
            perror("setsockopt(TCP_NODELAY) failed...");
            exit(EXIT_FAILURE);
        }
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
    char ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &addr.sin_addr, ip_str, INET_ADDRSTRLEN);

    std::cout << "[Socket] Storing IP: " << ip_str << " Port: " << ntohs(addr.sin_port) << std::endl;
    address = addr;
}

int gnetwork::Socket::get_socket_type() const {
    return socket_type;
}