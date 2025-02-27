
#include "TCPClient.hpp"

gnetwork::TCPClient::TCPClient(int domain, int service, int protocol, int port, uint32_t interface) {
    cli_socket = new ConnectingSocket(domain, service, protocol, port, interface);
    // think about memory leak
    cli_socket->conn2netw(cli_socket->get_sock(), cli_socket->get_address());
}

gnetwork::TCPClient::~TCPClient() {
    delete cli_socket;
}

gnetwork::ConnectingSocket* gnetwork::TCPClient::get_cli_socket() const {
    return cli_socket;
} 

// 10