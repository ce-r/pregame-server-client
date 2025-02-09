#include "BasicServer.hpp"

gnetwork::BasicServer::BasicServer(int domain, int service, int protocol, int port, u_long interface, int bklg) {
    socket = new ListeningSocket(domain, service, protocol, port, interface, bklg);
    // think about memory leak
}


gnetwork::ListeningSocket* gnetwork::BasicServer::get_socket(){
    return socket;
} 