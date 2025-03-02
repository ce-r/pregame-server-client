
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

    if (is_server){
        configure_context(tls_context);
        configure_context(dtls_context);
    }
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

    std::cout << (is_server ? "Server: Chat session started." : "Client: Chat session started.") << std::endl;

    while (true) {
        if (is_server) { 
            memset(buffer, 0, sizeof(buffer));
            int bytes = SSL_read(ssl, buffer, sizeof(buffer));
            if (bytes <= 0) {
                std::cout << "Client disconnected or error occurred." << std::endl;
                break;
            }

            std::cout << "Client: " << buffer << std::endl;
            std::string reply;
            std::cout << "Server: ";
            std::getline(std::cin, reply);

            SSL_write(ssl, reply.c_str(), reply.length());
        } else { 
            std::string msg;
            std::cout << "Client: ";
            std::getline(std::cin, msg);

            if (msg == "/quit") 
                break;
            
            SSL_write(ssl, msg.c_str(), msg.length());

            memset(buffer, 0, sizeof(buffer));
            int bytes = SSL_read(ssl, buffer, sizeof(buffer));
            if (bytes <= 0) {
                std::cout << "Server disconnected or error occurred." << std::endl;
                break;
            }
            std::cout << "Server: " << buffer << std::endl;
        }
    }
}

// 55

void gnetwork::TTU_ChatBase::handle_dtls_chat(SSL_CTX* dtls_ctx, int udp_sock, struct sockaddr_in &peer_addr, bool is_server) {
    char buffer[BUFFER_SIZE];
    socklen_t peer_len = sizeof(peer_addr);
    
    // Create SSL session from the context
    SSL* ssl = SSL_new(dtls_ctx);
    if (!ssl) {
        ERR_print_errors_fp(stderr);
        throw std::runtime_error("Failed to create DTLS SSL session");
    }

    BIO* bio = BIO_new_dgram(udp_sock, BIO_NOCLOSE);
    if (!bio) {
        ERR_print_errors_fp(stderr);
        SSL_free(ssl);
        throw std::runtime_error("Failed to create BIO for DTLS");
    }

    SSL_set_bio(ssl, bio, bio);
    BIO_ctrl(bio, BIO_CTRL_DGRAM_SET_RECV_TIMEOUT, 1, nullptr);

    if (is_server) {
        if (SSL_accept(ssl) <= 0) {
            ERR_print_errors_fp(stderr);
            SSL_free(ssl);
            return;
        }
    } else {
        if (SSL_connect(ssl) <= 0) {
            ERR_print_errors_fp(stderr);
            SSL_free(ssl);
            return;
        }
    }

    std::cout << "[DTLS] Secure chat established!" << std::endl;

    std::thread send_thread([&]() {
        while (true) {
            std::string msg;
            std::cout << (is_server ? "[DTLS Server] " : "[DTLS Client] ") << "Enter message: ";
            std::getline(std::cin, msg);
            SSL_write(ssl, msg.c_str(), msg.length());
        }
    });

    while (true) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes = SSL_read(ssl, buffer, sizeof(buffer));
        if (bytes > 0) {
            std::cout << (is_server ? "[DTLS Server] " : "[DTLS Client] ") << "Received: " << buffer << std::endl;
        }
    }

    send_thread.join();
    SSL_free(ssl);  // Free SSL session
}