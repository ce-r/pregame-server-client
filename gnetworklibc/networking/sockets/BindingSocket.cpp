
#include "BindingSocket.hpp"


gnetwork::BindingSocket::BindingSocket(int domain, int service, int protocol, int port, ulong interface) 
                                        : Socket(domain, service, protocol, port, interface) {
    conn_to_netw(get_sock(), get_address());
    test_conn(binding);
};

int gnetwork::BindingSocket::conn_to_netw(int sock, struct sockaddr_in address) {
    binding = bind(sock, (struct sockaddr*) &address, sizeof(address));
    return binding;
}

int gnetwork::BindingSocket::get_binding() {
    return binding;
}