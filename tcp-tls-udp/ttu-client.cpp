
// client.cpp

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <thread>

#define SERVER_IP "127.0.0.1"
#define TCP_PORT 4443
#define UDP_PORT 5555
#define BUFFER_SIZE 1024

void init_openssl() {
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
}

void cleanup_openssl() {
    EVP_cleanup();
}

SSL_CTX* create_client_context() {
    const SSL_METHOD* method = TLS_client_method();
    SSL_CTX* ctx = SSL_CTX_new(method);
    if (!ctx) {
        perror("Unable to create SSL context");
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
    return ctx;
}

void handle_tls_chat(SSL* ssl, int udp_sock, struct sockaddr_in udp_addr) {
    char buffer[BUFFER_SIZE];
    while (true) {
        std::string msg;
        std::cout << "Client: ";
        std::getline(std::cin, msg);

        if (msg == "/quit") {
            break;
        } else if (msg == "/stream") {
            // trigger UDP message
            const char* udp_msg = "Triggered UDP Stream Message!";
            sendto(udp_sock, udp_msg, strlen(udp_msg), 0, (struct sockaddr*) &udp_addr, sizeof(udp_addr));
            std::cout << "[UDP Sent]: " << udp_msg << std::endl;
        } else {
            // send over TLS
            SSL_write(ssl, msg.c_str(), msg.length());

            memset(buffer, 0, BUFFER_SIZE);
            int bytes = SSL_read(ssl, buffer, sizeof(buffer));
            if (bytes <= 0) {
                std::cout << "Server disconnected or error occurred." << std::endl;
                break;
            }
            std::cout << "Server: " << buffer << std::endl;
        }
    }
}

int main() {
    init_openssl();
    SSL_CTX* ctx = create_client_context();

    // TCP/TLS connection
    int tcp_sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in tcp_addr{};
    tcp_addr.sin_family = AF_INET;
    tcp_addr.sin_port = htons(TCP_PORT);
    inet_pton(AF_INET, SERVER_IP, &tcp_addr.sin_addr);

    if (connect(tcp_sock, (struct sockaddr*) &tcp_addr, sizeof(tcp_addr)) < 0) {
        perror("TCP connection failed");
        exit(EXIT_FAILURE);
    }

    SSL* ssl = SSL_new(ctx);
    SSL_set_fd(ssl, tcp_sock);

    if (SSL_connect(ssl) <= 0) {
        ERR_print_errors_fp(stderr);
    } else {
        std::cout << "TLS connection established!" << std::endl;

        int udp_sock = socket(AF_INET, SOCK_DGRAM, 0);
        struct sockaddr_in udp_addr{};
        udp_addr.sin_family = AF_INET;
        udp_addr.sin_port = htons(UDP_PORT);
        inet_pton(AF_INET, SERVER_IP, &udp_addr.sin_addr);

        // start chat + manual UDP trigger
        handle_tls_chat(ssl, udp_sock, udp_addr);

        close(udp_sock);
    }

    SSL_free(ssl);
    close(tcp_sock);
    SSL_CTX_free(ctx);
    cleanup_openssl();

    return 0;
}
