#include "ListeningSocket.hpp"


gnetwork::ListeningSocket::ListeningSocket(int domain, int service, int protocol, int port, ulong interface, int bklg) 
                                            : BindingSocket(domain, service, protocol, port, interface){
    backlog = bklg;
    _listening();
    test_conn(listening);
}

void gnetwork::ListeningSocket::_listening() {
    listening = listen(get_sock(), backlog);
}

int gnetwork::ListeningSocket::get_listening() {
    return listening;
}

int gnetwork::ListeningSocket::get_backlog() {
    return backlog;
}