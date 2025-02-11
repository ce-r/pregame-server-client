
#include "ConnectingSocket.hpp"


gnetwork::ConnectingSocket::ConnectingSocket(int domain, int service, int protocol, int port, uint32_t interface) 
                                            : Socket(domain, service, protocol, port, interface) {};

gnetwork::ConnectingSocket::~ConnectingSocket() { 
    close(get_sock()); 
}

int gnetwork::ConnectingSocket::conn_to_netw(int sock, const struct sockaddr_in& address) {
    return connect(sock, (struct sockaddr*) &address, sizeof(address));
}
