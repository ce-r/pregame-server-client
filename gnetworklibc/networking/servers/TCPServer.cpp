
#include "TCPServer.hpp"


gnetwork::TCPServer::TCPServer(int domain, int service, int protocol, int port, uint32_t interface, int bklg) {
    // serv_sock = new AcceptingSocket(domain, service, protocol, port, interface, bklg);
    serv_sock = new AcceptingSocket(domain, SOCK_STREAM, IPPROTO_TCP, port, interface, bklg);
}

gnetwork::TCPServer::~TCPServer() {
    delete serv_sock;
}

gnetwork::AcceptingSocket* gnetwork::TCPServer::get_serv_socket() const {
    return serv_sock;
} 

// 5