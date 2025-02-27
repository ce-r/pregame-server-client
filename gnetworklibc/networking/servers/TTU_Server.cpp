

// #include "../servers/TCPServer.hpp"
// #include "../ttu/TTU_ChatBase.hpp"
// #include "TTU_Server.hpp"

// #define TCP_PORT 4443
// #define UDP_PORT 5555
// #define BUFFER_SIZE 1024

#include "TTU_Server.hpp"


gnetwork::TTU_Server::TTU_Server(const std::string &filen_crt, const std::string &filen_key, int domain, int service, 
                       int protocol, int port, uint32_t interface, int bklg) : 
                       TTU_ChatBase(filen_crt, filen_key, true), TCPServer(domain, service, protocol, port, interface, bklg), 
                       UDPServer(domain, service, SOCK_DGRAM, 5555, interface) {} // TTU_ChatBase bool true for server

void gnetwork::TTU_Server::launch() {
    std::thread udp_thread(&TTU_Server::udp_listener, this);
    udp_thread.detach();

    std::cout << "[TTU_Server] Waiting for client connection..." << std::endl;

    int client_sock = get_serv_socket()->accnetw(get_serv_socket()->get_sock(), get_serv_socket()->get_address());
    if (client_sock < 0) {
        perror("Client connection failed!");
        return;
    }
    std::cout << "Client connected! Attempting TLS handshake..." << std::endl;

    SSL* ssl = SSL_new(get_tls_context());
    SSL_set_fd(ssl, client_sock);
    
    if (SSL_accept(ssl) > 0) {
        std::cout << "TLS connection established!" << std::endl;
        handle_tls_chat(ssl, true);
    } else {
        ERR_print_errors_fp(stderr);
        std::cerr << "TLS handshake failed!" << std::endl;
    }

    SSL_free(ssl);
    close(client_sock);
}


// 30