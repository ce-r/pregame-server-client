
#include "UDPClient.hpp"
#include <iostream>
#include <cstring>
#include <arpa/inet.h>


gnetwork::UDPClient::UDPClient(int domain, int service, int protocol, int port, uint32_t interface) {
    udp_socket = new ConnectingSocket(domain, service, protocol, port, interface);
    server_address.sin_family = domain;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = interface;
}

gnetwork::UDPClient::~UDPClient() {
    delete udp_socket;
}

void gnetwork::UDPClient::send_udp_message(const std::string &msg) {
    sendto(udp_socket->get_sock(), msg.c_str(), msg.length(), 0, 
           (struct sockaddr*) &server_address, sizeof(server_address));
    std::cout << "[UDP Sent]: " << msg << std::endl;
}


