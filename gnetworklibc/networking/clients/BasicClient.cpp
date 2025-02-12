
#include "BasicClient.hpp"

gnetwork::BasicClient::BasicClient(int domain, int service, int protocol, int port, uint32_t interface) {
    cli_socket = new ConnectingSocket(domain, service, protocol, port, interface);
    // think about memory leak
}

gnetwork::BasicClient::~BasicClient() {
    delete cli_socket;
}

gnetwork::ConnectingSocket* gnetwork::BasicClient::get_cli_socket() const {
    return cli_socket;
} 