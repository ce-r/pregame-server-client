

// server.cpp

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

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

SSL_CTX* create_server_context() {
    const SSL_METHOD* method = TLS_server_method();
    SSL_CTX* ctx = SSL_CTX_new(method);
    if (!ctx) {
        perror("Unable to create SSL context");
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
    return ctx;
}

void configure_context(SSL_CTX* ctx) {
    if (SSL_CTX_use_certificate_file(ctx, "server.crt", SSL_FILETYPE_PEM) <= 0 ||
        SSL_CTX_use_PrivateKey_file(ctx, "server.key", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
}

void handle_tcp_tls(SSL* ssl) {
    char buffer[BUFFER_SIZE] = {0};
    int bytes = SSL_read(ssl, buffer, sizeof(buffer));
    if (bytes > 0) {
        std::cout << "TCP/TLS Received: " << buffer << std::endl;
        SSL_write(ssl, "Message received over TLS", strlen("Message received over TLS"));
    } else {
        int error = SSL_get_error(ssl, bytes);
        if (error == SSL_ERROR_ZERO_RETURN) {
            std::cout << "TLS connection closed cleanly by peer." << std::endl;
        } else {
            ERR_print_errors_fp(stderr);
        }
    }
}

void udp_streaming(int udp_sock) {
    char buffer[BUFFER_SIZE];
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);

    while (true) {
        int bytes = recvfrom(udp_sock, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&client_addr, &addr_len);
        if (bytes > 0) {
            buffer[bytes] = '\0';
            std::cout << "UDP Stream Received: " << buffer << std::endl;
        }
    }
}

int main() {
    init_openssl();
    SSL_CTX* ctx = create_server_context();
    configure_context(ctx);

    // TCP Socket
    int tcp_sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in tcp_addr{};
    tcp_addr.sin_family = AF_INET;
    tcp_addr.sin_port = htons(TCP_PORT);
    tcp_addr.sin_addr.s_addr = INADDR_ANY;

    bind(tcp_sock, (struct sockaddr*)&tcp_addr, sizeof(tcp_addr));
    listen(tcp_sock, 1);

    std::cout << "TCP/TLS Server listening on port " << TCP_PORT << std::endl;

    // UDP Socket
    int udp_sock = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in udp_addr{};
    udp_addr.sin_family = AF_INET;
    udp_addr.sin_port = htons(UDP_PORT);
    udp_addr.sin_addr.s_addr = INADDR_ANY;

    bind(udp_sock, (struct sockaddr*)&udp_addr, sizeof(udp_addr));
    std::cout << "UDP Server listening on port " << UDP_PORT << std::endl;

    // Accept TCP/TLS Connection
    struct sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);
    int client_sock = accept(tcp_sock, (struct sockaddr*)&client_addr, &len);
    SSL* ssl = SSL_new(ctx);
    SSL_set_fd(ssl, client_sock);

    if (SSL_accept(ssl) <= 0) {
        ERR_print_errors_fp(stderr);
    } else {
        handle_tcp_tls(ssl);
    }

    // Start UDP Streaming in the main thread
    udp_streaming(udp_sock);

    // Cleanup
    SSL_free(ssl);
    close(client_sock);
    close(tcp_sock);
    close(udp_sock);
    SSL_CTX_free(ctx);
    cleanup_openssl();

    return 0;
}

