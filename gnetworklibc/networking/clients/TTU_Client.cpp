
#include "../clients/TTU_Client.hpp"


gnetwork::TTU_Client::TTU_Client(int domain, int service, int protocol, int port, uint32_t interface) : 
                       TTU_ChatBase("", "", false), // pass true for server certs
                       TCPClient(domain, service, protocol, port, interface), // inet_addr("127.0.0.1")
                       UDPClient(domain, SOCK_DGRAM, IPPROTO_UDP, 5555, interface) {} // inet_addr("127.0.0.1")

void gnetwork::TTU_Client::launch() {
    std::cout << "TCP connection established, now starting TLS..." << std::endl;

    SSL* ssl = SSL_new(get_tls_context());
    SSL_set_fd(ssl, get_cli_socket()->get_sock());
    
    if (SSL_connect(ssl) > 0) {
        std::cout << "TLS connection established!" << std::endl;
        handle_tls_chat(ssl, false);
    } else {
        ERR_print_errors_fp(stderr);
        std::cerr << "TLS handshake failed!" << std::endl;
    }
    
    SSL_free(ssl);
}

// 20