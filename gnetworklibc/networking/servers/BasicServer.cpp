#include "BasicServer.hpp"

gnetwork::BasicServer::BasicServer(int domain, int service, int protocol, int port, uint32_t interface, int bklg) {
    socket = new ListeningSocket(domain, service, protocol, port, interface, bklg);
    // think about memory leak
}

gnetwork::BasicServer::~BasicServer() {
    delete socket;
}

gnetwork::ListeningSocket* gnetwork::BasicServer::get_socket() const {
    return socket;
} 