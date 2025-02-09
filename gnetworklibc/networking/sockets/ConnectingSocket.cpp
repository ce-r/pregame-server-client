
#include "ConnectingSocket.hpp"


gnetwork::ConnectingSocket::ConnectingSocket(int domain, int service, int protocol, int port, ulong interface) 
                                            : Socket(domain, service, protocol, port, interface) {};

int gnetwork::ConnectingSocket::conn_to_netw(int sock, struct sockaddr_in address) {
    return connect(sock, (struct sockaddr*) &address, sizeof(address));
}
