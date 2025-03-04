
#include "../clients/TTU_Client.hpp"


gnetwork::TTU_Client::TTU_Client(int domain, int service, int protocol, int port, uint32_t interface) : 
                       TTU_ChatBase("", "", false), // pass true for server certs
                       TCPClient(domain, service, protocol, port, interface), // inet_addr("127.0.0.1")
                       UDPClient(domain, SOCK_DGRAM, IPPROTO_UDP, 5555, interface) {} // inet_addr("127.0.0.1")

void gnetwork::TTU_Client::launch() {
    std::cout << "Establishing TLS (TCP) connection..." << std::endl;

    // Establish TLS, TCP connection
    std::thread tls_thread([this]() {
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
    });

    // Establish DTLS, UDP connection
    std::thread dtls_thread([this]() {
        std::cout << "[TTU_Client] Connecting to DTLS chat..." << std::endl;
        struct sockaddr_in server_addr;
        int udp_sock = get_udp_socket()->get_sock();

        handle_dtls_chat(get_dtls_context(), udp_sock, server_addr, false);
    });

    tls_thread.join();
    dtls_thread.join();
}

// 20