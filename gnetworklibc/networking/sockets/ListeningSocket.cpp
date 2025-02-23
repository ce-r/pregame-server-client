
#include "ListeningSocket.hpp"


gnetwork::ListeningSocket::ListeningSocket(int domain, int service, int protocol, int port, uint32_t interface, int bklg) 
                                            : BindingSocket(domain, service, protocol, port, interface) {
    backlog = bklg;
    _listening();
    test_sock(listening);
}

gnetwork::ListeningSocket::~ListeningSocket() { 
    close(get_sock()); 
}

void gnetwork::ListeningSocket::_listening() {
    listening = listen(get_sock(), backlog);
    if (listening < 0) {
        throw std::runtime_error("Failed to listen on socket...");
    }
}

int gnetwork::ListeningSocket::get_listening() {
    return listening;
}

int gnetwork::ListeningSocket::get_backlog() {
    return backlog;
}
