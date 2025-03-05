
#include "TTU_Server.hpp"


gnetwork::TTU_Server::TTU_Server(const std::string &filen_crt, const std::string &filen_key, int domain, int service, 
                                 int protocol, int port, uint32_t interface, int bklg) : 
                                 TTU_ChatBase(filen_crt, filen_key, true), TCPServer(domain, service, protocol, port, interface, bklg), 
                                 UDPServer(domain, service, SOCK_DGRAM, 5555, interface) {} // TTU_ChatBase bool true for server


void gnetwork::TTU_Server::launch() {
    std::cout << "[TTU_Server] Waiting for client connections..." << std::endl;

    std::vector<std::thread> client_threads;

    // Accept multiple TLS clients, TCP
    std::thread tcp_thread([this, &client_threads]() {
        while (true) {
            int client_sock = get_serv_socket()->acceptserv(get_serv_socket()->get_sock(), get_serv_socket()->get_address());
            if (client_sock < 0) {
                perror("Client connection failed!");
                continue;
            }

            std::cout << "[TTU_Server] New client connected! Attempting TLS handshake..." << std::endl;

            client_threads.emplace_back([this, client_sock]() {
                SSL* ssl = SSL_new(get_tls_context());
                SSL_set_fd(ssl, client_sock);

                if (SSL_accept(ssl) > 0) {
                    std::cout << "[TTU_Server] TLS connection established!" << std::endl;
                    handle_tls_chat(ssl, true);
                } else {
                    ERR_print_errors_fp(stderr);
                    std::cerr << "[TTU_Server] TLS handshake failed!" << std::endl;
                }

                SSL_free(ssl);
                close(client_sock);
            });
        }
    });

    // Accept DTLS clients, UDP
    std::thread dtls_thread([this]() {
        std::cout << "[TTU_Server] Waiting for DTLS clients..." << std::endl;
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int udp_sock = get_udp_socket()->get_sock();

        handle_dtls_chat(get_dtls_context(), udp_sock, client_addr, true);
    });

    tcp_thread.join();
    dtls_thread.join();

    for (auto &thread : client_threads) {
        if (thread.joinable()) thread.join();
    }
}

// 30