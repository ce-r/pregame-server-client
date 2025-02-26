
#include "TTU.hpp"
#include <string>


gnetwork::TTU::TTU(std::string& filen_crt, std::string& filen_key) 
                : server_crt(filen_crt), server_key(filen_key), ctx(nullptr){
    init_openssl();
    ctx = create_context(true);  // Assuming server by default
    configure_context(ctx);
};

gnetwork::TTU::~TTU() {
    if (ctx) {
        SSL_CTX_free(ctx);
    }
    cleanup_openssl();
};

void gnetwork::TTU::init_openssl() {
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
}

void gnetwork::TTU::cleanup_openssl() {
    EVP_cleanup();
}

SSL_CTX* gnetwork::TTU::create_context(bool is_server) { // server and client 
    const SSL_METHOD* method = is_server ? TLS_server_method() : TLS_client_method();
    SSL_CTX* ctx = SSL_CTX_new(method);
    if (!ctx) {
        ERR_print_errors_fp(stderr);
        throw std::runtime_error("Unable to create SSL context");
    }
    return ctx;
}

void gnetwork::TTU::configure_context(SSL_CTX* ctx) {
    if (SSL_CTX_use_certificate_file(ctx, server_crt.c_str(), SSL_FILETYPE_PEM) <= 0 ||
        SSL_CTX_use_PrivateKey_file(ctx, server_key.c_str(), SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        throw std::runtime_error("Failed to configure SSL context");
    }
}