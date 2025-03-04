
#include "TTU_ChatBase.hpp"
#include <iostream>
#include <cstring>
#include <openssl/err.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>

#define BUFFER_SIZE 1024


gnetwork::TTU_ChatBase::TTU_ChatBase(const std::string &filen_crt, const std::string &filen_key, bool server) : 
                                     TTU(filen_crt, filen_key, server), is_server(server) {
    tls_context = create_context(is_server);
    dtls_context = create_context(is_server);
}

gnetwork::TTU_ChatBase::~TTU_ChatBase() {
    SSL_CTX_free(tls_context);
    SSL_CTX_free(dtls_context);
}

SSL_CTX* gnetwork::TTU_ChatBase::get_tls_context() const {
    return tls_context;
}

SSL_CTX* gnetwork::TTU_ChatBase::get_dtls_context() const {
    return dtls_context;
}

void gnetwork::TTU_ChatBase::handle_tls_chat(SSL* ssl, bool is_server) {
    char buffer[BUFFER_SIZE];
    int sockfd = SSL_get_fd(ssl);

    std::cout << (is_server ? "[TLS Server] Chat session started." : "[TLS Client] Chat session started.") << std::endl;

    // Launch sending thread
    std::thread send_thread([&]() {
        while (true) { 
            std::string msg;
            std::cout << "\n" << (is_server ? "[TLS Server] " : "[TLS Client] ") << "Enter message: ";
            std::getline(std::cin, msg);

            if (msg == "/quit")
                break;

            SSL_write(ssl, msg.c_str(), msg.length());
        }
    });

    fd_set read_fds;
    struct timeval timeout;
    
    while (true) {
        FD_ZERO(&read_fds);
        FD_SET(sockfd, &read_fds);

        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        // Check if there's data available
        if (select(sockfd + 1, &read_fds, nullptr, nullptr, &timeout) > 0) {
            while (true) { // Read all available messages
                memset(buffer, 0, sizeof(buffer));
                int bytes = SSL_read(ssl, buffer, sizeof(buffer) - 1);

                if (bytes > 0) {
                    std::cout << "\n" << (is_server ? "[TLS Server] " : "[TLS Client] ") << "Received: " << buffer << std::endl;
                } else {
                    break;
                }
            }
        }
    }

    send_thread.join();
}

// multiple clients
void gnetwork::TTU_ChatBase::handle_dtls_chat(SSL_CTX* dtls_ctx, int udp_sock, struct sockaddr_in &peer_addr, bool is_server) {
    char buffer[BUFFER_SIZE];
    socklen_t peer_len = sizeof(peer_addr);
    std::set<std::string> active_clients;

    while (true) {
        memset(buffer, 0, BUFFER_SIZE);
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);

        int bytes = recvfrom(udp_sock, buffer, sizeof(buffer), 0, (struct sockaddr*) &client_addr, &client_len);
        if (bytes <= 0) 
            continue;

        // Convert client address to string for tracking
        std::string client_key = inet_ntoa(client_addr.sin_addr) + std::to_string(ntohs(client_addr.sin_port));
        active_clients.insert(client_key);

        std::cout << "\n[DTLS Server] Received from " << client_key << ": " << buffer << std::endl;

        // Echo message back to all active clients
        for (const std::string &client : active_clients) {
            sendto(udp_sock, buffer, bytes, 0, (struct sockaddr*) &client_addr, client_len);
        }
    }
}
