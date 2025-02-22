

// client.cpp

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

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

void handle_tcp_tls(SSL* ssl) {
    const char* msg = "Hello from TLS client!";
    SSL_write(ssl, msg, strlen(msg));

    char buffer[BUFFER_SIZE] = {0};
    int bytes = SSL_read(ssl, buffer, sizeof(buffer));
    if (bytes > 0) {
        std::cout << "TCP/TLS Response: " << buffer << std::endl;
    } else {
        ERR_print_errors_fp(stderr);
    }
}

void udp_streaming(int udp_sock, struct sockaddr_in udp_addr) {
    const char* stream_msg = "Streaming via UDP!";
    for (int i = 0; i < 10; ++i) {
        sendto(udp_sock, stream_msg, strlen(stream_msg), 0, (struct sockaddr*)&udp_addr, sizeof(udp_addr));
        std::cout << "Sent UDP Stream: " << stream_msg << std::endl;
        sleep(1);
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

    connect(tcp_sock, (struct sockaddr*)&tcp_addr, sizeof(tcp_addr));

    SSL* ssl = SSL_new(ctx);
    SSL_set_fd(ssl, tcp_sock);

    if (SSL_connect(ssl) <= 0) {
        ERR_print_errors_fp(stderr);
    } else {
        handle_tcp_tls(ssl);
    }

    // UDP streaming
    int udp_sock = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in udp_addr{};
    udp_addr.sin_family = AF_INET;
    udp_addr.sin_port = htons(UDP_PORT);
    inet_pton(AF_INET, SERVER_IP, &udp_addr.sin_addr);

    udp_streaming(udp_sock, udp_addr);

    SSL_free(ssl);
    close(tcp_sock);
    close(udp_sock);
    SSL_CTX_free(ctx);
    cleanup_openssl();

    return 0;
}
