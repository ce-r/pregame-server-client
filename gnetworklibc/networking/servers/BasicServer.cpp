
#include "BasicServer.hpp"

gnetwork::BasicServer::BasicServer(int domain, int service, int protocol, int port, uint32_t interface, int bklg) {
    serv_sock = new AcceptingSocket(domain, service, protocol, port, interface, bklg);
    // think about memory leak
}

gnetwork::BasicServer::~BasicServer() {
    delete serv_sock;
}

gnetwork::AcceptingSocket* gnetwork::BasicServer::get_serv_socket() const {
    return serv_sock;
} 