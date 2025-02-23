
#include "AcceptingSocket.hpp"


gnetwork::AcceptingSocket::AcceptingSocket(int domain, int service, int protocol, int port, uint32_t interface, int bklg) 
                                        : ListeningSocket(domain, service, protocol, port, interface, bklg) {};

gnetwork::AcceptingSocket::~AcceptingSocket() { 
    close(get_sock()); 
}

int gnetwork::AcceptingSocket::accnetw(int sock, const struct sockaddr_in &server_address) {
    struct sockaddr_in client_address;
    socklen_t addrlen = sizeof(client_address);

    accept_sock = accept(sock, (struct sockaddr*) &client_address, &addrlen);

    if (accept_sock < 0) {
        perror("Accept failed");
    } else {
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_address.sin_addr, client_ip, INET_ADDRSTRLEN);
        std::cout << "Accepted connection from " << client_ip << ":" << ntohs(client_address.sin_port) << std::endl;
        std::cout << "Connected socket FD: " << accept_sock << std::endl;  // Log accepted socket
    }

    return accept_sock;
}

int gnetwork::AcceptingSocket::get_accepting() const {
    return accept_sock;
}

